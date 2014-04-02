#include "dynamic_graph_bridge/ros_interpreter.hh"

namespace dynamicgraph
{
  static const int queueSize = 5;

  Interpreter::Interpreter (ros::NodeHandle& nodeHandle)
    : interpreter_ (),
      nodeHandle_ (nodeHandle),
      runCommandSrv_ (),
      runPythonFileSrv_ (),
      mutex_(new boost::interprocess::interprocess_mutex() )
  {
  }

  void Interpreter::startRosService ()
  {
    runCommandCallback_t runCommandCb =
      boost::bind (&Interpreter::runCommandCallback, this, _1, _2);
    runCommandSrv_ =
      nodeHandle_.advertiseService ("run_command", runCommandCb);

    runPythonFileCallback_t runPythonFileCb =
      boost::bind (&Interpreter::runPythonFileCallback, this, _1, _2);
    runPythonFileSrv_ =
      nodeHandle_.advertiseService ("run_script", runPythonFileCb);
  }

  bool
  Interpreter::runCommandCallback
  (dynamic_graph_bridge_msgs::RunCommand::Request& req,
   dynamic_graph_bridge_msgs::RunCommand::Response& res)
  {
    while(! mutex_->try_lock() ){}
    interpreter_.python(req.input, res.result, res.stdout, res.stderr);
    mutex_->unlock();

    return true;
  }

  bool
  Interpreter::runPythonFileCallback (dynamic_graph_bridge_msgs::RunPythonFile::Request& req,
                                      dynamic_graph_bridge_msgs::RunPythonFile::Response& res)
  {
    while(! mutex_->try_lock() ){}
    interpreter_.runPythonFile(req.input, res.result);
    mutex_->unlock();
    return true;
  }

  void Interpreter::runCommand
  (const std::string & command, 
   std::string &result,
   std::string &out, 
   std::string &err)
  {
    while(! mutex_->try_lock() ){}
    interpreter_.python(command, result, out, err);
    mutex_->unlock();
  }

  void Interpreter::runPythonFile( std::string ifilename,
                                   std::string &err){
      while(! mutex_->try_lock() ){}
      interpreter_.runPythonFile(ifilename, err);
      mutex_->unlock();
  }

} // end of namespace dynamicgraph.


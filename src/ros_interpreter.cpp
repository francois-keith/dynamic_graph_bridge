#include "dynamic_graph_bridge/ros_interpreter.hh"

namespace dynamicgraph
{
  static const int queueSize = 5;

  Interpreter::Interpreter (ros::NodeHandle& nodeHandle)
    : interpreter_ (),
      nodeHandle_ (nodeHandle),
      runCommandSrv_ (),
      runPythonFileSrv_ ()
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
    interpreter_.python(req.input, res.result, res.stdout, res.stderr);
    return true;
  }

  bool
  Interpreter::runPythonFileCallback (dynamic_graph_bridge_msgs::RunPythonFile::Request& req,
                                      dynamic_graph_bridge_msgs::RunPythonFile::Response& res)
  {
    interpreter_.runPythonFile(req.input, res.result);
    return true;
  }

  void Interpreter::runCommand
  (const std::string & command, 
   std::string &result,
   std::string &out, 
   std::string &err)
  {
    interpreter_.python(command, result, out, err);
  }

  void Interpreter::runPythonFile( std::string ifilename,
                                   std::string &err){
      interpreter_.runPythonFile(ifilename, err);
  }

} // end of namespace dynamicgraph.


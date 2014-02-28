#ifndef DYNAMIC_GRAPH_BRIDGE_ROBOT_MODEL_HH
# define DYNAMIC_GRAPH_BRIDGE_ROBOT_MODEL_HH

# include <string>
# include <set>

#include <sot-dynamic/dynamic.h>
#include <dynamic-graph/linear-algebra.h>
#include "XmlRpcValue.h"

namespace dynamicgraph
{
  class RosRobotModel;

  /// \brief This entity load either the current model available in
  /// the robot_description parameter or a specified file and build
  /// a Dynamic entity
  ///
  /// This relies on jrl_dynamics_urdf to load the model and jrl-dynamics
  /// to realize the computation.
  class RosRobotModel : public sot::Dynamic
  {
    DYNAMIC_GRAPH_ENTITY_DECL();
  public:
    RosRobotModel(const std::string& n);
    virtual ~RosRobotModel();

    void loadUrdf(const std::string& filename);
    void setNamespace (const std::string& ns);
    void loadFromParameterServer();
    Vector curConf() const;

    void addJointMapping(const std::string& link, const std::string& repName);

    void ignoreJoint(const std::string &link);

  protected:

    unsigned getDimension () const
    {
      if (!m_HDR)
	throw std::runtime_error ("no robot loaded");
      return m_HDR->numberDof();
    }


  private:
    /// \brief Name of the parameter where the joints list will be published
    std::string jointsParameterName_;

    /// \brief Name of the controller namespace
    std::string ns_;

    /// \brief Special joints map for the parser
    std::map<std::string, std::string> specialJoints_;

    /// \brief Ignored joints for the parser: they will be converted into fixed joint.
    std::set<std::string> ignoredJoints_;
  };
} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_BRIDGE_ROBOT_MODEL_HH

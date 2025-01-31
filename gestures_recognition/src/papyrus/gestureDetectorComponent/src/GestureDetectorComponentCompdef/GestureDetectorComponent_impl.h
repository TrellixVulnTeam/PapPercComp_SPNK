// --------------------------------------------------------
// Code generated by Papyrus C++
// --------------------------------------------------------

#ifndef GESTUREDETECTORCOMPONENTCOMPDEF_GESTUREDETECTORCOMPONENT_IMPL_H
#define GESTUREDETECTORCOMPONENTCOMPDEF_GESTUREDETECTORCOMPONENT_IMPL_H

/************************************************************
 GestureDetectorComponent_impl class header
 ************************************************************/

#include "GestureDetectorComponentCompdef/Pkg_GestureDetectorComponentCompdef.h"

#include "GestureDetectorComponentCompdef/GestureDetectorComponent.h"
#include "geometry_msgs/msg/pose_array.hpp"

namespace ros2Library {
namespace rclcpp {
class NodeOptions;
}
}

namespace GestureDetectorComponentCompdef {

/************************************************************/
/**
 * This is a skeleton class generated for component GestureDetectorComponent
 * Copy it into the source folder as an initial base (or copy parts
 * of it whenever you add modify the component).
 * 
 */
class GestureDetectorComponent_impl: public GestureDetectorComponent {
public:

	/**
	 * 
	 * @param options 
	 */
	GestureDetectorComponent_impl(rclcpp::NodeOptions /*in*/options);

	/**
	 * 
	 * @param posearray 
	 */
	void pointsHandler(
			const geometry_msgs::msg::PoseArray::SharedPtr /*in*/posearray);

};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace GestureDetectorComponentCompdef

/************************************************************
 End of GestureDetectorComponent_impl class header
 ************************************************************/

#endif

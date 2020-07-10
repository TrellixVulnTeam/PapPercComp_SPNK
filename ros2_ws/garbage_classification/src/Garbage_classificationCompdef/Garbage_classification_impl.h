// --------------------------------------------------------
// Code generated by Papyrus C++
// --------------------------------------------------------

#ifndef GARBAGE_CLASSIFICATIONCOMPDEF_GARBAGE_CLASSIFICATION_IMPL_H
#define GARBAGE_CLASSIFICATIONCOMPDEF_GARBAGE_CLASSIFICATION_IMPL_H

/************************************************************
 Garbage_classification_impl class header
 ************************************************************/

#include "Garbage_classificationCompdef/Pkg_Garbage_classificationCompdef.h"

#include "Garbage_classificationCompdef/Garbage_classification.h"
#include "sensor_msgs/msg/image.hpp"


#include <torch/script.h>
#include <opencv2/opencv.hpp>

namespace ros2Library {
namespace rclcpp {
class NodeOptions;
}
}

namespace Garbage_classificationCompdef {

/************************************************************/
/**
 * This is a skeleton class generated for component Garbage_classification
 * Copy it into the source folder as an initial base (or copy parts
 * of it whenever you add modify the component).
 * 
 */
class Garbage_classification_impl: public Garbage_classification {
public:

	/**
	 * 
	 * @param options 
	 */
	Garbage_classification_impl(rclcpp::NodeOptions /*in*/options);

	/**
	 * 
	 * @param image 
	 */
	void classify(const sensor_msgs::msg::Image::SharedPtr /*in*/image);
private:
	sensor_msgs::msg::Image img_msg_;
	std_msgs::msg::String classification_;
	torch::jit::Module classifier_;
	int camNum_;
};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

} // of namespace Garbage_classificationCompdef

/************************************************************
 End of Garbage_classification_impl class header
 ************************************************************/

#endif
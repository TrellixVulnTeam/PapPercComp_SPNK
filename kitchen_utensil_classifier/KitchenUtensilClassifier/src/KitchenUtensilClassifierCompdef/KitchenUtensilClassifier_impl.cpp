// --------------------------------------------------------
// Code generated by Papyrus C++
// --------------------------------------------------------

#define KitchenUtensilClassifierCompdef_KitchenUtensilClassifier_impl_BODY

#define MODEL_PATH "data/kitchen_classifier.onnx"
#define CLASS_LIST_PATH "data/class_list.txt"
#define NR_SKIP_FRAMES 30
#define IMG_SCALE_FACTOR 1.0

/************************************************************
 KitchenUtensilClassifier_impl class body
 ************************************************************/

// include associated header file
#include "KitchenUtensilClassifierCompdef/KitchenUtensilClassifier_impl.h"

// Derived includes directives
#include "rclcpp/rclcpp.hpp"
#include "stdio.h"
#include <sstream>
#include <string>
#include <fstream>

#include <unistd.h>

using namespace cv;

namespace KitchenUtensilClassifierCompdef {

// static attributes (if any)

static void softmax(Mat *outputs, double max_val);


/**
 * 
 * @param options 
 */

KitchenUtensilClassifier_impl::KitchenUtensilClassifier_impl(
		rclcpp::NodeOptions /*in*/options) :
		KitchenUtensilClassifier(options) {
	net = dnn::readNet(MODEL_PATH);
	counter = NR_SKIP_FRAMES;
	classes = {};
	read_classes_from_file(CLASS_LIST_PATH);
}

/**
 * This function is called whenever an image is published. Every NR_SKIP_FRAMES'th
 * frame will resul in a class name being published for a specific piece of cutlery
 * (though it is worth noting that, depending on the model being used and the
 * configuration of the class-name list file, any type of object can be potentially
 * classified).
 * 
 * @param image An input image
 */
void KitchenUtensilClassifier_impl::classifyKitchenUtensil(
		const sensor_msgs::msg::Image::SharedPtr /*in*/image) {
	Mat blob, prob;
	Point classIdPoint;
	int classId;
	double confidence;
	std_msgs::msg::String className = std_msgs::msg::String();

	// simply skip some frames so that we don't get clogged up on too many input images
	if (counter++ < NR_SKIP_FRAMES)
		return;
	counter = 0;


	Mat frameIn(image->height, image->width, CV_8UC3, image->data.data());
	Mat frame;

	frameIn.convertTo(frame, CV_32F);
	frame = frame / 256;
	frame = frame / 0.13;

	blob = dnn::blobFromImage(frame, IMG_SCALE_FACTOR, Size{inputWidth, inputHeight}, mean, false, CV_32F);
	net.setInput(blob);
	prob = net.forward();
	debug_net_efficiency();

	minMaxLoc(prob, 0, &confidence, 0, &classIdPoint);
	softmax(&prob, confidence);
    debug_confidences(&prob);

	classId = classIdPoint.x;
	if (0 <= classId && (unsigned)classId < classes.size()) {
		className.set__data(classes[classId]);
	} else {
		RCLCPP_WARN(this->get_logger(), "Name list is shorter than output tensor;"
				"this is most likely due to using an incorrect model or an invalid"
				"class list.");
		className.set__data("Unknown");
	}

	RCLCPP_INFO(this->get_logger(), format("%s: %.4f",
			(format("Class %s (#%d)", className.data.c_str(), classId).c_str()), confidence));

	utensilClass_pub_->publish(className);
}

/*
 * Outputs the confidence values for the individual classes, based on a matrix
 * containing confidences (ideally between 0 and 1).
 */
void KitchenUtensilClassifier_impl::debug_confidences(Mat *confidences){
	// print out probability for each class
	MatIterator_<float> it = confidences->begin<float>();
	MatIterator_<float> end = confidences->end<float>();
	Point p;
	String fancy_output;
	unsigned nr_of_chars;
	unsigned class_counter = 0;

    RCLCPP_INFO(this->get_logger(), format("Confidences:"));
	for(;it != end; ++it, ++class_counter) {
		const char *class_name;
		if (class_counter < classes.size()) {
			class_name = classes[class_counter];
		} else {
			RCLCPP_WARN(this->get_logger(), "Name list is shorter than output tensor;"
					"this is most likely due to using an incorrect model or an invalid"
					"class list.");
			class_name = "UNKNOWN";
		}
		p = it.pos();
		nr_of_chars = (unsigned)(50 * confidences->at<float>(p));
		fancy_output = "";

		for (unsigned i = 0; i < nr_of_chars; ++i){
			fancy_output.append("|");
		}
		for (unsigned i = 0; i < 50 - nr_of_chars; ++i){
			fancy_output.append(" ");
		}

		RCLCPP_INFO(this->get_logger(), format("  %s %s",
				fancy_output.c_str(), class_name));
	}
}

/*
 * Simple function for reading a list of classes from a file; each line
 * of the file should have exactly one class name in it (empty lines are ignored).
 */
void KitchenUtensilClassifier_impl::read_classes_from_file(const char *path){
	std::ifstream infile(path);
	std::string line;
	while (std::getline(infile, line))
	{
		if (line == "") {
			continue;
	    }
	    classes.push_back(strdup(line.c_str()));
	}
}

/*
 * Outputs the most recent inference time of the net
 */
void KitchenUtensilClassifier_impl::debug_net_efficiency(){
	// Efficiency information.
	std::vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double t = net.getPerfProfile(layersTimes) / freq;

	RCLCPP_INFO(this->get_logger(), format("Inference time: %.2f ms", t));
}

/*
 * Applies the softmax function to a series of outputs,
 * editing the values of the matrix in-place.
 */
static void softmax(Mat *outputs, double max_val) {
	*outputs = *outputs - max_val;
	exp(*outputs, *outputs);
	double s = sum(*outputs)[0];
	*outputs = *outputs / s;
}

} // of namespace KitchenUtensilClassifierCompdef

/************************************************************
 End of KitchenUtensilClassifier_impl class body
 ************************************************************/

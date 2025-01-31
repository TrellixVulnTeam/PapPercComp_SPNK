# Fruit and Disease Detection as ROS2 component
**Author** [Philipp Seitz](https://github.com/Gistbatch)

This package contains a component created with Papyrus for robotics.
![component](https://github.com/robmosys-tum/PapPercComp/blob/master/fruit_detection_component/images/fruit_detection.compdef.png)

## Overview

The goal of this project was to provide object detection for fruits and identfiy diseases in them.
It uses a two step approach:
* Detect fruit objects in an image
* Classifiy diseases for the detected fruits

The steps are realized with neural networks implemented in TensorFlow.
For detection a faster_rcnn is used. For classification a SqueezeNet is used.

## ROS2
This package was created for ROS2-Eloquent. It contains a launch file [fruit_detection_launch](https://github.com/robmosys-tum/PapPercComp/blob/master/fruit_detection_component/launch/fruit_detection_launch.py).
The node interaction is described here ![interaction](https://github.com/robmosys-tum/PapPercComp/blob/master/fruit_detection_component/images/overview.png).

### Nodes
The following nodes are declared:
* FruitDetection is the main component. The node processes images in the
/image topic and publishes the results to /BoxImage
* detection_service.py (service) is a service wich detects fruits using a 
faster_rcnn. *Parameters: module_handle* URL for the model.
* disease_service.py (service) is a service wich classifies fruit diseases using a 
neural network. *Parameters: model_file* filepath for the classification network.

### Messages
The package declares two new message types:
* ClassBox
```c
string fruit
int32 fruit_score
float32 xmin
float32 xmax
float32 ymin
float32 ymax
string disease "undefined"
int32 disease_score 0
```
The bounding box is given in relative values cwith reference to the image.
You can get the absolute pixel value by multiplying it with the hight/width respectively.
* BoxesImage
```c
sensor_msgs/Image img
ClassBox[] boxes
```

## Install
This package is not published oficially and has to be built from source.
The repositorie is comprised of multiple individual packages.
To get it working do:
```bash
git clone https://github.com/robmosys-tum/PapPercComp.git
mv PapPercComp/fruit_detection_component/ <ros2_ws>/src/fruit_detection
```
To do this clone the package and build it as an regular ROS2 package using *colcon*.
Since this packages contains python and c++ nodes some additional steps are required.
ROS2 supports *python3*. Make sure you have an correct executable at *usr/bin/python3*
and linked correctly `alias python="python3"`
Additionally virtual environments are not supported which requires you to make sure *pip3*
is set e.g. `alias pip="pip3"`.
Some dependencies need to be installed manually:
```bash
python3 -m pip install pip --upgrade
pip3 install tensorflow tensorflow-hub numpy opencv-python
```
The neural networks are built with TensorFlow2. It is possible to use a dedicated
GPU to speed up the prediction process in both services. The necessary steps are
documented [here](https://www.tensorflow.org/install/gpu).

## Usage

The package is straight forward to use. Any image published will be picked up and
processed. The nodes can also be launched individually. You can customize the neural
networks by doing the following:
* Object Detection: To change the object detection you can use the *module_handle* parameter.
* Disease Classification: To change the classification network you can use the *model_file* parameter.

Simple example:

1. Change to your ros2 workspace: `cd <ros2_ws>`
2. Build the project: `colcon build --packages-select fruit_detection`
3. Source the setup: `source install/setup.bash`
4. Launch the nodes: `ros2 launch fruit_detection fruit_detection_launch.py`
5. Unzip the example: `unzip src/fruit_detection/example/example.zip -d src/fruit_detection/example`
6. Play the example bag: `ros2 bag play src/fruit_detection/example/example_0.db3`

Keep in mind that logging doesn't work properly when using `ros2 launch`.
If you use the the nodes individually make sure to set their parameters!
Sometimes playing the bag doesn't get registered in the *FruitDetection* node.
In this case just replay the bag.

## Develop

The development of the FruitDetection node was done with Papyrus. It is
recommended to use it for further development.
The scripts are writen in plain python3. If you add additional dependencies make
sure to add them to the installation part of this readme since ROS2 does not 
specify a way to add them to the package directly. If you add additional folders
make sure you add them in the CMakeLists.txt.
The code was linted with pylint and cpplint respectively. 

### Available Models

Currently two neural networks are provided. To change the network update the launch
file and parameters accordingly:
1. squeezenet.h5
    - Binary classifier `python ['healthy', 'rotten]`
    - Trained on a public [Kaggle](https://www.kaggle.com/sriramr/fruits-fresh-and-rotten-for-classification) dataset
2. squeezenet_v1.h5
    - Six class classifier `python ['blotch', 'canker', 'healthy', 'mold', 'rot', 'scab']`
    - Trained on a private dataset (no licensing) of 2106 images. The images were collected
    from google and contain fruits according to the OpenImages_v6 classes.

### Customizing models

If you want to customize the neural networks you can use the provided squeezenet
for classification. The file can be run as python3 script. Just define the desired
classes and create a dataset in the form off:
```
train
'---class1
'---class2
...
```

The detection network is a pretrained network from tensorflow hub.
Tensorflow provides promising detection Networks (especially ShapeMask) which
in theory perform far better especially if you train them on OpenImages.
Unfortunately the [tensorflow model garden](https://github.com/tensorflow/models)
is still under development and prediction was not working on my machine.
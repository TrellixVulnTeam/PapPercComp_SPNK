cmake_minimum_required(VERSION 3.5.0)
project(gesturedetectorcomponent)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(
	# assure that generated .h files are found
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src-gen
)

add_executable(GestureDetectorComponent src/GestureDetectorComponentCompdef/GestureDetectorComponent_impl.cpp)
ament_target_dependencies(GestureDetectorComponent rclcpp rclcpp_lifecycle geometry_msgs std_msgs)


# Start of user code dependencies
# End of user code

install(TARGETS
	GestureDetectorComponent
	DESTINATION lib/${PROJECT_NAME}
)
ament_package()

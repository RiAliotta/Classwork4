#include "ros/ros.h"
#include "cw4/frame_tf.h"
#include <tf/transform_listener.h>
#include "geometry_msgs/Pose.h"
#include "std_msgs/String.h"

using namespace std;

//Callback function
//	Return value: boolean
//		If this function returns true, the service function has been corretly called
//		You can use this value to check if the function has been called with correct parameters
//		i.e. call a service calculating the square of a number, calling the service with a negative number
//	Input values:  the request part of the servive 
//				   the output of the service to fill
bool service_callback( cw4::frame_tf::Request &req, cw4::frame_tf::Response &res) {


	cout << "Starting frame: " << req.frame_a << "\tEnd frame: " << req.frame_b;

    try {
        listener.lookupTransform(req.frame_a, req.frame_b, ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(0.1).sleep();
        return false;
    }

    res.Point.x = transform.getOrigin().x();
    res.Point.y = transform.getOrigin().y();
    res.Point.z = transform.getOrigin().z();
    res.Quaternion.rpy(transform.getRotation().x(), transform.getRotation().y(), transform.getRotation().z());

	return true;
}


int main(int argc, char **argv) {

	ros::init(argc, argv, "frame_service");
	ros::NodeHandle n;   

    tf::TransformListener listener;
    tf::StampedTransform transform;
    ros::Rate rate(100);

	//Initialize the service object: name of the service and callback function
	//	Like subscribers, also the callback function can be declared as a class function
	ros::ServiceServer service = n.advertiseService("frame_tf", service_callback);


	//Call the spin function to maintain the node alive
	ros::spin();

	return 0;
}
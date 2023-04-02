#include "ros/ros.h"
#include "cw4/frame_tf.h"
#include "geometry_msgs/Pose.h"
#include "std_msgs/String.h"

using namespace std;

int main(int argc, char **argv) {

	//Init the ROS node with service_client name
	ros::init(argc, argv, "dynamic_tf");
	ros::NodeHandle n;

	//Init the service client. Data to use for the service (the .srv file) and the name of the service
	ros::ServiceClient client = n.serviceClient<cw4::frame_tf>("frame_service");
	
	//Define and initialize the service data structure 
	//	This datastructure brings with it the input value (in the request fields) and the output values, in the response field
	cw4::frame_tf srv;
	
	//Wait that in the ROS network, the service sum is advertised
	//	If you call a service and the service has not been advertised, you will have back an error
	ROS_INFO("Waiting for the client server");
	client.waitForExistence();
	ROS_INFO("Client server up now");

	srv.request.frame_a.data = "/base_link";
	srv.request.frame_b.data = "/camera_link";
	
	//Call the service callback
	//	The return value is false if:
	//		- the callback returns false
	//		- the service has not been found in the ROS network
	if (!client.call(srv)) {
		ROS_ERROR("Error calling the service");
		return 1;
	}

	//Just print the output
	cout << "Service output: " << "All right!" << endl;
	
	return 0;
}
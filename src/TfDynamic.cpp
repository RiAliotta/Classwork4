#include "ros/ros.h"
#include "cw4/frame_tf.h"
#include "geometry_msgs/Pose.h"
#include <tf/transform_broadcaster.h>
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
	
	tf::TransformBroadcaster br;
    tf::Transform transform;

	ros::Rate r(100);

	ROS_INFO("Waiting for the client server");
	client.waitForExistence();
	ROS_INFO("Client server up now");


	cout << "Insert the reference frame: ";
	cin >> srv.request.frame_a.data;
	cout << "Insert the target frame: ";
	cin >> srv.request.frame_b.data;

    while (ros::ok()) {

		if (!client.call(srv)) {
			ROS_ERROR("Error calling the service");
			return 1;
		}

        transform.setOrigin(tf::Vector3(srv.response.pose.position.x, srv.response.pose.position.y, srv.response.pose.position.z));
        tf::Quaternion q(srv.response.pose.orientation.x, srv.response.pose.orientation.y, srv.response.pose.orientation.z, srv.response.pose.orientation.w);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), srv.request.frame_a.data, srv.request.frame_b.data));

        r.sleep();
    }

	return 0;
}
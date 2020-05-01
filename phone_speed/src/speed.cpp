#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "nav_msgs/Odometry.h"




using namespace ros;

bool F=1;

double vecOld[3]={0,0,0};
double timeOld=0;
double vel;

Publisher imuPub;

void poseCallback(const nav_msgs::Odometry::ConstPtr& data)
{
	ROS_INFO("%f", data->pose.pose.position.x);
	return;
}

void imuRedirect(const sensor_msgs::Imu::ConstPtr& data)
{
	sensor_msgs::Imu corrected = *data;
	corrected.header.frame_id = "base_footprint";
	imuPub.publish(corrected);
}

int main(int argc, char **argv)
{
    init(argc, argv, "speed");

	NodeHandle node;
	imuPub = node.advertise<sensor_msgs::Imu>("imu_data", 10);
	
	Subscriber imuSub = node.subscribe("/android/imu", 10, &imuRedirect);
		
    Subscriber poseSub = node.subscribe("/odometry/filtered", 10, &poseCallback);

	

    Rate rate(10);
    ROS_INFO("Started");
    while (ok()) {
		spinOnce();
        rate.sleep();
    	
    }
}
/*
#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "_msgs/NavSatFix.h"




using namespace ros;

bool F=1;

double vecOld[3]={0,0,0};
double timeOld=0;
double vel;

Publisher imuPub, satPub, magPub;

void imuCallback(const sensor_msgs::Imu::ConstPtr& data)
{
	imuPub.publish(data);
}

int main(int argc, char **argv)
{
    init(argc, argv, "speed");

	NodeHandle node;
	
	imuPub = node.advertise<sensor_msgs::Imu>("/imu/data", 10);
	//satPub = node.advertise<sensor_msgs::NavSatFix>("/sat/data", 10);
	//magPub = node.advertise<sensor_msgs::MagneticField>("/imu/data", 10);
	
    Subscriber poseSub = node.subscribe("", 10, &imuCallback);
	//Subscriber satSub = node.subscribe("android/fix", 10, &satCallback);
	//Subscriber imuSub = node.subscribe("android/imu", 10, &phoneCallback);

	

    Rate rate(10);
    ROS_INFO("Started");
    while (ok()) {
		spinOnce();
        rate.sleep();
    	
    }
}
*/

#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "nav_msgs/Odometry.h"




using namespace ros;

bool F=1;

double vecOld[3]={0,0,0};
double timeOld=0;
geometry_msgs::Vector3 Vel;
double vel;

Publisher imuPub;
int LtimeS = -1, LtimeN=0;

void poseCallback(const nav_msgs::Odometry::ConstPtr& data)
{
	ROS_INFO("%f", data->pose.pose.position.x);
	return;
}

void imuRedirect(const sensor_msgs::Imu::ConstPtr& data)
{
	int t;
	geometry_msgs::Quaternion q = data->orientation;
	geometry_msgs::Quaternion tmp;
	geometry_msgs::Vector3 acc = data->linear_acceleration;
	geometry_msgs::Quaternion grav;
	grav.w=0;grav.x=0;grav.y=0;grav.z=10;



	q.x=-q.x;q.y=-q.y;q.z=-q.z;

    tmp.w = q.w * grav.w - q.x * grav.x - q.y * grav.y - q.z * grav.z;
    tmp.x = q.w * grav.x + q.x * grav.w + q.y * grav.z - q.z * grav.y;
    tmp.y = q.w * grav.y - q.x * grav.z + q.y * grav.w + q.z * grav.x;
    tmp.z = q.w * grav.z + q.x * grav.y - q.y * grav.x + q.z * grav.w;

	q.x=-q.x;q.y=-q.y;q.z=-q.z;

    grav.w = tmp.w * q.w - tmp.x * q.x - tmp.y * q.y - tmp.z * q.z;
    grav.x = tmp.w * q.x + tmp.x * q.w + tmp.y * q.z - tmp.z * q.y;
    grav.y = tmp.w * q.y - tmp.x * q.z + tmp.y * q.w + tmp.z * q.x;
    grav.z = tmp.w * q.z + tmp.x * q.y - tmp.y * q.x + tmp.z * q.w;

    acc.x-=grav.x;acc.y-=grav.y;acc.z-=grav.z;



    if(LtimeS!=-1)
    {
    	t=(data->header.stamp.sec-LtimeS)*1000000000+data->header.stamp.nsec-LtimeN;
    	Vel.x+=acc.x*t/1000000000.0;
    	Vel.y+=acc.y*t/1000000000.0;
    	Vel.z+=acc.z*t/1000000000.0;
    }
    LtimeS=data->header.stamp.sec;
    LtimeN=data->header.stamp.nsec;

    vel=sqrt(Vel.x*Vel.x+Vel.y*Vel.y+Vel.z*Vel.z);



    ROS_INFO_STREAM("grav: X: "<<grav.x<<"y:"<<grav.y<<"z:"<<grav.z);
    ROS_INFO_STREAM("acc: X: "<<acc.x<<"y:"<<acc.y<<"z:"<<acc.z);
    ROS_INFO_STREAM("Vel: X: "<<Vel.x<<"y:"<<Vel.y<<"z:"<<Vel.z <<" |" <<vel<<"|");


}

int main(int argc, char **argv)
{
    init(argc, argv, "speed");

	NodeHandle node;
//	imuPub = node.advertise<geometry_msgs::Vector3>("speed", 10);
	Vel.x=0;Vel.y=0;Vel.z=0;
	Subscriber imuSub = node.subscribe("/android/imu", 10, &imuRedirect);
		

	

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

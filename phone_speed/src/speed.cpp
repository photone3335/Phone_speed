#include "ros/ros.h"
#include "sensor_msgs/Imu.h"

using namespace ros;

bool F=1;

double vecOld[3]={0,0,0};
double timeOld=0;
double vel;

void phoneCallback(const sensor_msgs::Imu::ConstPtr& data)
{
	if(F)
	{	
		timeOld=data->header.stamp.sec*1.0+data->header.stamp.nsec/1000000000.0;
		vecOld[0]=data->orientation.x/data->orientation.w;
		vecOld[1]=data->orientation.y/data->orientation.w;
		vecOld[2]=data->orientation.z/data->orientation.w;
		F=0;
		return;
	}	
	double vec[3];
	double time=data->header.stamp.sec+data->header.stamp.nsec/1000000000.0;
	vec[0]=data->orientation.x/data->orientation.w;
	vec[1]=data->orientation.y/data->orientation.w;
	vec[2]=data->orientation.z/data->orientation.w;
	
	vel=(vec[0]-vecOld[0])*(vec[0]-vecOld[0])+(vec[1]-vecOld[1])*(vec[1]-vecOld[1])+(vec[2]-vecOld[2])*(vec[2]-vecOld[2])/(time-timeOld);
	ROS_INFO("vel :%0.5f, time:%f",vel,time-timeOld);
	vecOld[0]=vec[0];
	vecOld[1]=vec[1];
	vecOld[2]=vec[2];
	timeOld=time;
}

int main(int argc, char **argv)
{
    init(argc, argv, "speed");

	NodeHandle node;
	
    Subscriber imuSub = node.subscribe("android/imu", 10, &phoneCallback);

    Rate rate(10);
    ROS_INFO("Started");
    while (ok()) {
		spinOnce();
        rate.sleep();
    	
    }
}

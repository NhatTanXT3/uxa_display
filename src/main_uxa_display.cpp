#include <iostream>
#include <ros/ros.h>
using namespace std;
#include "uxa_display/main_uxa_display.h"

#include "uxa_display/dataSensorMsg.h"
#include "uxa_display/SAMJointPos12Msg.h"
#include "uxa_display/SAMJointStateMsg.h"
#include "sensor_msgs/JointState.h"
#include "uxa_display/displayCmdMsg.h"
#include "uxa_display/SAMJointPos12UpperMsg.h"



void sensor_callback(const uxa_display::dataSensorMsg::ConstPtr& msg){
    //    ROS_INFO("sensor data: %d",msg->zmp_P0 );
}

void sam_feedback_callback(const uxa_display::SAMJointStateMsg::ConstPtr& msg){
//        ROS_INFO("sam data Feedback Joint State: %d",msg->SAMPosDegree[0]);
    if(sys_flag.display_sam_feedback){
        sys_flag.sam_feedback_avail=1;
        for (unsigned char i=0; i<25;i++)
        {
            samPos12Avail[i]=msg->SAMPos12Avail[i];
            samDegree[i]=msg->SAMPosDegree[i];
        }
    }
}
void sam_control_callback(const uxa_display::SAMJointPos12Msg::ConstPtr& msg){
    //    ROS_INFO("sam data control joint: %d",msg->SAMPos12[0]);

    if(sys_flag.display_sam_control){
        sys_flag.sam_control_avail=1;
        for (unsigned char i=0; i<25;i++)
        {
            samMode[i]=msg->SAMMode[i];
            samPos12[i]=msg->SAMPos12[i];
        }
    }
}
void sam_control_upper_callback(const uxa_display::SAMJointPos12UpperMsg::ConstPtr& msg){
    if(sys_flag.display_sam_control_upper){
        sys_flag.sam_control_avail=1;
        for (unsigned char i=0; i<25;i++)
        {
            samMode[i]=msg->SAMMode[i];
            samPos12[i]=msg->SAMPos12[i];
        }
    }
}

#define DISPLAY_TURN_OFF_ 0
#define DISPLAY_SAM_CONTROL_ 1
#define DISPLAY_SAM_FEEDBACK_ 2
#define DISPLAY_SAM_UPPER_  3
void sub_function_cmd(const uxa_display::displayCmdMsg::ConstPtr& msg){
    switch(msg->command){
    case DISPLAY_SAM_CONTROL_:
        ROS_INFO("sam data control joint lower link");
        sys_flag.display_sam_control=1;
        sys_flag.display_sam_feedback=0;
        sys_flag.display_sam_control_upper=0;

        break;
    case DISPLAY_SAM_FEEDBACK_:
        ROS_INFO("sam data Feedback Joint State");
        sys_flag.display_sam_control=0;
        sys_flag.display_sam_feedback=1;
        sys_flag.display_sam_control_upper=0;

        break;
    case DISPLAY_SAM_UPPER_:
        ROS_INFO("sam data control joint upper link");
        sys_flag.display_sam_control=0;
        sys_flag.display_sam_feedback=0;
        sys_flag.display_sam_control_upper=1;

        break;
    default:
        sys_flag.display_sam_control=0;
        sys_flag.display_sam_feedback=0;
        sys_flag.display_sam_control_upper=0;
        break;
    }
}
int main(int argc, char **argv){
    ros::init(argc, argv, "uxa_display");
    ros::NodeHandle n;
    ros::Rate loop_rate(LOOP_RATE_1000Hz_);

    ros::Subscriber sensor_data_sub =  n.subscribe<uxa_display::dataSensorMsg>("sensor_pub",1000,sensor_callback);
    ros::Subscriber sam_pos12_sub=n.subscribe<uxa_display::SAMJointPos12Msg>("sam_pos12_sub",1000,sam_control_callback);
     ros::Subscriber sam_pos12_upper_sub=n.subscribe<uxa_display::SAMJointPos12UpperMsg>("sam_pos12_upper_sub",1000,sam_control_upper_callback);

    ros::Subscriber sam_joint_state_sub =  n.subscribe<uxa_display::SAMJointStateMsg>("sam_pub",1000,sam_feedback_callback);
    ros::Publisher rviz_jointState = n.advertise<sensor_msgs::JointState>("UXAJointState", 1000);

    ros::Subscriber display_sub=n.subscribe<uxa_display::displayCmdMsg>("display_cmd",200,sub_function_cmd);
    ROS_INFO("%s", "setup_uxa_display");

    sensor_msgs::JointState joint_state;
    joint_state.name.resize(24);
    joint_state.position.resize(24);
    joint_state.name[0] ="J0";
    joint_state.name[1] ="J1";
    joint_state.name[2] ="J0_2";
    joint_state.name[3] ="J1_3";
    joint_state.name[4] ="J2_4";
    joint_state.name[5] ="J3_5";
    joint_state.name[6] ="J4_6";
    joint_state.name[7] ="J5_7";
    joint_state.name[8] ="J6_8";
    joint_state.name[9] ="J7_9";
    joint_state.name[10] ="J8_10";
    joint_state.name[11] ="J9_11";
    joint_state.name[12] ="J12_14";
    joint_state.name[13] ="J13_15";
    joint_state.name[14] ="J14_16";
    joint_state.name[15] ="J15_17";
    joint_state.name[16] ="J16_18";
    joint_state.name[17] ="J17_19";
    joint_state.name[18] ="J18";
    joint_state.name[19] ="J19";
    joint_state.name[20] ="J22_23_13_12";
    joint_state.name[21] ="J23_24";
    joint_state.name[22] ="J24";

    while(ros::ok())
    {
        if(FlagTimer.Hz_50)
        {
            FlagTimer.Hz_50=0;
            //================


        }

        if(FlagTimer.Hz_25)
        {
            FlagTimer.Hz_25=0;
            //=========
            if(sys_flag.sam_control_avail)
            {
                sys_flag.sam_control_avail=0;
                for(unsigned char i=0;i<25;i++)
                {
                    if(samMode[i])
                    {
                        angle[i]=((double)samPos12[i]-(double)samPos12_hardware[i])*pos12bitTorad*angle_sign_rviz[i];
                    }
                    else
                    {
                        angle[i]=0;
                    }
                }
                for(unsigned char i=0;i<20;i++){
                    if(samMode[i])
                        joint_state.position[i]=angle[i];
                }
                if(samMode[22])
                    joint_state.position[20]=angle[22];
                if(samMode[23])
                    joint_state.position[21]=angle[23];
                if(samMode[24])
                    joint_state.position[22]=angle[24];
                joint_state.header.stamp = ros::Time::now();
                rviz_jointState.publish(joint_state);
            }
            else if(sys_flag.sam_feedback_avail){
                sys_flag.sam_feedback_avail=0;
                for(unsigned char i=0;i<25;i++)
                {
                    if(samPos12Avail[i])
                    {
                        angle[i]=samDegree[i]*M_PI/180*angle_sign_rviz[i];
                    }
                }
                for(unsigned char i=0;i<20;i++){
                    if(samPos12Avail[i])
                        joint_state.position[i]=angle[i];
                }
                if(samPos12Avail[22])
                    joint_state.position[20]=angle[22];
                if(samPos12Avail[23])
                    joint_state.position[21]=angle[23];
                if(samPos12Avail[24])
                    joint_state.position[22]=angle[24];

                joint_state.header.stamp = ros::Time::now();
                rviz_jointState.publish(joint_state);
            }

        }
        if(FlagTimer.Hz_100)
        {
            FlagTimer.Hz_100=0;
            //====================

        }
        if(FlagTimer.Hz_125)
        {
            FlagTimer.Hz_125=0;
            //===============


        }



        //==========================================


        ros::spinOnce();
        loop_rate.sleep();
        Timer_handler();
    }
    return 0;
}

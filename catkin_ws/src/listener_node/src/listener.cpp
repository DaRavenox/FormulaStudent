//This is the main file.
//Author:   Jonas Hongisto
//Date:     20.04.2018

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
//#include <arpa/inet.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

#define PORT 0
#define IP "127:0.0.xx"



int main(int argc, char **argv)
{

    //variables
    int listenfd = 0,connfd = 0,nt = 0, count = 0;
    struct sockaddr_in serv_addr;
    char recvBuff[1025];
    int numrv;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(recvBuff, '0', sizeof(recvBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(listenfd, 10) == -1){
        printf("Failed to listen\n");
        return -1;
    }


    ros::init(argc, argv, "talker");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("ethernetIN", 1000);
    ros::Rate loop_rate(10);


    //Main loop
    while (ros::ok()) {

        //accept connection
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
        //read into buffer
        nt = recv(connfd, &recvBuff, sizeof(recvBuff), 0);

        std_msgs::String msg;
        std::stringstream ss;
        ss << recvBuff << count;
        msg.data = ss.str();

        ROS_INFO("%s", msg.data.c_str());
        //publish the message gotten over the ethernet on the topic /ethernetIN
        chatter_pub.publish(msg);

        ros::spinOnce();
        loop_rate.sleep();
        ++count;

    }
    return 0;
}
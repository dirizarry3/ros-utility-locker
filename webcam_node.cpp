#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> //convert command line param

using namespace std;

int main(int argc, char** argv)
{
   //init ros from potential command line args and give it a name
   ros::init(argc, argv, "webcam_publisher");

   //get node handle so we can publish the camera data
   ros::NodeHandle nHandle;
   image_transport::ImageTransport it(nHandle);
   image_transport::Publisher pub = it.advertise("camera_image", 1);
   
   //define the video source and open up the cv_image stream
   //Note: for simplicity, I could just plug in a '0' which would infer
   //the laptops onboard cam, but this allows for peripheral cams as well
   std::istringstream video_sourceCmd(argv[1]);
   int video_source;
   if(!(video_sourceCmd >> video_source)) {
      return 1;
   }
   cv::VideoCapture cap(video_source);
   if(!cap.isOpened()) {
      return 1;
   }
   
   //declare and set variables before the ros spin
   cv::Mat frame;
   sensor_msgs::ImagePtr msg;
   ros::Rate loop_rate(5);

   
   while(nHandle.ok()) {
      cap >> frame;
      if(!frame.empty()) {
         //convert frame to ros image message and publish
         msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
         pub.publish(msg);
         cv::waitKey(1);
      }
      ros::spinOnce();
      loop_rate.sleep();
   }
   return 0;
}

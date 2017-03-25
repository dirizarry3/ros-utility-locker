#include <ros/ros.h>
//#include <std_msgs/String.h>
//#include <sensor_msgs/Image.msg>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> //convert command line param

using namespace std;

int main(int argc, char** argv)
{
   //init ros from potential command line args and give it a name
   ros::init(argc, argv, "webcam_publisher");
   cout << "here1" << endl;
   //get node handle so we can publish the camera data
   ros::NodeHandle nHandle;
   image_transport::ImageTransport it(nHandle);
   image_transport::Publisher pub = it.advertise("camera_image", 1);
   cout << "here2" << endl;
   std::istringstream video_sourceCmd(argv[1]);
   int video_source;
   
   if(!(video_sourceCmd >> video_source)) return 1;
   cout << "here3" << endl;
   cv::VideoCapture cap(video_source);
   if(!cap.isOpened()) return 1;
   
   cv::Mat frame;
   sensor_msgs::ImagePtr msg;
   cout << "here4" << endl;
   ros::Rate loop_rate(5);
   cout << "here5" << endl;
   while(nHandle.ok()) {
      cap >> frame;
      if(!frame.empty()) {
         msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
         pub.publish(msg);
         cv::waitKey(1);
      }
      ros::spinOnce();
      loop_rate.sleep();
   }
   return 0;
}

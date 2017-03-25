#include <ros/ros.h>
//#include <std_msgs/String.h>
//#include <sensor_msgs/Image.msg>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> //convert command line param

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
   ros::init(argc, argv, "imageProc");

   cout << "Using OpenCV: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

   //read image and define keypoint vector
   Mat img = imread("/home/riskit/Desktop/testIm.jpg", IMREAD_COLOR);
   vector<KeyPoint> keypoints;

   //create feature detectors, AKAZE is for scale/rotation invarient
   Ptr<FeatureDetector> featureDetector = FeatureDetector::create("FAST");
   //Ptr<KAZE> akaze = KAZE::create();
   
   // Detect the keypoints
   featureDetector->detect(img, keypoints); // NOTE: featureDetector is a pointer hence the '->'.

   //create a smart pointer to the ORB feature descriptor.
   Ptr<DescriptorExtractor> featureExtractor = DescriptorExtractor::create("ORB");

   // Compute the ORB descriptor at each keypoint.
   // Each row in "descriptors" correspond to the ORB descriptor for each keypoint
   Mat descriptors;
   Scalar keypointColor = Scalar(0, 255, 0);     // Blue keypoints.
   featureExtractor->compute(img, keypoints, descriptors);

   Mat output;
   //drawKeypoints(img, keypoints, output, keypointColor, DrawMatchesFlags::DEFAULT);
   drawKeypoints(img, keypoints, output, Scalar::all(-1));
   cout << descriptors << endl;
   imshow("Keypoints 1", output);
      waitKey(0);

   //imwrite("siftIm.jpg", output);
   
   cout << "made it this far" << endl;

   return 0;
}

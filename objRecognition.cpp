/*Please refer to README file*/

#define _CRT_SECURE_NO_DEPRECATE
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

//Constant declarations
const int FRAME_WIDTH = 680; 
const int FRAME_HEIGHT = 480; 
string window1 = "Circle Tracking";
string window2 = "Threshold (8-bit color depth"; 

//Function declarations
void morph(Mat &img); 

int main()
{
	
	//Declare matrix objects to hold the webcam Feed and the threshold image
	Mat webcamFeed, 
		thresholdFeed; 
	//Object for capturing video
	VideoCapture capture;
	//Open built-in webcam(1)
	capture.open(0);
	//Set webcam width and height
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//Vector to hold circle x and y coordinates and its radius
	vector<Vec3f> circles;
	//circle radius
	int radius; 

	while (1){
		//Take picture using the built-in webcam
		capture.read(webcamFeed);

		//Convert input image to an 8-bit-color depth image
		cvtColor(webcamFeed, 
			thresholdFeed, 
			CV_BGR2Lab);
		//Project red color objects of the input image onto the threshold image
		inRange(webcamFeed, 
			Scalar(CV_RGB(150, 0, 0)), 
			Scalar(CV_RGB(256, 100, 100)), 
			thresholdFeed);
		//Remove and fill small objects in the FG 
		morph(thresholdFeed); 
		//Reduce nouse of threshold image. This will facilitate circle detection
		GaussianBlur(thresholdFeed, 
			thresholdFeed, 
			Size(9, 9), 
			2, 
			2);
		//Perform hough transformation t find circles and store them in vector circles
		HoughCircles(thresholdFeed, 
			circles, 
			CV_HOUGH_GRADIENT, 
			2, thresholdFeed.cols / 4, 
			100, 
			50,
			10, 
			400);

		//Draw the detected circles
		for (size_t i = 0; i < circles.size(); i++)
		{
			//Display ball x and y coordinates
			cout << "Ball position x = " << circles[i][0]
				 << " y = " << circles[i][1]
				 << " r = " << circles[i][2]
				 << endl; 
			
			//Create object of 2D point 
			Point center(cvRound(circles[i][0]), 
				cvRound(circles[i][1]));
			//Store radius
			radius = cvRound(circles[i][2]);
			//Draw inner circle (green)
			circle(webcamFeed, 
				center, 
				3, 
				Scalar(0, 255, 0), -1, 8, 0);
			//Draw outter circle
			circle(webcamFeed, 
				center, 
				radius, 
				Scalar(0, 0, 255), 
				3, 
				8, 
				0);
		}

		//Display image (video)
		imshow(window1, webcamFeed);
		imshow(window2, thresholdFeed);
		
		//Delay picture (video)
		waitKey(33);
	}
	return 0;
}

//Function definitions
void morph(Mat &img){

	//Morphological(opening) operation to remove small objects from the FG
	erode(img, 
		img, 
		getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(img, 
		img, 
		getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		
	//Morphological(closing) operation to fill small holes in the FG
	dilate(img, 
		img, 
		getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(img, 
		img, 
		getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
}

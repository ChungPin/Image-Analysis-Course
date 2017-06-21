#include <stdio.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>
#include <iostream>
#include <cstring>

using namespace std;
using namespace cv;
int main() {
	// declares all required variables
	char filename[128];
	Rect2d roi1,roi2,roi3,roi4,roi5;
	Mat frame;
	VideoWriter output;
	Size videoSize;
	int fps;
	// create a tracker object
	Ptr<Tracker> trackerKCF = Tracker::create("KCF");
	Ptr<Tracker> trackerMIL = Tracker::create("MIL");
	Ptr<Tracker> trackerBOOSTING = Tracker::create("BOOSTING");
	Ptr<Tracker> trackerTLD = Tracker::create("TLD");
	Ptr<Tracker> trackerMEDIANFLOW = Tracker::create("MEDIANFLOW");
	// set input video
	std::string video = "Blurface/img/%04d.jpg";
	VideoCapture cap(video);
	// get bounding box
	cap >> frame;
	roi1 = selectROI("tracker", frame);
	roi2 = roi1;
	roi3 = roi1;
	roi4 = roi1;
	roi5 = roi1;
	//videowriter setting
	videoSize = Size(frame.cols, frame.rows);
	fps = cap.get(CV_CAP_PROP_FPS);
	sprintf_s(filename, "blurface.avi");
	output.open(filename, CV_FOURCC('M', 'J', 'P', 'G'), fps, videoSize);
	
	// initialize the tracker
	trackerKCF->init(frame, roi1);
	trackerMIL->init(frame, roi2);
	trackerTLD->init(frame, roi3);
	trackerBOOSTING->init(frame, roi4);
	trackerMEDIANFLOW->init(frame, roi5);
	// perform the tracking process
	printf("Start the tracking process, press ESC to quit.\n");
	for (;; ) {
		// get frame from the video
		cap >> frame;
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;
		// update the tracking result and draw the tracked object
		trackerKCF->update(frame, roi1);
		rectangle(frame, roi1, Scalar(255, 0, 0), 2, 1); //KCF blue
		trackerMIL->update(frame, roi2);
		rectangle(frame, roi2, Scalar(0, 255, 0), 2, 1); //MIL green
		trackerTLD->update(frame, roi3);
		rectangle(frame, roi3, Scalar(0, 0, 255), 2, 1); //TLD red
		trackerBOOSTING->update(frame, roi4);
		rectangle(frame, roi4, Scalar(255, 255, 0), 2, 1); //BOOSTING cyan
		trackerMEDIANFLOW->update(frame, roi5);
		rectangle(frame, roi5, Scalar(255, 0, 255), 2, 1); //MEDIANFLOW magenta
		
		// show image with the tracked object
		imshow("tracker", frame);
		//write
		output.write(frame);
		//quit on ESC button
		if (waitKey(1) == 27)break;
	}
	output.release();
	return 0;
}

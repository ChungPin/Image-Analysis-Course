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
	char trackerMethod[5][15] = { "MIL","BOOSTING","MEDIANFLOW","TLD","KCF" };
	Rect2d roi;
	Mat frame;
	VideoWriter output;
	Size videoSize;
	int fps;
	int m = 4;
	// create a tracker object
	Ptr<Tracker> tracker = Tracker::create(trackerMethod[m]);
	// set input video
	std::string video = "Basketball/img/%04d.jpg";
	VideoCapture cap(video);
	// get bounding box
	cap >> frame;
	roi = selectROI("tracker", frame);
	//videowriter setting
	videoSize = Size(frame.cols, frame.rows);
	fps = cap.get(CV_CAP_PROP_FPS);
	sprintf_s(filename, "Basketball_%s.avi",trackerMethod[m]);
	output.open(filename, CV_FOURCC('M', 'J', 'P', 'G'), fps, videoSize);
	//quit if ROI was not selected
	if (roi.width == 0 || roi.height == 0)
		return 0;
	// initialize the tracker
	tracker->init(frame, roi);
	// perform the tracking process
	printf("Start the tracking process, press ESC to quit.\n");
	for (;; ) {
		// get frame from the video
		cap >> frame;
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;
		// update the tracking result
		tracker->update(frame, roi);
		// draw the tracked object
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);
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

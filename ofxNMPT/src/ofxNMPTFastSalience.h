#pragma once

#include "ofMain.h"
#include <opencv2/opencv.hpp>
#include "FastSalience.h"
#include "LQRPointTracker.h"
#include "NMPTUtils.h"

using namespace cv;

class ofxNMPTFastSalience {
	
public:
	
	ofxNMPTFastSalience();
	
	void	update(IplImage* colorImage);
	Mat		getSalMat();
	
	cv::Point			getLqrpt();
	vector<KeyPoint>	getPoints();
	
private:
	
	FastSalience*			salTracker;
	LQRPointTracker*		salientSpot;
	vector<double>			lqrpt;
	vector<KeyPoint>		points;
	CvMat*					cameraMat;
};

#pragma once

#include "ofMain.h"
#include "cv.h";
#include "MIPOMDP.h"

class ofxNMPTFaceTracker {
	
public:
	
	ofxNMPTFaceTracker(int width, int height, int numScales, int gridXSize, int gridYSize, string haarDetectorXMLFile);
	
	void		update(IplImage* grayImage);
	int			getX();
	int			getY();
	
	cv::Point	getPosition();
	IplImage*	getFoveaRepresentation();
	
	
private:
	
	MIPOMDP* faceTracker;
};

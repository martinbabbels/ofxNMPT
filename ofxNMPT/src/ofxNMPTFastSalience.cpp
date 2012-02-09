#include "ofxNMPTFastSalience.h"

/*
 * ofxNMPTFastSalience
 *
 */
ofxNMPTFastSalience::ofxNMPTFastSalience() {
	// Create a Salience tracker
	//salTracker = new FastSalience();
	salTracker = new FastSalience(2, 6, 1.0, 0);
	// Create a LQR point tracker
	salientSpot = new LQRPointTracker(2);
	
	// Initialize the lqrpt
	lqrpt.push_back(2);
	lqrpt.push_back(.5);
	
	// Set the salient spot tracker target
	salientSpot->setTrackerTarget(lqrpt);

}

/*
 * update
 * 
 * @param colorImage IplImage*	Input frame
 *
 */
void ofxNMPTFastSalience::update(IplImage* colorImage) {
	// Set the camera Mat
	CvMat stub;
	cameraMat = cvGetMat(colorImage, &stub, 0, 0);
	
	// Update the salience tracker
	salTracker->updateSalience(colorImage);
	
	// Detect and store points
	points.clear();
	salTracker->detect(cameraMat, points);
}

/*
 * getPoints
 * 
 * @returns	vector<cv::KeyPoint>
 *
 */
vector<cv::KeyPoint> ofxNMPTFastSalience::getPoints() {
	return points;
}

/*
 * getLqrpt
 * 
 * @returns	cv::Point
 *
 */
cv::Point ofxNMPTFastSalience::getLqrpt() {
	double min, max; 
	cv::Point minloc, maxloc; 
	cv::Mat salMat = getSalMat();
	
	minMaxLoc(salMat, &min, &max, &minloc, &maxloc); 
	
	lqrpt[0] = maxloc.x*1.0 / salMat.cols;  
	lqrpt[1] = maxloc.y*1.0 / salMat.rows; 
	
	salientSpot->setTrackerTarget(lqrpt); 
	
	salientSpot->updateTrackerPosition(); 
	lqrpt = salientSpot->getCurrentPosition();
	
	return cv::Point(lqrpt[0]*salMat.cols, lqrpt[1]*salMat.rows);
}

/*
 * getSalMat
 * 
 * @returns	cv::Mat	The current Salience representation
 *
 */
cv::Mat ofxNMPTFastSalience::getSalMat() {
	cv::Mat salMat;
	salTracker->getSalImage(salMat);
	
	return salMat;
}
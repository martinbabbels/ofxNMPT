#include "ofxNMPTFaceTracker.h"

/*
 * ofxNMPTFaceTracker
 * 
 * @param width int						Input frame width
 * @param height int					Input frame height
 * @param numScales int					Number of Patches in the Image Patch Pyramid.
 * @param gridXSize						SubImage size x
 * @param gridYSize						SubImage size y
 * @param haarDetectorXMLFile string	The Haar detector xml file
 *
 */
ofxNMPTFaceTracker::ofxNMPTFaceTracker(int width, int height, int numScales, int gridXSize, int gridYSize, string haarDetectorXMLFile) {
	
	int scaleGridCells[numScales]; 
	int ngrid_pts = 1; 
	
	for (int i = 0; i < numScales; i++) {
		int inc = ngrid_pts+2; 
		int halfwidth = (numScales - i - 1)*inc + ngrid_pts;
		
		scaleGridCells[i] =  halfwidth*2+1; //,9,15,21
	}
	
	int numGridPoints = scaleGridCells[0]; 
	
	// The size of the images that will be given to the IPP to turn into MIPOMDP observations. 
	// This allocates memory for underlying data, but it can be changed easily later if needed 
	// without recreating the object by calling the changeInputSize() functions.
	CvSize inputImageSize = cvSize(width, height); 
	
	// The common size to which all image patches will be reduced, creating the foveation effect. 
	// The smaller subImageSize is, the faster search is, and the more extreme the effect of foveation. 
	// This allocates memory for underlying data, but it can be changed easily later if needed without 
	// recreating the object by calling the changeInputSize() functions.
	CvSize subImageSize = cvSize(gridXSize, gridYSize); 
	
	// Size of the discretization of the image. The number of POMDP states is the product of the 
	// demensions of this size (e.g. 21x21).
	CvSize gridSize = cvSize(numGridPoints, numGridPoints); 
	
	// A matrix that describes the size and shape of each level (patch) of the IP Pyramid. 
	// This must be a matrix with size [numSubImages x 2]. Each row contains the width and height 
	// of the corresponding levels. These should be in order of *decreasing* size, so that the largest 
	// Image Patch is first. For example, in Butko and Movellan CVPR 2009, we used [21 21; 15 15; 9 9; 3 3]. 
	// Finaly, note that it is not necessary that the largest patch cover the entire image. However, when the 
	// largest patch is the same size as grid-cell-matrix, special optimizations become available that reduce 
	// the complexity of the algorithm when the same image, or same frame of video, is fixated multiple times.
	CvMat* subImageGridPoints = cvCreateMat(numScales, 2, CV_32SC1); 
	
	for (int i = 0; i < numScales; i++) {
		cvSetReal2D(subImageGridPoints, i, 0, scaleGridCells[i]); 
		cvSetReal2D(subImageGridPoints, i, 1, scaleGridCells[i]); 
	}
	
	// Instatiate MOPOMDP as tracker
	faceTracker = new MIPOMDP(
							  cvSize(width, height), 
							  subImageSize,
							  gridSize,
							  numScales,
							  subImageGridPoints,
							  ofToDataPath(haarDetectorXMLFile).c_str()
	);
	
}

/*
 * update
 * 
 * @param grayImage IplImage*	Input frame
 *
 */
void ofxNMPTFaceTracker::update(IplImage* grayImage) {
	faceTracker->searchNewFrame(grayImage);
}

/*
 * getPosition
 * 
 * @return cv::Point	The current position of the face
 *
 */
cv::Point ofxNMPTFaceTracker::getPosition() {
	return faceTracker->getMostLikelyTargetLocation();
}

/*
 * getX
 * 
 * @return int	The current X position of the face
 *
 */
int ofxNMPTFaceTracker::getX() {
	getPosition().x;
}

/*
 * getY
 * 
 * @return int	The current Y position of the face
 *
 */
int ofxNMPTFaceTracker::getY() {
	getPosition().y;
}

/*
 * getFoveaRepresentation
 * 
 * @returns	IplImage*	The current Fovea representation
 *
 */
IplImage* ofxNMPTFaceTracker::getFoveaRepresentation() {
	return faceTracker->foveaRepresentation;
}
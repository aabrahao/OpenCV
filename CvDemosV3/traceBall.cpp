#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <string>

void info(void) {
	std::cout << "EML 4840 Robot Design" << std::endl
			  << "Florida International University" << std::endl
			  << "Department of Mechanical and Materials Engineering" << std::endl
			  << "Anthony Abrahao <aabra037@fiu.edu>" << std::endl
			  << "Miami, Spring 2019" << std::endl
			  << std::endl;
}

#define DEFAULT_CAMERA 0
#define KEY_ESC 27

// Windows
const std::string windowImage = "FIU-EML4840: Image";
const std::string windwoMask = "FIU-EML4840: Mask";

bool isEqual(double a, double b, double epsilon = 1.0e-8 ) {
	return std::abs(a - b) <= epsilon;
}

cv::Point computeCentroid(const cv::Mat &mask) {
	// find moments of the image
	cv::Moments m = cv::moments(mask, true);
		if( isEqual(m.m00, 0.0) )
			return cv::Point(-100, -100);
	return cv::Point(m.m10/m.m00, m.m01/m.m00);
}

cv::Mat createMask( const cv::Mat &image, 
                    const cv::Scalar &lower,
                    const cv::Scalar &upper,
                    bool clean = true ) {
	// Convert frame image from BGR to HSV color space
	cv::Mat hsv, kernel;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
	// Create a mask using calibrated color
    cv::Mat mask;
    cv::inRange(hsv, lower, upper, mask);
    if (clean) {
        // Clean isolated pixels
        cv::erode(mask, mask, kernel, cv::Point(-1,-1), 4);
        // Enhance surviving pixels
        cv::dilate(mask, mask, kernel, cv::Point(-1,-1), 4);
    }
    return mask;
}

void processImage(const cv::Mat &image) {
   // Define heu level of treshold color
	int color = 46; // Some sort of lime color!!!
	// Define lower and upper treshold color range
	cv::Scalar lower(color - 20, 80, 80);
	cv::Scalar upper(color + 20, 255, 255);
	// Create mask	
	auto mask = createMask(image, lower, upper);
	// Display mask
	// cv::imshow(windowMask, mask);
	// Compute center of the mask
	cv::Point c = computeCentroid(mask);
	// Draw centroid over image
	cv::circle(image, c, 10, cv::Scalar(0,0,255), 10);
	// Display image
	cv::imshow(windowImage, image);
}

int main(int argc, char **argv) {
	info();
	// Create a capture object
	cv::VideoCapture cap;
    // Check arguments
    if (argc > 1)
        cap.open( argv[1] ); // Open filename
    else
        cap.open( DEFAULT_CAMERA ); // Open camera
	// Check if any error happened
	if( !cap.isOpened() ) {
		std::cout << "Ops, capture cannot be created!" << std::endl;
		return -1;
	}
	std::cout << "Press 'Esc' to quit..." << std::endl;
	while( true ) {
		// Create a image object 
		cv::Mat image;
		// Capture frame
		bool okay = cap.read( image );
		// Skip if any capture error happened (or video ended)
		if( okay )
			processImage(image); // Implement our image processing
		// Break loop if the key 'ESC' was pressed
		if( cv::waitKey(30) == KEY_ESC )
			break;
	}
	return 0;
}

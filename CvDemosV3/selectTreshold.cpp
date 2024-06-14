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
const std::string window = "Treshold";

// Trackbars values
int g_lh = 0, g_ls = 0, g_lv = 0; // Lower color values
int g_uh = 180, g_us = 255, g_uv = 255; // Upper color values

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

void createWindow(void) {
	// Creating a window
    cv::namedWindow(window, cv::WINDOW_NORMAL);
    // Creating trackbars
	cv::createTrackbar("Lower H", window, &g_lh, 180);
	cv::createTrackbar("Lower S", window, &g_ls, 255);
	cv::createTrackbar("Lower V", window, &g_lv, 255);
    cv::createTrackbar("Upper H", window, &g_uh, 180);
	cv::createTrackbar("Upper S", window, &g_us, 255);
	cv::createTrackbar("Upper V", window, &g_uv, 255);
}

void updateWindow(const cv::Mat &image) {
   // Convert frame image from BGR to HSV color space
	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    // Set threshold from trackbars
	cv::Scalar lower(g_lh, g_ls, g_lv);
	cv::Scalar upper(g_uh, g_us, g_uv);
	// Create mask	
	auto mask = createMask(image, lower, upper, false);
	cv::Mat result;
	cv::bitwise_and(image, image, result, mask);
	// Display image
	cv::imshow(window, result);
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
	createWindow();
	cv::Mat frame, image;
    while( true ) {
		// Capture frame
		bool okay = cap.read( frame );
		// Skip if any capture error happened (or video ended)
        if( okay )
            image = frame;
		updateWindow(image); // Implement our image processing
		// Break loop if the key 'ESC' was pressed
		if( cv::waitKey(30) == KEY_ESC )
			break;
	}
	return 0;
}

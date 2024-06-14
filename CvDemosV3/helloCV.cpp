#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

void info(void) {
	std::cout << "EML 4840 Robot Design" << std::endl
			  << "Florida International University" << std::endl
			  << "Department of Mechanical and Materials Engineering" << std::endl
			  << "Anthony Abrahao <aabrahao@fiu.edu>" << std::endl
			  << "Miami, Spring 2022" << std::endl
			  << std::endl;
}

#define KEY_ESC 27

int main(int argc, char **argv) {
	info();
	// Create a capture object from device number (or video filename)
	cv::VideoCapture cap(0);
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
			cv::imshow("Image", image);
		// Break loop if the key 'ESC' was pressed
		if( cv::waitKey(30) == KEY_ESC )
			break;
	}
	return 0;
}

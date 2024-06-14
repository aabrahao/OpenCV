#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <string>

typedef cv::Mat Image;
typedef cv::Scalar Color;
typedef std::vector<Color> Colors;
typedef cv::Point Point;
typedef std::vector<Point> Points;
typedef std::vector<Points> Contours;

void info(void) {
	std::cout << "EML 4840 Robot Design" << std::endl
		  << "Florida International University" << std::endl
		  << "Department of Mechanical and Materials Engineering" << std::endl
		  << "Anthony Abrahao <aabra037@fiu.edu>" << std::endl
		  << "Miami, Spring 2019" << std::endl
		  << std::endl;
}

#define DEFAULT_CAMERA "../balls05.mp4" // Instead of 0
#define KEY_ESC 27  

// Windows
const std::string window = "FIU-EML4840: Image";
float g_zoom = 0.75f;

// Remember track
Points g_track;
Colors g_colors;

Image resize(const Image &image, float factor) {
    cv::Size s = image.size();
    Image resized;
    cv::resize( image, resized, cv::Size( factor*s.width, factor*s.height ) );
    return resized;  
}

Points computeCentroidContours(const Contours &contours) {
	Points centers;
    // Find moments for each contour and store in centroids
    for ( const auto &contour : contours ) {
        cv::Moments m = cv::moments(contour);
        centers.push_back( m.m00 <= 1e-4 ? Point(-1, -1) : Point(m.m10/m.m00, m.m01/m.m00) );
    }
    return centers;
}

Image createMask( const Image &image, 
                  const Color &lower,
                  const Color &upper,
                  bool clean = true ) {
	// Convert frame image from BGR to HSV color space
	Image hsv, kernel;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
	// Create a mask using calibrated color
    Image mask;
    cv::inRange(hsv, lower, upper, mask);
    if (clean) {
        // Clean isolated pixels
        cv::erode(mask, mask, kernel, Point(-1,-1), 4);
        // Enhance surviving pixels
        cv::dilate(mask, mask, kernel, Point(-1,-1), 4);
    }
    return mask;
}

Points traceBall(const Image &image, const Color &lower, const Color &upper) {
    // Create mask
	auto mask = createMask(image, lower, upper);
    // Find indidual balls
    Contours contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
	// Compute center of the mask
	return computeCentroidContours(contours);
}

void processImage(Image &image) {
    // Ball color ranges
    Colors lowers = { Color( 95, 160, 150),   // Blue
                      Color( 50, 115, 170),   // Green
                      Color(167, 167, 230) }; // Pink
    Colors uppers = { Color(110, 224, 237),  // Blue
                      Color( 70, 190, 244),   // Green
                      Color(171, 220, 248) }; // Pink
    for ( int i = 0; i < uppers.size(); ++i ) {
        // Trace the ball
        auto centroids =  traceBall(image, lowers[i], uppers[i]);
        // For each contour
        for ( auto centroid : centroids ) {
            g_track.push_back( centroid );
            g_colors.push_back( lowers[i] );
        }
    }
    // Draw all centroids
    for ( int i = 0; i < g_track.size(); ++i) {
        cv::circle(image, g_track[i], 2, g_colors[i], 3);
    }
    // Display image
    image = resize(image, g_zoom);
    cv::imshow(window, image);
}

int main(int argc, char **argv) {
	info();
	// Create a capture object
	cv::VideoCapture capture;
    // Check arguments
    if (argc > 1)
        capture.open( argv[1] ); // Open filename
    else
        capture.open( DEFAULT_CAMERA ); // Open camera
	// Check if any error happened
	if( !capture.isOpened() ) {
		std::cout << "Ops, capture cannot be created!" << std::endl;
		return -1;
	}
	std::cout << "Press 'Esc' to quit (or 'c' to clear tracks)..." << std::endl;
	while( true ) {
		// Create a image object 
		Image image;
		// Capture frame
		bool okay = capture.read( image );
		// Skip if any capture error happened (or video ended)
		if( okay )
            processImage(image); // Implement our image processing
            // Get pressed key
		int key = cv::waitKey(30);
		// Break loop if the key 'ESC' was pressed
		if( key == KEY_ESC )
			break;
		else if( key == 'c' )
			g_track.clear();
	}
	return 0;
}

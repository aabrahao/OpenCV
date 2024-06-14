#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

int main(int argc, char ** argv) {
    
    auto image = cv::imread("../shapes.jpg");
    if ( image.empty() ) {
        std::cout << "Could not read the image! " << std::endl;
        return 1;
    }
    cv::imshow("Image", image);
    cv::imwrite("0_image.png", image);
    
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("Gray", gray);
    cv::imwrite("1_gray.png", gray);
    
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
    cv::imshow("Blurred", blurred);
    cv::imwrite("2_blurred.png", blurred);
    
    cv::Mat mask;
    cv::threshold(blurred, mask, 60, 255, cv::THRESH_BINARY);
    cv::imshow("Mask", mask);
    cv::imwrite("3_mask.png", mask);
    
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    for ( int i = 0; i < contours.size(); i++ )
        cv::drawContours( image, contours, i, cv::Scalar(0, 0, 255), 2, cv::LINE_8, hierarchy, 0 );
    cv::imshow("Countours", image);
    cv::imwrite("4_contours.png", image);
    
    int key = cv::waitKey(0); // Wait for a keystroke in the window
    
    return 0;
}

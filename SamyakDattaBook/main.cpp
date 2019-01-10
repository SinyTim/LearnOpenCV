#include <opencv2/opencv.hpp>
#include <iostream>



int main() {

    cv::Mat image = cv::imread("images/lena.jpg", cv::IMREAD_COLOR);

    cv::imshow("Image", image);

    cv::waitKey();

    return 0;
}

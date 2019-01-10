#pragma once

#include <opencv2/opencv.hpp>
#include <vector>



void showChannels(const cv::Mat& image);



void chapter1() {

    cv::Mat image = cv::imread("images/lena.jpg", cv::IMREAD_COLOR);

    cv::imshow("Image", image);

    showChannels(image);

    cv::waitKey(0);
}



void showChannels(const cv::Mat& image) {

    std::vector<cv::Mat> channels;

    cv::split(image, channels);

    cv::imshow("Blue", channels[0]);
    cv::imshow("Green", channels[1]);
    cv::imshow("Red", channels[2]);
}

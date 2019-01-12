#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include "measureTime.h"


void showChannels(const cv::Mat& image);
void clearRedChannel(cv::Mat& image);
void clearGreenChannel(cv::Mat& image);
void clearBlueChannel(cv::Mat& image);



void chapter1() {

    cv::Mat image = cv::imread("images/lena.jpg", cv::IMREAD_COLOR);
    
    measureTime(clearRedChannel, image);
    measureTime(clearGreenChannel, image);
    measureTime(clearBlueChannel, image);

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

void clearRedChannel(cv::Mat& image) {

    int numChannels = image.channels();
    int numRows = image.rows;
    int numCols = image.cols * numChannels;

    if (image.isContinuous()) {
        numCols *= numRows;
        numRows = 1;
    }

    static const int RED_CHANNEL = 2;

    for (size_t i = 0; i < numRows; ++i) {
        uchar* rowPtr = image.ptr<uchar>(i);
        for (size_t j = 0; j < numCols; ++j) {
            if (j % numChannels == RED_CHANNEL) {
                rowPtr[j] = 0;
            }
        }
    }
}

void clearGreenChannel(cv::Mat& image) {

    static const int GREEN_CHANNEL = 1;

    for (size_t i = 0; i < image.rows; ++i) {
        for (size_t j = 0; j < image.cols; ++j) {
            auto& pixel = image.at<cv::Vec3b>(i, j);  // if 1 channel: Vec3b->uchar
            pixel[GREEN_CHANNEL] = 0;
        }
    }
}

void clearBlueChannel(cv::Mat& image) {

    cv::Mat_<cv::Vec3b> imageWrap = image;

    static const int BLUE_CHANNEL = 0;

    for (size_t i = 0; i < image.rows; ++i) {
        for (size_t j = 0; j < image.cols; ++j) {
            imageWrap(i, j)[BLUE_CHANNEL] = 0;
        }
    }
}

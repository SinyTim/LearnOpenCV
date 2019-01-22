#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <functional>
#include "measureTime.h"



class Chapter1 {

public:

    static void run() {

        //cv::Mat image = cv::imread("images/lena.jpg", cv::IMREAD_COLOR);
        cv::Mat image = cv::imread("images/td.jpg", cv::IMREAD_COLOR);
        cv::resize(image, image, cv::Size(), 0.3, 0.3);

        //testTraversals(image);
        //testLUT(image);

        cv::waitKey(0);
    }

    static void testTraversals(const cv::Mat& image) {

        auto imageCopy = image.clone();

        cv::imshow("Image", imageCopy);

        measureTime(clearRedChannel, imageCopy);
        measureTime(clearGreenChannel, imageCopy);
        measureTime(clearBlueChannel, imageCopy);

        cv::imshow("Processed image", imageCopy);

        showChannels(imageCopy);
    }

    static void testLUT(const cv::Mat& image) {

        auto imageCopy = image.clone();
        cv::cvtColor(imageCopy, imageCopy, cv::COLOR_BGR2GRAY);
        cv::imshow("Image", imageCopy);

        //processImage(imageCopy, negativeTransform);

        /*auto f = std::bind(piecewiseLinearTransform, std::placeholders::_1, 
                           cv::Point2i(256 * 3/10, 255 * 1/5), 
                           cv::Point2i(256 * 7/10, 255 * 4/5));
        processImage(imageCopy, f);*/

        /*double maxValue = 0;
        cv::minMaxLoc(imageCopy, nullptr, &maxValue);
        auto f = std::bind(logTransform, std::placeholders::_1, maxValue);
        processImage(imageCopy, f);*/

        //logTransformImage(imageCopy);

        /*double maxValue = 0;
        cv::minMaxLoc(imageCopy, nullptr, &maxValue);
        auto f = std::bind(expTransform, std::placeholders::_1, maxValue);
        processImage(imageCopy, f);*/

        //expTransformImage(imageCopy);

        //cv::LUT(imageCopy, getLutMat(negativeTransform), imageCopy);

        cv::imshow("Processed image", imageCopy);
    }

private:

    static void showChannels(const cv::Mat& image) {

        std::vector<cv::Mat> channels;

        cv::split(image, channels);

        cv::imshow("Blue", channels[0]);
        cv::imshow("Green", channels[1]);
        cv::imshow("Red", channels[2]);
    }

    // Traversals:

    static void clearRedChannel(cv::Mat& image) {

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

    static void clearGreenChannel(cv::Mat& image) {

        static const int GREEN_CHANNEL = 1;

        for (size_t i = 0; i < image.rows; ++i) {
            for (size_t j = 0; j < image.cols; ++j) {
                auto& pixel = image.at<cv::Vec3b>(i, j);  // if 1 channel: Vec3b->uchar
                pixel[GREEN_CHANNEL] = 0;
            }
        }
    }

    static void clearBlueChannel(cv::Mat& image) {

        cv::Mat_<cv::Vec3b> imageWrap = image;

        static const int BLUE_CHANNEL = 0;

        for (size_t i = 0; i < image.rows; ++i) {
            for (size_t j = 0; j < image.cols; ++j) {
                imageWrap(i, j)[BLUE_CHANNEL] = 0;
            }
        }
    }

    // ---
    //
    // Lookup table (LUT):
    //
    // Linear transformations:
    // ~ Identity
    // ~ Negative
    // Logarithmic transformations:
    // ~ Log
    // ~ Inverse log (exponential)

    static void processImage(cv::Mat& image, 
                             const std::function<uchar(uchar)>& transform) {

        auto lut = getLUT(transform);

        for (size_t i = 0; i < image.rows; ++i) {
            for (size_t j = 0; j < image.cols; ++j) {
                auto& pixel = image.at<uchar>(i, j);
                pixel = lut[pixel];
            }
        }
    }

    static std::vector<uchar> getLUT(const std::function<uchar(uchar)>& transform) {

        std::vector<uchar> lut(256, 0);

        for (size_t i = 0; i < lut.size(); ++i) {
            lut[i] = transform(static_cast<uchar>(i));
        }

        return lut;
    }

    static cv::Mat getLutMat(const std::function<uchar(uchar)>& transform) {

        cv::Mat lut(1, 256, CV_8U);

        for (size_t i = 0; i < 256; ++i) {
            lut.at<uchar>(0, i) = transform(static_cast<uchar>(i));
        }

        return lut;
    }

    static uchar identityTransform(uchar pixel) {
        return pixel;
    }

    static uchar negativeTransform(uchar pixel) {
        return 255 - pixel;
    }

    static uchar piecewiseLinearTransform(uchar pixel, 
                                          const cv::Point2i& p1, 
                                          const cv::Point2i& p2) {

        if ((pixel >= 0) && (pixel < p1.x)) {
            return (p1.y / p1.x) * pixel;

        } else if ((pixel >= p1.x) && (pixel <= p2.x)) {
            return (p2.y - p1.y) / (p2.x - p1.y) * (pixel - p1.x) + p1.y;

        } else if ((pixel > p2.x) && (pixel < 256)) {
            return (255 - p2.y) / (255 - p2.x) * (pixel - p2.x) + p2.y;

        } else {
            return pixel;
        }
    }

    static uchar logTransform(uchar pixel, uchar maxValue) {
        double c = 255.0 / std::log10(1.0 + static_cast<double>(maxValue));
        return static_cast<uchar>(c * std::log10(1 + pixel));
    }

    static void logTransformImage(cv::Mat& image) {
        image.convertTo(image, CV_32F);
        image += 1;
        cv::log(image, image);
        cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
        cv::convertScaleAbs(image, image);
    }

    static uchar expTransform(uchar pixel, uchar maxValue) {
        static const double BASE = 1.02;
        double c = 255.0 / (std::pow(BASE, maxValue) - 1); 
        return static_cast<uchar>(c * (std::pow(BASE, pixel) - 1));
    }

    static void expTransformImage(cv::Mat& image) {
        static const double C = cv::log(1.02);
        image.convertTo(image, CV_32F);
        image *= C;
        cv::exp(image, image);
        image -= 1;
        cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
        cv::convertScaleAbs(image, image);
    }

    // ---
};

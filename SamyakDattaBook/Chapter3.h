#pragma once

#include <opencv2/opencv.hpp>



class Chapter3 {

public:

    static void run() {

        const std::string fileName = "td2.jpg";  // lena.jpg  td1/2.jpg  mandelbrot1/2.png
        const std::string filePath = "images/" + fileName;
        const float scale = 0.3f;

        cv::Mat image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
        cv::resize(image, image, cv::Size(), scale, scale);
        cv::imshow("Image", image);

        //testThreshold(image);
        auto binaryImage = testAdaptiveThreshold(image);

        testDilation(binaryImage);
        testErosion(binaryImage);
        testMorphology(binaryImage);

        cv::waitKey(0);
    }

private:

    // Thresholding:

    static void testThreshold(const cv::Mat& image) {

        cv::Mat binaryImage;

        cv::threshold(image, binaryImage, 100, 255, cv::THRESH_BINARY);

        cv::imshow("Binary image", binaryImage);
    }

    static cv::Mat testAdaptiveThreshold(const cv::Mat& image) {

        cv::Mat binaryImage;

        cv::adaptiveThreshold(image, binaryImage, 
                              200, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 51, 0.0);

        cv::imshow("Binary image", binaryImage);

        return binaryImage;
    }

    // ---
    //
    // Morphological operations:
    // ~ Dilation (less black)
    // ~ Erosion (more black)

    static void testDilation(const cv::Mat& image) {

        cv::Mat dilatedImage;

        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, 
                                                    cv::Size(5, 5), cv::Point(-1, -1));

        cv::dilate(image, dilatedImage, element, cv::Point(-1, -1), 1);

        imshow("Dilated image", dilatedImage);
    }

    static void testErosion(const cv::Mat& image) {

        cv::Mat erodedImage;

        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                    cv::Size(5, 5), cv::Point(-1, -1));

        cv::erode(image, erodedImage, element, cv::Point(-1, -1), 1);

        imshow("Eroded image", erodedImage);
    }

    static void testMorphology(const cv::Mat& image) {

        cv::Mat result;

        cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                    cv::Size(5, 5), cv::Point(-1, -1));
        cv::morphologyEx(image, result, cv::MORPH_GRADIENT, element);

        imshow("Morphology", result);
    }

    // ---
};
#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>



class Chapter2 {

public:

    static void run() {

        const std::string fileName = "td.jpg";  // lena.jpg  td.jpg  mandelbrot1/2.png
        const std::string filePath = "images/" + fileName;
        const float scale = 0.3f;

        cv::Mat image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
        cv::resize(image, image, cv::Size(), scale, scale);
        cv::imshow("Image", image);

        //testBoxFilter(image);
        //testBlur(image);
        //printGaussianKernel();
        //testGaussian(image);
        //testOwnFilter(image);

        vignetting(image);

        cv::waitKey(0);
    }

private:

    // Blurring:

    static void testBoxFilter(const cv::Mat& image) {

        cv::Mat filteredImage;

        cv::boxFilter(
            image,
            filteredImage,
            -1,                   // CV_32F, CV_64F, ...
            cv::Size(3, 3),       // kernel size
            cv::Point(-1, -1),    // anchor point
            true,                 // normalization: kernel(3x3) * 1/9
            cv::BORDER_REPLICATE
        );

        cv::imshow("Filtered", filteredImage);
    }

    static void testBlur(const cv::Mat& image) {

        cv::Mat filteredImage;

        cv::blur(image, filteredImage, 
                 cv::Size(3, 3), cv::Point(-1, -1), 
                 cv::BORDER_REPLICATE);

        cv::imshow("Filtered", filteredImage);
    }

    static void printGaussianKernel() {

        cv::Mat gaussianKernel = cv::getGaussianKernel(7, 1.0);  // Have one single column.

        std::cout << "Gaussian Kernel: ";

        for (int i = 0; i < gaussianKernel.rows; ++i) {
            std::cout << gaussianKernel.at<double>(i, 0) << " ";
        }
        std::cout << "\n";
    }

    static void testGaussian(const cv::Mat& image) {

        cv::Mat filteredImage;

        GaussianBlur(image, filteredImage, 
                     cv::Size(7, 7), 
                     1.0, 1.0,               // Standard deviations
                     cv::BORDER_REPLICATE);

        imshow("Filtered", filteredImage);
    }

    static void testOwnFilter(const cv::Mat& image) {

        cv::Mat filteredImage;  

        static const int kernelSize = 7;
        cv::Mat kernel = cv::Mat::ones(kernelSize, kernelSize, CV_32F) 
                         / (float)(kernelSize * kernelSize);

        filter2D(image, filteredImage, -1, 
                 kernel, cv::Point(-1, 1), 
                 0, cv::BORDER_REPLICATE);
        
        imshow("Filtered", filteredImage);
    }

    // ---
    //
    // Vignetting:

    static void vignetting(const cv::Mat& image) {
        
        auto mask = getGaussianSquareNormalMaskForMat(image, 100.0);

        cv::Mat processedImage;
        image.convertTo(processedImage, CV_64F);

        cv::multiply(mask, processedImage, processedImage);
        cv::convertScaleAbs(processedImage, processedImage);

        cv::imshow("Processed image", processedImage);
    }

    static cv::Mat getGaussianSquareNormalMaskForMat(const cv::Mat& mat, double sigma) {

        cv::Mat kernelX = cv::getGaussianKernel(mat.cols, sigma);
        transpose(kernelX, kernelX);

        cv::Mat kernelY = cv::getGaussianKernel(mat.rows, sigma);

        cv::Mat kernel = kernelY * kernelX;

        cv::normalize(kernel, kernel, 0, 1, cv::NORM_MINMAX);

        return kernel;
    }

    // ---
};

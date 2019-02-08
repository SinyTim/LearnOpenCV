#pragma once

#include <opencv2/opencv.hpp>



class Chapter3 {

public:

    static void run() {

        const std::string fileName = "td.jpg";  // lena.jpg  td.jpg  mandelbrot1/2.png
        const std::string filePath = "images/" + fileName;
        const float scale = 0.3f;

        cv::Mat image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
        cv::resize(image, image, cv::Size(), scale, scale);
        cv::imshow("Image", image);

        //

        cv::waitKey(0);
    }

private:
};
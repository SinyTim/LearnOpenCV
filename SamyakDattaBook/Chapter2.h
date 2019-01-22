#pragma once

#include <opencv2/opencv.hpp>



class Chapter2 {

public:

    static void run() {

        //cv::Mat image = cv::imread("images/lena.jpg", cv::IMREAD_COLOR);
        cv::Mat image = cv::imread("images/td.jpg", cv::IMREAD_COLOR);
        cv::resize(image, image, cv::Size(), 0.3, 0.3);

        //

        cv::waitKey(0);
    }
};

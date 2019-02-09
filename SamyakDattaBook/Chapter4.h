#pragma once

#include <opencv2/opencv.hpp>



class Chapter4 {

public:

    static void run() {

        const std::string fileName = "td2.jpg";  // lena.jpg  td1/2.jpg  mandelbrot1/2.png
        const std::string filePath = "images/" + fileName;
        const float scale = 0.3f;

        cv::Mat image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
        cv::resize(image, image, cv::Size(), scale, scale);
        cv::imshow("Image", image);

        printHistogram(image);

        cv::waitKey(0);
    }

private:

    // Histograms:

    static cv::Mat getHistogram(const cv::Mat& image) {

        cv::Mat histogram = cv::Mat::zeros(256, 1, CV_32F);

        /*for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                float binIdx = static_cast<float>(image.at<uchar>(i, j));
                ++histogram.at<float>(binIdx, 0);
            }
        }*/
        
        int channels[] = {0};
        int histSize[] = {256};
        float range[] = {0, 256};
        const float* ranges[] = {range};
        cv::calcHist(&image, 1, channels, cv::Mat(), histogram, 1, histSize, ranges, true, false);
        
        return histogram;
    }

    static void printHistogram(const cv::Mat& image) {

        auto histogram = getHistogram(image);

        std::cout << "Histogram:\n";
        for (int i = 0; i < histogram.rows; ++i) {
            std::cout << i << ": " << histogram.at<float>(i, 0) << "\n";
        }
    }
};


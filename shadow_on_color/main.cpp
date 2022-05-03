#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    cv::Mat originalColor(cv::Size(768, 256), CV_8UC3, cv::Scalar(52, 67, 175));
    cv::Mat color(cv::Size(256, 256), CV_8UC3, cv::Scalar(52, 67, 175));
    cv::Mat gray;
    cv::Mat gray1(cv::Size(256, 256), CV_8UC1, cv::Scalar(70));
    cv::Mat gray2(cv::Size(256, 256), CV_8UC1, cv::Scalar(170));
    cv::Mat gray3(cv::Size(256, 256), CV_8UC1, cv::Scalar(240));

    cv::hconcat(gray2, gray1, gray);
    cv::hconcat(gray3, gray, gray);

    cv::cvtColor(gray, color, cv::COLOR_GRAY2BGR);

    cout << " Gamma correction - Bright" << endl;
    cv::Mat lookUpTable(1, 256, CV_8UC3);
    double gamma = 1 / 2.2;

    for (int i = 0; i < 256; ++i)
    {
        lookUpTable.at<cv::Vec3b>(i)[0] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
        lookUpTable.at<cv::Vec3b>(i)[1] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
        lookUpTable.at<cv::Vec3b>(i)[2] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }

    cv::LUT(color, lookUpTable, color);
    cout << " Gamma correction - Bright" << endl;

    cv::multiply(color, cv::Scalar(52, 67, 175), color, 1 / 256.0);

    cv::cvtColor(gray, gray, cv::COLOR_GRAY2BGR);

    cv::vconcat(gray, originalColor, originalColor);
    cv::vconcat(originalColor, color, color);

    cv::imshow("Simulate shadow on color", color);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

// main.cpp

#include <iostream>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    cv::Mat frame;
    cv::VideoCapture cap;
    int deviceID = 0;
    int apiID = cv::CAP_ANY;

    cap.open(deviceID, apiID);

    cout << "Start grabbing" << endl
         << "Press any key to terminate" << endl;

    int minHue = 0, maxHue = 59, minSat = 255, maxSat = 255, minValue = 255, maxValue = 255;

    cv::namedWindow("Trackbars");
    cv::createTrackbar("Min hue", "Trackbars", &minHue, 255);
    cv::createTrackbar("Max hue", "Trackbars", &maxHue, 255);
    cv::createTrackbar("Min Sat", "Trackbars", &minSat, 255);
    cv::createTrackbar("Max Sat", "Trackbars", &maxSat, 255);
    cv::createTrackbar("Min Val", "Trackbars", &minValue, 255);
    cv::createTrackbar("Max Val", "Trackbars", &maxValue, 255);

    while (true)
    {
        cv::Scalar lower(minHue, minSat, minValue);
        cv::Scalar upper(maxHue, maxSat, maxValue);

        cap.read(frame);

        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        cv::Mat imgHSV;
        cv::cvtColor(frame, imgHSV, cv::COLOR_BGR2HSV);

        cv::Mat mask;
        cv::inRange(imgHSV, lower, upper, mask);

        cout << minHue << " (minHue)" << maxHue << " (maxHue)"
             << minValue << " (minValue)" << maxValue << " (maxValue)"
             << minSat << "(minSat)" << maxSat << "(maxSat)" << endl;

        cv::imshow("Original", frame);
        cv::imshow("Original HSV", imgHSV);
        cv::imshow("Mask", mask);
        cv::waitKey(1);
    }

    cv::destroyAllWindows();

    return 0;
}

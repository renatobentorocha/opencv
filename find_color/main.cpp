// main.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

// cv::Mat blueMatrix = cv::Mat(1, 1, CV_8UC3, cv::Scalar(255, 0, 0));
// cv::Mat blueMatrixHSV;
// cv::cvtColor(blueMatrix, blueMatrixHSV, cv::COLOR_BGR2HSV);

// cv::Mat greenMatrix = cv::Mat(1, 1, CV_8UC3, cv::Scalar(0, 255, 0));
// cv::Mat greenMatrixHSV;
// cv::cvtColor(greenMatrix, greenMatrixHSV, cv::COLOR_BGR2HSV);

// cv::Mat redMatrix = cv::Mat(1, 1, CV_8UC3, cv::Scalar(0, 0, 255));
// cv::Mat redMatrixHSV;
// cv::cvtColor(redMatrix, redMatrixHSV, cv::COLOR_BGR2HSV);

// std::cout << "Blue" << std::endl;
// std::cout << blueMatrix << endl;
// std::cout << blueMatrixHSV << endl;

// std::cout << "Green" << std::endl;
// std::cout << greenMatrix << endl;
// std::cout << greenMatrixHSV << endl;

// std::cout << "Red" << std::endl;
// std::cout << redMatrix << endl;
// std::cout << redMatrixHSV << endl;
// cv::imshow("Blue image", blueMatrix);

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        cout << "Expecting a image file to be passed to program" << endl;
        return -1;
    }

    cv::Mat img;
    img = cv::imread(argv[1]);

    if (img.empty())
    {
        cout << "Not a valid image file" << endl;
        return -1;
    }

    int minHue = 0, maxHue = 59, minSat = 255, maxSat = 255, minValue = 255, maxValue = 255;

    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

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

        cv::Mat mask;
        cv::inRange(imgHSV, lower, upper, mask);

        cv::imshow("Original", img);
        cv::imshow("Original HSV", imgHSV);
        cv::imshow("Mask", mask);
        cv::waitKey(1);
    }

    cv::destroyAllWindows();

    return 0;
}

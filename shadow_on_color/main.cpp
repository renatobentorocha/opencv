// main.cpp

#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

void locator(int event, int x, int y, int flags, void *userdata)
{ // function to track mouse movement and click//
    if (event == cv::EVENT_LBUTTONDOWN)
    { // when left button clicked//
        cout << "Left click has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == cv::EVENT_RBUTTONDOWN)
    { // when right button clicked//
        cout << "Rightclick has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == cv::EVENT_MBUTTONDOWN)
    { // when middle button clicked//
        cout << "Middleclick has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == cv::EVENT_MOUSEMOVE)
    { // when mouse pointer moves//
        cout << "Current mouse position:(" << x << "," << y << ")" << endl;
    }
}

int main(int argc, char **argv)
{
    cv::Mat originalColor(cv::Size(768, 256), CV_8UC3, cv::Scalar(52, 67, 175));
    cv::Mat color(cv::Size(256, 256), CV_8UC3, cv::Scalar(52, 67, 175));
    cv::Mat gray;
    cv::Mat gray1(cv::Size(256, 256), CV_8UC1, cv::Scalar(70));
    cv::Mat gray2(cv::Size(256, 256), CV_8UC1, cv::Scalar(170));
    cv::Mat gray3(cv::Size(256, 256), CV_8UC1, cv::Scalar(255));

    cv::hconcat(gray2, gray1, gray);
    cv::hconcat(gray3, gray, gray);

    cout << "R " << ceil(255 * 175 * 1 / 256.0) << endl;
    cout << "G " << ceil(255 * 67 * 1 / 256.0) << endl;
    cout << "R " << ceil(255 * 52 * 1 / 256.0) << endl;

    cv::cvtColor(gray, color, cv::COLOR_GRAY2BGR);
    cv::multiply(color, cv::Scalar(52, 67, 175), color, 1 / 256.0);

    color.convertTo(color, color.type(), 1, 0.5);

    cv::cvtColor(gray, gray, cv::COLOR_GRAY2BGR);

    cv::vconcat(gray, originalColor, originalColor);
    cv::vconcat(originalColor, color, color);

    cv::imshow("Simulate shadow on color", color);

    // cout << "Color\n\n\n"
    //      << color << endl;
    // cv::setMouseCallback("Mouse callback", locator, NULL); // Mouse callback function on define window//

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

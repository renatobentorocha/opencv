// main.cpp

#include <iostream>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

// 0 (minHue)
// 255 (maxHue)
// 0 (minValue)
// 255 (maxValue)
// 0(minSat)
// 189(maxSat)

cv::Mat frame;

void getContours(cv::Mat img)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> conPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    cv::Point p(0, 0);

    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);
        cout << area << endl;

        if (area > 1000)
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            boundRect[i] = boundingRect(conPoly[i]);
            p.x = boundRect[i].x + boundRect[i].width / 2;
            p.y = boundRect[i].y;

            drawContours(frame, contours, i, cv::Scalar(255, 0, 255), 2);
            rectangle(frame, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 5);
        }
    }
}

void findColor()
{
    cv::Scalar lower(107, 199, 2);
    cv::Scalar upper(122, 255, 74);

    cv::Mat imgHSV;
    cv::cvtColor(frame, imgHSV, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(imgHSV, lower, upper, mask);
    getContours(mask);
}

int main(int argc, char **argv)
{
    cv::VideoCapture cap;

    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API

    // open selected camera using selected API
    cap.open(deviceID, apiID);

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
         << "Press any key to terminate" << endl;
    while (true)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);

        // check if we succeeded
        if (frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // show live and wait for a key with timeout long enough to show images
        findColor();
        imshow("Live", frame);

        if (cv::waitKey(1) >= 0)
            break;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor

    return 0;
}

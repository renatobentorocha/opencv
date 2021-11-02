// main.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

void locator(int event, int x, int y, int flags, void *userdata)
{ //function to track mouse movement and click//
    if (event == cv::EVENT_LBUTTONDOWN)
    { //when left button clicked//
        cout << "Left click has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == cv::EVENT_RBUTTONDOWN)
    { //when right button clicked//
        cout << "Rightclick has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == cv::EVENT_MBUTTONDOWN)
    { //when middle button clicked//
        cout << "Middleclick has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == cv::EVENT_MOUSEMOVE)
    { //when mouse pointer moves//
        cout << "Current mouse position:(" << x << "," << y << ")" << endl;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Expecting a image file to be passed to program" << endl;
        return -1;
    }

    cv::Mat img = cv::imread(argv[1]);

    if (img.empty())
    {
        cout << "Not a valid image file" << endl;
        return -1;
    }

    cv::namedWindow("Mouse callback", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Mouse callback", locator, NULL); //Mouse callback function on define window//
    cv::imshow("Mouse callback", img);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

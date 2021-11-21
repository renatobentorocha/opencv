// main.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

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

    cout << "Width : " << img.size().width << endl;
    cout << "Height: " << img.size().height << endl;
    cout << "Channels: :" << img.channels() << endl;

    cv::Mat cropped = img(cv::Range(100, 400), cv::Range(100, 400));

    cv::imshow("Original", img);
    cv::imshow("Cropped", cropped);
    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}

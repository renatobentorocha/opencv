// main.cpp

#include <iostream>
#include <string>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/objdetect.hpp"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Expecting an image and a xml file to be passed to program" << endl;
        return -1;
    }

    cv::Mat img;
    img = cv::imread(argv[1]);

    if (img.empty())
    {
        cout << "Not a valid image file" << endl;
        return -1;
    }

    cv::CascadeClassifier classifier;

    if (!classifier.load(argv[2]))
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };

    cv::Mat gray_img;
    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray_img, gray_img);

    std::vector<cv::Rect> faces;
    classifier.detectMultiScale(gray_img, faces);

    for (size_t i = 0; i < faces.size(); i++)
    {
        rectangle(img, faces[i].tl(), faces[i].br(), cv::Scalar(0, 255, 0), 5);
    }

    cv::namedWindow("Window", cv::WINDOW_KEEPRATIO);
    cv::imshow("Window", img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

// main.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{

    // 1 - Read the Image and convert it to Grayscale Format
    // 2 - Apply Binary Thresholding
    // 3 - Find the Contours
    // 4 - Draw Contours on the Original RGB Image.

    if (argc != 2)
    {
        cout << "Expecting a image file to be passed to program" << endl;
        return -1;
    }

    // 1 - Read the Image and convert it to Grayscale Format
    cv::Mat img = cv::imread(argv[1]);

    if (img.empty())
    {
        cout << "Not a valid image file" << endl;
        return -1;
    }

    cv::imshow("Original", img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    // 2 - Apply Binary Thresholding
    cv::Mat thresh;
    cv::threshold(img_gray, thresh, 150, 255, cv::THRESH_BINARY);
    cv::imshow("Binary mage", thresh);
    cv::waitKey(0);
    cv::imwrite("image_thres1.jpg", thresh);
    cv::destroyAllWindows();

    // 3 - Find the Contours
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

    // 4 - Draw Contours on the Original RGB Image.
    cv::Mat image_copy = img.clone();
    cv::drawContours(image_copy, contours, -1, cv::Scalar(0, 255, 0), 2);
    cv::imshow("None approximation", image_copy);
    cv::waitKey(0);
    cv::imwrite("contours_none_image1.jpg", image_copy);
    cv::destroyAllWindows();

    return 0;
}

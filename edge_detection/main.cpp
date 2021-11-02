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

double median(cv::Mat channel, int size)
{
    double m = (channel.rows * channel.cols) / 2;
    int bin = 0;
    double med = -1.0;

    int histSize = size;
    float range[] = {0, static_cast<float>(size)};
    const float *histRange = {range};
    bool uniform = true;
    bool accumulate = false;
    cv::Mat hist;
    cv::calcHist(&channel, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

    for (int i = 0; i < histSize && med < 0.0; ++i)
    {
        bin += cvRound(hist.at<float>(i));
        if (bin > m && med < 0.0)
            med = i;
    }

    return med;
}

cv::Mat auto_canny(cv::Mat image, float sigma = 0.33)
{
    // compute the median of the single channel pixel intensities
    double v = median(image, image.cols);

    // apply automatic Canny edge detection using the computed median
    double lower = int(std::max(0.0, (1.0 - sigma) * v));
    double upper = int(std::min(255.0, (1.0 + sigma) * v));
    // cv::Mat edged = cv::Canny(image, lower, upper);
    cv::Mat edged;

    cv::Canny(image, edged, lower, upper);

    return edged;
}

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
    cv::imshow("Grayed", img_gray);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // 2 - Apply a GaussianBlur to smooth the image
    cv::Mat blurred;
    cv::GaussianBlur(img_gray, blurred, cv::Size(3, 3), 0);
    cv::imshow("Blurred", blurred);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // 3 - Edge detection
    cv::Mat edged = auto_canny(blurred);
    cv::namedWindow("Auto canny edge", cv::WINDOW_AUTOSIZE);
    // cv::setMouseCallback("Auto canny edge", locator, NULL); //Mouse callback function on define window//
    cv::imshow("Auto canny edge", edged);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

// main.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

cv::Mat img;
cv::Mat img_gray;
cv::Mat blurred;
cv::Mat edged;

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

void onClick(int event, int x, int y, int flags, void *userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    { //when left button clicked//

        int connectivity = 4;
        int newMaskVal = 255;
        int ffillMode = 0;
        uchar fillValue = 128;
        cv::Scalar newVal = cv::Scalar(255, 0, 0);
        cv::Rect ccomp;
        int flags = connectivity + (newMaskVal << 8) +
                    (ffillMode == 1 ? cv::FLOODFILL_FIXED_RANGE : 0);

        // cv::threshold(edged, edged, 1, 128, cv::THRESH_BINARY);
        int area = cv::floodFill(img, edged, cv::Point(x, y), newVal, &ccomp, cv::Scalar(10, 10, 10), cv::Scalar(10, 10, 10), flags);
        cout << "Area: " << area << endl;
        cv::imshow("Flooded", img);
    }
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        cout << "Expecting a image file to be passed to program" << endl;
        return -1;
    }

    img = cv::imread(argv[1]);

    if (img.empty())
    {
        cout << "Not a valid image file" << endl;
        return -1;
    }

    cv::imshow("Original", img);
    cv::waitKey(0);

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    cv::imshow("Gray", gray);
    cv::waitKey(0);

    cv::Mat threshold;
    cv::threshold(gray, threshold, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

    cv::namedWindow("Threshold", cv::WINDOW_AUTOSIZE);
    // cv::setMouseCallback("Original", onClick, NULL);
    cv::imshow("Threshold", threshold);
    cv::waitKey(0);

    // noise removal
    cv::Mat kernel(3, 3, CV_8UC1, cv::Scalar::all(1));
    cv::Mat opening;
    cv::morphologyEx(threshold, opening, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);

    cv::imshow("Opening", opening);
    cv::waitKey(0);

    // sure background area
    cv::Mat sure_bg;
    cv::dilate(opening, sure_bg, kernel, cv::Point(-1, -1), 3);

    cv::imshow("Sure bg", sure_bg);
    cv::waitKey(0);

    //Finding sure foreground area
    cv::Mat dist_transform;
    cv::distanceTransform(opening, dist_transform, cv::DIST_L2, 5);
    // Seems be the same that Opening
    cv::imshow("dist_transform", dist_transform);
    cv::waitKey(0);

    double minVal;
    double maxVal;
    cv::Point minLoc;
    cv::Point maxLoc;
    cv::Mat sure_fg;

    cv::minMaxLoc(dist_transform, &minVal, &maxVal, &minLoc, &maxLoc);
    cv::threshold(dist_transform, sure_fg, 0.7 * maxVal, 255, 0);

    cv::imshow("sure_fg", sure_fg);
    cv::waitKey(0);
    // sure_fg = cv2.threshold(dist_transform, 0.7 * dist_transform.max(), 255, 0)

    // Finding unknown region
    // Error here
    cv::Mat unknown(sure_bg.rows, sure_bg.cols, sure_bg.type());
    cv::subtract(sure_bg, sure_fg, unknown);

    cv::imshow("unknown", unknown);
    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}

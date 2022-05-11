// main.cpp

#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

cv::Mat img;
cv::Mat img_gray;
cv::Mat blurred;
cv::Mat edged;

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
    { // when left button clicked//

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

cv::Mat imgOriginal, imgGray, imgCanny, imgBlur, imgDil, imgErode, imgWarp;
std::vector<cv::Point> initialPoints, docPoints;

cv::Mat preProcessing(cv::Mat img)
{
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 0);
    imgCanny = auto_canny(imgBlur);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(imgCanny, imgDil, kernel);
    // cv::erode(imgCanny, imgErode, kernel);

    return imgDil;
}

std::vector<cv::Point> getContours(cv::Mat imgDil)
{

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point> biggest;

    findContours(imgDil, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> conPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    unsigned maxArea = 0;

    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);

        if (area > 1000)
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            if (area > maxArea && conPoly[i].size() == 4)
            {
                biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
                maxArea = area;
            }
            // cout << conPoly[i].size() << endl;
            // boundRect[i] = boundingRect(conPoly[i]);

            drawContours(imgOriginal, contours, i, cv::Scalar(255, 0, 255), 2);
            // rectangle(imgOriginal, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 5);
        }
    }

    return biggest;
}

void drawPoints(std::vector<cv::Point> points, cv::Scalar color)
{
    for (int i = 0; i < points.size(); i++)
    {
        cv::circle(imgOriginal, points[i], 5, color, cv::FILLED);
        cv::putText(imgOriginal, std::to_string(i), points[i], cv::FONT_HERSHEY_PLAIN, 4, color, 4);
    }
}

std::vector<cv::Point> reorder(std::vector<cv::Point> points)
{
    std::vector<cv::Point> newsPoints;
    std::vector<int> sumPoints, subPoints;

    for (int i = 0; i < points.size(); i++)
    {
        sumPoints.push_back(points[i].x + points[i].y);
        subPoints.push_back(points[i].x - points[i].y);
    }

    newsPoints.push_back(
        points[std::min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 0
    newsPoints.push_back(
        points[std::max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 1
    newsPoints.push_back(
        points[std::min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 2
    newsPoints.push_back(
        points[std::max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 3

    return newsPoints;
}

cv::Mat getWarp(cv::Mat img, std::vector<cv::Point> points, float w, float h)
{
    cv::Point2f src[4] = {points[0], points[1], points[2], points[3]};
    cv::Point2f dst[4] = {{0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h}};

    cv::Mat matrix = cv::getPerspectiveTransform(src, dst);
    cv::warpPerspective(img, imgWarp, matrix, cv::Point(w, h));

    return imgWarp;
}

int main(int argc, char **argv)
{

    imgOriginal = cv::imread("../document.jpeg");
    cv::imshow("Original", imgOriginal);

    cv::resize(imgOriginal, imgOriginal, cv::Size(), 0.5, 0.5);

    cv::Mat imgThreshold = preProcessing(imgOriginal);

    initialPoints = getContours(imgThreshold);

    docPoints = reorder(initialPoints);
    drawPoints(docPoints, cv::Scalar(0, 0, 255));
    getWarp(imgOriginal, docPoints, imgOriginal.size().width, imgOriginal.size().height);
    cv::resize(imgWarp, imgWarp, cv::Size(), 1.5, 1.5);

    cv::imshow("Resized", imgOriginal);
    cv::imshow("Threshold ", imgThreshold);
    cv::imshow("Warp ", imgWarp);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

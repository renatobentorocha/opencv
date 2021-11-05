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

    cv::Mat result;
    img.copyTo(result);

    cv::imshow("Original", img);
    cv::waitKey(0);

    // Create binary image from source image
    cv::Mat bw;
    cv::cvtColor(img, bw, cv::COLOR_BGR2GRAY);
    cv::threshold(bw, bw, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
    cv::imshow("Binary image", bw);
    cv::waitKey(0);

    // Perform the distance transform algorithm
    cv::Mat dist;
    cv::distanceTransform(bw, dist, cv::DIST_L2, 5);

    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);
    cv::imshow("Distance transforme", dist);
    cv::waitKey(0);

    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    cv::threshold(dist, dist, 0, 255, cv::THRESH_BINARY);
    cv::imshow("Distance transforme", dist);
    cv::waitKey(0);

    cv::Mat markers(img.size(), CV_32S);
    dist.convertTo(dist, CV_8UC1);
    int nLabels = cv::connectedComponents(dist, markers, 8);

    cv::Mat markers_cp = markers.clone();
    markers_cp.convertTo(markers_cp, CV_8UC1);
    imshow("Connected Components", markers_cp);
    cv::waitKey(0);

    std::vector<cv::Vec3b> colors(nLabels);
    colors[0] = cv::Vec3b(0, 0, 0); //background

    for (int label = 1; label < nLabels; ++label)
    {
        colors[label] = cv::Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
    }

    cv::Mat dst(img.size(), CV_8UC3);

    for (int r = 0; r < dst.rows; ++r)
    {
        for (int c = 0; c < dst.cols; ++c)
        {
            int label = markers.at<int>(r, c);
            cv::Vec3b &pixel = dst.at<cv::Vec3b>(r, c);
            pixel = colors[label];
        }
    }

    imshow("Connected Components Color", dst);
    cv::waitKey(0);

    cv::watershed(img, markers);
    markers.convertTo(markers, CV_8UC1);

    imshow("Watershed", markers);
    cv::waitKey(0);

    cv::Mat mask(markers.size(), CV_8UC3, cv::Scalar::all(255));
    mask.setTo(cv::Scalar(0), markers);

    imshow("Mask from watershed", mask);
    cv::waitKey(0);

    result.setTo(cv::Scalar(0, 0, 255), mask);
    imshow("Result with Mask", result);
    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}

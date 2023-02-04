#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main() {

    Mat image;
    Mat image2;
    namedWindow("Display window");
    VideoCapture cap(0);
    Rect r = Rect(0, 0, 1280, 360); //1280 × 720
    Rect r2 = Rect(640, 0, 640, 720); //1280 × 720

    int centH = 360, centW = 640;
    Vec3b vec;

    while (true) {
        cap >> image;

        cvtColor(image, image2, COLOR_BGR2HSV);

        vec = image2.at<Vec3b>(centH, centW);
        Vec3b vec2;
        int sec = 180 / 3;
        int sec2 = sec / 2;
        if ((vec[0] > sec2) && (vec[0] < (sec2 + sec))) {
            vec2[0] = 0;
            vec2[1] = 255;
            vec2[2] = 0;
        } else if ((vec[0] > sec2 + sec) && (vec[0] < 180 - sec2)) {
            vec2[0] = 255;
            vec2[1] = 0;
            vec2[2] = 0;
        } else {
            vec2[0] = 0;
            vec2[1] = 0;
            vec2[2] = 255;
        }

        rectangle(image, r, Scalar(vec2), 1, 8, 0);
        rectangle(image, r2, Scalar(vec2), 1, 8, 0);

        imshow("Display window2", image2);
        imshow("Display window", image);
        waitKey(25);
    }
    return 0;
}

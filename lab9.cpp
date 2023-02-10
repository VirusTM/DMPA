    #include <iostream>
    #include <time.h>
    #include "opencv2/highgui/highgui.hpp"
    #include "opencv2/imgproc/imgproc.hpp"

    using namespace cv;
    using namespace std;

    int main( int argc, char** argv ) {
    cout << getNumThreads() << endl;
    VideoCapture cap(-1); //capture the video from webcam
    int ret;
    ret = cap.set(3, 320);
    ret = cap.set(4, 240);
    if ( !cap.isOpened() ) // if not success, exit program
    {
    cout << "Cannot open the web cam" << endl;
    return -1;
    }
    namedWindow("Control", WINDOW_NORMAL); //create a window called"Control"

    int iLowH = 0;
    int iHighH = 30;
    int iLowS = 150;
    int iHighS = 255;
    int iLowV = 60;
    int iHighV = 255;

    //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);
    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);
    createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);int iLastX = -1;
    int iLastY = -1;

    // Capture a temporary image from the camera
    Mat imgTmp;
    cap.read(imgTmp);
    // Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
    time_t start,end;
    time (&start);
    int frames = 0;
    while (true) {
    Mat imgOriginal;
    bool bSuccess = cap.read(imgOriginal); // read a new frame
    //if not success, break loop
    if (!bSuccess) {
    cout << "Cannot read a frame from video stream" << endl;
    break;
    }Mat imgHSV;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
    Mat imgThresholded;
    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS,
    iHighV), imgThresholded); //Threshold the image
    //morphological opening (removes small objects from the foreground)
    erode(imgThresholded, imgThresholded,
    getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded,
    getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //morphological closing (removes small holes from the foreground)
    dilate( imgThresholded, imgThresholded,
    getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded,
    getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );//Calculate the moments of the thresholded image
    Moments oMoments = moments(imgThresholded);
    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;
    // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
    if (dArea > 10000)
    {
    //calculate the position of the object
    int posX = dM10 / dArea;
    int posY = dM01 / dArea;
    if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
    {
    //Draw a red line from the previous point to the current point
    //line(imgLines, Point(posX, posY), Point(iLastX, iLastY),
    //Scalar(0,0,255), 2);
    Point topLeft(posX - 60, posY - 60);
    Point bottomRight (posX + 60, posY + 60);

    rectangle(imgOriginal, topLeft, bottomRight, Scalar(0, 0, 0), 2, LINE_8);
    }
    iLastX = posX;
    iLastY = posY;
    }imshow("Thresholded Image", imgThresholded); //show the thresholded image
    imgOriginal = imgOriginal + imgLines;
    imshow("Original", imgOriginal); //show the original image
    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
    cout << "esc key is pressed by user" << endl;
    break;
    }
    frames++;
    }
    time (&end);
    double dif = difftime (end,start);
    printf("FPS %.2lf seconds.\r\n", (frames / dif));
    return 0;
    }


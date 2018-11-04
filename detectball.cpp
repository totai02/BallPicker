#include "detectball.h"

DetectBall::DetectBall()
{
    ifstream in;
    in.open("/home/pi/threshold.txt");
    if (!in.is_open()) return;
    for(int i = 0; i < 6; i++)
    {
        in >> red_threshold[i / 2][i % 2];
    }

    for(int i = 0; i < 6; i++)
    {
        in >> blue_threshold[i / 2][i % 2];
    }
}

DetectBall::~DetectBall()
{
    ofstream out;
    out.open("/home/pi/threshold.txt");
    for(int i = 0; i < 6; i++)
    {
        out << red_threshold[i / 2][i % 2] << " ";
    }
    out << endl;
    for(int i = 0; i < 6; i++)
    {
        out << blue_threshold[i / 2][i % 2] << " ";
    }
}

int *DetectBall::getRedThreshold(int model, int level)
{
    return &red_threshold[model][level];
}

int *DetectBall::getBlueThreshold(int model, int level)
{
    return &blue_threshold[model][level];
}

void DetectBall::detectBall(const Mat &src, int color)
{
    Mat imgHSV = preprocess(src);
    redBalls.clear();
    blueBalls.clear();
    findContour(imgHSV, color);
}

Mat DetectBall::preprocess(const Mat &src)
{
    Mat imgHSV;
    cvtColor(src, imgHSV, COLOR_BGR2HSV);
    return imgHSV;
}

void DetectBall::findContour(const Mat &imgHSV, int color)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat imgThresholded = morphological(imgHSV, color);
	
	if (Config::DISPLAY_COLOR_THRESHOLD){
		if (color == 1) imshow("Control Red", imgThresholded);
		else imshow("Control Blue", imgThresholded);
	}
	
    /// Find contours
    findContours( imgThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }

    for( int i = 0; i < contours.size(); i++ )
    {
        if (radius[i] < imgThresholded.rows/4)
        {
            if (contourArea(Mat(contours[i])) > radius[i] * radius[i] * M_PI * 0.6)
            {
                if (color == 1) redBalls.push_back(Vec3f(center[i].x, center[i].y, radius[i]));
                else blueBalls.push_back(Vec3f(center[i].x, center[i].y, radius[i]));
            }
        }
    }
}

Mat DetectBall::morphological(const Mat &imgHSV, int color)
{
    Mat imgThresholded;

    if (color == 1) inRange(imgHSV, Scalar(red_threshold[0][0], red_threshold[1][0], red_threshold[2][0]), Scalar(red_threshold[0][1], red_threshold[1][1], red_threshold[2][1]), imgThresholded);
    else inRange(imgHSV, Scalar(blue_threshold[0][0], blue_threshold[1][0], blue_threshold[2][0]), Scalar(blue_threshold[0][1], blue_threshold[1][1], blue_threshold[2][1]), imgThresholded);

    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    /// Reduce the noise so we avoid false circle detection
    medianBlur(imgThresholded, imgThresholded, 3);
//    GaussianBlur( imgThresholded, imgThresholded, Size(9, 9), 2, 2 );

    return imgThresholded;
}

vector<Vec3f> DetectBall::getBlueBalls() const
{
    return blueBalls;
}

vector<Vec3f> DetectBall::getRedBalls() const
{
    return redBalls;
}





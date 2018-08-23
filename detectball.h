#ifndef DETECTBALL_H
#define DETECTBALL_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "config.h"

using namespace std;
using namespace cv;

class DetectBall
{
public:
    DetectBall();
    ~DetectBall();

    int *getRedThreshold(int model, int level);
    int *getBlueThreshold(int model, int level);

    void detectBall(const Mat &src, int color);

    vector<Vec3f> getRedBalls() const;

    vector<Vec3f> getBlueBalls() const;

protected:

    Mat preprocess(const Mat &src);
    void findContour(const Mat &imgHSV, int color);
    Mat morphological(const Mat &imgHSV, int color);

    int red_threshold[3][2];
    int blue_threshold[3][2];

    vector<Vec3f> redBalls;
    vector<Vec3f> blueBalls;
};

#endif // DETECTBALL_H

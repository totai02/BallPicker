#ifndef BALLPICKER_H
#define BALLPICKER_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "motorcontrol.h"
#include "detectball.h"
#include "timer.h"
#include "armcontrol.h"
#include "config.h"

using namespace std;
using namespace cv;

class BallPicker{
public:
	BallPicker();
	int start();
	bool isStart();
	
	enum Ball {
		RED = 1,
		BLUE
	};
	
	enum Mode {
		SEARCH_BALL,
		PICK_BALL
	};
	
	enum ManualKey {
		UP = 1,
		RIGHT = 2,
		LEFT = 4,
		DOWN = 8,
		TRIANGLE = 16,
		CIRCLE = 32,
		X = 64,
		SQUARE = 128,
		L2 = 256,
		R2 = 512,
		L1 = 1024,
		R1 = 2048,
		START = 4069,
		SELECT = 8192
	};
	
private:
	void init();
	void drawBall(const vector<Vec3f> &, bool isRed);
	void process(const vector<Vec3f> &, const vector<Vec3f> &);
	
	int mode = SEARCH_BALL;
	bool manual_mode = false;
	
	int ballColor = 0;
	bool run = false;
	int frameSkip = 0;
	unsigned short data[3];
	
	int WIDTH = 208;
	int HEIGHT = 160;
	Size frame;

	Mat src;
	Timer timer;
	MotorControl motor;
	DetectBall detect;
	ArmControl arm;
	VideoCapture capture;
};

#endif

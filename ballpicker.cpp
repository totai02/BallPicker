#include "ballpicker.h"

BallPicker::BallPicker(): capture(VideoCapture(0)),
						frame(Size(WIDTH, HEIGHT))
{
	init();
}

int BallPicker::start()
{
	while(waitKey(1) != 'q'){
		
		if(digitalRead (Config::BTN1) == LOW)
		{
			ballColor = RED;
		}
		
		if(digitalRead (Config::BTN2) == LOW)
		{
			ballColor = BLUE;
		}
		
		if(digitalRead (Config::BTN3) == LOW)
		{
			delay(250);
			if(digitalRead (Config::BTN3) != LOW)
			{
				ballColor = 0;
				motor.stop();
			} 
			else
			{
				delay(1750);
				if(digitalRead (Config::BTN3) == LOW)
				{
					return 1;
				} 
			} 
		}
		
		if (digitalRead (Config::BTN4) == LOW)
		{
			delay(2000);
			if(digitalRead (Config::BTN4) == LOW)
			{
				system("sudo shutdown now");
			}
		}
		
		if (ballColor != 0){
			if (frameSkip > 0) frameSkip--;
			timer.update();
			capture >> src;
			src.convertTo(src, -1, Config::ALPHA / 100.0f, 0);
			resize(src, src, frame);
			detect.detectBall(src, ballColor);
			process(detect.getRedBalls(), detect.getBlueBalls());
		}
		
    }
    capture.release();
    return 0;
}

bool BallPicker::isStart()
{
	return run;
}

void BallPicker::init()
{
	namedWindow( "Origin", CV_WINDOW_AUTOSIZE );
    if (Config::DISPLAY_COLOR_THRESHOLD)
    {
        namedWindow( "Control Red", CV_WINDOW_AUTOSIZE );
        namedWindow( "Control Blue", CV_WINDOW_AUTOSIZE );

        cvCreateTrackbar("LowH", "Control Red", detect.getRedThreshold(0, 0), 179);
        cvCreateTrackbar("HighH", "Control Red", detect.getRedThreshold(0, 1), 179);

        cvCreateTrackbar("LowS", "Control Red", detect.getRedThreshold(1, 0), 255);
        cvCreateTrackbar("HighS", "Control Red", detect.getRedThreshold(1, 1), 255);

        cvCreateTrackbar("LowV", "Control Red", detect.getRedThreshold(2, 0), 255);
        cvCreateTrackbar("HighV", "Control Red", detect.getRedThreshold(2, 1), 255);

        cvCreateTrackbar("LowH", "Control Blue", detect.getBlueThreshold(0, 0), 179);
        cvCreateTrackbar("HighH", "Control Blue", detect.getBlueThreshold(0, 1), 179);

        cvCreateTrackbar("LowS", "Control Blue", detect.getBlueThreshold(1, 0), 255);
        cvCreateTrackbar("HighS", "Control Blue", detect.getBlueThreshold(1, 1), 255);

        cvCreateTrackbar("LowV", "Control Blue", detect.getBlueThreshold(2, 0), 255);
        cvCreateTrackbar("HighV", "Control Blue", detect.getBlueThreshold(2, 1), 255);
    }
    
    pinMode (Config::BTN1, INPUT);
	pullUpDnControl (Config::BTN1, PUD_UP) ;
	
	pinMode (Config::BTN2, INPUT);
	pullUpDnControl (Config::BTN2, PUD_UP) ;
	
	pinMode (Config::BTN3, INPUT);
	pullUpDnControl (Config::BTN3, PUD_UP) ;

	pinMode (Config::BTN4, INPUT);
	pullUpDnControl (Config::BTN4, PUD_UP) ;
	
	motor.init();
	
	arm.idle();
}

void BallPicker::process(const vector<Vec3f> &redBalls, const vector<Vec3f> &blueBalls)
{
    float x, y;
    float max_radius = 0;


    for( size_t i = 0; i < redBalls.size(); i++ )
    {
        if (redBalls[i][2] > max_radius){
            x = redBalls[i][0];
            y = redBalls[i][1];
            max_radius = redBalls[i][2];
        }
    }

    if (redBalls.size() == 0)
    {
        for( size_t i = 0; i < blueBalls.size(); i++ )
        {
            if (blueBalls[i][2] > max_radius){
                x = blueBalls[i][0];
                y = blueBalls[i][1];
                max_radius = blueBalls[i][2];
            }
        }
    }

    if (redBalls.size() > 0 || blueBalls.size() > 0) {
		mode = PICK_BALL;
        line(src, Point(WIDTH / 2, HEIGHT / 2), Point(cvRound(x), cvRound(y)), Scalar(0,255,0), 2, 4, 0);
		float error = (float) (x - WIDTH / 2) / (WIDTH / 2);
		
		float destRadius = WIDTH / 24;
		if (max_radius < destRadius) {
			float x = max_radius / destRadius;
			motor.move_forward(error, 1 - pow(x, 5));
		}
		else {
			if (frameSkip == 0){
				float high = (float) (HEIGHT - y) / HEIGHT;
				motor.stop();
				delay(100);
				arm.pickUp(error - 0.08f, high, max_radius / destRadius);
				delay(100);
				motor.move_back(0);
				delay(1600);
				motor.stop();
				delay(100);
				arm.drop();
				delay(100);
				frameSkip = 10;
			}
		}
    } else {
		if (mode == SEARCH_BALL) motor.move_forward(0, -1);
		else motor.stop();
    }
	
	if (Config::DISPLAY_STREAM){
		line(src, Point(WIDTH / 2 - 5, HEIGHT / 2), Point(WIDTH / 2 + 5, HEIGHT/ 2), Scalar(255,255,255), 2, 8, 0);
		line(src, Point(WIDTH / 2, HEIGHT / 2 - 5), Point(WIDTH / 2, HEIGHT/ 2 + 5), Scalar(255,255,255), 2, 8, 0);

		drawBall(redBalls, true);
		drawBall(blueBalls, false);
		int fps = cvRound(1.0 / timer.getDeltaTime());
		putText(src, to_string(fps), Point2f(5,15), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0));
		imshow( "Origin", src);
	}
}

void BallPicker::drawBall(const vector<Vec3f> &balls, bool isRed)
{
	for( size_t i = 0; i < balls.size(); i++ )
    {
        Point center(cvRound(balls[i][0]), cvRound(balls[i][1]));
        int radius = cvRound(balls[i][2]);
        if (isRed){
            circle(src, center, 3, Scalar(255,0,0), -1, 8, 0 );
            circle(src, center, radius, Scalar(0,0,255), 2, 8, 0 );
        } else {
            circle(src, center, 3, Scalar(0,0,255), -1, 8, 0 );
            circle(src, center, radius, Scalar(255, 0, 0), 2, 8, 0 );
        }
    }
    
}

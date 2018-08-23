#ifndef CONFIG_H
#define CONFIG_H

using namespace std;
class Config
{
public:
	static bool DISPLAY_STREAM;
	
	static bool DISPLAY_COLOR_THRESHOLD;
	
	static bool DISPLAY_PID_CONFIG; 

	static float ALPHA;
	
	static float VELOCITY;
	
	static float MANUAL_VELOCITY;
	
	static float KP;
	
	static float KI;
	
	static float KD;	
	
	// button pin
	static int BTN1;
	static int BTN2;
	static int BTN3;
	static int BTN4;
	
	// softPWM pin
	static int sPWM1_1;
	static int sPWM1_2;
	static int sPWM2_1;
	static int sPWM2_2;
	
	// servo pin
	static int SERVO1;
	static int SERVO2;
	static int SERVO3;
	static int SERVO4;
};

#endif // CONFIG_H

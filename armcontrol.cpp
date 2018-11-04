#include "armcontrol.h"

int run = 1;

int inRange(int value, int min = 600, int max = 2400);

int inRange(int value, int min, int max)
{
	if (value > max) value = max;
	else if (value < min) value = min;
	return value;
}

void stop(int signum)
{
   run = 0;
   gpioServo(Config::SERVO1, 0);
   gpioServo(Config::SERVO2, 0);
   gpioServo(Config::SERVO3, 0);
   gpioTerminate();
}

ArmControl::ArmControl()
{
	if (gpioInitialise() < 0) return;
	gpioSetSignalFunc(SIGINT, stop);
}

void ArmControl::idle()
{
	gpioServo(Config::SERVO1, 900);
	time_sleep(0.5);
	
	gpioServo(Config::SERVO2, 1800);
	time_sleep(0.5);

	gpioServo(Config::SERVO3, 2300);
	time_sleep(0.5);
	
	gpioServo(Config::SERVO4, 1400);
	time_sleep(0.5);
}

void ArmControl::pickUp(float error, float height, float distance)
{
	cout << "Height: " << height << " Distance: " << distance << endl; 

	if (height > 0.5) height = 0.5;
	gpioServo(Config::SERVO4, 1400 + error * 1000);
	time_sleep(0.3);
	
	gpioServo(Config::SERVO3, 1800);
	time_sleep(0.2);
	
	int value2 = inRange(round(1767.462 + 15.24894 * height + 3240.47 * height * height), 1820, 2100);
	
	setServo(Config::SERVO2, value2);
	time_sleep(0.2);
	setServo(Config::SERVO1, 1300);
	time_sleep(0.2);
	gpioServo(Config::SERVO3, 2400);
	time_sleep(0.2);
}

void ArmControl::drop()
{
	setServo(Config::SERVO1, 1000, 20);
	time_sleep(0.2);
	setServo(Config::SERVO2, 1800, 20);
	time_sleep(0.2);
	setServo(Config::SERVO4, 2400, 20);
	time_sleep(0.2);
	gpioServo(Config::SERVO3, 1800);
	time_sleep(0.5);
	gpioServo(Config::SERVO3, 2300);
	time_sleep(0.2);
	setServo(Config::SERVO4, 1300, 20);
	time_sleep(0.2);
}

void ArmControl::setServo(int servo, int pulsewidth, int step)
{
	while (abs(gpioGetServoPulsewidth(servo) - pulsewidth) > step / 2 + 1)
	{
		if (gpioGetServoPulsewidth(servo) > pulsewidth)
		{
			gpioServo(servo, gpioGetServoPulsewidth(servo) - step);
		} else {
			gpioServo(servo, gpioGetServoPulsewidth(servo) + step);
		}
		time_sleep(0.02);
	}
}

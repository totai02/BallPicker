#include "armcontrol.h"

int run = 1;

int range(int value)
{
	if (value > 2400) value = 2400;
	else if (value < 600) value = 600;
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
	gpioServo(Config::SERVO1, 1300);
	time_sleep(0.5);
	
	gpioServo(Config::SERVO2, 1900);
	time_sleep(0.5);

	gpioServo(Config::SERVO3, 1050);
	time_sleep(0.5);
	
	gpioServo(Config::SERVO4, 1600);
	time_sleep(0.5);
}

void ArmControl::pickUp(float error, float high, float x)
{
	gpioServo(Config::SERVO4, 1500);
	time_sleep(0.3);
	
	gpioServo(Config::SERVO3, 1500);
	time_sleep(0.2);
	setServo(Config::SERVO2, range(round(2100 - 1500 * high - (x - 1.01) * 1500)));
	time_sleep(0.2);
	setServo(Config::SERVO1, range(round(4960 - 4000 * x)));
	time_sleep(0.2);
	gpioServo(Config::SERVO3, 1050);
	time_sleep(0.2);
}

void ArmControl::drop()
{
	setServo(Config::SERVO1, 1300, 20);
	time_sleep(0.2);
	setServo(Config::SERVO2, 1900, 20);
	time_sleep(0.2);
	setServo(Config::SERVO4, 2300, 20);
	time_sleep(0.2);
	gpioServo(Config::SERVO3, 1500);
	time_sleep(0.5);
	gpioServo(Config::SERVO3, 1050);
	time_sleep(0.2);
	setServo(Config::SERVO4, 1600, 20);
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

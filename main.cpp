#include <iostream>

#include "ballpicker.h"

using namespace std;

int main(int argc, char *argv[])
{
	setenv("WIRINGPI_GPIOMEM", "1", 1);
    wiringPiSetup();
    BallPicker ballPicker;
	int result = ballPicker.start();
 	return result;
}

#include <iostream>
#include "manual.h"

using namespace std;

int main()
{
	Manual manual;
	manual.init();
	while (true)
	{
		manual.read();
		cout << manual.getLeftX() << endl;
	}
	return 0;
}

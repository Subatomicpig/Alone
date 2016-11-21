#include <math.h>
#include "Controllers\Joystick.h"

static const int MAX_JOYSTICK_VALUE = 32768;

int adjustForDeadzone(int value, int tolerance)
{
	if(abs(value) > tolerance)
	{
		return value - tolerance;
	}
	return 0;
}

void fixDeadzones(int& x1, int& y1, int& x2, int& y2, int dead_zone)
{
	x1 = adjustForDeadzone(x1, dead_zone);
	y1 = adjustForDeadzone(y1, dead_zone);
	x2 = adjustForDeadzone(x2, dead_zone);
	y2 = adjustForDeadzone(y2, dead_zone);
}

float remapWithDeadzone(float value, int tolerance, int max_value)
{
	if(abs(value) > tolerance)
	{
		if(value < 0.0f)
			value += tolerance;
		else
			value -= tolerance;
		return value / (max_value - tolerance);
	}
	return 0.0f;
}

float fixAndRemapDeadzones(int value, int dead_zone)
{
	return remapWithDeadzone(value, dead_zone, MAX_JOYSTICK_VALUE);
}

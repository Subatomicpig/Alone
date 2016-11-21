#ifndef JOYSTICK_H
#define JOYSTICK_H

#define CONTROLLER_TOLERANCE 0.001f
#define MOVE_CONTROLLER_TOLERANCE 0.001f
#define MIN_DEAD_ZONE 5000

int adjustForDeadzone(int value, int tolerance);
void fixDeadzones(int& x1, int& y1, int& x2, int& y2, int dead_zone = MIN_DEAD_ZONE);
float remapWithDeadzone(float value, int tolerance, int max_value);
float fixAndRemapDeadzones(int value, int dead_zone = MIN_DEAD_ZONE);

#endif
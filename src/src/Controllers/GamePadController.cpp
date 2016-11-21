#include "Controllers\GamePadController.h"
#include "Controllers\Joystick.h"

namespace Alone 
{
	static const int MAX_JOYSTICK_VALUE = 32768;
	static bool joystickUpdateCalledOnce = false;

	GamePadController::GamePadController(int joystick, int dead_zone)
	{
		mControllerID = joystick;
		mInputManager = InputManager::getSingletonPtr();
		mDeadZone = dead_zone;
		for(int i=0; i < NUM_AXES; i++)
		{
			x[i] = 0.0f;
			y[i] = 0.0f;
		}
	}

	void GamePadController::update(Ogre::Real seconds)
	{
		joystickUpdateCalledOnce = true;
		int x1 = mInputManager->getAxisValue(mControllerID, 3);
		int y1 = mInputManager->getAxisValue(mControllerID, 2);
		int x2 = mInputManager->getAxisValue(mControllerID, 1);
		int y2 = mInputManager->getAxisValue(mControllerID, 0);

		x[0] = fixAndRemapDeadzones(x1, mDeadZone);
		y[0] = -fixAndRemapDeadzones(y1, mDeadZone);

		x[1] = fixAndRemapDeadzones(x2, mDeadZone);
		y[1] = -fixAndRemapDeadzones(y2, mDeadZone);
	}

	Ogre::Real GamePadController::getX(int axis)
	{
		assert(joystickUpdateCalledOnce);
		return x[axis];
	}

	Ogre::Real GamePadController::getY(int axis)
	{
		assert(joystickUpdateCalledOnce);
		return y[axis];
	}

};
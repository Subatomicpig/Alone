#ifndef ALONE_GAMEPAD_CONTROLLER_H
#define ALONE_GAMEPAD_CONTROLLER_H

#include "Managers\InputManager.h"

namespace Alone
{
	static const int NUM_AXES = 2;

	class GamePadController
	{
	public:
		GamePadController(int id, int dead_zone);
		void update(Ogre::Real seconds);
		Ogre::Real getX(int axis);
		Ogre::Real getY(int axis);
	private:
		int mControllerID;
		InputManager* mInputManager;
		int mDeadZone;

		Ogre::Real x[NUM_AXES];
		Ogre::Real y[NUM_AXES];
	};

	typedef std::deque<GamePadController*> GamePadControllerList;
};
#endif
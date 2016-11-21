#include <OgreException.h>
#include <OgreString.h>

#include "Factories\WorkStateFactory.h"

namespace Alone
{
	WorkStateFactory* WorkStateFactory::mWorkStateFactory;

	WorkStateFactory::~WorkStateFactory()
	{
		TStateMap::iterator current = mRegisteredStates.begin();
		while(current != mRegisteredStates.end())
		{
			delete (*current).second;
			current++;
		}

	}

	void WorkStateFactory::registerState( Ogre::String state_name, WorkState *gameState )
	{
		mRegisteredStates[state_name.c_str()] = gameState;
	}

	WorkState* WorkStateFactory::getState( Ogre::String state_name )
	{
		TStateMap::iterator current = mRegisteredStates.find(state_name);
		if(current == mRegisteredStates.end())
		{
			Ogre::LogManager::getSingleton().logMessage(state_name + " not registered");
	
			Ogre::String error_string = "Error - [" + Ogre::String(state_name) + "] not a registered state";
			throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR,
				error_string,
				"Fatal Error" );
		}
		return (*current).second;
	}

	WorkStateFactory* WorkStateFactory::getSingletonPtr( void )
	{
		if( !mWorkStateFactory ) 
		{
			mWorkStateFactory = new WorkStateFactory();
		}

		return mWorkStateFactory;
	}
}
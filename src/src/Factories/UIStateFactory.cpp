#include <OgreException.h>
#include <OgreString.h>

#include "Factories\UIStateFactory.h"

namespace Alone
{
	UIStateFactory* UIStateFactory::mUIStateFactory;

	UIStateFactory::~UIStateFactory()
	{
		TUIStateMap::iterator current = mRegisteredStates.begin();
		while(current != mRegisteredStates.end())
		{
			delete (*current).second;
			current++;
		}

	}

	void UIStateFactory::registerState( Ogre::String state_name, UIState *gameState )
	{
		mRegisteredStates[state_name] = gameState;
	}

	UIState* UIStateFactory::getState( Ogre::String state_name )
	{
		TUIStateMap::iterator current = mRegisteredStates.find(state_name);
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

	UIStateFactory* UIStateFactory::getSingletonPtr( void )
	{
		if( !mUIStateFactory ) 
		{
			mUIStateFactory = new UIStateFactory();
		}

		return mUIStateFactory;
	}
}
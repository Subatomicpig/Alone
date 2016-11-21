#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>

#include "States\EmptyUIState.h"
#include "Managers\GameManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"
#include "Managers\InputManager.h"

using namespace Ogre;
using namespace Alone;

EmptyUIState* EmptyUIState::mEmptyUIState;

EmptyUIState::EmptyUIState( void ) 
{
	mClassName = "EmptyUI";
}

EmptyUIState::~EmptyUIState( void ) 
{
}

void EmptyUIState::enter( void ) 
{
	InputManager* input = InputManager::getSingletonPtr();
	input->addKeyListener(mEmptyUIState, mClassName);
}

void EmptyUIState::exit( void ) 
{
	InputManager* input = InputManager::getSingletonPtr();
	input->removeKeyListener(this);
}
 

void EmptyUIState::update( Ogre::Real seconds ) 
{
}

bool EmptyUIState::keyPressed( const OIS::KeyEvent &e ) 
{
	return true;
}

bool EmptyUIState::keyReleased( const OIS::KeyEvent &e ) 
{
	return true;
}

EmptyUIState* EmptyUIState::getSingletonPtr( void ) {
    if( !mEmptyUIState ) {
        mEmptyUIState = new EmptyUIState();
		UIStateFactory::getSingletonPtr()->registerState(mEmptyUIState->getName(), mEmptyUIState);
    }

    return mEmptyUIState;
}
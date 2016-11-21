#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>

#include "States\EmptyWorkState.h"
#include "Managers\GameManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"
#include "Managers\InputManager.h"

using namespace Ogre;
using namespace Alone;

EmptyWorkState* EmptyWorkState::mEmptyWorkState;

EmptyWorkState::EmptyWorkState( void ) 
{
	mClassName = "EmptyWork";
}

EmptyWorkState::~EmptyWorkState( void ) 
{
}

void EmptyWorkState::enter( void ) 
{
    mRoot         = Root::getSingletonPtr();
	mSceneManager     = mRoot->getSceneManager( "ST_GENERIC" );

    mCamera       = mSceneManager->createCamera( "EmptyWorkStateCamera" );
	mWindow		  = GameManager::getSingletonPtr()->getRenderWindow();

	mWindow->addViewport( mCamera );
}

void EmptyWorkState::exit( void ) 
{
	mSceneManager->destroyCamera(mCamera);
    mWindow->removeViewport(0);
}


void EmptyWorkState::update( Ogre::Real seconds ) 
{
}

void EmptyWorkState::do_focus( void )
{
}

void EmptyWorkState::do_defocus( void )
{
}

bool EmptyWorkState::keyPressed( const OIS::KeyEvent &e ) 
{
	return true;
}

bool EmptyWorkState::keyReleased( const OIS::KeyEvent &e ) 
{
	return true;
}

EmptyWorkState* EmptyWorkState::getSingletonPtr( void ) 
{
    if( !mEmptyWorkState ) 
	{
        mEmptyWorkState = new EmptyWorkState();
		WorkStateFactory::getSingletonPtr()->registerState(mEmptyWorkState->getName(), mEmptyWorkState);
    }

    return mEmptyWorkState;
}
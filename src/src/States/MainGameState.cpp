#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>

#include "States\MainGameState.h"
#include "Managers\GameManager.h"
#include "Managers\GlobalSettingsManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

using namespace Ogre;
using namespace Alone;

MainGameState* MainGameState::mMainGameState;

MainGameState::MainGameState( void ) 
{
	mClassName = "MainGameState";
}

MainGameState::~MainGameState( void ) 
{
}

void MainGameState::enter( void ) 
{
    mRoot         = Root::getSingletonPtr();
	mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );

    mCamera       = mSceneMgr->createCamera( "MainGameStateCamera" );
	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(1000);
	mCamera->setFOVy(Ogre::Radian(Ogre::Degree(45.0f)));
	mCamera->setPosition(0.0f, 0.0f, 500.0f);

	mWindow		  = GameManager::getSingletonPtr()->getRenderWindow();
	mWindow->addViewport( mCamera );

	Ogre::Entity* entity = NULL;
	Ogre::SceneNode* node = NULL;
   
	// Create entity + node then attached entity to node.
	entity = mSceneMgr->createEntity( "Entity1", "robot.mesh" );
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Node1" );
	node->translate(Ogre::Vector3( -200.0f, 0.0f, 0.0f ));
    node->attachObject( entity );
	node->showBoundingBox(true);
	mEntities.push_back(entity);
	mSceneNodes.push_back(node);

	//Environment* e = new Environment();
	// e->setup(mSceneMgr, "test");
}

void MainGameState::do_focus( void )
{
	mInputManager = InputManager::getSingletonPtr();
	mInputManager->addKeyListener(this, mClassName);
	mInputManager->addJoystickListener(this, mClassName);
}

void MainGameState::do_defocus( void )
{
	mInputManager->removeKeyListener(this);
	mInputManager->removeJoystickListener(this);
}

void MainGameState::exit( void ) 
{
	// delete environment;

	for(SceneNodeList::iterator it = mSceneNodes.begin(); it != mSceneNodes.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mSceneNodes.clear();

	for(EntityList::iterator it = mEntities.begin(); it != mEntities.end(); it++)
	{
		Ogre::SceneNode* parent = (*it)->getParentSceneNode();
		if(parent)
		{
			parent->detachAllObjects();
		}
		mSceneMgr->destroyEntity(*it);
	}
	mEntities.clear();

	mSceneMgr->destroyCamera(mCamera);
    mWindow->removeViewport(0);
}

void MainGameState::update( Ogre::Real seconds ) 
{
	mTime += seconds;

}

// Optional input handlers
bool MainGameState::keyPressed( const OIS::KeyEvent &e ) 
{
	return true;
}

bool MainGameState::keyReleased( const OIS::KeyEvent &e ) 
{
    if( e.key == OIS::KC_ESCAPE ) 
	{
     	GameManager::getSingletonPtr()->requestShutdown();
    }
    else if( e.key == OIS::KC_1 ) 
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
    }
	return true;
}

bool MainGameState::mouseMoved( const OIS::MouseEvent &e )
{
	return true;
}

bool MainGameState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	return true;
}

bool MainGameState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	return true;
}

bool MainGameState::povMoved( const OIS::JoyStickEvent &e, int pov )
{
	switch(e.state.mPOV[0].direction)
	{
		case (OIS::Pov::North):
		{
			break;
		}
		case (OIS::Pov::South):
		{
			break;
		}
		case (OIS::Pov::East):
		{
			mCamera->move(Ogre::Vector3(1,0,0));
			break;
		}
		case (OIS::Pov::West):
		{
			mCamera->move(Ogre::Vector3(-1,0,0));
			break;
		}
		case (OIS::Pov::NorthEast):
		{
			break;
		}
		case (OIS::Pov::SouthEast):
		{
			break;
		}
		case (OIS::Pov::NorthWest):
		{
			break;
		}
		case (OIS::Pov::SouthWest):
		{
			break;
		}
	}
	return true;
}

bool MainGameState::axisMoved( const OIS::JoyStickEvent &e, int axis )
{
	return true;
}

MainGameState* MainGameState::getSingletonPtr( void ) 
{
    if( !mMainGameState ) 
	{
        mMainGameState = new MainGameState();
		WorkStateFactory::getSingletonPtr()->registerState(mMainGameState->getName(), mMainGameState);
    }

    return mMainGameState;
}

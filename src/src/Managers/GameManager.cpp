#include "States\WorkState.h"
#include "States\UIState.h"
#include "Managers\GameManager.h"
#include "windows.h"
#include "winnt.h"
#include "winbase.h"
#include <OgreWindowEventUtilities.h>

using namespace Ogre;
using namespace Alone;

#define RESOURCE_CONFIG  "resources.cfg"
#define PLUGIN_CONFIG "plugins.cfg"

GameManager* GameManager::mGameManager;

GameManager::GameManager( void ) :
    mRoot( 0 ),
	mRenderWindow(NULL),
    bShutdown( false ),
	mMicrosecondWait(0.0f)
{
	mInputMgr = NULL;
	mSettingsMgr = NULL;
#ifdef USE_SOUNDS
	mSoundMgr = NULL;
#endif
}

GameManager::~GameManager( void ) 
{
    // Clean up all the states
    while( !mUIStates.empty() ) 
	{
        mUIStates.back()->exit();
        mUIStates.pop_back();
    }

	while( !mStates.empty() ) 
	{
		mStates.back()->defocus();
        mStates.back()->exit();
        mStates.pop_back();
    }

#ifdef USE_SOUNDS
	if(mSoundMgr)
	{
		delete mSoundMgr;
		mSoundMgr = NULL;
	}
#endif

    if( mInputMgr ) 
	{
        delete mInputMgr;
        mInputMgr = NULL;
    }

	if(mSettingsMgr)
	{
		delete mSettingsMgr;
		mSettingsMgr = NULL;
	}

    if( mRoot ) {
        delete mRoot;
        mRoot = 0;
    }
}

void GameManager::startup(WorkState *gameState, UIState* uiState)
{
	// Load default plugin.cfg, but ignore ogre.cfg
    mRoot = new Root(PLUGIN_CONFIG, "");

    // Setup settings manager
	LogManager::getSingleton().logMessage("Retrieving game configuration");
    mSettingsMgr = GlobalSettingsManager::getSingletonPtr();
	mSettingsMgr->parseConfigFiles();

	LogManager::getSingleton().logMessage("Starting game");

    // Setup and configure game
    this->setupResources();
    if( !this->configureGame() ) 
	{
        // If game can't be configured, throw exception and quit application
        throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR,
            "Error - Couldn't Configure Renderwindow",
            "Western Holdem Showdown - Error" );
        return;
    }

    // Setup input
	LogManager::getSingleton().logMessage("Initializing OIS input system");
    mInputMgr = InputManager::getSingletonPtr();
    mInputMgr->initialise( mRenderWindow );

#ifdef USE_SOUNDS
	mSoundMgr = new SoundManager;
	mSoundMgr->Initialize();
#endif

    // Change to first state
	LogManager::getSingleton().logMessage("Change into first game state");
    this->changeState( gameState );
    this->changeUIState( uiState );

    // lTimeLastFrame remembers the last time that it was checked
    // We use it to calculate the time since last frame
	resetClock();
	Ogre::Real seconds = 0.0f;

    // Main while-loop
    while( !bShutdown ) 
	{
        // Calculate time since last frame and remember current time for next frame
        mTimeCurrentFrame = mRoot->getTimer()->getMicroseconds();
        mTimeSinceLastFrame = mTimeCurrentFrame - mTimeLastFrame;

		if(mTimeSinceLastFrame > mMicrosecondWait)
		{
			mTimeLastFrame = mTimeCurrentFrame;

			// Update inputmanager
			mInputMgr->capture();

			// Update current state
			seconds = mTimeSinceLastFrame / 1000000.0f;
			mStates.back()->update( seconds );
			mUIStates.back()->update( seconds );

			// Render next frame
			mRoot->renderOneFrame();

			WindowEventUtilities::messagePump();
		}
    }
}

Ogre::RenderWindow* GameManager::createRenderWindow(const Ogre::String &name, unsigned int width, unsigned int height, 
			bool fullScreen, const Ogre::NameValuePairList *miscParams)
{
    mRoot = Root::getSingletonPtr();
	if(mRoot)
	{
		mRenderWindow = mRoot->createRenderWindow(name, width, height, fullScreen, miscParams);
		return mRenderWindow;
	}
	return NULL;
}



bool GameManager::configureGame( void ) 
{
	mRoot							= Root::getSingletonPtr();
	Ogre::SceneManager* sceneMgr	= mRoot->createSceneManager( ST_GENERIC, "ST_GENERIC" );

	Ogre::RenderSystemList *renderSystems = mRoot->getAvailableRenderers();
	for(Ogre::RenderSystemList::iterator r_it = renderSystems->begin(); r_it != renderSystems->end(); r_it++)
	{
		Ogre::String name = (*r_it)->getName();
		if(name.find(mSettingsMgr->getString("Renderer")) != Ogre::String::npos)
		{
			mRoot->setRenderSystem(*r_it);
		}
	}

	Ogre::NameValuePairList mGameOpts = mSettingsMgr->getGameOpts();
	Ogre::NameValuePairList mRenderOpts = mSettingsMgr->getGameOpts();

	Ogre::Real mFPS = mSettingsMgr->getFPS();
	mMicrosecondWait = 1000000.0f/mFPS;
	
	int width	= mSettingsMgr->getInteger("width");
	int height	= mSettingsMgr->getInteger("height");
	bool full_screen = false;
	int num_screens = false;
	num_screens = mSettingsMgr->getInteger("monitors");
	if(num_screens > 2)
	{
		num_screens = 2;
	}
	width *= num_screens;

	Ogre::String resolution = Ogre::StringConverter::toString(width) + "x" + Ogre::StringConverter::toString(height);
	mRenderOpts.insert(Ogre::NameValuePairList::value_type("resolution", resolution));
	if(mSettingsMgr->getBool("Full Screen"))
	{
		full_screen = true;
		mRenderOpts.insert(Ogre::NameValuePairList::value_type("fullscreen", "true"));
	}
	else
	{
		full_screen = false;
		mRenderOpts.insert(Ogre::NameValuePairList::value_type("fullscreen", "false"));
	}
	mRenderOpts.insert(Ogre::NameValuePairList::value_type("vsync", mGameOpts["vsync"]));
	if(mSettingsMgr->getString("Renderer") == "OpenGL")
	{
		mRenderOpts.insert(Ogre::NameValuePairList::value_type("displayFrequency", Ogre::StringConverter::toString(mFPS)));
		mRenderOpts.insert(Ogre::NameValuePairList::value_type("FSAA", mGameOpts["FSAA"]));
		mRenderOpts.insert(Ogre::NameValuePairList::value_type("RTT Preferred Mode", "FBO"));
	}
	else
	{
		mRenderOpts.insert(Ogre::NameValuePairList::value_type("FSAA", mGameOpts["FSAA"]));
		mRenderOpts.insert(Ogre::NameValuePairList::value_type("useNVPerfHUD", mGameOpts["useNVPerfHUD"]));
	}

	// Initialise and create a default rendering window
	mRoot->initialise( false, mGameOpts["Title"]);
	LogManager::getSingleton().logMessage("Creating first window");
	createRenderWindow(mGameOpts["Title"], width, height, full_screen, &mRenderOpts);

	LogManager::getSingleton().logMessage("--- Creating camera: DUMMY_CAMERA");
	Ogre::Camera* tempCam	 = sceneMgr->createCamera( "DUMMY_CAMERA" );
	int zOrder = 100;
	for(int i=0; i < num_screens; i++)
	{
		mRenderWindow->addViewport( tempCam, zOrder+i, float(i)/float(num_screens), 0.0f, 1.0f/float(num_screens), 1.0f );
	}

	// Initialise the rest of the resource groups, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Turn off rendering of everything except overlays
	sceneMgr->clearSpecialCaseRenderQueues();
	sceneMgr->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
	sceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);

	// Back to full rendering
	sceneMgr->clearSpecialCaseRenderQueues();
	sceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

	sceneMgr->destroyCamera(tempCam);

	// Remove all viewports
	for(int i=0; i < num_screens; i++)
	{
		mRenderWindow->removeViewport(zOrder + i);
	}
    return true;
}

void GameManager::setupResources( void ) 
{
    // Load resource paths from config file
    ConfigFile cf;
    cf.load( RESOURCE_CONFIG );

    // Go through all settings in the file
    ConfigFile::SectionIterator itSection = cf.getSectionIterator();

    String sSection, sType, sArch;
    while( itSection.hasMoreElements() ) 
	{
        sSection = itSection.peekNextKey();

        ConfigFile::SettingsMultiMap *mapSettings = itSection.getNext();
        ConfigFile::SettingsMultiMap::iterator itSetting = mapSettings->begin();
        while( itSetting != mapSettings->end() ) 
		{
            sType = itSetting->first;
            sArch = itSetting->second;
                            
            ResourceGroupManager::getSingleton().addResourceLocation(
                sArch, sType, sSection );

            ++itSetting;
        }
    }
}

void GameManager::changeState( WorkState *gameState ) 
{
	bool is_different = false;
    // Cleanup the current state
    if( !mStates.empty() ) 
	{
		if(mStates.back()->getName() != gameState->getName())
		{
			is_different = true;
			mStates.back()->defocus();
			mStates.back()->exit();
			mStates.pop_back();
		}
    }
	else
	{
		is_different = true;
	}

	if(is_different)
	{
		// Store and init the new state
		mStates.push_back( gameState );
		mStates.back()->enter();
		mStates.back()->focus();
		resetClock();
	}
}

void GameManager::pushState( WorkState *gameState ) 
{
    // Pause current state
    if( !mStates.empty() ) {
        mStates.back()->pause();
    }

    // Store and init the new state
    mStates.push_back( gameState );
    mStates.back()->enter();
	mStates.back()->focus();
}

void GameManager::popState() 
{
    // Cleanup the current state
    if( !mStates.empty() ) {
		mStates.back()->focus();
        mStates.back()->exit();
        mStates.pop_back();
    }

    // Resume previous state
    if( !mStates.empty() ) {
        mStates.back()->resume();
    }
}

void GameManager::changeUIState( UIState *gameUIState ) 
{
	bool is_different = false;
    // Cleanup the current state
    if( !mUIStates.empty() ) 
	{
		if(mUIStates.back()->getName() != gameUIState->getName())
		{
			is_different = true;
			mUIStates.back()->exit();
			mUIStates.pop_back();
		}
    }
	else
	{
		is_different = true;
	}

	if(is_different)
	{
		// Store and init the new state
		mUIStates.push_back( gameUIState );
		mUIStates.back()->enter();
		resetClock();
	}
}

void GameManager::pushUIState( UIState *gameUIState ) 
{
    // Pause current state
    if( !mUIStates.empty() ) {
        mUIStates.back()->pause();
    }

    // Store and init the new state
    mUIStates.push_back( gameUIState );
    mUIStates.back()->enter();
}

void GameManager::popUIState() 
{
    // Cleanup the current state
    if( !mUIStates.empty() ) {
        mUIStates.back()->exit();
        mUIStates.pop_back();
    }

    // Resume previous state
    if( !mUIStates.empty() ) {
        mUIStates.back()->resume();
    }
}
WorkState *GameManager::getWorkState()
{
	return mStates.back();
}

UIState *GameManager::getUIState()
{
	return mUIStates.back();
}

void GameManager::resetClock()
{
	mRoot->getTimer()->reset();
    mTimeLastFrame = 0;
    mTimeCurrentFrame = 0;
    mTimeSinceLastFrame = 0;
}

void GameManager::requestShutdown( void ) 
{
    bShutdown = true;
}

GameManager* GameManager::getSingletonPtr( void ) {
    if( !mGameManager ) {
        mGameManager = new GameManager();
    }

    return mGameManager;
}
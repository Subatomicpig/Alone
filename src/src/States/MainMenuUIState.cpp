#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>

#include "tinyxml.h"

#include "States\MainMenuUIState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"


using namespace Ogre;
using namespace Alone;

bool menuMusicSetting;
bool menuSFXSetting;
bool menuControls;
bool menuInvert;
bool exitConfirm = false;
bool confirmYes = false;

Alone::GamePadControllerList mGamePads;

MainMenuUIState* MainMenuUIState::mMainMenuUIState;

void MainMenuUIState::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

MainMenuUIState::MainMenuUIState( void ) 
{
	mClassName = "MainMenuUI";
}

MainMenuUIState::~MainMenuUIState( void ) 
{
}

void MainMenuUIState::enter( void ) 
{
	selectedOption = 0;
	stickReleased = true;
	buttonReleased = true;

	loadOptions();

	mOverlayMgr   = OverlayManager::getSingletonPtr();
    mMenuOverlay = mOverlayMgr->getByName( "Overlay/MainMenuUIState" );
	mCursorOverlay = mOverlayMgr->getByName("Overlay/MainMenuSelect");
	mMenuTextOverlay = mOverlayMgr->getByName("Overlay/MainMenuText");
	mMenuControlsOverlay = mOverlayMgr->getByName("Overlay/MainMenuControls");
	mExitConfirmOverlay = mOverlayMgr->getByName("Overlay/MainMenuExit");

	mMenuTextOverlay->setScale(0.25, 0.45);
	mMenuTextOverlay->setScroll(0, -0.37);

	mCursorOverlay->setScale(1, 0.80);

	moveCursor();

	mGamePads.push_back(new Alone::GamePadController(0, 4000));

	input = InputManager::getSingletonPtr();
	input->addKeyListener(mMainMenuUIState, mClassName);
	input->addJoystickListener(mMainMenuUIState, mClassName);

	GameManager::getSingletonPtr()->getWorkState()->defocus();

	channel1 = 0;
	channel2 = 0;
	channel3 = 0;

	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	audio_result = system->createStream("Sounds/Music/MainMenu.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	if(menuMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}

	audio_result = system->createSound("Sounds/SFX/Menu/Metal/Short Switch.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Movement);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Menu/Metal/Clang 2.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Selection);
	ERRCHECK(audio_result);
}

void MainMenuUIState::update( Ogre::Real seconds ) 
{
	mMenuOverlay->show();
	mCursorOverlay->show();
	mMenuTextOverlay->show();
	mMenuControlsOverlay->show();
	

	system->update();
	if(exitConfirm == false)
	{
		mExitConfirmOverlay->hide();
		moveJoystick(seconds);
		moveCursor();

		if(input->isButtonDown(0,1) && buttonReleased == true)
		{
			buttonReleased = false;

			switch(selectedOption)
			{
			case 0:
				GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("CharacterEditor"));
				GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
				break;
			case 1:
				GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
				GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("LoadGameUI"));
				break;
			case 2:
				GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
				GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("OptionsUI"));
				break;
			case 3:
				GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
				GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("CreditsUI"));
				break;
			case 4:
				GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("SampleWork5"));
				GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
				break;
			}
		}
		else if(!input->isButtonDown(0,1))
		{
			buttonReleased = true;
		}
	}
	else if(exitConfirm == true)
	{
		mExitConfirmOverlay->show();

		if(confirmYes)
		{
			GameManager::getSingletonPtr()->requestShutdown();
		}
	}
}

void MainMenuUIState::exit( void ) 
{
    mMenuOverlay->hide();
	mCursorOverlay->hide();
	mMenuTextOverlay->hide();
	mMenuControlsOverlay->hide();

	input->removeKeyListener(mMainMenuUIState);
	input->removeJoystickListener(mMainMenuUIState);

	audio_result = audio_track->release();
	ERRCHECK(audio_result);

	audio_result = cursor_Movement->release();
	ERRCHECK(audio_result);

	audio_result = cursor_Selection->release();
	ERRCHECK(audio_result);

	audio_result = system->close();
	ERRCHECK(audio_result);

	audio_result = system->release();
	ERRCHECK(audio_result);


	GameManager::getSingletonPtr()->getWorkState()->focus();
}

void MainMenuUIState::moveJoystick(Ogre::Real seconds)
{
	mGamePads[0]->update(seconds);

	Real yLeft = mGamePads[0]->getY(0);

	if(yLeft > 0.5f && stickReleased == true)
	{
		selectedOption--;

		if(selectedOption == -1)
		{
			selectedOption = 4;
		}

		stickReleased = false;
	}
	else if(yLeft < -0.5f && stickReleased == true)
	{
		selectedOption++;

		if(selectedOption == 5)
		{
			selectedOption = 0;
		}

		stickReleased = false;
	}
	else if(yLeft > -0.5f && yLeft < 0.5 && stickReleased == false)
	{
		stickReleased = true;
	}
}
void MainMenuUIState::moveCursor(void)
{
	switch(selectedOption)
	{
	case 0:
		mCursorOverlay->setScroll(0, -0.65);
		break;
	case 1:
		mCursorOverlay->setScroll(0, -0.855);
		break;
	case 2:
		mCursorOverlay->setScroll(0, -1.06);
		break;
	case 3:
		mCursorOverlay->setScroll(0, -1.27);
		break;
	case 4:
		mCursorOverlay->setScroll(0, -1.475);
		break;
	}
}
bool MainMenuUIState::keyPressed( const OIS::KeyEvent &e ) 
{
	if( e.key == OIS::KC_Q ) 
	{
		if(exitConfirm)
		{
			if(menuSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
				ERRCHECK(audio_result);
			}
     		confirmYes = true;
		}
    }

	if( e.key == OIS::KC_X ) 
	{
		if(menuSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
			ERRCHECK(audio_result);
		}
		if(!exitConfirm)
		{
     		exitConfirm = true;
		}
		else if(exitConfirm)
		{
     		exitConfirm = false;
		}
    }
	return true;
}

bool MainMenuUIState::keyReleased( const OIS::KeyEvent &e ) 
{
    if( e.key == OIS::KC_ESCAPE ) 
	{
     	GameManager::getSingletonPtr()->requestShutdown();
    }

	if(e.key == OIS::KC_DOWN)
	{
		if(menuSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		selectedOption++;

		if(selectedOption == 5)
		{
			selectedOption = 0;
		}
	}

	if(e.key == OIS::KC_UP)
	{
		if(menuSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		selectedOption--;

		if(selectedOption == -1)
		{
			selectedOption = 4;
		}
	}

	if(e.key == OIS::KC_RETURN)
	{
		if(menuSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
			ERRCHECK(audio_result);
		}

		switch(selectedOption)
		{
		case 0:
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("CharacterEditor"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
			break;
		case 1:
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("LoadGameUI"));
			break;
		case 2:
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("OptionsUI"));
			break;
		case 3:
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("CreditsUI"));
			break;
		case 4:
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("SampleWork5"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
			break;
		}
	}
  
	return true;
}

void MainMenuUIState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	menuMusicSetting = atoi(element->Attribute("Music"));
	menuSFXSetting = atoi(element->Attribute("SFX"));
	menuControls = atoi(element->Attribute("Keyboard"));
	menuInvert = atoi(element->Attribute("Invert"));
}

MainMenuUIState* MainMenuUIState::getSingletonPtr( void ) {
    if( !mMainMenuUIState ) {
        mMainMenuUIState = new MainMenuUIState();
		UIStateFactory::getSingletonPtr()->registerState(mMainMenuUIState->getName(), mMainMenuUIState);
    }

    return mMainMenuUIState;
}
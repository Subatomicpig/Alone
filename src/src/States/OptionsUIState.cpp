#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>

#include "tinyxml.h"

#include "States\OptionsUIState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

using namespace Ogre;
using namespace Alone;

bool optionsMusicSetting;
bool optionsSFXSetting;
bool optionsControls;
bool optionsInvert;

Alone::GamePadControllerList mOptionsGamePad;
OptionsUIState* OptionsUIState::mOptionsUIState;

void OptionsUIState::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

OptionsUIState::OptionsUIState(void)
{
	mClassName = "OptionsUI";
}

OptionsUIState::~OptionsUIState(void)
{
}

void OptionsUIState::enter(void)
{
	selectedOption = 0;
	buttonReleased = false;
	stickReleased = true;

	optionsMusicSetting = true;
	optionsSFXSetting = true;
	optionsControls = true;
	optionsInvert = true;

	loadOptions();

	mOverlayMgr = OverlayManager::getSingletonPtr();

	mOptionsOverlay = mOverlayMgr->getByName("Overlay/OptionsUIState");
	
	mOptionsTextOverlay = mOverlayMgr->getByName("Overlay/OptionsText");
	mOptionsTextOverlay->setScroll(0.63, -0.92);
	
	mOptionsControlsOverlay = mOverlayMgr->getByName("Overlay/OptionsControls");

	mMusicOnOverlay = mOverlayMgr->getByName("Overlay/MusicOn");
	mMusicOnOverlay->setScale(1, 0.80);
	mMusicOnOverlay->setScroll(1.23, -0.72);

	mMusicOffOverlay = mOverlayMgr->getByName("Overlay/MusicOff");
	mMusicOffOverlay->setScale(1, 0.80);
	mMusicOffOverlay->setScroll(1.23, -0.72);

	mSFXOnOverlay = mOverlayMgr->getByName("Overlay/SFXOn");
	mSFXOnOverlay->setScale(1, 0.80);
	mSFXOnOverlay->setScroll(1.23, -0.925);
	
	mSFXOffOverlay = mOverlayMgr->getByName("Overlay/SFXOff");
	mSFXOffOverlay->setScale(1, 0.80);
	mSFXOffOverlay->setScroll(1.23, -0.925);

	mKeyboardOverlay = mOverlayMgr->getByName("Overlay/Keyboard");
	mKeyboardOverlay->setScale(1, 0.80);
	mKeyboardOverlay->setScroll(1.18, -1.13);

	mGamepadOverlay = mOverlayMgr->getByName("Overlay/Gamepad");
	mGamepadOverlay->setScale(1, 0.80);
	mGamepadOverlay->setScroll(1.18, -1.13);

	mInvertOnOverlay = mOverlayMgr->getByName("Overlay/InvertOn");
	mInvertOnOverlay->setScale(1, 0.80);
	mInvertOnOverlay->setScroll(1.23, -1.335);

	mInvertOffOverlay = mOverlayMgr->getByName("Overlay/InvertOff");
	mInvertOffOverlay->setScale(1, 0.80);
	mInvertOffOverlay->setScroll(1.23, -1.335);

	mOptionsSelectorOverlay = mOverlayMgr->getByName("Overlay/OptionsSelector");
	mOptionsSelectorOverlay->setScale(1, 0.80);
	
	mOptionsGamePad.push_back(new Alone::GamePadController(0, 4000));

	input = InputManager::getSingletonPtr();
	input->addKeyListener(mOptionsUIState, mClassName);
	input->addJoystickListener(mOptionsUIState, mClassName);

	GameManager::getSingletonPtr()->getWorkState()->defocus();

	moveCursor();
	loadSettings();

	channel1 = 0;
	channel2 = 0;
	channel3 = 0;

	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	audio_result = system->createStream("Sounds/Music/MainMenu.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	if(optionsMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}

	audio_result = system->createSound("Sounds/SFX/Menu/Metal/Short Switch.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Movement);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Menu/Metal/Clang 2.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Selection);
	ERRCHECK(audio_result);
}

void OptionsUIState::update(Ogre::Real seconds)
{
	mOptionsOverlay->show();
	mOptionsTextOverlay->show();
	mOptionsSelectorOverlay->show();
	mOptionsControlsOverlay->show();

	moveJoystick(seconds);
	moveCursor();

	system->update();

	if(optionsMusicSetting == 1)
	{
		mMusicOnOverlay->show();
		mMusicOffOverlay->hide();
	}
	else
	{
		mMusicOffOverlay->show();
		mMusicOnOverlay->hide();
	}

	if(optionsSFXSetting == 1)
	{
		mSFXOnOverlay->show();
		mSFXOffOverlay->hide();
	}
	else
	{
		mSFXOnOverlay->hide();
		mSFXOffOverlay->show();
	}

	if(optionsControls == 1)
	{
		mKeyboardOverlay->show();
		mGamepadOverlay->hide();
	}
	else
	{
		mKeyboardOverlay->hide();
		mGamepadOverlay->show();
	}

	if(optionsInvert == 1)
	{
		mInvertOnOverlay->show();
		mInvertOffOverlay->hide();
	}
	else
	{
		mInvertOnOverlay->hide();
		mInvertOffOverlay->show();
	}

	if(input->isButtonDown(0,2))
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
	}
}

void OptionsUIState::exit(void)
{
	mOptionsOverlay->hide();
	mOptionsTextOverlay->hide();
	mOptionsSelectorOverlay->hide();
	mOptionsControlsOverlay->hide();

	mMusicOnOverlay->hide();
	mMusicOffOverlay->hide();

	mSFXOnOverlay->hide();
	mSFXOffOverlay->hide();

	mKeyboardOverlay->hide();
	mGamepadOverlay->hide();

	mInvertOnOverlay->hide();
	mInvertOffOverlay->hide();

	GameManager::getSingletonPtr()->getWorkState()->focus();
	
	InputManager* input = InputManager::getSingletonPtr();
	input->removeKeyListener(mOptionsUIState);
	input->removeJoystickListener(mOptionsUIState);

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

	//Save Options
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Options");
	doc.LinkEndChild(root);

	TiXmlElement *settings = new TiXmlElement("Settings");
	root->LinkEndChild(settings);
	settings->SetAttribute("Music", optionsMusicSetting);
	settings->SetAttribute("SFX", optionsSFXSetting);
	settings->SetAttribute("Keyboard", optionsControls);
	settings->SetAttribute("Invert", optionsInvert);

	doc.SaveFile("Options.xml");
}

void OptionsUIState::loadSettings(void)
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");
	musicSetting = atoi(element->Attribute("Music"));
	SFXSetting = atoi(element->Attribute("SFX"));
	controls = atoi(element->Attribute("Keyboard"));
	invert = atoi(element->Attribute("Invert"));
}
void OptionsUIState::moveCursor(void)
{
	switch(selectedOption)
	{
	case 0:
		mOptionsSelectorOverlay->setScroll(0, -0.65);
		break;
	case 1:
		mOptionsSelectorOverlay->setScroll(0, -0.855);
		break;
	case 2:
		mOptionsSelectorOverlay->setScroll(0, -1.06);
		break;
	case 3:
		mOptionsSelectorOverlay->setScroll(0, -1.27);
		break;
	}
}

void OptionsUIState::moveJoystick(Ogre::Real seconds)
{
	mOptionsGamePad[0]->update(seconds);

	Real yLeft = mOptionsGamePad[0]->getY(0);

	if(yLeft < -0.5f && stickReleased == true)
	{
		selectedOption++;

		if(selectedOption == 4)
		{
			selectedOption = 0;
		}

		stickReleased = false;
	}
	else if(yLeft > 0.5f && stickReleased == true)
	{
		selectedOption--;

		if(selectedOption == -1)
		{
			selectedOption = 3;
		}

		stickReleased = false;
	}
	else if(yLeft > -0.5f && yLeft < 0.5f && stickReleased == false)
	{
		stickReleased = true;
	}

	if(input->isButtonDown(0,1) && buttonReleased == true)
	{
		switch(selectedOption)
		{
		case 0:
			if(optionsMusicSetting == 1)
			{
				optionsMusicSetting = 0;
			}
			else if(optionsMusicSetting == 0)
			{
				optionsMusicSetting = 1;
			}
			break;
		case 1:
			if(optionsSFXSetting == 1)
			{
				optionsSFXSetting = 0;
			}
			else if(optionsSFXSetting == 0)
			{
				optionsSFXSetting = 1;
			}
			break;
		case 2:
			if(optionsControls == 1)
			{
				optionsControls = 0;
			}
			else if(optionsControls == 0)
			{
				optionsControls = 1;
			}
			break;
		case 3:
			if(optionsInvert == 1)
			{
				optionsInvert = 0;
			}
			else if(optionsInvert == 0)
			{
				optionsInvert = 1;
			}
			break;
		}
		buttonReleased = false;
	}
	else if(!input->isButtonDown(0,1))
	{
		buttonReleased = true;
	}
}

bool OptionsUIState::keyPressed(const OIS::KeyEvent &e)
{
	return true;
}

bool OptionsUIState::keyReleased(const OIS::KeyEvent &e)
{
	if(e.key == OIS::KC_ESCAPE)
	{
		GameManager::getSingletonPtr()->requestShutdown();
	}
	else if(e.key == OIS::KC_X)
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
	}

	if(e.key == OIS::KC_RIGHT || e.key == OIS::KC_LEFT)
	{
		if(optionsSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
			ERRCHECK(audio_result);
		}

		switch(selectedOption)
		{
		case 0:
			if(optionsMusicSetting == 1)
			{
				optionsMusicSetting = 0;
			}
			else if(optionsMusicSetting == 0)
			{
				optionsMusicSetting = 1;
			}
			break;
		case 1:
			if(optionsSFXSetting == 1)
			{
				optionsSFXSetting = 0;
			}
			else if(optionsSFXSetting == 0)
			{
				optionsSFXSetting = 1;
			}
			break;
		case 2:
			if(optionsControls == 1)
			{
				optionsControls = 0;
			}
			else if(optionsControls == 0)
			{
				optionsControls = 1;
			}
			break;
		case 3:
			if(optionsInvert == 1)
			{
				optionsInvert = 0;
			}
			else if(optionsInvert == 0)
			{
				optionsInvert = 1;
			}
			break;
		}
	}

	if(e.key == OIS::KC_DOWN)
	{
		selectedOption++;

		if(optionsSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		if(selectedOption == 4)
		{
			selectedOption = 0;
		}
	}

	if(e.key == OIS::KC_UP)
	{
		selectedOption--;

		if(optionsSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		if(selectedOption == -1)
		{
			selectedOption = 3;
		}
	}

	return true;
}

void OptionsUIState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	optionsMusicSetting = atoi(element->Attribute("Music"));
	optionsSFXSetting = atoi(element->Attribute("SFX"));
	optionsControls = atoi(element->Attribute("Keyboard"));
	optionsInvert = atoi(element->Attribute("Invert"));
}

OptionsUIState* OptionsUIState::getSingletonPtr(void)
{
	if(!mOptionsUIState)
	{
		mOptionsUIState = new OptionsUIState();
		UIStateFactory::getSingletonPtr()->registerState(mOptionsUIState->getName(), mOptionsUIState);
	}

	return mOptionsUIState;
}
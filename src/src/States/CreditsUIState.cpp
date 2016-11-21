#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>

#include "tinyxml.h"

#include "States\CreditsUIState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

using namespace Ogre;
using namespace Alone;

bool creditsMusicSetting;
bool creditsSFXSetting;
bool creditsControls;
bool creditsInvert;

CreditsUIState* CreditsUIState::mCreditsUIState;

void CreditsUIState::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

CreditsUIState::CreditsUIState(void)
{
	mClassName = "CreditsUI";
}

CreditsUIState::~CreditsUIState(void)
{
}

void CreditsUIState::enter(void)
{
	scrollSpeed = 0.001;
	xPos = 0.0;
	yPos = -0.25;

	loadOptions();

	mOverlayMgr		= OverlayManager::getSingletonPtr();
	mCreditsOverlay = mOverlayMgr->getByName("Overlay/CreditsUIState");
	mCreditsOverlay->setScroll(xPos, yPos);
	mCreditsOverlay->show();

	input = InputManager::getSingletonPtr();
	input->addKeyListener(mCreditsUIState, mClassName);
	input->addJoystickListener(mCreditsUIState, mClassName);

	GameManager::getSingletonPtr()->getWorkState()->defocus();

	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(2, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	audio_result = system->createStream("Sounds/Music/Credits.mp3", FMOD_HARDWARE | FMOD_LOOP_NORMAL, 0, &audio_track);
	ERRCHECK(audio_result);

	if(creditsMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}
}

void CreditsUIState::update(Ogre::Real seconds)
{
	yPos += scrollSpeed;

	mCreditsOverlay->setScroll(xPos, yPos);

	if(input->isButtonDown(0,2))
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
	}
}

void CreditsUIState::exit(void)
{
	mCreditsOverlay->hide();
	GameManager::getSingletonPtr()->getWorkState()->focus();

	input->removeKeyListener(mCreditsUIState);
	input->removeJoystickListener(mCreditsUIState);

	channel1->stop();
	channel2->stop();

	audio_result = audio_track->release();
	ERRCHECK(audio_result);

	audio_result = system->release();
	ERRCHECK(audio_result);

	audio_result = system->close();
	ERRCHECK(audio_result);
}

bool CreditsUIState::keyPressed(const OIS::KeyEvent &e)
{
	return true;
}

bool CreditsUIState::keyReleased(const OIS::KeyEvent &e)
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
	return true;
}

void CreditsUIState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	creditsMusicSetting = atoi(element->Attribute("Music"));
	creditsSFXSetting = atoi(element->Attribute("SFX"));
	creditsControls = atoi(element->Attribute("Keyboard"));
	creditsInvert = atoi(element->Attribute("Invert"));
}

CreditsUIState* CreditsUIState::getSingletonPtr(void)
{
	if(!mCreditsUIState)
	{
		mCreditsUIState = new CreditsUIState();
		UIStateFactory::getSingletonPtr()->registerState(mCreditsUIState->getName(), mCreditsUIState);
	}

	return mCreditsUIState;
}
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "tinyxml.h"

#include "States\AchievementsUIState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

using namespace Ogre;
using namespace Alone;

bool achievementsMusicSetting;
bool achievementsSFXSetting;
bool achievementsControls;
bool achievementsInvert;

AchievementsUIState* AchievementsUIState::mAchievementsUIState;

AchievementsUIState::AchievementsUIState(void)
{
	mClassName = "AchievementsUI";
}

AchievementsUIState::~AchievementsUIState(void)
{
}

void AchievementsUIState::enter(void)
{
	loadOptions();

	mOverlayMgr = OverlayManager::getSingletonPtr();
}

void AchievementsUIState::update(Ogre::Real seconds)
{
}

void AchievementsUIState::exit(void)
{
}

bool AchievementsUIState::keyPressed(const OIS::KeyEvent &e)
{
	return true;
}

bool AchievementsUIState::keyReleased(const OIS::KeyEvent &e)
{
	if(e.key == OIS::KC_ESCAPE)
	{
		GameManager::getSingletonPtr()->requestShutdown();
	}

	if(e.key == OIS::KC_X)
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
	}

	return true;
}

void AchievementsUIState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	achievementsMusicSetting = atoi(element->Attribute("Music"));
	achievementsSFXSetting = atoi(element->Attribute("SFX"));
	achievementsControls = atoi(element->Attribute("Keyboard"));
	achievementsInvert = atoi(element->Attribute("Invert"));
}

AchievementsUIState* AchievementsUIState::getSingletonPtr(void)
{
	if(!mAchievementsUIState)
	{
		mAchievementsUIState = new AchievementsUIState();
		UIStateFactory::getSingletonPtr()->registerState(mAchievementsUIState->getName(), mAchievementsUIState);
	}
	return mAchievementsUIState;
}
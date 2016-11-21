/* ----------------------------------------------------------------------------
Copyright (c) 2007 Nelson Yu

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
---------------------------------------------------------------------------- */

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>

#include "States\HighScoresUIState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

using namespace Ogre;
using namespace Musicala;

HighScoresUIState* HighScoresUIState::mHighScoresUIState;

HighScoresUIState::HighScoresUIState(void)
{
	mClassName = "HighScoresUI";
}

HighScoresUIState::~HighScoresUIState(void)
{
}

void HighScoresUIState::enter(void)
{
	mOverlayMgr = OverlayManager::getSingletonPtr();
	
	mHighScoresPanel = static_cast<Ogre::OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "HighScoresPanel"));
	mHighScoresPanel->setMetricsMode(Ogre::GMM_PIXELS);
	mHighScoresPanel->setPosition(0,0);
	mHighScoresPanel->setDimensions(200,480);

	mFirstNameTextBox = static_cast<Ogre::TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "FirstPlaceName"));
	mFirstNameTextBox->setDimensions(50,50);
	mFirstNameTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mFirstNameTextBox->setPosition(325,0);
	mFirstNameTextBox->setFontName("MyFont");
	mFirstNameTextBox->setCharHeight(16);
	mFirstNameTextBox->setColour(Ogre::ColourValue::Black);

	mSecondNameTextBox = static_cast<Ogre::TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "SecondPlaceName"));
	mSecondNameTextBox->setDimensions(50,50);
	mSecondNameTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSecondNameTextBox->setPosition(325,0);
	mSecondNameTextBox->setFontName("MyFont");
	mSecondNameTextBox->setCharHeight(16);
	mSecondNameTextBox->setColour(Ogre::ColourValue::Black);

	mFirstScoreTextBox = static_cast<Ogre::TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "FirstPlaceScore"));
	mFirstScoreTextBox->setDimensions(50,50);
	mFirstScoreTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mFirstScoreTextBox->setPosition(425,0);
	mFirstScoreTextBox->setFontName("MyFont");
	mFirstScoreTextBox->setCharHeight(16);
	mFirstScoreTextBox->setColour(Ogre::ColourValue::Black);

	mSecondScoreTextBox = static_cast<Ogre::TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "SecondPlaceScore"));
	mSecondScoreTextBox->setDimensions(50,50);
	mSecondScoreTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSecondScoreTextBox->setPosition(425,0);
	mSecondScoreTextBox->setFontName("MyFont");
	mSecondScoreTextBox->setCharHeight(16);
	mSecondScoreTextBox->setColour(Ogre::ColourValue::Black);
}

void HighScoresUIState::update(Ogre::Real seconds)
{
}

void HighScoresUIState::exit(void)
{
}

bool HighScoresUIState::keyPressed(const OIS::KeyEvent &e)
{
	return true;
}

bool HighScoresUIState::keyReleased(const OIS::KeyEvent &e)
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

HighScoresUIState* HighScoresUIState::getSingletonPtr(void)
{
	if(!mHighScoresUIState)
	{
		mHighScoresUIState = new HighScoresUIState();
		UIStateFactory::getSingletonPtr()->registerState(mHighScoresUIState->getName(), mHighScoresUIState);
	}

	return mHighScoresUIState;
}
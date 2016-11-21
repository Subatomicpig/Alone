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

#ifndef HIGH_SCORES_UI_STATE_H
#define HIGH_SCROES_UI_STATE_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>

#include "States\UIState.h"
#include "Managers\InputManager.h"
#include "Controllers\GamePadController.h"

class HighScoresUIState:public Musicala::UIState
{
public:
	~HighScoresUIState(void);

	void enter(void);
	void update(Ogre::Real seconds);
	void exit(void);

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	static HighScoresUIState* getSingletonPtr(void);

private:
	HighScoresUIState(void);

	HighScoresUIState(const HighScoresUIState&) {}
	HighScoresUIState & operator = (const HighScoresUIState&);

	Ogre::OverlayManager			*mOverlayMgr;

	Ogre::OverlayContainer			*mHighScoresPanel;

	Ogre::Overlay					*mHighScoresOverlay;
	Ogre::Overlay					*mDebugOverlay;

	Ogre::OverlayElement			*mBottomLeft;
	Ogre::OverlayElement			*mBottomRight;

	Ogre::TextAreaOverlayElement	*mFirstNameTextBox;
	Ogre::TextAreaOverlayElement	*mSecondNameTextBox;
	Ogre::TextAreaOverlayElement	*mThirdNameTextBox;
	Ogre::TextAreaOverlayElement	*mFourthNameTextBox;
	Ogre::TextAreaOverlayElement	*mFifthNameTextBox;
	Ogre::TextAreaOverlayElement	*mSixthNameTextBox;
	Ogre::TextAreaOverlayElement	*mSeventhNameTextBox;
	Ogre::TextAreaOverlayElement	*mEightNameTextBox;
	Ogre::TextAreaOverlayElement	*mNinthNameTextBox;
	Ogre::TextAreaOverlayElement	*mTenthNameTextBox;

	Ogre::TextAreaOverlayElement	*mFirstScoreTextBox;
	Ogre::TextAreaOverlayElement	*mSecondScoreTextBox;
	Ogre::TextAreaOverlayElement	*mThirdScoreTextBox;
	Ogre::TextAreaOverlayElement	*mFourthScoreTextBox;
	Ogre::TextAreaOverlayElement	*mFifthScoreTextBox;
	Ogre::TextAreaOverlayElement	*mSixthScoreTextBox;
	Ogre::TextAreaOverlayElement	*mSeventhScoreTextBox;
	Ogre::TextAreaOverlayElement	*mEightScoreTextBox;
	Ogre::TextAreaOverlayElement	*mNinthScoreTextBox;
	Ogre::TextAreaOverlayElement	*mTenthScoreTextBox;


	static HighScoresUIState *mHighScoresUIState;
};
#endif
#ifndef CREDITS_UI_STATE_H
#define CREDITS_UI_STATE_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "States\UIState.h"
#include "Managers\InputManager.h"
#include "Controllers\GamePadController.h"

#include <stdio.h>
#include "fmod.hpp"
#include "fmod_errors.h"

class CreditsUIState:public Alone::UIState
{
public:
	~CreditsUIState(void);

	void enter(void);
	void update(Ogre::Real seconds);
	void exit(void);

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	void ERRCHECK(FMOD_RESULT result);

	void loadOptions( void );

	static CreditsUIState* getSingletonPtr(void);

private:
	CreditsUIState(void);

	CreditsUIState(const CreditsUIState&) {}
	CreditsUIState & operator = (const CreditsUIState&);

	Ogre::OverlayManager		*mOverlayMgr;
	Ogre::Overlay				*mCreditsOverlay;
	Ogre::Overlay				*mDebugOverlay;
	Ogre::OverlayElement		*mBottomLeft;
	Ogre::OverlayElement		*mBottomRight;

	float scrollSpeed, xPos, yPos;

	static CreditsUIState		*mCreditsUIState;
	InputManager* input;

	FMOD::System *system;
	FMOD::Channel *channel1;
	FMOD::Channel *channel2;

	FMOD::Sound *audio_track;
	FMOD_RESULT audio_result;
};
#endif
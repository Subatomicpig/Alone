#ifndef OPTIONS_UI_STATE_H
#define OPTIONS_UI_STATE_H

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

#include <stdio.h>
#include "fmod.hpp"
#include "fmod_errors.h"

class OptionsUIState:public Alone::UIState
{
public:
	~OptionsUIState(void);

	void enter(void);
	void update(Ogre::Real seconds);
	void exit(void);

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);
	
	void loadSettings(void);
	void moveJoystick(Ogre::Real seconds);
	void moveCursor(void);

	void loadOptions( void );

	void ERRCHECK(FMOD_RESULT result);

	static OptionsUIState* getSingletonPtr(void);

private:
	OptionsUIState(void);

	OptionsUIState(const OptionsUIState&) {}
	OptionsUIState & operator = (const OptionsUIState&);

	Ogre::OverlayManager			*mOverlayMgr;
	
	Ogre::Overlay					*mOptionsOverlay;
	Ogre::Overlay					*mOptionsTextOverlay;
	Ogre::Overlay					*mOptionsSelectorOverlay;

	Ogre::Overlay					*mOptionsControlsOverlay;

	Ogre::Overlay					*mMusicOnOverlay;
	Ogre::Overlay					*mMusicOffOverlay;

	Ogre::Overlay					*mSFXOnOverlay;
	Ogre::Overlay					*mSFXOffOverlay;

	Ogre::Overlay					*mKeyboardOverlay;
	Ogre::Overlay					*mGamepadOverlay;

	Ogre::Overlay					*mInvertOnOverlay;
	Ogre::Overlay					*mInvertOffOverlay;

	Ogre::Overlay					*mDebugOverlay;

	Ogre::OverlayElement			*mBottomLeft;
	Ogre::OverlayElement			*mBottomRight;

	int musicSetting, SFXSetting, controls, invert;

	static OptionsUIState			*mOptionsUIState;

	int selectedOption;
	bool stickReleased, buttonReleased;

	InputManager* input;

	FMOD::System *system;
	FMOD::Channel *channel1;
	FMOD::Channel *channel2;
	FMOD::Channel *channel3;

	FMOD::Sound *audio_track;
	FMOD::Sound *cursor_Movement;
	FMOD::Sound *cursor_Selection;
	FMOD_RESULT audio_result;
};
#endif
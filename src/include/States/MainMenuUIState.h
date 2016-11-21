#ifndef MAIN_MENU_UI_STATE_H
#define MAIN_MENU_UI_STATE_H

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

class MainMenuUIState : public Alone::UIState 
{
public:
    ~MainMenuUIState( void );

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

	void moveJoystick(Ogre::Real seconds);
	void moveCursor(void);
	void loadOptions( void );

	void ERRCHECK(FMOD_RESULT result);

    static MainMenuUIState* getSingletonPtr( void );

private:
	MainMenuUIState( void );
    MainMenuUIState( const MainMenuUIState& ) { }
    MainMenuUIState & operator = ( const MainMenuUIState& );

    Ogre::OverlayManager *mOverlayMgr;
    Ogre::Overlay        *mMenuOverlay;
	Ogre::Overlay		 *mCursorOverlay;
	Ogre::Overlay		 *mMenuTextOverlay;
	Ogre::Overlay		 *mMenuControlsOverlay;
	Ogre::Overlay		 *mExitConfirmOverlay;
    Ogre::Overlay        *mDebugOverlay;
	Ogre::OverlayElement *mBottomLeft;
	Ogre::OverlayElement *mBottomRight;

    static MainMenuUIState    *mMainMenuUIState;
	InputManager* input;

	int selectedOption;
	bool stickReleased;
	bool buttonReleased;

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
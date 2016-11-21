#ifndef STORE_STATE_H
#define STORE_STATE_H

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

class StoreState : public Alone::UIState 
{
public:
    ~StoreState( void );

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

	void moveJoystick(Ogre::Real seconds);
	void moveCursor(void);
	void loadOptions( void );
	void loadMerchandise( void );
	void updateMerchandise( void );

	void ERRCHECK(FMOD_RESULT result);

    static StoreState* getSingletonPtr( void );

private:
	StoreState( void );
    StoreState( const StoreState& ) { }
    StoreState & operator = ( const StoreState& );

    Ogre::OverlayManager *storeOverlayMgr;
	Ogre::OverlayContainer *storePanel;
    Ogre::Overlay        *storeOverlay;
	Ogre::Overlay		 *storeCursorOverlay;
	Ogre::Overlay		 *currentInfoOverlay;
	Ogre::Overlay		 *syringeImage;
	Ogre::Overlay		 *painkillersImage;
    Ogre::Overlay        *trainingDebugOverlay;
	Ogre::TextAreaOverlayElement    *exitStoreText;
	Ogre::TextAreaOverlayElement    *moneyText;
	Ogre::TextAreaOverlayElement    *steroidText;
	Ogre::TextAreaOverlayElement    *painkillerText;
	Ogre::TextAreaOverlayElement    *descriptionText;
	Ogre::OverlayElement *storeBottomLeft;
	Ogre::OverlayElement *storeBottomRight;

    static StoreState    *mStoreState;
	InputManager* input;

	int selectedOption;
	bool stickReleased;

	FMOD::System *system;
	FMOD::Channel *channel1;
	FMOD::Channel *channel2;
	FMOD::Channel *channel3;

	FMOD::Sound *audio_track;
	FMOD::Sound *cursor_Movement;
	FMOD::Sound *cursor_Selection;
	FMOD::Sound *cursor_SelectionSyringe;
	FMOD_RESULT audio_result;
};
#endif
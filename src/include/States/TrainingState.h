#ifndef TRAINING_STATE_H
#define TRAINING_STATE_H

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

class TrainingState : public Alone::UIState 
{
public:
    ~TrainingState( void );

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

	void moveJoystick(Ogre::Real seconds);
	void moveCursor(void);
	void loadOptions( void );
	void loadStats( void );
	void updateText( void );
	void updateStats( void );
	void loadMerchandise( void );
	void updateMerchandise( void );

	void ERRCHECK(FMOD_RESULT result);

    static TrainingState* getSingletonPtr( void );

private:
	TrainingState( void );
    TrainingState( const TrainingState& ) { }
    TrainingState & operator = ( const TrainingState& );

    Ogre::OverlayManager *trainingOverlayMgr;
	Ogre::OverlayContainer *workoutPanel;
    Ogre::Overlay        *trainingOverlay;
	Ogre::Overlay		 *trainingCursorOverlay;
	Ogre::Overlay		 *trainingPicture1;
	Ogre::Overlay		 *trainingPicture2;
	Ogre::Overlay		 *trainingPicture3;
	Ogre::Overlay		 *trainingPicture4;
	Ogre::Overlay		 *statsOverlay;
    Ogre::Overlay        *trainingDebugOverlay;
	Ogre::TextAreaOverlayElement    *levelText;
	Ogre::TextAreaOverlayElement    *steroidsText;
	Ogre::TextAreaOverlayElement    *upperBodyText;
	Ogre::TextAreaOverlayElement    *lowerBodyText;
	Ogre::TextAreaOverlayElement    *healthText;
	Ogre::TextAreaOverlayElement    *grappleText;
	Ogre::OverlayElement *trainingBottomLeft;
	Ogre::OverlayElement *trainingBottomRight;

    static TrainingState    *mTrainingState;
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
	FMOD_RESULT audio_result;
};
#endif
#ifndef SAMPLE_STATE5_H
#define SAMPLE_STATE5_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreFontManager.h> 
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>

#include "States\WorkState.h"
#include "Managers\InputManager.h"
#include "Controllers\GamePadController.h"
#include "CommonOgreTypes.h"

#include <stdio.h>
#include "fmod.hpp"
#include "fmod_errors.h"

class SampleState5 : public Alone::WorkState 
{
public:
    ~SampleState5( void );

    Ogre::OverlayManager *combatOverlayMgr;
	Ogre::Overlay		 *combatPowerMeterOverlay;
	Ogre::Overlay		 *combatPowerBar1Overlay;
	Ogre::Overlay		 *combatPowerBar2Overlay;
	Ogre::Overlay		 *combatPowerBar3Overlay;
    Ogre::Overlay        *combatPowerBar4Overlay;
    Ogre::Overlay        *combatPowerBar5Overlay;
	Ogre::OverlayElement *mBottomLeft;
	Ogre::OverlayElement *mBottomRight;

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );
	void initHUD( void );

	void do_focus( void );
	void do_defocus( void );

	void moveAIForward( Ogre::SceneNode *node );
	void moveAIBackward( Ogre::SceneNode *node );
	void moveAILeft(Ogre::SceneNode *node);
	void moveAIRight(Ogre::SceneNode *node);

	bool checkPlayerOpponentCollision(void);

	void npcFacePlayer( Ogre::SceneNode* node, Ogre::Camera* Camera );
	
	void updateAI1( void );

	void increasePower( void );
	void wait(int seconds );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

	bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
	bool buttonReleased( const OIS::JoyStickEvent &arg, int button );

		//char collision functions//
	bool forwardCollisionCheckS5( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode );
	bool backwardCollisionCheckS5( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode );
	bool leftCollisionCheckS5( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode );
	bool rightCollisionCheckS5( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode );

    static SampleState5* getSingletonPtr( void );
	void loadOptions( void );
	void gamepadCharacterMovement( Ogre::Real seconds );
	void keyboardMouseCharacterMovement( void );
	void checkCollision( void );

	void loadCharacter( void );

	void ERRCHECK(FMOD_RESULT result);

private:
	SampleState5( void );
    SampleState5( const SampleState5& ) { }
    SampleState5 & operator = ( const SampleState5& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
	Ogre::RenderWindow 	 *mWindow;
	
	Ogre::Radian mouseMoveX;
	Ogre::Radian mouseMoveY;

	Ogre::Real mTime;

	Ogre::Vector3 mTranslateVector;
	float mMoveScale;

	bool keyForwardPress;
	bool keyBackwardPress;
	bool keyLeftPress;
	bool keyRightPress;
	bool forwardCollisionMovement;

	Ogre::Light			 *mainLight1;
	Ogre::Light			 *mainLight2;
	Ogre::Light			 *mainLight3;
	Ogre::Light			 *mainLight4;

	Ogre::Vector3 previous_position;

	//HUD Elements
	Ogre::OverlayManager			*SampleState5OverlayMgr;
    Ogre::Overlay					*HUDOverlay;
	Ogre::FontManager				*SampleState5FontMgr;
	Ogre::OverlayContainer			*HUDPanel;
	Ogre::TextAreaOverlayElement	*x_AxisTextBox;
	Ogre::TextAreaOverlayElement	*y_AxisTextBox;
	Ogre::TextAreaOverlayElement	*z_AxisTextBox;
	Ogre::TextAreaOverlayElement	*otherTextBox;

	FMOD::System *system;
	FMOD::Channel *channel1;
	FMOD::Channel *channel2;
	FMOD::Channel *channel3;

	FMOD::Sound *audio_track;
	FMOD::Sound *left_Punch_Sound;
	FMOD::Sound *right_Punch_Sound;
	FMOD::Sound *left_Kick_Sound;
	FMOD::Sound *right_Kick_Sound;
	FMOD::Sound *attack_Miss_Sound;
	FMOD::Sound *opponent_Kicked_Grunt_Sound;
	FMOD::Sound *opponent_Punched_Grunt_Sound;
	FMOD::Sound *opponent_Grapple_Grunt_Sound;
	FMOD::Sound *player_Punched_Grunt_Sound;
	FMOD::Sound *player_Kicked_Grunt_Sound;
	FMOD::Sound *player_Grapple_Grunt_Sound;
	FMOD_RESULT audio_result;

    static SampleState5    *mSampleState5;
};
#endif
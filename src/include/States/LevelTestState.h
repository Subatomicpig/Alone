#ifndef LEVEL_TEST_STATE_H
#define LEVEL_TEST_STATE_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreFontManager.h> 
#include <OgreTextAreaOverlayElement.h>

#include "States\WorkState.h"
#include "Managers\InputManager.h"
#include "Controllers\GamePadController.h"
#include "CommonOgreTypes.h"

#include "fmod.hpp"
#include "fmod_errors.h"

#include <stdio.h>
#include <cctype>
#include <string.h>

class LevelTestState : public Alone::WorkState 
{
public:
    ~LevelTestState( void );

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );
	void initHUD( void );
	void yesNo( void );
	
	void ERRCHECK(FMOD_RESULT result);

	void loadInitialLevel( void );
	void loadCellsLevel( void );
	void loadCellsNight( void );
	void loadCafeteriaLevel( void );
	void loadCourtyardLevel( void );
	void unloadLevel( void );

	void openDoor( void );
	void checkObjectCollision( void );
	void checkPlayerCollision( void );
	bool checkObjectDetection( void );
	void checkItemCollision( void );
	void checkLoadCharacterDetection( void );

	void moveAIForward( Ogre::SceneNode *node, float speed);
	void moveAILeft(Ogre::SceneNode *node, float speed);
	void moveAIRight(Ogre::SceneNode *node, float speed);
	void turnAIAround( Ogre::SceneNode *node);
	
	void updateAIRat1( void );
	void updateAIRat2( void );
	void updateAIRat3( void );
	void updateAIRat4( void );
	void updateAIRat5( void );
	void updateGuardAI( Ogre::Real seconds );

	void initInventory( void );
	void saveInventory( void );
	void initQuests( void );
	void saveQuests( void );
	void updateQuest( void );
	void updateLinMasQuest( void );
	void updateJollysQuest( void );
	void updateSantosoQuest( void );
	void updateGeminiQuest( void );

	void wait(int seconds);

	void do_focus( void );
	void do_defocus( void );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

	bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
	bool buttonReleased( const OIS::JoyStickEvent &arg, int button );

    static LevelTestState* getSingletonPtr( void );

	void gamepadCharacterMovement( Ogre::Real seconds );
	void keyboardMouseCharacterMovement( void );
	void checkCollision( void );
	bool checkDistance( Ogre::SceneNode* node1, Ogre::SceneNode* node2);

	//char collision functions//
	bool forwardCollisionCheckLTS( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode );
	bool backwardCollisionCheckLTS( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode );
	bool leftCollisionCheckLTS( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode );
	bool rightCollisionCheckLTS( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode );

	void loadOptions( void );
	void loadCharacter();
	void loadStats( void );
	void updateStats( void );
	void loadMerchandise( void );
	void updateMerchandise( void );
	void checkAICollision( void );
	void rotateCamera( void );
	void loadLevelCharacter( void );

private:
	LevelTestState( void );
    LevelTestState( const LevelTestState& ) { }
    LevelTestState & operator = ( const LevelTestState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
	Ogre::RenderWindow 	 *mWindow;
	Ogre::Light			 *mainLight1;
	Ogre::Light			 *mainLight2;
	Ogre::Light			 *mainLight3;
	Ogre::Light			 *mainLight4;
	Ogre::Light			 *mainLight5;
	Ogre::Light			 *mainLight6;
	
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

	Ogre::Vector3 previous_position;

	//HUD Elements
	Ogre::OverlayManager			*LevelTestStateOverlayMgr;
    Ogre::FontManager				*LevelTestStateFontMgr;
	
	Ogre::OverlayContainer			*HUDPanel;
	Ogre::Overlay					*HUDOverlay;
	Ogre::Overlay					*mFaceButtonOverlay;
	Ogre::Overlay					*mAButtonOverlay;
	Ogre::Overlay					*mBButtonOverlay;
	Ogre::Overlay					*mXButtonOverlay;
	Ogre::Overlay					*mYButtonOverlay;
	Ogre::Overlay					*mCrouchOverlay;
	Ogre::Overlay					*mStandOverlay;
	Ogre::Overlay					*textBox;
	Ogre::TextAreaOverlayElement    *npcTalk;
	Ogre::TextAreaOverlayElement    *pickAFaction;
	Ogre::TextAreaOverlayElement    *yButtonTextBox;
	Ogre::TextAreaOverlayElement    *detectObject;
	Ogre::TextAreaOverlayElement	*x_AxisTextBox;
	Ogre::TextAreaOverlayElement	*y_AxisTextBox;
	Ogre::TextAreaOverlayElement	*z_AxisTextBox;
	Ogre::TextAreaOverlayElement	*otherTextBox;
	Ogre::Overlay					*knockOut;
	Ogre::Overlay					*Story01;
	Ogre::Overlay					*Story02;


	FMOD::System *system;
	FMOD::Channel *channel1;
	FMOD::Channel *channel2;
	FMOD::Channel *channel3;

	FMOD::Sound *audio_track;
	FMOD::Sound *cell_Door_Open;
	FMOD::Sound *cell_Door_Close;
	FMOD::Sound *baton_Smack;
	FMOD::Sound *heart_Beat;
	FMOD::Sound *rat_Squished;
	FMOD_RESULT audio_result;

	int skin;
	int hair;
	int shirt;

	std::string name;
	bool stickReleased;
	bool gamepadReleased;
	bool dialogueShuffle;

    static LevelTestState    *mLevelTestState;
};
#endif
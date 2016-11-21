#ifndef PAUSE_UI_STATE_H
#define PAUSE_UI_STATE_H

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
#include "CommonOgreTypes.h"

#include <stdio.h>
#include <cctype>
#include <string.h>

#include <stdio.h>
#include "fmod.hpp"
#include "fmod_errors.h"

class PauseUIState : public Alone::UIState 
{
public:
    ~PauseUIState( void );

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

	void inventoryShown();
	void mapShown();
	void missionsShown();
	void statusShown();
	void gameOptionsShown();
	void getQuestDescription();

	void moveItemCursor();
	void moveJoystick(Ogre::Real seconds);

	void retrieveNames();
	void saveGameSlot01Name();
	void saveGameSlot02Name();
	void saveGameSlot03Name();
	void displayWarning();
	void resetWarning();

	void loadSaveGameFiles();
	void loadCharacterInfo();
	void loadMap();
	void loadItem();
	void loadItemDescription();
	void loadStats( void );

	void loadOptions( void );

	void ERRCHECK(FMOD_RESULT result);

    static PauseUIState* getSingletonPtr( void );
private:
	PauseUIState( void );
    PauseUIState( const PauseUIState& ) { }
    PauseUIState & operator = ( const PauseUIState& );

    Ogre::OverlayManager *pauseOverlayMgr;
    Ogre::Overlay        *inventoryOverlay;
    Ogre::Overlay        *mapOverlay;
    Ogre::Overlay        *missionsOverlay;
    Ogre::Overlay        *statusOverlay;
    Ogre::Overlay        *gameOptionsOverlay;
	Ogre::Overlay		 *mMapOverlay;
	Ogre::Overlay		 *mItemOverlay;
	Ogre::Overlay        *pExitConfirmOverlay;
	Ogre::Overlay        *pSaveConfirmOverlay;

	//Inventory Items
	Ogre::OverlayContainer	*mInventoryPanel;

	Ogre::TextAreaOverlayElement	*mCigarettesTextBox;
	Ogre::TextAreaOverlayElement	*mCigarettesTotalTextBox;
	Ogre::TextAreaOverlayElement	*mPainKillersTextBox;
	Ogre::TextAreaOverlayElement	*mPainKillersTotalTextBox;
	Ogre::TextAreaOverlayElement	*mMoneyTextBox;
	Ogre::TextAreaOverlayElement	*mMoneyTotalTextBox;
	Ogre::TextAreaOverlayElement	*mSyringeTextBox;
	Ogre::TextAreaOverlayElement	*mSyringeTotalTextBox;
	Ogre::TextAreaOverlayElement	*mSoapTextBox;
	Ogre::TextAreaOverlayElement	*mSoapTotalTextBox;
	Ogre::TextAreaOverlayElement	*mSoapMoldTextBox;
	Ogre::TextAreaOverlayElement	*mSoapMoldTotalTextBox;
	Ogre::TextAreaOverlayElement	*mCellKeyTextBox;
	Ogre::TextAreaOverlayElement	*mCellKeyTotalTextBox;
	Ogre::TextAreaOverlayElement	*mCellKeyCopyTextBox;
	Ogre::TextAreaOverlayElement	*mCellKeyCopyTotalTextBox;
	Ogre::TextAreaOverlayElement	*mPendantTextBox;
	Ogre::TextAreaOverlayElement	*mPendantTotalTextBox;
	Ogre::TextAreaOverlayElement	*mEaringTextBox;
	Ogre::TextAreaOverlayElement	*mEaringTotalTextBox;
	Ogre::TextAreaOverlayElement	*mCockroachTextBox;
	Ogre::TextAreaOverlayElement	*mCockroachTotalTextBox;
	Ogre::TextAreaOverlayElement	*mRatTextBox;
	Ogre::TextAreaOverlayElement	*mRatTotalTextBox;
	Ogre::TextAreaOverlayElement	*mItemDescription;

	//Quest Log Items

	Ogre::OverlayContainer	*mQuestPanel;

	Ogre::TextAreaOverlayElement	*mQuestDescriptionTextBox;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot01;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot02;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot03;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot04;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot05;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot06;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot07;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot08;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot09;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot10;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot11;
	Ogre::TextAreaOverlayElement	*mQuestLogSlot12;

	Ogre::OverlayContainer			*mGameOptionsPanel;
	Ogre::TextAreaOverlayElement	*mSaveGameTextBox;
	Ogre::TextAreaOverlayElement	*mExitGameTextBox;
	Ogre::TextAreaOverlayElement	*mSaveGameSlot01;
	Ogre::TextAreaOverlayElement	*mSaveGameSlot02;
	Ogre::TextAreaOverlayElement	*mSaveGameSlot03;
	Ogre::TextAreaOverlayElement	*mWarningControlsTextBox;

	Ogre::OverlayContainer			*mAttributesPanel;
	Ogre::TextAreaOverlayElement	*mNameTextBox;
	Ogre::TextAreaOverlayElement	*mName;
	Ogre::TextAreaOverlayElement	*mUpperBodyTextBox;
	Ogre::TextAreaOverlayElement	*mUpperBody;
	Ogre::TextAreaOverlayElement	*mLowerBodyTextBox;
	Ogre::TextAreaOverlayElement	*mLowerBody;
	Ogre::TextAreaOverlayElement	*mHealthTextBox;
	Ogre::TextAreaOverlayElement	*mHealth;
	Ogre::TextAreaOverlayElement	*mGrappleTextBox;
	Ogre::TextAreaOverlayElement	*mGrapple;
	Ogre::TextAreaOverlayElement	*mSpeedTextBox;
	Ogre::TextAreaOverlayElement	*mSpeed;

	int HeightSpacing;
	int FontSize;
	int xBasePosition, yBasePosition, xBaseTotalPosition;
	int HilightedQuest, HilightedItem, QuestCounter, TotalQuests, CurrentQuest;

	std::string QuestName[11];
	std::string name;

	int hair, shirt, skin;
	bool stickReleased;
	bool pauseReleased;
	bool buttonReleased;
	bool warningDisplayed;

	InputManager* input;

    static PauseUIState    *mPauseUIState;

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
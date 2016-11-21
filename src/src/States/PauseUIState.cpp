
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreFontManager.h> 
#include <OgreTextAreaOverlayElement.h>
#include <cstdlib>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <time.h>

#include "States\PauseUIState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"
#include "fmod.hpp"

#include "tinyxml.h"

using namespace Ogre;
using namespace Alone;

//STATE LOCAL VARIABLES(FLAGS)
int currentScreen;
int currentSelection;
int saveGameSlot;
bool backgroundMovement;
String pauseScreen = "MissionShown";
String gameOption = "SaveGame";

int pCurrentLevel;
int pUpperBodyStat;
int pLowerBodyStat;
int pHealthStat;
int pGrappleStat;

bool pauseMusicSetting;
bool pauseSFXSetting;
bool pauseControls;
bool pauseInvert;

Alone::GamePadControllerList mPauseMenuGamePad;
PauseUIState* PauseUIState::mPauseUIState;

void PauseUIState::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

PauseUIState::PauseUIState( void ) 
{
	mClassName = "PauseMenu";
}

PauseUIState::~PauseUIState( void ) 
{
}

void PauseUIState::enter( void ) 
{
	//options->missions->map->inventory->status

	//INITIALIZES STATE VARIABLES
	currentScreen = 0;
	currentSelection = 0;
	HeightSpacing = 25;
	FontSize = 16;
	xBasePosition = 110;
	xBaseTotalPosition = 400;
	yBasePosition = 200;
	QuestCounter = 0;
	HilightedQuest = 1;
	HilightedItem = 1;
	TotalQuests = 0;
	CurrentQuest = 0;
	stickReleased = true;
	pauseReleased = true;
	buttonReleased = true;
	warningDisplayed = false;
	saveGameSlot = 1;

	loadOptions();
	loadStats();

	//CREATES THE OVERLAY MANAGER FOR THIS SYSTEM
	pauseOverlayMgr   = OverlayManager::getSingletonPtr();

	//INITIALIZES ALL OVERLAYS USED
    inventoryOverlay = pauseOverlayMgr->getByName( "Overlay/InventoryUI" );
    mapOverlay = pauseOverlayMgr->getByName( "Overlay/MapUI" );
    missionsOverlay = pauseOverlayMgr->getByName( "Overlay/MissionsUI" );
    statusOverlay = pauseOverlayMgr->getByName( "Overlay/StatusUI" );
	gameOptionsOverlay = pauseOverlayMgr->getByName("Overlay/GameOptionsUI");
	mItemOverlay = pauseOverlayMgr->getByName("Overlay/CigaretteBox");
	pExitConfirmOverlay = pauseOverlayMgr->getByName("Overlay/PauseMenuExit");
	pSaveConfirmOverlay = pauseOverlayMgr->getByName("Overlay/PauseMenuSave");
	loadMap();

	//POSITIONS ALL OVERLAYS ACCORDINGLY
	missionsOverlay->setScroll(0.0, 0.0);
	inventoryOverlay->setScroll(0.0, 0.0);
	mapOverlay->setScroll(0.0, 0.0);
	mMapOverlay->setScroll(0.0,0.0);
	statusOverlay->setScroll(0.0, 0.0);
	gameOptionsOverlay->setScroll(0.0, 0.0);

	//SHOWS/HIDES ALL OVERLAYS IN THEIR RESPECTIVE POSITIONS
     missionsOverlay->hide();
	inventoryOverlay->hide();
	mapOverlay->hide();
	mMapOverlay->hide();
	statusOverlay->show();

	//Inventory Items
	mInventoryPanel = static_cast<Ogre::OverlayContainer*>(pauseOverlayMgr->createOverlayElement("Panel", "InventoryPanel"));
	mInventoryPanel->setMetricsMode(Ogre::GMM_PIXELS);
	mInventoryPanel->setPosition(0,0);
	mInventoryPanel->setDimensions(200,480);

	mCigarettesTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Cigarettes"));
	mCigarettesTextBox->setDimensions(50,50);
	mCigarettesTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCigarettesTextBox->setPosition(xBasePosition,yBasePosition);
	mCigarettesTextBox->setFontName("MyFont");
	mCigarettesTextBox->setCharHeight(FontSize);
	mCigarettesTextBox->setColour(Ogre::ColourValue::Black);
	mCigarettesTextBox->setCaption("Cigarettes");

	mCigarettesTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "CigarettesTotal"));
	mCigarettesTotalTextBox->setDimensions(50,50);
	mCigarettesTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCigarettesTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition);
	mCigarettesTotalTextBox->setFontName("MyFont");
	mCigarettesTotalTextBox->setCharHeight(FontSize);
	mCigarettesTotalTextBox->setColour(Ogre::ColourValue::Black);

	mPainKillersTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "PainKillers"));
	mPainKillersTextBox->setDimensions(50,50);
	mPainKillersTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mPainKillersTextBox->setPosition(xBasePosition,yBasePosition + HeightSpacing);
	mPainKillersTextBox->setFontName("MyFont");
	mPainKillersTextBox->setCharHeight(FontSize);
	mPainKillersTextBox->setColour(Ogre::ColourValue::Black);
	mPainKillersTextBox->setCaption("Painkillers");

	mPainKillersTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "PainKillersTotal"));
	mPainKillersTotalTextBox->setDimensions(50,50);
	mPainKillersTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mPainKillersTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + + HeightSpacing);
	mPainKillersTotalTextBox->setFontName("MyFont");
	mPainKillersTotalTextBox->setCharHeight(FontSize);
	mPainKillersTotalTextBox->setColour(Ogre::ColourValue::Black);

	mMoneyTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Money"));
	mMoneyTextBox->setDimensions(50,50);
	mMoneyTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mMoneyTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 2));
	mMoneyTextBox->setFontName("MyFont");
	mMoneyTextBox->setCharHeight(FontSize);
	mMoneyTextBox->setColour(Ogre::ColourValue::Black);
	mMoneyTextBox->setCaption("Money");

	mMoneyTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "MoneyTotal"));
	mMoneyTotalTextBox->setDimensions(50,50);
	mMoneyTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mMoneyTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 2));
	mMoneyTotalTextBox->setFontName("MyFont");
	mMoneyTotalTextBox->setCharHeight(FontSize);
	mMoneyTotalTextBox->setColour(Ogre::ColourValue::Black);

	mSyringeTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Syringe"));
	mSyringeTextBox->setDimensions(50,50);
	mSyringeTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSyringeTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 3));
	mSyringeTextBox->setFontName("MyFont");
	mSyringeTextBox->setCharHeight(FontSize);
	mSyringeTextBox->setColour(Ogre::ColourValue::Black);
	mSyringeTextBox->setCaption("Steroid Syringe");

	mSyringeTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "SyringeTotal"));
	mSyringeTotalTextBox->setDimensions(50,50);
	mSyringeTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSyringeTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 3));
	mSyringeTotalTextBox->setFontName("MyFont");
	mSyringeTotalTextBox->setCharHeight(FontSize);
	mSyringeTotalTextBox->setColour(Ogre::ColourValue::Black);

	mSoapTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Soap"));
	mSoapTextBox->setDimensions(50,50);
	mSoapTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSoapTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 4));
	mSoapTextBox->setFontName("MyFont");
	mSoapTextBox->setCharHeight(FontSize);
	mSoapTextBox->setColour(Ogre::ColourValue::Black);
	mSoapTextBox->setCaption("Soap");

	mSoapTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "SoapTotal"));
	mSoapTotalTextBox->setDimensions(50,50);
	mSoapTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSoapTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 4));
	mSoapTotalTextBox->setFontName("MyFont");
	mSoapTotalTextBox->setCharHeight(FontSize);
	mSoapTotalTextBox->setColour(Ogre::ColourValue::Black);

	mSoapMoldTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "SoapMold"));
	mSoapMoldTextBox->setDimensions(50,50);
	mSoapMoldTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSoapMoldTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 5));
	mSoapMoldTextBox->setFontName("MyFont");
	mSoapMoldTextBox->setCharHeight(FontSize);
	mSoapMoldTextBox->setColour(Ogre::ColourValue::Black);
	mSoapMoldTextBox->setCaption("Soap Mould");

	mSoapMoldTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "SoapMoldTotal"));
	mSoapMoldTotalTextBox->setDimensions(50,50);
	mSoapMoldTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSoapMoldTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 5));
	mSoapMoldTotalTextBox->setFontName("MyFont");
	mSoapMoldTotalTextBox->setCharHeight(FontSize);
	mSoapMoldTotalTextBox->setColour(Ogre::ColourValue::Black);

	mCellKeyTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "CellKey"));
	mCellKeyTextBox->setDimensions(50,50);
	mCellKeyTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCellKeyTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 6));
	mCellKeyTextBox->setFontName("MyFont");
	mCellKeyTextBox->setCharHeight(FontSize);
	mCellKeyTextBox->setColour(Ogre::ColourValue::Black);
	mCellKeyTextBox->setCaption("Office Key");

	mCellKeyTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "CellKeyTotal"));
	mCellKeyTotalTextBox->setDimensions(50,50);
	mCellKeyTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCellKeyTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 6));
	mCellKeyTotalTextBox->setFontName("MyFont");
	mCellKeyTotalTextBox->setCharHeight(FontSize);
	mCellKeyTotalTextBox->setColour(Ogre::ColourValue::Black);

	mCellKeyCopyTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "CellKeyCopy"));
	mCellKeyCopyTextBox->setDimensions(50,50);
	mCellKeyCopyTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCellKeyCopyTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 7));
	mCellKeyCopyTextBox->setFontName("MyFont");
	mCellKeyCopyTextBox->setCharHeight(FontSize);
	mCellKeyCopyTextBox->setColour(Ogre::ColourValue::Black);
	mCellKeyCopyTextBox->setCaption("Cell Block Keys");

	mCellKeyCopyTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "CellKeyCopyTotal"));
	mCellKeyCopyTotalTextBox->setDimensions(50,50);
	mCellKeyCopyTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCellKeyCopyTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 7));
	mCellKeyCopyTotalTextBox->setFontName("MyFont");
	mCellKeyCopyTotalTextBox->setCharHeight(FontSize);
	mCellKeyCopyTotalTextBox->setColour(Ogre::ColourValue::Black);

	mPendantTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Pendant"));
	mPendantTextBox->setDimensions(50,50);
	mPendantTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mPendantTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 8));
	mPendantTextBox->setFontName("MyFont");
	mPendantTextBox->setCharHeight(FontSize);
	mPendantTextBox->setColour(Ogre::ColourValue::Black);
	mPendantTextBox->setCaption("Pendant");

	mPendantTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "PendantTotal"));
	mPendantTotalTextBox->setDimensions(50,50);
	mPendantTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mPendantTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 8));
	mPendantTotalTextBox->setFontName("MyFont");
	mPendantTotalTextBox->setCharHeight(FontSize);
	mPendantTotalTextBox->setColour(Ogre::ColourValue::Black);

	mEaringTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Earing"));
	mEaringTextBox->setDimensions(50,50);
	mEaringTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mEaringTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 9));
	mEaringTextBox->setFontName("MyFont");
	mEaringTextBox->setCharHeight(FontSize);
	mEaringTextBox->setColour(Ogre::ColourValue::Black);
	mEaringTextBox->setCaption("Earing");

	mEaringTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "EaringTotal"));
	mEaringTotalTextBox->setDimensions(50,50);
	mEaringTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mEaringTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 9));
	mEaringTotalTextBox->setFontName("MyFont");
	mEaringTotalTextBox->setCharHeight(FontSize);
	mEaringTotalTextBox->setColour(Ogre::ColourValue::Black);

	mCockroachTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Cockroach"));
	mCockroachTextBox->setDimensions(50,50);
	mCockroachTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCockroachTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 10));
	mCockroachTextBox->setFontName("MyFont");
	mCockroachTextBox->setCharHeight(FontSize);
	mCockroachTextBox->setColour(Ogre::ColourValue::Black);
	mCockroachTextBox->setCaption("Cockroach");

	mCockroachTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "CockroachTotal"));
	mCockroachTotalTextBox->setDimensions(50,50);
	mCockroachTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCockroachTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 10));
	mCockroachTotalTextBox->setFontName("MyFont");
	mCockroachTotalTextBox->setCharHeight(FontSize);
	mCockroachTotalTextBox->setColour(Ogre::ColourValue::Black);

	mRatTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Rat"));
	mRatTextBox->setDimensions(50,50);
	mRatTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mRatTextBox->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 11));
	mRatTextBox->setFontName("MyFont");
	mRatTextBox->setCharHeight(FontSize);
	mRatTextBox->setColour(Ogre::ColourValue::Black);
	mRatTextBox->setCaption("Rat");

	mRatTotalTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "RatTotal"));
	mRatTotalTextBox->setDimensions(50,50);
	mRatTotalTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mRatTotalTextBox->setPosition(xBaseTotalPosition,yBasePosition + (HeightSpacing * 11));
	mRatTotalTextBox->setFontName("MyFont");
	mRatTotalTextBox->setCharHeight(FontSize);
	mRatTotalTextBox->setColour(Ogre::ColourValue::Black);

	mItemDescription = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "ItemDescription"));
	mItemDescription->setDimensions(10,50);
	mItemDescription->setMetricsMode(Ogre::GMM_PIXELS);
	mItemDescription->setPosition(525,525);
	mItemDescription->setFontName("MyFont");
	mItemDescription->setCharHeight(FontSize);
	mItemDescription->setColour(Ogre::ColourValue::Black);

	mInventoryPanel->addChild(mCigarettesTextBox);
	mInventoryPanel->addChild(mCigarettesTotalTextBox);
	mInventoryPanel->addChild(mPainKillersTextBox);
	mInventoryPanel->addChild(mPainKillersTotalTextBox);
	mInventoryPanel->addChild(mMoneyTextBox);
	mInventoryPanel->addChild(mMoneyTotalTextBox);
	mInventoryPanel->addChild(mSyringeTextBox);
	mInventoryPanel->addChild(mSyringeTotalTextBox);
	mInventoryPanel->addChild(mSoapTextBox);
	mInventoryPanel->addChild(mSoapTotalTextBox);
	mInventoryPanel->addChild(mSoapMoldTextBox);
	mInventoryPanel->addChild(mSoapMoldTotalTextBox);
	mInventoryPanel->addChild(mCellKeyTextBox);
	mInventoryPanel->addChild(mCellKeyTotalTextBox);
	mInventoryPanel->addChild(mCellKeyCopyTextBox);
	mInventoryPanel->addChild(mCellKeyCopyTotalTextBox);
	mInventoryPanel->addChild(mPendantTextBox);
	mInventoryPanel->addChild(mPendantTotalTextBox);
	mInventoryPanel->addChild(mEaringTextBox);
	mInventoryPanel->addChild(mEaringTotalTextBox);
	mInventoryPanel->addChild(mCockroachTextBox);
	mInventoryPanel->addChild(mCockroachTotalTextBox);
	mInventoryPanel->addChild(mRatTextBox);
	mInventoryPanel->addChild(mRatTotalTextBox);
	mInventoryPanel->addChild(mItemDescription);

	inventoryOverlay->add2D(mInventoryPanel);

	//Quest Log Items

	mQuestPanel = static_cast<Ogre::OverlayContainer*>(pauseOverlayMgr->createOverlayElement("Panel", "QuestPanel"));
	mQuestPanel->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestPanel->setPosition(0,0);
	mQuestPanel->setDimensions(200, 480);

	mQuestLogSlot01 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot01"));
	mQuestLogSlot01->setDimensions(50,50);
	mQuestLogSlot01->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot01->setPosition(xBasePosition,yBasePosition);
	mQuestLogSlot01->setFontName("MyFont");
	mQuestLogSlot01->setCharHeight(FontSize);
	mQuestLogSlot01->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot02 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot02"));
	mQuestLogSlot02->setDimensions(50,50);
	mQuestLogSlot02->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot02->setPosition(xBasePosition,yBasePosition + HeightSpacing);
	mQuestLogSlot02->setFontName("MyFont");
	mQuestLogSlot02->setCharHeight(FontSize);
	mQuestLogSlot02->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot03 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot03"));
	mQuestLogSlot03->setDimensions(50,50);
	mQuestLogSlot03->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot03->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 2));
	mQuestLogSlot03->setFontName("MyFont");
	mQuestLogSlot03->setCharHeight(FontSize);
	mQuestLogSlot03->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot04 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot04"));
	mQuestLogSlot04->setDimensions(50,50);
	mQuestLogSlot04->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot04->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 3));
	mQuestLogSlot04->setFontName("MyFont");
	mQuestLogSlot04->setCharHeight(FontSize);
	mQuestLogSlot04->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot05= static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot05"));
	mQuestLogSlot05->setDimensions(50,50);
	mQuestLogSlot05->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot05->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 4));
	mQuestLogSlot05->setFontName("MyFont");
	mQuestLogSlot05->setCharHeight(FontSize);
	mQuestLogSlot05->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot06 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot06"));
	mQuestLogSlot06->setDimensions(50,50);
	mQuestLogSlot06->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot06->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 5));
	mQuestLogSlot06->setFontName("MyFont");
	mQuestLogSlot06->setCharHeight(FontSize);
	mQuestLogSlot06->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot07 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot07"));
	mQuestLogSlot07->setDimensions(50,50);
	mQuestLogSlot07->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot07->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 6));
	mQuestLogSlot07->setFontName("MyFont");
	mQuestLogSlot07->setCharHeight(FontSize);
	mQuestLogSlot07->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot08 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot08"));
	mQuestLogSlot08->setDimensions(50,50);
	mQuestLogSlot08->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot08->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 7));
	mQuestLogSlot08->setFontName("MyFont");
	mQuestLogSlot08->setCharHeight(FontSize);
	mQuestLogSlot08->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot09 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot09"));
	mQuestLogSlot09->setDimensions(50,50);
	mQuestLogSlot09->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot09->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 8));
	mQuestLogSlot09->setFontName("MyFont");
	mQuestLogSlot09->setCharHeight(FontSize);
	mQuestLogSlot09->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot10 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot10"));
	mQuestLogSlot10->setDimensions(50,50);
	mQuestLogSlot10->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot10->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 9));
	mQuestLogSlot10->setFontName("MyFont");
	mQuestLogSlot10->setCharHeight(FontSize);
	mQuestLogSlot10->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot11 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot11"));
	mQuestLogSlot11->setDimensions(50,50);
	mQuestLogSlot11->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot11->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 10));
	mQuestLogSlot11->setFontName("MyFont");
	mQuestLogSlot11->setCharHeight(FontSize);
	mQuestLogSlot11->setColour(Ogre::ColourValue::Black);

	mQuestLogSlot12 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestLogSlot12"));
	mQuestLogSlot12->setDimensions(50,50);
	mQuestLogSlot12->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestLogSlot12->setPosition(xBasePosition,yBasePosition + (HeightSpacing * 11));
	mQuestLogSlot12->setFontName("MyFont");
	mQuestLogSlot12->setCharHeight(FontSize);
	mQuestLogSlot12->setColour(Ogre::ColourValue::Black);

	mQuestDescriptionTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "QuestDescription"));
	mQuestDescriptionTextBox->setDimensions(10,50);
	mQuestDescriptionTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mQuestDescriptionTextBox->setPosition(525,250);
	mQuestDescriptionTextBox->setFontName("MyFont");
	mQuestDescriptionTextBox->setCharHeight(FontSize);
	mQuestDescriptionTextBox->setColour(Ogre::ColourValue::Black);
	
	mQuestPanel->addChild(mQuestLogSlot01);
	mQuestPanel->addChild(mQuestLogSlot02);
	mQuestPanel->addChild(mQuestLogSlot03);
	mQuestPanel->addChild(mQuestLogSlot04);
	mQuestPanel->addChild(mQuestLogSlot05);
	mQuestPanel->addChild(mQuestLogSlot06);
	mQuestPanel->addChild(mQuestLogSlot07);
	mQuestPanel->addChild(mQuestLogSlot08);
	mQuestPanel->addChild(mQuestLogSlot09);
	mQuestPanel->addChild(mQuestLogSlot10);
	mQuestPanel->addChild(mQuestLogSlot11);
	mQuestPanel->addChild(mQuestLogSlot12);
	mQuestPanel->addChild(mQuestDescriptionTextBox);
	missionsOverlay->add2D(mQuestPanel);

	mGameOptionsPanel = static_cast<Ogre::OverlayContainer*>(pauseOverlayMgr->createOverlayElement("Panel", "GameOptionsPanel"));
	mGameOptionsPanel->setMetricsMode(Ogre::GMM_PIXELS);
	mGameOptionsPanel->setPosition(0,0);
	mGameOptionsPanel->setDimensions(200,480);

	mSaveGameTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "SaveGame"));
	mSaveGameTextBox->setDimensions(50,50);
	mSaveGameTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSaveGameTextBox->setPosition(600,250);
	mSaveGameTextBox->setFontName("MyFont");
	mSaveGameTextBox->setCharHeight(FontSize);
	mSaveGameTextBox->setColour(Ogre::ColourValue::Black);
	mSaveGameTextBox->setCaption("Save Game");

	mSaveGameSlot01 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "SaveGameSlot01"));
	mSaveGameSlot01->setDimensions(50,50);
	mSaveGameSlot01->setMetricsMode(Ogre::GMM_PIXELS);
	mSaveGameSlot01->setPosition(615, 275);
	mSaveGameSlot01->setFontName("MyFont");
	mSaveGameSlot01->setCharHeight(FontSize);
	mSaveGameSlot01->setColour(Ogre::ColourValue::Red);

	mSaveGameSlot02 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "SaveGameSlot02"));
	mSaveGameSlot02->setDimensions(50,50);
	mSaveGameSlot02->setMetricsMode(Ogre::GMM_PIXELS);
	mSaveGameSlot02->setPosition(615, 300);
	mSaveGameSlot02->setFontName("MyFont");
	mSaveGameSlot02->setCharHeight(FontSize);
	mSaveGameSlot02->setColour(Ogre::ColourValue::Black);

	mSaveGameSlot03 = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "SaveGameSlot03"));
	mSaveGameSlot03->setDimensions(50,50);
	mSaveGameSlot03->setMetricsMode(Ogre::GMM_PIXELS);
	mSaveGameSlot03->setPosition(615, 325);
	mSaveGameSlot03->setFontName("MyFont");
	mSaveGameSlot03->setCharHeight(FontSize);
	mSaveGameSlot03->setColour(Ogre::ColourValue::Black);

	mExitGameTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "ExitGame"));
	mExitGameTextBox->setDimensions(50,50);
	mExitGameTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mExitGameTextBox->setPosition(600,350);
	mExitGameTextBox->setFontName("MyFont");
	mExitGameTextBox->setCharHeight(FontSize);
	mExitGameTextBox->setColour(Ogre::ColourValue::Black);
	mExitGameTextBox->setCaption("Exit Game");
	
	mWarningControlsTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "WarningControls"));
	mWarningControlsTextBox->setDimensions(50,50);
	mWarningControlsTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mWarningControlsTextBox->setPosition(525, 475);
	mWarningControlsTextBox->setFontName("MyFont");
	mWarningControlsTextBox->setCharHeight(FontSize);
	mWarningControlsTextBox->setColour(Ogre::ColourValue::Black);

	mGameOptionsPanel->addChild(mSaveGameTextBox);
	mGameOptionsPanel->addChild(mExitGameTextBox);
	mGameOptionsPanel->addChild(mSaveGameSlot01);
	mGameOptionsPanel->addChild(mSaveGameSlot02);
	mGameOptionsPanel->addChild(mSaveGameSlot03);
	mGameOptionsPanel->addChild(mWarningControlsTextBox);
	gameOptionsOverlay->add2D(mGameOptionsPanel);

	mAttributesPanel = static_cast<Ogre::OverlayContainer*>(pauseOverlayMgr->createOverlayElement("Panel", "AttributesPanel"));
	mAttributesPanel->setMetricsMode(Ogre::GMM_PIXELS);
	mAttributesPanel->setPosition(0,0);
	mAttributesPanel->setDimensions(200,480);

	mNameTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "NameTextBox"));
	mNameTextBox->setDimensions(50,50);
	mNameTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mNameTextBox->setPosition(600,250);
	mNameTextBox->setFontName("MyFont");
	mNameTextBox->setCharHeight(FontSize);
	mNameTextBox->setColour(Ogre::ColourValue::Black);
	mNameTextBox->setCaption("Name:");

	mName = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Name"));
	mName->setDimensions(50,50);
	mName->setMetricsMode(Ogre::GMM_PIXELS);
	mName->setPosition(725,250);
	mName->setFontName("MyFont");
	mName->setCharHeight(FontSize);
	mName->setColour(Ogre::ColourValue::Black);

	mUpperBodyTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "UpperBodyTextBox"));
	mUpperBodyTextBox->setDimensions(50,50);
	mUpperBodyTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mUpperBodyTextBox->setPosition(600,250 + HeightSpacing);
	mUpperBodyTextBox->setFontName("MyFont");
	mUpperBodyTextBox->setCharHeight(FontSize);
	mUpperBodyTextBox->setColour(Ogre::ColourValue::Black);
	mUpperBodyTextBox->setCaption("Upper Body:");

	mUpperBody = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "UpperBody"));
	mUpperBody->setDimensions(50,50);
	mUpperBody->setMetricsMode(Ogre::GMM_PIXELS);
	mUpperBody->setPosition(725,250 + HeightSpacing);
	mUpperBody->setFontName("MyFont");
	mUpperBody->setCharHeight(FontSize);
	mUpperBody->setColour(Ogre::ColourValue::Black);

	mLowerBodyTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "LowerBodyTextBox"));
	mLowerBodyTextBox->setDimensions(50,50);
	mLowerBodyTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mLowerBodyTextBox->setPosition(600,250 + (HeightSpacing * 2));
	mLowerBodyTextBox->setFontName("MyFont");
	mLowerBodyTextBox->setCharHeight(FontSize);
	mLowerBodyTextBox->setColour(Ogre::ColourValue::Black);
	mLowerBodyTextBox->setCaption("Lower Body:");

	mLowerBody = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "LowerBody"));
	mLowerBody->setDimensions(50,50);
	mLowerBody->setMetricsMode(Ogre::GMM_PIXELS);
	mLowerBody->setPosition(725,250 + (HeightSpacing * 2));
	mLowerBody->setFontName("MyFont");
	mLowerBody->setCharHeight(FontSize);
	mLowerBody->setColour(Ogre::ColourValue::Black);

	mHealthTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "HealthTextBox"));
	mHealthTextBox->setDimensions(50,50);
	mHealthTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mHealthTextBox->setPosition(600,250 + (HeightSpacing * 3));
	mHealthTextBox->setFontName("MyFont");
	mHealthTextBox->setCharHeight(FontSize);
	mHealthTextBox->setColour(Ogre::ColourValue::Black);
	mHealthTextBox->setCaption("Health:");

	mHealth = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Health"));
	mHealth->setDimensions(50,50);
	mHealth->setMetricsMode(Ogre::GMM_PIXELS);
	mHealth->setPosition(725,250 + (HeightSpacing * 3));
	mHealth->setFontName("MyFont");
	mHealth->setCharHeight(FontSize);
	mHealth->setColour(Ogre::ColourValue::Black);

	mGrappleTextBox = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "GrappleTextBox"));
	mGrappleTextBox->setDimensions(50,50);
	mGrappleTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mGrappleTextBox->setPosition(600,250 + (HeightSpacing * 4));
	mGrappleTextBox->setFontName("MyFont");
	mGrappleTextBox->setCharHeight(FontSize);
	mGrappleTextBox->setColour(Ogre::ColourValue::Black);
	mGrappleTextBox->setCaption("Grapple:");

	mGrapple = static_cast<Ogre::TextAreaOverlayElement*>(pauseOverlayMgr->createOverlayElement("TextArea", "Grapple"));
	mGrapple->setDimensions(50,50);
	mGrapple->setMetricsMode(Ogre::GMM_PIXELS);
	mGrapple->setPosition(725,250 + (HeightSpacing * 4));
	mGrapple->setFontName("MyFont");
	mGrapple->setCharHeight(FontSize);
	mGrapple->setColour(Ogre::ColourValue::Black);

	mAttributesPanel->addChild(mNameTextBox);
	mAttributesPanel->addChild(mName);
	mAttributesPanel->addChild(mUpperBodyTextBox);
	mAttributesPanel->addChild(mUpperBody);
	mAttributesPanel->addChild(mLowerBodyTextBox);
	mAttributesPanel->addChild(mLowerBody);
	mAttributesPanel->addChild(mHealthTextBox);
	mAttributesPanel->addChild(mHealth);
	mAttributesPanel->addChild(mGrappleTextBox);
	mAttributesPanel->addChild(mGrapple);
	statusOverlay->add2D(mAttributesPanel);

	moveItemCursor();

	mPauseMenuGamePad.push_back(new Alone::GamePadController(0, 4000));

	//CREATES THE INPUT MANAGER FOR THIS SYSTEM
	input = InputManager::getSingletonPtr();
	input->addKeyListener(mPauseUIState, mClassName);
	input->addJoystickListener(mPauseUIState, mClassName);

	//DEFOCUSES THE CURRENT WORKSTATE
	GameManager::getSingletonPtr()->getWorkState()->defocus();

	channel1 = 0;
	channel2 = 0;
	channel3 = 0;

	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Menu/Paper/Scribbles 2.mp3", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Movement);
	ERRCHECK(audio_result);
}

void PauseUIState::exit( void ) 
{
	audio_result = cursor_Movement->release();
	ERRCHECK(audio_result);

	audio_result = system->release();
	ERRCHECK(audio_result);

	audio_result = system->close();
	ERRCHECK(audio_result);

	//HIDES ALL OVERLAYS
	missionsOverlay->hide();
	statusOverlay->hide();
	inventoryOverlay->hide();
	mapOverlay->hide();
	mMapOverlay->hide();
	gameOptionsOverlay->hide();
	mItemOverlay->hide();
	pExitConfirmOverlay->hide();
	pSaveConfirmOverlay->hide();

	pauseOverlayMgr->destroyOverlayElement("Cigarettes");
	pauseOverlayMgr->destroyOverlayElement("CigarettesTotal");
	pauseOverlayMgr->destroyOverlayElement("PainKillers");
	pauseOverlayMgr->destroyOverlayElement("PainKillersTotal");
	pauseOverlayMgr->destroyOverlayElement("Money");
	pauseOverlayMgr->destroyOverlayElement("MoneyTotal");
	pauseOverlayMgr->destroyOverlayElement("Syringe");
	pauseOverlayMgr->destroyOverlayElement("SyringeTotal");
	pauseOverlayMgr->destroyOverlayElement("Soap");
	pauseOverlayMgr->destroyOverlayElement("SoapTotal");
	pauseOverlayMgr->destroyOverlayElement("SoapMold");
	pauseOverlayMgr->destroyOverlayElement("SoapMoldTotal");
	pauseOverlayMgr->destroyOverlayElement("CellKey");
	pauseOverlayMgr->destroyOverlayElement("CellKeyTotal");
	pauseOverlayMgr->destroyOverlayElement("CellKeyCopy");
	pauseOverlayMgr->destroyOverlayElement("CellKeyCopyTotal");
	pauseOverlayMgr->destroyOverlayElement("Pendant");
	pauseOverlayMgr->destroyOverlayElement("PendantTotal");
	pauseOverlayMgr->destroyOverlayElement("Earing");
	pauseOverlayMgr->destroyOverlayElement("EaringTotal");
	pauseOverlayMgr->destroyOverlayElement("Cockroach");
	pauseOverlayMgr->destroyOverlayElement("CockroachTotal");
	pauseOverlayMgr->destroyOverlayElement("Rat");
	pauseOverlayMgr->destroyOverlayElement("RatTotal");
	pauseOverlayMgr->destroyOverlayElement("ItemDescription");
	pauseOverlayMgr->destroyOverlayElement("InventoryPanel");

	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot01");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot02");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot03");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot04");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot05");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot06");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot07");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot08");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot09");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot10");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot11");
	pauseOverlayMgr->destroyOverlayElement("QuestLogSlot12");
	pauseOverlayMgr->destroyOverlayElement("QuestDescription");
	pauseOverlayMgr->destroyOverlayElement("QuestPanel");

	pauseOverlayMgr->destroyOverlayElement("SaveGame");
	pauseOverlayMgr->destroyOverlayElement("ExitGame");
	pauseOverlayMgr->destroyOverlayElement("SaveGameSlot01");
	pauseOverlayMgr->destroyOverlayElement("SaveGameSlot02");
	pauseOverlayMgr->destroyOverlayElement("SaveGameSlot03");
	pauseOverlayMgr->destroyOverlayElement("WarningControls");
	pauseOverlayMgr->destroyOverlayElement("GameOptionsPanel");

	pauseOverlayMgr->destroyOverlayElement("NameTextBox");
	pauseOverlayMgr->destroyOverlayElement("Name");
	pauseOverlayMgr->destroyOverlayElement("UpperBodyTextBox");
	pauseOverlayMgr->destroyOverlayElement("UpperBody");
	pauseOverlayMgr->destroyOverlayElement("LowerBodyTextBox");
	pauseOverlayMgr->destroyOverlayElement("LowerBody");
	pauseOverlayMgr->destroyOverlayElement("HealthTextBox");
	pauseOverlayMgr->destroyOverlayElement("Health");
	pauseOverlayMgr->destroyOverlayElement("GrappleTextBox");
	pauseOverlayMgr->destroyOverlayElement("Grapple");
	pauseOverlayMgr->destroyOverlayElement("AttributesPanel");

	//DESTROYS THE INPUT MANAGER FOR THIS SYSTEM
	input->removeKeyListener(mPauseUIState);
	input->removeJoystickListener(mPauseUIState);

	//REFOCUSES THE CURRENT WORKSTATE
	GameManager::getSingletonPtr()->getWorkState()->focus();
}

//KEEP CODE TO A MINIMUM IN THE UPDATE; THE LESS THAT IS DONE HERE = THE FASTER AND MORE EFFICIENT THE PROGRAM

void PauseUIState::update( Ogre::Real seconds ) 
{
	system->update();

	moveJoystick(seconds);

	//CHECKS TO SEE WHICH SCREEN IS CURRENTLY ACTIVE
	if(currentScreen == 0)//game options screen
	{
		pauseScreen = "GameOptionsShown";
		retrieveNames();
		gameOptionsShown();
	}
	else if(currentScreen == 1)//missions screen
	{
		pauseScreen = "MissionShown";
		missionsShown();
	}
	else if(currentScreen == 2)//map screen
	{
		pauseScreen = "MapShown";
		mapShown();
	}
	else if(currentScreen == 3)//inventory screen
	{
		pauseScreen = "InventoryShown";
		inventoryShown();
	}
	else if(currentScreen = 4)//status screen
	{
		pauseScreen = "StatusShown";
		statusShown();
	}

	if(input->isButtonDown(0,5) && buttonReleased == true)
	{
		buttonReleased = false;
		
		if(pauseScreen == "GameOptionsShown")
		{
			resetWarning();
		}

		if(currentScreen < 4)
		{
			currentScreen++;
		}
	}
	else if(input->isButtonDown(0,4) && buttonReleased == true)
	{
		buttonReleased = false;
		
		if(pauseScreen == "GameOptionsShown")
		{
			resetWarning();
		}

		if(currentScreen > 0)
		{
			currentScreen--;
		}
	}
	else if(input->isButtonDown(0,9) && buttonReleased == true)
	{
		if(pauseReleased == false)
		{
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
		}

		buttonReleased = false;
	}
	else if(input->isButtonDown(0,1) && buttonReleased == true)
	{
		buttonReleased = false;

		if(pauseScreen == "GameOptionsShown")
		{
			if(warningDisplayed == false)
			{
				displayWarning();
			}
			else if(warningDisplayed == true)
			{
				if(saveGameSlot != 4)
				{
					resetWarning();
					warningDisplayed = false;
					loadSaveGameFiles();
				}
				else if(saveGameSlot == 4)
				{
					GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
					GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
				}
			}
		}
	}
	else if(input->isButtonDown(0,2) && buttonReleased == true && warningDisplayed == true)
	{
		buttonReleased = false;

		if(pauseScreen == "GameOptionsShown")
		{
			resetWarning();
			warningDisplayed = false;
		}
	}
	else if(!input->isButtonDown(0,5) && !input->isButtonDown(0,4) && !input->isButtonDown(0,9) && !input->isButtonDown(0,1) && !input->isButtonDown(0,2))
	{
		buttonReleased = true;
		pauseReleased = false;
	}
}

void PauseUIState::displayWarning()
{
bool joystick;

	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	if(atoi(element->Attribute("Keyboard")) == 1)
	{
		joystick = false;
	}
	else
	{
		joystick = true;
	}

	if(saveGameSlot != 4)
	{
		pSaveConfirmOverlay->show();
		//mWarningTextBox->setCaption("Overwrite the save slot?");
	}
	else if(saveGameSlot == 4)
	{
		pExitConfirmOverlay->show();
		//mWarningTextBox->setCaption("Are you sure you want to exit?");
	}

	if(joystick == true)
	{
		mWarningControlsTextBox->setCaption("Press A (Accept) or B (Decline) on the joypad.");
	}
	else
	{
		mWarningControlsTextBox->setCaption("Press Y (Accept) or N (Decline) on the keyboard.");
	}

	warningDisplayed = true;
}

void PauseUIState::resetWarning()
{
	pSaveConfirmOverlay->hide();
	pExitConfirmOverlay->hide();
	mWarningControlsTextBox->setCaption("");
	warningDisplayed = false;
}

void PauseUIState::moveJoystick(Ogre::Real seconds)
{
	mPauseMenuGamePad[0]->update(seconds);

	Real yLeft = mPauseMenuGamePad[0]->getY(0);

	if(yLeft > 0.5f && stickReleased == true)
	{
		stickReleased = false;

		if(pauseScreen == "GameOptionsShown")
		{
			if(warningDisplayed == false)
			{
				saveGameSlot--;
			}

			if(saveGameSlot == 0)
			{
				saveGameSlot = 4;
			}
		}
		else if(pauseScreen == "MissionShown")
		{
			if(pauseSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
				ERRCHECK(audio_result);
			}

			if(HilightedQuest != 0)
			{
				HilightedQuest--;
			}
		}
		else if(pauseScreen == "InventoryShown")
		{
			if(HilightedItem > 0)
				HilightedItem--;

			if(HilightedItem == 0)
			{
				HilightedItem = 12;
			}

			moveItemCursor();
		}
	}
	else if(yLeft < -0.5f && stickReleased == true)
	{
		stickReleased = false;
		
		if(pauseScreen == "GameOptionsShown")
		{
			if(warningDisplayed == false)
			{
				saveGameSlot++;
			}

			if(saveGameSlot == 5)
			{
				saveGameSlot = 1;
			}
		}
		else if(pauseScreen == "MissionShown")
		{
			if(pauseSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
				ERRCHECK(audio_result);
			}

			if(HilightedQuest < TotalQuests)
			{
				HilightedQuest++;
			}
		}
		else if(pauseScreen == "InventoryShown")
		{
			if(HilightedItem < 13)
				HilightedItem++;

			if(HilightedItem == 13)
			{
				HilightedItem = 1;
			}

			moveItemCursor();
		}
	}
	else if(yLeft > -0.5f && yLeft < 0.5f && stickReleased == false)
	{
		stickReleased = true;
	} 
}

void PauseUIState::loadSaveGameFiles()
{
	loadCharacterInfo();

	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("SaveGame");
	doc.LinkEndChild(root);

	TiXmlElement *info = new TiXmlElement("Info");
	root->LinkEndChild(info);
	
	info->SetAttribute("Exist", true);
	info->SetAttribute("Name", name.c_str());
	info->SetAttribute("Skin", skin);
	info->SetAttribute("Shirt", shirt);
	info->SetAttribute("Hair", hair);

	TiXmlElement *Stat = new TiXmlElement("Stat");
	root->LinkEndChild(Stat);
	
	Stat->SetAttribute("Level", pCurrentLevel);
	Stat->SetAttribute("UpperBody", pUpperBodyStat);
	Stat->SetAttribute("LowerBody", pLowerBodyStat);
	Stat->SetAttribute("Health", pHealthStat);
	Stat->SetAttribute("Grapple", pGrappleStat);

	TiXmlDocument loadDoc("Inventory.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Inventory");

	TiXmlElement *inventory = new TiXmlElement("Inventory");
	root->LinkEndChild(inventory);

	inventory->SetAttribute("Cigarettes", atoi(element->Attribute("Cigarettes")));
	inventory->SetAttribute("PainKillers", atoi(element->Attribute("PainKillers")));
	inventory->SetAttribute("Money", atoi(element->Attribute("Money")));
	inventory->SetAttribute("Syringe", atoi(element->Attribute("Syringe")));
	inventory->SetAttribute("Soap", atoi(element->Attribute("Soap")));
	inventory->SetAttribute("SoapMold", atoi(element->Attribute("SoapMold")));
	inventory->SetAttribute("LinMasKey", atoi(element->Attribute("LinMasKey")));
	inventory->SetAttribute("CellKey", atoi(element->Attribute("CellKey")));
	inventory->SetAttribute("CellKeyCopy", atoi(element->Attribute("CellKeyCopy")));
	inventory->SetAttribute("GoldCockroach", atoi(element->Attribute("GoldCockroach")));
	inventory->SetAttribute("Pendant", atoi(element->Attribute("Pendant")));
	inventory->SetAttribute("Earing", atoi(element->Attribute("Earing")));
	inventory->SetAttribute("Cockroach", atoi(element->Attribute("Cockroach")));
	inventory->SetAttribute("Rat", atoi(element->Attribute("Rat")));

	element = loadDoc.RootElement()->FirstChildElement("QuestTracker");

	TiXmlElement *QuestTracker = new TiXmlElement("QuestTracker");
	root->LinkEndChild(QuestTracker);

	QuestTracker->SetAttribute("linMasBugInTheSystemQuestCompleted", atoi(element->Attribute("linMasBugInTheSystemQuestCompleted")));
	QuestTracker->SetAttribute("linMasTheRatScuttleQuestCompleted", atoi(element->Attribute("linMasTheRatScuttleQuestCompleted")));
	QuestTracker->SetAttribute("linMaswhoIsEscapingQuestCompleted", atoi(element->Attribute("linMaswhoIsEscapingQuestCompleted")));
	QuestTracker->SetAttribute("linMastrainQuestCompleted", atoi(element->Attribute("linMastrainQuestCompleted")));
	QuestTracker->SetAttribute("linMasMoreBugsInTheSystemQuestCompleted", atoi(element->Attribute("linMasMoreBugsInTheSystemQuestCompleted")));
	QuestTracker->SetAttribute("linMasPickAFactionQuestCompleted", atoi(element->Attribute("linMasPickAFactionQuestCompleted")));
	QuestTracker->SetAttribute("linMasFinalQuestQuestCompleted", atoi(element->Attribute("linMasFinalQuestQuestCompleted")));
	QuestTracker->SetAttribute("linMasTrainingQuestCompleted", atoi(element->Attribute("linMasTrainingQuestCompleted")));

	QuestTracker->SetAttribute("jollytalkToTheSwordQuestCompleted", atoi(element->Attribute("jollytalkToTheSwordQuestCompleted")));
	QuestTracker->SetAttribute("jollyKeyToThePlanQuestCompleted", atoi(element->Attribute("jollyKeyToThePlanQuestCompleted")));
	QuestTracker->SetAttribute("jollyTwoPendantsQuestCompleted", atoi(element->Attribute("jollyTwoPendantsQuestCompleted")));
	QuestTracker->SetAttribute("jollyKnockEmOutQuestCompleted", atoi(element->Attribute("jollyKnockEmOutQuestCompleted")));

	QuestTracker->SetAttribute("geminitalkToTheSwordQuestCompleted", atoi(element->Attribute("geminitalkToTheSwordQuestCompleted")));
	QuestTracker->SetAttribute("gemniThePrideOfJollyQuestCompleted", atoi(element->Attribute("gemniThePrideOfJollyQuestCompleted")));
	QuestTracker->SetAttribute("geminiKnockEmOutQuestCompleted", atoi(element->Attribute("geminiKnockEmOutQuestCompleted")));
	QuestTracker->SetAttribute("geminiKeyToThePlanQuestCompleted", atoi(element->Attribute("geminiKeyToThePlanQuestCompleted")));

	QuestTracker->SetAttribute("johnny666TrainingDoneQuestCompleted", atoi(element->Attribute("johnny666TrainingDoneQuestCompleted")));
	QuestTracker->SetAttribute("johnny66680RepQuestCompleted",atoi(element->Attribute("johnny66680RepQuestCompleted")));
	QuestTracker->SetAttribute("johnny666CheatALittleQuestCompleted", atoi(element->Attribute("johnny666CheatALittleQuestCompleted")));

	element = loadDoc.RootElement()->FirstChildElement("CurrentQuest");

	TiXmlElement *CurrentQuest = new TiXmlElement("CurrentQuest");
	root->LinkEndChild(CurrentQuest);

	CurrentQuest->SetAttribute("linMasFinalQuest", atoi(element->Attribute("linMasFinalQuest")));
	CurrentQuest->SetAttribute("linMasTraining", atoi(element->Attribute("linMasTraining")));
	CurrentQuest->SetAttribute("johnnny666Rep", atoi(element->Attribute("johnnny666Rep")));
	CurrentQuest->SetAttribute("johnny666CheatALittle", atoi(element->Attribute("johnny666CheatALittle")));
	CurrentQuest->SetAttribute("johnny66680Rep", atoi(element->Attribute("johnny66680Rep")));
	CurrentQuest->SetAttribute("johnny666TrainingDone", atoi(element->Attribute("johnny666TrainingDone")));
	CurrentQuest->SetAttribute("geminiThePrideOfJolly", atoi(element->Attribute("geminiThePrideOfJolly")));
	CurrentQuest->SetAttribute("geminiKnockEmOut", atoi(element->Attribute("geminiKnockEmOut")));
	CurrentQuest->SetAttribute("geminiKeyToThePlan", atoi(element->Attribute("geminiKeyToThePlan")));
	CurrentQuest->SetAttribute("geminiTalkToTheSword", atoi(element->Attribute("geminiTalkToTheSword")));
	CurrentQuest->SetAttribute("jollyTwoPendants", atoi(element->Attribute("jollyTwoPendants")));
	CurrentQuest->SetAttribute("jollyKnockEmOut", atoi(element->Attribute("jollyKnockEmOut")));
	CurrentQuest->SetAttribute("jollyKeyToThePlan", atoi(element->Attribute("jollyKeyToThePlan")));
	CurrentQuest->SetAttribute("santosoKeyToThePlan", atoi(element->Attribute("santosoKeyToThePlan")));
	CurrentQuest->SetAttribute("santosoTalkToTheSword", atoi(element->Attribute("santosoTalkToTheSword")));
	CurrentQuest->SetAttribute("jollyTalkToTheSword", atoi(element->Attribute("jollyTalkToTheSword")));
	CurrentQuest->SetAttribute("linMaTrain", atoi(element->Attribute("linMaTrain")));
	CurrentQuest->SetAttribute("linMasWhoIsEscaping", atoi(element->Attribute("linMasWhoIsEscaping")));
	CurrentQuest->SetAttribute("linMasTheRatScuttle", atoi(element->Attribute("linMasTheRatScuttle")));
	CurrentQuest->SetAttribute("linMasMoreBugsInTheSystem", atoi(element->Attribute("linMasMoreBugsInTheSystem")));
	CurrentQuest->SetAttribute("linMasBugInTheSystem", atoi(element->Attribute("linMasBugInTheSystem")));
	CurrentQuest->SetAttribute("linMasPickAFaction", atoi(element->Attribute("linMasPickAFaction")));

	element = loadDoc.RootElement()->FirstChildElement("Other");

	TiXmlElement *Other = new TiXmlElement("Other");
	root->LinkEndChild(Other);

	Other->SetAttribute("emptyCellRightOfPlayerTaken", atoi(element->Attribute("emptyCellRightOfPlayerTaken")));
	Other->SetAttribute("dannyBoySteroidsTaken", atoi(element->Attribute("dannyBoySteroidsTaken")));
	Other->SetAttribute("santosoPainKillerTaken", atoi(element->Attribute("santosoPainKillerTaken")));
	Other->SetAttribute("linMasMoneyTaken", atoi(element->Attribute("linMasMoneyTaken")));
	Other->SetAttribute("linMasSoapTaken", atoi(element->Attribute("linMasSoapTaken")));
	Other->SetAttribute("factionedPickedJolly", atoi(element->Attribute("factionedPickedJolly")));
	Other->SetAttribute("factionedPickedGemini", atoi(element->Attribute("factionedPickedGemini")));

	switch(saveGameSlot)
	{
	case 1:
		doc.SaveFile("SG01.xml");
		break;
	case 2:
		doc.SaveFile("SG02.xml");
		break;
	case 3:
		doc.SaveFile("SG03.xml");
		break;
	}
}

void PauseUIState::loadCharacterInfo()
{
	TiXmlDocument loadDoc("CharacterEditor.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");
	name = element->Attribute("Name");
	skin = atoi(element->Attribute("Skin"));
	shirt = atoi(element->Attribute("Shirt"));
	hair = atoi(element->Attribute("Hair"));
}

void PauseUIState::retrieveNames()
{
	saveGameSlot01Name();
	saveGameSlot02Name();
	saveGameSlot03Name();
}

void PauseUIState::saveGameSlot01Name()
{
	TiXmlDocument loadDoc("SG01.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	if(atoi(element->Attribute("Exist")) == 1)
	{
		mSaveGameSlot01->setCaption(element->Attribute("Name"));
	}
	else
	{
		mSaveGameSlot01->setCaption("<empty>");
	}
}

void PauseUIState::saveGameSlot02Name()
{
	TiXmlDocument loadDoc("SG02.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	if(atoi(element->Attribute("Exist")) == 1)
	{
		mSaveGameSlot02->setCaption(element->Attribute("Name"));
	}
	else
	{
		mSaveGameSlot02->setCaption("<empty>");
	}
}

void PauseUIState::saveGameSlot03Name()
{
	TiXmlDocument loadDoc("SG03.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	if(atoi(element->Attribute("Exist")) == 1)
	{
		mSaveGameSlot03->setCaption(element->Attribute("Name"));
	}
	else
	{
		mSaveGameSlot03->setCaption("<empty>");
	}
}

void PauseUIState::loadMap()
{
	TiXmlDocument loadDoc("Inventory.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Other");

	switch(atoi(element->Attribute("LevelLoaded")))
	{
	case 2:
		mMapOverlay = pauseOverlayMgr->getByName("Overlay/CafeteriaMap");
		break;
	case 3:
		mMapOverlay = pauseOverlayMgr->getByName("Overlay/CourtyardMap");
		break;
	case 4:
		mMapOverlay = pauseOverlayMgr->getByName("Overlay/ArenaMap");
		break;
	default:
		mMapOverlay = pauseOverlayMgr->getByName("Overlay/CellsMap");
		break;
	}
}

void PauseUIState::loadItem()
{
	mItemOverlay->hide();

	switch(HilightedItem)
	{
	case 1:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/CigaretteBox");
		break;
	case 2:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/PainKillers");
		break;
	case 3:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/Cash");
		break;
	case 4:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/SteroidSyringe");
		break;
	case 5:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/Soap");
		break;
	case 6:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/SoapMould");
		break;
	case 7:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/OfficeKey");
		break;
	case 8:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/CellBlockKey");
		break;
	case 9:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/Pendant");
		break;
	case 10:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/Earring");
		break;
	case 11:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/Cockroach");
		break;
	case 12:
		mItemOverlay = pauseOverlayMgr->getByName("Overlay/Rat");
		break;
	}

	mItemOverlay->setScale(0.30, 0.30);
	mItemOverlay->setScroll(0.35, 0.15);
	mItemOverlay->show();
}

void PauseUIState::loadItemDescription()
{
	TiXmlDocument loadDoc("QuestDescription.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("ItemDescription");

	switch(HilightedItem)
	{
	case 1:
		mItemDescription->setCaption(element->Attribute("Cigarettes"));
		break;
	case 2:
		mItemDescription->setCaption(element->Attribute("PainKillers"));
		break;
	case 3:
		mItemDescription->setCaption(element->Attribute("Money"));
		break;
	case 4:
		mItemDescription->setCaption(element->Attribute("SteroidSyringe"));
		break;
	case 5:
		mItemDescription->setCaption(element->Attribute("Soap"));
		break;
	case 6:
		mItemDescription->setCaption(element->Attribute("SoapMould"));
		break;
	case 7:
		mItemDescription->setCaption(element->Attribute("OfficeKey"));
		break;
	case 8:
		mItemDescription->setCaption(element->Attribute("CellBlockKey"));
		break;
	case 9:
		mItemDescription->setCaption(element->Attribute("Pendant"));
		break;
	case 10:
		mItemDescription->setCaption(element->Attribute("Earring"));
		break;
	case 11:
		mItemDescription->setCaption(element->Attribute("Cockroach"));
		break;
	case 12:
		mItemDescription->setCaption(element->Attribute("Rat"));
		break;
	}
}

bool PauseUIState::keyPressed( const OIS::KeyEvent &e ) 
{
	if(e.key == OIS::KC_RETURN)
	{
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
	}
	return true;
}

bool PauseUIState::keyReleased( const OIS::KeyEvent &e ) 
{
	//EXITS THE GAME
	if( e.key == OIS::KC_ESCAPE ) 
	{
 		GameManager::getSingletonPtr()->requestShutdown();
	}
	//MOVES TO PREVIOUS SCREEN
	else if( e.key == OIS::KC_1 ) 
	{
		if(pauseScreen == "GameOptionsShown")
		{
			resetWarning();
		}

		if(pauseSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		if((currentScreen - 1) < 0)
		{
			currentScreen = 4;
		}
		else
		{
			currentScreen--;
		}
	}
	//MOVES TO NEXT SCREEN
	else if( e.key == OIS::KC_2 ) 
	{
		if(pauseScreen == "GameOptionsShown")
		{
			resetWarning();
		}

		if(pauseSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		if((currentScreen + 1) > 4)
		{
			currentScreen = 0;
		}
		else
		{
			currentScreen++;
		}
	}

	if(pauseScreen == "GameOptionsShown")
	{
		if(e.key == OIS::KC_DOWN)
		{
			if(warningDisplayed == false)
			{
				resetWarning();
				saveGameSlot++;
			}

			if(saveGameSlot == 5)
				saveGameSlot = 1;
		}
		else if(e.key == OIS::KC_UP)
		{
			if(warningDisplayed == false)
			{
				resetWarning();
				saveGameSlot--;
			}

			if(saveGameSlot == 0)
			{
				saveGameSlot = 4;
			}
		}
		
		if(e.key == OIS::KC_SPACE)
		{
			displayWarning();
		}

		if(e.key == OIS::KC_E && warningDisplayed == true)
		{
			if(saveGameSlot != 4)
			{
				resetWarning();
				warningDisplayed = false;
				loadSaveGameFiles();
			}
			else if(saveGameSlot == 4)
			{
				GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
				GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
			}
		}
		else if(e.key == OIS::KC_X && warningDisplayed == true)
		{
			resetWarning();
			warningDisplayed = false;
		}
	}
	else if(pauseScreen == "MissionShown")
	{
		if(e.key == OIS::KC_DOWN)
		{
			if(pauseSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
				ERRCHECK(audio_result);
			}

			if(HilightedQuest < TotalQuests)
			{
				HilightedQuest++;
			}
		}
		else if(e.key == OIS::KC_UP)
		{
			if(pauseSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
				ERRCHECK(audio_result);
			}

			if(HilightedQuest != 0)
			{
				HilightedQuest--;
			}
		}
	}
	else if(pauseScreen == "InventoryShown")
	{
		if(e.key == OIS::KC_DOWN)
		{
			if(HilightedItem < 13)
				HilightedItem++;

			if(HilightedItem == 13)
			{
				HilightedItem = 1;
			}
		}
		else if(e.key == OIS::KC_UP)
		{
			if(HilightedItem > 0)
				HilightedItem--;

			if(HilightedItem == 0)
			{
				HilightedItem = 12;
			}

		}

		moveItemCursor();
	}	

	return true;
}

void PauseUIState::moveItemCursor()
{
	switch(HilightedItem)
	{
	case 1:
		mRatTextBox->setColour(Ogre::ColourValue::Black);
		mCigarettesTextBox->setColour(Ogre::ColourValue::Red);
		mPainKillersTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 2:
		mCigarettesTextBox->setColour(Ogre::ColourValue::Black);
		mPainKillersTextBox->setColour(Ogre::ColourValue::Red);
		mMoneyTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 3:
		mPainKillersTextBox->setColour(Ogre::ColourValue::Black);
		mMoneyTextBox->setColour(Ogre::ColourValue::Red);
		mSyringeTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 4:
		mMoneyTextBox->setColour(Ogre::ColourValue::Black);
		mSyringeTextBox->setColour(Ogre::ColourValue::Red);
		mSoapTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 5:
		mSyringeTextBox->setColour(Ogre::ColourValue::Black);
		mSoapTextBox->setColour(Ogre::ColourValue::Red);
		mSoapMoldTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 6:
		mSoapTextBox->setColour(Ogre::ColourValue::Black);
		mSoapMoldTextBox->setColour(Ogre::ColourValue::Red);
		mCellKeyTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 7:
		mSoapMoldTextBox->setColour(Ogre::ColourValue::Black);
		mCellKeyTextBox->setColour(Ogre::ColourValue::Red);
		mCellKeyCopyTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 8:
		mCellKeyTextBox->setColour(Ogre::ColourValue::Black);
		mCellKeyCopyTextBox->setColour(Ogre::ColourValue::Red);
		mPendantTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 9:
		mCellKeyCopyTextBox->setColour(Ogre::ColourValue::Black);
		mPendantTextBox->setColour(Ogre::ColourValue::Red);
		mEaringTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 10:
		mPendantTextBox->setColour(Ogre::ColourValue::Black);
		mEaringTextBox->setColour(Ogre::ColourValue::Red);
		mCockroachTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 11:
		mEaringTextBox->setColour(Ogre::ColourValue::Black);
		mCockroachTextBox->setColour(Ogre::ColourValue::Red);
		mRatTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 12:
		mCockroachTextBox->setColour(Ogre::ColourValue::Black);
		mRatTextBox->setColour(Ogre::ColourValue::Red);
		mCigarettesTextBox->setColour(Ogre::ColourValue::Black);
		break;
	}
}

void PauseUIState::inventoryShown()
{
	TiXmlDocument loadDoc("Inventory.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Inventory");

	mCigarettesTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("Cigarettes")));
	mPainKillersTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("PainKillers")));
	mMoneyTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("Money")));
	mSyringeTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("Syringe")));
	mSoapTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("Soap")));
	mSoapMoldTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("SoapMold")));
	mCellKeyTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("CellKey")));
	mCellKeyCopyTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("CellKeyCopy")));
	mPendantTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("Pendant")));
	mEaringTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("Earing")));
	mCockroachTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("Cockroach")));
	mRatTotalTextBox->setCaption(static_cast<const char*>(element->Attribute("Rat")));

	inventoryOverlay->show();
	mapOverlay->hide();
	mMapOverlay->hide();
	missionsOverlay->hide();
	gameOptionsOverlay->hide();
	statusOverlay->hide();

	loadItem();
	loadItemDescription();
	mItemOverlay->show();
}

//ENTERED WHEN THE MAP SCREEN IS SHOWN

void PauseUIState::mapShown()
{
	mapOverlay->show();
	mMapOverlay->show();
	inventoryOverlay->hide();
	missionsOverlay->hide();
	gameOptionsOverlay->hide();
	statusOverlay->hide();
	mItemOverlay->hide();
}

//ENTERED WHEN THE MISSIONS SCREEN IS SHOWN

void PauseUIState::missionsShown()
{
	TiXmlDocument loadDoc("Inventory.xml");
	loadDoc.LoadFile();

	TiXmlElement *QuestElement = loadDoc.RootElement()->FirstChildElement("CurrentQuest");
	TiXmlElement *NameElement = loadDoc.RootElement()->FirstChildElement("Name");


	//Check Lin Ma's Quests
	if(atoi(QuestElement->Attribute("linMasPickAFaction")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("linMasPickAFaction");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("linMasBugInTheSystem")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("linMasBugInTheSystem");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("linMasMoreBugsInTheSystem")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("linMasMoreBugsInTheSystem");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("linMasTheRatScuttle")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("linMasTheRatScuttle");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("linMasWhoIsEscaping")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("linMasWhoIsEscaping");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("linMasTraining")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("linMasTraining");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("linMasFinalQuest")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("linMasFinalQuest");
		QuestCounter++;
	}

	//Check Johnny666's Quests
	if(atoi(QuestElement->Attribute("johnny666CheatALittle")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("johnny666CheatALittle");
		QuestCounter++;
	}
	
	//Check Gemini Faction Quests
	if(atoi(QuestElement->Attribute("geminiThePrideOfJolly")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("geminiThePrideOfJolly");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("geminiKnockEmOut")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("geminiKnockEmOut");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("geminiKeyToThePlan")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("geminiKeyToThePlan");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("geminiTalkToTheSword")) == 1)
	{		
		QuestName[QuestCounter] = NameElement->Attribute("geminiTalkToTheSword");
		QuestCounter++;
	}

	//Check Chaos Cult Faction Quests

	if(atoi(QuestElement->Attribute("jollyTwoPendants")) == 1)
	{		
		QuestName[QuestCounter] = NameElement->Attribute("jollyTwoPendants");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("jollyKnockEmOut")) == 1)
	{		
		QuestName[QuestCounter] = NameElement->Attribute("jollyKnockEmOut");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("jollyKeyToThePlan")) == 1)
	{		
		QuestName[QuestCounter] = NameElement->Attribute("jollyKeyToThePlan");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("jollyTalkToTheSword")) == 1)
	{		
		QuestName[QuestCounter] = NameElement->Attribute("jollyTalkToTheSword");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("santosoKeyToThePlan")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("santosoKeyToThePlan");
		QuestCounter++;
	}

	if(atoi(QuestElement->Attribute("santosoTalkToTheSword")) == 1)
	{
		QuestName[QuestCounter] = NameElement->Attribute("santosoTalkToTheSword");
		QuestCounter++;
	}

	getQuestDescription();
	TotalQuests = QuestCounter;

	QuestCounter = 0;

	missionsOverlay->show();
	mapOverlay->hide();
	mMapOverlay->hide();
	inventoryOverlay->hide();
	gameOptionsOverlay->hide();
	statusOverlay->hide();
	mItemOverlay->hide();
}

void PauseUIState::getQuestDescription()
{

	TiXmlDocument loadDoc("QuestDescription.xml");
	loadDoc.LoadFile();

	if(QuestCounter >= 1)
	{
		TiXmlElement *elementName = loadDoc.RootElement()->FirstChildElement("QuestCaptionName");

		for(int x = 0; x < QuestCounter; x++)
		{
			switch(x)
			{
			case 0:
				mQuestLogSlot01->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 1:
				mQuestLogSlot02->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 2:
				mQuestLogSlot03->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 3:
				mQuestLogSlot04->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 4:
				mQuestLogSlot05->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 5:
				mQuestLogSlot06->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 6:
				mQuestLogSlot07->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 7:
				mQuestLogSlot08->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 8:
				mQuestLogSlot09->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 9:
				mQuestLogSlot10->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 10:
				mQuestLogSlot11->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			case 11:
				mQuestLogSlot12->setCaption(elementName->Attribute(QuestName[x].c_str()));
				break;
			}
		}
	}

	if(TotalQuests != 0)
	{
		TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Description");

		switch(HilightedQuest)
		{
		case 1:
			mQuestLogSlot01->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot02->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[0].c_str()));
			break;
		case 2:
			mQuestLogSlot01->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot02->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot03->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[1].c_str()));
			break;
		case 3:
			mQuestLogSlot02->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot03->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot04->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[2].c_str()));
			break;
		case 4:
			mQuestLogSlot03->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot04->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot05->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[3].c_str()));
			break;
		case 5:
			mQuestLogSlot04->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot05->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot06->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[4].c_str()));
			break;
		case 6:
			mQuestLogSlot05->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot06->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot07->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[5].c_str()));
			break;
		case 7:
			mQuestLogSlot06->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot07->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot08->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[6].c_str()));
			break;
		case 8:
			mQuestLogSlot07->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot08->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot09->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[7].c_str()));
			break;
		case 9:
			mQuestLogSlot08->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot09->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot10->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[8].c_str()));
			break;
		case 10:
			mQuestLogSlot09->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot10->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot11->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[9].c_str()));
			break;
		case 11:
			mQuestLogSlot10->setColour(Ogre::ColourValue::Black);
			mQuestLogSlot11->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot12->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[10].c_str()));
			break;
		case 12:
			mQuestLogSlot12->setColour(Ogre::ColourValue::Red);
			mQuestLogSlot11->setColour(Ogre::ColourValue::Black);
			mQuestDescriptionTextBox->setCaption(element->Attribute(QuestName[11].c_str()));
			break;
		}                                   
	} 
}

//ENTERED WHEN THE STATUS SCREEN IS SHOWN

void PauseUIState::statusShown()
{
	loadCharacterInfo();

	TiXmlDocument loadDoc("Attributes.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Stat");

	mName->setCaption(name);
	mUpperBody->setCaption(static_cast<const char*>(element->Attribute("UpperBody")));
	mLowerBody->setCaption(static_cast<const char*>(element->Attribute("LowerBody")));
	mHealth->setCaption(static_cast<const char*>(element->Attribute("Health")));
	mGrapple->setCaption(static_cast<const char*>(element->Attribute("Grapple")));

	statusOverlay->show();
	inventoryOverlay->hide();
	missionsOverlay->hide();
	gameOptionsOverlay->hide();
	mapOverlay->hide();
	mMapOverlay->hide();
	mItemOverlay->hide();
}

//ENTERED WHEN THE GAME OPTIONS SCREEN IS SHOWN

void PauseUIState::gameOptionsShown()
{
	gameOptionsOverlay->show();

	switch(saveGameSlot)
	{
	case 1:
		mExitGameTextBox->setColour(Ogre::ColourValue::Black);
		mSaveGameSlot01->setColour(Ogre::ColourValue::Red);
		mSaveGameSlot02->setColour(Ogre::ColourValue::Black);
		break;
	case 2:
		mSaveGameSlot01->setColour(Ogre::ColourValue::Black);
		mSaveGameSlot02->setColour(Ogre::ColourValue::Red);
		mSaveGameSlot03->setColour(Ogre::ColourValue::Black);
		break;
	case 3:
		mSaveGameSlot02->setColour(Ogre::ColourValue::Black);
		mSaveGameSlot03->setColour(Ogre::ColourValue::Red);
		mExitGameTextBox->setColour(Ogre::ColourValue::Black);
		break;
	case 4:
		mSaveGameSlot03->setColour(Ogre::ColourValue::Black);
		mExitGameTextBox->setColour(Ogre::ColourValue::Red);
		mSaveGameSlot01->setColour(Ogre::ColourValue::Black);
		break;
	}
	inventoryOverlay->hide();
	missionsOverlay->hide();
	mapOverlay->hide();
	mMapOverlay->hide();
	statusOverlay->hide();
	mItemOverlay->hide();
}

void PauseUIState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	pauseMusicSetting = atoi(element->Attribute("Music"));
	pauseSFXSetting = atoi(element->Attribute("SFX"));
	pauseControls = atoi(element->Attribute("Keyboard"));
	pauseInvert = atoi(element->Attribute("Invert"));
}

void PauseUIState::loadStats( void )
{
	TiXmlDocument loadDoc("attributes.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Stat");

	pCurrentLevel = atoi(element->Attribute("Level"));
	pUpperBodyStat = atoi(element->Attribute("UpperBody"));
	pLowerBodyStat = atoi(element->Attribute("LowerBody"));
	pHealthStat = atoi(element->Attribute("Health"));
	pGrappleStat = atoi(element->Attribute("Grapple"));
}

PauseUIState* PauseUIState::getSingletonPtr( void ) {
    if( !mPauseUIState ) {
        mPauseUIState = new PauseUIState();
		UIStateFactory::getSingletonPtr()->registerState(mPauseUIState->getName(), mPauseUIState);
    }

    return mPauseUIState;
}
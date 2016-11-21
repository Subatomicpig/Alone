#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>

#include "tinyxml.h"

#include "States\LoadGameUIState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

#include <stdio.h>
#include <cctype>
#include <string.h>

std::string filename;
bool exist;

int loadLevel;
int loadUpperBody;
int loadLowerBody;
int loadHealth;
int loadGrapple;

int loadPainkillers;
int loadSteroids;
int loadMoney;

using namespace Ogre;
using namespace Alone;

Alone::GamePadControllerList mLoadGameGamePad;
LoadGameUIState* LoadGameUIState::mLoadGameUIState;

LoadGameUIState::LoadGameUIState(void)
{
	mClassName = "LoadGameUI";
}

LoadGameUIState::~LoadGameUIState(void)
{
}

void LoadGameUIState::enter(void)
{
	selectedOption = 0;
	stickReleased = true;
	selectionReleased = true;
	filename = "";
	exist = false;

	mOverlayMgr = OverlayManager::getSingletonPtr();
	mLoadGameOneOverlay = mOverlayMgr->getByName("Overlay/LoadGameOne");
	mLoadGameTwoOverlay = mOverlayMgr->getByName("Overlay/LoadGameTwo");
	mLoadGameThreeOverlay = mOverlayMgr->getByName("Overlay/LoadGameThree");
	
	mLoadGameControlsOverlay = mOverlayMgr->getByName("Overlay/LoadGameControls");

	mLoadGamePanel = static_cast<Ogre::OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "LoadGamePanel"));
	mLoadGamePanel->setMetricsMode(Ogre::GMM_PIXELS);
	mLoadGamePanel->setPosition(0,0);
	mLoadGamePanel->setDimensions(200,480);

	mSaveGameNameTextBox = static_cast<Ogre::TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "SaveGameNameTextBox"));
	mSaveGameNameTextBox->setDimensions(50,50);
	mSaveGameNameTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mSaveGameNameTextBox->setPosition(200, 460);
	mSaveGameNameTextBox->setFontName("MyFont");
	mSaveGameNameTextBox->setCharHeight(20);
	mSaveGameNameTextBox->setColour(Ogre::ColourValue::Black);

	mLoadGamePanel->addChild(mSaveGameNameTextBox);

	mLoadGameOneOverlay->add2D(mLoadGamePanel);
	mLoadGameTwoOverlay->add2D(mLoadGamePanel);
	mLoadGameThreeOverlay->add2D(mLoadGamePanel);

	mLoadGameGamePad.push_back(new Alone::GamePadController(0, 4000));

	input = InputManager::getSingletonPtr();
	input->addKeyListener(mLoadGameUIState, mClassName);
	input->addJoystickListener(mLoadGameUIState, mClassName);

	GameManager::getSingletonPtr()->getWorkState()->focus();

}

void LoadGameUIState::update(Ogre::Real seconds)
{
	mLoadGameControlsOverlay->show();
	moveJoystick(seconds);

	if(selectedOption == 0)
	{
		LoadGameOne();
	}
	else if(selectedOption == 1)
	{
		LoadGameTwo();
	}
	else if(selectedOption == 2)
	{
		LoadGameThree();
	}

	if(input->isButtonDown(0, 1) && stickReleased == true)
	{
		LoadCharacterInfo();
		LoadInventory();

		if(exist == true && selectionReleased == false)
		{
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("LevelTest"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
		}
		else
			exist = false;
	}
	else if(input->isButtonDown(0, 2))
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
	}
	else if(!input->isButtonDown(0,1))
	{
		selectionReleased = false;
	}
}

void LoadGameUIState::LoadGameOne()
{
	mLoadGameOneOverlay->show();
	mLoadGameTwoOverlay->hide();
	mLoadGameThreeOverlay->hide();

	TiXmlDocument loadDoc("SG01.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	if(atoi(element->Attribute("Exist")) == 1)
	{
		mSaveGameNameTextBox->setCaption(element->Attribute("Name"));
	}
	else
	{
		mSaveGameNameTextBox->setCaption("<empty>");
	}
}

void LoadGameUIState::LoadGameTwo()
{
	mLoadGameOneOverlay->hide();
	mLoadGameTwoOverlay->show();
	mLoadGameThreeOverlay->hide();

	TiXmlDocument loadDoc("SG02.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	if(atoi(element->Attribute("Exist")) == 1)
	{
		mSaveGameNameTextBox->setCaption(element->Attribute("Name"));
	}
	else
	{
		mSaveGameNameTextBox->setCaption("<empty>");
	}
}

void LoadGameUIState::LoadGameThree()
{
	mLoadGameOneOverlay->hide();
	mLoadGameTwoOverlay->hide();
	mLoadGameThreeOverlay->show();

	TiXmlDocument loadDoc("SG03.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	if(atoi(element->Attribute("Exist")) == 1)
	{
		mSaveGameNameTextBox->setCaption(element->Attribute("Name"));
	}
	else
	{
		mSaveGameNameTextBox->setCaption("<empty>");
	}
}

void LoadGameUIState::LoadCharacterInfo()
{
	switch(selectedOption)
	{
	case 0:
		filename = "SG01.xml";
		break;
	case 1:
		filename = "SG02.xml";
		break;
	case 2:
		filename = "SG03.xml";
		break;
	}

	TiXmlDocument loadDoc(filename.c_str());
	loadDoc.LoadFile();
	loadStats();
	updateStats();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	if(atoi(element->Attribute("Exist")) == 1)
	{
		exist = true;

		TiXmlDocument doc;

		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
		doc.LinkEndChild(decl);

		TiXmlElement *root = new TiXmlElement("Character");
		doc.LinkEndChild(root);

		TiXmlElement *info = new TiXmlElement("Info");
		root->LinkEndChild(info);
		info->SetAttribute("Exist", true);
		info->SetAttribute("Name", element->Attribute("Name"));
		info->SetAttribute("Skin", atoi(element->Attribute("Skin")));
		info->SetAttribute("Shirt", atoi(element->Attribute("Shirt")));
		info->SetAttribute("Hair", atoi(element->Attribute("Hair")));

		doc.SaveFile("CharacterEditor.xml");
	}
}

void LoadGameUIState::LoadInventory()
{
	switch(selectedOption)
	{
	case 0:
		filename = "SG01.xml";
		break;
	case 1:
		filename = "SG02.xml";
		break;
	case 2:
		filename = "SG03.xml";
		break;
	}

	TiXmlDocument loadDoc(filename.c_str());
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	if(atoi(element->Attribute("Exist")) == 1)
	{
		element = loadDoc.RootElement()->FirstChildElement("Inventory");

		TiXmlDocument doc;

		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
		doc.LinkEndChild(decl);

		TiXmlElement *root = new TiXmlElement("Inventory");
		doc.LinkEndChild(root);

		TiXmlElement *inventory = new TiXmlElement("Inventory");
		root->LinkEndChild(inventory);
		
		loadPainkillers = atoi(element->Attribute("PainKillers"));
		loadMoney = atoi(element->Attribute("Money"));
		loadSteroids = atoi(element->Attribute("Syringe"));

		updateMerchandise();

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
		QuestTracker->SetAttribute("johnny66680RepQuestCompleted", atoi(element->Attribute("johnny66680RepQuestCompleted")));
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

		doc.SaveFile("Inventory.xml");
	}
}

void LoadGameUIState::exit()
{
	mLoadGameOneOverlay->hide();
	mLoadGameTwoOverlay->hide();
	mLoadGameThreeOverlay->hide();
	mLoadGameControlsOverlay->hide();

	mLoadGameOneOverlay->remove2D(mLoadGamePanel);
	mLoadGameTwoOverlay->remove2D(mLoadGamePanel);
	mLoadGameThreeOverlay->remove2D(mLoadGamePanel);

	mOverlayMgr->destroyOverlayElement("SaveGameNameTextBox");
	mOverlayMgr->destroyOverlayElement("LoadGamePanel");	

	input->removeKeyListener(mLoadGameUIState);
	input->removeJoystickListener(mLoadGameUIState);

	GameManager::getSingletonPtr()->getWorkState()->focus();
}

void LoadGameUIState::moveJoystick(Ogre::Real seconds)
{
	mLoadGameGamePad[0]->update(seconds);

	Real xLeft = mLoadGameGamePad[0]->getX(0);

	if(xLeft > 0.5f && stickReleased == true)
	{
		selectedOption++;
		stickReleased = false;

		if(selectedOption == 3)
		{
			selectedOption = 0;
		}
	}
	else if(xLeft < -0.5f && stickReleased == true)
	{
		selectedOption--;
		stickReleased = false;

		if(selectedOption == -1)
		{
			selectedOption = 2;
		}
	}
	else if(xLeft > -0.5f && xLeft < 0.5f && stickReleased == false)
	{
		stickReleased = true;
	} 
}

bool LoadGameUIState::keyPressed(const OIS::KeyEvent &e)
{
	return true;
}

bool LoadGameUIState::keyReleased(const OIS::KeyEvent &e)
{
	if(e.key == OIS::KC_RIGHT)
	{
		selectedOption++;

		if(selectedOption == 3)
		{
			selectedOption = 0;
		}
	}

	if(e.key == OIS::KC_LEFT)
	{
		selectedOption--;

		if(selectedOption == -1)
		{
			selectedOption = 2;
		}
	}

	if(e.key == OIS::KC_RETURN)
	{
		LoadCharacterInfo();
		LoadInventory();

		if(exist == true)
		{
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("LevelTest"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
		}
	}

	if(e.key == OIS::KC_ESCAPE)
	{
		GameManager::getSingletonPtr()->requestShutdown();
	}

	if(e.key == OIS::KC_X)
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
	}

	return true;
}

void LoadGameUIState::loadStats( void )
{
	TiXmlDocument loadDoc(filename.c_str());
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Stat");

	loadLevel = atoi(element->Attribute("Level"));
	loadUpperBody = atoi(element->Attribute("UpperBody"));
	loadLowerBody = atoi(element->Attribute("LowerBody"));
	loadHealth = atoi(element->Attribute("Health"));
	loadGrapple = atoi(element->Attribute("Grapple"));
}

void LoadGameUIState::updateStats( void )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Attributes");
	doc.LinkEndChild(root);

	TiXmlElement *stat = new TiXmlElement("Stat");
	root->LinkEndChild(stat);
	stat->SetAttribute("Level", loadLevel);
	stat->SetAttribute("UpperBody", loadUpperBody);
	stat->SetAttribute("LowerBody", loadLowerBody);
	stat->SetAttribute("Health", loadHealth);
	stat->SetAttribute("Grapple", loadGrapple);

	doc.SaveFile("attributes.xml");
}

void LoadGameUIState::updateMerchandise( void )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Merchandise");
	doc.LinkEndChild(root);

	TiXmlElement *item = new TiXmlElement("Item");
	root->LinkEndChild(item);
	item->SetAttribute("PainKillers", loadPainkillers);
	item->SetAttribute("Money", loadMoney);
	item->SetAttribute("Syringe", loadSteroids);

	doc.SaveFile("merchandise.xml");
}

LoadGameUIState* LoadGameUIState::getSingletonPtr(void)
{
	if(!mLoadGameUIState)
	{
		mLoadGameUIState = new LoadGameUIState();
		UIStateFactory::getSingletonPtr()->registerState(mLoadGameUIState->getName(), mLoadGameUIState);
	}

	return mLoadGameUIState;
}
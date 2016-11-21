#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>

#include "tinyxml.h"

#include "States\StoreState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

using namespace Ogre;
using namespace Alone;

bool storeMusicSetting;
bool storeSFXSetting;
bool storeControls;
bool storeInvert;

int currentMoney;
int currentSteroid;
int currentPainkillers;
int currentCigarettes;

int steroidCost = 55;
int painkillerCost = 10;

int maxSteroids = 99;
int maxPainkillers = 1;

Alone::GamePadControllerList storeGamePads;

StoreState* StoreState::mStoreState;

void StoreState::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

StoreState::StoreState( void ) 
{
	mClassName = "StoreState";
}

StoreState::~StoreState( void ) 
{
}

void StoreState::enter( void ) 
{
	selectedOption = 0;
	stickReleased = true;

	loadOptions();
	loadMerchandise();

	storeOverlayMgr   = OverlayManager::getSingletonPtr();
    storeOverlay = storeOverlayMgr->getByName( "Overlay/StoreState" );
	storeCursorOverlay = storeOverlayMgr->getByName("Overlay/StoreSelect");
	syringeImage = storeOverlayMgr->getByName("Overlay/Syringe");
	painkillersImage = storeOverlayMgr->getByName("Overlay/Painkillers");

	syringeImage->setScroll(1.1, -0.3);
	painkillersImage->setScroll(1.1, -0.3);

	storePanel = static_cast<OverlayContainer*>(storeOverlayMgr->createOverlayElement("Panel", "StorePanel"));
	storePanel->setMetricsMode(Ogre::GMM_PIXELS);
	storePanel->setPosition(0, 0);

	moneyText = static_cast<TextAreaOverlayElement*>(storeOverlayMgr->createOverlayElement("TextArea", "Money"));
	moneyText->setMetricsMode(Ogre::GMM_PIXELS);
	moneyText->setPosition(650, 625);
	moneyText->setFontName("MyFont");
	moneyText->setCharHeight(42);
	moneyText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

	steroidText = static_cast<TextAreaOverlayElement*>(storeOverlayMgr->createOverlayElement("TextArea", "Steroids"));
	steroidText->setMetricsMode(Ogre::GMM_PIXELS);
	steroidText->setPosition(210, 180);
	steroidText->setFontName("MyFont");
	steroidText->setCharHeight(24);
	steroidText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

	painkillerText = static_cast<TextAreaOverlayElement*>(storeOverlayMgr->createOverlayElement("TextArea", "Painkillers"));
	painkillerText->setMetricsMode(Ogre::GMM_PIXELS);
	painkillerText->setPosition(210, 240);
	painkillerText->setFontName("MyFont");
	painkillerText->setCharHeight(24);
	painkillerText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

	descriptionText = static_cast<TextAreaOverlayElement*>(storeOverlayMgr->createOverlayElement("TextArea", "Description"));
	descriptionText->setMetricsMode(Ogre::GMM_PIXELS);
	descriptionText->setPosition(535, 450);
	descriptionText->setFontName("MyFont");
	descriptionText->setCharHeight(24);
	descriptionText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

	exitStoreText = static_cast<TextAreaOverlayElement*>(storeOverlayMgr->createOverlayElement("TextArea", "ExitStore"));
	exitStoreText->setMetricsMode(Ogre::GMM_PIXELS);
	exitStoreText->setPosition(210, 600);
	exitStoreText->setFontName("MyFont");
	exitStoreText->setCharHeight(24);
	exitStoreText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

	storePanel->addChild(moneyText);
	storePanel->addChild(steroidText);
	storePanel->addChild(painkillerText);
	storePanel->addChild(descriptionText);
	storePanel->addChild(exitStoreText);
	currentInfoOverlay = storeOverlayMgr->create("CurrentInfoOverlay");
	currentInfoOverlay->add2D(storePanel);
	currentInfoOverlay->setZOrder(600);

	currentInfoOverlay->show();

	moveCursor();

	storeGamePads.push_back(new Alone::GamePadController(0, 4000));

	input = InputManager::getSingletonPtr();
	input->addKeyListener(mStoreState, mClassName);
	input->addJoystickListener(mStoreState, mClassName);

	GameManager::getSingletonPtr()->getWorkState()->defocus();

	channel1 = 0;
	channel2 = 0;
	channel3 = 0;

	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Pills/VGD_SFX_Pills3.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Selection);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Menu/Metal/Short Switch.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_SelectionSyringe);
	ERRCHECK(audio_result);
}

void StoreState::update( Ogre::Real seconds ) 
{
	storeOverlay->show();
	storeCursorOverlay->show();

	system->update();

	updateMerchandise();

	moneyText->setCaption("$" + Ogre::StringConverter::toString(currentMoney));
	steroidText->setCaption("Steroids:    $55     " + Ogre::StringConverter::toString(currentSteroid));
	painkillerText->setCaption("Painkillers:   $10    " + Ogre::StringConverter::toString(currentPainkillers));
	exitStoreText->setCaption("Exit Store");

	moveJoystick(seconds);
	moveCursor();

	if(input->isButtonDown(0,1))
	{
		if(storeSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
			ERRCHECK(audio_result);
		}

		switch(selectedOption)
		{
		case 0:
			if(currentMoney >= steroidCost && currentSteroid < maxSteroids)
			{
				currentMoney -= steroidCost;
				currentSteroid++;
			}
			break;
		case 1:
			if(currentMoney >= painkillerCost && currentPainkillers < maxPainkillers)
			{
				currentMoney -= painkillerCost;
				currentPainkillers++;
			}
			break;
		case 2:
			//GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("CharacterEditor"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
			break;
		}
	}
}

void StoreState::exit( void ) 
{
	storeOverlay->hide();
	storeCursorOverlay->hide();
	currentInfoOverlay->hide();

	//Destroy overlay elements
	storeOverlayMgr->destroyOverlayElement(storePanel);
	storeOverlayMgr->destroyOverlayElement(exitStoreText);
	storeOverlayMgr->destroyOverlayElement(moneyText);
	storeOverlayMgr->destroyOverlayElement(steroidText);
	storeOverlayMgr->destroyOverlayElement(painkillerText);
	storeOverlayMgr->destroyOverlayElement(descriptionText);
	storeOverlayMgr->destroy(currentInfoOverlay);


	input->removeKeyListener(mStoreState);
	input->removeJoystickListener(mStoreState);

	audio_result = cursor_Selection->release();
	ERRCHECK(audio_result);

	audio_result = system->close();
	ERRCHECK(audio_result);

	audio_result = system->release();
	ERRCHECK(audio_result);


	GameManager::getSingletonPtr()->getWorkState()->focus();
}

void StoreState::moveJoystick(Ogre::Real seconds)
{
	storeGamePads[0]->update(seconds);

	Real yLeft = storeGamePads[0]->getY(0);

	if(yLeft > 0.5f && stickReleased == true)
	{
		selectedOption--;

		if(selectedOption == -1)
		{
			selectedOption = 2;
		}

		stickReleased = false;
	}
	else if(yLeft < -0.5f && stickReleased == true)
	{
		selectedOption++;

		if(selectedOption == 3)
		{
			selectedOption = 0;
		}

		stickReleased = false;
	}
	else if(yLeft > -0.5f && yLeft < 0.5 && stickReleased == false)
	{
		stickReleased = true;
	}
}
void StoreState::moveCursor(void)
{
	switch(selectedOption)
	{
	case 0:
		storeCursorOverlay->setScroll(0.39, -0.44);
		descriptionText->setCaption("Boosts muscle power and \nprovides extraordinary \nstrength. This item, when \nused, gives you 3 exercise \npoints.");
		syringeImage->show();
		painkillersImage->hide();
		break;
	case 1:
		storeCursorOverlay->setScroll(0.39, -0.59);
		descriptionText->setCaption("Blocks out the pain.\nThis item heals the player\nin the arena, while in \ncombat.(You can only have \none of these in your \ninventory at all times.)");
		syringeImage->hide();
		painkillersImage->show();
		break;
	case 2:
		storeCursorOverlay->setScroll(0.39, -1.525);
		descriptionText->setCaption("");
		syringeImage->hide();
		painkillersImage->hide();
		break;
	}
}
bool StoreState::keyPressed( const OIS::KeyEvent &e ) 
{
	return true;
}

bool StoreState::keyReleased( const OIS::KeyEvent &e ) 
{
    if( e.key == OIS::KC_INSERT ) 
	{
     	GameManager::getSingletonPtr()->requestShutdown();
    }

	if(e.key == OIS::KC_DOWN)
	{
		selectedOption++;

		if(selectedOption == 3)
		{
			selectedOption = 0;
		}
	}

	if(e.key == OIS::KC_UP)
	{
		selectedOption--;

		if(selectedOption == -1)
		{
			selectedOption = 2;
		}
	}

	if(e.key == OIS::KC_RETURN)
	{
		if(storeSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
			ERRCHECK(audio_result);
		}

		switch(selectedOption)
		{
		case 0:
			if(currentMoney >= steroidCost && currentSteroid < maxSteroids)
			{
				currentMoney -= steroidCost;
				currentSteroid++;
			}
			break;
		case 1:
			if(currentMoney >= painkillerCost && currentPainkillers < maxPainkillers)
			{
				currentMoney -= painkillerCost;
				currentPainkillers++;
			}
			break;
		case 2:
			//GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("CharacterEditor"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
			break;
		}
	}
  
	return true;
}

void StoreState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	storeMusicSetting = atoi(element->Attribute("Music"));
	storeSFXSetting = atoi(element->Attribute("SFX"));
	storeControls = atoi(element->Attribute("Keyboard"));
	storeInvert = atoi(element->Attribute("Invert"));
}

void StoreState::loadMerchandise( void )
{
	TiXmlDocument loadDoc("Inventory.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Inventory");

	currentPainkillers = atoi(element->Attribute("PainKillers"));
	currentMoney = atoi(element->Attribute("Money"));
	currentSteroid = atoi(element->Attribute("Syringe"));
}

void StoreState::updateMerchandise( void )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Merchandise");
	doc.LinkEndChild(root);

	TiXmlElement *item = new TiXmlElement("Item");
	root->LinkEndChild(item);
	item->SetAttribute("PainKillers", currentPainkillers);
	item->SetAttribute("Money", currentMoney);
	item->SetAttribute("Syringe", currentSteroid);

	doc.SaveFile("merchandise.xml");
}

StoreState* StoreState::getSingletonPtr( void ) {
    if( !mStoreState ) {
        mStoreState = new StoreState();
		UIStateFactory::getSingletonPtr()->registerState(mStoreState->getName(), mStoreState);
    }

    return mStoreState;
}
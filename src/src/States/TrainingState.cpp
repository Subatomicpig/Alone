#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>

#include "tinyxml.h"

#include "States\TrainingState.h"
#include "Managers\GameManager.h"
#include "Managers\InputManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

using namespace Ogre;
using namespace Alone;

bool trainingMusicSetting;
bool trainingSFXSetting;
bool trainingControls;
bool trainingInvert;
bool buffed;

int currentLevel;
int currentSteroids;
int upperBodyStat;
int lowerBodyStat;
int healthStat;
int grappleStat;

int bUpperBody = 2;
int bGrapple = 1;
int cuGrapple = 2;
int cuUpperBody = 1;
int crHealth = 3;
int sLowerBody = 3;

int trainSteroids;
int trainCigarettes;
int trainMoney;
int trainPainkillers;

int maxStat = 25;

Alone::GamePadControllerList trainingGamePads;

TrainingState* TrainingState::mTrainingState;

void TrainingState::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

TrainingState::TrainingState( void ) 
{
	mClassName = "TrainingState";
}

TrainingState::~TrainingState( void ) 
{
}

void TrainingState::enter( void ) 
{
	selectedOption = 0;
	stickReleased = true;

	loadOptions();
	loadStats();
	loadMerchandise();

	trainingOverlayMgr   = OverlayManager::getSingletonPtr();
    trainingOverlay = trainingOverlayMgr->getByName( "Overlay/TrainingState" );
	trainingCursorOverlay = trainingOverlayMgr->getByName("Overlay/TrainingSelect");
	trainingPicture1 = trainingOverlayMgr->getByName("Overlay/BenchPress");
	trainingPicture2 = trainingOverlayMgr->getByName("Overlay/Curls");
	trainingPicture3 = trainingOverlayMgr->getByName("Overlay/Crunches");
	trainingPicture4 = trainingOverlayMgr->getByName("Overlay/Squats");

	trainingPicture1->setScroll(1.6, -0.4);
	trainingPicture1->setRotate(Degree(6));
	trainingPicture2->setScroll(1.6, -0.4);
	trainingPicture2->setRotate(Degree(6));
	trainingPicture3->setScroll(1.6, -0.4);
	trainingPicture3->setRotate(Degree(6));
	trainingPicture4->setScroll(1.6, -0.4);
	trainingPicture4->setRotate(Degree(6));

	workoutPanel = static_cast<OverlayContainer*>(trainingOverlayMgr->createOverlayElement("Panel", "TrainingPanel"));
	workoutPanel->setMetricsMode(Ogre::GMM_PIXELS);
	workoutPanel->setPosition(0, 0);

	levelText = static_cast<TextAreaOverlayElement*>(trainingOverlayMgr->createOverlayElement("TextArea", "Level"));
	levelText->setMetricsMode(Ogre::GMM_PIXELS);
	levelText->setPosition(718, 20);
	levelText->setFontName("MyFont");
	levelText->setCharHeight(20);
	levelText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

	steroidsText = static_cast<TextAreaOverlayElement*>(trainingOverlayMgr->createOverlayElement("TextArea", "Steroids"));
	steroidsText->setMetricsMode(Ogre::GMM_PIXELS);
	steroidsText->setPosition(403, 276);
	steroidsText->setFontName("MyFont");
	steroidsText->setCharHeight(20);
	steroidsText->setColour(Ogre::ColourValue(0.0, 0.0, 0.0, 1.0));

	upperBodyText = static_cast<TextAreaOverlayElement*>(trainingOverlayMgr->createOverlayElement("TextArea", "UpperBody"));
	upperBodyText->setMetricsMode(Ogre::GMM_PIXELS);
	upperBodyText->setPosition(718, 511);
	upperBodyText->setFontName("MyFont");
	upperBodyText->setCharHeight(20);
	upperBodyText->setColour(Ogre::ColourValue(0.0, 0.0, 0.0, 1.0));

	lowerBodyText = static_cast<TextAreaOverlayElement*>(trainingOverlayMgr->createOverlayElement("TextArea", "LowerBody"));
	lowerBodyText->setMetricsMode(Ogre::GMM_PIXELS);
	lowerBodyText->setPosition(718, 571);
	lowerBodyText->setFontName("MyFont");
	lowerBodyText->setCharHeight(20);
	lowerBodyText->setColour(Ogre::ColourValue(0.0, 0.0, 0.0, 1.0));

	healthText = static_cast<TextAreaOverlayElement*>(trainingOverlayMgr->createOverlayElement("TextArea", "Health"));
	healthText->setMetricsMode(Ogre::GMM_PIXELS);
	healthText->setPosition(718, 694);
	healthText->setFontName("MyFont");
	healthText->setCharHeight(20);
	healthText->setColour(Ogre::ColourValue(0.0, 0.0, 0.0, 1.0));

	grappleText = static_cast<TextAreaOverlayElement*>(trainingOverlayMgr->createOverlayElement("TextArea", "Grapple"));
	grappleText->setMetricsMode(Ogre::GMM_PIXELS);
	grappleText->setPosition(718, 633);
	grappleText->setFontName("MyFont");
	grappleText->setCharHeight(20);
	grappleText->setColour(Ogre::ColourValue(0.0, 0.0, 0.0, 1.0));
	
	workoutPanel->addChild(levelText);
	workoutPanel->addChild(steroidsText);
	workoutPanel->addChild(upperBodyText);
	workoutPanel->addChild(lowerBodyText);
	workoutPanel->addChild(healthText);
	workoutPanel->addChild(grappleText);
	statsOverlay = trainingOverlayMgr->create("StatsOverlay");
	statsOverlay->add2D(workoutPanel);
	statsOverlay->setZOrder(600);

	statsOverlay->show();

	moveCursor();

	trainingGamePads.push_back(new Alone::GamePadController(0, 4000));

	input = InputManager::getSingletonPtr();
	input->addKeyListener(mTrainingState, mClassName);
	input->addJoystickListener(mTrainingState, mClassName);

	GameManager::getSingletonPtr()->getWorkState()->defocus();

	channel1 = 0;
	channel2 = 0;
	channel3 = 0;

	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Menu/Paper/Scribbles 4.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Movement);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Menu/Metal/Short Switch.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Selection);
	ERRCHECK(audio_result);
}

void TrainingState::update( Ogre::Real seconds ) 
{
	trainingOverlay->show();
	trainingCursorOverlay->show();

	system->update();

	updateStats();
	updateMerchandise();

	//levelText->setCaption("Level: " + Ogre::StringConverter::toString(currentLevel));
	steroidsText->setCaption(Ogre::StringConverter::toString(trainSteroids));
	upperBodyText->setCaption(Ogre::StringConverter::toString(upperBodyStat));
	lowerBodyText->setCaption(Ogre::StringConverter::toString(lowerBodyStat));
	healthText->setCaption(Ogre::StringConverter::toString(healthStat));
	grappleText->setCaption(Ogre::StringConverter::toString(grappleStat));

	moveJoystick(seconds);
	moveCursor();

	if(input->isButtonDown(0,1))
	{
		if(trainingSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
			ERRCHECK(audio_result);
		}

		switch(selectedOption)
		{
		case 0:
			if(trainSteroids > 0)
			{
				if(upperBodyStat < maxStat)
				{
					upperBodyStat += bUpperBody;
					buffed = true;
				}
				if(grappleStat < maxStat)
				{
					grappleStat += bGrapple;
					buffed = true;
				}
				if(buffed)
				{
					currentLevel++;
					trainSteroids--;
					buffed = false;
				}
			}
			break;
		case 1:
			if(trainSteroids > 0)
			{
				if(grappleStat < maxStat)
				{
					grappleStat += cuGrapple;
					buffed = true;
				}
				if(upperBodyStat < maxStat)
				{
					upperBodyStat += cuUpperBody;
					buffed = true;
				}
				if(buffed)
				{
					currentLevel++;
					trainSteroids--;
					buffed = false;
				}
			}
			break;
		case 2:
			if(trainSteroids > 0)
			{
				if(healthStat < maxStat)
				{
					healthStat += crHealth;
					buffed = true;
				}
				if(buffed)
				{
					currentLevel++;
					trainSteroids--;
					buffed = false;
				}
			}
			break;
		case 3:
			if(trainSteroids > 0)
			{
				if(lowerBodyStat < maxStat)
				{
					lowerBodyStat += sLowerBody;
					buffed = true;
				}
				if(buffed)
				{
					currentLevel++;
					trainSteroids--;
					buffed = false;
				}
			}
			break;
		case 4:
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
			break;
		}
		if(upperBodyStat > maxStat)
		{
			upperBodyStat = maxStat;
		}
		if(lowerBodyStat > maxStat)
		{
			lowerBodyStat = maxStat;
		}
		if(healthStat > maxStat)
		{
			healthStat = maxStat;
		}
		if(grappleStat > maxStat)
		{
			grappleStat = maxStat;
		}
	}
}

void TrainingState::exit( void ) 
{
    trainingOverlay->hide();
	trainingCursorOverlay->hide();
	statsOverlay->hide();
	trainingPicture1->hide();
	trainingPicture2->hide();
	trainingPicture3->hide();
	trainingPicture4->hide();

	//Destroy overlay elements
	trainingOverlayMgr->destroyOverlayElement(workoutPanel);
	trainingOverlayMgr->destroyOverlayElement(levelText);
	trainingOverlayMgr->destroyOverlayElement(steroidsText);
	trainingOverlayMgr->destroyOverlayElement(upperBodyText);
	trainingOverlayMgr->destroyOverlayElement(lowerBodyText);
	trainingOverlayMgr->destroyOverlayElement(healthText);
	trainingOverlayMgr->destroyOverlayElement(grappleText);
	trainingOverlayMgr->destroy(statsOverlay);


	input->removeKeyListener(mTrainingState);
	input->removeJoystickListener(mTrainingState);

	audio_result = cursor_Movement->release();
	ERRCHECK(audio_result);

	audio_result = cursor_Selection->release();
	ERRCHECK(audio_result);

	audio_result = system->close();
	ERRCHECK(audio_result);

	audio_result = system->release();
	ERRCHECK(audio_result);


	GameManager::getSingletonPtr()->getWorkState()->focus();
}

void TrainingState::moveJoystick(Ogre::Real seconds)
{
	trainingGamePads[0]->update(seconds);

	Real yLeft = trainingGamePads[0]->getY(0);

	if(yLeft > 0.5f && stickReleased == true)
	{
		selectedOption--;

		if(selectedOption == -1)
		{
			selectedOption = 4;
		}

		stickReleased = false;
	}
	else if(yLeft < -0.5f && stickReleased == true)
	{
		selectedOption++;

		if(selectedOption == 5)
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
void TrainingState::moveCursor(void)
{
	switch(selectedOption)
	{
	case 0:
		trainingCursorOverlay->setScroll(0.415, -0.845);
		trainingPicture1->show();
		trainingPicture2->hide();
		trainingPicture3->hide();
		trainingPicture4->hide();
		break;
	case 1:
		trainingCursorOverlay->setScroll(0.415, -0.965);
		trainingPicture1->hide();
		trainingPicture2->show();
		trainingPicture3->hide();
		trainingPicture4->hide();
		break;
	case 2:
		trainingCursorOverlay->setScroll(0.415, -1.09);
		trainingPicture1->hide();
		trainingPicture2->hide();
		trainingPicture3->show();
		trainingPicture4->hide();
		break;
	case 3:
		trainingCursorOverlay->setScroll(0.415, -1.215);
		trainingPicture1->hide();
		trainingPicture2->hide();
		trainingPicture3->hide();
		trainingPicture4->show();
		break;
	case 4:
		trainingCursorOverlay->setScroll(0.415, -1.37);
		trainingPicture1->hide();
		trainingPicture2->hide();
		trainingPicture3->hide();
		trainingPicture4->hide();
		break;
	}
}
bool TrainingState::keyPressed( const OIS::KeyEvent &e ) 
{
	return true;
}

bool TrainingState::keyReleased( const OIS::KeyEvent &e ) 
{
    if( e.key == OIS::KC_INSERT ) 
	{
     	GameManager::getSingletonPtr()->requestShutdown();
    }

	if(e.key == OIS::KC_DOWN)
	{
		if(trainingSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		selectedOption++;

		if(selectedOption == 5)
		{
			selectedOption = 0;
		}
	}

	if(e.key == OIS::KC_UP)
	{
		if(trainingSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		selectedOption--;

		if(selectedOption == -1)
		{
			selectedOption = 4;
		}
	}

	if(e.key == OIS::KC_RETURN)
	{
		if(trainingSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel3);
			ERRCHECK(audio_result);
		}

		switch(selectedOption)
		{
		case 0:
			if(trainSteroids > 0)
			{
				if(upperBodyStat < maxStat)
				{
					upperBodyStat += bUpperBody;
					buffed = true;
				}
				if(grappleStat < maxStat)
				{
					grappleStat += bGrapple;
					buffed = true;
				}
				if(buffed)
				{
					currentLevel++;
					trainSteroids--;
					buffed = false;
				}
			}
			break;
		case 1:
			if(trainSteroids > 0)
			{
				if(grappleStat < maxStat)
				{
					grappleStat += cuGrapple;
					buffed = true;
				}
				if(upperBodyStat < maxStat)
				{
					upperBodyStat += cuUpperBody;
					buffed = true;
				}
				if(buffed)
				{
					currentLevel++;
					trainSteroids--;
					buffed = false;
				}
			}
			break;
		case 2:
			if(trainSteroids > 0)
			{
				if(healthStat < maxStat)
				{
					healthStat += crHealth;
					buffed = true;
				}
				if(buffed)
				{
					currentLevel++;
					trainSteroids--;
					buffed = false;
				}
			}
			break;
		case 3:
			if(trainSteroids > 0)
			{
				if(lowerBodyStat < maxStat)
				{
					lowerBodyStat += sLowerBody;
					buffed = true;
				}
				if(buffed)
				{
					currentLevel++;
					trainSteroids--;
					buffed = false;
				}
			}
			break;
		case 4:
			//GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("CharacterEditor"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
			break;
		}
		if(upperBodyStat > maxStat)
		{
			upperBodyStat = maxStat;
		}
		if(lowerBodyStat > maxStat)
		{
			lowerBodyStat = maxStat;
		}
		if(healthStat > maxStat)
		{
			healthStat = maxStat;
		}
		if(grappleStat > maxStat)
		{
			grappleStat = maxStat;
		}
	}
  
	return true;
}

void TrainingState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	trainingMusicSetting = atoi(element->Attribute("Music"));
	trainingSFXSetting = atoi(element->Attribute("SFX"));
	trainingControls = atoi(element->Attribute("Keyboard"));
	trainingInvert = atoi(element->Attribute("Invert"));
}

void TrainingState::loadStats( void )
{
	TiXmlDocument loadDoc("attributes.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Stat");

	currentLevel = atoi(element->Attribute("Level"));
	upperBodyStat = atoi(element->Attribute("UpperBody"));
	lowerBodyStat = atoi(element->Attribute("LowerBody"));
	healthStat = atoi(element->Attribute("Health"));
	grappleStat = atoi(element->Attribute("Grapple"));
}

void TrainingState::updateStats( void )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("attributes");
	doc.LinkEndChild(root);

	TiXmlElement *stat = new TiXmlElement("Stat");
	root->LinkEndChild(stat);
	stat->SetAttribute("Level", currentLevel);
	stat->SetAttribute("UpperBody", upperBodyStat);
	stat->SetAttribute("LowerBody", lowerBodyStat);
	stat->SetAttribute("Health", healthStat);
	stat->SetAttribute("Grapple", grappleStat);

	doc.SaveFile("attributes.xml");
}

void TrainingState::loadMerchandise( void )
{
	TiXmlDocument loadDoc("merchandise.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Item");

	trainPainkillers = atoi(element->Attribute("PainKillers"));
	trainMoney = atoi(element->Attribute("Money"));
	trainSteroids = atoi(element->Attribute("Syringe"));
}

void TrainingState::updateMerchandise( void )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Merchandise");
	doc.LinkEndChild(root);

	TiXmlElement *item = new TiXmlElement("Item");
	root->LinkEndChild(item);
	item->SetAttribute("PainKillers", trainPainkillers);
	item->SetAttribute("Money", trainMoney);
	item->SetAttribute("Syringe", trainSteroids);

	doc.SaveFile("merchandise.xml");
}

TrainingState* TrainingState::getSingletonPtr( void ) {
    if( !mTrainingState ) {
        mTrainingState = new TrainingState();
		UIStateFactory::getSingletonPtr()->registerState(mTrainingState->getName(), mTrainingState);
    }

    return mTrainingState;
}
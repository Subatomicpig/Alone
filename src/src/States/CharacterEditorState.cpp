#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>

#include "tinyxml.h"

#include "States\CharacterEditorState.h"
#include "Managers\GameManager.h"
#include "Managers\GlobalSettingsManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

using namespace Ogre;
using namespace Alone;

bool editorMusicSetting;
bool editorSFXSetting;
bool editorControls;
bool editorInvert;

int mCurrentLevel;
int mCurrentSteroids;
int mUpperBodyStat;
int mLowerBodyStat;
int mHealthStat;
int mGrappleStat;
int mSpeedStat;

int cigarettes;
int painkillers;
int steroid;
int money;

void CharacterEditorState::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

Alone::GamePadControllerList mCharacterEditorGamePad;

CharacterEditorState* CharacterEditorState::mCharacterEditorState;

CharacterEditorState::CharacterEditorState(void)
{
	mClassName = "CharacterEditor";
}

CharacterEditorState::~CharacterEditorState(void)
{
}

void CharacterEditorState::enter(void)
{
	hilighter = 0;
	skin = 0;
	shirt = 0;
	hair = 0;
	name = "";
	i = 0;

	mCurrentLevel = 1;
	mCurrentSteroids = 0;
	mUpperBodyStat = 5;
	mLowerBodyStat = 5;
	mHealthStat = 5;
	mGrappleStat = 5;
	mSpeedStat = 12;
	cigarettes = 0;
	painkillers = 0;
	steroid = 0;
	money = 0;

	loadOptions();

	updateStats();
	updateMerchandise();

	mRoot = Root::getSingletonPtr();

	mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "SceneManagerEditor");
	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) );
    mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);

	mCamera = mSceneMgr->createCamera("CharacterEditorCamera");
	mCamera->setNearClipDistance(5.0f);
	mCamera->setFarClipDistance(0.0f);
	mCamera->setPosition(Ogre::Vector3(-130.0f, 55.0f, -80.0f));


	//mCamera->setNearClipDistance(5.0f);
	//mCamera->setFarClipDistance(0.0f);
	//mCamera->setFOVy(Radian(Degree(25.0f)));
	//mCamera->setPosition(Ogre::Vector3(-200.0f, 40.0f, 0.0f));
	//mCamera->setOrientation(Ogre::Quaternion(Ogre::Degree(-200.0f), Ogre::Vector3::UNIT_Y));

	mWindow = GameManager::getSingletonPtr()->getRenderWindow();
	mWindow->addViewport(mCamera);

	mLight = mSceneMgr->createLight("MainLight");
	mLight->setType(Ogre::Light::LT_POINT);
	//mLight->setDirection(Vector3( 0, -1, 1 )); 
	mLight->setPosition(-50.0f, 100.0f, 20.0f);
	//mLight->setPowerScale(20.0);
	mLight->setCastShadows(false);

	mOverlayMgr = OverlayManager::getSingletonPtr();

	//Clipboard Overlay
	mCharacterEditorClipboardOverlay = mOverlayMgr->getByName("Overlay/CharacterEditorClipboard");
	//mCharacterEditorClipboardOverlay->setScroll(0, -0.50);

	//Hilighter Overlay
	mCharacterEditorHilighterOverlay = mOverlayMgr->getByName("Overlay/CharacterEditorHilighter");
	mCharacterEditorHilighterOverlay->setScale(1.2, 1.125);
	moveHilighter();
	
	//Clipboard Text Overlay
	//mCharacterEditorTextOverlay = mOverlayMgr->getByName("Overlay/CharacterEditorText");
	//mCharacterEditorTextOverlay->setScroll(0.07, -1.17);

	//Character Skin Selector
	mCharacterEditorSkinSelectorOverlay = mOverlayMgr->getByName("Overlay/CharacterEditorSkinSelector");
	
	//Character Shirt Selector
	mCharacterEditorShirtSelectorOverlay = mOverlayMgr->getByName("Overlay/CharacterEditorShirtSelector");
	
	//Character Hair Selector
	mCharacterEditorHairSelectorOverlay = mOverlayMgr->getByName("Overlay/CharacterEditorHairSelector");

	mCharacterNamePanel = static_cast<Ogre::OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "CharacterNamePanel"));
	mCharacterNamePanel->setMetricsMode(Ogre::GMM_PIXELS);
	mCharacterNamePanel->setPosition(0,0);
	mCharacterNamePanel->setDimensions(200,480);

	mCharacterNameTextBox = static_cast<Ogre::TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "Name"));
	mCharacterNameTextBox->setDimensions(50,50);
	mCharacterNameTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	mCharacterNameTextBox->setPosition(134,500);
	mCharacterNameTextBox->setFontName("MyFont");
	mCharacterNameTextBox->setCharHeight(18);
	mCharacterNameTextBox->setColour(Ogre::ColourValue::Black);
	
	mCharacterNamePanel->addChild(mCharacterNameTextBox);

	mCharacterEditorClipboardOverlay->add2D(mCharacterNamePanel);

	CellEntity = mSceneMgr->createEntity("Cell", "VGD_Alone_lvl_playercell.mesh");
	CellNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CellNode");
	CellNode->setPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));
	CellNode->yaw(Ogre::Degree(-45.0f));
	CellNode->attachObject( CellEntity );

	mShirtEntity = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirt.mesh");
	mShirtEntity->setMaterialName("VGD_ALONE_Main_tex_shirt_hair");
	mShirtlessEntity = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtless.mesh");
	mShirtlessEntity->setMaterialName("VGD_ALONE_Main_tex_tank_hair");
	mPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
	mPlayerNode->setPosition(Ogre::Vector3(-50.0f, 0.0f, 50.0f));
	mPlayerNode->yaw(Ogre::Degree(-180.0f));

	mCamera->lookAt(mPlayerNode->getWorldPosition().x+30.0f, mPlayerNode->getWorldPosition().y + 50.0f,mPlayerNode->getWorldPosition().z);

	mCharacterEditorGamePad.push_back(new Alone::GamePadController(0, 4000));

	input = InputManager::getSingletonPtr();
	input->addKeyListener(mCharacterEditorState, mClassName);
	input->addJoystickListener(mCharacterEditorState, mClassName);

	channel1 = 0;
	channel2 = 0;
	channel3 = 0;

	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	audio_result = system->createStream("Sounds/Music/MainMenu.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	if(editorMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}

	audio_result = system->createSound("Sounds/SFX/Menu/Paper/Scribbles 3.mp3", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Scribbles);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Grunt1/Short Switch.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Movement);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Menu/Metal/Short Switch.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cursor_Selection);
	ERRCHECK(audio_result);
	
}

void CharacterEditorState::update(Ogre::Real seconds)
{
	mCharacterNameTextBox->setCaption(name);

	mCharacterEditorClipboardOverlay->show();
	mCharacterEditorHilighterOverlay->show();
	//mCharacterEditorTextOverlay->show();
	mCharacterEditorSkinSelectorOverlay->show();
	mCharacterEditorShirtSelectorOverlay->show();
	mCharacterEditorHairSelectorOverlay->show();

	moveJoystick(seconds);

	system->update();

	moveHilighter();
	moveSelector();

	changeCharStyle();

	if(input->isButtonDown(0, 1) && name != "")
	{
		TiXmlDocument doc;

		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
		doc.LinkEndChild(decl);

		TiXmlElement *root = new TiXmlElement("Character");
		doc.LinkEndChild(root);

		TiXmlElement *info = new TiXmlElement("Info");
		root->LinkEndChild(info);
		info->SetAttribute("Exist", false);
		info->SetAttribute("Name", name.c_str());
		info->SetAttribute("Skin", skin);
		info->SetAttribute("Shirt", shirt);
		info->SetAttribute("Hair", hair);

		doc.SaveFile("CharacterEditor.xml");

		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("LevelTest"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
	}
	else if(input->isButtonDown(0,2))
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
	}
}

void CharacterEditorState::moveJoystick(Ogre::Real seconds)
{
	mCharacterEditorGamePad[0]->update(seconds);

	Real yLeft = mCharacterEditorGamePad[0]->getY(0);
	Real xLeft = mCharacterEditorGamePad[0]->getX(0);

	if(yLeft > 0.5f && stickReleased == true)
	{
		hilighter--;

		if(hilighter == -1)
		{
			hilighter = 3;
		}

		stickReleased = false;
	}
	else if(yLeft < -0.5f && stickReleased == true)
	{
		hilighter++;

		if(hilighter == 4)
		{
			hilighter = 0;
		}

		stickReleased = false;
	}

	if(xLeft > 0.5f && stickReleased == true)
	{
			switch(hilighter)
		{
		case 1:
			skin++;
			
			if(skin == 3)
			{
				skin = 0;
			}

			break;
		case 2:
			shirt++;

			if(shirt == 3)
			{
				shirt = 0;
			}

			break;
		case 3:
			hair++;

			if(hair == 3)
			{
				hair = 0;
			}

			break;
		}

		stickReleased = false;
	}
	else if(xLeft < -0.5f && stickReleased == true)
	{
		switch(hilighter)
		{
		case 1:
			skin--;

			if(skin == -1)
			{
				skin = 2;
			}
			break;
		case 2:
			shirt--;

			if(shirt == -1)
			{
				shirt = 2;
			}
			break;
		case 3:
			hair--;

			if(hair == -1)
			{
				hair = 2;
			}
			break;
		}

		stickReleased = false;
	}

	if(yLeft > -0.5f && yLeft < 0.5 && xLeft > -0.5f && xLeft < 0.5f && stickReleased == false)
	{
		stickReleased = true;
	}
}

void CharacterEditorState::exit(void)
{
	mCharacterEditorClipboardOverlay->hide();
	mCharacterEditorHilighterOverlay->hide();
	//mCharacterEditorTextOverlay->hide();
	mCharacterEditorSkinSelectorOverlay->hide();
	mCharacterEditorShirtSelectorOverlay->hide();
	mCharacterEditorHairSelectorOverlay->hide();

	mOverlayMgr->destroyOverlayElement("CharacterNamePanel");
	mOverlayMgr->destroyOverlayElement("Name");

	audio_result = audio_track->release();
	ERRCHECK(audio_result);

	audio_result = cursor_Movement->release();
	ERRCHECK(audio_result);

	audio_result = cursor_Selection->release();
	ERRCHECK(audio_result);

	audio_result = system->close();
	ERRCHECK(audio_result);

	audio_result = system->release();
	ERRCHECK(audio_result);

	mSceneMgr->destroyCamera(mCamera);
	mSceneMgr->destroyAllLights();
	mRoot->destroySceneManager(mSceneMgr);
	mWindow->removeViewport(0);
}

bool CharacterEditorState::keyPressed(const OIS::KeyEvent &e)
{
	return true;
}

bool CharacterEditorState::keyReleased(const OIS::KeyEvent &e)
{
	if(e.key == OIS::KC_ESCAPE)
	{
		GameManager::getSingletonPtr()->requestShutdown();
	}

	if(e.key == OIS::KC_RETURN)
	{
		if(name != "")
		{
			if(editorSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Selection, false, &channel2);
				ERRCHECK(audio_result);
			}

			TiXmlDocument doc;

			TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
			doc.LinkEndChild(decl);

			TiXmlElement *root = new TiXmlElement("Character");
			doc.LinkEndChild(root);

			TiXmlElement *info = new TiXmlElement("Info");
			root->LinkEndChild(info);
			info->SetAttribute("Exist", false);
			info->SetAttribute("Name", name.c_str());
			info->SetAttribute("Skin", skin);
			info->SetAttribute("Shirt", shirt);
			info->SetAttribute("Hair", hair);

			doc.SaveFile("CharacterEditor.xml");

			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("LevelTest"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
		}
	}

	if(e.key == OIS::KC_DOWN)
	{
		if(editorSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		hilighter++;
		
		if(hilighter == 4)
		{
			hilighter = 0;
		}
	}
	else if(e.key == OIS::KC_UP)
	{
		if(editorSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Movement, false, &channel2);
			ERRCHECK(audio_result);
		}

		hilighter--;

		if(hilighter == -1)
		{
			hilighter = 3;
		}
	}

	if(e.key == OIS::KC_RIGHT)
	{
		if(editorSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Scribbles, false, &channel3);
			ERRCHECK(audio_result);
		}

		switch(hilighter)
		{
		case 1:
			skin++;
			
			if(skin == 3)
			{
				skin = 0;
			}

			break;
		case 2:
			shirt++;

			if(shirt == 3)
			{
				shirt = 0;
			}

			break;
		case 3:
			hair++;

			if(hair == 3)
			{
				hair = 0;
			}

			break;
		}
	}
	else if(e.key == OIS::KC_LEFT)
	{
		if(editorSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Scribbles, false, &channel3);
			ERRCHECK(audio_result);
		}

		switch(hilighter)
		{
		case 1:
			skin--;

			if(skin == -1)
			{
				skin = 2;
			}
			break;
		case 2:
			shirt--;

			if(shirt == -1)
			{
				shirt = 2;
			}
			break;
		case 3:
			hair--;

			if(hair == -1)
			{
				hair = 2;
			}
			break;
		}
	}

	if(hilighter == 0 && i < 10)
	{
		if(editorSFXSetting && hilighter > 0)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Scribbles, false, &channel3);
			ERRCHECK(audio_result);
		}

		if(e.key == OIS::KC_A)
		{
			if(i == 0)
			{
				character[i] = "A";
			}
			else
			{
				character[i] = "a";
			}

		}
		else if(e.key == OIS::KC_B)
		{
			if(i == 0)
			{
				character[i] = "B";
			}
			else
			{
				character[i] = "b";
			}
		}
		else if(e.key == OIS::KC_C)
		{
			if(i == 0)
			{
				character[i] = "C";
			}
			else
			{
				character[i] = "c";
			}
		}
		else if(e.key == OIS::KC_D)
		{
			if(i == 0)
			{
				character[i] = "D";
			}
			else
			{
				character[i] = "d";
			}
		}
		else if(e.key == OIS::KC_E)
		{
			if(i == 0)
			{
				character[i] = "E";
			}
			else
			{
				character[i] = "e";
			}
		}
		else if(e.key == OIS::KC_F)
		{
			if(i == 0)
			{
				character[i] = "F";
			}
			else
			{
				character[i] = "f";
			}
		}
		else if(e.key == OIS::KC_G)
		{
			if(i == 0)
			{
				character[i] = "G";
			}
			else
			{
				character[i] = "g";
			}
		}
		else if(e.key == OIS::KC_H)
		{
			if(i == 0)
			{
				character[i] = "H";
			}
			else
			{
				character[i] = "h";
			}
		}
		else if(e.key == OIS::KC_I)
		{
			if(i == 0)
			{
				character[i] = "I";
			}
			else
			{
				character[i] = "i";
			}
		}
		else if(e.key == OIS::KC_J)
		{
			if(i == 0)
			{
				character[i] = "J";
			}
			else
			{
				character[i] = "j";
			}
		}
		else if(e.key == OIS::KC_K)
		{
			if(i == 0)
			{
				character[i] = "K";
			}
			else
			{
				character[i] = "k";
			}
		}
		else if(e.key == OIS::KC_L)
		{
			if(i == 0)
			{
				character[i] = "L";
			}
			else
			{
				character[i] = "l";
			}
		}
		else if(e.key == OIS::KC_M)
		{
			if(i == 0)
			{
				character[i] = "M";
			}
			else
			{
				character[i] = "m";
			}
		}
		else if(e.key == OIS::KC_N)
		{
			if(i == 0)
			{
				character[i] = "N";
			}
			else
			{
				character[i] = "n";
			}
		}
		else if(e.key == OIS::KC_O)
		{
			if(i == 0)
			{
				character[i] = "O";
			}
			else
			{
				character[i] = "o";
			}
		}
		else if(e.key == OIS::KC_P)
		{
			if(i == 0)
			{
				character[i] = "P";
			}
			else
			{
				character[i] = "p";
			}
		}
		else if(e.key == OIS::KC_Q)
		{
			if(i == 0)
			{
				character[i] = "Q";
			}
			else
			{
				character[i] = "q";
			}
		}
		else if(e.key == OIS::KC_R)
		{
			if(i == 0)
			{
				character[i] = "R";
			}
			else
			{
				character[i] = "r";
			}
		}
		else if(e.key == OIS::KC_S)
		{
			if(i == 0)
			{
				character[i] = "S";
			}
			else
			{
				character[i] = "s";
			}
		}
		else if(e.key == OIS::KC_T)
		{
			if(i == 0)
			{
				character[i] = "T";
			}
			else
			{
				character[i] = "t";
			}
		}
		else if(e.key == OIS::KC_U)
		{
			if(i == 0)
			{
				character[i] = "U";
			}
			else
			{
				character[i] = "u";
			}
		}
		else if(e.key == OIS::KC_V)
		{
			if(i == 0)
			{
				character[i] = "V";
			}
			else
			{
				character[i] = "v";
			}
		}
		else if(e.key == OIS::KC_W)
		{
			if(i == 0)
			{
				character[i] = "W";
			}
			else
			{
				character[i] = "w";
			}
		}
		else if(e.key == OIS::KC_X)
		{
			if(i == 0)
			{
				character[i] = "X";
			}
			else
			{
				character[i] = "x";
			}
		}
		else if(e.key == OIS::KC_Y)
		{
			if(i == 0)
			{
				character[i] = "Y";
			}
			else
			{
				character[i] = "y";
			}
		}
		else if(e.key == OIS::KC_Z)
		{
			if(i == 0)
			{
				character[i] = "Z";
			}
			else
			{
				character[i] = "z";
			}
		}

		if(character[i] != "")
		{
			name += character[i];
			i++;
		}
	}

	if(e.key == OIS::KC_BACK && hilighter == 0)
	{
		if(editorSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, cursor_Scribbles, false, &channel3);
			ERRCHECK(audio_result);
		}

		if(i > 0)
		{
			i--;
			character[i] = "";
			name = "";

			for(int x = 0; x < i; x++)
			{
				name += character[x];
			}
		}
	}

	if(e.key == OIS::KC_5 )
	{
		mPlayerNode->yaw(Ogre::Degree(-5.0f));
	}
	if(e.key == OIS::KC_6 )
	{
		mPlayerNode->yaw(Ogre::Degree(5.0f));
	}

	return true;
}

void CharacterEditorState::do_focus(void)
{
	mInputManager = InputManager::getSingletonPtr();
	mInputManager->addKeyListener(this, mClassName);
	mInputManager->addJoystickListener(this, mClassName);
}

void CharacterEditorState::do_defocus(void)
{
	mInputManager->removeKeyListener(this);
	mInputManager->removeJoystickListener(this);
}

void CharacterEditorState::moveHilighter(void)
{
	switch(hilighter)
	{
	case 0:
		mCharacterEditorHilighterOverlay->setScroll(0.27, -1.424);
		break;
	case 1:
		mCharacterEditorHilighterOverlay->setScroll(0.27, -1.55);
		break;
	case 2:
		mCharacterEditorHilighterOverlay->setScroll(0.27, -1.66);
		break;
	case 3:
		mCharacterEditorHilighterOverlay->setScroll(0.27, -1.77);
		break;
	}
}

void CharacterEditorState::moveSelector(void)
{
	switch(skin)
	{
	case 0:
		mCharacterEditorSkinSelectorOverlay->setScroll(0.25, -1.42);
		break;
	case 1:
		mCharacterEditorSkinSelectorOverlay->setScroll(0.48, -1.42);
		break;
	case 2: 
		mCharacterEditorSkinSelectorOverlay->setScroll(0.71, -1.42);
		break;
	}

	switch(shirt)
	{
	case 0:
		mCharacterEditorShirtSelectorOverlay->setScroll(0.25, -1.53);
		break;
	case 1:
		mCharacterEditorShirtSelectorOverlay->setScroll(0.48, -1.53);
		break;
	case 2:
		mCharacterEditorShirtSelectorOverlay->setScroll(0.71, -1.53);
		break;
	}

	switch(hair)
	{
	case 0:
		mCharacterEditorHairSelectorOverlay->setScroll(0.25, -1.635);
		break;
	case 1:
		mCharacterEditorHairSelectorOverlay->setScroll(0.48, -1.635);
		break;
	case 2:
		mCharacterEditorHairSelectorOverlay->setScroll(0.71, -1.635);
		break;
	}
}

void CharacterEditorState::changeCharStyle(void)
{
	switch(shirt)
	{
	case 0:
		mPlayerNode->detachAllObjects();
		mPlayerNode->attachObject(mShirtlessEntity);
		if(hair == 0 && skin == 0)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_Tank_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_Tank_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_Tank_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_Tank_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_Tank_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_Tank_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_Tank_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_Tank_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_Tank_Bald");
		}

		break;
	case 1:
		mPlayerNode->detachAllObjects();
		mPlayerNode->attachObject(mShirtEntity);
		if(hair == 0 && skin == 0)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_Black_Collar_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_White_Collar_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_Asian_Collar_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_Black_Collar_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_White_Collar_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_Asian_Collar_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_Black_Collar_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_White_Collar_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			mShirtEntity->setMaterialName("VGD_ALONE_Main_Asian_Collar_Bald");
		}
		break;
	case 2:
		mPlayerNode->detachAllObjects();
		mPlayerNode->attachObject(mShirtlessEntity);
		if(hair == 0 && skin == 0)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_None_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_None_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_None_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_None_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_None_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_None_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_None_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_None_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			mShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_None_Bald");
		}
		break;
	}
}

void CharacterEditorState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	editorMusicSetting = atoi(element->Attribute("Music"));
	editorSFXSetting = atoi(element->Attribute("SFX"));
	editorControls = atoi(element->Attribute("Keyboard"));
	editorInvert = atoi(element->Attribute("Invert"));
}

void CharacterEditorState::updateStats( void )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Attributes");
	doc.LinkEndChild(root);

	TiXmlElement *stat = new TiXmlElement("Stat");
	root->LinkEndChild(stat);
	stat->SetAttribute("Level", mCurrentLevel);
	stat->SetAttribute("UpperBody", mUpperBodyStat);
	stat->SetAttribute("LowerBody", mLowerBodyStat);
	stat->SetAttribute("Health", mHealthStat);
	stat->SetAttribute("Grapple", mGrappleStat);
	stat->SetAttribute("Speed", mSpeedStat);

	doc.SaveFile("attributes.xml");
}

void CharacterEditorState::updateMerchandise( void )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Merchandise");
	doc.LinkEndChild(root);

	TiXmlElement *item = new TiXmlElement("Item");
	root->LinkEndChild(item);
	item->SetAttribute("Cigarettes", cigarettes);
	item->SetAttribute("PainKillers", painkillers);
	item->SetAttribute("Money", money);
	item->SetAttribute("Syringe", steroid);

	doc.SaveFile("merchandise.xml");
}

CharacterEditorState* CharacterEditorState::getSingletonPtr(void)
{
	if(!mCharacterEditorState)
	{
		mCharacterEditorState = new CharacterEditorState();
		WorkStateFactory::getSingletonPtr()->registerState(mCharacterEditorState->getName(), mCharacterEditorState);
	}

	return mCharacterEditorState;
}
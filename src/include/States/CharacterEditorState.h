#ifndef CHARACTER_EDITOR_STATE_H
#define CHARACTER_EDITOR_STATE_H

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

class CharacterEditorState:public Alone::WorkState
{
public:
	~CharacterEditorState(void);

	void enter(void);
	void update(Ogre::Real seconds);
	void exit(void);

	void ERRCHECK(FMOD_RESULT result);

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	void moveHilighter(void);
	void moveSelector(void);

	void changeCharStyle(void);
	
	void do_focus(void);
	void do_defocus(void);

	void loadOptions( void );
	void updateStats( void );
	void updateMerchandise( void );

	void moveJoystick(Ogre::Real seconds);

	static CharacterEditorState* getSingletonPtr(void);

private:
	CharacterEditorState(void);

	CharacterEditorState(const CharacterEditorState&) {}
	CharacterEditorState & operator = (const CharacterEditorState&);

	Ogre::Root			*mRoot;
	Ogre::Camera		*mCamera;
	Ogre::SceneManager	*mSceneMgr;
	Ogre::RenderWindow	*mWindow;
	Ogre::Light			*mLight;
	Ogre::Light			*mLight2;
	Ogre::Light			*mLight3;

	Ogre::Entity		*mCharacterEntity;
	Ogre::SceneNode		*mCharacterSceneNode;
	Ogre::Entity		*CellEntity;
	Ogre::SceneNode		*CellNode;

	Ogre::OverlayManager	*mOverlayMgr;

	Ogre::Overlay			*mCharacterEditorTextOverlay;
	Ogre::Overlay			*mCharacterEditorClipboardOverlay;
	Ogre::Overlay			*mCharacterEditorHilighterOverlay;
	Ogre::Overlay			*mCharacterEditorSkinSelectorOverlay;
	Ogre::Overlay			*mCharacterEditorShirtSelectorOverlay;
	Ogre::Overlay			*mCharacterEditorHairSelectorOverlay;

	Ogre::OverlayContainer			*mCharacterNamePanel;
	Ogre::TextAreaOverlayElement	*mCharacterNameTextBox;

	Ogre::Entity					*mShirtEntity;
	Ogre::Entity					*mShirtlessEntity;

	Ogre::SceneNode					*mPlayerNode;

	int hilighter;
	int skin;
	int hair;
	int shirt;

	std::string name;
	std::string character[10];
	int i;

	InputManager *input;
	bool stickReleased;

	FMOD::System *system;
	FMOD::Channel *channel1;
	FMOD::Channel *channel2;
	FMOD::Channel *channel3;

	FMOD::Sound *audio_track;
	FMOD::Sound *cursor_Scribbles;
	FMOD::Sound *cursor_Movement;
	FMOD::Sound *cursor_Selection;
	FMOD_RESULT audio_result;

	static CharacterEditorState	*mCharacterEditorState;
};
#endif
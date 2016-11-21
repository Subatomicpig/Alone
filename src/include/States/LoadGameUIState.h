#ifndef LOAD_GAME_UI_STATE_H
#define LOAD_GAME_UI_STATE_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>

#include "States\UIState.h"
#include "Managers\InputManager.h"
#include "Controllers\GamePadController.h"
#include "CommonOgreTypes.h"

class LoadGameUIState:public Alone::UIState
{
public:
	~LoadGameUIState(void);

	void enter(void);
	void update(Ogre::Real seconds);
	void exit(void);

	void moveJoystick(Ogre::Real seconds);

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	void LoadGameOne();
	void LoadGameTwo();
	void LoadGameThree();

	void LoadCharacterInfo();
	void LoadInventory();
	void updateStats(void);
	void loadStats(void);
	void updateMerchandise(void);

	static LoadGameUIState* getSingletonPtr(void);

private:
	LoadGameUIState(void);

	LoadGameUIState(const LoadGameUIState&) {}
	LoadGameUIState & operator = (const LoadGameUIState&);

	Ogre::OverlayManager	*mOverlayMgr;

	Ogre::Overlay			*mLoadGameOneOverlay;
	Ogre::Overlay			*mLoadGameTwoOverlay;
	Ogre::Overlay			*mLoadGameThreeOverlay;
	Ogre::Overlay			*mLoadGameControlsOverlay;

	Ogre::OverlayContainer			*mLoadGamePanel;

	Ogre::TextAreaOverlayElement	*mSaveGameNameTextBox;

	static LoadGameUIState	*mLoadGameUIState;

	bool stickReleased, selectionReleased;
	InputManager* input;

	int selectedOption;
};
#endif
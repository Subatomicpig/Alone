#ifndef ACHIEVEMENTS_UI_STATE_H
#define ACHIEVEMENTS_UI_STATE_H

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

class AchievementsUIState:public Alone::UIState
{
public:
	~AchievementsUIState(void);

	void enter(void);
	void update(Ogre::Real seconds);
	void exit(void);

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	void loadOptions( void );

	static AchievementsUIState* getSingletonPtr(void);

private:
	AchievementsUIState(void);

	AchievementsUIState(const AchievementsUIState&) {}
	AchievementsUIState & operator = (const AchievementsUIState&);

	Ogre::OverlayManager			*mOverlayMgr;
	Ogre::OverlayContainer			*mAchievementsPanel;

	Ogre::Overlay					*mAchievementsOverlay;
	Ogre::Overlay					*mDebugOverlay;

	Ogre::OverlayElement			*mBottomLeft;
	Ogre::OverlayElement			*mBottomRight;

	static AchievementsUIState		*mAchievementsUIState;
};
#endif
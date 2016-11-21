#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <OgreRoot.h>
#include "Managers\InputManager.h"
#include "Managers\GlobalSettingsManager.h"

#ifdef USE_SOUNDS
#include "Managers\SoundManager.h"
#endif

namespace Alone
{
	class WorkState;
	class UIState;

	class GameManager 
	{
	public:
		~GameManager( void );

		void startup(WorkState *gameState, UIState* uiState);
		void changeState( WorkState *gameState );
		void pushState( WorkState *gameState );
		void popState( void );

		void changeUIState( UIState *state );
		void pushUIState( UIState *state );
		void popUIState( void );

		void resetClock();

		WorkState *getWorkState();
		UIState *getUIState();

		void requestShutdown( void );
		Ogre::RenderWindow* getRenderWindow() { return mRenderWindow; }

		Ogre::RenderWindow* createRenderWindow(const Ogre::String &name, unsigned int width, unsigned int height, 
				bool fullScreen, const Ogre::NameValuePairList *miscParams);

		static GameManager* getSingletonPtr( void );
	private:
		GameManager( void );
		GameManager( const GameManager& ) { }
		GameManager & operator = ( const GameManager& );

		bool configureGame( void );
		void setupResources( void );

		Ogre::Root*					mRoot;
		Ogre::RenderWindow*			mRenderWindow;

		InputManager*				mInputMgr;
		GlobalSettingsManager*		mSettingsMgr;
#ifdef USE_SOUNDS
		SoundManager*				mSoundMgr;
#endif
		unsigned long mMicrosecondWait;
		unsigned long mTimeLastFrame;
		unsigned long mTimeCurrentFrame;
		unsigned long mTimeSinceLastFrame;

		bool bShutdown;

		std::vector<WorkState*> mStates;
		std::vector<UIState*> mUIStates;

		static GameManager *mGameManager;
	};
}
#endif
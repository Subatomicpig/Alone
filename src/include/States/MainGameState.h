#ifndef MAIN_GAME_STATE_H
#define MAIN_GAME_STATE_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include "States\WorkState.h"
#include "Managers\InputManager.h"
#include "CommonOgreTypes.h"

class MainGameState : public Alone::WorkState 
{
public:
    ~MainGameState( void );

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );

	void do_focus( void );
	void do_defocus( void );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    bool povMoved( const OIS::JoyStickEvent &e, int pov );
    bool axisMoved( const OIS::JoyStickEvent &e, int axis );

    static MainGameState* getSingletonPtr( void );
private:
	MainGameState( void );
    MainGameState( const MainGameState& ) { }
    MainGameState & operator = ( const MainGameState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
	Ogre::RenderWindow 	 *mWindow;

	EntityList mEntities;
	SceneNodeList mSceneNodes;

	Ogre::Real mTime;
    static MainGameState    *mMainGameState;
};
#endif
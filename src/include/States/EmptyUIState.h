#ifndef EMPTY_UI_STATE_H
#define EMPTY_UI_STATE_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include "UIState.h"

class EmptyUIState : public Alone::UIState 
{
public:
    ~EmptyUIState( void );

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

    static EmptyUIState* getSingletonPtr( void );
private:
	EmptyUIState( void );
    EmptyUIState( const EmptyUIState& ) { }
    EmptyUIState & operator = ( const EmptyUIState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneManager;
	Ogre::RenderWindow 	 *mWindow;
	Ogre::Viewport		 *mViewport;

    static EmptyUIState    *mEmptyUIState;
};
#endif
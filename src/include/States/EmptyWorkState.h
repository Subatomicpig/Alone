#ifndef EMPTY_WORK_STATE_H
#define EMPTY_WORK_STATE_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include "States\WorkState.h"

class EmptyWorkState : public Alone::WorkState 
{
public:
    ~EmptyWorkState( void );

    void enter( void );
    void exit( void );
    void update( Ogre::Real seconds );

	void do_focus( void );
	void do_defocus( void );

	bool keyPressed( const OIS::KeyEvent &e );
	bool keyReleased( const OIS::KeyEvent &e );

    static EmptyWorkState* getSingletonPtr( void );
private:
	EmptyWorkState( void );
    EmptyWorkState( const EmptyWorkState& ) { }
    EmptyWorkState & operator = ( const EmptyWorkState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneManager;
	Ogre::RenderWindow 	 *mWindow;

    static EmptyWorkState    *mEmptyWorkState;
};
#endif
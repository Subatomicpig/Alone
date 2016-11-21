#ifndef WORKSTATE_H
#define WORKSTATE_H

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoystick.h>
#include <Ogre.h>
#include "Managers\InputManager.h"

namespace Alone
{
	class WorkState : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener 
	{
	public:
		virtual ~WorkState( void ) { }

		virtual void enter( void )  = 0;
		virtual void exit( void )   = 0;
		virtual void do_focus( void ) = 0;
		virtual void do_defocus( void ) = 0;
		virtual void focus( void )
		{
			do_focus();
			mFocused = true;
		}
		virtual void defocus( void)
		{
			do_defocus();
			mFocused = false;
		}
		Ogre::String getName( void )
		{
			return mClassName;
		}
		virtual void pause( void )  {}
		virtual void resume( void ) {}
		virtual void update( Ogre::Real seconds ) = 0;
	    
	public:
		virtual bool keyPressed( const OIS::KeyEvent &e ) { return true; }
		virtual bool keyReleased( const OIS::KeyEvent &e )  { return true; }

		virtual bool mouseMoved( const OIS::MouseEvent &e )  { return true; }
		virtual bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )  { return true; }
		virtual bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) { return true; }

		virtual bool buttonPressed( const OIS::JoyStickEvent &arg, int button ) { return true; }
		virtual bool buttonReleased( const OIS::JoyStickEvent &arg, int button ) { return true; }
		virtual bool axisMoved( const OIS::JoyStickEvent &arg, int axis ) { return true; }
		virtual bool povMoved( const OIS::JoyStickEvent &e, int pov ) { return true; }

	protected:
		WorkState( void ) { mFocused = true; mClassName = "INVALID"; mInputManager = NULL;}
		void initOgre();
		void initOverlays();
		void destroyOgre();
		void destroyOverlays();
		bool mFocused;
		Ogre::String		mClassName;
		InputManager*		mInputManager;
	private:
		WorkState( const WorkState& ) { }
		WorkState & operator = ( const WorkState& );
	};
}

#endif
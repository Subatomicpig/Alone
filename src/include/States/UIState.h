#ifndef UISTATE_H
#define UISTATE_H

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoystick.h>
#include <Ogre.h>

namespace Alone
{
	class UIState : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener 
	{
	public:
		virtual ~UIState( void ) { }

		virtual void enter( void )  = 0;
		virtual void exit( void )   = 0;
		virtual void pause( void )  {}
		virtual void resume( void ) {}

		Ogre::String getName( void )
		{
			return mClassName;
		}

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
		UIState( void ) {}
		Ogre::String mClassName;

	private:
		UIState( const UIState& ) { }
		UIState & operator = ( const UIState& );
	};
}

#endif
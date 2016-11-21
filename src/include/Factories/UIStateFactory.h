#ifndef UISTATEFACTORY_H
#define UISTATEFACTORY_H

#include "States\UIState.h"

namespace Alone
{
	typedef std::map<std::string, UIState*> TUIStateMap;

	class UIStateFactory {
	public:
		~UIStateFactory( void );

		void registerState( Ogre::String name, UIState *gameState );
		UIState* getState( Ogre::String name );

		static UIStateFactory* getSingletonPtr( void );
	private:
		UIStateFactory( void ) { }
		UIStateFactory( const UIStateFactory& ) { }
		UIStateFactory & operator = ( const UIStateFactory& );
		TUIStateMap mRegisteredStates;
		static UIStateFactory *mUIStateFactory;
	};
}
#endif
#ifndef WORKSTATEFACTORY_H
#define WORKSTATEFACTORY_H

#include "States\WorkState.h"

namespace Alone
{
	typedef std::map<std::string, WorkState*> TStateMap;

	class WorkStateFactory {
	public:
		~WorkStateFactory( void );

		void registerState( Ogre::String name, WorkState *gameState );
		WorkState* getState( Ogre::String name );

		static WorkStateFactory* getSingletonPtr( void );
	private:
		WorkStateFactory( void ) { }
		WorkStateFactory( const WorkStateFactory& ) { }
		WorkStateFactory & operator = ( const WorkStateFactory& );
		TStateMap mRegisteredStates;
		static WorkStateFactory *mWorkStateFactory;
	};
}
#endif
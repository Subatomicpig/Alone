#ifndef GLOBALSETTINGSMANAGER_H
#define GLOBALSETTINGSMANAGER_H

#include <Ogre.h>
#include <OgreCommon.h>

namespace Alone
{
	class GlobalSettingsManager {
	public:
		~GlobalSettingsManager( void );

		bool parseConfigFiles( void );

		Ogre::NameValuePairList& getGameOpts() { return mGameOpts; }
		Ogre::NameValuePairList& getGraphicsOpts() { return mGameOpts; }
		Ogre::NameValuePairList& getRenderOpts() { return mRenderOpts; }
		Ogre::NameValuePairList& getNetworkOpts() { return mGameOpts; }
		Ogre::NameValuePairList& getLoggingOpts() { return mGameOpts; }
		Ogre::NameValuePairList& getAudioOpts() { return mGameOpts; }
		Ogre::Real getFPS();
		
		Ogre::Real getReal(Ogre::String name);
		int getInteger(Ogre::String name);
		bool getBool(Ogre::String name);
		Ogre::String getString(Ogre::String name);

		static GlobalSettingsManager* getSingletonPtr( void );
	private:
		GlobalSettingsManager( void );
		GlobalSettingsManager( const GlobalSettingsManager& ) { }
		GlobalSettingsManager & operator = ( const GlobalSettingsManager& );

		Ogre::NameValuePairList mRenderOpts;
		Ogre::NameValuePairList mGameOpts;
		Ogre::NameValuePairList mVisemeOpts;

		Ogre::Real mFPS;
		static GlobalSettingsManager *mGlobalSettingsManager;
	};
}

#endif
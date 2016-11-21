#include "Managers\GlobalSettingsManager.h"

using namespace Ogre;

#define GAME_CONFIG "Alone.cfg"

Ogre::NameValuePairList readConfigFile(Ogre::String filename)
{
	LogManager::getSingleton().logMessage("Parsing [" + filename + "] config file");
	Ogre::NameValuePairList opts;
	ConfigFile cf;
	cf.load( filename );
	ConfigFile::SectionIterator itSection = cf.getSectionIterator();
	String sSection, sType, sArch;
	while( itSection.hasMoreElements() ) 
	{
		sSection = itSection.peekNextKey();

		ConfigFile::SettingsMultiMap *mapSettings = itSection.getNext();
		ConfigFile::SettingsMultiMap::iterator itSetting = mapSettings->begin();
		while( itSetting != mapSettings->end() ) 
		{
			sType = itSetting->first;
			sArch = itSetting->second;                           
			opts.insert(Ogre::NameValuePairList::value_type(sType, sArch));
			++itSetting;
		}
	}
	return opts;
}

void validateOpts(Ogre::NameValuePairList& opts, const char* name, const char* default_value)
{
	if(opts.count(name) == 0)
	{
		opts[name] = default_value;
	}
	LogManager::getSingleton().logMessage("[" + Ogre::String(name) + "] = [" + opts[name] + "]");
}

namespace Alone
{
	GlobalSettingsManager* GlobalSettingsManager::mGlobalSettingsManager;

	GlobalSettingsManager::GlobalSettingsManager( void )
	{
		mFPS = 60.0f;
	}

	GlobalSettingsManager::~GlobalSettingsManager( void ) 
	{
	}

	Ogre::Real GlobalSettingsManager::getReal(Ogre::String name)	
	{ 
		return atof(mGameOpts[name].c_str()); 
	}

	int GlobalSettingsManager::getInteger(Ogre::String name)	
	{ 
		return atoi(mGameOpts[name].c_str()); 
	}

	bool GlobalSettingsManager::getBool(Ogre::String name)
	{
		Ogre::String value = mGameOpts[name];
		if(value == "Yes" || value == "yes" || value == "Y" || value == "y" || value == "1")
		{
			return true;
		}
		return false;
	}

	Ogre::String GlobalSettingsManager::getString(Ogre::String name)
	{ 
		return mGameOpts[name]; 
	}

	Ogre::Real GlobalSettingsManager::getFPS()
	{
		return mFPS;
	}

	bool GlobalSettingsManager::parseConfigFiles( void ) 
	{
		LogManager::getSingleton().logMessage("Parsing config files");

		mGameOpts    = readConfigFile(GAME_CONFIG);

		// Graphics
		validateOpts(mGameOpts, "Renderer", "Direct3D");
		validateOpts(mGameOpts, "FPS", "60");
		validateOpts(mGameOpts, "width", "1024");
		validateOpts(mGameOpts, "height", "768");
		validateOpts(mGameOpts, "Full Screen", "No");
		validateOpts(mGameOpts, "Title", "??? Missing title ???");
		validateOpts(mGameOpts, "vsync", "true");
		validateOpts(mGameOpts, "FSAA", "0");
		validateOpts(mGameOpts, "useNVPerfHUD","false");
		validateOpts(mGameOpts, "show_FPS","0");

		mFPS = getReal("FPS");

		return true;
	}

	GlobalSettingsManager* GlobalSettingsManager::getSingletonPtr( void ) 
	{
		if( !mGlobalSettingsManager ) {
			mGlobalSettingsManager = new GlobalSettingsManager();
		}

		return mGlobalSettingsManager;
	}
}
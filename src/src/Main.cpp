
// Any added C files for states must be added to the include section below;
// for UI and Work states.
// Also, those states must be added where it says initialize game states below.

// NOTE: Any states added must be shut down properly in their respective
// "Exit" functions

#include <OgreException.h>

#include "Managers\GameManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

#include "States\EmptyUIState.h"
#include "States\MainMenuUIState.h"
#include "States\CreditsUIState.h"
#include "States\OptionsUIState.h"
#include "States\AchievementsUIState.h"
#include "States\LoadGameUIState.h"
#include "States\PauseUIState.h"
#include "States\TrainingState.h"
#include "States\StoreState.h"

#include "States\EmptyWorkState.h"
#include "States\MainGameState.h"
//#include "States\SampleState1.h"
//#include "States\SampleState2.h"
//#include "States\SampleState3.h"
//#include "States\SampleState4.h"
#include "States\SampleState5.h"
#include "States\LevelTestState.h"
#include "States\CharacterEditorState.h"


#include <objbase.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) {
#else
int main( int argc, char **argv ) {
#endif

	// Init COM - for plugins that use COM interfaces
	::CoInitialize(NULL); 

	using namespace Alone;

	// Design from
	// http://www.ogre3d.org/wiki/index.php/Managing_Game_States_with_OGRE
	WorkStateFactory* factory	= WorkStateFactory::getSingletonPtr();
	UIStateFactory* factory_ui	= UIStateFactory::getSingletonPtr();
    GameManager *gameManager	= GameManager::getSingletonPtr();

	// Initialize game states
	EmptyUIState::getSingletonPtr();
	MainMenuUIState::getSingletonPtr();
	CreditsUIState::getSingletonPtr();
	OptionsUIState::getSingletonPtr();
	LoadGameUIState::getSingletonPtr();
	AchievementsUIState::getSingletonPtr();
	PauseUIState::getSingletonPtr();
	TrainingState::getSingletonPtr();
	StoreState::getSingletonPtr();

	EmptyWorkState::getSingletonPtr();
	MainGameState::getSingletonPtr();
	//SampleState1::getSingletonPtr();
	//SampleState2::getSingletonPtr();
	//SampleState3::getSingletonPtr();
	//SampleState4::getSingletonPtr();
	SampleState5::getSingletonPtr();
	LevelTestState::getSingletonPtr();
	CharacterEditorState::getSingletonPtr();

    try 
	{
        // Initialise the game and switch to the first state (but in Lua)
		gameManager->startup(factory->getState("EmptyWork"), factory_ui->getState("MainMenuUI"));
    }
    catch ( Ogre::Exception& ex ) 
	{
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                MessageBox( NULL, ex.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL );
        #else
                std::cerr << "An exception has occured: " << ex.getFullDescription();
        #endif
    }

    delete gameManager;
	delete factory_ui;
	delete factory;

	// Uninit COM
	::CoUninitialize();
    return 0;
}
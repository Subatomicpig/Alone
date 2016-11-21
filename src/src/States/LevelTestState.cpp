#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "States\LevelTestState.h"
#include "Managers\GameManager.h"
#include "Managers\GlobalSettingsManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

#include "tinyxml.h"

using namespace Ogre;
using namespace Alone;

char bufferLTS[10];
int LevelLoaded;

bool playerLeftPunchLTS = false;
bool playerRightPunchLTS = false;
bool playerLeftKickLTS = false;
bool playerRightKickLTS = false;

bool shiftPressedLTS = false;
bool setAnimationLTS = false;


///////////////////
// FADE STUFF    //
///////////////////
TextureUnitState *fadeKnockout;
TextureUnitState *fadeStory01;
TextureUnitState *fadeStory02;
float knockOutFadeVar;
float story01FadeVar;
float story02FadeVar;

bool story01Continue = NULL;
bool story02Continue = NULL;

bool jollyTalkAtNight = NULL;
bool geminiTalkAtNight = NULL;
bool train = NULL;
bool jollyEarringTaken = NULL;
bool geminiPendantTaken = NULL;
bool fight = NULL;

///new item shit//
bool itemSoap = NULL;
bool itemDannySteroids = NULL;
bool itemSantosoSteroids = NULL;
bool itemLinMaMoney = NULL;
bool itemCellBlockKey = NULL;

bool geminiKnockedOut = NULL;
bool jollyKnockedOut = NULL;

///////////////////
//  XML STUFF    //
///////////////////
TiXmlDocument doc;
TiXmlDeclaration *decl = NULL;
TiXmlElement *root = NULL;
TiXmlElement *inventoryFile = NULL;
TiXmlElement *inventoryQuest = NULL;
TiXmlElement *inventoryCurrentQuest = NULL;
TiXmlElement *inventoryName = NULL;
TiXmlElement *inventoryOther = NULL;

//quest tracker xml//
TiXmlDocument doc3;
TiXmlDeclaration *decl3 = NULL;
TiXmlElement *root3 = NULL;
TiXmlElement *questTracker;

//quests xml//
TiXmlDocument doc2;
TiXmlDeclaration *decl2 = NULL;
TiXmlElement *root2 = NULL;
TiXmlElement *questLog = NULL;

std::string *Test = NULL;
char *currentHeartBeat = NULL;

////////////////////////
// GUARD PATROL FLAGS //
////////////////////////
bool atPos01 = false;
bool atPos02 = false;
bool atPos03 = false;
bool atPos04 = false;

int currentGuardNode = 1;

//////////////////////////////
//       NPC NODES          //
//////////////////////////////
SceneNode* cellsWalkNode1 = NULL;
SceneNode* cellsWalkNode2 = NULL;
SceneNode* cellsWalkNode3 = NULL;
SceneNode* cellsWalkNode4 = NULL;

bool levelsMusicSetting;
bool levelsSFXSetting;
bool levelsControls;
bool levelsInvert;
bool crouch;
bool changeInHealth;
bool batonSndPlayed = false;
bool camera1MoveCCW = false;
bool camera1MoveCW = true;
bool camera2MoveCCW = false;
bool camera2MoveCW = true;
bool camera3MoveCCW = false;
bool camera3MoveCW = true;
bool camera4MoveCCW = false;
bool camera4MoveCW = true;

///////////////////////////
// UPDATE QUEST STUFF    //
///////////////////////////
bool updateLinMasQuests = NULL;
bool updateJollysQuests = NULL;
bool updateSantosoQuests = NULL;
bool updateGeminiQuests = NULL;


//////////////////////////
//         STORE        //
//////////////////////////
bool store = NULL;
bool storeCheck = NULL;


///////////////////////////
//      FACTIONED        //
///////////////////////////
bool factioned = NULL;
bool yes = NULL;
bool no = NULL;
//no faction flags//
bool noFactionJolly = NULL;
bool noFactionGemini = NULL;


//micheal flags//
bool michealDialogue = NULL;
bool dannyBoyDialogue = NULL;


//lin mas final quest//
bool linMasFinalQuest = NULL;
bool linMasFinalQuestCompleted = NULL;
bool linMasFinalQuestFailure = NULL;
bool linMasTraining = NULL;
bool linMasTrainingNotCompleted = NULL;
bool linMasTrainingCompleted = NULL;

//johnny666 quests//
//johnny666 no rep//
bool johnny666NoRep = NULL;
bool updateJohnny666Quests = NULL;
bool johnny666RepQuestCompleted = NULL;
bool johnny666Rep = NULL;
bool johnny666CheatALittle = NULL;
bool johnny666CheatALittleDialogueCompleted = NULL;
bool johnny666CheatALittleDialogueFailed = NULL;
bool johnny66680Rep = NULL;
bool johnny666TrainingDone = NULL;
bool santosoShop = NULL;
bool santosoTalkToTheSwordQuestCompleted = NULL;
bool santosoKeyToThePlanQuestCompleted = NULL;

//aligned with gemini//
//geminis quest completed//
bool geminiQuestsCompleted = NULL;
bool geminiThePrideOfJolly = NULL;
bool geminiThePrideOfJollyDialogueCompleted = NULL;
bool geminiThePrideOfJollyDialogueFailed = NULL;
bool geminiKnockEmOut = NULL;
bool geminiKnockEmOutDialogueCompleted = NULL;
bool geminiKnockEmOutDialogueFailed = NULL;
bool geminiKeyToThePlan = NULL;
bool geminiTalkToTheSwordTwo = NULL;
bool geminiTalkToTheSwordTwoDialogueCompleted = NULL;
bool geminiTalkToTheSwordTwoDialogueFailed = NULL;
bool geminiTalkToTheSword = NULL;
bool geminiTalkToTheSwordDialogueCompleted = NULL;
bool factionedGeminiJolly = NULL;

//santoso dialouge//
bool santosoDialogue = NULL;

//jollys quests//
//aligned with jolly talking with gemini//
bool facionedJollyGemini = NULL;
bool factionedPickedJolly = NULL;
bool factionedPickedGemini = NULL;
bool factionedPickedJollyQuestCompleted = NULL;
bool factionedJolly = NULL;
bool factionedGemini = NULL;

bool jollyTalkedToSantoso = NULL;
bool jollyQuestsCompleted = NULL;
bool jollyTwoPendants = NULL;
bool jollyTwoPendantsDialogueCompleted = NULL;
bool jollyTwoPendantsDialogueFailed = NULL;
bool jollyKnockEmOut = NULL;
bool jollyKnockEmOutDialogueCompleted = NULL;
bool jollyKnockEmOutDialogueFailed = NULL;
bool jollyKeyToThePlanCompleted = NULL;
bool jollyKeyToThePlanCompletedDialogueCompleted = NULL;
bool jollyKeyToThePlanCompletedDialogueFailed = NULL;
bool santosoKeyToThePlanCompleted = NULL;
bool santosoKeyToThePlanCompletedDialogue = NULL;
bool jollyKeyToThePlan = NULL;
bool jollyKeyToThePlanDialogueCompleted = NULL;
bool jollyKeyToThePlanDialogueFailed = NULL;

bool santosoKeyToThePlan = NULL;
bool santosoKeyToThePlanDialogueCompleted = NULL;
bool santosoKeyToThePlanDialogueFailed = NULL;
bool santosoTalkToTheSword = NULL;
bool santosoTalkToTheSwordDialogueCompleted = NULL;
bool santosoTalkToTheSwordDialogueFailed = NULL;
bool jollyTalkToTheSword = NULL;
bool jollyTalkToTheSwordDialogueCompleted = NULL;
bool jollyTalkToTheSwordDialogueFailed = NULL;


//lin ma train//
bool linMaTrain = NULL;
bool linMaTrainDialogueCompleted = NULL;
bool linMaTrainDialogueFailed = NULL;
bool linMasWhoIsEscaping = NULL;
bool linMasWhoIsEscapingDialogueCompleted = NULL;
bool linMasWhoIsEscapingDialogueFailed = NULL;
bool linMasTheRatScuttle = NULL;
bool linMasTheRatScuttleDialogueCompleted = NULL;
bool linMasTheRatScuttleDialogueFailed = NULL;
bool linMasTheRatScuttleCompleted = NULL;
bool linMasMoreBugsInTheSystem = NULL;
bool limMasMoreBugsInTheSystemDialogueCompleted = NULL;
bool limMasMoreBugsInTheSystemDialogueFailed = NULL;
bool linMasBugInTheSystem = NULL;
bool linMasBugInTheSystemDialogueCompleted = NULL;
bool linMasBugInTheSystemDialogueFailed = NULL;
bool linMasPickAFactionDialogueCompleted = NULL;
bool linMasPickAFaction = NULL;
bool linMasPickAFactionCompleted = NULL;

bool santosoDialogueCompleted = NULL;

//quest bool completion variables//
bool linMasPickAFactionQuestCompleted = NULL;
bool linMasBugInTheSystemQuestCompleted = NULL;
bool linMasMoreBugsInTheSystemQuestCompleted = NULL;
bool linMasTheRatScuttleQuestCompleted = NULL;
bool linMaswhoIsEscapingQuestCompleted = NULL;
bool linMastrainQuestCompleted = NULL;

bool jollytalkToTheSwordQuestCompleted = NULL;
bool jollyKeyToThePlanQuestCompleted = NULL;
bool jollyKeyToThePlan2QuestCompleted = NULL;
bool jollyTwoPendantsQuestCompleted = NULL;
bool jollyKnockEmOutQuestCompleted = NULL;

bool geminitalkToTheSwordQuestCompleted = NULL;
bool geminitalkToTheSword2QuestCompleted = NULL;
bool gemniThePrideOfJollyQuestCompleted = NULL;
bool geminiKeyToThePlanQuestCompleted = NULL;
bool geminiKnockEmOutQuestCompleted = NULL;
bool geminiQuestsAllCompleted = NULL;

bool johnny666TrainingDoneQuestCompleted = NULL;
bool johnny66680RepQuestCompleted = NULL;
bool johnny666CheatALittleQuestCompleted = NULL;

bool linMasFinalQuestQuestCompleted = NULL;
bool linMasTrainingQuestCompleted = NULL;



////////////////////////////////
//           DIALOGUE         //
////////////////////////////////
int dialogueCounter = NULL;
bool inConversation = NULL;



////////////////////////////////
//            ITEMS           //
////////////////////////////////
//empty cell right of player//
Entity* emptyCellRightOfPlayerCigarettes = NULL;
SceneNode* emptyCellRightOfPlayerCigarettesNode = NULL;
bool emptyCellRightOfPlayerTaken;

//danny boy cell//
Entity* dannyBoySteroids = NULL;
SceneNode* dannyBoySteroidsNode = NULL;
bool dannyBoySteroidsTaken;

//painkiller in santoso shop//
Entity* santosoPainKiller = NULL;
SceneNode* santosoPainKillerNode = NULL;
bool santosoPainKillerTaken;

//money on the ground lin mas//
Entity* linMasMoney = NULL;
SceneNode* linMasMoneyNode = NULL;
bool linMasMoneyTaken;

//lin mas soap//
Entity* linMasSoap = NULL;
SceneNode* linMasSoapNode = NULL;
bool linMasSoapTaken;

//Cell block Key//
Entity* cellBlockKey = NULL;
SceneNode* cellBlockKeyNode = NULL;

//escape plans//
Entity* escapePlans = NULL;
SceneNode* escapePlansNode = NULL;
bool itemEscapePlans;


///////////////////////////
//   INVENTORY SHIT      //
///////////////////////////
int Cigarettes;
int Syringe;
int Soap;
int SoapMold;
int LinMasKey;
int CellKey;
int CellKeyCopy;
int GoldCockroach;
int Pendant;
int Earing;
int Cockroach;
int Rat;
int PainKillers;
int Money;
bool WhoIsEscaping;
int StoreSyringe;

///////////////////////////
//  STATS TRACKER        //
///////////////////////////
int lCurrentLevel;
int lUpperBodyStat;
int lLowerBodyStat;
int lHealthStat;
int lGrappleStat;
int lPlayerHealth = 5;

LevelTestState* LevelTestState::mLevelTestState;

///////////////////////////
//          LISTS        //
///////////////////////////
EntityList mEntitiesLTS;
EntityList mItemsLTS;
EntityList mPlayerEntitiesLTS;
SceneNodeList mSceneNodesLTS;
SceneNodeList mPlayerSceneNodesLTS;
SceneNodeList mLoadCharacterSceneNodesLTS;
SceneNodeList mItemsNodeLTS;
Alone::GamePadControllerList mGamePadsLTS;

//nodes for detecting which characters to load
SceneNode* detectNode1 = NULL;
SceneNode* detectNode2 = NULL;
SceneNode* detectNode3 = NULL;
SceneNode* detectNode4 = NULL;
SceneNode* detectNode5 = NULL;
SceneNode* detectNode6 = NULL;
SceneNode* mainLightNode = NULL;

///////////////////////////
//          NPCS         //
///////////////////////////
Entity* linMa = NULL;
SceneNode* linMaNode = NULL;
Entity* jolly = NULL;
SceneNode* jollyNode = NULL;
Entity* santoso = NULL;
SceneNode* santosoNode = NULL;
Entity* terranceDougie = NULL;
SceneNode* terranceDougieNode = NULL;
Entity* dougieTerrance = NULL;
SceneNode* dougieTerranceNode = NULL;
Entity* dannyBoy = NULL;
SceneNode* dannyBoyNode = NULL;
Entity* micheal = NULL;
SceneNode* michealNode = NULL;
Entity* johnny666 = NULL;
SceneNode* johnny666Node = NULL;
Entity* trainerEnt = NULL;
SceneNode* trainerNode = NULL;
Entity* guardNPC = NULL;
SceneNode* guardNPCNode = NULL;

//level entities and nodes
Entity* cellsEntity = NULL;
SceneNode* cellsNode = NULL;
Entity* cellsCollisionEntity = NULL;
SceneNode* cellsCollisionNode = NULL;

//camera entities and nodes
Entity* camera1Entity = NULL;
SceneNode* camera1Node = NULL;
Entity* camera2Entity = NULL;
SceneNode* camera2Node = NULL;
Entity* camera3Entity = NULL;
SceneNode* camera3Node = NULL;
Entity* camera4Entity = NULL;
SceneNode* camera4Node = NULL;

//night time node//
Entity* nightTime = NULL;
SceneNode* nightTimeNode = NULL;

Entity* cafeteriaEntity = NULL;
SceneNode* cafeteriaNode = NULL;

Entity* cafeteriaCollisionEntity = NULL;
SceneNode* cafeteriaCollisionNode = NULL;

Entity* courtyardEntity = NULL;
SceneNode* courtyardNode = NULL;
Entity* courtyardCEntity = NULL;
SceneNode* courtyardCNode = NULL;

//camera and collision nodes
SceneNode* cameraNodeLTS = NULL;
SceneNode* forwardCollisionNodeLTS = NULL;
SceneNode* backwardCollisionNodeLTS = NULL;
SceneNode* leftCollisionNodeLTS = NULL;
SceneNode* rightCollisionNodeLTS = NULL;

//camera variables
Vector3 cameraInitialPosLTS; 
Quaternion cameraInitialRotLTS;

//main character 
Entity* temp = NULL;


Entity* ShirtEntityLTS = NULL;
Entity* ShirtlessEntityLTS = NULL;
SceneNode* PlayerNodeLTS = NULL;

Entity* robot_entity2LTS = NULL;
SceneNode* robot_node2LTS = NULL;
Entity* robot_entity3LTS = NULL;
SceneNode* robot_node3LTS = NULL;
Entity* robot_entity4LTS = NULL;
SceneNode* robot_node4LTS = NULL;
Entity* robot_entity5LTS = NULL;
SceneNode* robot_node5LTS = NULL;

Entity* AI_Entity1LTS = NULL;
SceneNode* AI_Node1LTS = NULL;
Entity* AI_Entity2LTS = NULL;
SceneNode* AI_Node2LTS = NULL;
Entity* AI_Entity3LTS = NULL;
SceneNode* AI_Node3LTS = NULL;
Entity* AI_Entity4LTS = NULL;
SceneNode* AI_Node4LTS = NULL;
Entity* AI_Entity5LTS = NULL;
SceneNode* AI_Node5LTS = NULL;

//objects to change levels//
Entity* object_Entity1LTS = NULL;
SceneNode* object_Node1LTS = NULL;
Entity* object_Entity2LTS = NULL;
SceneNode* object_Node2LTS = NULL;
Entity* object_Entity3LTS = NULL;
SceneNode* object_Node3LTS = NULL;
Entity* object_Entity4LTS = NULL;
SceneNode* object_Node4LTS = NULL;

//collision cube
Entity* collision_EntityLTS = NULL;
SceneNode* objectCollisionNodeLTS = NULL;

AnimationState* robot_anim1LTS = NULL;
AnimationState* AI_anim1LTS = NULL;
AnimationState* AI_anim2LTS = NULL;
AnimationState* AI_anim3LTS = NULL;
AnimationState* AI_anim4LTS = NULL;
AnimationState* AI_anim5LTS = NULL;

//character animations//
AnimationState* jolly_Idle = NULL;
AnimationState* micheal_Idle = NULL;
AnimationState* linMaSit = NULL;
AnimationState* johnny666_Idle = NULL;
AnimationState* dannyBoy_Idle = NULL;
AnimationState* terranceDougie_Idle = NULL;
AnimationState* dougieTerrance_Idle = NULL;
AnimationState* santoso_Idle = NULL;

//LevelLoaded = 0//
AnimationState* batonGrab = NULL;
AnimationState* batonHit = NULL;
AnimationState* guardWalk = NULL;


//door nodes/Entities//
Entity* playerCellDoor = NULL;
SceneNode* playerCellDoorNode = NULL;
Entity* playerCellDoorCol = NULL;
SceneNode* playerCellDoorColNode = NULL;

//door right of players cell//
Entity* rightOfPlayerCellCol = NULL;
SceneNode* rightOfPlayerCellColNode = NULL;
Entity* rightOfPlayerCell = NULL;
SceneNode* rightOfPlayerCellNode = NULL;

//johnny666 door//
Entity* johnny666CellDoorCol = NULL;
SceneNode* johnny666CellDoorColNode = NULL;
Entity* johnny666CellDoor = NULL;
SceneNode* johnny666CellDoorNode = NULL;

//danny boys cell door//
Entity* dannyBoyCellDoorCol = NULL;
SceneNode* dannyBoyCellDoorColNode = NULL;
Entity* dannyBoyCellDoor = NULL;
SceneNode* dannyBoyCellDoorNode = NULL;

//jollys cell door//
Entity* jollyCellDoorCol = NULL;
SceneNode* jollyCellDoorColNode = NULL;
Entity* jollyCellDoor = NULL;
SceneNode* jollyCellDoorNode = NULL;

//empty cell next to jollys//
Entity* emptyCellJollyDoorCol = NULL;
SceneNode* emptyCellJollyDoorColNode = NULL;
Entity* emptyCellJollyDoor = NULL;
SceneNode* emptyCellJollyDoorNode = NULL;

//chaos cult cells doors//
SceneNode* chaosCellDoorColNode = NULL;
Entity* chaosCellDoorCol = NULL;
Entity* chaosCellDoor = NULL;
SceneNode* chaosCellDoorNode = NULL;

//empty right corner cell//
Entity* emptyRightCornerDoorCol = NULL;
SceneNode* emptyRightCornerDoorColNode = NULL;
Entity* emptyRightCornerDoor = NULL;
SceneNode* emptyRightCornerDoorNode = NULL;

//terrence + dougle stalls//
Entity* terranceDougieCellDoorCol = NULL;
SceneNode* terranceDougieCellDoorColNode = NULL;
Entity* terranceDougieCellDoor = NULL;
SceneNode* terranceDougieCellDoorNode = NULL;

//empty cell terrance//
Entity* emptyTerranceCellDoorCol = NULL;
SceneNode* emptyTerranceCellDoorColNode = NULL;
Entity* emptyTerranceCellDoor = NULL;
SceneNode* emptyTerranceCellDoorNode = NULL;

//santoso shop//
Entity* santosoCellDoorCol = NULL;
SceneNode* santosoCellDoorColNode = NULL;
Entity* santosoCellDoor = NULL;
SceneNode* santosoCellDoorNode = NULL;

//lin mas office door//
Entity* linMasDoorCol = NULL;
SceneNode* linMasDoorColNode = NULL;
Entity* linMasDoor = NULL;
SceneNode* linMasDoorNode = NULL;

//door pitch counter//
Ogre::Real door01Pitch = 0;
Ogre::Real door02Pitch = 0;
Ogre::Real door03Pitch = 0;
Ogre::Real door04Pitch = 0;
Ogre::Real door05Pitch = 0;
Ogre::Real door06Pitch = 0;
Ogre::Real door07Pitch = 0;
Ogre::Real door08Pitch = 0;
Ogre::Real door09Pitch = 0;
Ogre::Real door10Pitch = 0;
Ogre::Real door11Pitch = 0;
Ogre::Real door12Pitch = 0;

//camera variables//
Ogre::Real cameraSpeed = 0.5;
Ogre::Real camera1Pitch = 0.0;
Ogre::Real camera2Pitch = 0.0;
Ogre::Real camera3Pitch = 0.0;
Ogre::Real camera4Pitch = 0.0;

//door variables//
bool Door01_Open = false;
bool Door02_Open = false;
bool Door03_Open = false;
bool Door04_Open = false;
bool Door05_Open = false;
bool Door06_Open = false;
bool Door07_Open = false;
bool Door08_Open = false;
bool Door09_Open = false;
bool Door10_Open = false;
bool Door11_Open = false;
bool Door12_Open = false;

//controller variables
float pitchLTS;
int keyboardMouse;
int invertControls;
int speed;
int charHeight;

bool isButtonPressed = false;
bool changeStance = false;

//AI Variables
int AI1MoveDuration = 0;
int AI1MoveDir = 0;
int AI1MaxMoveDuration = 5;
int AI2MoveDuration = 0;
int AI2MoveDir = 0;
int AI2MaxMoveDuration = 5;
int AI3MoveDuration = 0;
int AI3MoveDir = 0;
int AI3MaxMoveDuration = 5;
int AI4MoveDuration = 0;
int AI4MoveDir = 0;
int AI4MaxMoveDuration = 5;
int AI5MoveDuration = 0;
int AI5MoveDir = 0;
int AI5MaxMoveDuration = 5;
int randomNum;
int ratCounter;

bool gamepadControlsActiveLTS;
bool keyboardControlsActiveLTS;
bool gamepadLookInvertedLTS;
bool keyboardLookInvertedLTS;

bool pauseMenuState;
bool existingCharacter;

RaySceneQuery* raySceneQuery1LTS = NULL;
RaySceneQuery* raySceneQuery2LTS = NULL;
RaySceneQuery* forwardCollisionLTS = NULL;
RaySceneQuery* backwardCollisionLTS = NULL;
RaySceneQuery* leftCollisionLTS = NULL;
RaySceneQuery* rightCollisionLTS = NULL;

RaySceneQuery* AI1CollisionLTS = NULL;
RaySceneQuery* AI2CollisionLTS = NULL;
RaySceneQuery* AI3CollisionLTS = NULL;
RaySceneQuery* AI4CollisionLTS = NULL;
RaySceneQuery* AI5CollisionLTS = NULL;

Ogre::Vector3 robot_velocity1LTS = Ogre::Vector3::ZERO;
Ogre::Vector3 robot_velocity2LTS = Ogre::Vector3::ZERO;

void LevelTestState::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

LevelTestState::LevelTestState( void ) 
{
	mClassName = "LevelTest";
	mTime = 0.0f;
}

LevelTestState::~LevelTestState( void ) 
{
}

void GetMeshInformationLTS(const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, unsigned long* &indices, const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );

        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }

        // Add the indices
        index_count += submesh->indexData->indexCount;
    }


    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];

    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
            float* pReal;

            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }


        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                    static_cast<unsigned long>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
} 

bool RaycastFromPointLTS(RaySceneQuery* query, Ogre::Entity* parent, const Vector3 &point, const Vector3 &normal, Vector3 &result)
{
    // create the ray to test
    Ogre::Ray ray(Ogre::Vector3(point.x, point.y, point.z),
                  Ogre::Vector3(normal.x, normal.y, normal.z));

    // check we are initialised
    if (query != NULL)
    {
        // create a query object
        query->setRay(ray);

        // execute the query, returns a vector of hits
        if (query->execute().size() <= 0)
        {
            // raycast did not hit an objects bounding box
            return (false);
        }
    }
    else
    {
        // LOG_ERROR << "Cannot raycast without RaySceneQuery instance" << ENDLOG;
        return (false);
    }   

    // at this point we have raycast to a series of different objects bounding boxes.
    // we need to test these different objects to see which is the first polygon hit.
    // there are some minor optimizations (distance based) that mean we wont have to
    // check all of the objects most of the time, but the worst case scenario is that
    // we need to test every triangle of every object.
    Ogre::Real closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
	Ogre::String closest_name = "";
    Ogre::RaySceneQueryResult &query_result = query->getLastResults();
    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        // stop checking if we have found a raycast hit that is closer
        // than all remaining entities
        if ((closest_distance >= 0.0f) &&
            (closest_distance < query_result[qr_idx].distance))
        {
             break;
        }
       
        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != NULL) &&
            (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
        {
            // get the entity to check
            Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);           
			if(pentity != parent)
			{
				// mesh data to retrieve         
				size_t vertex_count;
				size_t index_count;
				Ogre::Vector3 *vertices;
				unsigned long *indices;

				// get the mesh information
				GetMeshInformationLTS(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
								  pentity->getParentNode()->getWorldPosition(),
								  pentity->getParentNode()->getWorldOrientation(),
								  pentity->getParentNode()->getScale());

				// test for hitting individual triangles on the mesh
				bool new_closest_found = false;
				for (int i = 0; i < static_cast<int>(index_count); i += 3)
				{
					// check for a hit against this triangle
					std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
						vertices[indices[i+1]], vertices[indices[i+2]], true, false);

					// if it was a hit check if its the closest
					if (hit.first)
					{
						if ((closest_distance < 0.0f) ||
							(hit.second < closest_distance))
						{
							// this is the closest so far, save it off
							closest_distance = hit.second;
							new_closest_found = true;
							closest_name = pentity->getName();
						}
					}
				}

			 // free the verticies and indicies memory
				delete[] vertices;
				delete[] indices;

				// if we found a new closest raycast for this object, update the
				// closest_result before moving on to the next object.
				if (new_closest_found)
				{
					closest_result = ray.getPoint(closest_distance);               
				}
			}
        }       
    }

    // return the result
    if (closest_distance >= 0.0f)
    {
        // raycast success
        result = closest_result;
        return (true);
    }
    else
    {
        // raycast failed
        return (false);
    } 
}
void LevelTestState::enter( void ) 
{
	stickReleased = true;
	gamepadReleased = true;
	changeStance = false;
	dialogueShuffle = false;
	speed = 6;
	charHeight = 68;
	fight = false;

	story01FadeVar = 1;
	story02FadeVar = 1;

	loadOptions();
	loadStats();

	//LOAD IN SOUNDS
	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	existingCharacter = false;

    mRoot         = Root::getSingletonPtr();
	mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
	mSceneMgr->setAmbientLight( ColourValue( 0, 0, 0 ) );
    mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	
	//set camera position and rotation
	cameraInitialPosLTS = Vector3( 0.0f, 0.0f, 0.0f );
	cameraInitialRotLTS = Quaternion(Degree(-90.0f), Vector3::UNIT_Y);
	
	// create main camera + node//
    mCamera       = mSceneMgr->createCamera( "LevelTestStateCamera" );
	mCamera->setNearClipDistance(5.0f);
	mCamera->setFarClipDistance(200);
	mCamera->setFOVy(Radian(Degree(50.0f)));
	mCamera->setPosition(cameraInitialPosLTS);
	mCamera->setOrientation(cameraInitialRotLTS);
		
	cameraNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("state5CameraNode");
	cameraNodeLTS->attachObject(mCamera);

	// create directional light
	mainLight1 = mSceneMgr->createLight("MainLight1");
	mainLight1->setType(Light::LT_POINT);
	//mainLight1->setDirection( 0, -1, -1 ); 
	//mainLight1->setPosition(-150.0f, 300.0f, -75.0f);
	//mainLight1->setPowerScale(1.0f);
	mainLight1->setCastShadows(false);

	// create directional light
	mainLight2 = mSceneMgr->createLight("MainLight2");
	mainLight2->setType(Light::LT_DIRECTIONAL);
	mainLight2->setDirection(Vector3( 1, 1, 1 )); 
	//mainLight2->setPosition(975.0f, 300.0f, 2085.0f );
	mainLight2->setPowerScale(1.0f);
	mainLight2->setCastShadows(false);

	mainLight3 = mSceneMgr->createLight("MainLight3");
	mainLight3->setType(Light::LT_DIRECTIONAL);
	mainLight3->setDirection( -1, -1, -1 ); 
	//mainLight3->setPosition(-150.0f, 300.0f, 2085.0f);
	mainLight3->setPowerScale(1.0f);
	mainLight3->setCastShadows(false);	

	//no faction flags//
	 noFactionJolly = false;
	 noFactionGemini = false;

	//lin mas final quest//
	 linMasFinalQuest = false;
	 linMasFinalQuestCompleted = false;
	 linMasFinalQuestFailure = false;
	 linMasTraining = false;
	 linMasTrainingNotCompleted = false;
	 linMasTrainingCompleted = false;

	//johnny666 quests//
	//johnny666 no rep//
	 johnny666NoRep = false;
	 johnny666Rep = false;
	 johnny666CheatALittle = false;
	 johnny666CheatALittleDialogueCompleted = false;
	 johnny666CheatALittleDialogueFailed = false;
	 johnny66680Rep = false;
	 johnny666TrainingDone = false;
	 santosoShop = false;
	 santosoTalkToTheSwordQuestCompleted = false;
	 santosoKeyToThePlanQuestCompleted = false;

	//aligned with gemini//
	//geminis quest completed//
	 geminiQuestsCompleted = false;
	 geminiThePrideOfJolly = false;
	 geminiThePrideOfJollyDialogueCompleted = false;
	 geminiThePrideOfJollyDialogueFailed = false;
	 geminiKnockEmOut = false;
	 geminiKnockEmOutDialogueCompleted = false;
	 geminiKnockEmOutDialogueFailed = false;
	 geminiKeyToThePlan = false;
	 geminiTalkToTheSwordTwo = false;
	 geminiTalkToTheSwordTwoDialogueCompleted = false;
	 geminiTalkToTheSwordTwoDialogueFailed = false;
	 geminiTalkToTheSword = false;
	 geminiTalkToTheSwordDialogueCompleted = false;
	 factionedGeminiJolly = false;

	 //santoso dialouge//
	 santosoDialogue = false;

	 //jollys quests//
	 //aligned with jolly talking with gemini//
	 facionedJollyGemini = false;
	 factionedPickedJolly = false;
	 factionedPickedGemini = false;
	 factionedPickedJollyQuestCompleted = false;
	 factionedJolly = false;
	 factionedGemini = false;

	 jollyQuestsCompleted = false;
	 jollyTwoPendants = false;
	 jollyTwoPendantsDialogueCompleted = false;
	 jollyTwoPendantsDialogueFailed = false;
	 jollyKnockEmOut = false;
	 jollyKnockEmOutDialogueCompleted = false;
	 jollyKnockEmOutDialogueFailed = false;
	 jollyKeyToThePlanCompleted = false;
	 jollyKeyToThePlanCompletedDialogueCompleted = false;
	 jollyKeyToThePlanCompletedDialogueFailed = false;
	 santosoKeyToThePlanCompleted = false;
	 santosoKeyToThePlanCompletedDialogue = false;
	 jollyKeyToThePlan = false;
	 jollyKeyToThePlanDialogueCompleted = false;
	 jollyKeyToThePlanDialogueFailed = false;

	 santosoKeyToThePlan = false;
	 santosoKeyToThePlanDialogueCompleted = false;
	 santosoKeyToThePlanDialogueFailed = false;
	 santosoTalkToTheSword = false;
	 santosoTalkToTheSwordDialogueCompleted = false;
	 santosoTalkToTheSwordDialogueFailed = false;
	 jollyTalkToTheSword = false;
	 jollyTalkToTheSwordDialogueCompleted = false;
	 jollyTalkToTheSwordDialogueFailed = false;


	 //lin ma train//
	 linMaTrain = false;
	 linMaTrainDialogueCompleted = false;
	 linMaTrainDialogueFailed = false;
	 linMasWhoIsEscaping = false;
	 linMasWhoIsEscapingDialogueCompleted = false;
	 linMasWhoIsEscapingDialogueFailed = false;
	 linMasTheRatScuttle = false;
	 linMasTheRatScuttleDialogueCompleted = false;
	 linMasTheRatScuttleDialogueFailed = false;
	 linMasTheRatScuttleCompleted = false;
	 linMasMoreBugsInTheSystem = false;
	 limMasMoreBugsInTheSystemDialogueCompleted = false;
	 limMasMoreBugsInTheSystemDialogueFailed = false;
	 linMasBugInTheSystem = false;
	 linMasBugInTheSystemDialogueCompleted = false;
	 linMasBugInTheSystemDialogueFailed = false;
	 linMasPickAFactionDialogueCompleted = false;
	 linMasPickAFaction = false;
	 linMasPickAFactionCompleted = false;

	 santosoDialogueCompleted = false;

	loadCharacter();
	initInventory();
	initQuests();
	
	if(existingCharacter)
	{
		LevelLoaded = 0;
		loadCellsLevel();
	}
	else
	{
		loadInitialLevel();
	}

	// Add joystick 0 and 1
	mGamePadsLTS.push_back(new GamePadController(0, 4000));
	mGamePadsLTS.push_back(new GamePadController(1, 4000));

	previous_position = cameraNodeLTS->getPosition();

	// load options.xml and assign appropriate options variables
	TiXmlDocument loadDocOptions("Options.xml");  
	loadDocOptions.LoadFile();
	
	TiXmlElement *settings = loadDocOptions.RootElement()->FirstChildElement("Settings");
	keyboardMouse = atoi(settings->Attribute("Keyboard"));
	invertControls = atoi(settings->Attribute("Invert"));
	
	if(inConversation == false)
	{
		if(keyboardMouse == 1)
		{
			gamepadControlsActiveLTS = false;
			keyboardControlsActiveLTS = true;
		}
		else 
		{
			gamepadControlsActiveLTS = true;
			keyboardControlsActiveLTS = false;
		}
		
		if(invertControls == 1)
		{
			gamepadLookInvertedLTS = true;
			keyboardLookInvertedLTS= true;
		}
		else
		{
			gamepadLookInvertedLTS = false;
			keyboardLookInvertedLTS= false;
		}
	}

	 // Create RaySceneQuery to keep Robots on the floor
	raySceneQuery1LTS = mSceneMgr->createRayQuery(Ray());
	raySceneQuery2LTS = mSceneMgr->createRayQuery(Ray());
	forwardCollisionLTS = mSceneMgr->createRayQuery(Ray());
	backwardCollisionLTS = mSceneMgr->createRayQuery(Ray());
	leftCollisionLTS = mSceneMgr->createRayQuery(Ray());
	rightCollisionLTS = mSceneMgr->createRayQuery(Ray());
	AI1CollisionLTS = mSceneMgr->createRayQuery(Ray());
	AI2CollisionLTS = mSceneMgr->createRayQuery(Ray());
	AI3CollisionLTS = mSceneMgr->createRayQuery(Ray());
	AI4CollisionLTS = mSceneMgr->createRayQuery(Ray());
	AI5CollisionLTS = mSceneMgr->createRayQuery(Ray());

	mWindow		  = GameManager::getSingletonPtr()->getRenderWindow();
	mWindow->addViewport( mCamera );

	initHUD();
}


bool LevelTestState::checkDistance( Ogre::SceneNode* node1, Ogre::SceneNode* node2)
{
	if( node1->getWorldPosition().x + 50 > node2->getWorldPosition().x   )
	{
		if( node1->getWorldPosition().x  - 50 < node2->getWorldPosition().x )
		{
			if( node1->getWorldPosition().z  + 50 > node2->getWorldPosition().z )
			{
				if( node1->getWorldPosition().z  - 50 < node2->getWorldPosition().z )
				{
					return true;
				}
			}
		}
	}
	else
		return false;

}
void LevelTestState::loadCafeteriaLevel( void )
{
	audio_result = system->createSound("Sounds/SFX/Heartbeat Slow Loop.wav", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
	ERRCHECK(audio_result);

	if(levelsSFXSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
		ERRCHECK(audio_result);
	}

	srand(time(NULL));	
	changeInHealth = true;	

	//load cafeteria level
	cafeteriaEntity = mSceneMgr->createEntity("Cafeteria", "Cafeteria.mesh");
	cafeteriaEntity->setCastShadows(false);
	cafeteriaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Cafeteria" );
	cafeteriaNode->setPosition(Vector3( 0.0f, -1.0f, 0.0f ));
	cafeteriaNode->attachObject( cafeteriaEntity );

	mEntitiesLTS.push_back(cafeteriaEntity);
	mSceneNodesLTS.push_back(cafeteriaNode);

	//load cafeteria level
	cafeteriaCollisionEntity = mSceneMgr->createEntity("CafeteriaCollision", "Cafeteria_Collision.mesh");
	cafeteriaCollisionEntity->setCastShadows(false);
	cafeteriaCollisionEntity->setVisible(false);
	cafeteriaCollisionNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CafeteriaCollision" );
	cafeteriaCollisionNode->setPosition(Vector3( 0.0f, -1.0f, 0.0f ));
	cafeteriaCollisionNode->attachObject( cafeteriaCollisionEntity );

	mEntitiesLTS.push_back(cafeteriaCollisionEntity);
	mSceneNodesLTS.push_back(cafeteriaCollisionNode);

	if(LevelLoaded == 1)
	{
		LevelLoaded = 2;


		ShirtEntityLTS = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirtNoTorso.mesh");
		ShirtlessEntityLTS = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtlessNoTorso.mesh");
		PlayerNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
		PlayerNodeLTS->setPosition(Vector3( 30.0f, 0.0f, -20.0f ));
		PlayerNodeLTS->yaw(Degree(180.0f));
		mCamera->setOrientation(Quaternion(Degree(0.0f), Vector3::UNIT_Y));	  

		mPlayerEntitiesLTS.push_back(ShirtEntityLTS);
		mPlayerEntitiesLTS.push_back(ShirtlessEntityLTS);
		mPlayerSceneNodesLTS.push_back(PlayerNodeLTS);

		loadLevelCharacter();

		//rotate camera and character to face proper direction

		mainLightNode = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("mainLightNode");
		mainLightNode->setPosition( Vector3(2,70,0) );
		mainLightNode->attachObject( mainLight1 );

		//mPlayerSceneNodesLTS.push_back(mainLightNode);

		// set animation state
		if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rest" );}
		else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rest" );}
		robot_anim1LTS->setLoop( true );
		robot_anim1LTS->setEnabled( true );
	}

	if(LevelLoaded == 3)
	{
		LevelLoaded = 2;

		ShirtEntityLTS = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirtNoTorso.mesh");
		ShirtlessEntityLTS = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtlessNoTorso.mesh");
		PlayerNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
		PlayerNodeLTS->setPosition(Vector3( 20.0f, 0.0f, -1075.0f )); 
		PlayerNodeLTS->yaw(Degree(0.0f));
		mCamera->setOrientation(Quaternion(Degree(180.0f), Vector3::UNIT_Y));	  
	  
		mPlayerEntitiesLTS.push_back(ShirtEntityLTS);
		mPlayerEntitiesLTS.push_back(ShirtlessEntityLTS);
		mPlayerSceneNodesLTS.push_back(PlayerNodeLTS);

		loadLevelCharacter();
	
		mainLightNode = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("mainLightNode");
		mainLightNode->setPosition( Vector3(2,70,0) );
		mainLightNode->attachObject( mainLight1 );

		//mPlayerSceneNodesLTS.push_back(mainLightNode);

		// set animation state
		if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rest" );}
		else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rest" );}
		robot_anim1LTS->setLoop( true );
		robot_anim1LTS->setEnabled( true );
	}
	
	//Create object collision node
	collision_EntityLTS = mSceneMgr->createEntity( "cube", "cube.mesh" );
	collision_EntityLTS->setCastShadows(true);
	objectCollisionNodeLTS = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("objectCollisionNodeLTS");
	objectCollisionNodeLTS->setPosition( Vector3(0,30,50) );
	objectCollisionNodeLTS->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	objectCollisionNodeLTS->attachObject( collision_EntityLTS );
	objectCollisionNodeLTS->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	objectCollisionNodeLTS->setVisible(false);

	mEntitiesLTS.push_back(collision_EntityLTS);

	//collision detection nodes//
	forwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("forwardCollisionNodeLTS");
	backwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("backwardCollisionNodeLTS");
	leftCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("leftCollisionNodeLTS");
	rightCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("rightCollisionNodeLTS");
	mSceneNodesLTS.push_back(forwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(backwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(leftCollisionNodeLTS);
	mSceneNodesLTS.push_back(rightCollisionNodeLTS);

	//jolly//
	jolly = mSceneMgr->createEntity("Jolly", "VGD_Alone_char_jolly.mesh");
	jollyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("JollyNode");
	jollyNode->attachObject(jolly);
	jollyNode->setPosition(Vector3(-822.0f, 0.0f, -298.0f));
	jollyNode->rotate(Vector3::UNIT_Y,Degree(180));
	jollyNode->setVisible(true);
	mPlayerEntitiesLTS.push_back(jolly);
	mPlayerSceneNodesLTS.push_back(jollyNode);

	jolly_Idle = jolly->getAnimationState("sitRestIn");
	jolly_Idle->setEnabled(true);
	jolly_Idle->setLoop(true);

	////Sets AutoTracking on the NPCS//
	//jollyNode->setAutoTracking(true,PlayerNodeLTS,Vector3::UNIT_Z);
	//jollyNode->setFixedYawAxis(true);

	//micheal//
	micheal = mSceneMgr->createEntity("Micheal", "VGD_Alone_char_michael.mesh");
	michealNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MichealNode");
	michealNode->attachObject(micheal);
	michealNode->setPosition(Vector3(-400.0f,0.0f,-560.0f));
	michealNode->rotate(Vector3::UNIT_Y,Degree(180));
	michealNode->setVisible(true);
	mPlayerEntitiesLTS.push_back(micheal);
	mPlayerSceneNodesLTS.push_back(michealNode);

	micheal_Idle = micheal->getAnimationState("sitRestIn");
	micheal_Idle->setEnabled(true);
	micheal_Idle->setLoop(true);

	////Sets AutoTracking on the NPCS//
	//michealNode->setAutoTracking(true,PlayerNodeLTS,Vector3::UNIT_Z);
	//michealNode->setFixedYawAxis(true);

	//node to transfer to the courtyard//
	object_Node2LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Barrel" );
	object_Node2LTS->setPosition(Vector3( 15.0f, 30.0f, -1130.0f  ));
	object_Node2LTS->scale(Vector3( 4.0f, 4.0f, 4.0f ));
	
	mSceneNodesLTS.push_back(object_Node2LTS);

	//node to transfer to the cells//

	object_Node3LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "knot" );
	object_Node3LTS->setPosition(Vector3( -25.0f, 30.0f, 30.0f ));
	object_Node3LTS->scale(Vector3( 0.125f, 0.125f, 0.125f ));
	
	mSceneNodesLTS.push_back(object_Node3LTS);

	AI_Entity1LTS = mSceneMgr->createEntity( "Fish", "VGD_Alone_Char_Rat.mesh" );
	AI_Entity1LTS->setCastShadows(false);
	AI_Node1LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Fish" );
	AI_Node1LTS->scale(Vector3( 0.25f, 0.25f, 0.25f ));
	AI_Node1LTS->translate(Vector3( -1325.0f, 0.0f, -945.0f ));
    AI_Node1LTS->attachObject( AI_Entity1LTS );
	//AI_Node1LTS->yaw(Degree(90));

	mEntitiesLTS.push_back(AI_Entity1LTS);
	mSceneNodesLTS.push_back(AI_Node1LTS);

	AI_anim1LTS = AI_Entity1LTS->getAnimationState( "RatWalk" );
    AI_anim1LTS->setLoop( true );
    AI_anim1LTS->setEnabled( true );

	AI_Entity2LTS = mSceneMgr->createEntity( "Fish1", "VGD_Alone_Char_Rat.mesh" );
	AI_Entity2LTS->setCastShadows(false);
	AI_Node2LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Fish1" );
	AI_Node2LTS->scale(Vector3( 0.25f, 0.25f, 0.25f ));
	AI_Node2LTS->translate(Vector3( -375.0f, 0.0f, -850.0f ));
    AI_Node2LTS->attachObject( AI_Entity2LTS );
	//AI_Node2LTS->yaw(Degree(-90));

	mEntitiesLTS.push_back(AI_Entity2LTS);
	mSceneNodesLTS.push_back(AI_Node2LTS);

	AI_anim2LTS = AI_Entity2LTS->getAnimationState( "RatWalk" );
    AI_anim2LTS->setLoop( true );
    AI_anim2LTS->setEnabled( true );

	AI_Entity3LTS = mSceneMgr->createEntity( "Fish3", "VGD_Alone_Char_Cockroach.mesh" );
	AI_Entity3LTS->setCastShadows(false);
	AI_Node3LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Fish3" );
	AI_Node3LTS->scale(Vector3( 2.0f, 2.0f, 2.0f ));
	AI_Node3LTS->translate(Vector3( 65.0f, 50.0f, -350.0f ));
    AI_Node3LTS->attachObject( AI_Entity3LTS );
	AI_Node3LTS->rotate(Vector3::UNIT_X, Degree(-90.0f));
	AI_Node3LTS->rotate(Vector3::UNIT_Z, Degree(90.0f));
	
	mEntitiesLTS.push_back(AI_Entity3LTS);
	mSceneNodesLTS.push_back(AI_Node3LTS);

	AI_anim3LTS = AI_Entity3LTS->getAnimationState( "CockroachWalk" );
    AI_anim3LTS->setLoop( true );
    AI_anim3LTS->setEnabled( true );

	AI_Entity4LTS = mSceneMgr->createEntity( "Fish4", "VGD_Alone_Char_Cockroach.mesh" );
	AI_Entity4LTS->setCastShadows(false);
	AI_Node4LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Fish4" );
	AI_Node4LTS->scale(Vector3( 2.0f, 2.0f, 2.0f ));
	AI_Node4LTS->translate(Vector3( -500.0f, 50.0f, -1125.0f ));
    AI_Node4LTS->attachObject( AI_Entity4LTS );
	AI_Node4LTS->rotate(Vector3::UNIT_X, Degree(90.0f));
	//AI_Node4LTS->rotate(Vector3::UNIT_Z, Degree(90.0f));
	
	mEntitiesLTS.push_back(AI_Entity4LTS);
	mSceneNodesLTS.push_back(AI_Node4LTS);

	AI_anim4LTS = AI_Entity4LTS->getAnimationState( "CockroachWalk" );
    AI_anim4LTS->setLoop( true );
    AI_anim4LTS->setEnabled( true );

	AI_Entity5LTS = mSceneMgr->createEntity( "Fish5", "VGD_Alone_Char_Cockroach.mesh" );
	AI_Entity5LTS->setCastShadows(false);
	AI_Node5LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Fish5" );
	AI_Node5LTS->scale(Vector3( 2.0f, 2.0f, 2.0f ));
	AI_Node5LTS->translate(Vector3( -750.0f, 50.0f, 25.0f ));
    AI_Node5LTS->attachObject( AI_Entity5LTS );
	AI_Node5LTS->rotate(Vector3::UNIT_X, Degree(-90.0f));
	//AI_Node5LTS->rotate(Vector3::UNIT_Z, Degree(90.0f));

	mEntitiesLTS.push_back(AI_Entity5LTS);
	mSceneNodesLTS.push_back(AI_Node5LTS);

	AI_anim5LTS = AI_Entity5LTS->getAnimationState( "CockroachWalk" );
    AI_anim5LTS->setLoop( true );
    AI_anim5LTS->setEnabled( true );

	AI1MoveDir = 2;
	AI1MaxMoveDuration = 300;
	AI1MoveDuration = 0;

	AI2MoveDir = 1;
	AI2MaxMoveDuration = 300;
	AI2MoveDuration = 0;

	AI3MoveDir = 0;
	AI3MaxMoveDuration = 300;
	AI3MoveDuration = 0;

	AI4MoveDir = 0;
	AI4MaxMoveDuration = 300;
	AI4MoveDuration = 0;

	AI5MoveDir = 0;
	AI5MaxMoveDuration = 300;
	AI5MoveDuration = 0;

	//reset variables
	pitchLTS = 0;

	audio_result = system->createStream("Sounds/Music/MainMenu.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Creatures/VGD_SFX_Rat_Killed.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &rat_Squished);
	ERRCHECK(audio_result);

	if(levelsMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}
}

void LevelTestState::loadCourtyardLevel( void )
{
	LevelLoaded = 3;

	audio_result = system->createSound("Sounds/SFX/Heartbeat Slow Loop.wav", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
	ERRCHECK(audio_result);

	if(levelsSFXSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
		ERRCHECK(audio_result);
	}

	changeInHealth = true;	

	// load Courtyard level
	courtyardEntity = mSceneMgr->createEntity("Courtyard", "Courtyard.mesh");
	courtyardEntity->setCastShadows(false);
	courtyardEntity->setVisible(true);
	courtyardNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Courtyard" );
	courtyardNode->setPosition(Vector3( 0.0f, 0.0f, 0.0f ));
    courtyardNode->attachObject( courtyardEntity );

	mEntitiesLTS.push_back(courtyardEntity);
	mSceneNodesLTS.push_back(courtyardNode);

	// load Courtyard level
	courtyardCEntity = mSceneMgr->createEntity("Courtyard_Collision", "Courtyard_Collision.mesh");
	courtyardCEntity->setCastShadows(false);
	courtyardCEntity->setVisible(false);
	courtyardCNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Courtyard_Collision" );
	courtyardCNode->setPosition(Vector3( 0.0f, 0.0f, 0.0f ));
    courtyardCNode->attachObject( courtyardCEntity );

	mEntitiesLTS.push_back(courtyardCEntity);
	mSceneNodesLTS.push_back(courtyardCNode);

	mSceneMgr->setSkyDome(true, "Examples/CloudySky",10,8,3000);

	ShirtEntityLTS = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirtNoTorso.mesh");
	ShirtlessEntityLTS = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtlessNoTorso.mesh");
	PlayerNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
	PlayerNodeLTS->setPosition(Ogre::Vector3( 0.0f, 1.0f, 100.0f ));
	PlayerNodeLTS->yaw(Degree(0.0f));
	mCamera->setOrientation(Quaternion(Degree(-180.0f), Vector3::UNIT_Y));	  
  
	mPlayerEntitiesLTS.push_back(ShirtEntityLTS);
	mPlayerEntitiesLTS.push_back(ShirtlessEntityLTS);
	mPlayerSceneNodesLTS.push_back(PlayerNodeLTS);

	loadLevelCharacter();

	mainLightNode = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("mainLightNode");
	mainLightNode->setPosition( Vector3(2,70,0) );
	mainLightNode->attachObject( mainLight1 );

	// set animation state
	if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rest" );}
	else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rest" );}
    robot_anim1LTS->setLoop( true );
    robot_anim1LTS->setEnabled( true );

	//mPlayerSceneNodesLTS.push_back(mainLightNode);

	//create object collision node 
	collision_EntityLTS = mSceneMgr->createEntity( "cube", "cube.mesh" );
	collision_EntityLTS->setCastShadows(true);
	objectCollisionNodeLTS = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("objectCollisionNodeLTS");
	objectCollisionNodeLTS->setPosition( Vector3(0,30,50) );
	objectCollisionNodeLTS->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	objectCollisionNodeLTS->attachObject( collision_EntityLTS );
	objectCollisionNodeLTS->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	objectCollisionNodeLTS->setVisible(false);

	mEntitiesLTS.push_back(collision_EntityLTS);
	//npc nodes//
	//Trainer//
	trainerEnt = mSceneMgr->createEntity("Trainer", "johnnyModel.mesh");
	trainerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Trainer");
	trainerNode->attachObject(trainerEnt);
	trainerNode->setPosition(Vector3(-1175.0f, 0.0f, 1275.0f));
	trainerNode->rotate(Vector3::UNIT_Y, Degree(90.0f));
	//trainerNode->setVisible(false);
	mPlayerEntitiesLTS.push_back(trainerEnt);
	mPlayerSceneNodesLTS.push_back(trainerNode);

	johnny666_Idle = trainerEnt->getAnimationState("rest");
	johnny666_Idle->setEnabled(true);
	johnny666_Idle->setLoop(true);

	//Terrance and dougie//
	terranceDougie = mSceneMgr->createEntity("Terrance","dougieModel.mesh");
	terranceDougieNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("TerranceNode");
	terranceDougieNode->attachObject(terranceDougie);
	terranceDougieNode->setPosition(Vector3(-1504.0f,0.0f,745.0f));
	mPlayerEntitiesLTS.push_back(terranceDougie);
	mPlayerSceneNodesLTS.push_back(terranceDougieNode);

	terranceDougie_Idle = terranceDougie->getAnimationState("sitRestIn");
	terranceDougie_Idle->setEnabled(true);
	terranceDougie_Idle->setLoop(true);

	
		//Terrance and dougie//
	dougieTerrance = mSceneMgr->createEntity("dougieTerrance","terrenceModel.mesh");
	dougieTerranceNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("dougieTerranceNode");
	dougieTerranceNode->attachObject(dougieTerrance);
	dougieTerranceNode->setPosition(Vector3(-1534.0f,0.0f,745.0f));
	mPlayerEntitiesLTS.push_back(dougieTerrance);
	mPlayerSceneNodesLTS.push_back(dougieTerranceNode);

	dougieTerrance_Idle = dougieTerrance->getAnimationState("sitRestOut");
	dougieTerrance_Idle->setEnabled(true);
	dougieTerrance_Idle->setLoop(true);


	//collision detection nodes//
	forwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("forwardCollisionNodeLTS");
	backwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("backwardCollisionNodeLTS");
	leftCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("leftCollisionNodeLTS");
	rightCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("rightCollisionNodeLTS");
	mSceneNodesLTS.push_back(forwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(backwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(leftCollisionNodeLTS);
	mSceneNodesLTS.push_back(rightCollisionNodeLTS);

	//node to transfer to the cafeteria//
	object_Node4LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "FishNode" );
	object_Node4LTS->scale(Vector3( 6.0f, 6.0f, 6.0f ));
	object_Node4LTS->setPosition(Vector3( -25.0f, 30.0f, -70.0f ));
	object_Node4LTS->rotate(Vector3::UNIT_Y, Degree(90.0f));

	mSceneNodesLTS.push_back(object_Node4LTS);

	//reset variables
	pitchLTS = 0;

	// rotate camera and character to face proper direction
	mCamera->setOrientation(Quaternion(Degree(180.0f), Vector3::UNIT_Y));
	PlayerNodeLTS->yaw(Radian(0));

	audio_result = system->createStream("Sounds/Music/Credits.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	if(levelsMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}
}

void LevelTestState::loadCellsLevel( void )
{
	audio_result = system->createSound("Sounds/SFX/Heartbeat Slow Loop.wav", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
	ERRCHECK(audio_result);

	if(levelsSFXSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
		ERRCHECK(audio_result);
	}

	changeInHealth = true;	
	
	/////////////////////
	//  LEVEL MODEL    //
	/////////////////////
	cellsEntity = mSceneMgr->createEntity("Cells", "VGD_Alone_lvl_cells_day.mesh");
	cellsEntity->setCastShadows(false);
	cellsNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Cells" );
	cellsNode->setPosition(Vector3( 0.0f, 0.0f, 0.0f ));
    cellsNode->attachObject( cellsEntity );
	mEntitiesLTS.push_back(cellsEntity);
	mSceneNodesLTS.push_back(cellsNode);

	cellsCollisionEntity = mSceneMgr->createEntity("CellsCollision", "VGD_Alone_lvl_cells_Collision.mesh");
	cellsCollisionEntity->setCastShadows(false);
	cellsCollisionNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CellsCollision" );
	cellsCollisionNode->setPosition(Vector3( 0.0f, 0.0f, 0.0f ));
    cellsCollisionNode->attachObject( cellsCollisionEntity );
	cellsCollisionNode->setVisible(false);

	mEntitiesLTS.push_back(	cellsCollisionEntity );
	mSceneNodesLTS.push_back(cellsCollisionNode);

	////////////////////
	//  CAMERA MODELS //
	////////////////////

	camera1Entity = mSceneMgr->createEntity("Camera 1", "VGD_Alone_prop_camera.mesh");
	camera1Entity->setCastShadows(false);
	camera1Node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Camera 1" );
	camera1Node->setPosition(Vector3( -60.0f, 190.0f, 470.0f ));
	camera1Node->setOrientation(Quaternion(Degree(45.0f), Vector3::UNIT_Y));
    camera1Node->attachObject( camera1Entity );
	mEntitiesLTS.push_back(camera1Entity);
	mSceneNodesLTS.push_back(camera1Node);

	camera2Entity = mSceneMgr->createEntity("Camera 2", "VGD_Alone_prop_camera.mesh");
	camera2Entity->setCastShadows(false);
	camera2Node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Camera 2" );
	camera2Node->setPosition(Vector3( -60.0f, 190.0f, 1510.0f ));
	camera2Node->setOrientation(Quaternion(Degree(135.0f), Vector3::UNIT_Y));
    camera2Node->attachObject( camera2Entity );
	mEntitiesLTS.push_back(camera2Entity);
	mSceneNodesLTS.push_back(camera2Node);

	camera3Entity = mSceneMgr->createEntity("Camera 3", "VGD_Alone_prop_camera.mesh");
	camera3Entity->setCastShadows(false);
	camera3Node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Camera 3" );
	camera3Node->setPosition(Vector3( 1050.0f, 190.0f, 1510.0f ));
	camera3Node->setOrientation(Quaternion(Degree(225.0f), Vector3::UNIT_Y));
    camera3Node->attachObject( camera3Entity );
	mEntitiesLTS.push_back(camera3Entity);
	mSceneNodesLTS.push_back(camera3Node);

	camera4Entity = mSceneMgr->createEntity("Camera 4", "VGD_Alone_prop_camera.mesh");
	camera4Entity->setCastShadows(false);
	camera4Node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Camera 4" );
	camera4Node->setPosition(Vector3( 1050.0f, 190.0f, 470.0f ));
	camera4Node->setOrientation(Quaternion(Degree(315.0f), Vector3::UNIT_Y));
    camera4Node->attachObject( camera4Entity );
	mEntitiesLTS.push_back(camera4Entity);
	mSceneNodesLTS.push_back(camera4Node);

	////////////////////////////////
	//     Optimization NODE      //
	////////////////////////////////
	detectNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode1" );
	detectNode1->setPosition(Vector3( 1050.0f, 0.0f, 580.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode1);

	detectNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode2" );
	detectNode2->setPosition(Vector3( -60.0f, 0.0f, 580.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode2);

	detectNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode3" );
	detectNode3->setPosition(Vector3( 1050.0f, 0.0f, 1410.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode3);

	detectNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode4" );
	detectNode4->setPosition(Vector3( -60.0f, 0.0f, 1410.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode4);

	detectNode5 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode5" );
	detectNode5->setPosition(Vector3( 540.0f, 0.0f, 475.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode5);

	detectNode6 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode6" );
	detectNode6->setPosition(Vector3( 430.0f, 0.0f, 475.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode6);


	//////////////////////////////////////
	// Loads The Char Based on Prev Pos //
	//////////////////////////////////////
	if(LevelLoaded == 0)
	{
		LevelLoaded = 1;

		ShirtEntityLTS = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirtNoTorso.mesh");
		ShirtlessEntityLTS = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtlessNoTorso.mesh");
		PlayerNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
		PlayerNodeLTS->setPosition(Ogre::Vector3(576.0f, 1.0f, 258.0f));
		PlayerNodeLTS->yaw(Degree(0.0f));
		mCamera->setOrientation(Quaternion(Degree(180.0f), Vector3::UNIT_Y));	  
	  
		mPlayerEntitiesLTS.push_back(ShirtEntityLTS);
		mPlayerEntitiesLTS.push_back(ShirtlessEntityLTS);
		mPlayerSceneNodesLTS.push_back(PlayerNodeLTS);

		loadLevelCharacter();
	}
	else if(LevelLoaded == 2)
	{
		LevelLoaded = 1;

		ShirtEntityLTS = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirtNoTorso.mesh");
		ShirtlessEntityLTS = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtlessNoTorso.mesh");
		PlayerNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
		PlayerNodeLTS->setPosition(Vector3( -60.0f, 1.0f, 1620.0f ));
		PlayerNodeLTS->yaw(Degree(180.0f));
		mCamera->setOrientation(Quaternion(Degree(0.0f), Vector3::UNIT_Y));	   
	  
		mPlayerEntitiesLTS.push_back(ShirtEntityLTS);
		mPlayerEntitiesLTS.push_back(ShirtlessEntityLTS);
		mPlayerSceneNodesLTS.push_back(PlayerNodeLTS);

		loadLevelCharacter();
	}
	else if(LevelLoaded == 4)
	{
		LevelLoaded = 1;

		ShirtEntityLTS = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirtNoTorso.mesh");
		ShirtlessEntityLTS = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtlessNoTorso.mesh");
		PlayerNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
		PlayerNodeLTS->setPosition(Vector3(576.0f, 1.0f, 258.0f));
		PlayerNodeLTS->yaw(Degree(0.0f));
		mCamera->setOrientation(Quaternion(Degree(-180.0f), Vector3::UNIT_Y));	  
	  
		mPlayerEntitiesLTS.push_back(ShirtEntityLTS);
		mPlayerEntitiesLTS.push_back(ShirtlessEntityLTS);
		mPlayerSceneNodesLTS.push_back(PlayerNodeLTS);

		loadLevelCharacter();
	}

	//set animation state
	if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rest" );}
	else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rest" );}
	robot_anim1LTS->setLoop( true );
	robot_anim1LTS->setEnabled( true );
	
	mainLightNode = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("mainLightNode");
	mainLightNode->setPosition( Vector3(2,70,0) );
	mainLightNode->attachObject( mainLight1 );
	
	//////////////////////////////
	//      CIGARETTES NODE     //
	//////////////////////////////
	emptyCellRightOfPlayerCigarettes = mSceneMgr->createEntity("EmptyCellCigs", "VGD_Alone_Prop_Cigeratte.mesh");
	emptyCellRightOfPlayerCigarettesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("EmptyCellCigsNode");
	emptyCellRightOfPlayerCigarettesNode->attachObject(emptyCellRightOfPlayerCigarettes);
	emptyCellRightOfPlayerCigarettesNode->setPosition(454.0f,50.0f,218.0f);
	//empty cell cigarettes pushed on//
	mItemsLTS.push_back(emptyCellRightOfPlayerCigarettes);
	mItemsNodeLTS.push_back(emptyCellRightOfPlayerCigarettesNode);
	
	if(itemDannySteroids == false)
	{
		//////////////////////////////
		//      DANNYBOY STEROIDS   //
		//////////////////////////////
		dannyBoySteroids = mSceneMgr->createEntity("DannyBoySteroids", "VGD_Alone_Prop_Steroid_Syringe.mesh");
		dannyBoySteroidsNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DannyBoyCellNode");
		dannyBoySteroidsNode->attachObject(dannyBoySteroids);
		dannyBoySteroidsNode->setPosition(329.0f,35.0f,734.0f);
		dannyBoySteroidsNode->setScale(2.0f,2.0f,2.0f);
		//danny boy cell node//
		mItemsLTS.push_back(dannyBoySteroids);
		mItemsNodeLTS.push_back(dannyBoySteroidsNode);
	}

	if(itemSantosoSteroids == false)
	{
		//////////////////////////////
		//    PAINKILLER SANTOSO    //
		//////////////////////////////
		santosoPainKiller = mSceneMgr->createEntity("SantosoPainKiller", "VGD_Alone_Prop_Steroid_Syringe.mesh");
		santosoPainKillerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SantosoPainKillerNode");
		santosoPainKillerNode->attachObject(santosoPainKiller);
		santosoPainKillerNode->setPosition(768.0f,33.0f,1340.0f);
		santosoPainKillerNode->setScale(2.0f,2.0f,2.0f);
		//push the painkiller nodes//
		mItemsLTS.push_back(santosoPainKiller);
		mItemsNodeLTS.push_back(santosoPainKillerNode);
	}

	if(itemLinMaMoney == false)
	{
		///////////////////////////////
		//       LIN MAS MONEY       //
		///////////////////////////////
		linMasMoney = mSceneMgr->createEntity("LinMasMoney", "VGD_Alone_prop_Money_Stack.mesh");
		linMasMoneyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LinMasMoneyNode");
		linMasMoneyNode->attachObject(linMasMoney);
		linMasMoneyNode->setPosition(641.0f,45.0f,1759.0f);
		linMasMoneyNode->setScale(3.0f,3.0f,3.0f);
		//push the nodes on//
		mItemsLTS.push_back(linMasMoney);
		mItemsNodeLTS.push_back(linMasMoneyNode);
	}

	if(itemSoap == false)
	{
		//////////////////////////////
		//           SOAP           //
		//////////////////////////////
		linMasSoap = mSceneMgr->createEntity("LinMasSoapNode","VGD_Alone_Prop_Soap_Mold.mesh");
		linMasSoapNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("linMasSoapNode");
		linMasSoapNode->attachObject(linMasSoap);
		linMasSoapNode->setPosition(691.0f,25.0f,1682.0f);
		linMasSoapNode->setScale(1.0f,1.0f,1.0f);
		//push the soap on the stack//
		mItemsLTS.push_back(linMasSoap);
		mItemsNodeLTS.push_back(linMasSoapNode);
	}


	/////////////////////////////////
	//       COLLISION NODE        //
	/////////////////////////////////
	collision_EntityLTS = mSceneMgr->createEntity( "cube", "cube.mesh" );
	collision_EntityLTS->setCastShadows(true);
	objectCollisionNodeLTS = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("objectCollisionNodeLTS");
    objectCollisionNodeLTS->setPosition( Vector3(0,30,50) );
	objectCollisionNodeLTS->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	objectCollisionNodeLTS->attachObject( collision_EntityLTS );
	objectCollisionNodeLTS->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	objectCollisionNodeLTS->setVisible(false);
	mEntitiesLTS.push_back(collision_EntityLTS);

	/////////////////////////////////
	// COLLISION DETECTION NODE    //
	/////////////////////////////////
	forwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("forwardCollisionNodeLTS");
	backwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("backwardCollisionNodeLTS");
	leftCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("leftCollisionNodeLTS");
	rightCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("rightCollisionNodeLTS");
	mSceneNodesLTS.push_back(forwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(backwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(leftCollisionNodeLTS);
	mSceneNodesLTS.push_back(rightCollisionNodeLTS);

	/////////////////////////////////
	//           CAF NODE          //
	/////////////////////////////////
	object_Node1LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "OgreHead" );
	object_Node1LTS->scale(Vector3( 0.5f, 0.5f, 0.5f ));
	object_Node1LTS->setPosition(Vector3( -60.0f, 50.0f, 1670.0f ));
	object_Node1LTS->rotate(Vector3::UNIT_Y, Degree(180.0f));

	mSceneNodesLTS.push_back(object_Node1LTS);

	//////////////////////////////
	//        LIN MA NODE       //
	//////////////////////////////
	linMa = mSceneMgr->createEntity("LinMa", "linMaBody.mesh");
	linMaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LinMaNode");
	linMaNode->attachObject(linMa);
	linMaNode->setPosition(Vector3(706.0f,0.0f,1964.0f));
	linMaNode->rotate(Vector3::UNIT_Y,Degree(-90));
	mPlayerEntitiesLTS.push_back(linMa);
	mPlayerSceneNodesLTS.push_back(linMaNode);
	
	linMaSit = linMa->getAnimationState("restSit");
	linMaSit->setEnabled(true);
	linMaSit->setWeight(5);
	linMaSit->setLoop(true);

	//Sets AutoTracking on the NPCS//
	/*linMaNode->setAutoTracking(true,PlayerNodeLTS,Vector3::UNIT_Z);
	linMaNode->setFixedYawAxis(true);*/

	//////////////////////////////
	//       SANTOSO NODE       //
	//////////////////////////////
	santoso = mSceneMgr->createEntity("Santoso", "santosoModel.mesh");
	santosoNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SantosoNode");
	santosoNode->attachObject(santoso);
	santosoNode->setScale(Vector3(1.5,1.5,1.5));
	santosoNode->setPosition(Vector3(852.0f,0.0f,1349.0f));
	mPlayerEntitiesLTS.push_back(santoso);
	mPlayerSceneNodesLTS.push_back(santosoNode);

	////Sets AutoTracking on the NPCS//
	//santosoNode->setAutoTracking(true,PlayerNodeLTS,Vector3::UNIT_Z);
	//santosoNode->setFixedYawAxis(true);

	santoso_Idle = santoso->getAnimationState("rest");
	santoso_Idle->setWeight(5);
	santoso_Idle->setEnabled(true);
	santoso_Idle->setLoop(true);

	//////////////////////////////
	//       DANNYBOY NODE      //
	//////////////////////////////
	dannyBoy = mSceneMgr->createEntity("DannyBoy","VGD_Alone_char_dannyboy.mesh");
	dannyBoyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DannyBoyNode");
	dannyBoyNode->attachObject(dannyBoy);
	dannyBoyNode->setPosition(Vector3(331.0f,0.0f,691.0f));
	dannyBoyNode->rotate(Vector3::UNIT_Y,Degree(90));
	mPlayerEntitiesLTS.push_back(dannyBoy);
	mPlayerSceneNodesLTS.push_back(dannyBoyNode);

	////Sets AutoTracking on the NPCS//
	//dannyBoyNode->setAutoTracking(true,PlayerNodeLTS,Vector3::UNIT_Z);
	//dannyBoyNode->setFixedYawAxis(true);

	dannyBoy_Idle = dannyBoy->getAnimationState("rest");
	dannyBoy_Idle->setEnabled(true);
	dannyBoy_Idle->setLoop(true);

	//////////////////////////////
	//       JOHNNY 666 NODE    //
	//////////////////////////////
	//Trainer//
	trainerEnt = mSceneMgr->createEntity("Trainer", "johnnyModel.mesh");
	trainerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Trainer");
	trainerNode->attachObject(trainerEnt);
	trainerNode->setPosition(Vector3(555.0f, 0.0f, 690.0f));
	trainerNode->rotate(Vector3::UNIT_Y, Degree(180));
	//trainerNode->setVisible(false);
	mPlayerEntitiesLTS.push_back(trainerEnt);
	mPlayerSceneNodesLTS.push_back(trainerNode);

	//Sets AutoTracking on the NPCS//
	trainerNode->setAutoTracking(true,PlayerNodeLTS,Vector3::UNIT_Z);
	trainerNode->setFixedYawAxis(true);

	johnny666_Idle = trainerEnt->getAnimationState("rest");
	johnny666_Idle->setEnabled(true);
	johnny666_Idle->setLoop(true);

	//////////////////////////////
	//       DOOR NODES         //
	//////////////////////////////
	playerCellDoorCol = mSceneMgr->createEntity("Door_01Col", "DoorCollision.mesh");
	playerCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_01ColNode");
	playerCellDoorColNode->attachObject(playerCellDoorCol);
	playerCellDoorColNode->setVisible(false);
	playerCellDoorColNode->setPosition(Vector3(550.0f, 0.0f, 415.0f));

	mEntitiesLTS.push_back(playerCellDoorCol);
	mSceneNodesLTS.push_back(playerCellDoorColNode);

	//door//
	playerCellDoor = mSceneMgr->createEntity("Door_01", "CellDoor_Cells.mesh");
	playerCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door01_Node");
	playerCellDoorNode->setPosition(Vector3(523.0f, 0.0f, 415.0f));
	playerCellDoorNode->attachObject(playerCellDoor);

	mEntitiesLTS.push_back(playerCellDoor);
	mSceneNodesLTS.push_back(playerCellDoorNode);

	//col//
	rightOfPlayerCellCol = mSceneMgr->createEntity("Door_02Col", "DoorCollision.mesh");
	rightOfPlayerCellColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_02ColNode");
	rightOfPlayerCellColNode->attachObject(rightOfPlayerCellCol);
	rightOfPlayerCellColNode->setPosition(Vector3(440.0f,0.0f,412.5f));
	rightOfPlayerCellColNode->setVisible(false);

	mEntitiesLTS.push_back(rightOfPlayerCellCol);
	mSceneNodesLTS.push_back(rightOfPlayerCellColNode);

	//door//
	rightOfPlayerCell = mSceneMgr->createEntity("Door_02", "CellDoor_Cells.mesh");
	rightOfPlayerCellNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door02_Node");
	rightOfPlayerCellNode->setPosition(Vector3(468.0f,0.0f,412.5f));
	rightOfPlayerCellNode->rotate(Vector3().UNIT_Y,Degree(180));
	rightOfPlayerCellNode->attachObject(rightOfPlayerCell);

	mEntitiesLTS.push_back(rightOfPlayerCell);
	mSceneNodesLTS.push_back(rightOfPlayerCellNode);

	//johnny 666 celldoorCol//
	johnny666CellDoorCol = mSceneMgr->createEntity("Door_03Col", "DoorCollision.mesh");
	johnny666CellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_03ColNode");
	johnny666CellDoorColNode->attachObject(johnny666CellDoorCol);
	johnny666CellDoorColNode->setPosition(Vector3(550.0f,0.0f,522.0f));
	johnny666CellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(johnny666CellDoorCol);
	mSceneNodesLTS.push_back(johnny666CellDoorColNode);

	//johnny 666 celldoor//
	johnny666CellDoor = mSceneMgr->createEntity("Door_03", "CellDoor_Cells.mesh");
	johnny666CellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door03_Node");
	johnny666CellDoorNode->setPosition(Vector3(523.0f,0.0f,523.0f));
	johnny666CellDoorNode->attachObject(johnny666CellDoor);

	mEntitiesLTS.push_back(johnny666CellDoor);
	mSceneNodesLTS.push_back(johnny666CellDoorNode);
	
	//coll node//
	dannyBoyCellDoorCol = mSceneMgr->createEntity("Door_04Col", "DoorCollision.mesh");
	dannyBoyCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_04ColNode");
	dannyBoyCellDoorColNode->attachObject(dannyBoyCellDoorCol);
	dannyBoyCellDoorColNode->setPosition(Vector3(441.0f,0.0f,520.0f));
	dannyBoyCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(dannyBoyCellDoorCol);
	mSceneNodesLTS.push_back(dannyBoyCellDoorColNode);

	//dannyboys cell door//
	dannyBoyCellDoor = mSceneMgr->createEntity("Door_04", "CellDoor_Cells.mesh");
	dannyBoyCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door04_node");
	dannyBoyCellDoorNode->attachObject(dannyBoyCellDoor);
	dannyBoyCellDoorNode->setPosition(Vector3(468.0f,0.0f,520.5f));
	dannyBoyCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));

	mEntitiesLTS.push_back(dannyBoyCellDoor);
	mSceneNodesLTS.push_back(dannyBoyCellDoorNode);

	//jolly celll door col//
	jollyCellDoorCol = mSceneMgr->createEntity("Door_05Col", "DoorCollision.mesh");
	jollyCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_05ColNode");
	jollyCellDoorColNode->attachObject(jollyCellDoorCol);
	jollyCellDoorColNode->setPosition(945.0f,0.0f,407.0f);
	jollyCellDoorColNode->rotate(Vector3().UNIT_Y,Degree(180));
	jollyCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(jollyCellDoorCol);
	mSceneNodesLTS.push_back(jollyCellDoorColNode);

	//jollys cell door//
	jollyCellDoor = mSceneMgr->createEntity("Door_05", "CellDoor_Cells.mesh");
	jollyCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door05_Node");
	jollyCellDoorNode->attachObject(jollyCellDoor);
	jollyCellDoorNode->setPosition(972.0f,0.0f,412.5f);
	jollyCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));

	mEntitiesLTS.push_back(jollyCellDoor);
	mSceneNodesLTS.push_back(jollyCellDoorNode);

	//empty cell//
	emptyCellJollyDoorCol = mSceneMgr->createEntity("Door_06Col", "DoorCollision.mesh");
	emptyCellJollyDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_06ColNode");
	emptyCellJollyDoorColNode->attachObject(emptyCellJollyDoorCol);
	emptyCellJollyDoorColNode->setPosition(945.5f,0.0f,523.0f);
	emptyCellJollyDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyCellJollyDoorCol);
	mSceneNodesLTS.push_back(emptyCellJollyDoorColNode);

	//empty cell across from jollys//
	emptyCellJollyDoor = mSceneMgr->createEntity("Door_06", "CellDoor_Cells.mesh");
	emptyCellJollyDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door06_Node");
	emptyCellJollyDoorNode->attachObject(emptyCellJollyDoor);
	emptyCellJollyDoorNode->setPosition(972.0f,0.0f,520.5f);
	emptyCellJollyDoorNode->rotate(Vector3().UNIT_Y,Degree(180));

	mEntitiesLTS.push_back(emptyCellJollyDoor);
	mSceneNodesLTS.push_back(emptyCellJollyDoorNode);

	//chaos col//
	chaosCellDoorCol = mSceneMgr->createEntity("Door_07Col", "DoorCollision.mesh");
	chaosCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_07ColNode");
	chaosCellDoorColNode->attachObject(chaosCellDoorCol);
	chaosCellDoorColNode->setPosition(45.5f,0.0f,520.0f);
	chaosCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(chaosCellDoorCol);
	mSceneNodesLTS.push_back(chaosCellDoorColNode);

	//chaos cult cell door//
	chaosCellDoor = mSceneMgr->createEntity("Door_07", "CellDoor_Cells.mesh");
	chaosCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_07");
	chaosCellDoorNode->attachObject(chaosCellDoor);
	chaosCellDoorNode->setPosition(18.0f,0.0f,523.0f);

	mEntitiesLTS.push_back(chaosCellDoor);
	mSceneNodesLTS.push_back(chaosCellDoorNode);

	//empty col//
	emptyRightCornerDoorCol = mSceneMgr->createEntity("Door_08Col", "DoorCollision.mesh");
	emptyRightCornerDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_08ColNode");
	emptyRightCornerDoorColNode->attachObject(emptyRightCornerDoorCol);
	emptyRightCornerDoorColNode->setPosition(45.5f,0.0f,412.0f);
	emptyRightCornerDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyRightCornerDoorCol);
	mSceneNodesLTS.push_back(emptyRightCornerDoorColNode);

	//empty cell right corner//
	emptyRightCornerDoor = mSceneMgr->createEntity("Door_08", "CellDoor_Cells.mesh");
	emptyRightCornerDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door");
	emptyRightCornerDoorNode->attachObject(emptyRightCornerDoor);
	emptyRightCornerDoorNode->setPosition(18.0f,0.0f,415.0f);

	mEntitiesLTS.push_back(emptyRightCornerDoor);
	mSceneNodesLTS.push_back(emptyRightCornerDoorNode);

	//terrance col//
	terranceDougieCellDoorCol = mSceneMgr->createEntity("Door_09Col", "DoorCollision.mesh");
	terranceDougieCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_09ColNode");
	terranceDougieCellDoorColNode->attachObject(terranceDougieCellDoorCol);
	terranceDougieCellDoorColNode->setPosition(45.5f,0.0f,1455.0f);
	terranceDougieCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(terranceDougieCellDoorCol);
	mSceneNodesLTS.push_back(terranceDougieCellDoorColNode);

	//terrance celld door//
	terranceDougieCellDoor = mSceneMgr->createEntity("Door_09", "CellDoor_Cells.mesh");
	terranceDougieCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_09");
	terranceDougieCellDoorNode->attachObject(terranceDougieCellDoor);
	terranceDougieCellDoorNode->setPosition(18.0f,0.0f,1459.5f);
	
	mEntitiesLTS.push_back(terranceDougieCellDoor);
	mSceneNodesLTS.push_back(terranceDougieCellDoorNode);

	//col//
	emptyTerranceCellDoorCol = mSceneMgr->createEntity("Door_10Col", "DoorCollision.mesh");
	emptyTerranceCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_10ColNode");
	emptyTerranceCellDoorColNode->attachObject(emptyTerranceCellDoorCol);
	emptyTerranceCellDoorColNode->setPosition(441.0f,0.0f,1454.0f);
	emptyTerranceCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyTerranceCellDoorCol);
	mSceneNodesLTS.push_back(emptyTerranceCellDoorColNode);

	//empty cell next to terrance//
	emptyTerranceCellDoor = mSceneMgr->createEntity("Door_10", "CellDoor_Cells.mesh");
	emptyTerranceCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_10");
	emptyTerranceCellDoorNode->attachObject(emptyTerranceCellDoor);
	emptyTerranceCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));
	emptyTerranceCellDoorNode->setPosition(469.0f,0.0f,1457.0f);

	mEntitiesLTS.push_back(emptyTerranceCellDoor);
	mSceneNodesLTS.push_back(emptyTerranceCellDoorNode);

	//col//
	santosoCellDoorCol = mSceneMgr->createEntity("Door_11Col", "DoorCollision.mesh");
	santosoCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_11ColNode");
	santosoCellDoorColNode->attachObject(santosoCellDoorCol);
	santosoCellDoorColNode->setPosition(946.0f,0.0f,1460.0f);
	santosoCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(santosoCellDoorCol);
	mSceneNodesLTS.push_back(santosoCellDoorColNode);

	//santosos shop cell door node//
	santosoCellDoor = mSceneMgr->createEntity("Door_11", "CellDoor_Cells.mesh");
	santosoCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_11");
	santosoCellDoorNode->attachObject(santosoCellDoor);
	santosoCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));
	santosoCellDoorNode->setPosition(972.0f,0.0f,1457.0f);

	mEntitiesLTS.push_back(santosoCellDoor);
	mSceneNodesLTS.push_back(santosoCellDoorNode);

	//lin mas col//
	linMasDoorCol = mSceneMgr->createEntity("Door_12Col", "DoorCollision.mesh");
	linMasDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_12ColNode");
	linMasDoorColNode->attachObject(linMasDoorCol);
	linMasDoorColNode->setPosition(385.5f,0.0f,1647.0f);
	linMasDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(linMasDoorCol);
	mSceneNodesLTS.push_back(linMasDoorColNode);

	//lin mas office door//
	linMasDoor = mSceneMgr->createEntity("Door_12", "VGD_Alone_prop_OfficeDoor.mesh");
	linMasDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_12");
	linMasDoorNode->attachObject(linMasDoor);
	linMasDoorNode->setPosition(357.0f,0.0f,1647.0f);

	mEntitiesLTS.push_back(linMasDoor);
	mSceneNodesLTS.push_back(linMasDoorNode);

	//night timeNode//	
	nightTime = mSceneMgr->createEntity("NightNode","cube.mesh");
	nightTimeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NightTimeNode");
	nightTimeNode->attachObject(nightTime);
	nightTimeNode->setPosition(670.0f,0.0f,218.0f);
	nightTimeNode->setScale(0.5,0.5,0.5);
	nightTimeNode->setVisible(false);

	mEntitiesLTS.push_back(nightTime);
	mSceneNodesLTS.push_back(nightTimeNode);

	//reset variables
	//door pitch counter//
	door01Pitch = 0;
	door02Pitch = 0;
	door03Pitch = 0;
	door04Pitch = 0;
	door05Pitch = 0;
	door06Pitch = 0;
	door07Pitch = 0;
	door08Pitch = 0;
	door09Pitch = 0;
	door10Pitch = 0;
	door11Pitch = 0;
	door12Pitch = 0;

	//door variables//
	Door01_Open = false;
	Door02_Open = false;
	Door03_Open = false;
	Door04_Open = false;
	Door05_Open = false;
	Door06_Open = false;
	Door07_Open = false;
	Door08_Open = false;
	Door09_Open = false;
	Door10_Open = false;
	Door11_Open = false;
	Door12_Open = false;

	//reset variables
	pitchLTS = 0;

	audio_result = system->createStream("Sounds/Music/Cells.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	if(levelsMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}
	audio_result = system->createSound("Sounds/SFX/cell open 1.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cell_Door_Open);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/cell close 2.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cell_Door_Close);
	ERRCHECK(audio_result);
}


void LevelTestState::loadCharacter()
{
	TiXmlDocument loadDoc("CharacterEditor.xml");
	loadDoc.LoadFile();
	
	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");
	existingCharacter = atoi(element->Attribute("Exist"));
	name = element->Attribute("Name");
	skin = atoi(element->Attribute("Skin"));
	shirt = atoi(element->Attribute("Shirt"));
	hair = atoi(element->Attribute("Hair"));
}

void LevelTestState::loadInitialLevel( void )
{
	LevelLoaded = 0;

	// load level model
	cellsEntity = mSceneMgr->createEntity("Cells", "VGD_Alone_lvl_cells_night.mesh");
	cellsEntity->setCastShadows(false);
	cellsNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Cells" );
	cellsNode->setPosition(Vector3( 0.0f, 0.0f, 0.0f ));
    cellsNode->attachObject( cellsEntity );
	mEntitiesLTS.push_back(cellsEntity);
	mSceneNodesLTS.push_back(cellsNode);

	cellsCollisionEntity = mSceneMgr->createEntity("CellsCollision", "VGD_Alone_lvl_cells_Collision.mesh");
	cellsCollisionEntity->setCastShadows(false);
	cellsCollisionNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CellsCollision" );
	cellsCollisionNode->setPosition(Vector3( 0.0f, 0.0f, 0.0f ));
    cellsCollisionNode->attachObject( cellsCollisionEntity );
	cellsCollisionNode->setVisible(false);

	mEntitiesLTS.push_back(	cellsCollisionEntity );
	mSceneNodesLTS.push_back(cellsCollisionNode);

	

	ShirtEntityLTS = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirtNoTorso.mesh");
	ShirtlessEntityLTS = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtlessNoTorso.mesh");
	PlayerNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
	PlayerNodeLTS->setPosition(Vector3( 568.0f,0.0f,239.0f));
	PlayerNodeLTS->yaw(Ogre::Degree(0.0f));
    mCamera->setOrientation(Quaternion(Degree(90.0f), Vector3::UNIT_Y));

	mPlayerEntitiesLTS.push_back(ShirtEntityLTS);
	mPlayerEntitiesLTS.push_back(ShirtlessEntityLTS);
	mPlayerSceneNodesLTS.push_back(PlayerNodeLTS);

	loadLevelCharacter();

	//rotate camera and character to face proper direction
	
	
	
	mainLightNode = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("mainLightNode");
	mainLightNode->setPosition( Vector3(2,70,0) );
	mainLightNode->attachObject( mainLight1 );

	//mPlayerSceneNodesLTS.push_back(mainLightNode);

	//set animation state
	if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rest" );}
	else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rest" );}
    robot_anim1LTS->setLoop( true );
    robot_anim1LTS->setEnabled( true );

	//Create object pickup node
	collision_EntityLTS = mSceneMgr->createEntity( "cube", "cube.mesh" );
	collision_EntityLTS->setCastShadows(true);
	objectCollisionNodeLTS = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("objectCollisionNodeLTS");
    objectCollisionNodeLTS->setPosition( Vector3(0,30,50) );
	objectCollisionNodeLTS->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	objectCollisionNodeLTS->attachObject( collision_EntityLTS );
	objectCollisionNodeLTS->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	objectCollisionNodeLTS->setVisible(false);
	mEntitiesLTS.push_back(collision_EntityLTS);

	//collision detection nodes//
	forwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("forwardCollisionNodeLTS");
	backwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("backwardCollisionNodeLTS");
	leftCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("leftCollisionNodeLTS");
	rightCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("rightCollisionNodeLTS");
	mSceneNodesLTS.push_back(forwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(backwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(leftCollisionNodeLTS);
	mSceneNodesLTS.push_back(rightCollisionNodeLTS);

	//node to transfer to the cafeteria//
	object_Node1LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "OgreHead" );
	object_Node1LTS->scale(Vector3( 0.5f, 0.5f, 0.5f ));
	object_Node1LTS->setPosition(Vector3( -60.0f, 50.0f, 1670.0f ));
	object_Node1LTS->rotate(Vector3::UNIT_Y, Degree(180.0f));

	mSceneNodesLTS.push_back(object_Node1LTS);
	
	//npc nodes//
	//lin ma//
	linMa = mSceneMgr->createEntity("LinMa", "linMaBody.mesh");
	linMaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LinMaNode");
	linMaNode->attachObject(linMa);
	linMaNode->setPosition(Vector3(692.0f,0.0f,290.0f));
	//linMaNode->setOrientation(Quaternion(Degree(-90.0f), Vector3::UNIT_Y));
	mPlayerEntitiesLTS.push_back(linMa);
	mPlayerSceneNodesLTS.push_back(linMaNode);

	//Sets AutoTracking on the NPCS//
	linMaNode->setAutoTracking(true,PlayerNodeLTS,Vector3::UNIT_Z);
	linMaNode->setFixedYawAxis(true);
	
	//ANIMATION STATES LIN MA//
	batonGrab = linMa->getAnimationState("batonGrab");
	batonHit = linMa->getAnimationState("batonHit");
	batonGrab->setEnabled(false);
	batonGrab->setLoop(false);
	batonHit->setEnabled(false);
	batonHit->setLoop(false);

	//players door node + collision////
	playerCellDoorCol = mSceneMgr->createEntity("Door_01Col", "DoorCollision.mesh");
	playerCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_01ColNode");
	playerCellDoorColNode->attachObject(playerCellDoorCol);
	playerCellDoorColNode->setVisible(false);
	playerCellDoorColNode->setPosition(Vector3(550.0f, 0.0f, 410.0f));

	mEntitiesLTS.push_back(playerCellDoorCol);
	mSceneNodesLTS.push_back(playerCellDoorColNode);

	//door//
	playerCellDoor = mSceneMgr->createEntity("Door_01", "CellDoor_Cells.mesh");
	playerCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door01_Node");
	playerCellDoorNode->setPosition(Vector3(522.0f, 0.0f, 410.0f));
	playerCellDoorNode->attachObject(playerCellDoor);

	mEntitiesLTS.push_back(playerCellDoor);
	mSceneNodesLTS.push_back(playerCellDoorNode);

	//col//
	rightOfPlayerCellCol = mSceneMgr->createEntity("Door_02Col", "DoorCollision.mesh");
	rightOfPlayerCellColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_02ColNode");
	rightOfPlayerCellColNode->attachObject(rightOfPlayerCellCol);
	rightOfPlayerCellColNode->setPosition(Vector3(440.0f,0.0f,410.0f));
	rightOfPlayerCellColNode->setVisible(false);

	mEntitiesLTS.push_back(rightOfPlayerCellCol);
	mSceneNodesLTS.push_back(rightOfPlayerCellColNode);

	//door//
	rightOfPlayerCell = mSceneMgr->createEntity("Door_02", "CellDoor_Cells.mesh");
	rightOfPlayerCellNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door02_Node");
	rightOfPlayerCellNode->setPosition(Vector3(468.0f,0.0f,410.0f));
	rightOfPlayerCellNode->rotate(Vector3().UNIT_Y,Degree(180));
	rightOfPlayerCellNode->attachObject(rightOfPlayerCell);

	mEntitiesLTS.push_back(rightOfPlayerCell);
	mSceneNodesLTS.push_back(rightOfPlayerCellNode);

	//johnny 666 celldoorCol//
	johnny666CellDoorCol = mSceneMgr->createEntity("Door_03Col", "DoorCollision.mesh");
	johnny666CellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_03ColNode");
	johnny666CellDoorColNode->attachObject(johnny666CellDoorCol);
	johnny666CellDoorColNode->setPosition(Vector3(550.0f,0.0f,522.0f));
	johnny666CellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(johnny666CellDoorCol);
	mSceneNodesLTS.push_back(johnny666CellDoorColNode);

	//johnny 666 celldoor//
	johnny666CellDoor = mSceneMgr->createEntity("Door_03", "CellDoor_Cells.mesh");
	johnny666CellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door03_Node");
	johnny666CellDoorNode->setPosition(Vector3(523.0f,0.0f,522.0f));
	johnny666CellDoorNode->attachObject(johnny666CellDoor);

	mEntitiesLTS.push_back(johnny666CellDoor);
	mSceneNodesLTS.push_back(johnny666CellDoorNode);
	
	//coll node//
	dannyBoyCellDoorCol = mSceneMgr->createEntity("Door_04Col", "DoorCollision.mesh");
	dannyBoyCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_04ColNode");
	dannyBoyCellDoorColNode->attachObject(dannyBoyCellDoorCol);
	dannyBoyCellDoorColNode->setPosition(Vector3(441.0f,0.0f,520.0f));
	dannyBoyCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(dannyBoyCellDoorCol);
	mSceneNodesLTS.push_back(dannyBoyCellDoorColNode);

	//dannyboys cell door//
	dannyBoyCellDoor = mSceneMgr->createEntity("Door_04", "CellDoor_Cells.mesh");
	dannyBoyCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door04_node");
	dannyBoyCellDoorNode->attachObject(dannyBoyCellDoor);
	dannyBoyCellDoorNode->setPosition(Vector3(467.0f,0.0f,520.0f));
	dannyBoyCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));

	mEntitiesLTS.push_back(dannyBoyCellDoor);
	mSceneNodesLTS.push_back(dannyBoyCellDoorNode);

	//jolly celll door col//
	jollyCellDoorCol = mSceneMgr->createEntity("Door_05Col", "DoorCollision.mesh");
	jollyCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_05ColNode");
	jollyCellDoorColNode->attachObject(jollyCellDoorCol);
	jollyCellDoorColNode->setPosition(945.0f,0.0f,410.0f);
	jollyCellDoorColNode->rotate(Vector3().UNIT_Y,Degree(180));
	jollyCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(jollyCellDoorCol);
	mSceneNodesLTS.push_back(jollyCellDoorColNode);

	//jollys cell door//
	jollyCellDoor = mSceneMgr->createEntity("Door_05", "CellDoor_Cells.mesh");
	jollyCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door05_Node");
	jollyCellDoorNode->attachObject(jollyCellDoor);
	jollyCellDoorNode->setPosition(972.0f,0.0f,410.0f);
	jollyCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));
	jollyCellDoorNode->setVisible(false);
	mEntitiesLTS.push_back(jollyCellDoor);
	mSceneNodesLTS.push_back(jollyCellDoorNode);

	//empty cell//
	emptyCellJollyDoorCol = mSceneMgr->createEntity("Door_06Col", "DoorCollision.mesh");
	emptyCellJollyDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_06ColNode");
	emptyCellJollyDoorColNode->attachObject(emptyCellJollyDoorCol);
	emptyCellJollyDoorColNode->setPosition(945.5f,0.0f,523.0f);
	emptyCellJollyDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyCellJollyDoorCol);
	mSceneNodesLTS.push_back(emptyCellJollyDoorColNode);

	//empty cell across from jollys//
	emptyCellJollyDoor = mSceneMgr->createEntity("Door_06", "CellDoor_Cells.mesh");
	emptyCellJollyDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door06_Node");
	emptyCellJollyDoorNode->attachObject(emptyCellJollyDoor);
	emptyCellJollyDoorNode->setPosition(972.0f,0.0f,523.0f);
	emptyCellJollyDoorNode->rotate(Vector3().UNIT_Y,Degree(180));
	emptyCellJollyDoorNode->setVisible(false);
	mEntitiesLTS.push_back(emptyCellJollyDoor);
	mSceneNodesLTS.push_back(emptyCellJollyDoorNode);

	//chaos col//
	chaosCellDoorCol = mSceneMgr->createEntity("Door_07Col", "DoorCollision.mesh");
	chaosCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_07ColNode");
	chaosCellDoorColNode->attachObject(chaosCellDoorCol);
	chaosCellDoorColNode->setPosition(45.5f,0.0f,520.0f);
	chaosCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(chaosCellDoorCol);
	mSceneNodesLTS.push_back(chaosCellDoorColNode);

	//chaos cult cell door//
	chaosCellDoor = mSceneMgr->createEntity("Door_07", "CellDoor_Cells.mesh");
	chaosCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_07");
	chaosCellDoorNode->attachObject(chaosCellDoor);
	chaosCellDoorNode->setPosition(18.0f,0.0f,520.0f);
	chaosCellDoorNode->setVisible(false);

	mEntitiesLTS.push_back(chaosCellDoor);
	mSceneNodesLTS.push_back(chaosCellDoorNode);

	//empty col//
	emptyRightCornerDoorCol = mSceneMgr->createEntity("Door_08Col", "DoorCollision.mesh");
	emptyRightCornerDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_08ColNode");
	emptyRightCornerDoorColNode->attachObject(emptyRightCornerDoorCol);
	emptyRightCornerDoorColNode->setPosition(45.5f,0.0f,410.0f);
	emptyRightCornerDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyRightCornerDoorCol);
	mSceneNodesLTS.push_back(emptyRightCornerDoorColNode);

	//empty cell right corner//
	emptyRightCornerDoor = mSceneMgr->createEntity("Door_08", "CellDoor_Cells.mesh");
	emptyRightCornerDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door");
	emptyRightCornerDoorNode->attachObject(emptyRightCornerDoor);
	emptyRightCornerDoorNode->setPosition(18.0f,0.0f,410.0f);
	emptyRightCornerDoorNode->setVisible(false);

	mEntitiesLTS.push_back(emptyRightCornerDoor);
	mSceneNodesLTS.push_back(emptyRightCornerDoorNode);

	//terrance col//
	terranceDougieCellDoorCol = mSceneMgr->createEntity("Door_09Col", "DoorCollision.mesh");
	terranceDougieCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_09ColNode");
	terranceDougieCellDoorColNode->attachObject(terranceDougieCellDoorCol);
	terranceDougieCellDoorColNode->setPosition(45.5f,0.0f,1455.0f);
	terranceDougieCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(terranceDougieCellDoorCol);
	mSceneNodesLTS.push_back(terranceDougieCellDoorColNode);

	//terrance celld door//
	terranceDougieCellDoor = mSceneMgr->createEntity("Door_09", "CellDoor_Cells.mesh");
	terranceDougieCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_09");
	terranceDougieCellDoorNode->attachObject(terranceDougieCellDoor);
	terranceDougieCellDoorNode->setPosition(18.0f,0.0f,1455.0f);
	terranceDougieCellDoorNode->setVisible(false);
	
	mEntitiesLTS.push_back(terranceDougieCellDoor);
	mSceneNodesLTS.push_back(terranceDougieCellDoorNode);

	//col//
	emptyTerranceCellDoorCol = mSceneMgr->createEntity("Door_10Col", "DoorCollision.mesh");
	emptyTerranceCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_10ColNode");
	emptyTerranceCellDoorColNode->attachObject(emptyTerranceCellDoorCol);
	emptyTerranceCellDoorColNode->setPosition(441.0f,0.0f,1455.0f);
	emptyTerranceCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyTerranceCellDoorCol);
	mSceneNodesLTS.push_back(emptyTerranceCellDoorColNode);

	//empty cell next to terrance//
	emptyTerranceCellDoor = mSceneMgr->createEntity("Door_10", "CellDoor_Cells.mesh");
	emptyTerranceCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_10");
	emptyTerranceCellDoorNode->attachObject(emptyTerranceCellDoor);
	emptyTerranceCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));
	emptyTerranceCellDoorNode->setPosition(469.0f,0.0f,1455.0f);
	emptyTerranceCellDoorNode->setVisible(false);

	mEntitiesLTS.push_back(emptyTerranceCellDoor);
	mSceneNodesLTS.push_back(emptyTerranceCellDoorNode);

	//col//
	santosoCellDoorCol = mSceneMgr->createEntity("Door_11Col", "DoorCollision.mesh");
	santosoCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_11ColNode");
	santosoCellDoorColNode->attachObject(santosoCellDoorCol);
	santosoCellDoorColNode->setPosition(946.0f,0.0f,1455.0f);
	santosoCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(santosoCellDoorCol);
	mSceneNodesLTS.push_back(santosoCellDoorColNode);

	//santosos shop cell door node//
	santosoCellDoor = mSceneMgr->createEntity("Door_11", "CellDoor_Cells.mesh");
	santosoCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_11");
	santosoCellDoorNode->attachObject(santosoCellDoor);
	santosoCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));
	santosoCellDoorNode->setPosition(972.0f,0.0f,1455.0f);
	santosoCellDoorNode->setVisible(false);

	mEntitiesLTS.push_back(santosoCellDoor);
	mSceneNodesLTS.push_back(santosoCellDoorNode);

	//lin mas col//
	linMasDoorCol = mSceneMgr->createEntity("Door_12Col", "DoorCollision.mesh");
	linMasDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_12ColNode");
	linMasDoorColNode->attachObject(linMasDoorCol);
	linMasDoorColNode->setPosition(385.5f,0.0f,1647.0f);
	linMasDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(linMasDoorCol);
	mSceneNodesLTS.push_back(linMasDoorColNode);

	////lin mas office door//
	linMasDoor = mSceneMgr->createEntity("Door_12", "CellDoor_Office.mesh");
	linMasDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_12");
	linMasDoorNode->attachObject(linMasDoor);
	linMasDoorNode->setPosition(357.0f,0.0f,1647.0f);
	linMasDoorNode->setVisible(false);

	mEntitiesLTS.push_back(linMasDoor);
	mSceneNodesLTS.push_back(linMasDoorNode);

	//reset variables
	//door pitch counter//
	door01Pitch = 0;
	door02Pitch = 0;
	door03Pitch = 0;
	door04Pitch = 0;
	door05Pitch = 0;
	door06Pitch = 0;
	door07Pitch = 0;
	door08Pitch = 0;
	door09Pitch = 0;
	door10Pitch = 0;
	door11Pitch = 0;
	door12Pitch = 0;

	//door variables//
	Door01_Open = false;
	Door02_Open = false;
	Door03_Open = false;
	Door04_Open = false;
	Door05_Open = false;
	Door06_Open = false;
	Door07_Open = false;
	Door08_Open = false;
	Door09_Open = false;
	Door10_Open = false;
	Door11_Open = false;
	Door12_Open = false;

	//reset variables
	pitchLTS = 0;

	//rotate camera and character to face proper direction
	mCamera->setOrientation(Quaternion(Degree(-180.0f), Vector3::UNIT_Y));
	PlayerNodeLTS->yaw(Radian(0));

	audio_result = system->createStream("Sounds/Music/Cells.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	if(levelsMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}
	audio_result = system->createSound("Sounds/SFX/cell open 1.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cell_Door_Open);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/cell close 2.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &cell_Door_Close);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Impact/Impact2.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &baton_Smack);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Heartbeat Slow Loop.wav", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
	ERRCHECK(audio_result);

	if(levelsSFXSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
		ERRCHECK(audio_result);
	}
	changeInHealth = true;	
}
void LevelTestState::loadCellsNight( void )
{
	LevelLoaded = 4;

	audio_result = system->createSound("Sounds/SFX/Heartbeat Slow Loop.wav", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
	ERRCHECK(audio_result);

	if(levelsSFXSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
		ERRCHECK(audio_result);
	}

	changeInHealth = true;	

	////////////////////
	//  CAMERA MODELS //
	////////////////////

	camera1Entity = mSceneMgr->createEntity("Camera 1", "VGD_Alone_prop_camera.mesh");
	camera1Entity->setCastShadows(false);
	camera1Node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Camera 1" );
	camera1Node->setPosition(Vector3( -60.0f, 190.0f, 470.0f ));
	camera1Node->setOrientation(Quaternion(Degree(45.0f), Vector3::UNIT_Y));
    camera1Node->attachObject( camera1Entity );
	mEntitiesLTS.push_back(camera1Entity);
	mSceneNodesLTS.push_back(camera1Node);

	camera2Entity = mSceneMgr->createEntity("Camera 2", "VGD_Alone_prop_camera.mesh");
	camera2Entity->setCastShadows(false);
	camera2Node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Camera 2" );
	camera2Node->setPosition(Vector3( -60.0f, 190.0f, 1510.0f ));
	camera2Node->setOrientation(Quaternion(Degree(135.0f), Vector3::UNIT_Y));
    camera2Node->attachObject( camera2Entity );
	mEntitiesLTS.push_back(camera2Entity);
	mSceneNodesLTS.push_back(camera2Node);

	camera3Entity = mSceneMgr->createEntity("Camera 3", "VGD_Alone_prop_camera.mesh");
	camera3Entity->setCastShadows(false);
	camera3Node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Camera 3" );
	camera3Node->setPosition(Vector3( 1050.0f, 190.0f, 1510.0f ));
	camera3Node->setOrientation(Quaternion(Degree(225.0f), Vector3::UNIT_Y));
    camera3Node->attachObject( camera3Entity );
	mEntitiesLTS.push_back(camera3Entity);
	mSceneNodesLTS.push_back(camera3Node);

	camera4Entity = mSceneMgr->createEntity("Camera 4", "VGD_Alone_prop_camera.mesh");
	camera4Entity->setCastShadows(false);
	camera4Node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Camera 4" );
	camera4Node->setPosition(Vector3( 1050.0f, 190.0f, 470.0f ));
	camera4Node->setOrientation(Quaternion(Degree(315.0f), Vector3::UNIT_Y));
    camera4Node->attachObject( camera4Entity );
	mEntitiesLTS.push_back(camera4Entity);
	mSceneNodesLTS.push_back(camera4Node);
	
	cellsEntity = mSceneMgr->createEntity("Cells", "VGD_Alone_lvl_cells_night.mesh");
	cellsEntity->setCastShadows(false);
	cellsNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Cells" );
	cellsNode->setPosition(Vector3( 0.0f, 0.0f, 0.0f ));
	cellsNode->attachObject( cellsEntity );
	mEntitiesLTS.push_back(cellsEntity);
	mSceneNodesLTS.push_back(cellsNode);

	cellsCollisionEntity = mSceneMgr->createEntity("CellsCollision", "VGD_Alone_lvl_cells_Collision.mesh");
	cellsCollisionEntity->setCastShadows(false);
	cellsCollisionNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CellsCollision" );
	cellsCollisionNode->setPosition(Vector3( 0.0f, 0.0f, 0.0f ));
	cellsCollisionNode->attachObject( cellsCollisionEntity );
	cellsCollisionNode->setVisible(false);

	mEntitiesLTS.push_back(	cellsCollisionEntity );
	mSceneNodesLTS.push_back(cellsCollisionNode);

	detectNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode1" );
	detectNode1->setPosition(Vector3( 1050.0f, 0.0f, 580.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode1);

	detectNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode2" );
	detectNode2->setPosition(Vector3( -60.0f, 0.0f, 580.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode2);

	detectNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode3" );
	detectNode3->setPosition(Vector3( 1050.0f, 0.0f, 1410.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode3);

	detectNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode4" );
	detectNode4->setPosition(Vector3( -60.0f, 0.0f, 1410.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode4);

	detectNode5 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode5" );
	detectNode5->setPosition(Vector3( 540.0f, 0.0f, 475.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode5);

	detectNode6 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "detectNode6" );
	detectNode6->setPosition(Vector3( 430.0f, 0.0f, 475.0f ));
	mLoadCharacterSceneNodesLTS.push_back(detectNode6);

	ShirtEntityLTS = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirtNoTorso.mesh");
	ShirtlessEntityLTS = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtlessNoTorso.mesh");
	PlayerNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
	PlayerNodeLTS->setPosition(Ogre::Vector3(576.0f, 1.0f, 258.0f));
	PlayerNodeLTS->yaw(Degree(0.0f));
	mCamera->setOrientation(Quaternion(Degree(180.0f), Vector3::UNIT_Y));	  
  
	mPlayerEntitiesLTS.push_back(ShirtEntityLTS);
	mPlayerEntitiesLTS.push_back(ShirtlessEntityLTS);
	mPlayerSceneNodesLTS.push_back(PlayerNodeLTS);

	loadLevelCharacter();


	

	//set animation state
	if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rest" );}
	else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rest" );}
	robot_anim1LTS->setLoop( true );
	robot_anim1LTS->setEnabled( true );

	mainLightNode = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("mainLightNode");
	mainLightNode->setPosition( Vector3(2,70,0) );
	mainLightNode->attachObject( mainLight1 );

	//////////////////////////////
	//      CIGARETTES NODE     //
	//////////////////////////////
	emptyCellRightOfPlayerCigarettes = mSceneMgr->createEntity("EmptyCellCigs", "VGD_Alone_Prop_Cigeratte.mesh");
	emptyCellRightOfPlayerCigarettesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("EmptyCellCigsNode");
	emptyCellRightOfPlayerCigarettesNode->attachObject(emptyCellRightOfPlayerCigarettes);
	emptyCellRightOfPlayerCigarettesNode->setPosition(454.0f,50.0f,218.0f);
	//empty cell cigarettes pushed on//
	mItemsLTS.push_back(emptyCellRightOfPlayerCigarettes);
	mItemsNodeLTS.push_back(emptyCellRightOfPlayerCigarettesNode);
	

	if(itemDannySteroids == false)
	{
		//////////////////////////////
		//      DANNYBOY STEROIDS   //
		//////////////////////////////
		dannyBoySteroids = mSceneMgr->createEntity("DannyBoySteroids", "VGD_Alone_Prop_Steroid_Syringe.mesh");
		dannyBoySteroidsNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DannyBoyCellNode");
		dannyBoySteroidsNode->attachObject(dannyBoySteroids);
		dannyBoySteroidsNode->setPosition(329.0f,35.0f,734.0f);
		dannyBoySteroidsNode->setScale(2.0f,2.0f,2.0f);
		//danny boy cell node//
		mItemsLTS.push_back(dannyBoySteroids);
		mItemsNodeLTS.push_back(dannyBoySteroidsNode);
	}
	
	if(itemEscapePlans== false)
	{
		escapePlans = mSceneMgr->createEntity("EscapePlans", "VGD_Alone_Prop_EscapePlans.mesh");
		escapePlansNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("EscapePlansNodes");
		escapePlansNode->attachObject(escapePlans);
		escapePlansNode->setPosition(329.0f,35.0f,734.0f);
		
		mItemsLTS.push_back(escapePlans);
		mItemsNodeLTS.push_back(escapePlansNode);
	}

	if(itemSantosoSteroids == false)
	{
		//////////////////////////////
		//    PAINKILLER SANTOSO    //
		//////////////////////////////
		santosoPainKiller = mSceneMgr->createEntity("SantosoPainKiller", "VGD_Alone_Prop_Steroid_Syringe.mesh");
		santosoPainKillerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SantosoPainKillerNode");
		santosoPainKillerNode->attachObject(santosoPainKiller);
		santosoPainKillerNode->setPosition(768.0f,33.0f,1340.0f);
		santosoPainKillerNode->setScale(2.0f,2.0f,2.0f);
		//push the painkiller nodes//
		mItemsLTS.push_back(santosoPainKiller);
		mItemsNodeLTS.push_back(santosoPainKillerNode);
	}

	if(itemLinMaMoney == false)
	{
		///////////////////////////////
		//       LIN MAS MONEY       //
		///////////////////////////////
		linMasMoney = mSceneMgr->createEntity("LinMasMoney", "VGD_Alone_prop_Money_Stack.mesh");
		linMasMoneyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LinMasMoneyNode");
		linMasMoneyNode->attachObject(linMasMoney);
		linMasMoneyNode->setPosition(641.0f,45.0f,1759.0f);
		linMasMoneyNode->setScale(3.0f,3.0f,3.0f);
		//push the nodes on//
		mItemsLTS.push_back(linMasMoney);
		mItemsNodeLTS.push_back(linMasMoneyNode);
	}


	if(itemSoap == false)
	{
		//////////////////////////////
		//           SOAP           //
		//////////////////////////////
		linMasSoap = mSceneMgr->createEntity("LinMasSoapNode","VGD_Alone_Prop_Soap_Mold.mesh");
		linMasSoapNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("linMasSoapNode");
		linMasSoapNode->attachObject(linMasSoap);
		linMasSoapNode->setPosition(691.0f,25.0f,1682.0f);
		linMasSoapNode->setScale(1.0f,1.0f,1.0f);
		//push the soap on the stack//
		mItemsLTS.push_back(linMasSoap);
		mItemsNodeLTS.push_back(linMasSoapNode);
	}

	cellBlockKey = mSceneMgr->createEntity("CellBlockKey", "VGD_Alone_Prop_Office_Key.mesh");
	cellBlockKeyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CellBLockKeyNode");
	cellBlockKeyNode->attachObject(cellBlockKey);
	cellBlockKeyNode->setPosition(Vector3(660.0f,40.0f,1944.0f));
	//cell block key//
	mItemsLTS.push_back(cellBlockKey);
	mItemsNodeLTS.push_back(cellBlockKeyNode);


	//////////////////////////////
	//       GUARD NODE         //
	//////////////////////////////
	guardNPC = mSceneMgr->createEntity("Guard", "guardModel.mesh");
	guardNPCNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("GuardNode");
	guardNPCNode->attachObject(guardNPC);
	guardNPCNode->setPosition(Vector3(1050.0f,0.0f,1505.0f));
	
	mPlayerEntitiesLTS.push_back(guardNPC);
	mPlayerSceneNodesLTS.push_back(guardNPCNode);

	guardWalk = guardNPC->getAnimationState("run");

	guardWalk->setEnabled(true);
	guardWalk->setLoop(true);


	currentGuardNode = 1;

	////////////////////////////////
	//        NPC WALK NODES      //
	////////////////////////////////
	cellsWalkNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("cellsWalkNode1");
	cellsWalkNode1->setPosition(Vector3(1050.0f,0.0f,1505.0f));
	
	cellsWalkNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("cellsWalkNode2");
	cellsWalkNode2->setPosition(Vector3(1050.0f,0.0f,460.0f));

	cellsWalkNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("cellsWalkNode3");
	cellsWalkNode3->setPosition(Vector3(-50.0f,0.0f,460.0f));

	cellsWalkNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("cellsWalkNode4");
	cellsWalkNode4->setPosition(Vector3(-50.0f,0.0f,1505.0f));

	mLoadCharacterSceneNodesLTS.push_back(cellsWalkNode1);
	mLoadCharacterSceneNodesLTS.push_back(cellsWalkNode2);
	mLoadCharacterSceneNodesLTS.push_back(cellsWalkNode3);
	mLoadCharacterSceneNodesLTS.push_back(cellsWalkNode4);


	//jolly//
	jolly = mSceneMgr->createEntity("Jolly", "johnnyModel.mesh");
	jollyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("JollyNode");
	jollyNode->attachObject(jolly);
	jollyNode->setPosition(Vector3(837.0f, 0.0f, 281.0f));
	mPlayerEntitiesLTS.push_back(jolly);
	mPlayerSceneNodesLTS.push_back(jollyNode);

	//Terrance and dougie//
	terranceDougie = mSceneMgr->createEntity("Terrance","VGD_Alone_char_otter.mesh");
	terranceDougieNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("TerranceNode");
	terranceDougieNode->attachObject(terranceDougie);
	terranceDougieNode->setPosition(Vector3(-1494.0f,0.0f,725.0f));
	mPlayerEntitiesLTS.push_back(terranceDougie);
	mPlayerSceneNodesLTS.push_back(terranceDougieNode);

	if(factionedJolly == true)
	{
		jollyTalkAtNight = true;
		jollyNode->setPosition(Vector3(607.0f, 0.0f, 320.0f));
	}

	if(factionedGemini == true)
	{
		geminiTalkAtNight = true;
		terranceDougieNode->setPosition(Vector3(607.0f, 0.0f, 320.0f));
	}

	//santoso//
	santoso = mSceneMgr->createEntity("Santoso", "VGD_Alone_char_santosa.mesh");
	santosoNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SantosoNode");
	santosoNode->attachObject(santoso);
	santosoNode->setPosition(Vector3(836.0f,0.0f,1342.0f));
	mPlayerEntitiesLTS.push_back(santoso);
	mPlayerSceneNodesLTS.push_back(santosoNode);

	//micheal//
	micheal = mSceneMgr->createEntity("Micheal", "VGD_Alone_char_michael.mesh");
	michealNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MichealNode");
	michealNode->attachObject(micheal);
	michealNode->setPosition(Vector3(111.0f,0.0f,672.0f));
	michealNode->rotate(Vector3::UNIT_Y,Degree(180));
	mPlayerEntitiesLTS.push_back(micheal);
	mPlayerSceneNodesLTS.push_back(michealNode);


	//danny boy//
	dannyBoy = mSceneMgr->createEntity("DannyBoy","VGD_Alone_char_dannyboy.mesh");
	dannyBoyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DannyBoyNode");
	dannyBoyNode->attachObject(dannyBoy);
	dannyBoyNode->setPosition(Vector3(346.0f,0.0f,669.0f));
	dannyBoyNode->rotate(Vector3::UNIT_Y,Degree(180));
	mPlayerEntitiesLTS.push_back(dannyBoy);
	mPlayerSceneNodesLTS.push_back(dannyBoyNode);

	//night timeNode//	
	nightTime = mSceneMgr->createEntity("NightNode","cube.mesh");
	nightTimeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NightTimeNode");
	nightTimeNode->attachObject(nightTime);
	nightTimeNode->setPosition(670.0f,0.0f,218.0f);
	nightTimeNode->setScale(0.5,0.5,0.5);
	nightTimeNode->setVisible(false);

	mEntitiesLTS.push_back(nightTime);
	mSceneNodesLTS.push_back(nightTimeNode);

	//Create object pickup node
	collision_EntityLTS = mSceneMgr->createEntity( "cube", "cube.mesh" );
	collision_EntityLTS->setCastShadows(true);
	objectCollisionNodeLTS = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("objectCollisionNodeLTS");
	objectCollisionNodeLTS->setPosition( Vector3(0,30,50) );
	objectCollisionNodeLTS->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	objectCollisionNodeLTS->attachObject( collision_EntityLTS );
	objectCollisionNodeLTS->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	objectCollisionNodeLTS->setVisible(false);
	mEntitiesLTS.push_back(collision_EntityLTS);

	//collision detection nodes//
	forwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("forwardCollisionNodeLTS");
	backwardCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("backwardCollisionNodeLTS");
	leftCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("leftCollisionNodeLTS");
	rightCollisionNodeLTS = mSceneMgr->getRootSceneNode()->createChildSceneNode("rightCollisionNodeLTS");
	mSceneNodesLTS.push_back(forwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(backwardCollisionNodeLTS);
	mSceneNodesLTS.push_back(leftCollisionNodeLTS);
	mSceneNodesLTS.push_back(rightCollisionNodeLTS);

	//node to transfer to the cafeteria//
	object_Node1LTS = mSceneMgr->getRootSceneNode()->createChildSceneNode( "OgreHead" );
	object_Node1LTS->scale(Vector3( 0.5f, 0.5f, 0.5f ));
	object_Node1LTS->setPosition(Vector3( -60.0f, 50.0f, 1670.0f ));
	object_Node1LTS->rotate(Vector3::UNIT_Y, Degree(180.0f));

	mSceneNodesLTS.push_back(object_Node1LTS);

	//////////////////////////////
	//       DOOR NODES         //
	//////////////////////////////
	playerCellDoorCol = mSceneMgr->createEntity("Door_01Col", "DoorCollision.mesh");
	playerCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_01ColNode");
	playerCellDoorColNode->attachObject(playerCellDoorCol);
	playerCellDoorColNode->setVisible(false);
	playerCellDoorColNode->setPosition(Vector3(550.0f, 0.0f, 415.0f));

	mEntitiesLTS.push_back(playerCellDoorCol);
	mSceneNodesLTS.push_back(playerCellDoorColNode);

	//door//
	playerCellDoor = mSceneMgr->createEntity("Door_01", "CellDoor_Cells.mesh");
	playerCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door01_Node");
	playerCellDoorNode->setPosition(Vector3(523.0f, 0.0f, 415.0f));
	playerCellDoorNode->attachObject(playerCellDoor);

	mEntitiesLTS.push_back(playerCellDoor);
	mSceneNodesLTS.push_back(playerCellDoorNode);

	//col//
	rightOfPlayerCellCol = mSceneMgr->createEntity("Door_02Col", "DoorCollision.mesh");
	rightOfPlayerCellColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_02ColNode");
	rightOfPlayerCellColNode->attachObject(rightOfPlayerCellCol);
	rightOfPlayerCellColNode->setPosition(Vector3(440.0f,0.0f,412.5f));
	rightOfPlayerCellColNode->setVisible(false);

	mEntitiesLTS.push_back(rightOfPlayerCellCol);
	mSceneNodesLTS.push_back(rightOfPlayerCellColNode);

	//door//
	rightOfPlayerCell = mSceneMgr->createEntity("Door_02", "CellDoor_Cells.mesh");
	rightOfPlayerCellNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door02_Node");
	rightOfPlayerCellNode->setPosition(Vector3(468.0f,0.0f,412.5f));
	rightOfPlayerCellNode->rotate(Vector3().UNIT_Y,Degree(180));
	rightOfPlayerCellNode->attachObject(rightOfPlayerCell);

	mEntitiesLTS.push_back(rightOfPlayerCell);
	mSceneNodesLTS.push_back(rightOfPlayerCellNode);

	//johnny 666 celldoorCol//
	johnny666CellDoorCol = mSceneMgr->createEntity("Door_03Col", "DoorCollision.mesh");
	johnny666CellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_03ColNode");
	johnny666CellDoorColNode->attachObject(johnny666CellDoorCol);
	johnny666CellDoorColNode->setPosition(Vector3(550.0f,0.0f,522.0f));
	johnny666CellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(johnny666CellDoorCol);
	mSceneNodesLTS.push_back(johnny666CellDoorColNode);

	//johnny 666 celldoor//
	johnny666CellDoor = mSceneMgr->createEntity("Door_03", "CellDoor_Cells.mesh");
	johnny666CellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door03_Node");
	johnny666CellDoorNode->setPosition(Vector3(523.0f,0.0f,523.0f));
	johnny666CellDoorNode->attachObject(johnny666CellDoor);

	mEntitiesLTS.push_back(johnny666CellDoor);
	mSceneNodesLTS.push_back(johnny666CellDoorNode);
	
	//coll node//
	dannyBoyCellDoorCol = mSceneMgr->createEntity("Door_04Col", "DoorCollision.mesh");
	dannyBoyCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_04ColNode");
	dannyBoyCellDoorColNode->attachObject(dannyBoyCellDoorCol);
	dannyBoyCellDoorColNode->setPosition(Vector3(441.0f,0.0f,520.0f));
	dannyBoyCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(dannyBoyCellDoorCol);
	mSceneNodesLTS.push_back(dannyBoyCellDoorColNode);

	//dannyboys cell door//
	dannyBoyCellDoor = mSceneMgr->createEntity("Door_04", "CellDoor_Cells.mesh");
	dannyBoyCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door04_node");
	dannyBoyCellDoorNode->attachObject(dannyBoyCellDoor);
	dannyBoyCellDoorNode->setPosition(Vector3(468.0f,0.0f,520.5f));
	dannyBoyCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));

	mEntitiesLTS.push_back(dannyBoyCellDoor);
	mSceneNodesLTS.push_back(dannyBoyCellDoorNode);

	//jolly celll door col//
	jollyCellDoorCol = mSceneMgr->createEntity("Door_05Col", "DoorCollision.mesh");
	jollyCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_05ColNode");
	jollyCellDoorColNode->attachObject(jollyCellDoorCol);
	jollyCellDoorColNode->setPosition(945.0f,0.0f,407.0f);
	jollyCellDoorColNode->rotate(Vector3().UNIT_Y,Degree(180));
	jollyCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(jollyCellDoorCol);
	mSceneNodesLTS.push_back(jollyCellDoorColNode);

	//jollys cell door//
	jollyCellDoor = mSceneMgr->createEntity("Door_05", "CellDoor_Cells.mesh");
	jollyCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door05_Node");
	jollyCellDoorNode->attachObject(jollyCellDoor);
	jollyCellDoorNode->setPosition(972.0f,0.0f,412.5f);
	jollyCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));

	mEntitiesLTS.push_back(jollyCellDoor);
	mSceneNodesLTS.push_back(jollyCellDoorNode);

	//empty cell//
	emptyCellJollyDoorCol = mSceneMgr->createEntity("Door_06Col", "DoorCollision.mesh");
	emptyCellJollyDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_06ColNode");
	emptyCellJollyDoorColNode->attachObject(emptyCellJollyDoorCol);
	emptyCellJollyDoorColNode->setPosition(945.5f,0.0f,523.0f);
	emptyCellJollyDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyCellJollyDoorCol);
	mSceneNodesLTS.push_back(emptyCellJollyDoorColNode);

	//empty cell across from jollys//
	emptyCellJollyDoor = mSceneMgr->createEntity("Door_06", "CellDoor_Cells.mesh");
	emptyCellJollyDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door06_Node");
	emptyCellJollyDoorNode->attachObject(emptyCellJollyDoor);
	emptyCellJollyDoorNode->setPosition(972.0f,0.0f,520.5f);
	emptyCellJollyDoorNode->rotate(Vector3().UNIT_Y,Degree(180));

	mEntitiesLTS.push_back(emptyCellJollyDoor);
	mSceneNodesLTS.push_back(emptyCellJollyDoorNode);

	//chaos col//
	chaosCellDoorCol = mSceneMgr->createEntity("Door_07Col", "DoorCollision.mesh");
	chaosCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_07ColNode");
	chaosCellDoorColNode->attachObject(chaosCellDoorCol);
	chaosCellDoorColNode->setPosition(45.5f,0.0f,520.0f);
	chaosCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(chaosCellDoorCol);
	mSceneNodesLTS.push_back(chaosCellDoorColNode);

	//chaos cult cell door//
	chaosCellDoor = mSceneMgr->createEntity("Door_07", "CellDoor_Cells.mesh");
	chaosCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_07");
	chaosCellDoorNode->attachObject(chaosCellDoor);
	chaosCellDoorNode->setPosition(18.0f,0.0f,523.0f);

	mEntitiesLTS.push_back(chaosCellDoor);
	mSceneNodesLTS.push_back(chaosCellDoorNode);

	//empty col//
	emptyRightCornerDoorCol = mSceneMgr->createEntity("Door_08Col", "DoorCollision.mesh");
	emptyRightCornerDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_08ColNode");
	emptyRightCornerDoorColNode->attachObject(emptyRightCornerDoorCol);
	emptyRightCornerDoorColNode->setPosition(45.5f,0.0f,412.0f);
	emptyRightCornerDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyRightCornerDoorCol);
	mSceneNodesLTS.push_back(emptyRightCornerDoorColNode);

	//empty cell right corner//
	emptyRightCornerDoor = mSceneMgr->createEntity("Door_08", "CellDoor_Cells.mesh");
	emptyRightCornerDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door");
	emptyRightCornerDoorNode->attachObject(emptyRightCornerDoor);
	emptyRightCornerDoorNode->setPosition(18.0f,0.0f,415.0f);

	mEntitiesLTS.push_back(emptyRightCornerDoor);
	mSceneNodesLTS.push_back(emptyRightCornerDoorNode);

	//terrance col//
	terranceDougieCellDoorCol = mSceneMgr->createEntity("Door_09Col", "DoorCollision.mesh");
	terranceDougieCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_09ColNode");
	terranceDougieCellDoorColNode->attachObject(terranceDougieCellDoorCol);
	terranceDougieCellDoorColNode->setPosition(45.5f,0.0f,1455.0f);
	terranceDougieCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(terranceDougieCellDoorCol);
	mSceneNodesLTS.push_back(terranceDougieCellDoorColNode);

	//terrance celld door//
	terranceDougieCellDoor = mSceneMgr->createEntity("Door_09", "CellDoor_Cells.mesh");
	terranceDougieCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_09");
	terranceDougieCellDoorNode->attachObject(terranceDougieCellDoor);
	terranceDougieCellDoorNode->setPosition(18.0f,0.0f,1459.5f);
	
	mEntitiesLTS.push_back(terranceDougieCellDoor);
	mSceneNodesLTS.push_back(terranceDougieCellDoorNode);

	//col//
	emptyTerranceCellDoorCol = mSceneMgr->createEntity("Door_10Col", "DoorCollision.mesh");
	emptyTerranceCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_10ColNode");
	emptyTerranceCellDoorColNode->attachObject(emptyTerranceCellDoorCol);
	emptyTerranceCellDoorColNode->setPosition(441.0f,0.0f,1454.0f);
	emptyTerranceCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(emptyTerranceCellDoorCol);
	mSceneNodesLTS.push_back(emptyTerranceCellDoorColNode);

	//empty cell next to terrance//
	emptyTerranceCellDoor = mSceneMgr->createEntity("Door_10", "CellDoor_Cells.mesh");
	emptyTerranceCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_10");
	emptyTerranceCellDoorNode->attachObject(emptyTerranceCellDoor);
	emptyTerranceCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));
	emptyTerranceCellDoorNode->setPosition(469.0f,0.0f,1457.0f);

	mEntitiesLTS.push_back(emptyTerranceCellDoor);
	mSceneNodesLTS.push_back(emptyTerranceCellDoorNode);

	//col//
	santosoCellDoorCol = mSceneMgr->createEntity("Door_11Col", "DoorCollision.mesh");
	santosoCellDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_11ColNode");
	santosoCellDoorColNode->attachObject(santosoCellDoorCol);
	santosoCellDoorColNode->setPosition(946.0f,0.0f,1460.0f);
	santosoCellDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(santosoCellDoorCol);
	mSceneNodesLTS.push_back(santosoCellDoorColNode);

	//santosos shop cell door node//
	santosoCellDoor = mSceneMgr->createEntity("Door_11", "CellDoor_Cells.mesh");
	santosoCellDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_11");
	santosoCellDoorNode->attachObject(santosoCellDoor);
	santosoCellDoorNode->rotate(Vector3().UNIT_Y,Degree(180));
	santosoCellDoorNode->setPosition(972.0f,0.0f,1457.0f);

	mEntitiesLTS.push_back(santosoCellDoor);
	mSceneNodesLTS.push_back(santosoCellDoorNode);

	//lin mas col//
	linMasDoorCol = mSceneMgr->createEntity("Door_12Col", "DoorCollision.mesh");
	linMasDoorColNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_12ColNode");
	linMasDoorColNode->attachObject(linMasDoorCol);
	linMasDoorColNode->setPosition(385.5f,0.0f,1647.0f);
	linMasDoorColNode->setVisible(false);

	mEntitiesLTS.push_back(linMasDoorCol);
	mSceneNodesLTS.push_back(linMasDoorColNode);

	//lin mas office door//
	linMasDoor = mSceneMgr->createEntity("Door_12", "CellDoor_Office.mesh");
	linMasDoorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Door_12");
	linMasDoorNode->attachObject(linMasDoor);
	linMasDoorNode->setPosition(357.0f,0.0f,1647.0f);

	mEntitiesLTS.push_back(linMasDoor);
	mSceneNodesLTS.push_back(linMasDoorNode);

	//reset variables
	//door pitch counter//
	door01Pitch = 0;
	door02Pitch = 0;
	door03Pitch = 0;
	door04Pitch = 0;
	door05Pitch = 0;
	door06Pitch = 0;
	door07Pitch = 0;
	door08Pitch = 0;
	door09Pitch = 0;
	door10Pitch = 0;
	door11Pitch = 0;
	door12Pitch = 0;

	//door variables//
	Door01_Open = false;
	Door02_Open = false;
	Door03_Open = false;
	Door04_Open = false;
	Door05_Open = false;
	Door06_Open = false;
	Door07_Open = false;
	Door08_Open = false;
	Door09_Open = false;
	Door10_Open = false;
	Door11_Open = false;
	Door12_Open = false;

	//reset variables
	pitchLTS = 0;

	//rotate camera and character to face proper direction
	mCamera->setOrientation(Quaternion(Degree(-180.0f), Vector3::UNIT_Y));
	PlayerNodeLTS->yaw(Radian(0));

	audio_result = system->createStream("Sounds/Music/Cells.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	if(levelsMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
	}

	audio_result = system->createSound(currentHeartBeat, FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
	ERRCHECK(audio_result);

	if(levelsSFXSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
		ERRCHECK(audio_result);
	}
}

void LevelTestState::initQuests( void )
{
	//delcare xml doc//
	decl2 = new TiXmlDeclaration( "1.0", "","");
	decl3 = new TiXmlDeclaration( "1.0", "","");
	doc2.LinkEndChild(decl2);
	doc3.LinkEndChild(decl3);

	//creates the xml stuff//
	root3 = new TiXmlElement("QuestTracker");
	root2 = new TiXmlElement("QuestLog");
	doc2.LinkEndChild(root2);
	doc3.LinkEndChild(root3);

	//creates the element to keep track of quest completed//
	questLog = new TiXmlElement("QuestLog");
	root2->LinkEndChild(questLog);
}
void LevelTestState::initInventory( void )
{
	if(existingCharacter == false)
	{
		/*
			//doc.Clear();
		//declare xml doc//
		decl = new TiXmlDeclaration( "1.0", "","");
		doc.LinkEndChild(decl);

		//creates the xml stuff//
		root = new TiXmlElement("Inventory");
		doc.LinkEndChild(root);

		inventoryFile = new TiXmlElement("Inventory");
		inventoryQuest = new TiXmlElement("QuestTracker");
		inventoryCurrentQuest = new TiXmlElement("CurrentQuest");
		inventoryName = new TiXmlElement("Name");
		inventoryOther = new TiXmlElement("Other"); 

		root->LinkEndChild(inventoryFile);
		root->LinkEndChild(inventoryQuest);
		root->LinkEndChild(inventoryCurrentQuest);
		root->LinkEndChild(inventoryName);
		root->LinkEndChild(inventoryOther); */
		//sets the variables to 0//
		Cigarettes = 0;
		Syringe = 0;
		Soap = 0;
		SoapMold = 0;
		LinMasKey = 0;
		CellKey = 0;
		CellKeyCopy = 0;
		GoldCockroach = 0;
		Pendant = 0;
		Earing = 0;
		Cockroach = 0;
		Rat = 0;
		PainKillers = 0;
		Money = 0;

		//sets the quest completion variables to 0//
		linMasBugInTheSystemQuestCompleted = false;
		linMasTheRatScuttleQuestCompleted = false;
		linMaswhoIsEscapingQuestCompleted = false;
		linMastrainQuestCompleted = false;
		linMasPickAFactionQuestCompleted = false;

		jollytalkToTheSwordQuestCompleted = false;
		jollyKeyToThePlanQuestCompleted = false;
		jollyTwoPendantsQuestCompleted = false;
		jollyKnockEmOutQuestCompleted = false;

		geminitalkToTheSwordQuestCompleted = false;	
		geminitalkToTheSword2QuestCompleted = false;
		gemniThePrideOfJollyQuestCompleted = false; 
		geminiKeyToThePlanQuestCompleted = false;
		geminiKnockEmOutQuestCompleted = false;

		johnny666TrainingDoneQuestCompleted = false;
		johnny66680RepQuestCompleted = false;
		johnny666CheatALittleQuestCompleted = false;

		linMasFinalQuestQuestCompleted = false;
		linMasTrainingQuestCompleted = false;
		linMasMoreBugsInTheSystemQuestCompleted = false;

		//sets the picked up flags//
		emptyCellRightOfPlayerTaken = false;
		dannyBoySteroidsTaken = false;
		santosoPainKillerTaken = false;
		linMasMoneyTaken = false;
		linMasSoapTaken = false;
		factionedPickedJolly = false;
		factionedPickedGemini = false;
	}
	else
	{
		TiXmlDocument loadDoc("Inventory.xml");
		loadDoc.LoadFile();

		TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Inventory");

		Cigarettes = atoi(element->Attribute("Cigarettes"));
		PainKillers = atoi(element->Attribute("PainKillers"));
		Money = atoi(element->Attribute("Money"));
		Syringe = atoi(element->Attribute("Syringe"));
		Soap = atoi(element->Attribute("Soap"));
		SoapMold = atoi(element->Attribute("SoapMold"));
		LinMasKey = atoi(element->Attribute("LinMasKey"));
		CellKey = atoi(element->Attribute("CellKey"));
		CellKeyCopy = atoi(element->Attribute("CellKeyCopy"));
		GoldCockroach = atoi(element->Attribute("GoldCockroach"));
		Pendant = atoi(element->Attribute("Pendant"));
		Earing = atoi(element->Attribute("Earing"));
		Cockroach = atoi(element->Attribute("Cockroach"));
		Rat = atoi(element->Attribute("Rat"));

		element = loadDoc.RootElement()->FirstChildElement("QuestTracker");

		//sets the quest completion variables to 0//
		linMasBugInTheSystemQuestCompleted = atoi(element->Attribute("linMasBugInTheSystemQuestCompleted"));
		linMasTheRatScuttleQuestCompleted = atoi(element->Attribute("linMasTheRatScuttleQuestCompleted"));
		linMaswhoIsEscapingQuestCompleted = atoi(element->Attribute("linMaswhoIsEscapingQuestCompleted"));
		linMastrainQuestCompleted = atoi(element->Attribute("linMastrainQuestCompleted"));
		linMasPickAFactionQuestCompleted = atoi(element->Attribute("linMasPickAFactionQuestCompleted"));

		jollytalkToTheSwordQuestCompleted = atoi(element->Attribute("jollytalkToTheSwordQuestCompleted"));
		jollyKeyToThePlanQuestCompleted = atoi(element->Attribute("jollyKeyToThePlanQuestCompleted"));
		jollyTwoPendantsQuestCompleted = atoi(element->Attribute("jollyTwoPendantsQuestCompleted"));
		jollyKnockEmOutQuestCompleted = atoi(element->Attribute("jollyKnockEmOutQuestCompleted"));

		geminitalkToTheSwordQuestCompleted = atoi(element->Attribute("geminitalkToTheSwordQuestCompleted"));
		//geminitalkToTheSword2QuestCompleted = atoi(atoi(element->Attribute("linMasBugInTheSystemQuestCompleted"));
		gemniThePrideOfJollyQuestCompleted = atoi(element->Attribute("gemniThePrideOfJollyQuestCompleted"));
		geminiKeyToThePlanQuestCompleted = atoi(element->Attribute("geminiKeyToThePlanQuestCompleted"));
		geminiKnockEmOutQuestCompleted = atoi(element->Attribute("geminiKnockEmOutQuestCompleted"));

		johnny666TrainingDoneQuestCompleted = atoi(element->Attribute("johnny666TrainingDoneQuestCompleted"));
		johnny66680RepQuestCompleted = atoi(element->Attribute("johnny66680RepQuestCompleted"));
		johnny666CheatALittleQuestCompleted = atoi(element->Attribute("johnny666CheatALittleQuestCompleted"));

		linMasFinalQuestQuestCompleted = atoi(element->Attribute("linMasFinalQuestQuestCompleted"));
		linMasTrainingQuestCompleted = atoi(element->Attribute("linMasBugInTheSystemQuestCompleted"));
		linMasMoreBugsInTheSystemQuestCompleted = atoi(element->Attribute("linMasMoreBugsInTheSystemQuestCompleted"));

		element = loadDoc.RootElement()->FirstChildElement("CurrentQuest");

		//Current quest//
		linMasFinalQuest = atoi(element->Attribute("linMasFinalQuest"));
		linMasTraining = atoi(element->Attribute("linMasTraining"));
		
		johnny666Rep = atoi(element->Attribute("johnnny666Rep"));
		johnny666CheatALittle = atoi(element->Attribute("johnny666CheatALittle"));
		johnny66680Rep = atoi(element->Attribute("johnny66680Rep"));
		//johnny666TrainingDone  
		
		geminiThePrideOfJolly = atoi(element->Attribute("geminiThePrideOfJolly"));
		geminiKnockEmOut = atoi(element->Attribute("geminiKnockEmOut"));
		geminiKeyToThePlan = atoi(element->Attribute("geminiKeyToThePlan"));
		geminiTalkToTheSword = atoi(element->Attribute("geminiTalkToTheSword"));
		
		jollyTwoPendants = atoi(element->Attribute("jollyTwoPendants"));
		jollyKnockEmOut = atoi(element->Attribute("jollyKnockEmOut"));
		jollyKeyToThePlan = atoi(element->Attribute("jollyKeyToThePlan"));
		santosoKeyToThePlan = atoi(element->Attribute("santosoKeyToThePlan"));
		santosoTalkToTheSword = atoi(element->Attribute("santosoTalkToTheSword"));
		jollyTalkToTheSword = atoi(element->Attribute("jollyTalkToTheSword"));

		linMaTrain = atoi(element->Attribute("linMaTrain"));
		linMasWhoIsEscaping = atoi(element->Attribute("linMasWhoIsEscaping"));
		linMasTheRatScuttle = atoi(element->Attribute("linMasTheRatScuttle"));
		linMasMoreBugsInTheSystem = atoi(element->Attribute("linMasMoreBugsInTheSystem"));
		linMasBugInTheSystem = atoi(element->Attribute("linMasBugInTheSystem"));
		linMasPickAFaction = atoi(element->Attribute("linMasPickAFaction"));

		element = loadDoc.RootElement()->FirstChildElement("Other");

		emptyCellRightOfPlayerTaken = atoi(element->Attribute("emptyCellRightOfPlayerTaken"));
		dannyBoySteroidsTaken = atoi(element->Attribute("dannyBoySteroidsTaken"));
		santosoPainKillerTaken = atoi(element->Attribute("santosoPainKillerTaken"));
		linMasMoneyTaken = atoi(element->Attribute("linMasMoneyTaken"));
		linMasSoapTaken = atoi(element->Attribute("linMasSoapTaken"));
		factionedPickedJolly = atoi(element->Attribute("factionedPickedJolly"));
		factionedPickedGemini = atoi(element->Attribute("factionedPickedGemini"));
	}

	
	/*
	//name//
	inventoryName->SetAttribute("linMasFinalQuest","linMasFinalQuest");
	inventoryName->SetAttribute("linMasTraining","linMasTraining");

	inventoryName->SetAttribute("johnnny666Rep","johnnny666Rep");
	inventoryName->SetAttribute("johnny666CheatALittle","johnny666CheatALittle");
	inventoryName->SetAttribute("johnny66680Rep","johnny66680Rep");
	inventoryName->SetAttribute("johnny666TrainingDone","johnny666TrainingDone");

	inventoryName->SetAttribute("geminiThePrideOfJolly","geminiThePrideOfJolly");
	inventoryName->SetAttribute("geminiKnockEmOut","geminiKnockEmOut");
	inventoryName->SetAttribute("geminiKeyToThePlan","geminiKeyToThePlan");
	inventoryName->SetAttribute("geminiTalkToTheSword","geminiTalkToTheSword");

	inventoryName->SetAttribute("jollyTwoPendants","jollyTwoPendants");
	inventoryName->SetAttribute("jollyKnockEmOut","jollyKnockEmOut");
	inventoryName->SetAttribute("jollyKeyToThePlan","jollyKeyToThePlan");
	inventoryName->SetAttribute("santosoKeyToThePlan","santosoKeyToThePlan");
	inventoryName->SetAttribute("santosoTalkToTheSword","santosoTalkToTheSword");
	inventoryName->SetAttribute("jollyTalkToTheSword","jollyTalkToTheSword");

	inventoryName->SetAttribute("linMaTrain","linMaTrain");
	inventoryName->SetAttribute("linMasWhoIsEscaping","linMasWhoIsEscaping");
	inventoryName->SetAttribute("linMasTheRatScuttle","linMasTheRatScuttle");
	inventoryName->SetAttribute("linMasMoreBugsInTheSystem","linMasMoreBugsInTheSystem");
	inventoryName->SetAttribute("linMasBugInTheSystem","linMasBugInTheSystem");
	inventoryName->SetAttribute("linMasPickAFaction","linMasPickAFaction");
	

	//Current quest//
	inventoryCurrentQuest->SetAttribute("linMasFinalQuest",linMasFinalQuest);
	inventoryCurrentQuest->SetAttribute("linMasTraining",linMasTraining);
	
	inventoryCurrentQuest->SetAttribute("johnnny666Rep",johnny666Rep);
	inventoryCurrentQuest->SetAttribute("johnny666CheatALittle",johnny666CheatALittle);
	inventoryCurrentQuest->SetAttribute("johnny66680Rep",johnny66680Rep);
	inventoryCurrentQuest->SetAttribute("johnny666TrainingDone",johnny666TrainingDone);
	
	inventoryCurrentQuest->SetAttribute("geminiThePrideOfJolly",geminiThePrideOfJolly);
	inventoryCurrentQuest->SetAttribute("geminiKnockEmOut", geminiKnockEmOut);
	inventoryCurrentQuest->SetAttribute("geminiKeyToThePlan",geminiKeyToThePlan);
	inventoryCurrentQuest->SetAttribute("geminiTalkToTheSword",geminiTalkToTheSword);
	
	inventoryCurrentQuest->SetAttribute("jollyTwoPendants",jollyTwoPendants);
	inventoryCurrentQuest->SetAttribute("jollyKnockEmOut",jollyKnockEmOut);
	inventoryCurrentQuest->SetAttribute("jollyKeyToThePlan",jollyKeyToThePlan);
	inventoryCurrentQuest->SetAttribute("santosoKeyToThePlan",santosoKeyToThePlan);
	inventoryCurrentQuest->SetAttribute("santosoTalkToTheSword",santosoTalkToTheSword);
	inventoryCurrentQuest->SetAttribute("jollyTalkToTheSword",jollyTalkToTheSword);
	
	inventoryCurrentQuest->SetAttribute("linMaTrain",linMaTrain);
	inventoryCurrentQuest->SetAttribute("linMasWhoIsEscaping",linMasWhoIsEscaping);
	inventoryCurrentQuest->SetAttribute("linMasTheRatScuttle",linMasTheRatScuttle);
	inventoryCurrentQuest->SetAttribute("linMasMoreBugsInTheSystem",linMasMoreBugsInTheSystem);
	inventoryCurrentQuest->SetAttribute("linMasBugInTheSystem",linMasBugInTheSystem);
	inventoryCurrentQuest->SetAttribute("linMasPickAFaction",linMasPickAFaction);

	//quest tracker//
	inventoryQuest->SetAttribute("linMasBugInTheSystemQuestCompleted",linMasBugInTheSystemQuestCompleted);
	inventoryQuest->SetAttribute("linMasTheRatScuttleQuestCompleted",linMasTheRatScuttleQuestCompleted);
	inventoryQuest->SetAttribute("linMaswhoIsEscapingQuestCompleted",linMaswhoIsEscapingQuestCompleted);
	inventoryQuest->SetAttribute("linMastrainQuestCompleted",linMastrainQuestCompleted);
	inventoryQuest->SetAttribute("linMasMoreBugsInTheSystemQuestCompleted",linMasMoreBugsInTheSystemQuestCompleted);
	inventoryQuest->SetAttribute("linMasPickAFactionQuestCompleted",linMasPickAFactionQuestCompleted);
	
	inventoryQuest->SetAttribute("jollytalkToTheSwordQuestCompleted",jollytalkToTheSwordQuestCompleted);
	inventoryQuest->SetAttribute("jollyKeyToThePlanQuestCompleted",jollyKeyToThePlanQuestCompleted);
	inventoryQuest->SetAttribute("jollyTwoPendantsQuestCompleted",jollyTwoPendantsQuestCompleted);
	inventoryQuest->SetAttribute("jollyKnockEmOutQuestCompleted",jollyKnockEmOutQuestCompleted);
	
	inventoryQuest->SetAttribute("geminitalkToTheSwordQuestCompleted",geminitalkToTheSwordQuestCompleted);
	inventoryQuest->SetAttribute("gemniThePrideOfJollyQuestCompleted",gemniThePrideOfJollyQuestCompleted);
	inventoryQuest->SetAttribute("geminiKnockEmOutQuestCompleted",geminiKnockEmOutQuestCompleted);
	inventoryQuest->SetAttribute("geminiKeyToThePlanQuestCompleted",geminiKeyToThePlanQuestCompleted);

	inventoryQuest->SetAttribute("johnny666TrainingDoneQuestCompleted",johnny666TrainingDoneQuestCompleted);
	inventoryQuest->SetAttribute("johnny66680RepQuestCompleted",johnny66680RepQuestCompleted);
	inventoryQuest->SetAttribute("johnny666CheatALittleQuestCompleted",johnny666CheatALittleQuestCompleted);

	inventoryQuest->SetAttribute("linMasFinalQuestQuestCompleted",linMasFinalQuestQuestCompleted);
	inventoryQuest->SetAttribute("linMasTrainingQuestCompleted",linMasTrainingQuestCompleted);

	//creates variables for the xml document//
	inventoryFile->SetAttribute("Cigarettes",Cigarettes);
	inventoryFile->SetAttribute("PainKillers",PainKillers);
	inventoryFile->SetAttribute("Money",Money);
	inventoryFile->SetAttribute("Syringe",Syringe);
	inventoryFile->SetAttribute("Soap",Soap);
	inventoryFile->SetAttribute("SoapMold",SoapMold);
	inventoryFile->SetAttribute("LinMasKey",LinMasKey);
	inventoryFile->SetAttribute("CellKey",CellKey);
	inventoryFile->SetAttribute("CellKeyCopy",CellKeyCopy);
	inventoryFile->SetAttribute("GoldCockroach",GoldCockroach);
	inventoryFile->SetAttribute("Pendant",Pendant);
	inventoryFile->SetAttribute("Earing",Earing);
	inventoryFile->SetAttribute("Cockroach",Cockroach);
	inventoryFile->SetAttribute("Rat",Rat);  */
}

void LevelTestState::saveInventory( void )
{
		TiXmlDocument doc;

		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
		doc.LinkEndChild(decl);
	
		TiXmlElement *root = new TiXmlElement("Inventory");
		doc.LinkEndChild(root);

		TiXmlElement *inventory = new TiXmlElement("Inventory");
		root->LinkEndChild(inventory);

		//creates variables for the xml document//
		inventory->SetAttribute("Cigarettes",Cigarettes);
		inventory->SetAttribute("PainKillers",PainKillers);
		inventory->SetAttribute("Money",Money);
		inventory->SetAttribute("Syringe",StoreSyringe+Syringe);
		inventory->SetAttribute("Soap",Soap);
		inventory->SetAttribute("SoapMold",SoapMold);
		inventory->SetAttribute("LinMasKey",LinMasKey);
		inventory->SetAttribute("CellKey",CellKey);
		inventory->SetAttribute("CellKeyCopy",CellKeyCopy);
		inventory->SetAttribute("GoldCockroach",GoldCockroach);
		inventory->SetAttribute("Pendant",Pendant);
		inventory->SetAttribute("Earing",Earing);
		inventory->SetAttribute("Cockroach",Cockroach);
		inventory->SetAttribute("Rat",Rat);

		TiXmlElement *QuestTracker = new TiXmlElement("QuestTracker");
		root->LinkEndChild(QuestTracker);

		QuestTracker->SetAttribute("linMasBugInTheSystemQuestCompleted",linMasBugInTheSystemQuestCompleted);
		QuestTracker->SetAttribute("linMasTheRatScuttleQuestCompleted",linMasTheRatScuttleQuestCompleted);
		QuestTracker->SetAttribute("linMaswhoIsEscapingQuestCompleted",linMaswhoIsEscapingQuestCompleted);
		QuestTracker->SetAttribute("linMastrainQuestCompleted",linMastrainQuestCompleted);
		QuestTracker->SetAttribute("linMasMoreBugsInTheSystemQuestCompleted",linMasMoreBugsInTheSystemQuestCompleted);
		QuestTracker->SetAttribute("linMasPickAFactionQuestCompleted",linMasPickAFactionQuestCompleted);
		
		QuestTracker->SetAttribute("jollytalkToTheSwordQuestCompleted",jollytalkToTheSwordQuestCompleted);
		QuestTracker->SetAttribute("jollyKeyToThePlanQuestCompleted",jollyKeyToThePlanQuestCompleted);
		QuestTracker->SetAttribute("jollyTwoPendantsQuestCompleted",jollyTwoPendantsQuestCompleted);
		QuestTracker->SetAttribute("jollyKnockEmOutQuestCompleted",jollyKnockEmOutQuestCompleted);
		
		QuestTracker->SetAttribute("geminitalkToTheSwordQuestCompleted",geminitalkToTheSwordQuestCompleted);
		QuestTracker->SetAttribute("gemniThePrideOfJollyQuestCompleted",gemniThePrideOfJollyQuestCompleted);
		QuestTracker->SetAttribute("geminiKnockEmOutQuestCompleted",geminiKnockEmOutQuestCompleted);
		QuestTracker->SetAttribute("geminiKeyToThePlanQuestCompleted",geminiKeyToThePlanQuestCompleted);

		QuestTracker->SetAttribute("johnny666TrainingDoneQuestCompleted",johnny666TrainingDoneQuestCompleted);
		QuestTracker->SetAttribute("johnny66680RepQuestCompleted",johnny66680RepQuestCompleted);
		QuestTracker->SetAttribute("johnny666CheatALittleQuestCompleted",johnny666CheatALittleQuestCompleted);

		QuestTracker->SetAttribute("linMasFinalQuestQuestCompleted",linMasFinalQuestQuestCompleted);
		QuestTracker->SetAttribute("linMasTrainingQuestCompleted",linMasTrainingQuestCompleted);

		TiXmlElement *CurrentQuest = new TiXmlElement("CurrentQuest");
		root->LinkEndChild(CurrentQuest);

		//Current quest//
		CurrentQuest->SetAttribute("linMasFinalQuest",linMasFinalQuest);
		CurrentQuest->SetAttribute("linMasTraining",linMasTraining);
		
		CurrentQuest->SetAttribute("johnnny666Rep",johnny666Rep);
		CurrentQuest->SetAttribute("johnny666CheatALittle",johnny666CheatALittle);
		CurrentQuest->SetAttribute("johnny66680Rep",johnny66680Rep);
		CurrentQuest->SetAttribute("johnny666TrainingDone",johnny666TrainingDone);
		
		CurrentQuest->SetAttribute("geminiThePrideOfJolly",geminiThePrideOfJolly);
		CurrentQuest->SetAttribute("geminiKnockEmOut", geminiKnockEmOut);
		CurrentQuest->SetAttribute("geminiKeyToThePlan",geminiKeyToThePlan);
		CurrentQuest->SetAttribute("geminiTalkToTheSword",geminiTalkToTheSword);
		
		CurrentQuest->SetAttribute("jollyTwoPendants",jollyTwoPendants);
		CurrentQuest->SetAttribute("jollyKnockEmOut",jollyKnockEmOut);
		CurrentQuest->SetAttribute("jollyKeyToThePlan",jollyKeyToThePlan);
		CurrentQuest->SetAttribute("santosoKeyToThePlan",santosoKeyToThePlan);
		CurrentQuest->SetAttribute("santosoTalkToTheSword",santosoTalkToTheSword);
		CurrentQuest->SetAttribute("jollyTalkToTheSword",jollyTalkToTheSword);

		CurrentQuest->SetAttribute("linMaTrain",linMaTrain);
		CurrentQuest->SetAttribute("linMasWhoIsEscaping",linMasWhoIsEscaping);
		CurrentQuest->SetAttribute("linMasTheRatScuttle",linMasTheRatScuttle);
		CurrentQuest->SetAttribute("linMasMoreBugsInTheSystem",linMasMoreBugsInTheSystem);
		CurrentQuest->SetAttribute("linMasBugInTheSystem",linMasBugInTheSystem);
		CurrentQuest->SetAttribute("linMasPickAFaction",linMasPickAFaction);
		
		TiXmlElement *Name = new TiXmlElement("Name");
		root->LinkEndChild(Name);

		Name->SetAttribute("linMasFinalQuest","linMasFinalQuest");
		Name->SetAttribute("linMasTraining","linMasTraining");

		Name->SetAttribute("johnnny666Rep","johnnny666Rep");
		Name->SetAttribute("johnny666CheatALittle","johnny666CheatALittle");
		Name->SetAttribute("johnny66680Rep","johnny66680Rep");
		Name->SetAttribute("johnny666TrainingDone","johnny666TrainingDone");

		Name->SetAttribute("geminiThePrideOfJolly","geminiThePrideOfJolly");
		Name->SetAttribute("geminiKnockEmOut","geminiKnockEmOut");
		Name->SetAttribute("geminiKeyToThePlan","geminiKeyToThePlan");
		Name->SetAttribute("geminiTalkToTheSword","geminiTalkToTheSword");

		Name->SetAttribute("jollyTwoPendants","jollyTwoPendants");
		Name->SetAttribute("jollyKnockEmOut","jollyKnockEmOut");
		Name->SetAttribute("jollyKeyToThePlan","jollyKeyToThePlan");
		Name->SetAttribute("santosoKeyToThePlan","santosoKeyToThePlan");
		Name->SetAttribute("santosoTalkToTheSword","santosoTalkToTheSword");
		Name->SetAttribute("jollyTalkToTheSword","jollyTalkToTheSword");

		Name->SetAttribute("linMaTrain","linMaTrain");
		Name->SetAttribute("linMasWhoIsEscaping","linMasWhoIsEscaping");
		Name->SetAttribute("linMasTheRatScuttle","linMasTheRatScuttle");
		Name->SetAttribute("linMasMoreBugsInTheSystem","linMasMoreBugsInTheSystem");
		Name->SetAttribute("linMasBugInTheSystem","linMasBugInTheSystem");
		Name->SetAttribute("linMasPickAFaction","linMasPickAFaction");

		TiXmlElement *Other = new TiXmlElement("Other");
		root->LinkEndChild(Other);

		Other->SetAttribute("emptyCellRightOfPlayerTaken", emptyCellRightOfPlayerTaken);
		Other->SetAttribute("dannyBoySteroidsTaken", dannyBoySteroidsTaken);
		Other->SetAttribute("santosoPainKillerTaken", santosoPainKillerTaken);
		Other->SetAttribute("linMasMoneyTaken", linMasMoneyTaken);
		Other->SetAttribute("linMasSoapTaken", linMasSoapTaken);
		Other->SetAttribute("factionedPickedJolly", factionedPickedJolly);
		Other->SetAttribute("factionedPickedGemini", factionedPickedGemini);
		Other->SetAttribute("LevelLoaded", LevelLoaded);
	
		doc.SaveFile("Inventory.xml");
	
}
void LevelTestState::saveQuests( void )
{
	doc2.SaveFile("QuestLog.xml");
}


void LevelTestState::updateLinMasQuest( void )
{
	//reads the xml document//
	TiXmlDocument loadDocOptions("QuestLog.xml");  
	loadDocOptions.LoadFile();

	//pick a faction quest//
	if(linMasPickAFactionQuestCompleted == false)
	{
		
		//lin mas initial dialogue quest//
		if(linMasPickAFaction == true)
		{	
			if(linMasPickAFactionDialogueCompleted == false)
			{
			
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("LinMasIntro");
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitalDialogue"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					inConversation = false;
					linMasPickAFactionDialogueCompleted = true;
					updateLinMasQuests = false;
					linMasPickAFactionQuestCompleted = true;
					npcTalk->setCaption("");
				}
			}
		}
	}
	
	if(linMasBugInTheSystemQuestCompleted == false)
	{
		//sets the flags accordingly//
		if(linMasBugInTheSystemDialogueCompleted == true && Cockroach != 10)
		{
			linMasBugInTheSystemDialogueFailed = true;
		}
		else if(linMasBugInTheSystemDialogueCompleted == true && Cockroach >= 10)
		{
			linMasBugInTheSystemDialogueCompleted = true;
			linMasBugInTheSystemDialogueFailed = false;
		}
		
		//lin mas bug in the system quest//
		if(linMasBugInTheSystem == true)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("BugsInTheSystem");


			if(linMasBugInTheSystemDialogueFailed == true )
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue04"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					inConversation = false;
					linMasBugInTheSystem = true;
					npcTalk->setCaption("");
				}
			}
			else if(linMasBugInTheSystemDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue03"));
				}
				else if(dialogueCounter == 4)
				{
					dialogueCounter = 0;
					inConversation = false;
					linMasBugInTheSystemDialogueCompleted = true;
					npcTalk->setCaption("");
				}
			}
			else if(linMasBugInTheSystemDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue05"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					linMasBugInTheSystemQuestCompleted = true;
					inConversation = false;
					updateLinMasQuests = false;
					Cockroach = 0;
					npcTalk->setCaption("");
				}
			}
			
			
		}
	}
	else if(linMasMoreBugsInTheSystemQuestCompleted == false)
	{
		//sets the flags accordingly//
		if(limMasMoreBugsInTheSystemDialogueCompleted == true && Cockroach != 20)
		{
			limMasMoreBugsInTheSystemDialogueFailed = true;
		}
		else if(limMasMoreBugsInTheSystemDialogueCompleted == true && Cockroach == 20)
		{
			limMasMoreBugsInTheSystemDialogueCompleted = true;
			limMasMoreBugsInTheSystemDialogueFailed = false;
		}

		//lin mas bugs in the system quest//
		if(linMasMoreBugsInTheSystem == true)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("MoreBugsInTheSystem");
			
			//if you have not completed the quest//
			if(limMasMoreBugsInTheSystemDialogueFailed == true)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasFailure"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					inConversation = false;
					linMasMoreBugsInTheSystem = true;
					npcTalk->setCaption("");
				}
			}
			else if(limMasMoreBugsInTheSystemDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					Cigarettes += 5;
					limMasMoreBugsInTheSystemDialogueCompleted = true;
					inConversation = false;
					npcTalk->setCaption("");
				}
			}
			else if(limMasMoreBugsInTheSystemDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					dialogueCounter = 0;
					Cigarettes += 10;
					linMasMoreBugsInTheSystemQuestCompleted = true;
					inConversation = false;
					updateLinMasQuests = false;
					npcTalk->setCaption("");
				}
			}
		}
	}
	//the rat scuttle quest//
	else if(linMasTheRatScuttleQuestCompleted == false)
	{	
		//sets the flags accordingly//
		if(linMasTheRatScuttleDialogueCompleted == true && Rat != 20)
		{
			linMasTheRatScuttleDialogueFailed= true;
		}
		else if(linMasTheRatScuttleDialogueCompleted == true && Rat == 20)
		{
			linMasTheRatScuttleDialogueCompleted = true;
			linMasTheRatScuttleDialogueFailed = false;
		}

		if(linMasTheRatScuttle == true)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("TheRatsScuttle");

			if(linMasTheRatScuttleDialogueFailed == true)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasFailure"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					inConversation = false;
					npcTalk->setCaption("");
				}
			}
			else if(linMasTheRatScuttleDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					linMasTheRatScuttleDialogueCompleted = true;
					inConversation = false;
					npcTalk->setCaption("");
				}
			}
			else if(linMasTheRatScuttleDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue02"));
				}
				else if(dialogueCounter == 2)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue03"));
				}
				else if(dialogueCounter == 3)
				{
					dialogueCounter = 0;
					inConversation = false;
					Cigarettes += 20;
					linMasTheRatScuttleQuestCompleted = true;
					linMasTheRatScuttleCompleted = true;
					updateLinMasQuests = false;
					npcTalk->setCaption("");
				}
			}
		}
	}
	//lin mas who is escaping//
	else if(linMaswhoIsEscapingQuestCompleted == false)
	{

		//sets the flags accordingly//
		if(linMasWhoIsEscapingDialogueCompleted == true && WhoIsEscaping == false)
		{
			linMasWhoIsEscapingDialogueFailed = true;
		}
		else if(linMasWhoIsEscapingDialogueCompleted == true && WhoIsEscaping == true)
		{
			linMasWhoIsEscapingDialogueCompleted = true;
			linMasWhoIsEscapingDialogueFailed = false;
		}
		
		if(linMasWhoIsEscaping == true)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("WhoIsEscaping");

			//if you fail the quest//
			if(linMasWhoIsEscapingDialogueFailed == true)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasFailure"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					inConversation = false;
					npcTalk->setCaption("");
				}
			}
			//get the quest from the character//
			else if(linMasWhoIsEscapingDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue03"));
				}
				else if(dialogueCounter == 4)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue04"));
				}
				else if(dialogueCounter == 5)
				{
					dialogueCounter = 0;
					linMasWhoIsEscapingDialogueCompleted = true;
					inConversation = false;
					npcTalk->setCaption("");
				}
			}
			//if you complete the quest//
			else if(linMasWhoIsEscapingDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue05"));
				}
				else if(dialogueCounter == 2)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue06"));
				}
				else if(dialogueCounter == 3)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue07"));
				}
				else if(dialogueCounter == 4)
				{
					dialogueCounter = 0;
					linMaswhoIsEscapingQuestCompleted = true;
					inConversation = false;
					updateLinMasQuests = false;
					npcTalk->setCaption("");
				}
			}
		}
	}
	else if(linMastrainQuestCompleted == false)
	{
		//lin mas train quest//
		if(linMaTrain == true)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Train");

			//getting the quest from npcs//
			if(linMaTrainDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue03"));
				}
				else if(dialogueCounter == 4)
				{
					inConversation = false;
					linMastrainQuestCompleted = true;
					updateLinMasQuests = false;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
		}
	}
}

void LevelTestState::updateGeminiQuest( void )
{
	//reads the xml document//
	TiXmlDocument loadDocOptions("QuestLog.xml");  
	loadDocOptions.LoadFile();


	//gemini knock em out//
	if(geminiKnockEmOutQuestCompleted == false)
	{
		if(geminiKnockEmOutDialogueCompleted == true && jollyKnockedOut == true)
		{
			geminiKnockEmOutDialogueCompleted = true;
		}
		else if(geminiKnockEmOutDialogueCompleted == true && jollyKnockedOut == false)
		{
			geminiKnockEmOutDialogueCompleted = false;
			geminiKnockEmOutDialogueFailed = true;
		}

		//gemini knock em out quest//
		if(geminiKnockEmOut == true)
		{
			//failure of the quest//
			if(geminiKnockEmOutDialogueFailed == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiKnockEmOut");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsFailure"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					jollyKnockedOut = true;
					geminiKnockEmOutQuestCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			//getting the quest//
			else if(geminiKnockEmOutDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiKnockEmOut");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiKnockEmOut");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiKnockEmOut");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue03"));
				}
				else if(dialogueCounter == 4)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiKnockEmOut");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue04"));
				}
				else if(dialogueCounter == 5)
				{
					inConversation = false;
					geminiKnockEmOutDialogueCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			//if the quest is completed
			else if(geminiKnockEmOutDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiKnockEmOut");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsCompleted"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					updateGeminiQuests = false;
					geminiKnockEmOutQuestCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			

		}
	}

	//gemini talk to the sword//
	if(geminitalkToTheSwordQuestCompleted == false)
	{
		//gemini talk to the sword//
		if(geminiTalkToTheSword == true)
		{
			if(dialogueCounter == 1)
			{
				pickAFaction->show();
				pickAFaction->setCaption("Join The Gemini Twins Faction Y/N");
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("AfterFactionAllignedGeminiTwins");
				npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue01"));
			}
			else if(dialogueCounter == 2 && yes == true)
			{
				pickAFaction->hide();
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("AfterFactionAllignedGeminiTwins");
				npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue02"));
			}
			else if(dialogueCounter == 3 && yes == true)
			{
				pickAFaction->hide();
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiTalkToTheSword");
				npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue01"));
			}
			else if(dialogueCounter  == 4 && yes == true)
			{
				pickAFaction->hide();
				inConversation = false;
				factioned = true;
				factionedGemini = true;
				linMasPickAFactionQuestCompleted = true;
				linMasPickAFaction = false;
				geminitalkToTheSwordQuestCompleted = true;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}

		}
	}
	//gemini talk to the sword//
	else if(geminitalkToTheSword2QuestCompleted == false)
	{
		//second part of talk to the sword//
		if(geminiTalkToTheSwordTwo == true)
		{
			//getting the quest//
			if(geminiTalkToTheSwordTwoDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiTalkToTheSword02");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiTalkToTheSword02");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					inConversation = false;
					geminiTalkToTheSwordDialogueCompleted = true;
					geminitalkToTheSword2QuestCompleted = true;
					geminiTalkToTheSwordTwoDialogueFailed = false;
					CellKey = 1;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			//if you finish the quest//
			else if(geminiTalkToTheSwordTwoDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiTalkToTheSword02");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsCompleted"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					updateGeminiQuests = false;
					CellKey = 1;
					geminitalkToTheSword2QuestCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			//failure of the quest//
			else if(geminiTalkToTheSwordTwoDialogueFailed == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiTalkToTheSword02");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsFailure"));
				}
				else if(dialogueCounter == 2)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiTalkToTheSword02");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsFailure02"));
				}
				else if(dialogueCounter == 3)
				{
					inConversation = false;
					geminitalkToTheSword2QuestCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
		}
	}
	//key to the plan quest//
	else if(geminiKeyToThePlanQuestCompleted == false)
	{
		//gemini key to the plan//
		if(geminiKeyToThePlan == true)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiKeyToThePlan");
				npcTalk->setCaption(quest->Attribute("GeminiTwinsCompleted"));
			}
			else if(dialogueCounter == 2)
			{
				inConversation = false;
				updateGeminiQuests = false;
				geminiKeyToThePlanQuestCompleted = true;
				dialogueCounter = 1;
				npcTalk->setCaption("");
			}
		}
	}
	
	
	
	else if(gemniThePrideOfJollyQuestCompleted == false)
	{
		//gemini pride of jolly//
		if(geminiThePrideOfJolly == true)
		{
			//getting the quest//
			if(geminiThePrideOfJollyDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiThePrideAndJoyOfJolly");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiThePrideAndJoyOfJolly");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiThePrideAndJoyOfJolly");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsInitialDialogue03"));
				}
				else if(dialogueCounter == 4)
				{
					inConversation = false;
					geminiThePrideOfJollyDialogueCompleted = true;
					gemniThePrideOfJollyQuestCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			//if you finish the quest//
			else if(geminiThePrideOfJollyDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiThePrideAndJoyOfJolly");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsCompleted"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					updateGeminiQuests = false;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			//if you fail the quest//
			else if(geminiThePrideOfJollyDialogueFailed == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiThePrideAndJoyOfJolly");
					npcTalk->setCaption(quest->Attribute("GeminiTwinsFailure"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}

		}
	}
	//finish all the gemini quests//
	else if(geminiQuestsCompleted == true)
	{
		if(dialogueCounter == 1)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("GeminiQuestsCompleted");
			npcTalk->setCaption(quest->Attribute("GeminiTwinsinitialDialogue01"));
		}
		else if(dialogueCounter == 2)
		{
			inConversation = false;
			updateGeminiQuests = false;
			dialogueCounter = 0;
			npcTalk->setCaption("");
		}
	}


}
void LevelTestState::updateSantosoQuest( void )
{

	//reads the xml document//
	TiXmlDocument loadDocOptions("QuestLog.xml");  
	loadDocOptions.LoadFile();



	//santoso shop//
	if(santosoShop == true)
	{
		yes = false;
		no = false;
		if(dialogueCounter == 1)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoBuyingSteroids");
			npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue01"));
		}
		else if(dialogueCounter == 2)
		{
			inConversation = false;
			santosoShop = false;
			if(yes == true)
			{
				store = true;
			}
			else if(no == true)
			{
				santosoShop = false;
			}
			dialogueCounter = 0;
			npcTalk->setCaption("");
		}
	}

	//santoso talk to the sword quest//
	if(santosoTalkToTheSwordQuestCompleted == false)
	{
		//santoso talk to the sword//
		if(santosoTalkToTheSword == true)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoDialogue");
				npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue01"));
			}
			else if(dialogueCounter == 2)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoDialogue");
				npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue02"));
			}
			else if(dialogueCounter == 3)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoDialogue");
				npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue03"));
			}
			else if(dialogueCounter == 4)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoKeyToThePlan");
				npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue01"));
			}
			else if(dialogueCounter == 5)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoKeyToThePlan");
				npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue02"));
			}
			else if(dialogueCounter == 6)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoKeyToThePlan");
				npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue03"));
			}
			else if(dialogueCounter == 7)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoKeyToThePlan");
				npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue04"));
			}
			else if(dialogueCounter == 8)
			{
				inConversation = false;
				santosoTalkToTheSword = false;
				updateSantosoQuests = false;
				santosoTalkToTheSwordQuestCompleted = true;
				santosoTalkToTheSword = false;
				jollyTalkedToSantoso = true;
				jollyTalkToTheSword = false;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}
		}
	}
	//santoso key to the plan
	else if(santosoKeyToThePlanQuestCompleted == false)
	{
		//santosos key to the plan two//
		if(santosoKeyToThePlanCompleted == true)
		{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoKeyToThePlan2");
					npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoKeyToThePlan2");
					npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("SantosoKeyToThePlan2");
					npcTalk->setCaption(quest->Attribute("SantosoInitialDialogue03"));
				}
				else if(dialogueCounter == 4)
				{
					inConversation = false;
					santosoKeyToThePlanQuestCompleted = true;
					updateSantosoQuests = false;
					santosoKeyToThePlanCompleted = false;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}	
		}
	}
}
void LevelTestState::updateJollysQuest( void )
{

	//reads the xml document//
	TiXmlDocument loadDocOptions("QuestLog.xml");  
	loadDocOptions.LoadFile();

	//jolly pick a faction quest//
	if(factionedPickedJollyQuestCompleted == false)
	{
		if(factionedJolly == true)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("FactionAllignedJolly");


				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("JollyInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					npcTalk->setCaption(quest->Attribute("JollyInitialDialogue02"));			
				}
				else if(dialogueCounter == 3 && yes == true)
				{
					pickAFaction->hide();
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("AfterFactionAllignedJolly");
					npcTalk->setCaption(quest->Attribute("JollyInitialDialogue01"));
				}
				else if(dialogueCounter == 4 && yes == true)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("AfterFactionAllignedJolly");
					npcTalk->setCaption(quest->Attribute("JollyInitialDialogue02"));
				}
				else if(dialogueCounter == 5 && yes == true)
				{
					inConversation = false;
					factionedJolly = true;
					updateJollysQuests = false;
					factioned = true;
					linMasPickAFactionQuestCompleted = true;
					linMasPickAFaction = false;
					factionedPickedJollyQuestCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
		}
	}
	if(jollytalkToTheSwordQuestCompleted == false)
	{
			if(jollyTalkToTheSword == true)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("JollyTalkToTheSword");


					if(dialogueCounter == 1)
					{
						npcTalk->setCaption(quest->Attribute("JollyInitialDialogue01"));
					}
					else if(dialogueCounter == 2)
					{
						npcTalk->setCaption(quest->Attribute("JollyInitialDialogue02"));			
					}
					else if(dialogueCounter == 3 )
					{
						inConversation = false;
						factionedJolly = true;
						updateJollysQuests = false;
						jollytalkToTheSwordQuestCompleted = true;
						dialogueCounter = 0;
						npcTalk->setCaption("");
					}
			}
	}
	////talk to the sword quest//
	//else if(jollytalkToTheSwordQuestCompleted == false)
	//{
	//	//jolly talk to the sword//
	//	if(jollyTalkToTheSword == true)
	//	{
	//		TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("JollyTalkToTheSword");

	//		if(jollyTalkToTheSwordDialogueCompleted == false)
	//		{
	//			if(dialogueCounter == 1)
	//			{
	//				npcTalk->setCaption(quest->Attribute("JollyInitialDialogue01"));
	//			}
	//			else if(dialogueCounter == 2)
	//			{
	//				npcTalk->setCaption(quest->Attribute("JollyInitialDialogue02"));
	//			}
	//			else if(dialogueCounter == 3)
	//			{
	//				dialogueCounter = 0;
	//				inConversation = false;
	//				jollytalkToTheSwordQuestCompleted = true;
	//				updateJollysQuests = false;
	//				npcTalk->setCaption("");
	//			}
	//		}
	//	}
	//}
	else if(jollyKeyToThePlanQuestCompleted == false)
	{
		//sets the flags accordingly//
		if(jollyKeyToThePlanDialogueCompleted == true && Soap != 1)
		{
			jollyKeyToThePlanDialogueFailed = true;
		}
		else if(jollyKeyToThePlanDialogueCompleted == true && Soap == 1)
		{
			jollyKeyToThePlanDialogueCompleted = true;
			jollyKeyToThePlanDialogueFailed = false;
		}
		
		//lin mas bug in the system quest//
		if(jollyKeyToThePlan == true)
		{
			TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("JollyKeyToThePlan");

			

			if(jollyKeyToThePlanDialogueFailed == true )
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("JollyFailure"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					inConversation = false;
					updateJollysQuest();
					jollyKeyToThePlan = true;
					npcTalk->setCaption("");
				}
			}
			else if(jollyKeyToThePlanDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("JollyInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					npcTalk->setCaption("");
				}

				if(LevelLoaded == 4)
				{
					if(dialogueCounter == 1)
					{
						npcTalk->setCaption(quest->Attribute("JollyInitialDialogue02"));
						CellKey = 1;
						jollyKeyToThePlanDialogueCompleted = true;
						jollyKeyToThePlan = true;
					}
					else if(dialogueCounter == 2)
					{
						dialogueCounter = 0;
						inConversation = false;
						jollyKeyToThePlanDialogueCompleted = true;
						jollyKeyToThePlan = true;
						npcTalk->setCaption("");
					}
				}
			}
			else if(jollyKeyToThePlanDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					npcTalk->setCaption(quest->Attribute("JollyCompleted"));
				}
				else if(dialogueCounter == 2)
				{
					dialogueCounter = 0;
					jollyKeyToThePlanQuestCompleted = true;
					inConversation = false;
					updateJollysQuests = false;
					npcTalk->setCaption("");
				}
			}
			
			
		}
	}
	//else if(jollyKeyToThePlanQuestCompleted == false)
	//{
	//	//sets the flags accordingly//
	//	if(jollyKeyToThePlanDialogueCompleted == true && Soap != 1)
	//	{
	//		jollyKeyToThePlanDialogueFailed = true;
	//	}
	//	else if(jollyKeyToThePlanDialogueCompleted == true && Soap == 1)
	//	{
	//		jollyKeyToThePlanDialogueCompleted = true;
	//		jollyKeyToThePlanDialogueFailed = false;
	//	}
	//	
	//	//lin mas bug in the system quest//
	//	if(jollyKeyToThePlan == true)
	//	{
	//		TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("JollyKeyToThePlan");

	//		

	//		if(jollyKeyToThePlanDialogueFailed == true )
	//		{
	//			if(dialogueCounter == 1)
	//			{
	//				npcTalk->setCaption(quest->Attribute("JollyFailure"));
	//			}
	//			else if(dialogueCounter == 2)
	//			{
	//				dialogueCounter = 0;
	//				inConversation = false;
	//				updateJollysQuest();
	//				jollyKeyToThePlan = true;
	//				npcTalk->setCaption("");
	//			}
	//		}
	//		else if(jollyKeyToThePlanDialogueCompleted == false)
	//		{
	//			if(dialogueCounter == 1)
	//			{
	//				npcTalk->setCaption(quest->Attribute("JollyInitialDialogue01"));
	//			}
	//			else if(dialogueCounter == 2)
	//			{
	//				npcTalk->setCaption("");
	//			}

	//			if(LevelLoaded == 4)
	//			{
	//				if(dialogueCounter == 1)
	//				{
	//					npcTalk->setCaption(quest->Attribute("JollyInitialDialogue02"));
	//					CellKey = 1;
	//					jollyKeyToThePlanDialogueCompleted = true;
	//					jollyKeyToThePlan = true;
	//				}
	//				else if(dialogueCounter == 2)
	//				{
	//					dialogueCounter = 0;
	//					inConversation = false;
	//					jollyKeyToThePlanDialogueCompleted = true;
	//					jollyKeyToThePlan = true;
	//					npcTalk->setCaption("");
	//				}
	//			}
	//		}
	//		else if(jollyKeyToThePlanDialogueCompleted == true)
	//		{
	//			if(dialogueCounter == 1)
	//			{
	//				npcTalk->setCaption(quest->Attribute("JollyCompleted"));
	//			}
	//			else if(dialogueCounter == 2)
	//			{
	//				dialogueCounter = 0;
	//				jollyKeyToThePlanQuestCompleted = true;
	//				inConversation = false;
	//				updateJollysQuests = false;
	//				npcTalk->setCaption("");
	//			}
	//		}
	//		
	//		
	//	}
	//}
	//jolly knock em out quest//
	else if(jollyKnockEmOutQuestCompleted == false)
	{
		//quest dialogue for jolly knock em out//
		if(jollyKnockEmOut == true)
		{
			//picking up the quest///
			if(jollyKnockEmOutDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("JollyKnockEmOut");
					npcTalk->setCaption(quest->Attribute("JollyInitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("JollyKnockEmOut");
					npcTalk->setCaption(quest->Attribute("JollyInitialDialogue02"));
				}
				else if(dialogueCounter == 3)
				{
					inConversation = false;
					jollyKnockEmOutQuestCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			////if you complete the quest//
			else if(jollyKnockEmOutDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("JollyKnockEmOut");
					npcTalk->setCaption(quest->Attribute("JollyCompleted"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					updateJollysQuests = false;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			//if you fail the quest//
			else if(jollyKnockEmOutDialogueFailed == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("JollyKnockEmOut");
					npcTalk->setCaption(quest->Attribute("JollyFailure"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
		
		}
	}

}
void LevelTestState::updateQuest( void )
{
	//reads the xml document//
	TiXmlDocument loadDocOptions("QuestLog.xml");  
	loadDocOptions.LoadFile();

	if(johnny666RepQuestCompleted == false)
	{
		if(johnny666Rep == true)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny666Rep");
				npcTalk->setCaption(quest->Attribute("Johnny666InitialDialogue01"));
			}
			else if(dialogueCounter == 2)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny666Rep");
				npcTalk->setCaption(quest->Attribute("Johnny666InitialDialogue02"));
			}
			else if(dialogueCounter == 3)
			{
				inConversation = false;
				johnny666RepQuestCompleted = true;
				johnny666Rep = false;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}
		}
	}
	//johnny666 cheat a little//
	if(johnny666CheatALittleQuestCompleted == false)
	{
		//sets the flags accordingly//
		if(johnny666CheatALittleDialogueCompleted == true && Syringe != 1)
		{
			johnny666CheatALittleDialogueFailed = true;
		}
		else if(johnny666CheatALittleDialogueCompleted == true && Syringe == 1)
		{
			johnny666CheatALittleDialogueCompleted = true;
			johnny666CheatALittleDialogueFailed = false;
		}

		if(johnny666CheatALittle == true)
		{
			//if you fail the quest//
			if(johnny666CheatALittleDialogueFailed == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny666CheatALittle");
					npcTalk->setCaption(quest->Attribute("Johnny666Failure"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			else if(johnny666CheatALittleDialogueCompleted == false)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny666CheatALittle");
					npcTalk->setCaption(quest->Attribute("Johnny666InitialDialogue01"));
				}
				else if(dialogueCounter == 2)
				{
					inConversation = false;
					johnny666CheatALittleDialogueCompleted = true;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
			else if(johnny666CheatALittleDialogueCompleted == true)
			{
				if(dialogueCounter == 1)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny666CheatALittle");
					npcTalk->setCaption(quest->Attribute("Johnny666Completed"));
				}
				else if(dialogueCounter == 2)
				{
					TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny666CheatALittle");
					npcTalk->setCaption(quest->Attribute("Johnny666Completed02"));
				}
				else if(dialogueCounter == 3)
				{
					inConversation = false;
					johnny666CheatALittleQuestCompleted = true;
					johnny666CheatALittle = false;
					dialogueCounter = 0;
					npcTalk->setCaption("");
				}
			}
		}
	}
	//johnny666 80 rep//
	if(johnny66680RepQuestCompleted == false)
	{
		if(johnny66680Rep == true)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny66680Rep");
				npcTalk->setCaption(quest->Attribute("Johnny666InitialDialogue01"));
			}
			else if(dialogueCounter == 2)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny66680Rep");
				npcTalk->setCaption(quest->Attribute("Johnny666InitialDialogue02"));
			}
			else if(dialogueCounter == 3)
			{
				inConversation = false;
				johnny66680RepQuestCompleted = true;
				johnny66680Rep = false;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}
		}
	}
	else if(johnny666TrainingDoneQuestCompleted == false)
	{
		//Johnny 666 training//
		if(johnny666TrainingDone == true)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("Johnny666TrainingDone");
				npcTalk->setCaption(quest->Attribute("Johnny666InitialDialogue01"));
			}
			else if(dialogueCounter == 2)
			{
				inConversation = false;
				johnny666TrainingDoneQuestCompleted = true;
				johnny666TrainingDone = false;
				fight = true;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}
		}
	}


	//lin mas train //
	if(linMasTraining == true)
	{
		if(linMasTrainingNotCompleted == true)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("LinMaTrainNotCompleted");
				npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
			}
			else if(dialogueCounter == 2)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("LinMaTrainNotCompleted");
				npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue02"));
			}
			else if(dialogueCounter == 3)
			{
				inConversation = false;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}
		}

		if(linMasTrainingCompleted == true)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("LinMaTrainCompleted");
				npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
			}
			else if(dialogueCounter == 2)
			{
				inConversation = false;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}
		}
	}
	//lin mas final quest//
	else if(linMasFinalQuest == true)
	{
		if(linMasFinalQuestCompleted == false)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("LinMaFinalQuest");
				npcTalk->setCaption(quest->Attribute("LinMasInitialDialogue01"));
			}
			else if(dialogueCounter == 3)
			{
				inConversation = false;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}
		}

		if(linMasFinalQuestFailure == true)
		{
			if(dialogueCounter == 1)
			{
				TiXmlElement *quest = loadDocOptions.RootElement()->FirstChildElement("LinMaFinalQuest");
				npcTalk->setCaption(quest->Attribute("LinMasFailure"));
			}
			else if(dialogueCounter == 2)
			{
				inConversation = false;
				dialogueCounter = 0;
				npcTalk->setCaption("");
			}
		}
	}
}
void LevelTestState::unloadLevel( void )
{
	audio_result = audio_track->release();
	ERRCHECK(audio_result);
	audio_result = heart_Beat->release();
	ERRCHECK(audio_result);

	for(SceneNodeList::iterator it = mLoadCharacterSceneNodesLTS.begin(); it != mLoadCharacterSceneNodesLTS.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mLoadCharacterSceneNodesLTS.clear();

	for(SceneNodeList::iterator it = mItemsNodeLTS.begin(); it != mItemsNodeLTS.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mItemsNodeLTS.clear();

	//iterate through all scene nodes on stack and destroy them
	for(SceneNodeList::iterator it = mSceneNodesLTS.begin(); it != mSceneNodesLTS.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mSceneNodesLTS.clear();

	for(SceneNodeList::iterator it = mPlayerSceneNodesLTS.begin(); it != mPlayerSceneNodesLTS.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mPlayerSceneNodesLTS.clear();
	
	mSceneMgr->destroySceneNode("objectCollisionNodeLTS");
	mSceneMgr->destroySceneNode("mainLightNode");
	
	//iterate through all entities on stack and destroy them
	for(EntityList::iterator it = mEntitiesLTS.begin(); it != mEntitiesLTS.end(); it++)
	{
		Ogre::SceneNode* parent = (*it)->getParentSceneNode();
		if(parent)
		{
			parent->detachAllObjects();
		}
		mSceneMgr->destroyEntity(*it);
	}
	mEntitiesLTS.clear();
	
	for(EntityList::iterator it = mPlayerEntitiesLTS.begin(); it != mPlayerEntitiesLTS.end(); it++)
	{
		Ogre::SceneNode* parent = (*it)->getParentSceneNode();
		if(parent)
		{
			parent->detachAllObjects();
		}
		mSceneMgr->destroyEntity(*it);
	}
	mPlayerEntitiesLTS.clear();

	//items//
	for(EntityList::iterator it = mItemsLTS.begin(); it != mItemsLTS.end(); it++)
	{
		Ogre::SceneNode* parent = (*it)->getParentSceneNode();
		if(parent)
		{
			parent->detachAllObjects();
		}
		mSceneMgr->destroyEntity(*it);
	}
	mItemsLTS.clear();

	
}

void LevelTestState::initHUD( void )
{
	LevelTestStateOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
	LevelTestStateFontMgr = Ogre::FontManager::getSingletonPtr();

	HUDPanel = static_cast<OverlayContainer*>(LevelTestStateOverlayMgr->createOverlayElement("Panel", "CardsNumbers"));
	HUDPanel->setMetricsMode(Ogre::GMM_PIXELS);
	HUDPanel->setPosition(0, 0);

	x_AxisTextBox = static_cast<TextAreaOverlayElement*>(LevelTestStateOverlayMgr->createOverlayElement("TextArea", "Player1Name"));
	x_AxisTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	x_AxisTextBox->setPosition(10, 20);
	x_AxisTextBox->setFontName("MyFont");
	x_AxisTextBox->setCharHeight(30);
	x_AxisTextBox->setColour(Ogre::ColourValue(1.0, 0.0, 0.0, 1.0));
	//x_AxisTextBox->setCaption("X-Axis");

	y_AxisTextBox = static_cast<TextAreaOverlayElement*>(LevelTestStateOverlayMgr->createOverlayElement("TextArea", "Player1Money"));
	y_AxisTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	y_AxisTextBox->setPosition(10, 60);
	y_AxisTextBox->setFontName("MyFont");
	y_AxisTextBox->setCharHeight(30);
	y_AxisTextBox->setColour(Ogre::ColourValue(0.0, 1.0, 0.0, 1.0)); 
	//y_AxisTextBox->setCaption("Y-Axis");

	z_AxisTextBox = static_cast<TextAreaOverlayElement*>(LevelTestStateOverlayMgr->createOverlayElement("TextArea", "Player1Status"));
	z_AxisTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	z_AxisTextBox->setPosition(10, 100);
	z_AxisTextBox->setFontName("MyFont");
	z_AxisTextBox->setCharHeight(30);
	z_AxisTextBox->setColour(Ogre::ColourValue(0.0, 0.0, 1.0, 1.0)); 
	//z_AxisTextBox->setCaption("Z-Axis");

	otherTextBox = static_cast<TextAreaOverlayElement*>(LevelTestStateOverlayMgr->createOverlayElement("TextArea", "Player1BetAmount"));
	otherTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	otherTextBox->setPosition(10, 140);
	otherTextBox->setFontName("MyFont");
	otherTextBox->setCharHeight(30);
	otherTextBox->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0)); 
	//otherTextBox->setCaption("Test");

	npcTalk = static_cast<TextAreaOverlayElement*>(LevelTestStateOverlayMgr->createOverlayElement("TextArea", "npcDialogue"));
	npcTalk->setMetricsMode(Ogre::GMM_PIXELS);
	npcTalk->setPosition(223,555);
	npcTalk->setFontName("MyFont");
	npcTalk->setCharHeight(18);
	npcTalk->setColour(Ogre::ColourValue(Ogre::ColourValue::White));
	

	pickAFaction = static_cast<TextAreaOverlayElement*>(LevelTestStateOverlayMgr->createOverlayElement("TextArea", "pickAFaction"));
	pickAFaction->setMetricsMode(Ogre::GMM_PIXELS);
	pickAFaction->setPosition(50,384);
	pickAFaction->setFontName("MyFont");
	pickAFaction->setCharHeight(30);
	pickAFaction->setColour(Ogre::ColourValue(Ogre::ColourValue::White));


	detectObject = static_cast<TextAreaOverlayElement*>(LevelTestStateOverlayMgr->createOverlayElement("TextArea", "detectObject"));
	detectObject->setMetricsMode(Ogre::GMM_PIXELS);
	detectObject->setPosition(650,700);
	detectObject->setFontName("MyFont");
	detectObject->setCharHeight(20);
	detectObject->setColour(Ogre::ColourValue::White);

	yButtonTextBox = static_cast<TextAreaOverlayElement*>(LevelTestStateOverlayMgr->createOverlayElement("TextArea", "yButtonTextBox"));
	yButtonTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	yButtonTextBox->setPosition(770,615);
	yButtonTextBox->setFontName("MyFont");
	yButtonTextBox->setCharHeight(20);
	yButtonTextBox->setColour(Ogre::ColourValue::White);


	HUDPanel->addChild(x_AxisTextBox);
	HUDPanel->addChild(y_AxisTextBox);
	HUDPanel->addChild(z_AxisTextBox);
	HUDPanel->addChild(otherTextBox);
	HUDPanel->addChild(npcTalk);
	HUDPanel->addChild(pickAFaction);
	HUDPanel->addChild(detectObject);
	HUDPanel->addChild(yButtonTextBox);

	HUDOverlay = LevelTestStateOverlayMgr->create("BlankOverlay");
	HUDOverlay->add2D(HUDPanel);

	mFaceButtonOverlay = LevelTestStateOverlayMgr->getByName("Overlay/FaceButtons");
	mAButtonOverlay = LevelTestStateOverlayMgr->getByName("Overlay/AButton");
	mBButtonOverlay = LevelTestStateOverlayMgr->getByName("Overlay/BButton");
	mXButtonOverlay	= LevelTestStateOverlayMgr->getByName("Overlay/XButton");
	mYButtonOverlay	= LevelTestStateOverlayMgr->getByName("Overlay/YButton");


	textBox = LevelTestStateOverlayMgr->getByName("Overlay/TextBox");
	knockOut = LevelTestStateOverlayMgr->getByName("Overlay/KnockOut");
	Story01 = LevelTestStateOverlayMgr->getByName("Overlay/Story01");
	Story02 = LevelTestStateOverlayMgr->getByName("Overlay/Story02");

	Ogre::MaterialPtr fadeMaterial = Ogre::MaterialManager::getSingleton().getByName("KnockOut");
	Ogre::MaterialPtr fadeStoryMaterial01 = Ogre::MaterialManager::getSingleton().getByName("Story01");
	Ogre::MaterialPtr fadeStoryMaterial02 = Ogre::MaterialManager::getSingleton().getByName("Story02");

	fadeKnockout = fadeMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	fadeStory01 = fadeStoryMaterial01->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	fadeStory02 = fadeStoryMaterial02->getTechnique(0)->getPass(0)->getTextureUnitState(0);

		
	npcTalk->show();
	mFaceButtonOverlay->show();


	HUDOverlay->show();
	HUDOverlay->setZOrder(1000);
}
void LevelTestState::do_focus( void )
{
	//reset variables
	pauseMenuState = false;

	loadStats();
	loadMerchandise();
	saveInventory(); 

	mFaceButtonOverlay->show();


	//add listeners for this class
	mInputManager = InputManager::getSingletonPtr();
	mInputManager->addKeyListener(this, mClassName);
	mInputManager->addJoystickListener(this, mClassName);

	inConversation = false;
}

void LevelTestState::do_defocus( void )
{
	//reset variables
	keyForwardPress = false;
	keyBackwardPress = false;
	keyLeftPress = false;
	keyRightPress = false;
	pauseMenuState = true;

	mAButtonOverlay->hide();
	mBButtonOverlay->hide();
	mXButtonOverlay->hide();
	mYButtonOverlay->hide();
	mFaceButtonOverlay->hide();



	//remove listeners for this class
	mInputManager->removeKeyListener(this);
	mInputManager->removeJoystickListener(this);

	inConversation = true;
}

void LevelTestState::exit( void ) 
{
	//UNLOAD ALL  SOUNDS

	audio_result = audio_track->release();
	ERRCHECK(audio_result);

	audio_result = heart_Beat->release();
	ERRCHECK(audio_result);

	if(LevelLoaded == 0 || LevelLoaded == 1 || LevelLoaded == 4)
	{
		audio_result = cell_Door_Open->release();
		ERRCHECK(audio_result);
		audio_result = cell_Door_Close->release();
		ERRCHECK(audio_result);
	}

	if(LevelLoaded == 2)
	{
		audio_result = rat_Squished->release();
		ERRCHECK(audio_result);
	}

	audio_result = system->release();
	ERRCHECK(audio_result);

	audio_result = system->close();
	ERRCHECK(audio_result);

	////iterate through all scene nodes on stack and destroy them
	for(SceneNodeList::iterator it = mLoadCharacterSceneNodesLTS.begin(); it != mLoadCharacterSceneNodesLTS.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mLoadCharacterSceneNodesLTS.clear();

	for(SceneNodeList::iterator it = mItemsNodeLTS.begin(); it != mItemsNodeLTS.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mItemsNodeLTS.clear();

	//iterate through all scene nodes on stack and destroy them
	for(SceneNodeList::iterator it = mSceneNodesLTS.begin(); it != mSceneNodesLTS.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mSceneNodesLTS.clear();

	//iterate through all scene nodes on stack and destroy them
	for(SceneNodeList::iterator it = mPlayerSceneNodesLTS.begin(); it != mPlayerSceneNodesLTS.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mPlayerSceneNodesLTS.clear();

	mSceneMgr->destroySceneNode("objectCollisionNodeLTS");
	mSceneMgr->destroySceneNode("state5CameraNode");
	mSceneMgr->destroySceneNode("mainLightNode");
	
	//destroy lights in scene
	mSceneMgr->destroyLight("MainLight1");
	mSceneMgr->destroyLight("MainLight2");
	mSceneMgr->destroyLight("MainLight3");

	//iterate through all entities on stack and destroy them
	for(EntityList::iterator it = mEntitiesLTS.begin(); it != mEntitiesLTS.end(); it++)
	{
		Ogre::SceneNode* parent = (*it)->getParentSceneNode();
		if(parent)
		{
			parent->detachAllObjects();
		}
		mSceneMgr->destroyEntity(*it);
	}
	mEntitiesLTS.clear();

	for(EntityList::iterator it = mPlayerEntitiesLTS.begin(); it != mPlayerEntitiesLTS.end(); it++)
	{
		Ogre::SceneNode* parent = (*it)->getParentSceneNode();
		if(parent)
		{
			parent->detachAllObjects();
		}
		mSceneMgr->destroyEntity(*it);
	}
	mPlayerEntitiesLTS.clear();

	//items//
	for(EntityList::iterator it = mItemsLTS.begin(); it != mItemsLTS.end(); it++)
	{
		Ogre::SceneNode* parent = (*it)->getParentSceneNode();
		if(parent)
		{
			parent->detachAllObjects();
		}
		mSceneMgr->destroyEntity(*it);
	}
	mItemsLTS.clear();

	//Destroy overlay elements
	LevelTestStateOverlayMgr->destroyOverlayElement(HUDPanel);
	LevelTestStateOverlayMgr->destroyOverlayElement(otherTextBox);
	LevelTestStateOverlayMgr->destroyOverlayElement(z_AxisTextBox);
	LevelTestStateOverlayMgr->destroyOverlayElement(y_AxisTextBox);
	LevelTestStateOverlayMgr->destroyOverlayElement(x_AxisTextBox);
	LevelTestStateOverlayMgr->destroyOverlayElement(npcTalk);
	LevelTestStateOverlayMgr->destroyOverlayElement(pickAFaction);
	LevelTestStateOverlayMgr->destroyOverlayElement(detectObject);
	LevelTestStateOverlayMgr->destroyOverlayElement(yButtonTextBox);
	LevelTestStateOverlayMgr->destroy(HUDOverlay);

	//destroy camera
	mSceneMgr->destroyCamera(mCamera);
    mWindow->removeViewport(0);
}

void LevelTestState::wait( int seconds )
{
	clock_t endWait;
	
	endWait = clock () + seconds * CLOCKS_PER_SEC ;
	while(clock() < endWait) {}

}

void LevelTestState::update( Real seconds ) 
{
	// tick the time
	mTime += seconds;
	
	system->update();

	if(lPlayerHealth > 0.5*lHealthStat && changeInHealth)
	{
		if(LevelLoaded == 2){ speed = 10;}
		else { speed = 6;}
		changeInHealth = false;
		audio_result = heart_Beat->release();
		ERRCHECK(audio_result);
		audio_result = system->createSound("Sounds/SFX/Heartbeat Slow Loop.wav", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
		ERRCHECK(audio_result);

		if(levelsSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
			ERRCHECK(audio_result);
		}
	}
	else if(lPlayerHealth <= 0.5*lHealthStat && lPlayerHealth > 0.25*lHealthStat && changeInHealth)
	{
		if(LevelLoaded == 2){ speed = 10;}
		else { speed = 6;}
		changeInHealth = false;
		audio_result = heart_Beat->release();
		ERRCHECK(audio_result);
		audio_result = system->createSound("Sounds/SFX/Heartbeat Medium Loop.wav", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
		ERRCHECK(audio_result);

		if(levelsSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
			ERRCHECK(audio_result);
		}
	}
	else if(lPlayerHealth <= 0.25*lHealthStat && changeInHealth)
	{
		if(LevelLoaded == 2){ speed = 10;}
		else { speed = 6;}
		changeInHealth = false;
		audio_result = heart_Beat->release();
		ERRCHECK(audio_result);
		audio_result = system->createSound("Sounds/SFX/Heartbeat Fast Loop.wav", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &heart_Beat);
		ERRCHECK(audio_result);

		if(levelsSFXSetting)
		{
			audio_result = system->playSound(FMOD_CHANNEL_FREE, heart_Beat, false, &channel3);
			ERRCHECK(audio_result);
		}
	}

	if(pauseMenuState == true)
	{
		mAButtonOverlay->hide();
		mBButtonOverlay->hide();
		mXButtonOverlay->hide();
		mYButtonOverlay->hide();
	}
	else if(pauseMenuState == false)
	{
		mAButtonOverlay->hide();
		mBButtonOverlay->hide();
		mXButtonOverlay->hide();
		mYButtonOverlay->hide();

		if(checkObjectDetection())
		{
			mAButtonOverlay->show();
		}
		
		// Animate robots
		robot_anim1LTS->addTime( seconds );
		
		//Check doors and open each one accordingly
		if(LevelLoaded == 0)
		{
			Story01->show();
			Story02->show();
			if(story01Continue == true)
			{
				story01FadeVar -= 0.03;
				fadeStory01->setAlphaOperation(LBX_MODULATE, Ogre::LBS_TEXTURE, Ogre::LBS_MANUAL, 1.0, story01FadeVar);
			}
			
			if(story02Continue == true)
			{
				story02FadeVar -= 0.03;
				fadeStory02->setAlphaOperation(LBX_MODULATE, Ogre::LBS_TEXTURE, Ogre::LBS_MANUAL, 1.0, story02FadeVar);
			}

			

			if(linMasPickAFactionDialogueCompleted == true)
			{
				batonGrab->setEnabled(true);
				batonGrab->addTime( seconds );
				if(batonGrab->hasEnded())
				{
					batonGrab->setEnabled(false);
					batonHit->setEnabled(true);
					batonHit->addTime( seconds );
				}

				if(batonHit->hasEnded())
				{
					if(!batonSndPlayed)
					{
						if(levelsSFXSetting)
						{
							audio_result = system->playSound(FMOD_CHANNEL_FREE, baton_Smack, false, &channel2);
							ERRCHECK(audio_result);
						}

						batonSndPlayed = true;
					}
					knockOut->show();
					knockOutFadeVar += 0.03;
					fadeKnockout->setAlphaOperation(LBX_MODULATE, Ogre::LBS_TEXTURE, Ogre::LBS_MANUAL, 1.0, knockOutFadeVar);
					if(knockOutFadeVar >= 1)
					{
						detectObject->setCaption("");
						wait(3);
						unloadLevel();
						loadCellsLevel();
					}
				}
			}

			
		}
		else if( LevelLoaded == 1)
		{ 

			
			if(knockOutFadeVar != 0)
			{
				knockOutFadeVar -= 0.01;
				fadeKnockout->setAlphaOperation(LBX_MODULATE, Ogre::LBS_TEXTURE, Ogre::LBS_MANUAL, 1.0, knockOutFadeVar);
			}
			
			
			santoso_Idle->addTime(seconds);
			johnny666_Idle->addTime(seconds);
			linMaSit->addTime(seconds);
			dannyBoy_Idle->addTime(seconds);

			openDoor(); 
			rotateCamera();
			checkLoadCharacterDetection();
		}
		else if(LevelLoaded == 2)
		{
			jolly_Idle->addTime(seconds);
			micheal_Idle->addTime(seconds);

			AI_anim1LTS->addTime( seconds );
			AI_anim2LTS->addTime( seconds );
			AI_anim3LTS->addTime( seconds );
			AI_anim4LTS->addTime( seconds );
			AI_anim5LTS->addTime( seconds );

			if(AI_Entity1LTS->isVisible())
			{
				updateAIRat1();
			}
			if(AI_Entity2LTS->isVisible())
			{
				updateAIRat2();
			}
			if(AI_Entity3LTS->isVisible())
			{
				updateAIRat3();
			}
			if(AI_Entity4LTS->isVisible())
			{
				updateAIRat4();
			}
			if(AI_Entity5LTS->isVisible())
			{
				updateAIRat5();
			}
		}
		else if(LevelLoaded == 3)
		{
			dougieTerrance_Idle->addTime(seconds);
			terranceDougie_Idle->addTime(seconds);
			johnny666_Idle->addTime(seconds);
		}
		else if(LevelLoaded == 4)
		{
			updateGuardAI( seconds );
			openDoor(); 
		}

		if(inConversation == true)
		{
			//updates the quests and dialogue//
			updateQuest();
			textBox->show();

			//finding rotation//
			if(pitchLTS < 0 )
			{
				if(keyboardLookInvertedLTS){ mCamera->pitch(Degree(1)); pitchLTS++;}
				else if(!keyboardLookInvertedLTS){ mCamera->pitch(Degree(-1)); pitchLTS++;}
			}
			else if(pitchLTS > 0)
			{
				if(keyboardLookInvertedLTS){ mCamera->pitch(Degree(-1)); pitchLTS--;}
				else if(!keyboardLookInvertedLTS){ mCamera->pitch(Degree(1)); pitchLTS--;}
			}
		}
		else if(inConversation == false)
		{
			//Check which controls are being used
			if(keyboardControlsActiveLTS)		{ keyboardMouseCharacterMovement(); }
			else if(gamepadControlsActiveLTS)	{ gamepadCharacterMovement( seconds ); }
			textBox->hide();
		}

		if(updateLinMasQuests == true)
		{
			updateLinMasQuest();
		}
		else if(updateJollysQuests == true)
		{
			updateJollysQuest();
		}
		else if(updateSantosoQuests == true)
		{
			updateSantosoQuest();
		}
		else if(updateGeminiQuests == true)
		{
			updateGeminiQuest();
		}

		if(mInputManager->isButtonDown(0,0) && gamepadReleased == true) // Pick-up Items
		{
			if(LevelLoaded == 0)
			{
				checkPlayerCollision();
			}
			else if(LevelLoaded == 1)
			{
				checkObjectCollision();
				checkPlayerCollision();
				checkItemCollision();
			}
			else if(LevelLoaded == 2)
			{
				checkObjectCollision();
				checkPlayerCollision();
				checkItemCollision();
			}
			else if(LevelLoaded == 3)
			{
				checkObjectCollision();
				checkPlayerCollision();
				checkItemCollision();
			}
			else if(LevelLoaded == 4)
			{
				checkObjectCollision();
				checkPlayerCollision();
				checkItemCollision();
			}

			gamepadReleased = false;

			if(inConversation == true)
			{
				no = true;
			}
		}
		else if(mInputManager->isButtonDown(0,1) && gamepadReleased == true)
		{
			gamepadReleased = false;
			checkObjectCollision();
			checkPlayerCollision();

			if(inConversation == true && dialogueShuffle == true)
			{
				dialogueShuffle = false;
				dialogueCounter++;
			}
		}
		else if(mInputManager->isButtonDown(0,3) && gamepadReleased == true) // Use Items
		{
			gamepadReleased = false;
		}
		else if(mInputManager->isButtonDown(0,8) && gamepadReleased == true) //Cycle through items
		{
			gamepadReleased = false;
		}
		else if(mInputManager->isButtonDown(0, 11) && gamepadReleased == true)
		{
			changeStance = true;
			gamepadReleased = false;
		}
		else if(!mInputManager->isButtonDown(0,0) && !mInputManager->isButtonDown(0,1) && !mInputManager->isButtonDown(0,8) && !mInputManager->isButtonDown(0,11) )
		{
			changeStance = false;
			gamepadReleased = true;

			if(inConversation == true)
			{
				dialogueShuffle = true;
			}
		}

		/*if(fight = true)
		{
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("SampleWork5"));
				GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
		}*/

		

		if(mInputManager->isButtonDown(0, 9) && stickReleased == true)
		{	
			//updates the xml info everytime the inventory is accesed//
			saveInventory(); 

			stickReleased = false;
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("PauseMenu"));
		}
		else if(!mInputManager->isButtonDown(0,9))
		{
			stickReleased = true;
		}

		//// Set captions of text boxes
		//x_AxisTextBox->setCaption(itoa(objectCollisionNodeLTS->getWorldPosition().x, bufferLTS, 10));
		////y_AxisTextBox->setCaption(itoa(PlayerNodeLTS->getWorldPosition().y, bufferLTS, 10));
		//z_AxisTextBox->setCaption(itoa(objectCollisionNodeLTS->getWorldPosition().z, bufferLTS, 10));
	/*	
		if(LevelLoaded == 0 )		{ otherTextBox->setCaption("InitialCells"); }
		else if(LevelLoaded == 1)	{ otherTextBox->setCaption("Cells DayTime"); }
		else if(LevelLoaded == 2)	{ otherTextBox->setCaption("Cafeteria"); }
		else if(LevelLoaded == 3)	{ otherTextBox->setCaption("Courtyard"); }
		else if(LevelLoaded == 4)	{ otherTextBox->setCaption("Cells NightTime"); }*/

		mCamera->setPosition(PlayerNodeLTS->getWorldPosition().x, PlayerNodeLTS->getWorldPosition().y + charHeight,PlayerNodeLTS->getWorldPosition().z);
		
		if(johnny666TrainingDoneQuestCompleted == true)
		{
			textBox->hide();
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("SampleWork5"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
		}	
	}
}
void LevelTestState::updateGuardAI( Real seconds )
{
	if(currentGuardNode == 1)
	{
		guardNPCNode->setAutoTracking(true,cellsWalkNode2,Vector3::UNIT_Z);
		guardNPCNode->setFixedYawAxis(true);
		moveAIForward(guardNPCNode,2.5);
		
		if( guardNPCNode->getWorldPosition().x + 2 > cellsWalkNode2->getWorldPosition().x   )
		{
			if( guardNPCNode->getWorldPosition().x  - 2 < cellsWalkNode2->getWorldPosition().x )
			{
				if( guardNPCNode->getWorldPosition().z  + 2 > cellsWalkNode2->getWorldPosition().z )
				{
					if( guardNPCNode->getWorldPosition().z  - 2 < cellsWalkNode2->getWorldPosition().z )
					{
						currentGuardNode = 2;
					}
				}
			}
		}
	}
	else if(currentGuardNode == 2)
	{
		guardNPCNode->setAutoTracking(true,cellsWalkNode3,Vector3::UNIT_Z);
		guardNPCNode->setFixedYawAxis(true);
		moveAIForward(guardNPCNode,2.5);
		
		if( guardNPCNode->getWorldPosition().x + 2 > cellsWalkNode3->getWorldPosition().x   )
		{
			if( guardNPCNode->getWorldPosition().x  - 2 < cellsWalkNode3->getWorldPosition().x )
			{
				if( guardNPCNode->getWorldPosition().z  + 2 > cellsWalkNode3->getWorldPosition().z )
				{
					if( guardNPCNode->getWorldPosition().z  - 2 < cellsWalkNode3->getWorldPosition().z )
					{
						currentGuardNode = 3;
					}
				}
			}
		}
	}
	else if(currentGuardNode == 3)
	{
		guardNPCNode->setAutoTracking(true,cellsWalkNode4,Vector3::UNIT_Z);
		guardNPCNode->setFixedYawAxis(true);
		moveAIForward(guardNPCNode,2.5);
		
		if( guardNPCNode->getWorldPosition().x + 2 > cellsWalkNode4->getWorldPosition().x   )
		{
			if( guardNPCNode->getWorldPosition().x  - 2 < cellsWalkNode4->getWorldPosition().x )
			{
				if( guardNPCNode->getWorldPosition().z  + 2 > cellsWalkNode4->getWorldPosition().z )
				{
					if( guardNPCNode->getWorldPosition().z  - 2 < cellsWalkNode4->getWorldPosition().z )
					{
						currentGuardNode = 4;
					}
				}
			}
		}
	}
	else if(currentGuardNode == 4)
	{
		guardNPCNode->setAutoTracking(true,cellsWalkNode1,Vector3::UNIT_Z);
		guardNPCNode->setFixedYawAxis(true);
		moveAIForward(guardNPCNode,2.5);
		
		if( guardNPCNode->getWorldPosition().x + 2 > cellsWalkNode1->getWorldPosition().x   )
		{
			if( guardNPCNode->getWorldPosition().x  - 2 < cellsWalkNode1->getWorldPosition().x )
			{
				if( guardNPCNode->getWorldPosition().z  + 2 > cellsWalkNode1->getWorldPosition().z )
				{
					if( guardNPCNode->getWorldPosition().z  - 2 < cellsWalkNode1->getWorldPosition().z )
					{
						currentGuardNode = 1;
					}
				}
			}
		}
	}
	
	guardWalk->addTime( seconds );

}
void LevelTestState::updateAIRat1()
{
	if(AI1MoveDuration < AI1MaxMoveDuration )
	{
		if(AI1MoveDir == 1) 
		{
			moveAIForward( AI_Node1LTS,1 ); 
		}
		else if(AI1MoveDir == 2) 
		{
			turnAIAround( AI_Node1LTS );

			srand( time(NULL) * 2);
			randomNum = ( rand() % 19 ) + 1;
			
			if(randomNum <= 10)
			{
				AI1MoveDir = 1;
				AI1MaxMoveDuration = ( rand() % 50 ) + 10;
			}
			else if(randomNum >=11 && randomNum <=14)
			{
				AI1MoveDir = 3;
				AI1MoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum >= 15 && randomNum <= 18)
			{
				AI1MoveDir = 4;
				AI1MaxMoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum == 19 )
			{
				AI1MoveDir = 0;
				AI1MaxMoveDuration = 100;
			}

			AI1MoveDuration = 0;
		}
		else if(AI1MoveDir == 3) 
		{
			moveAILeft( AI_Node1LTS,1 ); 
		}
		else if(AI1MoveDir == 4) 
		{
			moveAIRight( AI_Node1LTS,1 ); 
		}
		
		AI1MoveDuration++;


		Vector3 forwardResult_1;

		if(RaycastFromPointLTS(AI1CollisionLTS, AI_Entity1LTS, AI_Node1LTS->getPosition(), AI_Node1LTS->getOrientation() * (Vector3::UNIT_Z), forwardResult_1))
		{
			Vector3 forwardDistance = AI_Node1LTS->getPosition() - forwardResult_1;
			if(forwardDistance.length() < 20.0f )
			{
				AI1MoveDir = 2;
			}
		}
	}
	else if(AI1MoveDuration == AI1MaxMoveDuration )
	{
		srand( time(NULL) * 2);
		randomNum = ( rand() % 20 ) + 1;
		if(randomNum <= 10)
		{
			AI1MoveDir = 1;
			AI1MaxMoveDuration = ( rand() % 50 ) + 10;
		}
		else if(randomNum >=11 && randomNum <=14)
		{
			AI1MoveDir = 3;
			AI1MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum >= 15 && randomNum <= 18)
		{
			AI1MoveDir = 4;
			AI1MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum >= 19 )
		{
			AI1MoveDir = 0;
			AI1MaxMoveDuration = 100;
		}
		
		AI1MoveDuration = 0;
	}
}

void LevelTestState::updateAIRat2()
{
	if(AI2MoveDuration < AI2MaxMoveDuration )
	{
		if(AI2MoveDir == 1) 
		{
			moveAIForward( AI_Node2LTS,1 ); 
		}
		else if(AI2MoveDir == 2) 
		{
			turnAIAround( AI_Node2LTS );

			srand( time(NULL) *4);
			randomNum = ( rand() % 19 ) + 1;
			
			if(randomNum <= 10)
			{
				AI2MoveDir = 1;
				AI2MaxMoveDuration = ( rand() % 50 ) + 10;
			}
			else if(randomNum >=11 && randomNum <=14)
			{
				AI2MoveDir = 3;
				AI2MoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum >= 15 && randomNum <= 18)
			{
				AI2MoveDir = 4;
				AI2MaxMoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum == 19 )
			{
				AI2MoveDir = 0;
				AI2MaxMoveDuration = 100;
			}

			AI2MoveDuration = 0;
		}
		else if(AI2MoveDir == 3) 
		{
			moveAILeft( AI_Node2LTS,1 ); 
		}
		else if(AI2MoveDir == 4) 
		{
			moveAIRight( AI_Node2LTS,1 ); 
		}
		
		AI2MoveDuration++;


		Vector3 forwardResult_1;

		if(RaycastFromPointLTS(AI2CollisionLTS, AI_Entity2LTS, AI_Node2LTS->getPosition(), AI_Node2LTS->getOrientation() * (Vector3::UNIT_Z), forwardResult_1))
		{
			Vector3 forwardDistance = AI_Node2LTS->getPosition() - forwardResult_1;
			if(forwardDistance.length() < 20.0f )
			{
				AI2MoveDir = 2;
			}
		}
	}
	else if(AI2MoveDuration == AI2MaxMoveDuration )
	{
		srand( time(NULL) *4);
		randomNum = ( rand() % 20 ) + 1;
		if(randomNum <= 10)
		{
			AI2MoveDir = 1;
			AI2MaxMoveDuration = ( rand() % 50 ) + 10;
		}
		else if(randomNum >=11 && randomNum <=14)
		{
			AI2MoveDir = 3;
			AI2MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum >= 15 && randomNum <= 18)
		{
			AI2MoveDir = 4;
			AI2MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum >= 19 )
		{
			AI2MoveDir = 0;
			AI2MaxMoveDuration = 100;
		}
		
		AI2MoveDuration = 0;
	}
}

void LevelTestState::updateAIRat3()
{
	if(AI3MoveDuration < AI3MaxMoveDuration )
	{
		if(AI3MoveDir == 1) 
		{
			moveAIForward( AI_Node3LTS,0.3 ); 
		}
		else if(AI3MoveDir == 2) 
		{
			turnAIAround( AI_Node3LTS );

			srand( time(NULL) );
			randomNum = ( rand() % 19 ) + 1;
			
			if(randomNum <= 10)
			{
				AI3MoveDir = 1;
				AI3MaxMoveDuration = ( rand() % 50 ) + 10;
			}
			else if(randomNum >=11 && randomNum <=14)
			{
				AI3MoveDir = 3;
				AI3MoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum >= 15 && randomNum <= 18)
			{
				AI3MoveDir = 4;
				AI3MaxMoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum == 19 )
			{
				AI3MoveDir = 0;
				AI3MaxMoveDuration = 100;
			}

			AI3MoveDuration = 0;
		}
		else if(AI3MoveDir == 3) 
		{
			moveAILeft( AI_Node3LTS,0.3 ); 
		}
		else if(AI3MoveDir == 4) 
		{
			moveAIRight( AI_Node3LTS,0.3 ); 
		}
		
		AI3MoveDuration++;


		Vector3 forwardResult_1;

		if(RaycastFromPointLTS(AI3CollisionLTS, AI_Entity3LTS, AI_Node3LTS->getPosition(), AI_Node3LTS->getOrientation() * (Vector3::UNIT_Z), forwardResult_1))
		{
			Vector3 forwardDistance = AI_Node3LTS->getPosition() - forwardResult_1;
			if(forwardDistance.length() < 20.0f )
			{
				AI3MoveDir = 2;
			}
		}
	}
	else if(AI3MoveDuration == AI3MaxMoveDuration )
	{
		srand( time(NULL) );
		randomNum = ( rand() % 20 ) + 1;
		if(randomNum <= 10)
		{
			AI3MoveDir = 1;
			AI3MaxMoveDuration = ( rand() % 50 ) + 10;
		}
		else if(randomNum >=11 && randomNum <=14)
		{
			AI3MoveDir = 3;
			AI3MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum >= 15 && randomNum <= 18)
		{
			AI3MoveDir = 4;
			AI3MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum >= 19 )
		{
			AI3MoveDir = 0;
			AI3MaxMoveDuration = 100;
		}
		
		AI3MoveDuration = 0;
	}
}

void LevelTestState::updateAIRat4()
{
	if(AI4MoveDuration < AI4MaxMoveDuration )
	{
		if(AI4MoveDir == 1) 
		{
			moveAIForward( AI_Node4LTS, 0.3 ); 
		}
		else if(AI4MoveDir == 2) 
		{
			turnAIAround( AI_Node4LTS );

			srand( time(NULL) );
			randomNum = ( rand() % 19 ) + 1;
			
			if(randomNum <= 10)
			{
				AI4MoveDir = 1;
				AI4MaxMoveDuration = ( rand() % 50 ) + 10;
			}
			else if(randomNum >=11 && randomNum <=14)
			{
				AI4MoveDir = 3;
				AI4MoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum >= 15 && randomNum <= 18)
			{
				AI4MoveDir = 4;
				AI4MaxMoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum == 19 )
			{
				AI4MoveDir = 0;
				AI4MaxMoveDuration = 100;
			}

			AI4MoveDuration = 0;
		}
		else if(AI4MoveDir == 3) 
		{
			moveAILeft( AI_Node4LTS, 0.3 ); 
		}
		else if(AI3MoveDir == 4) 
		{
			moveAIRight( AI_Node4LTS, 0.3 ); 
		}
		
		AI4MoveDuration++;


		Vector3 forwardResult_1;

		if(RaycastFromPointLTS(AI4CollisionLTS, AI_Entity4LTS, AI_Node4LTS->getPosition(), AI_Node4LTS->getOrientation() * (Vector3::UNIT_Z), forwardResult_1))
		{
			Vector3 forwardDistance = AI_Node4LTS->getPosition() - forwardResult_1;
			if(forwardDistance.length() < 20.0f )
			{
				AI4MoveDir = 2;
			}
		}
	}
	else if(AI4MoveDuration == AI4MaxMoveDuration )
	{
		srand( time(NULL) );
		randomNum = ( rand() % 20 ) + 1;
		if(randomNum <= 10)
		{
			AI4MoveDir = 1;
			AI4MaxMoveDuration = ( rand() % 50 ) + 10;
		}
		else if(randomNum >=11 && randomNum <=14)
		{
			AI4MoveDir = 3;
			AI4MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum >= 15 && randomNum <= 18)
		{
			AI4MoveDir = 4;
			AI4MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum == 19 )
		{
			AI4MoveDir = 2;
			AI4MaxMoveDuration = 1;
		}
		else if(randomNum == 20 )
		{
			AI4MoveDir = 0;
			AI4MaxMoveDuration = 100;
		}
		
		AI4MoveDuration = 0;
	}
}

void LevelTestState::updateAIRat5()
{
	if(AI5MoveDuration < AI5MaxMoveDuration )
	{
		if(AI5MoveDir == 1) 
		{
			moveAIForward( AI_Node5LTS, 0.3 ); 
		}
		else if(AI5MoveDir == 2) 
		{
			turnAIAround( AI_Node5LTS );

			srand( time(NULL) );
			randomNum = ( rand() % 19 ) + 1;
			
			if(randomNum <= 10)
			{
				AI5MoveDir = 1;
				AI5MaxMoveDuration = ( rand() % 50 ) + 10;
			}
			else if(randomNum >=11 && randomNum <=14)
			{
				AI5MoveDir = 3;
				AI5MoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum >= 15 && randomNum <= 18)
			{
				AI5MoveDir = 4;
				AI5MaxMoveDuration = ( rand() % 20 ) + 10;
			}
			else if(randomNum == 19 )
			{
				AI5MoveDir = 0;
				AI5MaxMoveDuration = 100;
			}

			AI5MoveDuration = 0;
		}
		else if(AI5MoveDir == 3) 
		{
			moveAILeft( AI_Node4LTS, 0.3 ); 
		}
		else if(AI5MoveDir == 4) 
		{
			moveAIRight( AI_Node4LTS, 0.3 ); 
		}
		
		AI5MoveDuration++;


		Vector3 forwardResult_1;

		if(RaycastFromPointLTS(AI5CollisionLTS, AI_Entity5LTS, AI_Node5LTS->getPosition(), AI_Node5LTS->getOrientation() * (Vector3::UNIT_Z), forwardResult_1))
		{
			Vector3 forwardDistance = AI_Node5LTS->getPosition() - forwardResult_1;
			if(forwardDistance.length() < 20.0f )
			{
				AI5MoveDir = 2;
			}
		}
	}
	else if(AI5MoveDuration == AI5MaxMoveDuration )
	{
		srand( time(NULL) );
		randomNum = ( rand() % 20 ) + 1;
		if(randomNum <= 10)
		{
			AI5MoveDir = 1;
			AI5MaxMoveDuration = ( rand() % 50 ) + 10;
		}
		else if(randomNum >=11 && randomNum <=14)
		{
			AI5MoveDir = 3;
			AI5MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum >= 15 && randomNum <= 18)
		{
			AI5MoveDir = 4;
			AI5MaxMoveDuration = ( rand() % 20 ) + 10;
		}
		else if(randomNum == 19 )
		{
			AI5MoveDir = 2;
			AI5MaxMoveDuration = 1;
		}
		else if(randomNum == 20 )
		{
			AI5MoveDir = 0;
			AI5MaxMoveDuration = 100;
		}
		
		AI5MoveDuration = 0;
	}
}

void LevelTestState::moveAIForward( Ogre::SceneNode *node,float speed )
{
	node->translate(node->getOrientation()*(Vector3::UNIT_Z*speed));
}
void LevelTestState::moveAIRight( Ogre::SceneNode *node,float speed )
{
	node->translate(node->getOrientation()*(Vector3::UNIT_Z*speed));
	node->rotate(Vector3::UNIT_Y, Degree(-2));
}
void LevelTestState::moveAILeft( Ogre::SceneNode *node,float speed )
{
	node->translate(node->getOrientation()*(Vector3::UNIT_Z*speed));
	node->rotate(Vector3::UNIT_Y, Degree(2));
}
void LevelTestState::turnAIAround( Ogre::SceneNode *node )
{
	node->rotate(Vector3::UNIT_Y, Degree(180));
}
void LevelTestState::keyboardMouseCharacterMovement()
{
	//updates the orientation of the collision nodes so they face the right way//
	forwardCollisionNodeLTS->setOrientation(PlayerNodeLTS->getOrientation());
	backwardCollisionNodeLTS->setOrientation(PlayerNodeLTS->getOrientation());
	leftCollisionNodeLTS->setOrientation(PlayerNodeLTS->getOrientation());
	rightCollisionNodeLTS->setOrientation(PlayerNodeLTS->getOrientation());

	//collision vectors//
	Vector3 forwardResult_1;
	Vector3 forwardResult_2;
	Vector3 backwardResult_1;
	Vector3 backwardResult_2;
	Vector3 leftResult_1;
	Vector3 leftResult_2;
	Vector3 rightResult_1;
	Vector3 rightResult_2;

	const OIS::MouseState &ms = mInputManager->getMouse()->getMouseState();

	mouseMoveX = Ogre::Degree(-ms.X.rel * 0.13);
	mouseMoveY = Ogre::Degree(-ms.Y.rel * 0.13);

	if(shirt == 1){Ogre::Entity* temp = ShirtEntityLTS;}
	else{Ogre::Entity* temp = ShirtlessEntityLTS; }

	//finding rotation//
	if(mouseMoveY.operator < (Radian(0)) && pitchLTS < 45 )
	{
		if(keyboardLookInvertedLTS){ mCamera->pitch(Degree(1)); pitchLTS++;}
		else if(!keyboardLookInvertedLTS){ mCamera->pitch(Degree(-1)); pitchLTS++;}
	}
	else if(mouseMoveY.operator > (Radian(0)) && pitchLTS > -45)
	{
		if(keyboardLookInvertedLTS){ mCamera->pitch(Degree(-1)); pitchLTS--;}
		else if(!keyboardLookInvertedLTS){ mCamera->pitch(Degree(1)); pitchLTS--;}
	}

	if(ms.buttonDown(OIS::MouseButtonID(0)) && !shiftPressedLTS)
	{
		playerLeftPunchLTS = true;
	}
	else if(ms.buttonDown(OIS::MouseButtonID(1)) && !shiftPressedLTS)
	{
		playerRightPunchLTS = true;
	}
	else if(ms.buttonDown(OIS::MouseButtonID(0)) && shiftPressedLTS)
	{
		playerLeftKickLTS = true;
	}
	else if(ms.buttonDown(OIS::MouseButtonID(1)) && shiftPressedLTS)
	{
		playerRightKickLTS = true;
	}
	
	if(playerLeftPunchLTS)
	{
		if(setAnimationLTS == false)
		{
			// Set animation
			robot_anim1LTS->setLoop( false );
			robot_anim1LTS->setEnabled( false );
			if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "leftPunch" );}
			else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "leftPunch" );}
			robot_anim1LTS->setTimePosition(0);
			robot_anim1LTS->setLoop( false );
			robot_anim1LTS->setEnabled( true );
			setAnimationLTS = true;
			/*if(combatSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
				ERRCHECK(audio_result);
			}*/
		}
		
		if(robot_anim1LTS->hasEnded())
		{
			playerLeftPunchLTS = false;
			setAnimationLTS = false;

			checkAICollision();
		}
	}
	else if(playerRightPunchLTS)
	{
		if(setAnimationLTS == false)
		{
			// Set animation
			robot_anim1LTS->setLoop( false );
			robot_anim1LTS->setEnabled( false );
			if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rightPunch" );}
			else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rightPunch" );}
			robot_anim1LTS->setTimePosition(0);
			robot_anim1LTS->setLoop( false );
			robot_anim1LTS->setEnabled( true );
			setAnimationLTS = true;
			//PLAYS THE SOUND FOR A MISS HIT
			/*if(combatSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
				ERRCHECK(audio_result);
			}*/
		}

		if(robot_anim1LTS->hasEnded())
		{
			playerRightPunchLTS = false;
			setAnimationLTS = false;
			checkAICollision();
		}
	}
	else if(playerLeftKickLTS)
	{
		if(setAnimationLTS == false)
		{
			// Set animation
			robot_anim1LTS->setLoop( false );
			robot_anim1LTS->setEnabled( false );
			if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rightKick" );}
			else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rightKick" );}
			robot_anim1LTS->setTimePosition(0);
			robot_anim1LTS->setLoop( false );
			robot_anim1LTS->setEnabled( true );
			setAnimationLTS = true;
			//PLAYS THE SOUND FOR A MISS KICK
			/*if(combatSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
				ERRCHECK(audio_result);
			}*/
		}

		if(robot_anim1LTS->hasEnded())
		{
			playerLeftKickLTS = false;
			setAnimationLTS = false;
			checkAICollision();
		}
	}
	else if(playerRightKickLTS)
	{
		if(setAnimationLTS == false)
		{
			// Set animation
			robot_anim1LTS->setLoop( false );
			robot_anim1LTS->setEnabled( false );
			if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "leftKick" );}
			else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "leftKick" );}
			robot_anim1LTS->setTimePosition(0);
			robot_anim1LTS->setLoop( false );
			robot_anim1LTS->setEnabled( true );
			setAnimationLTS = true;
			//PLAYS THE SOUND FOR A MISS KICK
			/*if(combatSFXSetting)
			{
				audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
				ERRCHECK(audio_result);
			}*/
		}


		if(robot_anim1LTS->hasEnded())
		{
			playerRightKickLTS = false;
			setAnimationLTS = false;
			checkAICollision();
		}
	}
	else if(keyForwardPress || keyBackwardPress || keyRightPress || keyLeftPress )
	{
		//forward//
		if(keyForwardPress == true)
		{
			if(forwardCollisionCheckLTS(temp,PlayerNodeLTS,forwardCollisionNodeLTS) == true)
			{
				mMoveScale = speed;
				mTranslateVector.z = mMoveScale;
			}
		}
		
		//backward//
		if(keyBackwardPress == true)
		{
			if(backwardCollisionCheckLTS(temp,PlayerNodeLTS,backwardCollisionNodeLTS) == true)
			{
				mMoveScale = speed;
				mTranslateVector.z = -mMoveScale;
			}
		}
		
		//left//
		if(keyLeftPress == true)
		{
			if(leftCollisionCheckLTS(temp,PlayerNodeLTS,leftCollisionNodeLTS) == true)
			{
				mMoveScale = speed;
				mTranslateVector.x = mMoveScale;
			}
		}
		
		//right//
		if(keyRightPress == true)
		{
			if(rightCollisionCheckLTS(temp,PlayerNodeLTS,rightCollisionNodeLTS) == true)
			{
				mMoveScale = speed;	
				mTranslateVector.x = -mMoveScale;
			}
		}
		
		// Set animation
		robot_anim1LTS->setLoop( false);
		robot_anim1LTS->setEnabled( false );
		if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "walk" );}
		else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "walk" );}
		robot_anim1LTS->setLoop( true );
		robot_anim1LTS->setEnabled( true );
	}
	else
	{
		// Set animation
		robot_anim1LTS->setLoop( false);
		robot_anim1LTS->setEnabled( false );
		if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rest" );}
		else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rest" );}
		robot_anim1LTS->setLoop( true );
		robot_anim1LTS->setEnabled( true );
	}
	
	// move character
	PlayerNodeLTS->translate(PlayerNodeLTS->getWorldOrientation() * mTranslateVector, Ogre::SceneNode::TS_WORLD);
	
	//Yaws the camera and character according to the mouse relative movement//
	mCamera->yaw(mouseMoveX);
	PlayerNodeLTS->yaw(mouseMoveX);

	//reset translate vector
	mTranslateVector = Vector3::ZERO;

}
void LevelTestState::gamepadCharacterMovement( Real seconds )
{
	// Retrieve joystick values 
	mGamePadsLTS[0]->update(seconds);

	Real xLeft  = mGamePadsLTS[0]->getX(0);
	Real yLeft  = mGamePadsLTS[0]->getY(0);
	Real xRight = mGamePadsLTS[0]->getX(1);
	Real yRight = mGamePadsLTS[0]->getY(1);

	//updates the orientation of the collision nodes so they face the right way//
	forwardCollisionNodeLTS->setOrientation(PlayerNodeLTS->getOrientation());
	backwardCollisionNodeLTS->setOrientation(PlayerNodeLTS->getOrientation());
	leftCollisionNodeLTS->setOrientation(PlayerNodeLTS->getOrientation());
	rightCollisionNodeLTS->setOrientation(PlayerNodeLTS->getOrientation());

	//collision vectors//
	Vector3 forwardResult_1;
	Vector3 forwardResult_2;
	Vector3 backwardResult_1;
	Vector3 backwardResult_2;
	Vector3 leftResult_1;
	Vector3 leftResult_2;
	Vector3 rightResult_1;
	Vector3 rightResult_2;
	
	if(shirt == 1){Ogre::Entity* temp = ShirtEntityLTS;}
	else{Ogre::Entity* temp = ShirtlessEntityLTS; }
	
	// If joysticks are moved on any axis
	if(fabs(xLeft) > 0.0f || fabs(yLeft) > 0.0f)
	{
		//forward collisons//
		if(forwardCollisionCheckLTS(temp,PlayerNodeLTS,forwardCollisionNodeLTS) == true)			
		{	
			// If Left joystick moved on the y-axis
			if(yLeft > 0.5f)
			{
				mMoveScale = speed;	
				mTranslateVector.x = mMoveScale;
			}
			else if(yLeft > 0.0f)
			{
				mMoveScale = 1;	
				mTranslateVector.x = mMoveScale;
			}	
		}
	
		//backward collisons//
		if(backwardCollisionCheckLTS(temp,PlayerNodeLTS,backwardCollisionNodeLTS) == true)
		{	
			// If Left joystick moved on the y-axis
			if(yLeft < -0.5f)
			{
				mMoveScale = speed;	
				mTranslateVector.x = -mMoveScale;
			}
			else if(yLeft < 0.0f)
			{
				mMoveScale = 1;	
				mTranslateVector.x = -mMoveScale;
			}
		}
		
		//right collision//
		if(rightCollisionCheckLTS(temp,PlayerNodeLTS,rightCollisionNodeLTS) == true)
		{
			// If Left joystick moved on the x-axis
			if(xLeft > 0.5f)
			{	
				mMoveScale = speed;	
				mTranslateVector.z = mMoveScale;
			}
			else if(xLeft > 0.0f)
			{	
				mMoveScale = 1;	
				mTranslateVector.z = mMoveScale;
			}
		}
		
		//left collision//
		if(leftCollisionCheckLTS(temp,PlayerNodeLTS,leftCollisionNodeLTS) == true)
		{
			// If Left joystick moved on the x-axis
			if(xLeft < -0.5f)
			{
				mMoveScale = speed;	
				mTranslateVector.z = -mMoveScale;
			}
			else if(xLeft < 0.0f)
			{	
				mMoveScale = 1;	
				mTranslateVector.z = -mMoveScale;
			}
		}
		
		// Set animation
		if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "walk" );}
		else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "walk" );}
		robot_anim1LTS->setLoop( true );
		robot_anim1LTS->setEnabled( true );
	}
	// If joysticks are not moved on any axis
	else if( xLeft == 0.0f && yLeft == 0.0f )
	{	
		if(shirt == 1){robot_anim1LTS = ShirtEntityLTS->getAnimationState( "rest" );}
		else {robot_anim1LTS = ShirtlessEntityLTS->getAnimationState( "rest" );}
		robot_anim1LTS->setLoop( true );
		robot_anim1LTS->setEnabled( true );
	}
	
	// If Right joystick moved on the y-axis
	if(yRight > 0.5f && pitchLTS < 45)
	{	
		if(gamepadLookInvertedLTS){ mCamera->pitch(Degree(-1)); pitchLTS++;}
		else if(!gamepadLookInvertedLTS){ mCamera->pitch(Degree(1)); pitchLTS++;}
	}
	else if(yRight > 0.0f && pitchLTS < 45)
	{	
		if(gamepadLookInvertedLTS){ mCamera->pitch(Degree(-0.5)); pitchLTS+= 0.5;}
		else if(!gamepadLookInvertedLTS){ mCamera->pitch(Degree(0.5)); pitchLTS+= 0.5;}
	}
	else if(yRight < -0.5f && pitchLTS > -45)
	{
		if(gamepadLookInvertedLTS){ mCamera->pitch(Degree(1)); pitchLTS--;}
		else if(!gamepadLookInvertedLTS){ mCamera->pitch(Degree(-1)); pitchLTS--;}
	}
	else if(yRight < 0.0f && pitchLTS > -45)
	{
		if(gamepadLookInvertedLTS){ mCamera->pitch(Degree(0.5)); pitchLTS-= 0.5;}
		else if(!gamepadLookInvertedLTS){ mCamera->pitch(Degree(-0.5)); pitchLTS-= 0.5;}
	}
	
	// If Right joystick moved on the x-axis	
	if(xRight > 0.5f)
	{
		mCamera->yaw( Degree(-2));
		PlayerNodeLTS->yaw(Degree(-2));
	}
	if(xRight > 0.0f)
	{
		mCamera->yaw( Degree(-0.5));
		PlayerNodeLTS->yaw(Degree(-0.5));
	}
	else if(xRight < -0.5f)
	{
		mCamera->yaw( Degree(2));
		PlayerNodeLTS->yaw(Degree(2));
	}
	else if(xRight < 0.0f)
	{
		mCamera->yaw( Degree(0.5));
		PlayerNodeLTS->yaw(Degree(0.5));
	}
	
	// move character
	PlayerNodeLTS->translate(PlayerNodeLTS->getOrientation() * mTranslateVector, Ogre::SceneNode::TS_WORLD);
	// reset translate vector
	mTranslateVector = Vector3::ZERO;
}
bool LevelTestState::keyPressed( const OIS::KeyEvent &e ) 
{
	//move left
	if( e.key == OIS::KC_A )
	{
		keyLeftPress = true;
	}
	//move right
	if(e.key == OIS::KC_D )
	{
		keyRightPress = true;
	}
	//move forward
	if(e.key == OIS::KC_W )
	{
		keyForwardPress = true;
	}
	//move backward
	if(e.key == OIS::KC_S )
	{
		keyBackwardPress = true;
	}
	
	/*if(e.key == OIS::KC_B)
	{
		factioned = true;
	}*/
	if(e.key == OIS::KC_M)
	{
		inConversation = false;
		dialogueCounter = 0;
	}

	/*if(e.key == OIS::KC_0)
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("SampleWork5"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("EmptyUI"));
	}*/

	if(inConversation == true)
	{
		if(e.key == OIS::KC_Y && geminiTalkToTheSword == true)
		{
			yes = true;
			dialogueCounter++;
		}
		else if(e.key == OIS::KC_Y && factionedJolly == true)
		{
			yes = true;
			dialogueCounter++;
		}
	}

	/*if(e.key == OIS::KC_P)
	{
		jollyKnockedOut = true;
	}*/

	if(store == true)
	{
		if(e.key == OIS::KC_R)
		{
			npcTalk->setCaption("");
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("StoreState"));
		}
	}

	if(train == true)
	{
		if(e.key == OIS::KC_R)
		{
			npcTalk->setCaption("");
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("TrainingState"));
		}
	}
	
	if(e.key == OIS::KC_N && geminiTalkToTheSword == true)
	{
		dialogueCounter = 0;
		inConversation = false;
		npcTalk->setCaption("");
		textBox->hide();
		pickAFaction->hide();
		geminiTalkToTheSword = false;
		factioned = false;
		factionedJolly = false;
		updateGeminiQuests = false;
	}
	else if(e.key == OIS::KC_N && factionedJolly == true)
	{
		dialogueCounter = 0;
		inConversation = false;
		npcTalk->setCaption("");
		textBox->hide();
		pickAFaction->hide();
		factionedJolly = false;
		factioned = false;
		factionedGemini = false;
		updateJollysQuests = false;
	}
	

	//if(e.key == OIS::KC_I)
	//{
	//	Cockroach = 20;
	//}

	//if(e.key == OIS::KC_O)
	//{
	//	Rat = 20;
	//}
	//if(e.key == OIS::KC_P)
	//{
	//	WhoIsEscaping = true;
	//}

	//cycle through quest dialogue when it pops up//
	if(inConversation == true && e.key == OIS::KC_Q)
	{
		if(jollyKeyToThePlanDialogueCompleted == false && jollyKeyToThePlan == true)
		{
			dialogueCounter = 0;
			inConversation = false;
			npcTalk->setCaption("");
		}
		

		dialogueCounter++;
	}
	else if(e.key == OIS::KC_J)
	{
		if(story01Continue == false)
		{
			story01Continue = true;
		}

		if(story02Continue == false && story01FadeVar <= 0)
		{
			story02Continue = true;
		}
	}

	if(inConversation == false)
	{
			//Pause button
		if(e.key == OIS::KC_RETURN)
		{
			//updates the xml info everytime the inventory is accesed//
			saveInventory(); 
			//updateMerchandise();

			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("PauseMenu"));
		}
	}

	if(e.key == OIS::KC_LSHIFT )
	{
		shiftPressedLTS = true;
	}

	return true;
}

bool LevelTestState::keyReleased( const OIS::KeyEvent &e ) 
{
    if( e.key == OIS::KC_ESCAPE ) 
	{
     	GameManager::getSingletonPtr()->requestShutdown();
    }
	
	if(e.key == OIS::KC_LCONTROL)
	{
		if(crouch)
		{
			speed = 6;
			charHeight = 68;
			crouch = false;
		}
		else
		{
			speed = 3;
			charHeight = 34;
			crouch = true;
		}
	}

	if(e.key == OIS::KC_E )
	{
		if(LevelLoaded == 0)
		{
			checkPlayerCollision();
		}
		else if(LevelLoaded == 1)
		{
			checkObjectCollision();
			checkPlayerCollision();
			checkItemCollision();
		}
		else if(LevelLoaded == 2)
		{
			checkObjectCollision();
			checkPlayerCollision();
			checkItemCollision();
		}
		else if(LevelLoaded == 3)
		{
			checkObjectCollision();
			checkPlayerCollision();
			checkItemCollision();
		}
		else if(LevelLoaded == 4)
		{
			checkObjectCollision();
			checkPlayerCollision();
			checkItemCollision();
		}
	}

	if( e.key == OIS::KC_A )
	{
		keyLeftPress = false;
	}
	if(e.key == OIS::KC_D )
	{
		keyRightPress = false;
	}
	if(e.key == OIS::KC_W )
	{
		keyForwardPress = false;
	}
	if(e.key == OIS::KC_S )
	{
		keyBackwardPress = false;
	}

	if(e.key == OIS::KC_LSHIFT )
	{
		shiftPressedLTS = false;
	}
	return true;
}

void LevelTestState::checkLoadCharacterDetection( void )
{
	for(SceneNodeList::iterator objectIt = mLoadCharacterSceneNodesLTS.begin(); objectIt != mLoadCharacterSceneNodesLTS.end(); objectIt++)
	{
		if( PlayerNodeLTS->getWorldPosition().x + 100 > (*objectIt)->getWorldPosition().x   )
		{
			if( PlayerNodeLTS->getWorldPosition().x  - 100 < (*objectIt)->getWorldPosition().x )
			{
				if( PlayerNodeLTS->getWorldPosition().z  + 100 > (*objectIt)->getWorldPosition().z )
				{
					if( PlayerNodeLTS->getWorldPosition().z  - 100 < (*objectIt)->getWorldPosition().z )
					{
						////////////////////////////////
						//     THE CELLS LEVEL        //
						////////////////////////////////
						if(LevelLoaded == 1)
						{
							if((*objectIt)->getName() == detectNode1->getName())
							{
								///////////////////////////
								//        NPCS           //
								///////////////////////////
								linMaNode->setVisible(false);	
								santosoNode->setVisible(false);
								

								///////////////////////////
								//        DOORNODES      //
								///////////////////////////
								emptyRightCornerDoor->setVisible(false);
								chaosCellDoor->setVisible(false);
								terranceDougieCellDoor->setVisible(false);
								linMasDoor->setVisible(false);
								emptyTerranceCellDoor->setVisible(false);
								santosoCellDoor->setVisible(false);
								playerCellDoor->setVisible(true);
								johnny666CellDoor->setVisible(true);
								dannyBoyCellDoor->setVisible(true);
								rightOfPlayerCell->setVisible(true);
								emptyCellJollyDoor->setVisible(true);
								jollyCellDoor->setVisible(true);
							}
							else if((*objectIt)->getName() == detectNode2->getName())
							{
								///////////////////////////
								//        NPCS           //
								///////////////////////////
								linMaNode->setVisible(false);		
								dannyBoyNode->setVisible(true);
								santosoNode->setVisible(false);



								///////////////////////////
								//        DOORS          //
								///////////////////////////
								playerCellDoor->setVisible(true);
								johnny666CellDoor->setVisible(true);
								dannyBoyCellDoor->setVisible(true);
								rightOfPlayerCell->setVisible(true);
								chaosCellDoor->setVisible(true);
								emptyCellJollyDoor->setVisible(false);
								jollyCellDoor->setVisible(false);
								terranceDougieCellDoor->setVisible(false);
								linMasDoor->setVisible(false);
								emptyTerranceCellDoor->setVisible(false);
								santosoCellDoor->setVisible(false);
							}
							else if((*objectIt)->getName() == detectNode3->getName())
							{
								///////////////////////////
								//        NPCS           //
								///////////////////////////
								santosoNode->setVisible(true);
								linMaNode->setVisible(true);


								///////////////////////////
								//        DOOR           //
								///////////////////////////
								terranceDougieCellDoor->setVisible(true);
								linMasDoor->setVisible(true);
								emptyTerranceCellDoor->setVisible(true);
								santosoCellDoor->setVisible(true);
								playerCellDoor->setVisible(false);
								johnny666CellDoor->setVisible(false);
								dannyBoyCellDoor->setVisible(false);
								rightOfPlayerCell->setVisible(false);
								emptyRightCornerDoor->setVisible(false);
								emptyCellJollyDoor->setVisible(false);
								jollyCellDoor->setVisible(false);
							}
							else if((*objectIt)->getName() == detectNode4->getName())
							{
								///////////////////////////
								//        NPCS           //
								///////////////////////////
								santosoNode->setVisible(true);
								linMaNode->setVisible(true);


								///////////////////////////
								//        DOOR           //
								///////////////////////////
								emptyRightCornerDoor->setVisible(true);
								terranceDougieCellDoor->setVisible(true);
								linMasDoor->setVisible(true);
								emptyTerranceCellDoor->setVisible(true);
								santosoCellDoor->setVisible(true);
								playerCellDoor->setVisible(false);
								johnny666CellDoor->setVisible(false);
								dannyBoyCellDoor->setVisible(false);
								rightOfPlayerCell->setVisible(false);
								chaosCellDoor->setVisible(false);
							}
							else if((*objectIt)->getName() == detectNode5->getName())
							{

								///////////////////////////
								//        DOOR           //
								///////////////////////////
								emptyRightCornerDoor->setVisible(true);
								chaosCellDoor->setVisible(true);
				
							}
							else if((*objectIt)->getName() == detectNode6->getName())
							{

								///////////////////////////
								//         NPC           //
								///////////////////////////
								dannyBoyNode->setVisible(true);	

								///////////////////////////
								//        DOOR           //
								///////////////////////////
								emptyCellJollyDoor->setVisible(true);
								jollyCellDoor->setVisible(true);
							}
						}
						////////////////////////////////
						//  THE CELLS NIGHTIME LEVEL  //
						////////////////////////////////
						else if(LevelLoaded == 4)
						{
							if((*objectIt)->getName() == detectNode1->getName())
							{
								///////////////////////////
								//         NPC           //
								///////////////////////////
								linMaNode->setVisible(true);
								santosoNode->setVisible(false);


								///////////////////////////
								//         ITEMS         //
								///////////////////////////
								emptyCellRightOfPlayerCigarettes->setVisible(false);
								

								///////////////////////////
								//        DOOR           //
								///////////////////////////
								playerCellDoor->setVisible(true);
								johnny666CellDoor->setVisible(true);
								dannyBoyCellDoor->setVisible(true);
								rightOfPlayerCell->setVisible(true);
								emptyCellJollyDoor->setVisible(true);
								jollyCellDoor->setVisible(true);
								emptyRightCornerDoor->setVisible(false);
								chaosCellDoor->setVisible(false);
								terranceDougieCellDoor->setVisible(false);
								linMasDoor->setVisible(false);
								emptyTerranceCellDoor->setVisible(false);
								santosoCellDoor->setVisible(false);

								

									
						

								
							}
							else if((*objectIt)->getName() == detectNode2->getName())
							{
								linMaNode->setVisible(true);		
								dannyBoyNode->setVisible(true);	

								playerCellDoor->setVisible(true);
								johnny666CellDoor->setVisible(true);
								dannyBoyCellDoor->setVisible(true);
								rightOfPlayerCell->setVisible(true);
								chaosCellDoor->setVisible(true);

								santosoNode->setVisible(false);

								emptyCellJollyDoor->setVisible(false);
								jollyCellDoor->setVisible(false);
								terranceDougieCellDoor->setVisible(false);
								linMasDoor->setVisible(false);
								emptyTerranceCellDoor->setVisible(false);
								santosoCellDoor->setVisible(false);
							}
							else if((*objectIt)->getName() == detectNode3->getName())
							{
								santosoNode->setVisible(true);
								terranceDougie->setVisible(true);

								linMasMoney->setVisible(true);
								santosoPainKiller->setVisible(true);
								linMasSoap->setVisible(true);

								terranceDougieCellDoor->setVisible(true);
								linMasDoor->setVisible(true);
								emptyTerranceCellDoor->setVisible(true);
								santosoCellDoor->setVisible(true);

								linMaNode->setVisible(false);	

								playerCellDoor->setVisible(false);
								johnny666CellDoor->setVisible(false);
								dannyBoyCellDoor->setVisible(false);
								rightOfPlayerCell->setVisible(false);
								emptyRightCornerDoor->setVisible(false);
								emptyCellJollyDoor->setVisible(false);
								jollyCellDoor->setVisible(false);
							}
							else if((*objectIt)->getName() == detectNode4->getName())
							{
								santosoNode->setVisible(true);
								terranceDougie->setVisible(true);

								emptyRightCornerDoor->setVisible(true);
								terranceDougieCellDoor->setVisible(true);
								linMasDoor->setVisible(true);
								emptyTerranceCellDoor->setVisible(true);
								santosoCellDoor->setVisible(true);

								linMaNode->setVisible(false);		

								playerCellDoor->setVisible(false);
								johnny666CellDoor->setVisible(false);
								dannyBoyCellDoor->setVisible(false);
								rightOfPlayerCell->setVisible(false);
								chaosCellDoor->setVisible(false);
							}
							else if((*objectIt)->getName() == detectNode5->getName())
							{

								emptyRightCornerDoor->setVisible(true);
								chaosCellDoor->setVisible(true);

								dannyBoyNode->setVisible(false);	
							}
							else if((*objectIt)->getName() == detectNode6->getName())
							{
								dannyBoyNode->setVisible(true);	

								emptyCellJollyDoor->setVisible(true);
								jollyCellDoor->setVisible(true);
		
							}
						}
					}
				}
			}
		}
	}
}
bool LevelTestState::checkObjectDetection( void )
{
	
	for(SceneNodeList::iterator objectIt = mSceneNodesLTS.begin(); objectIt != mSceneNodesLTS.end(); objectIt++)
	{
		if( objectCollisionNodeLTS->getWorldPosition().x + 20 > (*objectIt)->getWorldPosition().x   )
		{
			if( objectCollisionNodeLTS->getWorldPosition().x  - 20 < (*objectIt)->getWorldPosition().x )
			{
				if( objectCollisionNodeLTS->getWorldPosition().z  + 20 > (*objectIt)->getWorldPosition().z )
				{
					if( objectCollisionNodeLTS->getWorldPosition().z  - 20 < (*objectIt)->getWorldPosition().z )
					{
						if(LevelLoaded == 0)
						{
							if((*objectIt)->getName() == playerCellDoorNode->getName() ||(*objectIt)->getName() == playerCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
						}
						if(LevelLoaded == 1)
						{
							if((*objectIt)->getName() == playerCellDoorNode->getName() ||(*objectIt)->getName() == playerCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == object_Node1LTS->getName())
							{
								detectObject->setCaption("Go To Cafeteria");
							}
							else if((*objectIt)->getName() == rightOfPlayerCellNode->getName() || (*objectIt)->getName() == rightOfPlayerCellColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == johnny666CellDoorNode->getName() || (*objectIt)->getName() == johnny666CellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == dannyBoyCellDoorNode->getName() || (*objectIt)->getName() == dannyBoyCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == jollyCellDoorNode->getName() || (*objectIt)->getName() == jollyCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == emptyCellJollyDoorNode->getName() || (*objectIt)->getName() == emptyCellJollyDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == chaosCellDoorNode->getName() || (*objectIt)->getName() == chaosCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == emptyRightCornerDoorNode->getName() || (*objectIt)->getName() == emptyRightCornerDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == terranceDougieCellDoorNode->getName() || (*objectIt)->getName() == terranceDougieCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == emptyTerranceCellDoorNode->getName() || (*objectIt)->getName() == emptyTerranceCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == santosoCellDoorNode->getName() || (*objectIt)->getName() == santosoCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == linMasDoorNode->getName() || (*objectIt)->getName() == linMasDoorColNode->getName())
							{	
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == nightTimeNode->getName())
							{
								detectObject->setCaption("Go To Sleep");
							}

						}	
						////If Cafeteria level loaded
						else if(LevelLoaded == 2)
						{
							//If player selects knot object
							if((*objectIt)->getName() == object_Node3LTS->getName())
							{
								detectObject->setCaption("Go To Cells");
							}
							//If player selects barrel object
							else if((*objectIt)->getName() == object_Node2LTS->getName())
							{
								detectObject->setCaption("Go To CourtYard");
							}
							else if((*objectIt)->getName() == AI_Node1LTS->getName())
							{
								detectObject->setCaption("Kill Rat");
							}
							else if((*objectIt)->getName() == AI_Node2LTS->getName())
							{
								detectObject->setCaption("Kill Rat");
							}
							else if((*objectIt)->getName() == AI_Node3LTS->getName())
							{
								detectObject->setCaption("Kill Cockroach");
							}
							else if((*objectIt)->getName() == AI_Node4LTS->getName())
							{
								detectObject->setCaption("Kill Cockroach");
							}
							else if((*objectIt)->getName() == AI_Node5LTS->getName())
							{
								detectObject->setCaption("Kill Cockroach");
							}

						}
						//If Courtyard level loaded
						else if(LevelLoaded == 3)
						{
							//If player selects fish object
							if((*objectIt)->getName() == object_Node4LTS->getName())
							{
								detectObject->setCaption("Go To Cafeteria");
							}
							return true;
						}
						else if(LevelLoaded == 4)
						{
							if((*objectIt)->getName() == playerCellDoorNode->getName() ||(*objectIt)->getName() == playerCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == object_Node1LTS->getName())
							{
								detectObject->setCaption("Go To Cafeteria");
							}
							else if((*objectIt)->getName() == rightOfPlayerCellNode->getName() || (*objectIt)->getName() == rightOfPlayerCellColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == johnny666CellDoorNode->getName() || (*objectIt)->getName() == johnny666CellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == dannyBoyCellDoorNode->getName() || (*objectIt)->getName() == dannyBoyCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == jollyCellDoorNode->getName() || (*objectIt)->getName() == jollyCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == emptyCellJollyDoorNode->getName() || (*objectIt)->getName() == emptyCellJollyDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == chaosCellDoorNode->getName() || (*objectIt)->getName() == chaosCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == emptyRightCornerDoorNode->getName() || (*objectIt)->getName() == emptyRightCornerDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == terranceDougieCellDoorNode->getName() || (*objectIt)->getName() == terranceDougieCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == emptyTerranceCellDoorNode->getName() || (*objectIt)->getName() == emptyTerranceCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == santosoCellDoorNode->getName() || (*objectIt)->getName() == santosoCellDoorColNode->getName())
							{
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == linMasDoorNode->getName() || (*objectIt)->getName() == linMasDoorColNode->getName())
							{	
								detectObject->setCaption("Interact Door");
							}
							else if((*objectIt)->getName() == nightTimeNode->getName())
							{
								detectObject->setCaption("Go To Sleep");
							}
						}

						return true;
					}
				}
			}
		}
	}

	for(SceneNodeList::iterator objectIt = mPlayerSceneNodesLTS.begin(); objectIt != mPlayerSceneNodesLTS.end(); objectIt++)
	{
		if( objectCollisionNodeLTS->getWorldPosition().x + 20 > (*objectIt)->getWorldPosition().x   )
		{
			if( objectCollisionNodeLTS->getWorldPosition().x  - 20 < (*objectIt)->getWorldPosition().x )
			{
				if( objectCollisionNodeLTS->getWorldPosition().z  + 20 > (*objectIt)->getWorldPosition().z )
				{
					if( objectCollisionNodeLTS->getWorldPosition().z  - 20 < (*objectIt)->getWorldPosition().z )
					{
						//Initial Scene//
						if(LevelLoaded == 0)
						{
							if((*objectIt)->getName() == linMaNode->getName())
							{
								detectObject->setCaption("Talk to Lin Ma");
							}
						}
						//If Cells level loaded
						else if(LevelLoaded == 1)
						{
							if((*objectIt)->getName() == linMaNode->getName())
							{
								detectObject->setCaption("Talk to Lin Ma");
							}
							//santoso node//
							else if((*objectIt)->getName() == santosoNode->getName())
							{
								detectObject->setCaption("Talk to Santoso");
								mYButtonOverlay->show();
								store = true;
								yButtonTextBox->setCaption("Use Store");
							}
							else if((*objectIt)->getName() == dannyBoyNode->getName())
							{
								detectObject->setCaption("Talk to DannyBoy");
							}
							/*else if((*objectIt)->getName() == guardNPCNode->getName())
							{
								detectObject->setCaption("Talk to Guard");
							}*/
						}
						else if(LevelLoaded == 2)
						{
							if((*objectIt)->getName() == michealNode->getName())
							{
								detectObject->setCaption("Talk to Micheal");
							}
							else if((*objectIt)->getName() == jollyNode->getName())
							{
								detectObject->setCaption("Talk to Jolly");
							}
						}
						else if(LevelLoaded == 3)
						{
							if((*objectIt)->getName() == trainerNode->getName())
							{
								detectObject->setCaption("Talk to Johnny");
								yButtonTextBox->setCaption("   Train");
								mYButtonOverlay->show();
								train = true;
							}
							else if((*objectIt)->getName() == terranceDougieNode->getName())
							{
								detectObject->setCaption("Talk to Gemini Twins");
							}
						}
						return true;
					}
				}
			}
		}
	}

	for(SceneNodeList::iterator objectIt = mItemsNodeLTS.begin(); objectIt != mItemsNodeLTS.end(); objectIt++)
	{
		if( objectCollisionNodeLTS->getWorldPosition().x + 20 > (*objectIt)->getWorldPosition().x   )
		{
			if( objectCollisionNodeLTS->getWorldPosition().x  - 20 < (*objectIt)->getWorldPosition().x )
			{
				if( objectCollisionNodeLTS->getWorldPosition().z  + 20 > (*objectIt)->getWorldPosition().z )
				{
					if( objectCollisionNodeLTS->getWorldPosition().z  - 20 < (*objectIt)->getWorldPosition().z )
					{
						//Initial Scene//
						if(LevelLoaded == 0)
						{
				
						}
						//If Cells level loaded
						else if(LevelLoaded == 1)
						{
							if((*objectIt)->getName() == dannyBoySteroidsNode->getName())
							{
								detectObject->setCaption("Steroids");
							}
							else if((*objectIt)->getName() == santosoPainKillerNode->getName())
							{
								detectObject->setCaption("Steroids");
							}
							else if((*objectIt)->getName() == linMasMoneyNode->getName())
							{
								detectObject->setCaption("Money");
							}
							else if((*objectIt)->getName() == linMasSoapNode->getName())
							{
								detectObject->setCaption("Soap");
							}

						}
						return true;
					}
				}
			}
		}
	}
	
	detectObject->setCaption("");
	yButtonTextBox->setCaption("");
	store = false;
	train = false;
	return false;
	
}
void LevelTestState::checkItemCollision( void )
{
	for(SceneNodeList::iterator objectIt = mItemsNodeLTS.begin(); objectIt != mItemsNodeLTS.end(); objectIt++)
	{
		if( objectCollisionNodeLTS->getWorldPosition().x + 20 > (*objectIt)->getWorldPosition().x   )
		{
			if( objectCollisionNodeLTS->getWorldPosition().x  - 20 < (*objectIt)->getWorldPosition().x )
			{
				if( objectCollisionNodeLTS->getWorldPosition().z  + 20 > (*objectIt)->getWorldPosition().z )
				{
					if( objectCollisionNodeLTS->getWorldPosition().z  - 20 < (*objectIt)->getWorldPosition().z )
					{
						//Initial Conversation//
						if(LevelLoaded == 0)
						{

						}
						//If Cells level loaded
						else if(LevelLoaded == 1)
						{
							/*bool itemSoap = NULL;
							bool itemDannySteroids = NULL;
							bool itemSantosoSteroids = NULL;
							bool itemLinMaMoney = NULL;*/
							if((*objectIt)->getName() == linMasSoapNode->getName())
							{
								if(itemSoap == false)
								{
									Soap = 1;
									itemSoap = true;
									linMasSoapNode->setPosition(Vector3(2000.0f,0.0f,0.0f));
								}

							}
							else if((*objectIt)->getName() == linMasMoneyNode->getName())
							{
								if(itemLinMaMoney == false)
								{
									Money += 10;
									itemLinMaMoney = true;
									linMasMoneyNode->setPosition(Vector3(2000.0f,0.0f,0.0f));
								}
							}
							else if((*objectIt)->getName() == santosoPainKillerNode->getName())
							{
								if(itemSantosoSteroids == false)
								{
									Syringe += 1;
									itemSantosoSteroids = true;
									santosoPainKillerNode->setPosition(Vector3(2000.0f,0.0f,0.0f));
								}
							}
							else if((*objectIt)->getName() == dannyBoySteroidsNode->getName())
							{
								if(itemDannySteroids == false)
								{
									Syringe += 1;
									dannyBoySteroidsNode->setPosition(Vector3(2000.0f,0.0f,0.0f));
									itemDannySteroids = true;
								}
							}
						}
						//If Cafeteria level loaded
						else if(LevelLoaded == 2)
						{
							
						}
						//If Courtyard level loaded
						else if(LevelLoaded == 3)
						{
							
						}
						//night time cells level//
						else if(LevelLoaded == 4)
						{
							if((*objectIt)->getName()== nightTimeNode->getName())
							{
								unloadLevel();
								loadCellsNight();
							}

							if((*objectIt)->getName() == cellBlockKeyNode->getName())
							{
								if(itemCellBlockKey == false && itemSoap == true)
								{
									CellKey = 0;
									SoapMold = 1;
									cellBlockKeyNode->setPosition(Vector3(20000.0f,0.0f,0.0f));
									itemCellBlockKey = true;
								}
							}
							if((*objectIt)->getName() == linMasSoapNode->getName())
							{
								if(itemSoap == false)
								{
									Soap = 1;
									itemSoap = true;
									linMasSoapNode->setPosition(Vector3(2000.0f,0.0f,0.0f));
								}

							}
							else if((*objectIt)->getName() == linMasMoneyNode->getName())
							{
								if(itemLinMaMoney == false)
								{
									Money += 10;
									itemLinMaMoney = true;
									linMasMoneyNode->setPosition(Vector3(2000.0f,0.0f,0.0f));
								}
							}
							else if((*objectIt)->getName() == santosoPainKillerNode->getName())
							{
								if(itemSantosoSteroids == false)
								{
									Syringe += 1;
									itemSantosoSteroids = true;
									santosoPainKillerNode->setPosition(Vector3(2000.0f,0.0f,0.0f));
								}
							}
							else if((*objectIt)->getName() == dannyBoySteroidsNode->getName())
							{
								if(itemDannySteroids == false)
								{
									Syringe += 1;
									dannyBoySteroidsNode->setPosition(Vector3(2000.0f,0.0f,0.0f));
									itemDannySteroids = true;
								}
							}
							else if((*objectIt)->getName() == escapePlansNode->getName())
							{
								if(itemEscapePlans == false)
								{
									WhoIsEscaping = true;
									escapePlansNode->setPosition(Vector3(20000.0f,0.0f,0.0f));
									itemEscapePlans = true;
								}
							}
						}
					}
				}
			}
		}
	}
}

void LevelTestState::checkAICollision(void)
{
	for(SceneNodeList::iterator objectIt = mSceneNodesLTS.begin(); objectIt != mSceneNodesLTS.end(); objectIt++)
	{
		if( objectCollisionNodeLTS->getWorldPosition().x + 20 > (*objectIt)->getWorldPosition().x   )
		{
			if( objectCollisionNodeLTS->getWorldPosition().x  - 20 < (*objectIt)->getWorldPosition().x )
			{
				if( objectCollisionNodeLTS->getWorldPosition().z  + 20 > (*objectIt)->getWorldPosition().z )
				{
					if( objectCollisionNodeLTS->getWorldPosition().z  - 20 < (*objectIt)->getWorldPosition().z )
					{
						if(LevelLoaded == 2)
						{
							if((*objectIt)->getName() == AI_Node1LTS->getName())
							{
								if(levelsSFXSetting)
								{
									audio_result = system->playSound(FMOD_CHANNEL_FREE, rat_Squished, false, &channel2);
									ERRCHECK(audio_result);
								}
								Rat++;
								AI_Node1LTS->setPosition(Vector3( -(rand() % 1350 + 70), 0.0f, -(rand() % 1100 + 10) ) );
							}
							else if((*objectIt)->getName() == AI_Node2LTS->getName())
							{
								if(levelsSFXSetting)
								{
									audio_result = system->playSound(FMOD_CHANNEL_FREE, rat_Squished, false, &channel2);
									ERRCHECK(audio_result);
								}
								Rat++;
								AI_Node2LTS->setPosition(Vector3( -(rand() % 1350 + 70), 0.0f, -(rand() % 1100 + 10) ));
							}
							else if((*objectIt)->getName() == AI_Node3LTS->getName())
							{
								if(levelsSFXSetting)
								{
									audio_result = system->playSound(FMOD_CHANNEL_FREE, rat_Squished, false, &channel2);
									ERRCHECK(audio_result);
								}
								Cockroach++;
								AI_Node3LTS->setPosition(Vector3( 65.0f, rand() % 80 + 30, -(rand() % 1100 + 10) ));
							}
							else if((*objectIt)->getName() == AI_Node4LTS->getName())
							{
								if(levelsSFXSetting)
								{
									audio_result = system->playSound(FMOD_CHANNEL_FREE, rat_Squished, false, &channel2);
									ERRCHECK(audio_result);
								}
								Cockroach++;
								AI_Node4LTS->setPosition(Vector3( -(rand() % 1350 + 70), rand() % 80 + 30, -1125.0f ));
							}
							else if((*objectIt)->getName() == AI_Node5LTS->getName())
							{
								if(levelsSFXSetting)
								{
									audio_result = system->playSound(FMOD_CHANNEL_FREE, rat_Squished, false, &channel2);
									ERRCHECK(audio_result);
								}
								Cockroach++;
								AI_Node5LTS->setPosition(Vector3( -(rand() % 1350 + 70), rand() % 80 + 30, 25.0f ));
							}
						}
					}
				}
			}
		}
	}

}
void LevelTestState::checkObjectCollision( void )
{
	for(SceneNodeList::iterator objectIt = mSceneNodesLTS.begin(); objectIt != mSceneNodesLTS.end(); objectIt++)
	{
		if( objectCollisionNodeLTS->getWorldPosition().x + 20 > (*objectIt)->getWorldPosition().x   )
		{
			if( objectCollisionNodeLTS->getWorldPosition().x  - 20 < (*objectIt)->getWorldPosition().x )
			{
				if( objectCollisionNodeLTS->getWorldPosition().z  + 20 > (*objectIt)->getWorldPosition().z )
				{
					if( objectCollisionNodeLTS->getWorldPosition().z  - 20 < (*objectIt)->getWorldPosition().z )
					{
						//initialConversation//
						if(LevelLoaded == 0)
						{
							
						}
						//If Cells level loaded
						if(LevelLoaded == 1)
						{
							mAButtonOverlay->show();

							//If player selects ogre object
							if((*objectIt)->getName() == object_Node1LTS->getName())
							{
								unloadLevel();
								loadCafeteriaLevel();
							}
							else if((*objectIt)->getName() == nightTimeNode->getName())
							{
								unloadLevel();
								loadCellsNight();
							}
							else if((*objectIt)->getName() == playerCellDoorNode->getName()||(*objectIt)->getName() == playerCellDoorColNode->getName())
							{
								if(Door01_Open)	
								{ 
									Door01_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door01_Open)
								{ 
									Door01_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == rightOfPlayerCellNode->getName()||(*objectIt)->getName() == rightOfPlayerCellColNode->getName())
							{
								if(Door02_Open)	
								{ 
									Door02_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door02_Open)
								{ 
									Door02_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == johnny666CellDoorNode->getName()||(*objectIt)->getName() == johnny666CellDoorColNode->getName())
							{
								if(Door03_Open)	
								{ 
									Door03_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door03_Open)
								{ 
									Door03_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == dannyBoyCellDoorNode->getName()||(*objectIt)->getName() == dannyBoyCellDoorColNode->getName())
							{
								if(Door04_Open)	
								{ 
									Door04_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door04_Open)
								{ 
									Door04_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == jollyCellDoorNode->getName()||(*objectIt)->getName() == jollyCellDoorColNode->getName())
							{
								if(Door05_Open)	
								{ 
									Door05_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door05_Open)
								{ 
									Door05_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == emptyCellJollyDoorNode->getName()||(*objectIt)->getName() == emptyCellJollyDoorColNode->getName())
							{
								if(Door06_Open)	
								{ 
									Door06_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door06_Open)
								{ 
									Door06_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == chaosCellDoorNode->getName()||(*objectIt)->getName() == chaosCellDoorColNode->getName())
							{
								if(Door07_Open)	
								{ 
									Door07_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door07_Open)
								{ 
									Door07_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
							}
							/*else if((*objectIt)->getName() == emptyRightCornerDoorNode->getName()||(*objectIt)->getName() == emptyRightCornerDoorColNode->getName())
							{

							}*/
							else if((*objectIt)->getName() == terranceDougieCellDoorNode->getName()||(*objectIt)->getName() == terranceDougieCellDoorColNode->getName())
							{
								if(Door09_Open)	
								{ 
									Door09_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door09_Open)
								{ 
									Door09_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == emptyTerranceCellDoorNode->getName()||(*objectIt)->getName() == emptyTerranceCellDoorColNode->getName())
							{
								if(Door10_Open)	
								{ 
									Door10_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door10_Open)
								{ 
									Door10_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == santosoCellDoorNode->getName()||(*objectIt)->getName() == santosoCellDoorColNode->getName())
							{
								if(Door11_Open)	
								{ 
									Door11_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door11_Open)
								{ 
									Door11_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == linMasDoorNode->getName()||(*objectIt)->getName() == linMasDoorColNode->getName())
							{	
								if(Door12_Open)	
								{ 
									Door12_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door12_Open)
								{ 
									Door12_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
						}
						//If Cafeteria level loaded
						else if(LevelLoaded == 2)
						{
							//If player selects knot object
							if((*objectIt)->getName() == object_Node3LTS->getName())
							{
								unloadLevel();
								loadCellsLevel();
							}
							//If player selects barrel object
							else if((*objectIt)->getName() == object_Node2LTS->getName())
							{
								unloadLevel();
								loadCourtyardLevel();
							}

						}
						//If Courtyard level loaded
						else if(LevelLoaded == 3)
						{
							//If player selects fish object
							if((*objectIt)->getName() == object_Node4LTS->getName())
							{
								unloadLevel();
								loadCafeteriaLevel();
							}
						}
						else if(LevelLoaded == 4)
						{
							if((*objectIt)->getName() == nightTimeNode->getName())
							{
								unloadLevel();
								loadCellsLevel();
							}
							if(CellKey == 1 || linMasWhoIsEscaping == true)
							{
								if((*objectIt)->getName() == playerCellDoorNode->getName()||(*objectIt)->getName() == playerCellDoorColNode->getName())
								{
									if(Door01_Open)	
									{ 
										Door01_Open = false; 
										if(levelsSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
											ERRCHECK(audio_result);
										}
									}
									else if(!Door01_Open)
									{ 
										Door01_Open = true; 
										if(levelsSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
											ERRCHECK(audio_result);
										}	
									}
									return;
								}
							}
							else if((*objectIt)->getName() == rightOfPlayerCellNode->getName()||(*objectIt)->getName() == rightOfPlayerCellColNode->getName())
							{
								if(Door02_Open)	
								{ 
									Door02_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door02_Open)
								{ 
									Door02_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == johnny666CellDoorNode->getName()||(*objectIt)->getName() == johnny666CellDoorColNode->getName())
							{
								if(Door03_Open)	
								{ 
									Door03_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door03_Open)
								{ 
									Door03_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							if(CellKey == 1 || linMasWhoIsEscaping == true)
							{
								if((*objectIt)->getName() == dannyBoyCellDoorNode->getName()||(*objectIt)->getName() == dannyBoyCellDoorColNode->getName())
								{
									if(Door04_Open)	
									{ 
										Door04_Open = false; 
										if(levelsSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
											ERRCHECK(audio_result);
										}
									}
									else if(!Door04_Open)
									{ 
										Door04_Open = true; 
										if(levelsSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
											ERRCHECK(audio_result);
										}	
									}
									return;
								}
							}
							else if((*objectIt)->getName() == jollyCellDoorNode->getName()||(*objectIt)->getName() == jollyCellDoorColNode->getName())
							{
								if(Door05_Open)	
								{ 
									Door05_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door05_Open)
								{ 
									Door05_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == emptyCellJollyDoorNode->getName()||(*objectIt)->getName() == emptyCellJollyDoorColNode->getName())
							{
								if(Door06_Open)	
								{ 
									Door06_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door06_Open)
								{ 
									Door06_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == chaosCellDoorNode->getName()||(*objectIt)->getName() == chaosCellDoorColNode->getName())
							{
								if(Door07_Open)	
								{ 
									Door07_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door07_Open)
								{ 
									Door07_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
							}
							/*else if((*objectIt)->getName() == emptyRightCornerDoorNode->getName()||(*objectIt)->getName() == emptyRightCornerDoorColNode->getName())
							{

							}*/
							else if((*objectIt)->getName() == terranceDougieCellDoorNode->getName()||(*objectIt)->getName() == terranceDougieCellDoorColNode->getName())
							{
								if(Door09_Open)	
								{ 
									Door09_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door09_Open)
								{ 
									Door09_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == emptyTerranceCellDoorNode->getName()||(*objectIt)->getName() == emptyTerranceCellDoorColNode->getName())
							{
								if(Door10_Open)	
								{ 
									Door10_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door10_Open)
								{ 
									Door10_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							else if((*objectIt)->getName() == santosoCellDoorNode->getName()||(*objectIt)->getName() == santosoCellDoorColNode->getName())
							{
								if(Door11_Open)	
								{ 
									Door11_Open = false; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
										ERRCHECK(audio_result);
									}
								}
								else if(!Door11_Open)
								{ 
									Door11_Open = true; 
									if(levelsSFXSetting)
									{
										audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
										ERRCHECK(audio_result);
									}	
								}
								return;
							}
							if(CellKey == 1)
							{
								if((*objectIt)->getName() == linMasDoorNode->getName()||(*objectIt)->getName() == linMasDoorColNode->getName())
								{	
									if(Door12_Open)	
									{ 
										Door12_Open = false; 
										if(levelsSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Close, false, &channel2);
											ERRCHECK(audio_result);
										}
									}
									else if(!Door12_Open)
									{ 
										Door12_Open = true; 
										if(levelsSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, cell_Door_Open, false, &channel2);
											ERRCHECK(audio_result);
										}	
									}
									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void LevelTestState::yesNo( void )
{
	yes = false;
	no = false;

	if(mInputManager->isKeyDown(OIS::KC_Y))
	{
		yes = true;
	}
	else if(mInputManager->isKeyDown(OIS::KC_N))
	{
		no = true;
	}
}
void LevelTestState::checkPlayerCollision( void )
{
	for(SceneNodeList::iterator objectIt = mPlayerSceneNodesLTS.begin(); objectIt != mPlayerSceneNodesLTS.end(); objectIt++)
	{
		if( objectCollisionNodeLTS->getWorldPosition().x + 20 > (*objectIt)->getWorldPosition().x   )
		{
			if( objectCollisionNodeLTS->getWorldPosition().x  - 20 < (*objectIt)->getWorldPosition().x )
			{
				if( objectCollisionNodeLTS->getWorldPosition().z  + 20 > (*objectIt)->getWorldPosition().z )
				{
					if( objectCollisionNodeLTS->getWorldPosition().z  - 20 < (*objectIt)->getWorldPosition().z )
					{
						//If Cells level loaded
						if(LevelLoaded == 0)
						{
							if((*objectIt)->getName() == linMaNode->getName())
							{
								if(factioned == false)
								{
									//pick a faction quest//
									if(linMasPickAFactionQuestCompleted == false)
									{
										if(linMasPickAFactionDialogueCompleted == false)
										{
											updateLinMasQuests = true;
											linMasPickAFaction = true;
											inConversation = true;
											dialogueCounter = 1;
										}
									}
								}
							}
						}
						if(LevelLoaded == 1)
						{
							/*if((*objectIt)->getName() == guardNPCNode->getName())
							{

							}*/
							

							//////////////////////////////////////
							//          LIN MAS QUESTS          //
							//////////////////////////////////////
							if((*objectIt)->getName() == linMaNode->getName())
							{
								if(factioned == true)
								{
									//bug in the system Quest/Dialogue//
									if(linMasBugInTheSystemQuestCompleted == false)
									{
										
										inConversation = true;
										updateLinMasQuests = true;
										linMasBugInTheSystem = true;
										dialogueCounter = 1;
						
									}
									//More Bugs in the system Quest/Dialogue//
									else if(linMasMoreBugsInTheSystemQuestCompleted == false)
									{
										if(limMasMoreBugsInTheSystemDialogueCompleted == false)
										{
											inConversation = true;
											linMasMoreBugsInTheSystem = true;
											updateLinMasQuests = true;
											dialogueCounter = 1;
										}
										else if(limMasMoreBugsInTheSystemDialogueCompleted == true)
										{
											inConversation = true;
											linMasMoreBugsInTheSystem = true;
											updateLinMasQuests = true;
											dialogueCounter = 1;
										}
										if(Cockroach == 20)
										{
											if(limMasMoreBugsInTheSystemDialogueCompleted == true)
											{
												inConversation = true;
												linMasMoreBugsInTheSystem = true;
												updateLinMasQuests = true;
												dialogueCounter = 1;
											}
										}
									}
									//the rat scuttle Quest/Dialogue//
									else if(linMasTheRatScuttleQuestCompleted == false)
									{
										inConversation = true;
										updateLinMasQuests = true;
										linMasTheRatScuttle = true;
										dialogueCounter = 1;
						
									}
									//lin mas who is escaping//
									else if(linMaswhoIsEscapingQuestCompleted == false)
									{
										if( linMasWhoIsEscapingDialogueCompleted == false)
										{
											inConversation = true;
											updateLinMasQuests = true;
											linMasWhoIsEscaping = true;
											dialogueCounter = 1;
										}
										else if(linMasWhoIsEscapingDialogueCompleted == true)
										{
											inConversation = true;
											updateLinMasQuests = true;
											linMasWhoIsEscaping = true;
											dialogueCounter = 1;
										}
									}
									else if(linMastrainQuestCompleted == false)
									{
										if(linMaTrainDialogueCompleted == false)
										{
											inConversation = true;
											updateLinMasQuests = true;
											linMaTrain = true;
											linMaTrainDialogueCompleted = false;
											dialogueCounter = 1;
										}
									}
								}
							}
							else
							{ 
								npcTalk->setCaption("");
								updateLinMasQuests = false;
							}
							

							//////////////////////////////////////
							//          SANTOSO QUESTS          //
							//////////////////////////////////////
							if((*objectIt)->getName() == santosoNode->getName())
							{								
					
									if(factioned == true)
									{
										if(santosoTalkToTheSwordQuestCompleted == false)
										{
											santosoTalkToTheSword = true;
											updateSantosoQuests = true;
											inConversation = true;
											dialogueCounter = 1;
										}
										if(jollyKeyToThePlanQuestCompleted == true || geminitalkToTheSword2QuestCompleted == true)
										{
											if(santosoKeyToThePlanQuestCompleted == false)
											{
												santosoKeyToThePlanCompleted = true;
												updateSantosoQuests = true;
												dialogueCounter = 1;
												inConversation = true;
											}
										}										
									}	
		
							}

						}
						//If Cafeteria level loaded
						else if(LevelLoaded == 2)
						{
							/////////////////////////////////
							////  RANDOM  MICHEAL DIALOGUE //
							/////////////////////////////////
							//if((*objectIt)->getName() == michealNode->getName())
							//{
							//	michealDialogue = true;
							//	dialogueCounter = 1;
							//	inConversation = true;
							//	updateSantosoQuests = true;
							//}
							
							//////////////////////////////////////
							//          JOLLYS QUESTS           //
							//////////////////////////////////////
							
								if((*objectIt)->getName() == jollyNode->getName())
								{
									
									if(factionedGemini == false)
									{
										if(factioned == false)
										{
											
											if(linMasPickAFactionDialogueCompleted == true)
											{
												//pick a faction jolly//
												if(factionedPickedJollyQuestCompleted == false)
												{
													factionedJolly = true;
													updateJollysQuests = true;
													inConversation = true;
													dialogueCounter = 1;
												}
											}
										}
										if(linMastrainQuestCompleted == true)
										{
											if(factioned == true)
											{
												if(jollytalkToTheSwordQuestCompleted == false)
												{
													jollyTalkToTheSword = true;
													updateJollysQuests = true;
													dialogueCounter = 1;
													inConversation = true;
												}
												else if(santosoTalkToTheSwordQuestCompleted == true)
												{
													 if(jollyKeyToThePlanQuestCompleted == false)
													{
														jollyKeyToThePlan = true;
														updateJollysQuests = true;
														dialogueCounter = 1;
														inConversation = true;
													}
												}	
												/*else if(jollyKeyToThePlanQuestCompleted == false)
												{
													if(jollyKeyToThePlanDialogueCompleted == false)
													{
														if(jollyTalkedToSantoso == true)
														{
															jollyKeyToThePlan = true;
															updateJollysQuests = true;
															inConversation = true;
															dialogueCounter = 1;
														}
													}
													else if(jollyKeyToThePlanDialogueFailed == true)
													{
														inConversation = true;
														updateJollysQuests = true;
														jollyKeyToThePlan = true;
														dialogueCounter = 1;
													}
													else if(Soap == 1)
													{
														if(jollyKeyToThePlanDialogueCompleted == true)
														{
															jollyKeyToThePlan = true;
															updateJollysQuests = true;
															inConversation = true;
															dialogueCounter = 1;
														}
													}
												}*/
												if(jollyKnockEmOutQuestCompleted == false)
												{
													if(jollyKnockEmOutDialogueCompleted == false)
													{
														inConversation = true;
														jollyKnockEmOut = true;
														updateJollysQuests = true;
														dialogueCounter = 1;
													}
													else if(jollyKnockEmOutDialogueCompleted == true)
													{
														inConversation = true;
														jollyKnockEmOut = true;
														updateJollysQuests = true;
														dialogueCounter = 1;
													}
													if(geminiKnockedOut == true)
													{
														if(jollyKnockEmOutDialogueCompleted == true)
														{
															inConversation = true;
															jollyKnockEmOut = true;
															updateJollysQuests = true;
															dialogueCounter = 1;
														}
													}
												}
											
											}
										}
									
								}
								else 
								{
									updateJollysQuests = false;
									npcTalk->setCaption("");
								}
							}
						}
						//If Courtyard level loaded
						else if(LevelLoaded == 3)
						{
							////If player selects trainer
							//if((*objectIt)->getName() == trainerNode->getName())
							//{
							//	GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("TrainingState"));		
							//}
							//johnny 666 node//
							//OMFGZ//
							if((*objectIt)->getName() == trainerNode->getName())
							{
								if(factioned == true)
								{
									if(jollyKnockEmOutQuestCompleted == true || geminiKnockEmOutQuestCompleted == true)
									{
										if(johnny666RepQuestCompleted == false)
										{
											dialogueCounter = 1;
											inConversation = true;
											updateJohnny666Quests = true;
											johnny666Rep = true;
										}
										else if(johnny666CheatALittleQuestCompleted == false)
										{
											if(johnny666CheatALittleDialogueCompleted == false)
											{
												dialogueCounter = 1;
												inConversation = true;
												updateJohnny666Quests = true;
												johnny666CheatALittle = true;
											}
											else if(johnny666CheatALittleDialogueCompleted == true)
											{
												dialogueCounter = 1;
												inConversation = true;
												updateJohnny666Quests = true;
												johnny666CheatALittle = true;
											}
											if(Syringe == 1)
											{
												if(johnny666CheatALittleDialogueCompleted == true)
												{
													inConversation = true;
													updateJohnny666Quests = true;
													dialogueCounter = 1;
												}
											}
										}
										else if(johnny66680RepQuestCompleted == false)
										{
											if(johnny66680Rep == false)
											{
												inConversation = true;
												updateJohnny666Quests = true;
												johnny66680Rep = true;
												dialogueCounter = 1;
											}
										}
										else if(johnny666TrainingDoneQuestCompleted == false)
										{
											if(johnny666TrainingDone == false)
											{
												inConversation = true;
												updateJohnny666Quests = true;
												johnny666TrainingDone = true;
												dialogueCounter = 1;
											}
										}
									}
								}

							}
							else
							{
								updateJohnny666Quests = false;
								npcTalk->setCaption("");
							}
							
							//gemini node//
							if((*objectIt)->getName() == terranceDougieNode->getName())
							{
								if(geminiKnockedOut == false)
								{
									if(factionedJolly == false)
									{
										if(factioned == false)
										{
											if(geminitalkToTheSwordQuestCompleted == false)
											{
												geminiTalkToTheSword = true;
												updateGeminiQuests = true;
												dialogueCounter = 1;
												inConversation = true;
											}
										}
										else if(factioned == true)
										{
											if(linMastrainQuestCompleted == true)
											{
												if(santosoTalkToTheSwordQuestCompleted == true)
												{
													if(geminitalkToTheSword2QuestCompleted == false)
													{
														geminiTalkToTheSwordTwo = true;
														updateGeminiQuests = true;
														inConversation = true;
														dialogueCounter = 1;
													}
												}
												else if(geminiKeyToThePlanQuestCompleted == false)
												{
													geminiKeyToThePlan = true;
													updateGeminiQuests = true;
													inConversation = true;
													dialogueCounter = 1;
												}
												if(geminiKnockEmOutQuestCompleted == false)
												{
													if(geminiKnockEmOutDialogueCompleted == false)
													{
														inConversation = true;
														geminiKnockEmOut = true;
														updateGeminiQuests = true;
														dialogueCounter = 1;
													}
													else if(geminiKnockEmOutDialogueCompleted == true)
													{
														inConversation = true;
														geminiKnockEmOut = true;
														updateGeminiQuests = true;
														dialogueCounter = 1;
													}
													if(jollyKnockedOut == true)
													{
														if(geminiKnockEmOutDialogueCompleted == true)
														{
															inConversation = true;
															geminiKnockEmOut = true;
															updateGeminiQuests = true;
															dialogueCounter = 1;
														}
													}
												}
											}
								
											
											
											/*else if(gemniThePrideOfJollyQuestCompleted == false)
											{
												geminiThePrideOfJolly = true;
												updateGeminiQuests = true;
												inConversation = true;
												dialogueCounter = 1;
											}
											else if(geminiQuestsAllCompleted == false)
											{
												geminiQuestsAllCompleted = false;
												geminiQuestsCompleted = true;
												inConversation = true;
												updateGeminiQuests = true;
												dialogueCounter = 1;
											}*/
										}
									}
								}
								else if(jollyKnockEmOut == true)
								{
									geminiKnockedOut = true;
								}

							}
							else
							{
								updateGeminiQuests = false;
								npcTalk->setCaption("");
							}
						}
						else if(LevelLoaded == 4)
						{
							//////////////////////////////////////
							//          JOLLYS QUESTS           //
							//////////////////////////////////////
							if((*objectIt)->getName() == jollyNode->getName())
							{
								if(factionedGemini == false)
								{
									if(factioned == false)
									{
										
										if(linMasPickAFactionDialogueCompleted == true)
										{
											//pick a faction jolly//
											if(factionedPickedJollyQuestCompleted == false)
											{
												factionedJolly = true;
												updateJollysQuests = true;
												inConversation = true;
												dialogueCounter = 1;
											}
										}
									}
									else if(factioned == true)
									{
										if(jollytalkToTheSwordQuestCompleted == false)
										{
											jollyTalkToTheSword = true;
											updateJollysQuests = true;
											inConversation = true;
											dialogueCounter = 1;
										}
										if(santosoTalkToTheSwordQuestCompleted == true)
										{										
											if(jollyKeyToThePlanQuestCompleted == false)
											{
												if(jollyKeyToThePlanDialogueCompleted == false)
												{
													inConversation = true;
													jollyKeyToThePlan = true;
													updateJollysQuests = true;
													dialogueCounter = 1;
												}
												else if(jollyKeyToThePlanDialogueCompleted == true)
												{
													inConversation = true;
													jollyKeyToThePlan = true;
													updateJollysQuests = true;
													dialogueCounter = 1;
												}
												if(Soap == 1)
												{
													if(jollyKeyToThePlanDialogueCompleted == true)
													{
														inConversation = true;
														jollyKeyToThePlan = true;
														updateJollysQuests = true;
														dialogueCounter = 1;
													}
												}
											}
										}
										else if(jollyKeyToThePlanQuestCompleted == false)
										{
											if(jollyKeyToThePlanDialogueCompleted == false)
											{
												if(jollyTalkedToSantoso == true)
												{
													jollyKeyToThePlan = true;
													updateJollysQuests = true;
													inConversation = true;
													dialogueCounter = 1;
												}
											}
											else if(jollyKeyToThePlanDialogueFailed == true)
											{
												inConversation = true;
												updateJollysQuests = true;
												jollyKeyToThePlan = true;
												dialogueCounter = 1;
											}
											else if(Soap == 1)
											{
												if(jollyKeyToThePlanDialogueCompleted == true)
												{
													jollyKeyToThePlan = true;
													updateJollysQuests = true;
													inConversation = true;
													dialogueCounter = 1;
												}
											}
										}
									}
								}
							}
							else 
							{
								updateJollysQuests = false;
								npcTalk->setCaption("");
							}

							//gemini node//
							if((*objectIt)->getName() == terranceDougieNode->getName())
							{
								if(factionedJolly == false)
								{
									if(factioned == true)
									{
										if(santosoTalkToTheSwordQuestCompleted == true)
										{
											if(geminiTalkToTheSwordTwoDialogueCompleted == false)
											{
												geminiTalkToTheSwordTwo = true;
												inConversation = true;
												dialogueCounter = 1;
												updateGeminiQuest();
											}
											else if(geminiTalkToTheSwordTwoDialogueCompleted == true)
											{
												if(Soap == 1)
												{
													geminiTalkToTheSwordTwo = true;
													inConversation = true;
													dialogueCounter = 1;
													updateGeminiQuest();
												}
											}
										}
									}
								}
								else 
								{
									npcTalk->setCaption("");
								}
							}
						}
					}
				}
			}
		}
	}

}
bool LevelTestState::buttonPressed( const OIS::JoyStickEvent &arg, int button )
{
	return true;
}

bool LevelTestState::buttonReleased( const OIS::JoyStickEvent &arg, int button )
{
	return true;
}

bool LevelTestState::forwardCollisionCheckLTS( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode )
{
	//collision vectors//
	Vector3 forwardResult_1;
	Vector3 forwardResult_2;
	
	//forward collision//
	if(RaycastFromPointLTS(forwardCollisionLTS, entity, entityNode->getPosition(), collisionNode->getOrientation() * Vector3::UNIT_Z, forwardResult_1))
	{
		Vector3 forwardDistance = entityNode->getPosition() - forwardResult_1;
		if(forwardDistance.length() > 10.0f )
		{
			if(RaycastFromPointLTS(forwardCollisionLTS, entity, entityNode->getPosition(), collisionNode->getOrientation() * -Vector3::UNIT_Z,forwardResult_2))
			{
				Vector3 backwardDistance = entityNode->getPosition() + forwardResult_2;
				if(backwardDistance.length() > 10.0f)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
bool LevelTestState::backwardCollisionCheckLTS( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode )
{
	//collision vectors//
	Vector3 backwardResult_1;
	Vector3 backwardResult_2;

	//backward Collision//
	if(RaycastFromPointLTS(backwardCollisionLTS, entity, entityNode->getPosition(), collisionNode->getOrientation() * -Vector3::UNIT_Z, backwardResult_1))
	{
		Vector3 backwardDistance_1 = entityNode->getPosition() - backwardResult_1;
		if(backwardDistance_1.length() > 10.0f )
		{
			if(RaycastFromPointLTS(backwardCollisionLTS, entity, entityNode->getPosition(), collisionNode->getOrientation() * Vector3::UNIT_Z,backwardResult_2))
			{
				Vector3 backwardDistance_2 = entityNode->getPosition() + backwardResult_2;
				if(backwardDistance_2.length() > 10.0f)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
bool LevelTestState::leftCollisionCheckLTS( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode )
{
	//collision vectors//
	Vector3 leftResult_1;
	Vector3 leftResult_2;

	//left collision//
	if(RaycastFromPointLTS(leftCollisionLTS, entity, entityNode->getPosition(), collisionNode->getOrientation() * Vector3::UNIT_X, leftResult_1))
	{
		Vector3 leftDistance = entityNode->getPosition() - leftResult_1;
		if(leftDistance.length() > 10.0f )
		{
			if(RaycastFromPointLTS(leftCollisionLTS, entity, entityNode->getPosition(), collisionNode->getOrientation() * -Vector3::UNIT_X,leftResult_2))
			{
				Vector3 leftDistance = entityNode->getPosition() + leftResult_2;
				if(leftDistance.length() > 10.0f)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
bool LevelTestState::rightCollisionCheckLTS( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode )
{
	//collison vectors//
	Vector3 rightResult_1;
	Vector3 rightResult_2;

	//right collision//
	if(RaycastFromPointLTS(rightCollisionLTS, entity, entityNode->getPosition(), collisionNode->getOrientation() * -Vector3::UNIT_X, rightResult_1))
	{
		Vector3 rightDistance = entityNode->getPosition() - rightResult_1;
		if(rightDistance.length() > 10.0f )
		{
			if(RaycastFromPointLTS(rightCollisionLTS, entity, entityNode->getPosition(), collisionNode->getOrientation() * Vector3::UNIT_X,rightResult_2))
			{
				Vector3 rightDistance = entityNode->getPosition() + rightResult_2;
				if(rightDistance.length() > 10.0f)
				{
					return true;
				}
			}
		}
	}
		
	return false;
}

void LevelTestState::openDoor( void )
{
	//iterates through the door nodes
	for(SceneNodeList::iterator doorNodeIt = mSceneNodesLTS.begin(); doorNodeIt != mSceneNodesLTS.end(); doorNodeIt++)
	{
		if((*doorNodeIt)->getName() == playerCellDoorNode->getName())
		{
			//checks if door 01 is open or closed//
			if( Door01_Open )
			{
				playerCellDoorColNode->setPosition(Vector3(550.0f, 100.0f, 410.0f));
				//open//
				if(door01Pitch < 90 )
				{
					door01Pitch += 1;
					playerCellDoorNode->yaw(Degree(1));
				}
				else if(door01Pitch == 90)
				{
					Door01_Open = true;
				}
			}
			else if(!Door01_Open)
			{
				playerCellDoorColNode->setPosition(Vector3(550.0f, 0.0f, 410.0f));
				//closed//
				if(door01Pitch > 0)
				{
					door01Pitch -= 1;
					playerCellDoorNode->yaw(Degree(-1));
				}
				else if(door01Pitch == 0)
				{
					Door01_Open = false;
				}
			}	
		}
		//cell beside the players//
		else if((*doorNodeIt)->getName() == rightOfPlayerCellNode->getName())
		{
			//checks if door 02 is open or closed//
			if( Door02_Open )
			{
				rightOfPlayerCellColNode->setPosition(Vector3(440.0f,100.0f,410.0f));
				//open//
				if(door02Pitch < 90 )
				{
					door02Pitch += 1;
					rightOfPlayerCellNode->yaw(Degree(-1));
				}
				else if(door02Pitch == 90)
				{
					Door02_Open = true;
				}
			}
			else if(!Door02_Open)
			{
				rightOfPlayerCellColNode->setPosition(Vector3(440.0f,0.0f,410.0f));
				//closed//
				if(door02Pitch > 0)
				{
					door02Pitch -= 1;
					rightOfPlayerCellNode->yaw(Degree(1));
				}
				else if(door02Pitch == 0)
				{
					Door02_Open = false;
				}
			}

		}
		//johnny666 cell door//
		else if((*doorNodeIt)->getName() == johnny666CellDoorNode->getName())
		{
			//checks if johnny 666 cell is open or closed//
			if( Door03_Open )
			{
				johnny666CellDoorColNode->setPosition(Vector3(550.0f,100.0f,522.0f));
				//open//
				if(door03Pitch < 90 )
				{
					door03Pitch += 1;
					johnny666CellDoorNode->yaw(Degree(-1));
				}
				else if(door03Pitch == 90)
				{
					Door03_Open = true;
				}
			}
			else if(!Door03_Open)
			{
				johnny666CellDoorColNode->setPosition(Vector3(550.0f,0.0f,522.0f));
				//closed//
				if(door03Pitch > 0)
				{
					door03Pitch -= 1;
					johnny666CellDoorNode->yaw(Degree(1));
				}
				else if(door03Pitch == 0)
				{
					Door03_Open = false;
				}
			}

		}
		//dannny boys cell door//
		else if((*doorNodeIt)->getName() == dannyBoyCellDoorNode->getName())
		{
			//checks if door 02 is open or closed//
			if( Door04_Open )
			{
				dannyBoyCellDoorColNode->setPosition(Vector3(441.0f,100.0f,520.0f));
				//open//
				if(door04Pitch < 90 )
				{
					door04Pitch += 1;
					dannyBoyCellDoorNode->yaw(Degree(1));
				}
				else if(door04Pitch == 90)
				{
					Door04_Open = true;
				}
			}
			else if(!Door04_Open)
			{
				dannyBoyCellDoorColNode->setPosition(Vector3(441.0f,0.0f,520.0f));
				//closed//
				if(door04Pitch > 0)
				{
					door04Pitch -= 1;
					dannyBoyCellDoorNode->yaw(Degree(-1));
				}
				else if(door04Pitch == 0)
				{
					Door04_Open = false;
				}
			}
		}
		//jolly cell door//
		else if((*doorNodeIt)->getName() == jollyCellDoorNode->getName())
		{
			//checks if jolly cell is open or closed//
			if( Door05_Open )
			{
				jollyCellDoorColNode->setPosition(945.0f,100.0f,407.0f);
				//open//
				if(door05Pitch < 90 )
				{
					door05Pitch += 1;
					jollyCellDoorNode->yaw(Degree(-1));
				}
				else if(door05Pitch == 90)
				{
					Door05_Open = true;
				}
			}
			else if(!Door05_Open)
			{
				jollyCellDoorColNode->setPosition(945.0f,0.0f,407.0f);
				//closed//
				if(door05Pitch > 0)
				{
					door05Pitch -= 1;
					jollyCellDoorNode->yaw(Degree(1));
				}
				else if(door05Pitch == 0)
				{
					Door05_Open = false;
				}
			}
		}
		else if((*doorNodeIt)->getName() == emptyCellJollyDoorNode->getName())
		{
			//checks if jolly cell is open or closed//
			if( Door06_Open )
			{
				emptyCellJollyDoorColNode->setPosition(945.5f,100.0f,523.0f);
				//open//
				if(door06Pitch < 90 )
				{
					door06Pitch += 1;
					emptyCellJollyDoorNode->yaw(Degree(1));
				}
				else if(door06Pitch == 90)
				{
					Door06_Open = true;
				}
			}
			else if(!Door06_Open)
			{
				emptyCellJollyDoorColNode->setPosition(945.5f,0.0f,523.0f);
				//closed//
				if(door06Pitch > 0)
				{
					door06Pitch -= 1;
					emptyCellJollyDoorNode->yaw(Degree(-1));
				}
				else if(door06Pitch == 0)
				{
					Door06_Open = false;
				}
			}
		}
		//chaos cell door//
		else if((*doorNodeIt)->getName() == chaosCellDoorNode->getName())
		{
			//chaos cell door//
			if( Door07_Open )
			{
				chaosCellDoorColNode->setPosition(45.5f,100.0f,520.0f);
				//open//
				if(door07Pitch < 90 )
				{
					door07Pitch += 1;
					chaosCellDoorNode->yaw(Degree(-1));
				}
				else if(door07Pitch == 90)
				{
					Door07_Open = true;
				}
			}
			else if(!Door07_Open)
			{
				chaosCellDoorColNode->setPosition(45.5f,0.0f,520.0f);
				//closed//
				if(door07Pitch > 0)
				{
					door07Pitch -= 1;
					chaosCellDoorNode->yaw(Degree(1));
				}
				else if(door07Pitch == 0)
				{
					Door07_Open = false;
				}
			}
		}
		else if((*doorNodeIt)->getName() == emptyRightCornerDoorNode->getName())
		{
			//chaos cell door//
			if( Door08_Open )
			{
				emptyRightCornerDoorColNode->setPosition(45.5f,100.0f,417.0f);
				//open//
				if(door08Pitch < 90 )
				{
					door08Pitch += 1;
					emptyRightCornerDoorNode->yaw(Degree(1));
				}
				else if(door08Pitch == 90)
				{
					Door08_Open = true;
				}
			}
			else if(!Door08_Open)
			{
				emptyRightCornerDoorColNode->setPosition(45.5f,0.0f,417.0f);
				//closed//
				if(door08Pitch > 0)
				{
					door08Pitch -= 1;
					emptyRightCornerDoorNode->yaw(Degree(-1));
				}
				else if(door08Pitch == 0)
				{
					Door08_Open = false;
				}
			}
		}
		else if((*doorNodeIt)->getName() == terranceDougieCellDoorNode->getName())
		{
			//terrance dougie cell door node//
			if( Door09_Open )
			{
				terranceDougieCellDoorColNode->setPosition(45.5f,100.0f,1455.0f);
				//open//
				if(door09Pitch < 90 )
				{
					door09Pitch += 1;
					terranceDougieCellDoorNode->yaw(Degree(1));
				}
				else if(door09Pitch == 90)
				{
					Door09_Open = true;
				}
			}
			else if(!Door09_Open)
			{
				terranceDougieCellDoorColNode->setPosition(45.5f,0.0f,1455.0f);
				//closed//
				if(door09Pitch > 0)
				{
					door09Pitch -= 1;
					terranceDougieCellDoorNode->yaw(Degree(-1));
				}
				else if(door09Pitch == 0)
				{
					Door09_Open = false;
				}
			}
		}
		else if((*doorNodeIt)->getName() == emptyTerranceCellDoorNode->getName())
		{
			if( Door10_Open )
			{
				emptyTerranceCellDoorColNode->setPosition(441.0f,100.0f,1455.0f);
				//open//
				if(door10Pitch < 90 )
				{
					door10Pitch += 1;
					emptyTerranceCellDoorNode->yaw(Degree(-1));
				}
				else if(door10Pitch == 90)
				{
					Door10_Open = true;
				}
			}
			else if(!Door10_Open)
			{
				emptyTerranceCellDoorColNode->setPosition(441.0f,0.0f,1455.0f);
				//closed//
				if(door10Pitch > 0)
				{
					door10Pitch -= 1;
					emptyTerranceCellDoorNode->yaw(Degree(1));
				}
				else if(door10Pitch == 0)
				{
					Door10_Open = false;
				}
			}
		}
		else if((*doorNodeIt)->getName() == santosoCellDoorNode->getName())
		{
			//terrance dougie cell door node//
			if( Door11_Open )
			{
				santosoCellDoorColNode->setPosition(946.0f,100.0f,1460.0f);
				//open//
				if(door11Pitch < 90 )
				{
					door11Pitch += 1;
					santosoCellDoorNode->yaw(Degree(-1));
				}
				else if(door11Pitch == 90)
				{
					Door11_Open = true;
				}
			}
			else if(!Door11_Open)
			{
				santosoCellDoorColNode->setPosition(946.0f,0.0f,1460.0f);
				//closed//
				if(door11Pitch > 0)
				{
					door11Pitch -= 1;
					santosoCellDoorNode->yaw(Degree(1));
				}
				else if(door11Pitch == 0)
				{
					Door11_Open = false;
				}
			}
		}
		else if((*doorNodeIt)->getName() == linMasDoorNode->getName())
		{
			//terrance dougie cell door node//
			if( Door12_Open )
			{
				linMasDoorColNode->setPosition(385.5f,100.0f,1647.0f);
				//open//
				if(door12Pitch < 90 )
				{
					door12Pitch += 1;
					linMasDoorNode->yaw(Degree(-1));
				}
				else if(door12Pitch == 90)
				{
					Door12_Open = true;
				}
			}
			else if(!Door12_Open)
			{
				linMasDoorColNode->setPosition(385.5f,0.0f,1647.0f);
				//closed//
				if(door12Pitch > 0)
				{
					door12Pitch -= 1;
					linMasDoorNode->yaw(Degree(1));
				}
				else if(door12Pitch == 0)
				{
					Door12_Open = false;
				}
			}
		}
	}
}

void LevelTestState::rotateCamera()
{
	if(camera1MoveCCW)
	{
		camera1Node->rotate(Vector3::UNIT_Y, Degree(cameraSpeed));
		camera1Pitch -= cameraSpeed;
	}
	else if(camera1MoveCW)
	{
		camera1Node->rotate(Vector3::UNIT_Y, Degree(-cameraSpeed));
		camera1Pitch += cameraSpeed;
	}
	if(camera1Pitch == 0.0)
	{
		camera1MoveCCW = false;
		camera1MoveCW = true;
	}
	else if(camera1Pitch == 150.0)
	{
		camera1MoveCW = false;
		camera1MoveCCW = true;
	}

	if(camera2MoveCCW)
	{
		camera2Node->rotate(Vector3::UNIT_Y, Degree(cameraSpeed));
		camera2Pitch -= cameraSpeed;
	}
	else if(camera2MoveCW)
	{
		camera2Node->rotate(Vector3::UNIT_Y, Degree(-cameraSpeed));
		camera2Pitch += cameraSpeed;
	}
	if(camera2Pitch == 0.0)
	{
		camera2MoveCCW = false;
		camera2MoveCW = true;
	}
	else if(camera2Pitch == 150.0)
	{
		camera2MoveCW = false;
		camera2MoveCCW = true;
	}

	if(camera3MoveCCW)
	{
		camera3Node->rotate(Vector3::UNIT_Y, Degree(cameraSpeed));
		camera3Pitch -= cameraSpeed;
	}
	else if(camera3MoveCW)
	{
		camera3Node->rotate(Vector3::UNIT_Y, Degree(-cameraSpeed));
		camera3Pitch += cameraSpeed;
	}
	if(camera3Pitch == 0.0)
	{
		camera3MoveCCW = false;
		camera3MoveCW = true;
	}
	else if(camera3Pitch == 150.0)
	{
		camera3MoveCW = false;
		camera3MoveCCW = true;
	}

	if(camera4MoveCCW)
	{
		camera4Node->rotate(Vector3::UNIT_Y, Degree(cameraSpeed));
		camera4Pitch -= cameraSpeed;
	}
	else if(camera4MoveCW)
	{
		camera4Node->rotate(Vector3::UNIT_Y, Degree(-cameraSpeed));
		camera4Pitch += cameraSpeed;
	}
	if(camera4Pitch == 0.0)
	{
		camera4MoveCCW = false;
		camera4MoveCW = true;
	}
	else if(camera4Pitch == 150.0)
	{
		camera4MoveCW = false;
		camera4MoveCCW = true;
	}
}

void LevelTestState::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	levelsMusicSetting = atoi(element->Attribute("Music"));
	levelsSFXSetting = atoi(element->Attribute("SFX"));
	levelsControls = atoi(element->Attribute("Keyboard"));
	levelsInvert = atoi(element->Attribute("Invert"));
}

void LevelTestState::loadStats( void )
{
	TiXmlDocument loadDoc("attributes.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Stat");

	lCurrentLevel = atoi(element->Attribute("Level"));
	lUpperBodyStat = atoi(element->Attribute("UpperBody"));
	lLowerBodyStat = atoi(element->Attribute("LowerBody"));
	lHealthStat = atoi(element->Attribute("Health"));
	lGrappleStat = atoi(element->Attribute("Grapple"));
}

void LevelTestState::updateStats( void )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("Attributes");
	doc.LinkEndChild(root);

	TiXmlElement *stat = new TiXmlElement("Stat");
	root->LinkEndChild(stat);
	stat->SetAttribute("Level", lCurrentLevel);
	stat->SetAttribute("UpperBody", lUpperBodyStat);
	stat->SetAttribute("LowerBody", lLowerBodyStat);
	stat->SetAttribute("Health", lHealthStat);
	stat->SetAttribute("Grapple", lGrappleStat);

	doc.SaveFile("attributes.xml");
}

void LevelTestState::loadMerchandise( void )
{
	TiXmlDocument loadDoc("merchandise.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Item");
	PainKillers = atoi(element->Attribute("PainKillers"));
	Money = atoi(element->Attribute("Money"));
	StoreSyringe = atoi(element->Attribute("Syringe"));
}

void LevelTestState::loadLevelCharacter( void )
{
	if(shirt == 0)
	{
		PlayerNodeLTS->detachAllObjects();
		PlayerNodeLTS->attachObject(ShirtlessEntityLTS);
		if(hair == 0 && skin == 0)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Black_Tank_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_White_Tank_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_Tank_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Black_Tank_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_White_Tank_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_Tank_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Black_Tank_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_White_Tank_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_Tank_Bald");
		}
	}
	else if(shirt == 1)
	{
		PlayerNodeLTS->detachAllObjects();
		PlayerNodeLTS->attachObject(ShirtEntityLTS);
		if(hair == 0 && skin == 0)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_Black_Collar_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_White_Collar_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_Collar_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_Black_Collar_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_White_Collar_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_Collar_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_Black_Collar_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_White_Collar_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			ShirtEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_Collar_Bald");
		}
	}
	else if(shirt == 2)
	{
		PlayerNodeLTS->detachAllObjects();
		PlayerNodeLTS->attachObject(ShirtlessEntityLTS);
		if(hair == 0 && skin == 0)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Black_None_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_White_None_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_None_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Black_None_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_White_None_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_None_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Black_None_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_White_None_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			ShirtlessEntityLTS->setMaterialName("VGD_ALONE_Main_Asian_None_Bald");
		}
	}
}

LevelTestState* LevelTestState::getSingletonPtr( void ) 
{
    if( !mLevelTestState ) 
	{
        mLevelTestState = new LevelTestState();
		WorkStateFactory::getSingletonPtr()->registerState(mLevelTestState->getName(), mLevelTestState);
    }

    return mLevelTestState;
}
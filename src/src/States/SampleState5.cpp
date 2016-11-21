#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <time.h>

#include "States\SampleState5.h"
#include "Managers\GameManager.h"
#include "Managers\GlobalSettingsManager.h"
#include "Factories\WorkStateFactory.h"
#include "Factories\UIStateFactory.h"

////////////////////
//HEADERS FOR FMOD//
////////////////////

#include "tinyxml.h"
#include <stdio.h>
#include <string.h>

using namespace Ogre;
using namespace Alone;


char buffer[10];

SampleState5* SampleState5::mSampleState5;

EntityList mEntitiesS5;
SceneNodeList mSceneNodesS5;
Alone::GamePadControllerList mGamePadsS5;

SceneNode* cameraNodeS5 = NULL;
SceneNode* forwardCollisionNodeS5 = NULL;
SceneNode* backwardCollisionNodeS5 = NULL;
SceneNode* leftCollisionNodeS5 = NULL;
SceneNode* rightCollisionNodeS5 = NULL;


Entity* collision_EntityS5 = NULL;
Entity* opponentCollision_EntityS5 = NULL;
SceneNode* objectCollisionNodeS5 = NULL;
SceneNode* opponentObjectCollisionNodeS5 = NULL;

Entity* rubberBandEntity = NULL;
SceneNode* rubberBandNode = NULL;

Entity* rubberBandInsideEntity = NULL;
SceneNode* rubberBandInsideNode = NULL;

Vector3 cameraInitialPosS5; 
Quaternion cameraInitialRotS5;

Entity* robot_entity1S5 = NULL;

Entity* ShirtEntity = NULL;
Entity* ShirtlessEntity = NULL;
SceneNode* PlayerNode = NULL;

SceneNode* mainLightNodeS5 = NULL;

Entity* arenaLevelEntity = NULL;
SceneNode* arenaLevelNode = NULL;

Entity* arenaCollisionLevelEntity = NULL;
SceneNode* arenaCollisionLevelNode = NULL;

Entity* AI_Entity1S5 = NULL;
SceneNode* AI_Node1S5 = NULL;

//////////////////////////////
//FLAG VARIABLES FOR OPTIONS//
//////////////////////////////

bool combatMusicSetting;
bool combatSFXSetting;
bool combatControls;
bool combatInvert;

AnimationState* robot_anim1S5 = NULL;
AnimationState* AI_anim1S5 = NULL;


float pitchS5;

int testCount;

int AI1MoveDurationSS5 = 0;
int AI1MoveDirSS5 = 0;
int AI1MaxMoveDurationSS5 = 5;

int randomNumSS5;

int currentPower = 0;

bool gamepadControlsActiveS5 = false;
bool keyboardControlsActiveS5 = true;
bool gamepadLookInvertedS5 = false;
bool keyboardLookInvertedS5= true;

bool playerLeftPunch = false;
bool playerRightPunch = false;
bool playerLeftKick = false;
bool playerRightKick = false;
bool playerGrapple = false;
bool playerHit = false;
bool playerSupermanPunch = false;
bool playerRoundhouseKick = false;
bool setAnimation = false;
bool shiftPressed = false;
bool controlPressed = false;
bool spacePressed = false;

bool opponentHit = false;
bool opponentSetAnimation = false;

bool fromGame;

int playerHealth = 25;
int opponentHealth = 25;
int shirt;
int skin; 
int hair;

RaySceneQuery* raySceneQuery1S5 = NULL;
RaySceneQuery* raySceneQuery2S5 = NULL;
RaySceneQuery* forwardCollisionS5 = NULL;
RaySceneQuery* AI1CollisionS5 = NULL;
RaySceneQuery* AI2CollisionS5 = NULL;
RaySceneQuery* AI3CollisionS5 = NULL;
RaySceneQuery* forwardCollisionAIS5 = NULL;
RaySceneQuery* backwardCollisionS5 = NULL;
RaySceneQuery* leftCollisionS5 = NULL;
RaySceneQuery* rightCollisionS5 = NULL;

Ogre::Vector3 robot_velocity1S5 = Ogre::Vector3::ZERO;
Ogre::Vector3 robot_velocity2S5 = Ogre::Vector3::ZERO;

SampleState5::SampleState5( void ) 
{
	mClassName = "SampleWork5";
	mTime = 0.0f;
}

SampleState5::~SampleState5( void ) 
{
}

//////////////////////////
//CHECKS ERRORS FOR FMOD//
//////////////////////////

void SampleState5::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		FILE *file;
		file = fopen("FMOD_ERRORS.txt", "w+");
		fprintf(file, "FMOD_ERRORS! (%d) %s\n", result, FMOD_ErrorString(result));
		fclose(file);
	}
}

void GetMeshInformationS5(const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, unsigned long* &indices, const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale)
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

bool RaycastFromPointS5(RaySceneQuery* query, Ogre::Entity* parent, const Vector3 &point, const Vector3 &normal, Vector3 &result)
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
				GetMeshInformationS5(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
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
void SampleState5::enter( void ) 
{
	//LOADS OPTIONS FOR THE GAME
	loadOptions();

    mRoot         = Root::getSingletonPtr();
	mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
	mSceneMgr->setAmbientLight( ColourValue( 0,0,0 ) );
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);

    mCamera       = mSceneMgr->createCamera( "SampleState5Camera" );
	mCamera->setNearClipDistance(12.0f);
	mCamera->setFarClipDistance(10000);
	mCamera->setFOVy(Radian(Degree(50.0f)));

	cameraInitialPosS5 = Vector3( 0.0f, 0.0f, 0.0f );
	cameraInitialRotS5 = Quaternion(Degree(0.0f), Vector3::UNIT_Y);
	/*mCamera->setPosition(cameraInitialPosS5);
	mCamera->setOrientation(cameraInitialRotS5);*/
	
	combatOverlayMgr   = OverlayManager::getSingletonPtr();
	combatPowerMeterOverlay = combatOverlayMgr->getByName("Overlay/PowerMeter");	
	combatPowerBar1Overlay = combatOverlayMgr->getByName("Overlay/PowerBar1");	
	combatPowerBar2Overlay = combatOverlayMgr->getByName("Overlay/PowerBar2");	
	combatPowerBar3Overlay = combatOverlayMgr->getByName("Overlay/PowerBar3");	
	combatPowerBar4Overlay = combatOverlayMgr->getByName("Overlay/PowerBar4");	
	combatPowerBar5Overlay = combatOverlayMgr->getByName("Overlay/PowerBar5");	

	combatPowerMeterOverlay->show();	

	// create directional light
	mainLight1 = mSceneMgr->createLight("MainLight1");
	mainLight1->setType(Light::LT_POINT);
	mainLight1->setCastShadows(false);

	// create directional light
	mainLight2 = mSceneMgr->createLight("MainLight2");
	mainLight2->setType(Light::LT_DIRECTIONAL);
	mainLight2->setDirection(Vector3( 1, -1, 1 )); 
	mainLight2->setCastShadows(false);

	mainLight3 = mSceneMgr->createLight("MainLight3");
	mainLight3->setType(Light::LT_DIRECTIONAL);
	mainLight3->setDirection( -1, 0, -1 ); 
	mainLight3->setCastShadows(false);	

	mainLight4 = mSceneMgr->createLight("MainLight4");
	mainLight4->setType(Light::LT_DIRECTIONAL);
	mainLight4->setDirection( 0, 1, 0 ); 
	mainLight4->setCastShadows(false);	

	mWindow		  = GameManager::getSingletonPtr()->getRenderWindow();
	mWindow->addViewport( mCamera );

	// Floor
	arenaLevelEntity = mSceneMgr->createEntity("Cells", "VGD_Alone_lvl_arena.mesh");
	arenaLevelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Cells" );
	arenaLevelNode->translate(Vector3( 0.0f, 0.0f, 0.0f ));
    arenaLevelNode->attachObject(arenaLevelEntity);
	mEntitiesS5.push_back(arenaLevelEntity);
	mSceneNodesS5.push_back(arenaLevelNode);

	arenaCollisionLevelEntity = mSceneMgr->createEntity("Collision", "ArenaCollision.mesh");
	arenaCollisionLevelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Collision" );
	arenaCollisionLevelNode->translate(Vector3( 0.0f, 0.0f, 0.0f ));
    arenaCollisionLevelNode->attachObject(arenaCollisionLevelEntity);
	arenaCollisionLevelNode->setVisible(false);
	mEntitiesS5.push_back(arenaCollisionLevelEntity);
	mSceneNodesS5.push_back(arenaCollisionLevelNode);

	 // Create RaySceneQuery to keep Robots on the floor
	raySceneQuery1S5 = mSceneMgr->createRayQuery(Ray());
	raySceneQuery2S5 = mSceneMgr->createRayQuery(Ray());
	forwardCollisionS5 = mSceneMgr->createRayQuery(Ray());
	AI1CollisionS5 = mSceneMgr->createRayQuery(Ray());
	backwardCollisionS5 = mSceneMgr->createRayQuery(Ray());
	leftCollisionS5 = mSceneMgr->createRayQuery(Ray());
	rightCollisionS5 = mSceneMgr->createRayQuery(Ray());

	ShirtEntity = mSceneMgr->createEntity("Shirt", "VGD_Alone_Char_MainShirt.mesh");
	ShirtlessEntity = mSceneMgr->createEntity("Shirtless", "VGD_Alone_char_playershirtless.mesh");
	PlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
	PlayerNode->setPosition(Ogre::Vector3(-100.0f, 0.0f, 0.0f));
	PlayerNode->yaw(Ogre::Degree(180.0f));
  
	mEntitiesS5.push_back(ShirtEntity);
	mEntitiesS5.push_back(ShirtlessEntity);
	mSceneNodesS5.push_back(PlayerNode);

	loadCharacter();

	//robot_anim1S5->setLoop( false );
    //robot_anim1S5->setEnabled( false );
	if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "combatStance" );}
	else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "combatStance" );}
    robot_anim1S5->setLoop( true );
    robot_anim1S5->setEnabled( true );

	//main camera node//
	cameraNodeS5 = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("state5CameraNode");
	//behind
	cameraNodeS5->setPosition( 0, 90,-60);
	//side
	//cameraNodeS5->setPosition( -60, 90,0);
	cameraNodeS5->attachObject(mCamera);
	mSceneNodesS5.push_back(cameraNodeS5);

	mainLightNodeS5 = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("mainLightNode");
	mainLightNodeS5->setPosition( Vector3(0,70,-15) );
	mainLightNodeS5->attachObject( mainLight1 );

	/*collision_EntityS5 = mSceneMgr->createEntity( "cube", "cube.mesh" );*/
	objectCollisionNodeS5 = mSceneMgr->getSceneNode("PlayerNode")->createChildSceneNode("objectCollisionNodeS5");
    objectCollisionNodeS5->setPosition( 0, 30,35);
	objectCollisionNodeS5->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	/*objectCollisionNodeS5->attachObject(collision_EntityS5);*/
	objectCollisionNodeS5->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	objectCollisionNodeS5->setVisible(false);
	//mEntitiesS5.push_back(collision_EntityS5);

	//collision nodes//
	forwardCollisionNodeS5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("forwardCollisionNodeS5");
	backwardCollisionNodeS5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("backwardCollisionNodeS5");
	leftCollisionNodeS5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("leftCollisionNodeS5");
	rightCollisionNodeS5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("rightCollisionNodeS5");
	
	mSceneNodesS5.push_back(forwardCollisionNodeS5);
	mSceneNodesS5.push_back(backwardCollisionNodeS5);
	mSceneNodesS5.push_back(leftCollisionNodeS5);
	mSceneNodesS5.push_back(rightCollisionNodeS5);

	AI_Entity1S5 = mSceneMgr->createEntity( "DannyBoy", "dougieModel.mesh" );
	AI_Entity1S5->setCastShadows(false);
	AI_Node1S5 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "DannyBoy" );
	AI_Node1S5->translate(Vector3( 100.0f, 0.0f, 0.0f ));
    AI_Node1S5->attachObject( AI_Entity1S5 );
	AI_Node1S5->rotate(Vector3::UNIT_Y, Degree(-90.0f));

	mEntitiesS5.push_back(AI_Entity1S5);
	mSceneNodesS5.push_back(AI_Node1S5);

	/*opponentCollision_EntityS5  = mSceneMgr->createEntity( "opponentCube", "cube.mesh" );*/
	opponentObjectCollisionNodeS5 = mSceneMgr->getSceneNode("DannyBoy")->createChildSceneNode("opponentObjectCollisionNodeS5");
    opponentObjectCollisionNodeS5->setPosition( 0, 30,35);
	opponentObjectCollisionNodeS5->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	/*opponentObjectCollisionNodeS5->attachObject(opponentCollision_EntityS5 );*/
	opponentObjectCollisionNodeS5->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	opponentObjectCollisionNodeS5->setVisible(false);
	/*mEntitiesS5.push_back(opponentCollision_EntityS5);*/
	
	/*rubberBandEntity  = mSceneMgr->createEntity( "rubberBandEntity", "cube.mesh" );*/
	rubberBandNode = mSceneMgr->getSceneNode("DannyBoy")->createChildSceneNode("rubberBandNode");
    rubberBandNode->setPosition( 0, 30, 150);
	rubberBandNode->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	/*rubberBandNode->attachObject(rubberBandEntity);*/
	rubberBandNode->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	rubberBandNode->setVisible(false);
	/*mEntitiesS5.push_back(rubberBandEntity);*/

	/*rubberBandInsideEntity  = mSceneMgr->createEntity( "rubberBandInsideEntity", "cube.mesh" );*/
	rubberBandInsideNode = mSceneMgr->getSceneNode("DannyBoy")->createChildSceneNode("rubberBandInsideNode");
    rubberBandInsideNode->setPosition( 0, 30, 20);
	rubberBandInsideNode->scale(Vector3( 0.05f, 0.05f, 0.05f ));
	/*rubberBandInsideNode->attachObject(rubberBandInsideEntity);*/
	rubberBandInsideNode->rotate(Vector3::UNIT_Y, Degree(-45.0f));
	rubberBandInsideNode->setVisible(false);
	/*mEntitiesS5.push_back(rubberBandInsideEntity);*/

	AI_Node1S5->setAutoTracking(true,PlayerNode,Vector3::UNIT_Z);
	AI_Node1S5->setFixedYawAxis(true);

	AI_anim1S5 = AI_Entity1S5->getAnimationState( "frontCombatWalk" );
    AI_anim1S5->setLoop( true );
    AI_anim1S5->setEnabled( true );

	// Add joystick 0 and 1
	mGamePadsS5.push_back(new GamePadController(0, 4000));
	mGamePadsS5.push_back(new GamePadController(1, 4000));

	previous_position = cameraNodeS5->getPosition();

	initHUD();
	
	mCamera->yaw(Radian(67.5));
	PlayerNode->yaw(Radian(67.5));

	AI1MoveDirSS5 = 5;
	AI1MaxMoveDurationSS5 = 50;
	AI1MoveDurationSS5 = 0;

	///////////////////////////////
	//INITIALIZES FMOD FOR COMBAT//
	///////////////////////////////

	channel1 = 0;
	channel2 = 0;
	channel3 = 0;

	audio_result = FMOD::System_Create(&system);
	ERRCHECK(audio_result);

	audio_result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(audio_result);

	audio_result = system->createStream("Sounds/Music/Combat.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &audio_track);
	ERRCHECK(audio_result);

	if(combatMusicSetting)
	{
		audio_result = system->playSound(FMOD_CHANNEL_FREE, audio_track, false, &channel1);
		ERRCHECK(audio_result);
		channel1->setVolume(0.5);
	}

	/////////////////////////
	//KICK AND PUNCH SOUNDS//
	/////////////////////////
	audio_result = system->createSound("Sounds/SFX/Fight/Impact/Impact5.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &left_Punch_Sound);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Impact/Impact6.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &right_Punch_Sound);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Impact/Impact4.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &left_Kick_Sound);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Impact/Impact7.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &right_Kick_Sound);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Swipe/Swipe2.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &attack_Miss_Sound);
	ERRCHECK(audio_result);

	/////////////////////////
	//OPPONENT GRUNT SOUNDS//
	/////////////////////////
	audio_result = system->createSound("Sounds/SFX/Fight/Grunt1/Grunt1a.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &opponent_Kicked_Grunt_Sound);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Grunt2/Gunt2e.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &opponent_Punched_Grunt_Sound);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Grunt2/Gunt2a.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &opponent_Grapple_Grunt_Sound);
	ERRCHECK(audio_result);

	///////////////////////
	//PLAYER GRUNT SOUNDS//
	///////////////////////
	audio_result = system->createSound("Sounds/SFX/Fight/Grunt4/Grunt4d.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &player_Kicked_Grunt_Sound);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Grunt3/Gunt3b.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &player_Punched_Grunt_Sound);
	ERRCHECK(audio_result);

	audio_result = system->createSound("Sounds/SFX/Fight/Grunt3/Grunt3f.wav", FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE, 0, &player_Grapple_Grunt_Sound);
	ERRCHECK(audio_result);

	
}
void SampleState5::initHUD( void )
{
	SampleState5OverlayMgr = Ogre::OverlayManager::getSingletonPtr();
	SampleState5FontMgr = Ogre::FontManager::getSingletonPtr();

	HUDPanel = static_cast<OverlayContainer*>(SampleState5OverlayMgr->createOverlayElement("Panel", "CardsNumbers"));
	HUDPanel->setMetricsMode(Ogre::GMM_PIXELS);
	HUDPanel->setPosition(0, 0);

	x_AxisTextBox = static_cast<TextAreaOverlayElement*>(SampleState5OverlayMgr->createOverlayElement("TextArea", "Player1Name"));
	x_AxisTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	x_AxisTextBox->setPosition(10, 20);
	x_AxisTextBox->setFontName("MyFont");
	x_AxisTextBox->setCharHeight(30);
	x_AxisTextBox->setColour(Ogre::ColourValue(1.0, 0.0, 0.0, 1.0));
	//x_AxisTextBox->setCaption("X-Axis");

	y_AxisTextBox = static_cast<TextAreaOverlayElement*>(SampleState5OverlayMgr->createOverlayElement("TextArea", "Player1Money"));
	y_AxisTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	y_AxisTextBox->setPosition(10, 60);
	y_AxisTextBox->setFontName("MyFont");
	y_AxisTextBox->setCharHeight(30);
	y_AxisTextBox->setColour(Ogre::ColourValue(0.0, 1.0, 0.0, 1.0)); 
	//y_AxisTextBox->setCaption("Y-Axis");

	z_AxisTextBox = static_cast<TextAreaOverlayElement*>(SampleState5OverlayMgr->createOverlayElement("TextArea", "Player1Status"));
	z_AxisTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	z_AxisTextBox->setPosition(10, 100);
	z_AxisTextBox->setFontName("MyFont");
	z_AxisTextBox->setCharHeight(30);
	z_AxisTextBox->setColour(Ogre::ColourValue(0.0, 0.0, 1.0, 1.0)); 
	//z_AxisTextBox->setCaption("Z-Axis");

	otherTextBox = static_cast<TextAreaOverlayElement*>(SampleState5OverlayMgr->createOverlayElement("TextArea", "Player1BetAmount"));
	otherTextBox->setMetricsMode(Ogre::GMM_PIXELS);
	otherTextBox->setPosition(10, 140);
	otherTextBox->setFontName("MyFont");
	otherTextBox->setCharHeight(30);
	otherTextBox->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0)); 
	//otherTextBox->setCaption("Test");

	HUDPanel->addChild(x_AxisTextBox);
	HUDPanel->addChild(y_AxisTextBox);
	HUDPanel->addChild(z_AxisTextBox);
	HUDPanel->addChild(otherTextBox);

	HUDOverlay = SampleState5OverlayMgr->create("BlankOverlay");
	HUDOverlay->add2D(HUDPanel);

	HUDOverlay->show();
}
void SampleState5::do_focus( void )
{
	mInputManager = InputManager::getSingletonPtr();
	mInputManager->addKeyListener(this, mClassName);
	mInputManager->addJoystickListener(this, mClassName);
}

void SampleState5::do_defocus( void )
{
	mInputManager->removeKeyListener(this);
	mInputManager->removeJoystickListener(this);
}

void SampleState5::exit( void ) 
{

	currentPower = 0;
	combatPowerBar1Overlay->hide();
	combatPowerBar2Overlay->hide();
	combatPowerBar3Overlay->hide();
	combatPowerBar4Overlay->hide();
	combatPowerBar5Overlay->hide();

	playerHealth = 10;
	opponentHealth = 10;

	for(SceneNodeList::iterator it = mSceneNodesS5.begin(); it != mSceneNodesS5.end(); it++)
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode((*it)->getName());
	}
	mSceneNodesS5.clear();

	for(EntityList::iterator it = mEntitiesS5.begin(); it != mEntitiesS5.end(); it++)
	{
		Ogre::SceneNode* parent = (*it)->getParentSceneNode();
		if(parent)
		{
			parent->detachAllObjects();
		}
		mSceneMgr->destroyEntity(*it);
	}
	mEntitiesS5.clear();

	pitchS5 = 0;
	SampleState5OverlayMgr->destroyOverlayElement(HUDPanel);
	SampleState5OverlayMgr->destroyOverlayElement(otherTextBox);
	SampleState5OverlayMgr->destroyOverlayElement(z_AxisTextBox);
	SampleState5OverlayMgr->destroyOverlayElement(y_AxisTextBox);
	SampleState5OverlayMgr->destroyOverlayElement(x_AxisTextBox);
	SampleState5OverlayMgr->destroy(HUDOverlay);

		//destroy lights in scene
	mSceneMgr->destroyLight("MainLight1");
	mSceneMgr->destroyLight("MainLight2");
	mSceneMgr->destroyLight("MainLight3");
	mSceneMgr->destroyLight("MainLight4");

	mSceneMgr->destroySceneNode("objectCollisionNodeS5");
	mSceneMgr->destroySceneNode("opponentObjectCollisionNodeS5");
	mSceneMgr->destroySceneNode("rubberBandNode");
	mSceneMgr->destroySceneNode("rubberBandInsideNode");
	mSceneMgr->destroySceneNode("mainLightNode");
	mSceneMgr->destroyCamera(mCamera);
    mWindow->removeViewport(0);


}

void SampleState5::update( Real seconds ) 
{
	mTime += seconds;

	//UPDATES FMOD
	system->update();	

	// Animate robots
	robot_anim1S5->addTime( seconds );
	AI_anim1S5->addTime( seconds );

	
//	objectCollisionNodeS5->setPosition(PlayerNode->getOrientation() * (Vector3(150,0,0)) );
	//x_AxisTextBox->setCaption(itoa(PlayerNode->getWorldPosition().x, buffer, 10));
	//y_AxisTextBox->setCaption(itoa(PlayerNode->getWorldPosition().y, buffer, 10));
	//z_AxisTextBox->setCaption(itoa(PlayerNode->getWorldPosition().z, buffer, 10));
	//otherTextBox->setCaption("Combat");
	
	if(keyboardControlsActiveS5)
	{
		keyboardMouseCharacterMovement();
	}
	else if(gamepadControlsActiveS5)
	{
		gamepadCharacterMovement( seconds );
	}
	
	updateAI1();

	
	//mCamera->setPosition(PlayerNode->getWorldPosition().x, PlayerNode->getWorldPosition().y + 68.0f,PlayerNode->getWorldPosition().z);
	mCamera->lookAt(PlayerNode->getWorldPosition().x, PlayerNode->getWorldPosition().y + 45.0f + pitchS5,PlayerNode->getWorldPosition().z);

	if(opponentHealth <= 0)
	{
		//AI_Node1S5->translate(AI_Node1S5->getOrientation()*-(Vector3::UNIT_Z*0.5));

		if(!opponentSetAnimation)
		{
			AI_anim1S5->setLoop( false );
			AI_anim1S5->setEnabled( false );
			AI_anim1S5 = AI_Entity1S5->getAnimationState( "fallDown" );
			AI_anim1S5->setTimePosition(0);
			AI_anim1S5->setLoop( false );
			AI_anim1S5->setEnabled( true );
			opponentSetAnimation = true;
			AI_Node1S5->setAutoTracking(false,PlayerNode,Vector3::UNIT_Z);
		}

		if(robot_anim1S5->hasEnded() )
		{
			playerLeftPunch = false;
			playerRightPunch = false;
			playerLeftKick = false;
			playerRightKick = false;
			playerGrapple = false;
			playerHit = false;
			playerSupermanPunch = false;
			playerRoundhouseKick = false;
			setAnimation = false;
			shiftPressed = false;
			controlPressed = false;
			spacePressed = false;

			opponentHit = false;
			opponentSetAnimation = false;

			wait(3);
		
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("CreditsUI"));

		}
	}

	if(playerHealth <= 0)	
	{
		if(!setAnimation)
		{
			robot_anim1S5->setLoop( false );
			robot_anim1S5->setEnabled( false );
			if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "fallDown" );}
			else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "fallDown" );}
			robot_anim1S5->setTimePosition(0);
			robot_anim1S5->setLoop( false );
			robot_anim1S5->setEnabled( true );
			setAnimation = true;
		}
		//fade to black
		if(robot_anim1S5->hasEnded() )
		{
			playerLeftPunch = false;
			playerRightPunch = false;
			playerLeftKick = false;
			playerRightKick = false;
			playerGrapple = false;
			playerHit = false;
			playerSupermanPunch = false;
			playerRoundhouseKick = false;
			setAnimation = false;
			shiftPressed = false;
			controlPressed = false;
			spacePressed = false;

			opponentHit = false;
			opponentSetAnimation = false;

			wait(3);
		
			GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
			GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("CreditsUI"));

		}
	}

	
	

}
void SampleState5::wait( int seconds )
{
	clock_t endWait;
	
	endWait = clock () + seconds * CLOCKS_PER_SEC ;
	while(clock() < endWait) {}

}
void SampleState5::npcFacePlayer( Ogre::SceneNode* node, Ogre::Camera* Camera )
{
	//node->lookAt(PlayerNode->Vector3::UNIT_Y,PlayerNode->TS_WORLD,Vector3::UNIT_Z);
}
void SampleState5::updateAI1()
{
	
	if(opponentHealth > 0)
	{
		if(playerGrapple)
		{
			AI1MoveDurationSS5--;
		}
		
		if(opponentHit)
		{
			//AI_Node1S5->translate(AI_Node1S5->getOrientation()*-(Vector3::UNIT_Z*0.5));

			if(!opponentSetAnimation)
			{
				AI_anim1S5->setLoop( false );
				AI_anim1S5->setEnabled( false );
				AI_anim1S5 = AI_Entity1S5->getAnimationState( "hit" );
				AI_anim1S5->setTimePosition(0);
				AI_anim1S5->setLoop( false );
				AI_anim1S5->setEnabled( true );
				opponentSetAnimation = true;
			}

			if(AI_anim1S5->hasEnded())
			{
				opponentHit = false;
				opponentSetAnimation = false;
				AI1MoveDurationSS5 = AI1MaxMoveDurationSS5;
				opponentHealth--;
			}
		}
		else if(!opponentHit)
		{
			
			if(AI1MoveDurationSS5 < AI1MaxMoveDurationSS5 )
			{
				if( rubberBandNode->getWorldPosition().x + 10 > PlayerNode->getWorldPosition().x   )
				{
					if( rubberBandNode->getWorldPosition().x  - 10 < PlayerNode->getWorldPosition().x )
					{
						if( rubberBandNode->getWorldPosition().z  + 10 > PlayerNode->getWorldPosition().z )
						{
							if( rubberBandNode->getWorldPosition().z  - 10 < PlayerNode->getWorldPosition().z )
							{
								AI1MoveDirSS5 = 1;
								AI1MaxMoveDurationSS5 = 80;
								AI1MoveDurationSS5 = 0;
							}
						}
					}
				}

				if( rubberBandInsideNode->getWorldPosition().x + 10 > PlayerNode->getWorldPosition().x   )
				{
					if( rubberBandInsideNode->getWorldPosition().x  - 10 < PlayerNode->getWorldPosition().x )
					{
						if( rubberBandInsideNode->getWorldPosition().z  + 10 > PlayerNode->getWorldPosition().z )
						{
							if( rubberBandInsideNode->getWorldPosition().z  - 10 < PlayerNode->getWorldPosition().z )
							{
								AI1MoveDirSS5 = 2;
								AI1MaxMoveDurationSS5 = 40;
								AI1MoveDurationSS5 = 0;
							}
						}
					}
				}

				if(AI1MoveDirSS5 == 1) 
				{
					AI_anim1S5->setLoop( false );
					AI_anim1S5->setEnabled( false );
					AI_anim1S5 = AI_Entity1S5->getAnimationState( "frontCombatWalk" );
					AI_anim1S5->setLoop( true );
					AI_anim1S5->setEnabled( true );

					moveAIForward( AI_Node1S5 ); 
				}
				else if(AI1MoveDirSS5 == 2) 
				{
					AI_anim1S5->setLoop( false );
					AI_anim1S5->setEnabled( false );
					AI_anim1S5 = AI_Entity1S5->getAnimationState( "backCombatWalk" );
					AI_anim1S5->setLoop( true );
					AI_anim1S5->setEnabled( true );

					moveAIBackward( AI_Node1S5 );
				}
				else if(AI1MoveDirSS5 == 3) 
				{
					AI_anim1S5->setLoop( false );
					AI_anim1S5->setEnabled( false );
					AI_anim1S5 = AI_Entity1S5->getAnimationState( "frontCombatWalk" );
					AI_anim1S5->setLoop( true );
					AI_anim1S5->setEnabled( true );

					moveAILeft( AI_Node1S5 ); 
				}
				else if(AI1MoveDirSS5 == 4) 
				{
					AI_anim1S5->setLoop( false );
					AI_anim1S5->setEnabled( false );
					AI_anim1S5 = AI_Entity1S5->getAnimationState( "backCombatWalk" );
					AI_anim1S5->setLoop( true );
					AI_anim1S5->setEnabled( true );

					moveAIRight( AI_Node1S5 ); 
				}
				else if(AI1MoveDirSS5 == 5) 
				{
					if(!opponentSetAnimation)
					{
						// Set animation
						AI_anim1S5->setLoop( false );
						AI_anim1S5->setEnabled( false );
						AI_anim1S5 = AI_Entity1S5->getAnimationState( "leftPunch" );
						AI_anim1S5->setTimePosition(0);
						AI_anim1S5->setLoop( false );
						AI_anim1S5->setEnabled( true );
						opponentSetAnimation = true;

						//PLAYS THE SOUND FOR A MISS HIT
						if(combatSFXSetting)
						{
							audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
							ERRCHECK(audio_result);
						}
					}

					if(AI_anim1S5->getTimePosition() > (AI_anim1S5->getLength() / 2) && !playerHit)
					{
						if( opponentObjectCollisionNodeS5->getWorldPosition().x + 10 > PlayerNode->getWorldPosition().x   )
						{
							if( opponentObjectCollisionNodeS5->getWorldPosition().x  - 10 < PlayerNode->getWorldPosition().x )
							{
								if( opponentObjectCollisionNodeS5->getWorldPosition().z  + 10 > PlayerNode->getWorldPosition().z )
								{
									if( opponentObjectCollisionNodeS5->getWorldPosition().z  - 10 < PlayerNode->getWorldPosition().z )
									{
										playerHit = true;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, left_Punch_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, player_Punched_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}

					if(AI_anim1S5->hasEnded())
					{
						opponentSetAnimation = false;
						AI1MoveDurationSS5 = AI1MaxMoveDurationSS5;
					}
				}
				else if(AI1MoveDirSS5 == 6) 
				{
					if(!opponentSetAnimation)
					{
						// Set animation
						AI_anim1S5->setLoop( false );
						AI_anim1S5->setEnabled( false );
						AI_anim1S5 = AI_Entity1S5->getAnimationState( "rightPunch" );
						AI_anim1S5->setTimePosition(0);
						AI_anim1S5->setLoop( false );
						AI_anim1S5->setEnabled( true );
						opponentSetAnimation = true;

						//PLAYS THE SOUND FOR A MISS HIT
						if(combatSFXSetting)
						{
							audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
							ERRCHECK(audio_result);
						}
					}

					if(AI_anim1S5->getTimePosition() > (AI_anim1S5->getLength() / 2) && !playerHit)
					{
						if( opponentObjectCollisionNodeS5->getWorldPosition().x + 10 > PlayerNode->getWorldPosition().x   )
						{
							if( opponentObjectCollisionNodeS5->getWorldPosition().x  - 10 < PlayerNode->getWorldPosition().x )
							{
								if( opponentObjectCollisionNodeS5->getWorldPosition().z  + 10 > PlayerNode->getWorldPosition().z )
								{
									if( opponentObjectCollisionNodeS5->getWorldPosition().z  - 10 < PlayerNode->getWorldPosition().z )
									{
										playerHit = true;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, left_Punch_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, player_Punched_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}

					if(AI_anim1S5->hasEnded())
					{
						opponentSetAnimation = false;
						AI1MoveDurationSS5 = AI1MaxMoveDurationSS5;
					}
				}
				else if(AI1MoveDirSS5 == 7) 
				{
					if(!opponentSetAnimation)
					{
						// Set animation
						AI_anim1S5->setLoop( false );
						AI_anim1S5->setEnabled( false );
						AI_anim1S5 = AI_Entity1S5->getAnimationState( "leftKick" );
						AI_anim1S5->setTimePosition(0);
						AI_anim1S5->setLoop( false );
						AI_anim1S5->setEnabled( true );
						opponentSetAnimation = true;

						//PLAYS THE SOUND FOR A MISS HIT
						if(combatSFXSetting)
						{
							audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
							ERRCHECK(audio_result);
						}
					}

					if(AI_anim1S5->getTimePosition() > (AI_anim1S5->getLength() / 2) && !playerHit)
					{
						if( opponentObjectCollisionNodeS5->getWorldPosition().x + 10 > PlayerNode->getWorldPosition().x   )
						{
							if( opponentObjectCollisionNodeS5->getWorldPosition().x  - 10 < PlayerNode->getWorldPosition().x )
							{
								if( opponentObjectCollisionNodeS5->getWorldPosition().z  + 10 > PlayerNode->getWorldPosition().z )
								{
									if( opponentObjectCollisionNodeS5->getWorldPosition().z  - 10 < PlayerNode->getWorldPosition().z )
									{
										playerHit = true;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, right_Kick_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, player_Kicked_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}

					if(AI_anim1S5->hasEnded())
					{
						opponentSetAnimation = false;
						AI1MoveDurationSS5 = AI1MaxMoveDurationSS5;
					}
				}
				else if(AI1MoveDirSS5 == 8) 
				{
					if(!opponentSetAnimation)
					{
						// Set animation
						AI_anim1S5->setLoop( false );
						AI_anim1S5->setEnabled( false );
						AI_anim1S5 = AI_Entity1S5->getAnimationState( "rightKick" );
						AI_anim1S5->setTimePosition(0);
						AI_anim1S5->setLoop( false );
						AI_anim1S5->setEnabled( true );
						opponentSetAnimation = true;

						//PLAYS THE SOUND FOR A MISS HIT
						if(combatSFXSetting)
						{
							audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
							ERRCHECK(audio_result);
						}
					}

					if(AI_anim1S5->getTimePosition() > (AI_anim1S5->getLength() / 2) && !playerHit)
					{
						if( opponentObjectCollisionNodeS5->getWorldPosition().x + 10 > PlayerNode->getWorldPosition().x   )
						{
							if( opponentObjectCollisionNodeS5->getWorldPosition().x  - 10 < PlayerNode->getWorldPosition().x )
							{
								if( opponentObjectCollisionNodeS5->getWorldPosition().z  + 10 > PlayerNode->getWorldPosition().z )
								{
									if( opponentObjectCollisionNodeS5->getWorldPosition().z  - 10 < PlayerNode->getWorldPosition().z )
									{
										playerHit = true;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, right_Kick_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, player_Kicked_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}

					if(AI_anim1S5->hasEnded())
					{
						opponentSetAnimation = false;
						AI1MoveDurationSS5 = AI1MaxMoveDurationSS5;
					}
				}
			}
			else if(AI1MoveDurationSS5 >= AI1MaxMoveDurationSS5 )
			{
				srand( ( rand() % 140 ) );
				if(AI1MoveDirSS5 > 4)
				{
					randomNumSS5 = ( rand() % 60 ) + 1;
				}
				else if(AI1MoveDirSS5 < 5)
				{
					randomNumSS5 = ( rand() % 80 ) + 61;
				}
				if(randomNumSS5 <= 30)
				{
					AI1MoveDirSS5 = 1;
					AI1MaxMoveDurationSS5 = 50;
				}
				else if(randomNumSS5 >=31 && randomNumSS5 <=36)
				{
					AI1MoveDirSS5 = 2;
					AI1MaxMoveDurationSS5 = 10;
				}
				else if(randomNumSS5 >= 37 && randomNumSS5 <= 38)
				{
					AI1MoveDirSS5 = 3;
					AI1MaxMoveDurationSS5 = 5;
				}
				else if(randomNumSS5 >= 39 && randomNumSS5 <= 40)
				{
					AI1MoveDirSS5 = 4;
					AI1MaxMoveDurationSS5 = 5;
				}
				else if(randomNumSS5 >=61 && randomNumSS5 <= 80)
				{
					AI1MoveDirSS5 = 5;
					opponentSetAnimation = false;
					AI1MaxMoveDurationSS5 = 100;
				}
				else if(randomNumSS5 >=81 && randomNumSS5 <= 100)
				{
					AI1MoveDirSS5 = 6;
					opponentSetAnimation = false;
					AI1MaxMoveDurationSS5 = 100;
				}
				else if(randomNumSS5 >=101 && randomNumSS5 <= 120)
				{
					AI1MoveDirSS5 = 7;
					opponentSetAnimation = false;
					AI1MaxMoveDurationSS5 = 100;
				}
				else if(randomNumSS5 >=121 && randomNumSS5 <= 140)
				{
					AI1MoveDirSS5 = 8;
					opponentSetAnimation = false;
					AI1MaxMoveDurationSS5 = 100;
				}
				
				AI1MoveDurationSS5 = 0;
				//y_AxisTextBox->setCaption(itoa(randomNumSS5, buffer, 10));
			}

			Vector3 forwardResult_1;

			if(RaycastFromPointS5(AI1CollisionS5, AI_Entity1S5, AI_Node1S5->getPosition(), AI_Node1S5->getOrientation() * (Vector3::UNIT_Z), forwardResult_1))
			{
				Vector3 forwardDistance = AI_Node1S5->getPosition() - forwardResult_1;
				if(forwardDistance.length() < 30.0f )
				{
					AI1MoveDirSS5 = 2;
				}
			}
			if(RaycastFromPointS5(AI1CollisionS5, AI_Entity1S5, AI_Node1S5->getPosition(), AI_Node1S5->getOrientation() * -(Vector3::UNIT_Z), forwardResult_1))
			{
				Vector3 forwardDistance = AI_Node1S5->getPosition() - forwardResult_1;
				if(forwardDistance.length() < 30.0f )
				{
					AI1MoveDirSS5 = 1;
				}
			}
			if(RaycastFromPointS5(AI1CollisionS5, AI_Entity1S5, AI_Node1S5->getPosition(), AI_Node1S5->getOrientation() * (Vector3::UNIT_X), forwardResult_1))
			{
				Vector3 forwardDistance = AI_Node1S5->getPosition() - forwardResult_1;
				if(forwardDistance.length() < 30.0f )
				{
					AI1MoveDirSS5 = 1;
				}
			}
			if(RaycastFromPointS5(AI1CollisionS5, AI_Entity1S5, AI_Node1S5->getPosition(), AI_Node1S5->getOrientation() * -(Vector3::UNIT_X), forwardResult_1))
			{
				Vector3 forwardDistance = AI_Node1S5->getPosition() - forwardResult_1;
				if(forwardDistance.length() < 30.0f )
				{
					AI1MoveDirSS5 = 1;
				}
			}

			
		}
		AI1MoveDurationSS5++;
	}
	

}

void SampleState5::moveAIForward( Ogre::SceneNode *node  )
{
	node->translate(node->getOrientation()*(Vector3::UNIT_Z*1));
}
void SampleState5::moveAIRight( Ogre::SceneNode *node )
{
	node->translate(node->getOrientation()*(Vector3::UNIT_X*1));
}
void SampleState5::moveAILeft( Ogre::SceneNode *node)
{
	node->translate(node->getOrientation()*-(Vector3::UNIT_X*1));
}
void SampleState5::moveAIBackward( Ogre::SceneNode *node )
{
	node->translate(node->getOrientation()*-(Vector3::UNIT_Z*1));
}
void SampleState5::keyboardMouseCharacterMovement()
{
	
	
	if(playerHealth > 0)
	{
		//updates the orientation of the collision nodes so they face the right way//
		forwardCollisionNodeS5->setOrientation(PlayerNode->getOrientation());
		backwardCollisionNodeS5->setOrientation(PlayerNode->getOrientation());
		leftCollisionNodeS5->setOrientation(PlayerNode->getOrientation());
		rightCollisionNodeS5->setOrientation(PlayerNode->getOrientation());

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

		
		if(ms.buttonDown(OIS::MouseButtonID(0)) && spacePressed && shiftPressed && currentPower > 3)
		{
			playerRoundhouseKick = true;
		}
		else if(ms.buttonDown(OIS::MouseButtonID(0)) && spacePressed && !shiftPressed && currentPower > 3)
		{
			playerSupermanPunch = true;
		}
		else if(ms.buttonDown(OIS::MouseButtonID(0)) && controlPressed)
		{
			playerGrapple = true;
		}
		else if(ms.buttonDown(OIS::MouseButtonID(0)) && !shiftPressed)
		{
			playerLeftPunch = true;
		}
		else if(ms.buttonDown(OIS::MouseButtonID(1)) && !shiftPressed)
		{
			playerRightPunch = true;
		}
		else if(ms.buttonDown(OIS::MouseButtonID(0)) && shiftPressed)
		{
			playerLeftKick = true;
		}
		else if(ms.buttonDown(OIS::MouseButtonID(1)) && shiftPressed)
		{
			playerRightKick = true;
		}
		
		//finding rotation//
		if(mouseMoveY.operator > (Radian(0)) && pitchS5 < 30 )
		{
			if(keyboardLookInvertedS5){ mCamera->pitch(Degree(1)); pitchS5++;}
			else if(!keyboardLookInvertedS5){ mCamera->pitch(Degree(-1)); pitchS5++;}
		}
		else if(mouseMoveY.operator < (Radian(0)) && pitchS5 > -5)
		{
			if(keyboardLookInvertedS5){ mCamera->pitch(Degree(-1)); pitchS5--;}
			else if(!keyboardLookInvertedS5){ mCamera->pitch(Degree(1)); pitchS5--;}
		}

		if(playerHit)
		{
			//PlayerNode->translate(PlayerNode->getOrientation()*-(Vector3::UNIT_Z*0.25));

			if(!setAnimation)
			{
				robot_anim1S5->setLoop( false );
				robot_anim1S5->setEnabled( false );
				if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "hit" );}
				else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "hit" );}
				robot_anim1S5->setTimePosition(0);
				robot_anim1S5->setLoop( false );
				robot_anim1S5->setEnabled( true );
				setAnimation = true;
			}

			if(robot_anim1S5->hasEnded() )
			{
				playerHit = false;
				setAnimation = false;
				playerHealth--;
			}
		}
		else 
		{
			if(playerSupermanPunch)
			{
				//PlayerNode->translate(PlayerNode->getOrientation()*(Vector3::UNIT_Z*0.5));
				
				if(setAnimation == false)
				{
					// Set animation
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( false );
					if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "supermanPunch" );}
					else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "supermanPunch" );}
					robot_anim1S5->setTimePosition(0);
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( true );
					setAnimation = true;
				}
				//PLAYS THE SOUND FOR A MISS HIT
				if(combatSFXSetting)
				{
					audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
					ERRCHECK(audio_result);
				}

				if(robot_anim1S5->getTimePosition() > (robot_anim1S5->getLength() / 2) && !opponentHit)
				{
					if( objectCollisionNodeS5->getWorldPosition().x + 20 > AI_Node1S5->getWorldPosition().x   )
					{
						if( objectCollisionNodeS5->getWorldPosition().x  - 20 < AI_Node1S5->getWorldPosition().x )
						{
							if( objectCollisionNodeS5->getWorldPosition().z  + 20 > AI_Node1S5->getWorldPosition().z )
							{
								if( objectCollisionNodeS5->getWorldPosition().z  - 20 < AI_Node1S5->getWorldPosition().z )
								{
									if(opponentHealth > 0)
									{
										opponentHit = true;
										opponentSetAnimation = false;
										increasePower();
										currentPower++;
										opponentHealth = opponentHealth - 3;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, left_Punch_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, opponent_Punched_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}
				}

				if(robot_anim1S5->hasEnded())
				{
					playerSupermanPunch = false;
					setAnimation = false;
					currentPower = 0;
					combatPowerBar1Overlay->hide();
					combatPowerBar2Overlay->hide();
					combatPowerBar3Overlay->hide();
					combatPowerBar4Overlay->hide();
					combatPowerBar5Overlay->hide();
				}

			}
			else if(playerRoundhouseKick)
			{
				//PlayerNode->translate(PlayerNode->getOrientation()*(Vector3::UNIT_Z*0.5));
				
				if(setAnimation == false)
				{
					// Set animation
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( false );
					if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "roundhouseKick" );}
					else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "roundhouseKick" );}
					robot_anim1S5->setTimePosition(0);
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( true );
					setAnimation = true;
				}
				//PLAYS THE SOUND FOR A MISS HIT
				if(combatSFXSetting)
				{
					audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
					ERRCHECK(audio_result);
				}

				if(robot_anim1S5->getTimePosition() > (robot_anim1S5->getLength() / 2) && !opponentHit)
				{
					if( objectCollisionNodeS5->getWorldPosition().x + 20 > AI_Node1S5->getWorldPosition().x   )
					{
						if( objectCollisionNodeS5->getWorldPosition().x  - 20 < AI_Node1S5->getWorldPosition().x )
						{
							if( objectCollisionNodeS5->getWorldPosition().z  + 20 > AI_Node1S5->getWorldPosition().z )
							{
								if( objectCollisionNodeS5->getWorldPosition().z  - 20 < AI_Node1S5->getWorldPosition().z )
								{
									if(opponentHealth > 0)
									{
										opponentHit = true;
										opponentSetAnimation = false;
										increasePower();
										currentPower++;
										opponentHealth = opponentHealth - 3;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, left_Punch_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, opponent_Punched_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}
				}

				if(robot_anim1S5->hasEnded())
				{
					playerRoundhouseKick = false;
					setAnimation = false;
					currentPower = 0;
					combatPowerBar1Overlay->hide();
					combatPowerBar2Overlay->hide();
					combatPowerBar3Overlay->hide();
					combatPowerBar4Overlay->hide();
					combatPowerBar5Overlay->hide();
				}

			}
			else if(playerGrapple)
			{
				if(setAnimation == false)
				{
					if(checkPlayerOpponentCollision())
					{
						// Set animation
						robot_anim1S5->setLoop( false );
						robot_anim1S5->setEnabled( false );
						if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "grapple" );}
						else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "grapple" );}
						robot_anim1S5->setTimePosition(0);
						robot_anim1S5->setLoop( false );
						robot_anim1S5->setEnabled( true );
						setAnimation = true;
					}
					else 
					{
						// Set animation
						robot_anim1S5->setLoop( false );
						robot_anim1S5->setEnabled( false );
						if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "grappleMiss" );}
						else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "grappleMiss" );}
						robot_anim1S5->setTimePosition(0);
						robot_anim1S5->setLoop( false );
						robot_anim1S5->setEnabled( true );
						setAnimation = true;
					}
				}

				if(robot_anim1S5->getTimePosition() > ((robot_anim1S5->getLength() / 5) * 3) && !opponentHit)
				{
					if( objectCollisionNodeS5->getWorldPosition().x + 10 > AI_Node1S5->getWorldPosition().x   )
					{
						if( objectCollisionNodeS5->getWorldPosition().x  - 10 < AI_Node1S5->getWorldPosition().x )
						{
							if( objectCollisionNodeS5->getWorldPosition().z  + 10 > AI_Node1S5->getWorldPosition().z )
							{
								if( objectCollisionNodeS5->getWorldPosition().z  - 10 < AI_Node1S5->getWorldPosition().z )
								{
									if(opponentHealth > 0)
									{
										opponentHit = true;
										opponentSetAnimation = false;
										playerGrapple = false;
										setAnimation = false;
										increasePower();
										currentPower++;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, left_Punch_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, opponent_Punched_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}
				}

				if(robot_anim1S5->hasEnded())
				{
					playerGrapple = false;
					setAnimation = false;
				}
			}
			else if(playerLeftPunch)
			{
				if(setAnimation == false)
				{
					// Set animation
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( false );
					if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "leftPunch" );}
					else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "leftPunch" );}
					robot_anim1S5->setTimePosition(0);
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( true );
					setAnimation = true;
				}
				//PLAYS THE SOUND FOR A MISS HIT
				if(combatSFXSetting)
				{
					audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
					ERRCHECK(audio_result);
				}

				if(robot_anim1S5->getTimePosition() > (robot_anim1S5->getLength() / 2) && !opponentHit)
				{
					if( objectCollisionNodeS5->getWorldPosition().x + 10 > AI_Node1S5->getWorldPosition().x   )
					{
						if( objectCollisionNodeS5->getWorldPosition().x  - 10 < AI_Node1S5->getWorldPosition().x )
						{
							if( objectCollisionNodeS5->getWorldPosition().z  + 10 > AI_Node1S5->getWorldPosition().z )
							{
								if( objectCollisionNodeS5->getWorldPosition().z  - 10 < AI_Node1S5->getWorldPosition().z )
								{
									if(opponentHealth > 0)
									{
										opponentHit = true;
										opponentSetAnimation = false;
										increasePower();
										currentPower++;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, left_Punch_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, opponent_Punched_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}
				}

				if(robot_anim1S5->hasEnded())
				{
					playerLeftPunch = false;
					setAnimation = false;
				}
			}
			else if(playerRightPunch)
			{
				if(setAnimation == false)
				{
					// Set animation
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( false );
					if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "rightPunch" );}
					else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "rightPunch" );}
					robot_anim1S5->setTimePosition(0);
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( true );
					setAnimation = true;
					//PLAYS THE SOUND FOR A MISS HIT
					if(combatSFXSetting)
					{
						audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
						ERRCHECK(audio_result);
					}
				}

				if(robot_anim1S5->getTimePosition() > (robot_anim1S5->getLength() / 2) && !opponentHit)
				{

					if( objectCollisionNodeS5->getWorldPosition().x + 10 > AI_Node1S5->getWorldPosition().x   )
					{
						if( objectCollisionNodeS5->getWorldPosition().x  - 10 < AI_Node1S5->getWorldPosition().x )
						{
							if( objectCollisionNodeS5->getWorldPosition().z  + 10 > AI_Node1S5->getWorldPosition().z )
							{
								if( objectCollisionNodeS5->getWorldPosition().z  - 10 < AI_Node1S5->getWorldPosition().z )
								{
									if(opponentHealth > 0)
									{
										opponentHit = true;
										opponentSetAnimation = false;
										increasePower();
										currentPower++;
										//PLAYS THE SOUND FOR A CONNECTED PUNCH AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, right_Punch_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, opponent_Punched_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}
				}

				if(robot_anim1S5->hasEnded())
				{
					playerRightPunch = false;
					setAnimation = false;
				}
			}
			else if(playerLeftKick)
			{
				if(setAnimation == false)
				{
					// Set animation
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( false );
					if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "rightKick" );}
					else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "rightKick" );}
					robot_anim1S5->setTimePosition(0);
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( true );
					setAnimation = true;
					//PLAYS THE SOUND FOR A MISS KICK
					if(combatSFXSetting)
					{
						audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
						ERRCHECK(audio_result);
					}
				}

				if(robot_anim1S5->getTimePosition() > (robot_anim1S5->getLength() / 2) && !opponentHit)
				{
					if( objectCollisionNodeS5->getWorldPosition().x + 10 > AI_Node1S5->getWorldPosition().x   )
					{
						if( objectCollisionNodeS5->getWorldPosition().x  - 10 < AI_Node1S5->getWorldPosition().x )
						{
							if( objectCollisionNodeS5->getWorldPosition().z  + 10 > AI_Node1S5->getWorldPosition().z )
							{
								if( objectCollisionNodeS5->getWorldPosition().z  - 10 < AI_Node1S5->getWorldPosition().z )
								{
									if(opponentHealth > 0)
									{
										opponentHit = true;
										opponentSetAnimation = false;
										increasePower();
										currentPower++;
										//PLAYS THE SOUND FOR A CONNECTED KICK AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, right_Kick_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, opponent_Kicked_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}
				}

				if(robot_anim1S5->hasEnded())
				{
					playerLeftKick = false;
					setAnimation = false;
				}
			}
			else if(playerRightKick)
			{
				if(setAnimation == false)
				{
					// Set animation
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( false );
					if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "leftKick" );}
					else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "leftKick" );}
					robot_anim1S5->setTimePosition(0);
					robot_anim1S5->setLoop( false );
					robot_anim1S5->setEnabled( true );
					setAnimation = true;
					//PLAYS THE SOUND FOR A MISS KICK
					if(combatSFXSetting)
					{
						audio_result = system->playSound(FMOD_CHANNEL_FREE, attack_Miss_Sound, false, &channel2);
						ERRCHECK(audio_result);
					}
				}

				if(robot_anim1S5->getTimePosition() > (robot_anim1S5->getLength() / 2)&& !opponentHit)
				{
					if( objectCollisionNodeS5->getWorldPosition().x + 10 > AI_Node1S5->getWorldPosition().x   )
					{
						if( objectCollisionNodeS5->getWorldPosition().x  - 10 < AI_Node1S5->getWorldPosition().x )
						{
							if( objectCollisionNodeS5->getWorldPosition().z  + 10 > AI_Node1S5->getWorldPosition().z )
							{
								if( objectCollisionNodeS5->getWorldPosition().z  - 10 < AI_Node1S5->getWorldPosition().z )
								{
									if(opponentHealth > 0)
									{
										opponentHit = true;
										opponentSetAnimation = false;
										increasePower();
										currentPower++;
										//PLAYS THE SOUND FOR A CONNECTED KICK AND GRUNT
										if(combatSFXSetting)
										{
											audio_result = system->playSound(FMOD_CHANNEL_FREE, left_Kick_Sound, false, &channel2);
											ERRCHECK(audio_result);
											audio_result = system->playSound(FMOD_CHANNEL_FREE, opponent_Kicked_Grunt_Sound, false, &channel3);
											ERRCHECK(audio_result);
										}
									}
								}
							}
						}
					}
				}

				if(robot_anim1S5->hasEnded())
				{
					playerRightKick = false;
					setAnimation = false;
				}
			}

			else if(keyForwardPress || keyBackwardPress || keyRightPress || keyLeftPress )
			{
				if(keyForwardPress == true)
				{
					//right collision//
					if(RaycastFromPointS5(rightCollisionS5, robot_entity1S5, PlayerNode->getPosition(), rightCollisionNodeS5->getOrientation() * Vector3::UNIT_Z, rightResult_1))
					{
						Vector3 rightDistance = PlayerNode->getPosition() - rightResult_1;
						if(rightDistance.length() > 20.0f )
						{
							if(RaycastFromPointS5(rightCollisionS5, robot_entity1S5, PlayerNode->getPosition(), rightCollisionNodeS5->getOrientation() * -Vector3::UNIT_Z,rightResult_2))
							{
								Vector3 rightDistance = PlayerNode->getPosition() + rightResult_2;
								if(rightDistance.length() > 20.0f)
								{
									mMoveScale = 3;	
									previous_position = mCamera->getPosition();
									mTranslateVector.z = mMoveScale;
								}
							}
						}
					}
					else
					{
						// if no collision is detected then move regardless//
						mMoveScale = 3;	
						mTranslateVector.z = mMoveScale;
					}
				}
				//backward//
				if(keyBackwardPress == true)
				{
					//left collision//
					if(RaycastFromPointS5(leftCollisionS5, robot_entity1S5, PlayerNode->getPosition(), leftCollisionNodeS5->getOrientation() * -Vector3::UNIT_Z, leftResult_1))
					{
						Vector3 leftDistance = PlayerNode->getPosition() - leftResult_1;
						if(leftDistance.length() > 20.0f )
						{
							if(RaycastFromPointS5(leftCollisionS5, robot_entity1S5, PlayerNode->getPosition(), leftCollisionNodeS5->getOrientation() * Vector3::UNIT_Z,leftResult_2))
							{
								Vector3 leftDistance = PlayerNode->getPosition() + leftResult_2;
								if(leftDistance.length() > 20.0f)
								{
									mMoveScale = 3;	
									previous_position = mCamera->getPosition();
									mTranslateVector.z = -mMoveScale;
								}
							}
						}
					}
					else
					{
						// if no collision is detected then move regardless//
						mMoveScale = 3;
						mTranslateVector.z = -mMoveScale;
					}
					
					
				}
				//left//
				if(keyLeftPress == true)
				{
					//forward collision//
					if(RaycastFromPointS5(forwardCollisionS5, robot_entity1S5, PlayerNode->getPosition(), forwardCollisionNodeS5->getOrientation() * Vector3::UNIT_X, forwardResult_1))
					{
						Vector3 forwardDistance = PlayerNode->getPosition() - forwardResult_1;
						if(forwardDistance.length() > 30.0f )
						{
							if(RaycastFromPointS5(forwardCollisionS5, robot_entity1S5, PlayerNode->getPosition(), forwardCollisionNodeS5->getOrientation() * -Vector3::UNIT_X,forwardResult_2))
							{
								Vector3 backwardDistance = PlayerNode->getPosition() + forwardResult_2;
								if(backwardDistance.length() > 30.0f)
								{
									mMoveScale = 3;	
									previous_position = mCamera->getPosition();
									mTranslateVector.x = mMoveScale;
								}
							}
						}
					}
					else
					{
						// if no collision is detected then move regardless//
						mMoveScale = 3;
						mTranslateVector.x = mMoveScale;
					}
					
				
				}
				//right//
				if(keyRightPress == true)
				{
					//backward Collision//
					if(RaycastFromPointS5(backwardCollisionS5, robot_entity1S5, PlayerNode->getPosition(), backwardCollisionNodeS5->getOrientation() * -Vector3::UNIT_X, backwardResult_1))
					{
						Vector3 backwardDistance_1 = PlayerNode->getPosition() - backwardResult_1;
						if(backwardDistance_1.length() > 30.0f )
						{
							if(RaycastFromPointS5(backwardCollisionS5, robot_entity1S5, PlayerNode->getPosition(), backwardCollisionNodeS5->getOrientation() * Vector3::UNIT_X,backwardResult_2))
							{
								Vector3 backwardDistance_2 = PlayerNode->getPosition() + backwardResult_2;
								if(backwardDistance_2.length() > 30.0f)
								{
									mMoveScale = 3;	
									previous_position = mCamera->getPosition();
									mTranslateVector.x = -mMoveScale;
								}
							}
						}
					}
					else
					{
						// if no collision is detected then move regardless//
						mMoveScale = 3;
						mTranslateVector.x = -mMoveScale;
					}
					
					
				}
				
				// Set animation
				robot_anim1S5->setLoop( false );
				robot_anim1S5->setEnabled( false );
				if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "frontCombatWalk" );}
				else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "frontCombatWalk" );}
				robot_anim1S5->setLoop( true );
				robot_anim1S5->setEnabled( true );
			}
			else
			{
				robot_anim1S5->setLoop( false );
				robot_anim1S5->setEnabled( false );
				if(shirt == 1){robot_anim1S5 = ShirtEntity->getAnimationState( "combatStance" );}
				else {robot_anim1S5 = ShirtlessEntity->getAnimationState( "combatStance" );}
				robot_anim1S5->setLoop( true );
				robot_anim1S5->setEnabled( true );
			}
		}
		
		PlayerNode->translate(PlayerNode->getOrientation() * mTranslateVector, Ogre::SceneNode::TS_WORLD);
		PlayerNode->yaw(mouseMoveX);
	}


	mTranslateVector = Vector3::ZERO;

}

bool SampleState5::checkPlayerOpponentCollision()
{
	if( objectCollisionNodeS5->getWorldPosition().x + 10 > AI_Node1S5->getWorldPosition().x   )
	{
		if( objectCollisionNodeS5->getWorldPosition().x  - 10 < AI_Node1S5->getWorldPosition().x )
		{
			if( objectCollisionNodeS5->getWorldPosition().z  + 10 > AI_Node1S5->getWorldPosition().z )
			{
				if( objectCollisionNodeS5->getWorldPosition().z  - 10 < AI_Node1S5->getWorldPosition().z )
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
void SampleState5::gamepadCharacterMovement( Real seconds )
{
	// Retrieve joystick values 
	mGamePadsS5[0]->update(seconds);

	Real xLeft  = mGamePadsS5[0]->getX(0);
	Real yLeft  = mGamePadsS5[0]->getY(0);
	Real xRight = mGamePadsS5[0]->getX(1);
	Real yRight = mGamePadsS5[0]->getY(1);

	//updates the orientation of the collision nodes so they face the right way//
	forwardCollisionNodeS5->setOrientation(PlayerNode->getOrientation());
	backwardCollisionNodeS5->setOrientation(PlayerNode->getOrientation());
	leftCollisionNodeS5->setOrientation(PlayerNode->getOrientation());
	rightCollisionNodeS5->setOrientation(PlayerNode->getOrientation());

	//collision vectors//
	Vector3 forwardResult_1;
	Vector3 forwardResult_2;
	Vector3 backwardResult_1;
	Vector3 backwardResult_2;
	Vector3 leftResult_1;
	Vector3 leftResult_2;
	Vector3 rightResult_1;
	Vector3 rightResult_2;
	
	// If joysticks are moved on any axis
	if(fabs(xLeft) > 0.0f || fabs(yLeft) > 0.0f)
	{
		//forward collision//
		if(RaycastFromPointS5(forwardCollisionS5, robot_entity1S5, PlayerNode->getPosition(), forwardCollisionNodeS5->getOrientation() * Vector3::UNIT_X, forwardResult_1))
		{
			Vector3 forwardDistance = PlayerNode->getPosition() - forwardResult_1;
			if(forwardDistance.length() > 30.0f )
			{
				if(RaycastFromPointS5(forwardCollisionS5, robot_entity1S5, PlayerNode->getPosition(), forwardCollisionNodeS5->getOrientation() * -Vector3::UNIT_X,forwardResult_2))
				{
					Vector3 backwardDistance = PlayerNode->getPosition() + forwardResult_2;
					if(backwardDistance.length() > 30.0f)
					{
						// If Left joystick moved on the y-axis
						if(yLeft > 0.5f)
						{
							mMoveScale = 3;	
							mTranslateVector.x = mMoveScale;
						}
						else if(yLeft > 0.0f)
						{
							mMoveScale = 1;	
							mTranslateVector.x = mMoveScale;
						}
					}
				}
			}
		}
		else
		{
			// if no collision is detected then move regardless//
			if(yLeft > 0.5f)
			{
				mMoveScale = 3;	
				mTranslateVector.x = mMoveScale;
			}
			else if(yLeft > 0.0f)
			{
				mMoveScale = 1;	
				mTranslateVector.x = mMoveScale;
			}
		}
	
	//backward collision//
	if(RaycastFromPointS5(backwardCollisionS5, robot_entity1S5, PlayerNode->getPosition(), backwardCollisionNodeS5->getOrientation() * -Vector3::UNIT_X, backwardResult_1))
	{
		Vector3 backwardDistance_1 = PlayerNode->getPosition() - backwardResult_1;
		if(backwardDistance_1.length() > 30.0f )
		{
			if(RaycastFromPointS5(backwardCollisionS5, robot_entity1S5, PlayerNode->getPosition(), backwardCollisionNodeS5->getOrientation() * Vector3::UNIT_X,backwardResult_2))
			{
				Vector3 backwardDistance_2 = PlayerNode->getPosition() + backwardResult_2;
				if(backwardDistance_2.length() > 30.0f)
				{
					if(yLeft < -0.5f)
					{
						mMoveScale = 3;	
						mTranslateVector.x = -mMoveScale;
					}
					else if(yLeft < 0.0f)
					{
						mMoveScale = 1;	
						mTranslateVector.x = -mMoveScale;
					}
				}
			}
		}
	}
	else
	{
		// if no collision is detected then move regardless//
		if(yLeft < -0.5f)
		{
			mMoveScale = 3;	
			mTranslateVector.x = -mMoveScale;
		}
		else if(yLeft < 0.0f)
		{
			mMoveScale = 1;	
			mTranslateVector.x = -mMoveScale;
		}
	}
	
	//right collision//
	if(RaycastFromPointS5(rightCollisionS5, robot_entity1S5, PlayerNode->getPosition(), rightCollisionNodeS5->getOrientation() * Vector3::UNIT_Z, rightResult_1))
	{
		Vector3 rightDistance = PlayerNode->getPosition() - rightResult_1;
		if(rightDistance.length() > 20.0f )
		{
			if(RaycastFromPointS5(rightCollisionS5, robot_entity1S5, PlayerNode->getPosition(), rightCollisionNodeS5->getOrientation() * -Vector3::UNIT_Z,rightResult_2))
			{
				Vector3 rightDistance = PlayerNode->getPosition() + rightResult_2;
				if(rightDistance.length() > 20.0f)
				{
					// If Left joystick moved on the x-axis
					if(xLeft > 0.5f)
					{	
						mMoveScale = 3;	
						mTranslateVector.z = mMoveScale;
					}
					else if(xLeft > 0.0f)
					{	
						mMoveScale = 1;	
						mTranslateVector.z = mMoveScale;
					}
				}
			}
		}
	}
	else
	{
		// If Left joystick moved on the x-axis
		if(xLeft > 0.5f)
		{	
			mMoveScale = 3;	
			mTranslateVector.z = mMoveScale;
		}
		else if(xLeft > 0.0f)
		{	
			mMoveScale = 1;	
			mTranslateVector.z = mMoveScale;
		}
	}

	//left collision//
	if(RaycastFromPointS5(leftCollisionS5, robot_entity1S5, PlayerNode->getPosition(), leftCollisionNodeS5->getOrientation() * -Vector3::UNIT_Z, leftResult_1))
	{
		Vector3 leftDistance = PlayerNode->getPosition() - leftResult_1;
		if(leftDistance.length() > 20.0f )
		{
			if(RaycastFromPointS5(leftCollisionS5, robot_entity1S5, PlayerNode->getPosition(), leftCollisionNodeS5->getOrientation() * Vector3::UNIT_Z,leftResult_2))
			{
				Vector3 leftDistance = PlayerNode->getPosition() + leftResult_2;
				if(leftDistance.length() > 20.0f)
				{	
					//if collision detected//
					if(xLeft < -0.5f)
					{
						mMoveScale = 3;	
						mTranslateVector.z = -mMoveScale;
					}
					else if(xLeft < 0.0f)
					{	
						mMoveScale = 1;	
						mTranslateVector.z = -mMoveScale;
					}
				}
			}
		}
	}
	else
	{
		//if no collision detected move regardless//
		if(xLeft < -0.5f)
		{
			mMoveScale = 3;	
			mTranslateVector.z = -mMoveScale;
		}
		else if(xLeft < 0.0f)
		{	
			mMoveScale = 1;	
			mTranslateVector.z = -mMoveScale;
		}
	}
		
		// Set animation
		robot_anim1S5->setLoop( false );
		robot_anim1S5->setEnabled( false );
		robot_anim1S5 = robot_entity1S5->getAnimationState( "frontCombatStance" );
		robot_anim1S5->setLoop( true );
		robot_anim1S5->setEnabled( true );
	}
	// If joysticks are not moved on any axis
	else if( xLeft == 0.0f && yLeft == 0.0f )
	{	
		robot_anim1S5->setLoop( false );
		robot_anim1S5->setEnabled( false );
		robot_anim1S5 = robot_entity1S5->getAnimationState( "combatStance" );
		robot_anim1S5->setLoop( true );
		robot_anim1S5->setEnabled( true );
	}
	
	// If Right joystick moved on the y-axis
	if(yRight > 0.5f && pitchS5 < 45)
	{	
		if(gamepadLookInvertedS5){ mCamera->pitch(Degree(-1)); pitchS5++;}
		else if(!gamepadLookInvertedS5){ mCamera->pitch(Degree(1)); pitchS5++;}
	}
	else if(yRight > 0.0f && pitchS5 < 45)
	{	
		if(gamepadLookInvertedS5){ mCamera->pitch(Degree(-0.5)); pitchS5+= 0.5;}
		else if(!gamepadLookInvertedS5){ mCamera->pitch(Degree(0.5)); pitchS5+= 0.5;}
	}
	else if(yRight < -0.5f && pitchS5 > -45)
	{
		if(gamepadLookInvertedS5){ mCamera->pitch(Degree(1)); pitchS5--;}
		else if(!gamepadLookInvertedS5){ mCamera->pitch(Degree(-1)); pitchS5--;}
	}
	else if(yRight < 0.0f && pitchS5 > -45)
	{
		if(gamepadLookInvertedS5){ mCamera->pitch(Degree(0.5)); pitchS5-= 0.5;}
		else if(!gamepadLookInvertedS5){ mCamera->pitch(Degree(-0.5)); pitchS5-= 0.5;}
	}
	
	// If Right joystick moved on the x-axis	
	if(xRight > 0.5f)
	{
		mCamera->yaw( Degree(-2));
		PlayerNode->yaw(Degree(-2));
	}
	if(xRight > 0.0f)
	{
		mCamera->yaw( Degree(-0.5));
		PlayerNode->yaw(Degree(-0.5));
	}
	else if(xRight < -0.5f)
	{
		mCamera->yaw( Degree(2));
		PlayerNode->yaw(Degree(2));
	}
	else if(xRight < 0.0f)
	{
		mCamera->yaw( Degree(0.5));
		PlayerNode->yaw(Degree(0.5));
	}
	
	// move PlayerNode
	PlayerNode->translate(PlayerNode->getOrientation() * mTranslateVector, Ogre::SceneNode::TS_WORLD);
	mTranslateVector = Vector3::ZERO;
}
bool SampleState5::keyPressed( const OIS::KeyEvent &e ) 
{
	

	if( e.key == OIS::KC_A )
	{
		keyLeftPress = true;
	}
	if(e.key == OIS::KC_D )
	{
		keyRightPress = true;
	}
	if(e.key == OIS::KC_W )
	{
		keyForwardPress = true;
	}
	if(e.key == OIS::KC_S )
	{
		keyBackwardPress = true;
	}
	if(e.key == OIS::KC_LSHIFT )
	{
		shiftPressed = true;
	}
	if(e.key == OIS::KC_LCONTROL )
	{
		controlPressed = true;
	}
	if(e.key == OIS::KC_SPACE  )
	{
		spacePressed = true;
	}
	

	return true;
}

bool SampleState5::keyReleased( const OIS::KeyEvent &e ) 
{
    if( e.key == OIS::KC_ESCAPE ) 
	{
     	GameManager::getSingletonPtr()->requestShutdown();
    }
    else if( e.key == OIS::KC_1 ) 
	{
		GameManager::getSingletonPtr()->changeState(WorkStateFactory::getSingletonPtr()->getState("EmptyWork"));
		GameManager::getSingletonPtr()->changeUIState(UIStateFactory::getSingletonPtr()->getState("MainMenuUI"));
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
		shiftPressed = false;
	}
	if(e.key == OIS::KC_LCONTROL )
	{
		controlPressed = false;
	}
	if(e.key == OIS::KC_SPACE  )
	{
		spacePressed = false;
	}

	return true;
}


bool SampleState5::buttonPressed( const OIS::JoyStickEvent &arg, int button )
{

	return true;
}

bool SampleState5::buttonReleased( const OIS::JoyStickEvent &arg, int button )
{
	return true;
}

bool SampleState5::forwardCollisionCheckS5( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode )
{
	//collision vectors//
	Vector3 forwardResult_1;
	Vector3 forwardResult_2;
	
	//forward collision//
	if(RaycastFromPointS5(forwardCollisionS5, entity, entityNode->getPosition(), collisionNode->getOrientation() * Vector3::UNIT_X, forwardResult_1))
	{
		Vector3 forwardDistance = entityNode->getPosition() - forwardResult_1;
		if(forwardDistance.length() > 30.0f )
		{
			if(RaycastFromPointS5(forwardCollisionS5, entity, entityNode->getPosition(), collisionNode->getOrientation() * -Vector3::UNIT_X,forwardResult_2))
			{
				Vector3 backwardDistance = entityNode->getPosition() + forwardResult_2;
				if(backwardDistance.length() > 30.0f)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
bool SampleState5::backwardCollisionCheckS5( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode )
{
	//collision vectors//
	Vector3 backwardResult_1;
	Vector3 backwardResult_2;

	//backward Collision//
	if(RaycastFromPointS5(backwardCollisionS5, entity, entityNode->getPosition(), collisionNode->getOrientation() * -Vector3::UNIT_X, backwardResult_1))
	{
		Vector3 backwardDistance_1 = entityNode->getPosition() - backwardResult_1;
		if(backwardDistance_1.length() > 30.0f )
		{
			if(RaycastFromPointS5(backwardCollisionS5, entity, entityNode->getPosition(), collisionNode->getOrientation() * Vector3::UNIT_X,backwardResult_2))
			{
				Vector3 backwardDistance_2 = entityNode->getPosition() + backwardResult_2;
				if(backwardDistance_2.length() > 30.0f)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
bool SampleState5::leftCollisionCheckS5( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode )
{
	//collision vectors//
	Vector3 leftResult_1;
	Vector3 leftResult_2;

	//left collision//
	if(RaycastFromPointS5(leftCollisionS5, entity, entityNode->getPosition(), collisionNode->getOrientation() * -Vector3::UNIT_Z, leftResult_1))
	{
		Vector3 leftDistance = entityNode->getPosition() - leftResult_1;
		if(leftDistance.length() > 20.0f )
		{
			if(RaycastFromPointS5(leftCollisionS5, entity, entityNode->getPosition(), collisionNode->getOrientation() * Vector3::UNIT_Z,leftResult_2))
			{
				Vector3 leftDistance = entityNode->getPosition() + leftResult_2;
				if(leftDistance.length() > 20.0f)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
bool SampleState5::rightCollisionCheckS5( Ogre::Entity *entity, Ogre::SceneNode* entityNode, Ogre::SceneNode* collisionNode )
{
	//collison vectors//
	Vector3 rightResult_1;
	Vector3 rightResult_2;

	//right collision//
	if(RaycastFromPointS5(rightCollisionS5, entity, entityNode->getPosition(), collisionNode->getOrientation() * Vector3::UNIT_Z, rightResult_1))
	{
		Vector3 rightDistance = entityNode->getPosition() - rightResult_1;
		if(rightDistance.length() > 20.0f )
		{
			if(RaycastFromPointS5(rightCollisionS5, entity, entityNode->getPosition(), collisionNode->getOrientation() * -Vector3::UNIT_Z,rightResult_2))
			{
				Vector3 rightDistance = entityNode->getPosition() + rightResult_2;
				if(rightDistance.length() > 20.0f)
				{
					return true;
				}
			}
		}
	}
		
	return false;
}

////////////////////////////////
//FUNTIONS TO LOAD ALL OPTIONS//
////////////////////////////////
void SampleState5::loadOptions( void ) 
{
	TiXmlDocument loadDoc("Options.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Settings");

	combatMusicSetting = atoi(element->Attribute("Music"));
	combatSFXSetting = atoi(element->Attribute("SFX"));
	combatControls = atoi(element->Attribute("Keyboard"));
	combatInvert = atoi(element->Attribute("Invert"));
}

void SampleState5::loadCharacter( void )
{
	TiXmlDocument loadDoc("Character_Traits.xml");
	loadDoc.LoadFile();

	TiXmlElement *element = loadDoc.RootElement()->FirstChildElement("Info");

	fromGame = atoi(element->Attribute("FromGame"));

	if(fromGame)
	{
		skin = atoi(element->Attribute("Skin"));
		shirt = atoi(element->Attribute("Shirt"));
		hair = atoi(element->Attribute("Hair"));
	}
	else
	{
		shirt = 0;
		skin = 1;
		hair = 0;
	}

	if(shirt == 0)
	{
		PlayerNode->detachAllObjects();
		PlayerNode->attachObject(ShirtlessEntity);
		if(hair == 0 && skin == 0)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_Tank_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_Tank_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_Tank_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_Tank_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_Tank_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_Tank_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_Tank_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_Tank_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_Tank_Bald");
		}
	}
	else if(shirt == 1)
	{
		PlayerNode->detachAllObjects();
		PlayerNode->attachObject(ShirtEntity);
		if(hair == 0 && skin == 0)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_Black_Collar_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_White_Collar_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_Asian_Collar_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_Black_Collar_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_White_Collar_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_Asian_Collar_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_Black_Collar_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_White_Collar_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			ShirtEntity->setMaterialName("VGD_ALONE_Main_Asian_Collar_Bald");
		}
	}
	else if(shirt == 2)
	{
		PlayerNode->detachAllObjects();
		PlayerNode->attachObject(ShirtlessEntity);
		if(hair == 0 && skin == 0)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_None_Crew");
		}
		else if(hair == 0 && skin == 1)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_None_Crew");
		}
		else if(hair == 0 && skin == 2)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_None_Crew");
		}
		else if(hair == 1 && skin == 0)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_None_Buzz");
		}
		else if(hair == 1 && skin == 1)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_None_Buzz");
		}
		else if(hair == 1 && skin == 2)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_None_Buzz");
		}
		else if(hair == 2 && skin == 0)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Black_None_Bald");
		}
		else if(hair == 2 && skin == 1)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_White_None_Bald");
		}
		else if(hair == 2 && skin == 2)
		{
			ShirtlessEntity->setMaterialName("VGD_ALONE_Main_Asian_None_Bald");
		}
	}
}

void SampleState5::increasePower()
{
	if(currentPower == 0)
	{
		combatPowerBar1Overlay->show();
	}
	else if(currentPower == 1)
	{
		combatPowerBar2Overlay->show();
	}
	else if(currentPower == 2)
	{
		combatPowerBar3Overlay->show();
	}
	else if(currentPower == 3)
	{
		combatPowerBar4Overlay->show();
	}
	else if(currentPower == 4)
	{
		combatPowerBar5Overlay->show();
	}
}

SampleState5* SampleState5::getSingletonPtr( void ) 
{
    if( !mSampleState5 ) 
	{
        mSampleState5 = new SampleState5();
		WorkStateFactory::getSingletonPtr()->registerState(mSampleState5->getName(), mSampleState5);
    }

    return mSampleState5;
}
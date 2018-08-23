#include "SceneRange.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "SoundEngine.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadHmap.h"
#include <sstream>
#define SP3_DEBUG

SceneRange::SceneRange()
{
}

SceneRange::~SceneRange()
{
	if (theMouse)
	{
		delete theMouse;
		theMouse = NULL;
	}
	if (theKeyboard)
	{
		delete theKeyboard;
		theKeyboard = NULL;
	}
}

void SceneRange::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	/*m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );*/
	/*m_programID = LoadShaders("Shader//comg.vertexshader", "Shader//MultiTexture.fragmentshader");*/
	/*m_programID = LoadShaders("Shader//Fog.vertexshader", "Shader//Fog.fragmentshader");*/
	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	// Get a handle for fogeriunos
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");
	// shadow shtuff
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] =
		glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] =
		glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] =
		glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE1] =
		glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] =
		glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE2] =
		glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");
	
	//paint uniform value parameters
	m_parameters[U_PAINT_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "paintTextureEnabled");
	m_parameters[U_PAINT_TEXTURE] = glGetUniformLocation(m_programID, "paintTexture");
	m_parameters[U_PAINT_TEXCOORDSTRETCH] = glGetUniformLocation(m_programID, "paintTexco");
	m_parameters[U_PAINT_TGASTRETCH_X] = glGetUniformLocation(m_programID, "paintTgaStrX");
	m_parameters[U_PAINT_TGASTRETCH_Y] = glGetUniformLocation(m_programID, "paintTgaStrY");
	//transparency/alpha uniform value parameters
	m_parameters[U_COLOR_ALPHA] = glGetUniformLocation(m_programID, "colorAlpha");
	//uv offset uniform value paprameters
	m_parameters[U_UV_OFFSET_ENABLED] = glGetUniformLocation(m_programID, "uvOffsetEnabled");
	m_parameters[U_UV_OFFSET] = glGetUniformLocation(m_programID, "uvOffset");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(0, 350.f + 50.f, 100);
	lights[0].color.Set(0, 69, 0);/*
	lights[0].position.Set(0, 350.f + 300.f, 0);
	lights[0].color.Set(1, 1, 1);*/
	lights[0].power = 0.5f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_SPOT;
	lights[1].position.Set(0, 350.f + 50.f, 100);
	lights[1].color.Set(155, 29, 29);
	lights[1].power = 2;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(60));
	lights[1].cosInner = cos(Math::DegreeToRadian(50));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	Color fogColor = (0.5f, 1.f, 1.f);
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_START], 10);
	glUniform1f(m_parameters[U_FOG_END], 1000);
	glUniform1f(m_parameters[U_FOG_DENSITY], 0.005f);
	glUniform1f(m_parameters[U_FOG_TYPE], 2);
	glUniform1f(m_parameters[U_FOG_ENABLED], 0);

	glUniform1f(m_parameters[U_COLOR_ALPHA], 1);

	glUniform1i(m_parameters[U_UV_OFFSET_ENABLED], 0);
	glUniform2f(m_parameters[U_UV_OFFSET], 0, 0);


	m_lightDepthFBO.Init(1024, 1024);

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 10.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureArray[0] = LoadTGA("Image//calibri.tga");
	/*meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);*/
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 1.f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0, 0, 0.5), 10.f);

	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 128, 1000.0f, 2250.0f, 1.0f, 1.0f);
	meshList[GEO_SKYPLANE]->textureArray[0] = LoadTGA("Image//skyplaneRange.tga");

	meshList[GEO_LEFTARM] = MeshBuilder::GenerateOBJ("GEO_LEFTARM", "OBJ//leftArm.obj");
	meshList[GEO_RIGHTARM] = MeshBuilder::GenerateOBJ("GEO_RIGHTARM", "OBJ//rightArm.obj");
	meshList[GEO_RIGHTARM]->textureArray[0] = LoadTGA("Image//rightArm.tga");
	meshList[GEO_RIGHTARM_AURA_FIRE] = MeshBuilder::GenerateOBJ("GEO_RIGHTARM_AURA_FIRE", "OBJ//rightArm.obj");
	meshList[GEO_RIGHTARM_AURA_FIRE]->textureArray[0] = LoadTGA("Image//fireball_texture.tga");
	meshList[GEO_RIGHTARM_AURA_ICE] = MeshBuilder::GenerateOBJ("GEO_RIGHTARM_AURA_ICE", "OBJ//rightArm.obj");
	meshList[GEO_RIGHTARM_AURA_ICE]->textureArray[0] = LoadTGA("Image//iceball_texture.tga");

	meshList[GEO_DRONE_HEAD] = MeshBuilder::GenerateOBJ("GEO_DRONE_HEAD", "OBJ//droneHead.obj");
	meshList[GEO_DRONE_LWING] = MeshBuilder::GenerateOBJ("GEO_DRONE_LWING", "OBJ//droneLeftwing.obj");
	meshList[GEO_DRONE_RWING] = MeshBuilder::GenerateOBJ("GEO_DRONE_RWING", "OBJ//droneRightwing.obj");

	meshList[GEO_BOLT] = MeshBuilder::GenerateOBJ("GEO_BOLT", "OBJ//bolt.obj");
	meshList[GEO_BOLT]->textureArray[0] = LoadTGA("Image//bolt.tga");
	meshList[GEO_BARREL] = MeshBuilder::GenerateOBJ("dummy", "OBJ//barrel.obj");
	meshList[GEO_BARREL]->textureArray[0] = LoadTGA("Image//barrel.tga");
	meshList[GEO_DRAGON] = MeshBuilder::GenerateOBJ("GEO_DRAGON", "OBJ//dragon.obj");
	meshList[GEO_DRAGON]->textureArray[0] = LoadTGA("Image//Tex_Dragon.tga");

	// For Ter Rain
	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//heightmapRange.raw", m_heightMap);
	meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//floor.tga");
	meshList[GEO_TERRAIN]->tgaLengthPaint = 256;
	meshList[GEO_TERRAIN]->texturePaintID = NewTGA(meshList[GEO_TERRAIN]->tgaLengthPaint);
	testvar = 0;

	//meshList[GEO_TESTPAINTQUAD] = MeshBuilder::GenerateQuad("GEO_TESTPAINTQUAD", Color(1, 1, 1), 1.f);
	//meshList[GEO_TESTPAINTQUAD]->textureArray[0] = LoadTGA("Image//moss1.tga");
	//meshList[GEO_TESTPAINTQUAD]->tgaLengthPaint = 1;
	//meshList[GEO_TESTPAINTQUAD]->texturePaintID = NewTGA(meshList[GEO_TESTPAINTQUAD]->tgaLengthPaint);

	//meshList[GEO_TESTPAINTQUAD2] = MeshBuilder::GenerateQuad("GEO_TESTPAINTQUAD2", Color(1, 1, 1), 1.f);
	//meshList[GEO_TESTPAINTQUAD2]->textureArray[0] = LoadTGA("Image//moss1.tga");
	//meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint = 64;
	//meshList[GEO_TESTPAINTQUAD2]->texturePaintID = NewTGA(meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint);


	meshList[GEO_PARTICLE_FIRE] = MeshBuilder::GenerateSphere("fireparticle", Color(1, 157.f / 255.f, 0), 6, 6, 1.f);
	meshList[GEO_PARTICLE_ICE] = MeshBuilder::GenerateSphere("iceparticle", Color(168.f/255.f, 241.f / 255.f, 1), 6, 6, 1.f);

	meshList[GEO_FIREBALL] = MeshBuilder::GenerateOBJ("fireball", "OBJ//ball.obj");
	meshList[GEO_FIREBALL]->textureArray[0] = LoadTGA("Image//fireball_texture.tga");
	meshList[GEO_ICEBALL] = MeshBuilder::GenerateOBJ("iceball", "OBJ//ball.obj");
	meshList[GEO_ICEBALL]->textureArray[0] = LoadTGA("Image//iceball_texture.tga");
	meshList[GEO_KILLERNADO] = MeshBuilder::GenerateOBJ("tornado", "OBJ//tornado.obj");
	meshList[GEO_KILLERNADO]->textureArray[0] = LoadTGA("Image//tornado.tga");
	meshList[GEO_ICEBLOCK] = MeshBuilder::GenerateOBJ("iceblock", "OBJ//iceblock.obj");
	meshList[GEO_ICEBLOCK]->textureArray[0] = LoadTGA("Image//icecube.tga");

	// Load the ground mesh and texture
	meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_DARKGREEN]->textureArray[0] = LoadTGA("Image//grass_darkgreen.tga");
	meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_LIGHTGREEN]->textureArray[0] = LoadTGA("Image//grass_lightgreen.tga");
	meshList[GEO_SPRITE_ANIMATION] =
		MeshBuilder::GenerateSpriteAnimation("fire", 4, 4);
	//meshList[GEO_SPRITE_ANIMATION]->textureArray[0] = LoadTGA("Image//fire.tga");
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 15, 0, 1.f, true);
	}
	meshList[GEO_SPRITEANIM_ACTIONLINES] = MeshBuilder::GenerateSpriteAnimation("actionlines", 2, 2);
	meshList[GEO_SPRITEANIM_ACTIONLINES]->textureArray[0] = LoadTGA("Image//action_lines.tga");
	SpriteAnimation* sa_AL = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITEANIM_ACTIONLINES]);
	if (sa_AL)
	{
		sa_AL->m_anim = new Animation();
		sa_AL->m_anim->Set(0, (2 * 2) - 1, 1, 0.25f, true);
	}

	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();
	camera.Init(playerInfo->getPos(), playerInfo->getTarget(), playerInfo->GetUp(), m_heightMap);
	playerInfo->AttachCamera(&camera);
	playerInfo->FirstHeight = 350.f*ReadHeightMap(m_heightMap, playerInfo->getPos().x / 4000.f, playerInfo->getPos().z / 4000.f);
	playerInfo->terrainHeight = 350.f * ReadHeightMap(m_heightMap, playerInfo->getPos().x / 4000, playerInfo->getPos().z / 4000);

	CNPC* npc = new CNPC(
		Vector3(0, 0, 80),
		Vector3(4, 12, 4),
		Vector3(0, 0, 80.f)
	);
	npc->setPlayerRef(playerInfo);

	for (int i = 0; i < 3; i++)
	{
		targets[i] = new CEntity();
		targets[i]->Init();
		targets[i]->setType(CEntity::E_TARGET);
		targets[i]->setPos(Vector3(-500 + i * 500, 100.f, 1500.f));
		targets[i]->setOriginPos(targets[i]->getPos());
		targets[i]->setScale(Vector3(40.f, 40.f, 40.f));
		targets[i]->setTarget(Vector3(0.f, 0.f, 0.f));
	}
	for (int i = 0; i < 3; i++) 
	{
		targetsMoving[i] = new CEntity();
		targetsMoving[i]->Init();
		targetsMoving[i]->setType(CEntity::E_MOVING_TARGET);
		targetsMoving[i]->setPos(Vector3(-500 + i * 500, 100.f, 1500.f));
		targetsMoving[i]->setOriginPos(targetsMoving[i]->getPos());
		targetsMoving[i]->setScale(Vector3(40.f, 40.f, 40.f));
		targetsMoving[i]->setTarget(Vector3(0 + i * 500, 100.f, 1500.f));
	}

	for (int i = 0; i < 3; i++)
	{
		targets1[i] = new CEntity();
		targets1[i]->Init();
		targets1[i]->setType(CEntity::E_TARGET_FIRE);
		targets1[i]->setPos(Vector3(-500 + i * 500, 100.f, -1500.f));
		targets1[i]->setOriginPos(targets1[i]->getPos());
		targets1[i]->setScale(Vector3(40.f, 40.f, 40.f));
		targets1[i]->setTarget(Vector3(0.f, 0.f, 0.f));
	}
	for (int i = 0; i < 3; i++) 
	{
		targetsMoving1[i] = new CEntity();
		targetsMoving1[i]->Init();
		targetsMoving1[i]->setType(CEntity::E_TARGET_ICE);
		targetsMoving1[i]->setPos(Vector3(-500 + i * 500, 100.f, -1500.f));
		targetsMoving1[i]->setOriginPos(targetsMoving1[i]->getPos());
		targetsMoving1[i]->setScale(Vector3(40.f, 40.f, 40.f));
		targetsMoving1[i]->setTarget(Vector3(0 + i * 500, 100.f, -1500.f));
	}

	// Hardware Abstraction
	theKeyboard = new CKeyboard();
	theKeyboard->Create(playerInfo);
	theMouse = new CMouse();
	theMouse->Create(playerInfo);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;
	m_particleCount = 0;
	MAX_PARTICLE = 1000;
	m_gravity.Set(0, -9.8f, 0);
	bLightEnabled = true;
	lights[0].type = Light::LIGHT_POINT;
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	///init sound
	SEngine = CSoundEngine::GetInstance();
	//CSoundEngine::GetInstance()->Init();
	SEngine->AddSound("Fireball", "Sound//fireball.mp3");
	SEngine->AddSound("Iceattack", "Sound//iceattack.mp3");
}

void SceneRange::Update(double dt)
{
	//cout << playerInfo->getPos() << endl;
	/*if (playerInfo->getPos().z > 740) { playerInfo->setPos(Vector3(playerInfo->getPos().x, playerInfo->getPos().y, 740)); }
	if (playerInfo->getPos().z < -800) { playerInfo->setPos(Vector3(playerInfo->getPos().x, playerInfo->getPos().y, -800)); }*/
	if (Application::IsKeyPressed(VK_ESCAPE))
	{
		CSceneManager::Instance()->GoToScene(CSceneManager::SCENE_IN_GAME_MENU);
	}

	TimeTrackerManager::GetInstance()->Update(dt);
	dt *= TimeTrackerManager::GetInstance()->getSpeed();

	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}
	if (playerInfo->GetSpellType() != CPlayerInfo::SPELL_NONE)
	{
		CProjectile* aa;
		if (playerInfo->GetSpellType() == CPlayerInfo::SPELL_FIREBALL)
		{
			if (playerInfo->GetSpellMod() == CProjectile::SMTYPE_NORMAL)
			{
				aa = new CProjectile(CProjectile::PTYPE_FIRE);
				Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 viewvec = (camtar - campos).Normalized();
				aa->Init(campos + viewvec, camtar + viewvec*1.5f);
			}
			else if (playerInfo->GetSpellMod() == CProjectile::SMTYPE_BURST)
			{
				aa = new CProjectile(CProjectile::PTYPE_FIRE, CProjectile::SMTYPE_BURST);
				CProjectile* aa2 = new CProjectile(CProjectile::PTYPE_FIRE, CProjectile::SMTYPE_BURST);
				CProjectile* aa3 = new CProjectile(CProjectile::PTYPE_FIRE, CProjectile::SMTYPE_BURST);
				Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 viewvec = (camtar - campos).Normalized();
				aa->Init(campos + viewvec, camtar + viewvec*1.5f);
				aa2->Init(campos + viewvec, camtar + viewvec*1.5f);
				aa2->SetBurstPivRotOff(120);
				aa3->Init(campos + viewvec, camtar + viewvec*1.5f);
				aa3->SetBurstPivRotOff(240);
			}
			else if (playerInfo->GetSpellMod() == CProjectile::SMTYPE_SPECIAL)
			{
				aa = new CProjectile(CProjectile::PTYPE_FIRE, CProjectile::SMTYPE_SPECIAL);
				Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 viewvec = (camtar - campos).Normalized();
				aa->Init(campos + viewvec, camtar + viewvec*1.5f);
			}

			CSoundEngine::GetInstance()->PlayASound("Fireball");
			playerInfo->setMana(playerInfo->getMana() - playerInfo->getManaCost());
		}
		else if (playerInfo->GetSpellType() == CPlayerInfo::SPELL_ICEBALL)
		{
			if (playerInfo->GetSpellMod() == CProjectile::SMTYPE_NORMAL)
			{
				aa = new CProjectile(CProjectile::PTYPE_ICE);
				Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 viewvec = (camtar - campos).Normalized();
				aa->Init(campos + viewvec, camtar + viewvec*1.5f);
			}
			else if (playerInfo->GetSpellMod() == CProjectile::SMTYPE_BURST)
			{
				aa = new CProjectile(CProjectile::PTYPE_ICE, CProjectile::SMTYPE_BURST);
				CProjectile* aa2 = new CProjectile(CProjectile::PTYPE_ICE, CProjectile::SMTYPE_BURST);
				CProjectile* aa3 = new CProjectile(CProjectile::PTYPE_ICE, CProjectile::SMTYPE_BURST);
				Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 viewvec = (camtar - campos).Normalized();
				aa->Init(campos + viewvec, camtar + viewvec*1.5f);
				Mtx44 rotation;
				Vector3 tempUp(0, 1, 0);
				if (playerInfo->rocketMode) tempUp = playerInfo->rocketUp;
				rotation.SetToRotation(30, tempUp.x, tempUp.y, tempUp.z);
				aa2->Init(campos + (rotation * viewvec), camtar + (rotation * viewvec)*1.5f);
				rotation.SetToRotation(-30, tempUp.x, tempUp.y, tempUp.z);
				aa3->Init(campos + (rotation * viewvec), camtar + (rotation * viewvec)*1.5f);
			}
			else if (playerInfo->GetSpellMod() == CProjectile::SMTYPE_SPECIAL)
			{
				aa = new CProjectile(CProjectile::PTYPE_ICE, CProjectile::SMTYPE_SPECIAL);
				Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
				Vector3 viewvec = (camtar - campos).Normalized();
				aa->Init(campos + viewvec, camtar + viewvec*1.5f);
			}

			CSoundEngine::GetInstance()->PlayASound("Iceattack");
			playerInfo->setMana(playerInfo->getMana() - playerInfo->getManaCost());
		}
		CameraEffectManager::GetInstance()->AddCamEffect(CameraEffect::CE_TYPE_ACTIONLINE_WHITE);
		playerInfo->SetSpellType(CPlayerInfo::SPELL_NONE);
		
	}
#ifdef SP3_DEBUG
	if (KeyboardController::GetInstance()->IsKeyPressed('H'))
	{
		cout << "key H was pressed" << endl;
		CProjectile* aa = new CProjectile(CProjectile::PTYPE_FIRE);
		Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
		Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
		Vector3 viewvec = (camtar - campos).Normalized();
		aa->Init(campos + viewvec, camtar + viewvec*1.5f);
		CameraEffectManager::GetInstance()->AddCamEffect(CameraEffect::CE_TYPE_ACTIONLINE_WHITE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('J'))
	{
		cout << "key J was pressed" << endl;
		CProjectile* aa = new CProjectile(CProjectile::PTYPE_ICE);
		Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
		Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
		Vector3 viewvec = (camtar - campos).Normalized();
		aa->Init(campos + viewvec, camtar + viewvec*1.5f);
		CameraEffectManager::GetInstance()->AddCamEffect(CameraEffect::CE_TYPE_ACTIONLINE_WHITE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('K'))
	{
		cout << "key K was pressed" << endl;
		CProjectile* aa = new CProjectile(CProjectile::PTYPE_BEAM);
		Vector3 campos = camera.position - Vector3(0, playerInfo->FirstHeight, 0);
		Vector3 camtar = camera.target - Vector3(0, playerInfo->FirstHeight, 0);
		Vector3 viewvec = (camtar - campos).Normalized();
		aa->Init(campos + viewvec * 5, camtar + viewvec* 6);

		//raycast check
		Vector3 tempProj(9999, 9999, 9999);
		std::list<CEntity*>::iterator it, it2, end;
		end = EntityManager::GetInstance()->entityList.end();
		for (it = EntityManager::GetInstance()->entityList.begin(); it != end; ++it)
		{
			if ((*it)->getType() == CEntity::E_ENEMY || (*it)->getType() == CEntity::E_WALL || (*it)->getType() == CEntity::E_TARGET || (*it)->getType() == CEntity::E_MOVING_TARGET)
			{
				Vector3 tempView = (aa->getTarget() - aa->getPos()).Normalized() * 1500;
				Vector3 tempTempProj = EntityManager::GetInstance()->CheckForLineIntersection(aa->getPos(), (*it), tempView, false);
				if (!(tempTempProj - Vector3(9999, 9999, 9999)).IsZero() && tempTempProj.Length() < tempProj.Length())
				{
					tempProj = tempTempProj;
					//(*it)->setTarget((*it)->getPos() + tempProj);
					//aa->setScale(aa->getScale() + Vector3(0, 0, tempProj.Length()));
				}
				tempTempProj = EntityManager::GetInstance()->CheckForLineIntersection(aa->getPos(), (*it), tempView, true);
				if (!(tempTempProj - Vector3(9999, 9999, 9999)).IsZero() && tempTempProj.Length() < tempProj.Length())
				{
					tempProj = tempTempProj;
					//(*it)->setTarget((*it)->getPos() + tempProj);
					//aa->setScale(aa->getScale() + Vector3(0, -0.5, tempProj.Length()));
				}
			}
		}

		//add for wall also
		if ((aa->getTarget() - aa->getPos()).Normalized().y < 0)
		{

			Vector3 tempTempProj = (aa->getTarget() - aa->getPos()).Normalized() * (aa->getPos().y / (aa->getPos() - aa->getTarget()).Normalized().y);
			if (tempTempProj.Length() >= tempProj.Length())
			{

			}
			else if (Math::FAbs((aa->getPos() + tempTempProj).x) >= 2000 || Math::FAbs((aa->getPos() + tempTempProj).z) >= 2000)
			{

			}
			else
			{
				tempProj = tempTempProj;
				meshList[GEO_TERRAIN]->texturePaintID = PaintTGA(meshList[GEO_TERRAIN]->texturePaintID, (((aa->getPos() + tempProj).x / 4000.f) + 0.5f) * (1 / (PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000.f)), (((aa->getPos() + tempProj).z / 4000.f) + 0.5f) * (1 / (PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000.f)), Vector3(0, 0, 0), 1, meshList[GEO_TERRAIN]->tgaLengthPaint, PAINT_PATTERNS::PAINT_BURST);//PaintTGA(meshList[GEO_TESTPAINTQUAD2]->texturePaintID, (entPos.x / 4000.f) * (1 / (PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 90)), (entPos.z / 4000.f) * (1 / (PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 160)), Vector3(0.5, 1, 0), 1, meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint);
			}
		}

		if ((tempProj - Vector3(9999, 9999, 9999)).IsZero()) aa->setIsDone(true);
		else
		{
			aa->setTarget(aa->getPos() + tempProj);
			aa->setScale(aa->getScale() + Vector3(24, 24, tempProj.Length()));
			CameraEffectManager::GetInstance()->AddCamEffect(CameraEffect::CE_TYPE_ACTIONLINE_WHITE);

			meshList[GEO_TERRAIN]->texturePaintID = PaintTGA(meshList[GEO_TERRAIN]->texturePaintID, ((camera.position.x / 4000.f) + 0.5f) * (1 / (PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000.f)), ((camera.position.z / 4000.f) + 0.5f) * (1 / (PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000.f)), Vector3(1, 0, 1), 1, meshList[GEO_TERRAIN]->tgaLengthPaint, PAINT_PATTERNS::PAINT_MAGICCIRCLE);//PaintTGA(meshList[GEO_TESTPAINTQUAD2]->texturePaintID, (entPos.x / 4000.f) * (1 / (PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 90)), (entPos.z / 4000.f) * (1 / (PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 160)), Vector3(0.5, 1, 0), 1, meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint);
		}

		////float tempScaleZ = aa->getPos().y / (aa->getPos() - aa->getTarget()).Normalized().y;
		////if (tempScaleZ <= 0)
		////{
		////	aa->setScale(aa->getScale() + Vector3(0, 0, 400));
		////}
		////else aa->setScale(aa->getScale() + Vector3(0, 0, tempScaleZ ));



		//aa->setTarget(aa->getTarget() + tempScaleZ / 2 * (aa->getTarget() - aa->getPos()).Normalized());
		//for (int i = 0; i < 10; ++i)
		//{
		//	ParticleManager::GetInstance()->AddParticle(aa);
		//}
		//aa->setTarget(aa->getTarget() - tempScaleZ / 2 * (aa->getTarget() - aa->getPos()).Normalized());

	}
	if (JoystickController::GetInstance()->IsButtonPressed(JoystickController::BUTTON_1))	
		cout << "joystick X button was pressed" << endl;
	if (Application::IsKeyPressed('B'))
	{
		CSceneManager::Instance()->GoToScene(CSceneManager::SCENE_BOSS);
	}
#endif // SP3_DEBUG

	if (KeyboardController::GetInstance()->IsKeyPressed('U'))
	{
		if (playerInfo->rocketMode)
		{
			playerInfo->SetUp(Vector3(0, 1, 0));
			playerInfo->setTarget(playerInfo->getPos() + (playerInfo->rocketTarget - playerInfo->rocketPosition).Normalized());
			playerInfo->SetOnFreeFall(true);
			playerInfo->rocketMode = false;
		}
		else if (!playerInfo->rocketMode)
		{
			playerInfo->rocketMode = true;
			playerInfo->rocketPosition = playerInfo->getPos() + Vector3(0, 10, 0);
			playerInfo->rocketTarget = playerInfo->rocketPosition + (playerInfo->getTarget() - playerInfo->getPos()).Normalized();
			playerInfo->rocketUp = Vector3(0, 1, 0);
			playerInfo->rocketPitchAccel = 0;
			playerInfo->rocketRollAccel = 0;
			playerInfo->rocketYawAccel = 0;
		}
	}

	if (playerInfo->rocketMode)
	{
		playerInfo->setPos(playerInfo->rocketPosition);
	}
	if (playerInfo->rocketMode && playerInfo->rocketPosition.y + 350 <= 350 * ReadHeightMap(m_heightMap, playerInfo->rocketPosition.x / 4000, playerInfo->rocketPosition.z / 4000))
	{
		playerInfo->SetUp(Vector3(0, 1, 0));
		playerInfo->setPos(playerInfo->getPos() + Vector3(0, - playerInfo->getPos().y, 0));
		playerInfo->setTarget(playerInfo->getPos() + (playerInfo->rocketTarget - playerInfo->rocketPosition).Normalized());
		playerInfo->terrainHeight = 350 * ReadHeightMap(m_heightMap, playerInfo->rocketPosition.x / 4000, playerInfo->rocketPosition.z / 4000);
		playerInfo->rocketMode = false;
	}

	if(Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if(Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if(Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if(Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if(Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if(Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	//rotateAngle += (float)(10 * dt);
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	if (sa)
	{
		sa->Update(dt);
		sa->m_anim->animActive = true;
	}
	SpriteAnimation* sa_AL = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITEANIM_ACTIONLINES]);
	if (sa_AL)
	{
		sa_AL->Update(dt);
	}

	// Hardware Abstraction
	theKeyboard->Read(dt);
	theMouse->Read(dt);
	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->terrainHeight = 350.f * ReadHeightMap(m_heightMap, playerInfo->getPos().x / 4000, playerInfo->getPos().z / 4000);
	//playerInfo->Update(dt);

	EntityManager::GetInstance()->Update(dt);
	ParticleManager::GetInstance()->Update(dt);
	CameraEffectManager::GetInstance()->Update(dt);
	//camera.Update(dt);

	playerInfo->SetNotMoving();

	bool shouldChange = true;
	//check if shouldnt change (any targets still up)
	switch (targetState)
	{
	case T_MOVING:
		for (int i = 0; i < 3; ++i)
		{
			if(!targetsMoving[i]->isDone()){ shouldChange = false; }
		}
		break;
	case T_STATIONARY:
		for (int i = 0; i < 3; ++i)
		{
			if (!targets[i]->isDone()) { shouldChange = false; }
		}
		break;
	default:
		break;
	}
	if (shouldChange && stateChangeTimer == 0) { stateChangeTimer = targets[1]->getPos().y + targets[1]->getScale().y + 10; }
	//if should change switches state
	if (shouldChange)
	{
		stateChangeTimer--;
		if (stateChangeTimer <= 0)
		{
			stateChangeTimer = 0;
			switch (targetState)
			{
			case T_MOVING:
				targetState = T_STATIONARY;
				for (int i = 0; i < 3; i++)
				{
					targets[i]->setIsDone(false);
					targets[i]->setType(CEntity::E_TARGET);
					targets[i]->setPos(Vector3(-500 + i * 500, 100.f, 1500.f));
					targets[i]->setOriginPos(targets[i]->getPos());
					targets[i]->setScale(Vector3(40.f, 40.f, 40.f));
					targets[i]->setTarget(Vector3(0.f, 0.f, 0.f));
				}
				break;
			case T_STATIONARY:
				targetState = T_MOVING;
				for (int i = 0; i < 3; i++)
				{
					targetsMoving[i]->setIsDone(false);
					targetsMoving[i]->setType(CEntity::E_MOVING_TARGET);
					targetsMoving[i]->setPos(Vector3(-500 + i * 500, 100.f, 1500.f));
					targetsMoving[i]->setOriginPos(targetsMoving[i]->getPos());
					targetsMoving[i]->setScale(Vector3(40.f, 40.f, 40.f));
					targetsMoving[i]->setTarget(Vector3(0 + i * 500, 100.f, 1500.f));
				}
				break;
			default:
				break;
			}
		}
	}
	//updating based on state (turns off all the other state ones and keeps the current state ones on if they still on
	switch (targetState)
	{
	case T_MOVING:
		for (int i = 0; i < 3; ++i)
		{
			targets[i]->setIsDone(true);
			if(!targetsMoving[i]->isDone()){ targetsMoving[i]->setIsDone(false); }
		}
		break;
	case T_STATIONARY:
		for (int i = 0; i < 3; ++i)
		{
			if (!targets[i]->isDone()) { targets[i]->setIsDone(false); }
			targetsMoving[i]->setIsDone(true);
		}
		break;
	default:
		break;
	}

	bool shouldChange1 = true;
	//check if shouldnt change (any targets still up)
	switch (targetState1)
	{
	case T_MOVING:
		for (int i = 0; i < 3; ++i)
		{
			if(!targetsMoving1[i]->isDone()){ shouldChange1 = false; }
		}
		break;
	case T_STATIONARY:
		for (int i = 0; i < 3; ++i)
		{
			if (!targets1[i]->isDone()) { 
				shouldChange1 = false; 
			}
		}
		break;
	default:
		break;
	}
	if (shouldChange1 && stateChangeTimer1 == 0) { stateChangeTimer1 = targets1[1]->getPos().y + targets1[1]->getScale().y + 10; }
	//if should change switches state
	if (shouldChange1)
	{
		stateChangeTimer1--;
		if (stateChangeTimer1 <= 0)
		{
			stateChangeTimer1 = 0;
			switch (targetState1)
			{
			case T_MOVING:
				targetState1 = T_STATIONARY;
				for (int i = 0; i < 3; i++)
				{
					targets1[i]->setIsDone(false);
					targets1[i]->setType(CEntity::E_TARGET_FIRE);
					targets1[i]->setPos(Vector3(-500 + i * 500, 100.f, -1500.f));
					targets1[i]->setOriginPos(targets1[i]->getPos());
					targets1[i]->setScale(Vector3(40.f, 40.f, 40.f));
					targets1[i]->setTarget(Vector3(0.f, 0.f, 0.f));
				}
				break;
			case T_STATIONARY:
				targetState1 = T_MOVING;
				for (int i = 0; i < 3; i++)
				{
					targetsMoving1[i]->setIsDone(false);
					targetsMoving1[i]->setType(CEntity::E_TARGET_ICE);
					targetsMoving1[i]->setPos(Vector3(-500 + i * 500, 100.f, -1500.f));
					targetsMoving1[i]->setOriginPos(targetsMoving1[i]->getPos());
					targetsMoving1[i]->setScale(Vector3(40.f, 40.f, 40.f));
					targetsMoving1[i]->setTarget(Vector3(0 + i * 500, 100.f, -1500.f));
				}
				break;
			default:
				break;
			}
		}
	}
	//updating based on state (turns off all the other state ones and keeps the current state ones on if they still on
	switch (targetState1)
	{
	case T_MOVING:
		for (int i = 0; i < 3; ++i)
		{
			targets1[i]->setIsDone(true);
			if(!targetsMoving1[i]->isDone()){ targetsMoving1[i]->setIsDone(false); }
		}
		break;
	case T_STATIONARY:
		for (int i = 0; i < 3; ++i)
		{
			if (!targets1[i]->isDone()) { targets1[i]->setIsDone(false); }
			targetsMoving1[i]->setIsDone(true);
		}
		break;
	default:
		break;
	}
	//NOTE : FUTURE REFERENCE FOR PLACING PAINT AT SPECIFIC LOCATIONS (when you're working on projectile collision)
	//PaintTGA documentation is in LoadTGA.h, the following 2 sentences are additional information regarding placement
	//TGA Length Modifier : (1 / (PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 90 ))   , this must be multiplied with the area that you want it to hit
	//I use the number '0.5' in this case because I want the paint to be in the center of the quad, and I must multiply it by the Modifier in order to make sure it renders at the correct position
	//meshList[GEO_TESTPAINTQUAD2]->texturePaintID = PaintTGA(meshList[GEO_TESTPAINTQUAD2]->texturePaintID, 0.5 * (1 / (PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 90)), 0.5 * (1 / (PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 160)), Vector3(0.5, 1, 0), 1, meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint);

	testvar += 0.05 * dt;

	//lights[1].position.Set(drone1->getPos().x, drone1->getPos().y + 350.f * ReadHeightMap(m_heightMap, drone1->getPos().x / 4000, drone1->getPos().z / 4000), drone1->getPos().z);
	//Vector3 tempView = (drone1->getTarget() - drone1->getPos()).Normalized();
	//lights[1].spotDirection.Set(tempView.x, tempView.y, tempView.z);

	glUniform1f(m_parameters[U_FOG_ENABLED], 0);

	fps = (float)(1.f / dt);
	rotateAngle++;
	//UpdateParticles(dt);
	//std::cout << camera.position << std::endl;
}

void SceneRange::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneRange::RenderTerrain() {
	modelStack.PushMatrix();
	modelStack.Scale(4000, 350.f, 4000); // values varies.
	glUniform1f(m_parameters[U_PAINT_TGASTRETCH_X], PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000);
	glUniform1f(m_parameters[U_PAINT_TGASTRETCH_Y], PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000);
	RenderMesh(meshList[GEO_TERRAIN], godlights);
	modelStack.PopMatrix();
}

void SceneRange::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureArray[0] <= 0)
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetInstance().GetWindowWidth() * 0.1f , 0, Application::GetInstance().GetWindowHeight() * 0.1f, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
					mesh->Render((unsigned)text[i] * 6, 6);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneRange::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size_x, float size_y, float x, float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-128, 128, -72, 72, -50, 50);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Scale(size_x, size_y, 1);
				modelStack.Translate(x, y, 0);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
				
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				modelView = viewStack.Top() * modelStack.Top(); // Week 6
				glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
				if (enableLight && bLightEnabled)
				{
					glUniform1i(m_parameters[U_LIGHTENABLED], 1);
					modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
					glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
					//modelView = viewStack.Top() * modelStack.Top();
					//glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
					//modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
					//glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

					Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
					glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

					//load material
					glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
					glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
					glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
					glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
				}
				else
				{
					glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				}
				for (int i = 0; i < MAX_TEXTURES; ++i) {
					if (mesh->textureArray[i] > 0) {
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
						glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
					}
					else {

						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
					}
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void SceneRange::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	if (m_renderPass == RENDER_PASS_PRE)
	{
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED	+ i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
			}
			else
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 0);
		}
		mesh->Render();
		return;
	}

	if (mesh->texturePaintID > 0)
	{
		glUniform1i(m_parameters[U_PAINT_TEXTURE_ENABLED], true);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, mesh->texturePaintID);
		glUniform1i(m_parameters[U_PAINT_TEXTURE], 1);
		glUniform1f(m_parameters[U_PAINT_TEXCOORDSTRETCH], mesh->texCoordStretch);
	}

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top(); // Week 6
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		//modelView = viewStack.Top() * modelStack.Top();
		//glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		//modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		//glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	for (int i = 0; i < MAX_TEXTURES; ++i) {
		if (mesh->textureArray[i] > 0) {
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else {

			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	mesh->Render();
	//if(mesh->textureID > 0)
	/*{
		glBindTexture(GL_TEXTURE_2D, 0);
	}*/

	if (mesh->texturePaintID > 0)
	{
		glUniform1i(m_parameters[U_PAINT_TEXTURE_ENABLED], false);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(m_parameters[U_PAINT_TEXTURE], 0);
		glUniform1f(m_parameters[U_PAINT_TEXCOORDSTRETCH], 1.0f);
	}

	glUniform1f(m_parameters[U_PAINT_TGASTRETCH_X], 0);
	glUniform1f(m_parameters[U_PAINT_TGASTRETCH_Y], 0);

}

void SceneRange::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(400.0f, 400.0f, 400.0f);

	for (int x=0; x<10; x++)
	{
		for (int z=0; z<10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x-5.0f, z-5.0f, 0.0f);
			if ( ((x*9+z) % 2) == 0)
				RenderMesh(meshList[GEO_GRASS_DARKGREEN], false);
			else
				RenderMesh(meshList[GEO_GRASS_LIGHTGREEN], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

void SceneRange::Render()
{
	//******************************* PRE RENDER PASS*************************************
		RenderPassGPass();
	//******************************* MAIN RENDER PASS************************************
		RenderPassMain();
}

void SceneRange::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	while (particleList.size() > 0)
	{
		ParticleObject *particle = particleList.back();
		delete particle;
		particleList.pop_back();
	}
	while (EntityManager::GetInstance()->entityList.size() > 0)
	{
		CEntity *entity = EntityManager::GetInstance()->entityList.back();
		if (entity->getType() != entity->E_PLAYER) { delete entity; }
		EntityManager::GetInstance()->entityList.pop_back();
	}
	playerInfo->DetachCamera();

	/*if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}*/
	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void SceneRange::RenderTrees() 
{
	Vector3 Pos; // Pos to set locate a position for the tree to be planted.
	Pos.Set(20.0f, 0, -100.0f);
	modelStack.PushMatrix();// Push Matrix.
	// To position the tree so that it is sitting properly on the terrain.
	modelStack.Translate(Pos.x, 450 * ReadHeightMap(m_heightMap, Pos.x / 4000,Pos.z / 4000), Pos.z);
	// Based on Method 1 on slide 4, rotate camera’s look position towards the tree.
	modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - Pos.x, camera.position.z - Pos.z)), 0, 1, 0);
	modelStack.Scale(100, 100, 100); // Scale by 100.
	RenderMesh(meshList[GEO_OBJECT], false); // Load tree.
	modelStack.PopMatrix(); //Pop Matrix.
}

// Week 11: Particles
ParticleObject* SceneRange::GetParticle(void)
{
	for (std::vector<ParticleObject *>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject *particle = (ParticleObject *)*it;
		if (!particle->active)
		{
			particle->active = true;
			m_particleCount++;
			return particle;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		ParticleObject *particle = new ParticleObject(ParticleObject_TYPE::P_SMOKE);
		particleList.push_back(particle);
	}
	ParticleObject *particle = particleList.back();
	particle->active = true;
	m_particleCount++;
	return particle;
}

// Week 11: Update Particles
void SceneRange::UpdateParticles(double dt)
{
	if (m_particleCount < MAX_PARTICLE)
	{
		ParticleObject* particle = GetParticle();
		int rand = std::rand() % 2 + 1;
		switch (rand) {
		case 1:
			particle->type = ParticleObject_TYPE::P_SMOKE;
			particle->scale.Set(5, 5, 5);
			particle->vel.Set(1, 5, 1);
			particle->rotationSpeed = 0;// Math::RandFloatMinMax(20.f, 40.f);
			particle->pos.Set(Math::RandFloatMinMax(-20, 10), (ReadHeightMap(m_heightMap, particle->pos.x / 4000.f, particle->pos.z / 4000.f) * 350.f) + 50, Math::RandFloatMinMax(80, 110));
			break;
		case 2:
			particle->type = ParticleObject_TYPE::P_SPARK;
			particle->scale.Set(5, 5, 5);
			particle->vel.Set(Math::RandFloatMinMax(-4.f, 4.f), 5, Math::RandFloatMinMax(-4.f, 4.f));
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos.Set(Math::RandFloatMinMax(-20, 10), (ReadHeightMap(m_heightMap, particle->pos.x / 4000.f, particle->pos.z / 4000.f) * 350.f), Math::RandFloatMinMax(80, 110));
			break;
		}
		
	}


	for (std::vector<ParticleObject *>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject *particle = (ParticleObject *)*it;
		if (particle->active)
		{
			if (particle->type == ParticleObject_TYPE::P_SMOKE)
			{
				/*particle->vel += m_gravity * (float)dt;*/

				particle->pos += particle->vel * (float)dt * 10.f;
				particle->scale *= 0.995f;
				particle->rotation += particle->rotationSpeed * (float)dt;
				if (particle->pos.y > 600.f)
				{
					particle->active = false;
					m_particleCount--;
				}
			}
			if (particle->type == ParticleObject_TYPE::P_SPARK)
			{
				particle->vel += m_gravity * (float)dt;

				particle->pos += particle->vel * (float)dt * 10.f;

				particle->rotation += particle->rotationSpeed * (float)dt;
				if (particle->pos.y < (ReadHeightMap(m_heightMap, particle->pos.x / 4000.f, particle->pos.z / 4000.f) * 350.f))
				{
					particle->active = false;
					m_particleCount--;
				}
			}

			/*if (particle->pos.y < (ReadHeightMap(m_heightMap, particle->pos.x / 4000.f, particle->pos.z / 4000.f) * 350.f))*/
			
		}
	}
}

void SceneRange::RenderParticles(ParticleObject *particle)
{
	switch (particle->type)
	{
	case ParticleObject_TYPE::P_SMOKE:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - particle->pos.x, camera.position.z - particle->pos.z)), 0, 1, 0);
		modelStack.Rotate(particle->rotation, 0, 0, 1);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		//RenderMesh(meshList[GEO_PARTICLE_SMOKE], godlights);
		modelStack.PopMatrix();
		break;
	case ParticleObject_TYPE::P_SPARK:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - particle->pos.x, camera.position.z - particle->pos.z)), 0, 1, 0);
		modelStack.Rotate(particle->rotation, 0, 0, 1);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		//RenderMesh(meshList[GEO_PARTICLE_SPARK], godlights);
		modelStack.PopMatrix();
		break;
	}
}

void SceneRange::RenderWorld()
{
	// Render all entities
	if (!EntityManager::GetInstance()->entityList.empty()) {
		std::list<CEntity*>::iterator it, end;
		end = EntityManager::GetInstance()->entityList.end();
		for (it = EntityManager::GetInstance()->entityList.begin(); it != end; ++it)
		{
			if ((*it)->isDone())
				continue;
			CEntity* ent = *it;
			Vector3 entPos = ent->getPos();
			Vector3 entTar = ent->getTarget();
			Vector3 entSca = ent->getScale();
			switch ((*it)->getType()) {
			case CEntity::E_ENEMY:
			{
				modelStack.PushMatrix();
				modelStack.Translate((*it)->getPos().x, (*it)->getPos().y + 350.f * ReadHeightMap(m_heightMap, (*it)->getPos().x / 4000, (*it)->getPos().z / 4000), (*it)->getPos().z);
				modelStack.Rotate(Math::RadianToDegree(atan2((*it)->getTarget().x - (*it)->getPos().x, (*it)->getTarget().z - (*it)->getPos().z)), 0, 1, 0);
				modelStack.Scale((*it)->getScale().x, (*it)->getScale().y, (*it)->getScale().z);
				RenderMesh(meshList[GEO_SPHERE], godlights);
				modelStack.PopMatrix();
				break;
			}
			
			case CEnemy::E_PROJECTILE:
			{
				CProjectile* proj = dynamic_cast<CProjectile*>(ent);

				if (proj->getProjType() == CProjectile::PTYPE_BEAM)
				{
					modelStack.PushMatrix();
					//modelStack.Translate(camera.position.x, camera.position.y - 1, camera.position.z);
					modelStack.Translate(entPos.x, entPos.y + playerInfo->FirstHeight, entPos.z);
					modelStack.Rotate(Math::RadianToDegree(atan2(entTar.x - entPos.x, entTar.z - entPos.z)) - 180, 0, 1, 0);
					modelStack.Rotate(Math::RadianToDegree(atan2((entTar-entPos).Normalized().y, 1)), 1, 0, 0);
					modelStack.Translate(0, 0, -entSca.z / 2);
					modelStack.Scale(entSca.x, entSca.y, entSca.z);
					RenderMesh(meshList[GEO_BOLT], false);
					modelStack.PopMatrix();
				}
				else
				{
					modelStack.PushMatrix();
					modelStack.Translate(entPos.x, entPos.y + playerInfo->FirstHeight, entPos.z);
					//modelStack.Rotate(Math::RadianToDegree(atan2f(entTar.x - entPos.x, entTar.z - entPos.z)), 0, 1, 0);
					if (proj->getProjType() == CProjectile::PTYPE_FIRE)
					{
						modelStack.Rotate(proj->getProjRot() * 360, 1, 1, 1);
						modelStack.Scale(entSca.x, entSca.y, entSca.z);
						RenderMesh(meshList[GEO_FIREBALL], false);
					}
					else if (proj->getProjType() == CProjectile::PTYPE_ICE)
					{
						modelStack.Rotate(proj->getProjRot() * 360, 1, 1, 1);
						modelStack.Scale(entSca.x, entSca.y, entSca.z);
						RenderMesh(meshList[GEO_ICEBALL], false);
					}
					else if (proj->getProjType() == CProjectile::PTYPE_SPECIAL_KILLERNADO)
					{
						modelStack.Rotate(proj->getProjRot() * 360, 0, 1, 0);
						modelStack.Scale(entSca.x, entSca.y, entSca.z);
						RenderMesh(meshList[GEO_KILLERNADO], false);
					}
					modelStack.PopMatrix();

					if (entPos.y < 350.f * ReadHeightMap(m_heightMap, entPos.x / 4000, entPos.z / 4000) - playerInfo->FirstHeight && proj->getProjType() != CProjectile::PTYPE_SPECIAL_KILLERNADO)
					{
						proj->setIsDone(true);
						proj->EmitParticles(Math::RandIntMinMax(16, 32));
						CSoundEngine::GetInstance()->AddSound("floorImpact", "Sound//floorImpact.mp3");
						CSoundEngine::GetInstance()->AddSound("IceImpact", "Sound//iceimpact.mp3");

						if (proj->getProjType() == CProjectile::PTYPE_FIRE)
						{
							CSoundEngine::GetInstance()->PlayASound("floorImpact");
							meshList[GEO_TERRAIN]->texturePaintID = PaintTGA(
								meshList[GEO_TERRAIN]->texturePaintID,
								((entPos.x / 4000.f) + 0.5f) * (1 / (PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000.f)),
								((entPos.z / 4000.f) + 0.5f) * (1 / (PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000.f)),
								Vector3(0, 0, 0),
								1,
								meshList[GEO_TERRAIN]->tgaLengthPaint,
								PAINT_PATTERNS::PAINT_UNIQUE_FIRE);

							if (proj->getSpellModType() == CProjectile::SMTYPE_SPECIAL)
							{
								CProjectile* aa = new CProjectile(CProjectile::PTYPE_SPECIAL_KILLERNADO);
								Vector3 viewvec = (proj->getTarget() - proj->getPos()).Normalized();
								viewvec.y = 0;
								viewvec.Normalize();
								aa->Init(proj->getPos() + viewvec, proj->getPos() + viewvec * 1.5);
								aa->setPos(aa->getPos() + Vector3(0, aa->getScale().y, 0));
								aa->setTarget(aa->getTarget() + Vector3(0, aa->getScale().y, 0));
							}
						}
						else if (proj->getProjType() == CProjectile::PTYPE_ICE)
						{
							CSoundEngine::GetInstance()->PlayASound("IceImpact");
							meshList[GEO_TERRAIN]->texturePaintID = PaintTGA(
								meshList[GEO_TERRAIN]->texturePaintID,
								((entPos.x / 4000.f) + 0.5f) * (1 / (PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000.f)),
								((entPos.z / 4000.f) + 0.5f) * (1 / (PAINT_LENGTH * meshList[GEO_TERRAIN]->tgaLengthPaint / 4000.f)),
								Vector3(0.6, 0.6, 1.0),
								1,
								meshList[GEO_TERRAIN]->tgaLengthPaint,
								PAINT_PATTERNS::PAINT_BURST);

							if (proj->getSpellModType() == CProjectile::SMTYPE_SPECIAL)
							{
								//spawn ice wall
							}
						}
					}
				}
			}
			break;
			case CEntity::E_DRONE:
			{
				float tempRotation = Math::RadianToDegree(atan2((*it)->getTarget().x - (*it)->getPos().x, (*it)->getTarget().z - (*it)->getPos().z));
				modelStack.PushMatrix();
				modelStack.Translate((*it)->getPos().x, (*it)->getPos().y + 350.f * ReadHeightMap(m_heightMap, (*it)->getPos().x / 4000, (*it)->getPos().z / 4000), (*it)->getPos().z);
				modelStack.Rotate(tempRotation + 180, 0, 1, 0);
				modelStack.Scale((*it)->getScale().x, (*it)->getScale().y, (*it)->getScale().z);
				RenderMesh(meshList[GEO_DRONE_HEAD], godlights);
				modelStack.PopMatrix();

				CDrone* tempDrone = (CDrone*)*it; //this is my lazy code and should be changed asap if possible

				modelStack.PushMatrix();
				modelStack.Translate((*it)->getPos().x, (*it)->getScale().y / 3 + (*it)->getPos().y + 350.f * ReadHeightMap(m_heightMap, (*it)->getPos().x / 4000, (*it)->getPos().z / 4000), (*it)->getPos().z);
				modelStack.Rotate(tempRotation + 180, 0, 1, 0);
				modelStack.Translate(16, 18, -15);
				modelStack.Rotate(tempDrone->getWingRotation(), 0, 0, 1);
				modelStack.Scale((*it)->getScale().x, (*it)->getScale().y, (*it)->getScale().z);
				RenderMesh(meshList[GEO_DRONE_LWING], godlights);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate((*it)->getPos().x, (*it)->getScale().y / 3 + (*it)->getPos().y + 350.f * ReadHeightMap(m_heightMap, (*it)->getPos().x / 4000, (*it)->getPos().z / 4000), (*it)->getPos().z);
				modelStack.Rotate(tempRotation + 180, 0, 1, 0);
				modelStack.Translate(-16, 18, -15);
				modelStack.Rotate(-tempDrone->getWingRotation(), 0, 0, 1);
				modelStack.Scale((*it)->getScale().x, (*it)->getScale().y, (*it)->getScale().z);
				RenderMesh(meshList[GEO_DRONE_RWING], godlights);
				modelStack.PopMatrix();


				
			}
				break;
			case CEntity::E_TARGET:
			case CEntity::E_MOVING_TARGET:
			{
				modelStack.PushMatrix();
				modelStack.Translate((*it)->getPos().x, (*it)->getPos().y + 350.f * ReadHeightMap(m_heightMap, (*it)->getPos().x / 4000, (*it)->getPos().z / 4000), (*it)->getPos().z);
				modelStack.Rotate(rotateAngle, 1, 1, 1);
				modelStack.Scale((*it)->getScale().x, (*it)->getScale().y, (*it)->getScale().z);
				RenderMesh(meshList[GEO_BARREL], godlights);
				modelStack.PopMatrix();
				break;
			}
			case CEntity::E_TARGET_FIRE:
			case CEntity::E_TARGET_ICE:
			{
				modelStack.PushMatrix();
				modelStack.Translate((*it)->getPos().x, (*it)->getPos().y + 350.f * ReadHeightMap(m_heightMap, (*it)->getPos().x / 4000, (*it)->getPos().z / 4000), (*it)->getPos().z);
				//modelStack.Rotate(rotateAngle, 1, 1, 1);
				modelStack.Scale((*it)->getScale().x, (*it)->getScale().y, (*it)->getScale().z);
				RenderMesh(meshList[GEO_BARREL], godlights);
				modelStack.PopMatrix();
				break;
			}
			case CEntity::E_WALL:
			{
				modelStack.PushMatrix();
				modelStack.Translate((*it)->getPos().x, (*it)->getPos().y + 350.f * ReadHeightMap(m_heightMap, (*it)->getPos().x / 4000, (*it)->getPos().z / 4000), (*it)->getPos().z);
				//modelStack.Rotate(rotateAngle, 1, 1, 1);
				modelStack.Scale((*it)->getScale().x, (*it)->getScale().y, (*it)->getScale().z);
				RenderMesh(meshList[GEO_BARREL], godlights);
				modelStack.PopMatrix();
				break;
			}
			case CEntity::E_NPC:
				modelStack.PushMatrix();
				modelStack.Translate(entPos.x, entPos.y + 350.f*ReadHeightMap(m_heightMap, entPos.x / 4000.f, entPos.z / 4000.f), entPos.z);
				modelStack.Rotate(Math::RadianToDegree(atan2(entTar.x - entPos.x, entTar.z - entPos.z)), 0, 1, 0);
				modelStack.PushMatrix();
				modelStack.Translate(0, entSca.y*5.f, 0);
				modelStack.Scale(entSca.x, 5, entSca.z);
				RenderMesh(meshList[GEO_CONE], false);
				modelStack.PopMatrix();
				modelStack.Scale(entSca.x, entSca.y, entSca.z);
				RenderMesh(meshList[GEO_CUBE], false);
				modelStack.PopMatrix();
				break;
			default:
				break;
			}
		}
	}
	if (stateChangeTimer > 0) 
	{
		for (int i = 0; i < 3; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-500 + i * 500, 75.f - stateChangeTimer + 350.f * ReadHeightMap(m_heightMap, (-500 + i * 500) / 4000, (1500.f) / 4000), 1500.f);
			//modelStack.Rotate(Math::RadianToDegree(atan2((*it)->getTarget().x - (*it)->getPos().x, (*it)->getTarget().z - (*it)->getPos().z)), 0, 1, 0);
			modelStack.Scale(40.f, 40.f, 40.f);
			RenderMesh(meshList[GEO_BARREL], godlights);
			modelStack.PopMatrix();
		}
	}
	if (stateChangeTimer1 > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-500 + i * 500, 75.f - stateChangeTimer1 + 350.f * ReadHeightMap(m_heightMap, (-500 + i * 500) / 4000, (1500.f) / 4000), -1500.f);
			//modelStack.Rotate(Math::RadianToDegree(atan2((*it)->getTarget().x - (*it)->getPos().x, (*it)->getTarget().z - (*it)->getPos().z)), 0, 1, 0);
			modelStack.Scale(40.f, 40.f, 40.f);
			RenderMesh(meshList[GEO_BARREL], godlights);
			modelStack.PopMatrix();
		}
	}

	if (!ParticleManager::GetInstance()->particleList.empty()) //RENDERING OF PARTICLES IN PARTICLE MANAGER
	{

		std::list < CParticle_2 *> ::iterator it, end;
		end = ParticleManager::GetInstance()->particleList.end();
		for (it = ParticleManager::GetInstance()->particleList.begin(); it != end; ++it)
		{
			CParticle_2* par = *it;
			Vector3 parPos = par->getPos();
			Vector3 parSca = par->getScale();
			Vector3 parOrigPos = par->getOriginPos();
			float bBoardRot = Math::RadianToDegree(atan2f(camera.position.x - parPos.x, camera.position.z - parPos.z));
			switch (par->getParType())
			{
			case CParticle_2::PTYPE_FIRE:
				modelStack.PushMatrix();
				modelStack.Translate(parPos.x, parPos.y + playerInfo->FirstHeight, parPos.z);
				modelStack.Rotate(bBoardRot, 0, 1, 0);
				modelStack.Rotate(par->getRot(), 0, 0, 1);
				modelStack.Scale(parSca.x, parSca.y, 0.1f);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f - par->getTransparency());
				RenderMesh(meshList[GEO_PARTICLE_FIRE], false);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f);
				modelStack.PopMatrix();
				break;
			case CParticle_2::PTYPE_ICE:
				modelStack.PushMatrix();
				modelStack.Translate(parPos.x, parPos.y + playerInfo->FirstHeight, parPos.z);
				modelStack.Rotate(bBoardRot, 0, 1, 0);
				modelStack.Rotate(par->getRot(), 0, 0, 1);
				modelStack.Scale(parSca.x, parSca.y, 0.1f);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f - par->getTransparency());
				RenderMesh(meshList[GEO_PARTICLE_ICE], false);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f);
				modelStack.PopMatrix();
				break;
			case CParticle_2::PTYPE_BEAM:
				modelStack.PushMatrix();
				modelStack.Translate(parPos.x, parPos.y + playerInfo->FirstHeight, parPos.z);
				modelStack.Rotate(bBoardRot, 0, 1, 0);
				modelStack.Rotate(par->getRot(), 0, 0, 1);
				modelStack.Scale(0.2f, parSca.y, 0.2f);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f - par->getTransparency());
				RenderMesh(meshList[GEO_PARTICLE_ICE], false);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f);
				modelStack.PopMatrix();
				break;
			case CParticle_2::PTYPE_SPECIAL_KILLERNADO:
				modelStack.PushMatrix();
				modelStack.Translate(parPos.x, parPos.y + playerInfo->FirstHeight, parPos.z);
				modelStack.Rotate(bBoardRot, 0, 1, 0);
				modelStack.Rotate(par->getRot(), 0, 0, 1);
				modelStack.Scale(0.2f, parSca.y, 0.2f);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f - par->getTransparency());
				RenderMesh(meshList[GEO_PARTICLE_ICE], false);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f);
				modelStack.PopMatrix();
				break;
			case CParticle_2::PTYPE_TEXT:
			{
				int textLength = par->text.length() + 1;
				modelStack.PushMatrix();
				modelStack.Translate(parPos.x, parPos.y + 350.f*ReadHeightMap(m_heightMap, parOrigPos.x / 4000.f, parOrigPos.z / 4000.f), parPos.z);
				modelStack.Rotate(bBoardRot, 0, 1, 0);
				modelStack.Translate(-parSca.x*textLength*0.4f, 0, 0);
				//RenderMesh(meshList[GEO_SPHERE], godlights);
				modelStack.Rotate(par->getRot(), 0, 0, 1);
				modelStack.Scale(parSca.x, parSca.y, 0.1f);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f - par->getTransparency());
				RenderText(meshList[GEO_TEXT], par->text, par->getColor());
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f);
				modelStack.PopMatrix();
				break;
			}
			}
		}
	}				//RENDERING OF PARTICLES IN PARTICLE MANAGER <<<<<<<<<<<<<<<<<<<<<<<<<END>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 400, 200);
	//modelStack.Rotate(90, 1, 0, 0);
	//modelStack.Scale(20, 20, 1);
	//glUniform1f(m_parameters[U_PAINT_TGASTRETCH_X], PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD]->tgaLengthPaint / 20);
	//glUniform1f(m_parameters[U_PAINT_TGASTRETCH_Y], PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD]->tgaLengthPaint / 20);
	//RenderMesh(meshList[GEO_TESTPAINTQUAD], godlights);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 350, 300);
	//modelStack.Rotate(90, 1, 0, 0);
	//modelStack.Scale(90, 160, 1);
	//glUniform1f(m_parameters[U_PAINT_TGASTRETCH_X], PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 90);
	//glUniform1f(m_parameters[U_PAINT_TGASTRETCH_Y], PAINT_LENGTH * meshList[GEO_TESTPAINTQUAD2]->tgaLengthPaint / 160);
	//RenderMesh(meshList[GEO_TESTPAINTQUAD2], godlights);
	//modelStack.PopMatrix();

	Vector3 tempDir = (camera.target - camera.position).Normalized();
	Vector3 lArmOffset, rArmOffset, lArmRot, rArmRot;
	if (playerInfo)
	{
		lArmOffset = playerInfo->GetLeftArmOffset();
		rArmOffset = playerInfo->GetRightArmOffset();
		lArmRot = playerInfo->GetLeftArmRotation();
		rArmRot = playerInfo->GetRightArmRotation();
	}
	modelStack.PushMatrix();
	if (!playerInfo->rocketMode)
	{
		modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
		modelStack.Rotate(Math::RadianToDegree(-atan2(tempDir.z, tempDir.x)) - 90, 0, 1, 0);
		modelStack.Translate(-3 /*+ playerInfo->GetCameraSway().x*/ + lArmOffset.x, -1.5 + lArmOffset.y, -1.5);
		modelStack.Rotate(Math::RadianToDegree(atan2(tempDir.y, 1)), 1, 0, 0);
		modelStack.Rotate(lArmRot.x, 1, 0, 0);
		modelStack.Rotate(lArmRot.y, 0, 1, 0);
		modelStack.Rotate(lArmRot.z, 0, 0, 1);
		modelStack.Scale(-1, 1, 1.5);
		RenderMesh(meshList[GEO_RIGHTARM], godlights);
	}
	else
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
			0, 0, 0,
			0, 0, -1,
			0, 1, 0
		);

		modelStack.PushMatrix();
		modelStack.Translate(0, -4.5, -5);
		modelStack.Scale(6, 4, -5);
		RenderMesh(meshList[GEO_DRAGON], godlights);
		modelStack.PopMatrix();

		modelStack.Translate(-3 + lArmOffset.x, -1.5 + lArmOffset.y, -1.5);
		modelStack.Rotate(lArmRot.x, 1, 0, 0);
		modelStack.Rotate(lArmRot.y, 0, 1, 0);
		modelStack.Rotate(lArmRot.z, 0, 0, 1);
		modelStack.Scale(-1, 1, 1.5);
		RenderMesh(meshList[GEO_RIGHTARM], godlights);
	}
	if (playerInfo->GetAnimState() == CPlayerInfo::PLR_ANIM_LEFTARM_CASTHOLDING || playerInfo->GetAnimState() == CPlayerInfo::PLR_ANIM_LEFTARM_CASTED)
	{
		modelStack.PushMatrix();
		float timeElapsed = TimeTrackerManager::GetInstance()->getElapsedTime();
		float cosVar = cosf(timeElapsed*8.f);
		modelStack.Scale(1.15f + 0.1f*cosVar, 1.15f + 0.1f*cosVar, 1.15f + 0.1f*cosVar);
		glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f - (0.2f + 0.2f * cosVar));
		glUniform1i(m_parameters[U_UV_OFFSET_ENABLED], 1);
		glUniform2f(m_parameters[U_UV_OFFSET], timeElapsed * 0.3f, timeElapsed * 0.3f);
		RenderMesh(meshList[GEO_RIGHTARM_AURA_ICE], godlights);
		glUniform1i(m_parameters[U_UV_OFFSET_ENABLED], 0);
		glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	if (playerInfo->rocketMode)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);
	}

	modelStack.PushMatrix();
	if (!playerInfo->rocketMode)
	{
		modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
		modelStack.Rotate(Math::RadianToDegree(-atan2(tempDir.z, tempDir.x)) - 90, 0, 1, 0);
		modelStack.Translate(3/* + playerInfo->GetCameraSway().x*/ + rArmOffset.x, -1.5 + rArmOffset.y, -1.5);
		modelStack.Rotate(Math::RadianToDegree(atan2(tempDir.y, 1)), 1, 0, 0);
		modelStack.Rotate(rArmRot.x, 1, 0, 0);
		modelStack.Rotate(rArmRot.y, 0, 1, 0);
		modelStack.Rotate(rArmRot.z, 0, 0, 1);
		modelStack.Scale(1, 1, 1.5);
		RenderMesh(meshList[GEO_RIGHTARM], godlights);
	}
	else
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
			0, 0, 0,
			0, 0, -1,
			0, 1, 0
		);

		modelStack.Translate(3 + rArmOffset.x, -1.5 + rArmOffset.y, -1.5);
		modelStack.Rotate(rArmRot.x, 1, 0, 0);
		modelStack.Rotate(rArmRot.y, 0, 1, 0);
		modelStack.Rotate(rArmRot.z, 0, 0, 1);
		modelStack.Scale(1, 1, 1.5);
		RenderMesh(meshList[GEO_RIGHTARM], godlights);
	}
	if (playerInfo->GetAnimState() == CPlayerInfo::PLR_ANIM_RIGHTARM_CASTHOLDING || playerInfo->GetAnimState() == CPlayerInfo::PLR_ANIM_RIGHTARM_CASTED)
	{
		modelStack.PushMatrix();
		float timeElapsed = TimeTrackerManager::GetInstance()->getElapsedTime();
		float cosVar = cosf(timeElapsed*8.f);
		modelStack.Scale(1.15f + 0.1f*cosVar, 1.15f + 0.1f*cosVar, 1.15f + 0.1f*cosVar);
		glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f - (0.2f + 0.2f * cosVar));
		glUniform1i(m_parameters[U_UV_OFFSET_ENABLED], 1);
		glUniform2f(m_parameters[U_UV_OFFSET], timeElapsed * 0.3f, timeElapsed * 0.3f);
		RenderMesh(meshList[GEO_RIGHTARM_AURA_FIRE], godlights);
		glUniform1i(m_parameters[U_UV_OFFSET_ENABLED], 0);
		glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	if (playerInfo->rocketMode)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);
	}
}

void SceneRange::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 1280.f / 720.f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	for (int i = 0; i < 2; ++i) //MAX IS THE NUMBER OF LIGHTS
	{
		if (lights[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (U_LIGHT1_POSITION - U_LIGHT0_POSITION) * i], 1, &lightDirection_cameraspace.x);
		}
		else if (lights[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (U_LIGHT1_POSITION - U_LIGHT0_POSITION) * i], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[i].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION + (U_LIGHT1_POSITION - U_LIGHT0_POSITION) * i], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (U_LIGHT1_POSITION - U_LIGHT0_POSITION) * i], 1, &lightPosition_cameraspace.x);
		}
	}

	glUniform1f(m_parameters[U_FOG_ENABLED], 0);

	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PushMatrix();
	modelStack.Translate(2, 2000, 2);
	RenderMesh(meshList[GEO_SKYPLANE], godlights);
	modelStack.PopMatrix();

	//RenderGround();
	//RenderSkybox();
	RenderTerrain();
	//RenderTrees();

	////////////////////////////////
	RenderWorld(); //casts shadow
	////////////////////////////////	

	//Render particles
	for (std::vector<ParticleObject *>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject *particle = (ParticleObject *)*it;
		if (particle->active)
		{
			RenderParticles(particle);
		}
	}

	glUniform1f(m_parameters[U_FOG_ENABLED], 0);

	// Render the crosshair
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 12.5f,12.5f);
	if (!CameraEffectManager::GetInstance()->camEfflist.empty()) //RENDERING OF CAMERA EFFECTS IN CAMERA EFFECT MANAGER
	{
		std::list < CameraEffect *> ::iterator it, end;
		end = CameraEffectManager::GetInstance()->camEfflist.end();
		for (it = CameraEffectManager::GetInstance()->camEfflist.begin(); it != end; ++it)
		{
			CameraEffect* camEff = *it;
			switch (camEff->getCamEffectType())
			{
			case CameraEffect::CE_TYPE_ACTIONLINE_WHITE:
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1.f - camEff->getTransparency());
				RenderMeshIn2D(meshList[GEO_SPRITEANIM_ACTIONLINES], false, Application::GetWindowWidth()*0.2f, Application::GetWindowHeight()*0.2f);
				glUniform1f(m_parameters[U_COLOR_ALPHA], 1);
				break;
			}
		}
	}															//RENDERING OF CAMERA EFFECTS IN CAMERA EFFECT MANAGER <<<<<<<<<<<<<<<<<END>>>>>>>>>>>>>>>>>>

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	if (playerInfo->GetCurrentNPC() != NULL)
	{
		CNPC* npc = static_cast<CNPC*>(playerInfo->GetCurrentNPC());
		ss << npc->getCurrentLine();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 0, 4);
	}
	else
	{
		ss << "Score: " << playerInfo->GetScore();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 0, 0);
		std::ostringstream ss1;
		ss1.precision(5);
		ss1 << "Health: " << playerInfo->GetHealth();
		RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 4, 0, 4);
		std::ostringstream ss2;
		ss2.precision(5);
		ss2 << "Mana: " << playerInfo->getMana();
		RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 4, 0, 8);

#ifdef SP3_DEBUG
		ss1.str("");
		ss1 << "TimeTracker Speed: " << TimeTrackerManager::GetInstance()->getSpeed();
		RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 4, 0, 12);
#endif
	}
	
}
void SceneRange::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	//These matrices should change when light position or direction changes
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-100, 100, -100, 100, -100, 200);
	else 
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 200);
	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	RenderWorld();
}
#ifndef SCENE_RANGE_H
#define SCENE_RANGE_H

#define PAINT_LENGTH 20

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Particle.h"
#include "Light.h"
#include "DepthFBO.h"
#include <vector>

#include "SceneManager.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "JoystickController.h"
#include "Entities/EntityManager.h"
#include "Entities/Entity.h"
#include "Entities/Enemy.h"
#include "Entities/Drone.h"
#include "Entities/NPC.h"
#include "PlayerInfo\PlayerInfo.h"
#include "HardwareAbstraction\Keyboard.h"
#include "HardwareAbstraction\Mouse.h"
#include "Entities/Projectile.h"
#include "CameraEffects\CameraEffectManager.h"
#include "TimeTrackerManager.h"

using namespace std;

class SceneRange : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,
		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,

		//PAINT UNIFORM VALUES
		U_PAINT_TEXTURE_ENABLED,
		U_PAINT_TEXTURE,
		U_PAINT_TEXCOORDSTRETCH,
		U_PAINT_TGASTRETCH_X,
		U_PAINT_TGASTRETCH_Y,

		U_SHADOW_MAP,
		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,
		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,
		
		//TRANSPARENCY/ALPHA UNIFORM VALUES
		U_COLOR_ALPHA,

		//UV OFFSET UNIFORM VALUES
		U_UV_OFFSET_ENABLED,
		U_UV_OFFSET,

		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_SPHERE2,
		GEO_SPHERE3,
		GEO_SPHERE4,
		GEO_SPHERE5,
		GEO_SPHERE6,
		GEO_SPHERE7,
		GEO_SPHERE8,
		GEO_SPHERE9,
		GEO_QUAD,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_GRASS_DARKGREEN,
		GEO_GRASS_LIGHTGREEN,
		GEO_OBJECT,
		GEO_TEXT,
		//TSL
		GEO_SKYPLANE,
		GEO_TERRAIN,
		GEO_HEART,
		GEO_MANA,
		GEO_SPRITE_ANIMATION,
		GEO_LIGHT_DEPTH_QUAD,
		//GEO_TESTPAINTQUAD,
		//GEO_TESTPAINTQUAD2,
		GEO_LEFTARM,
		GEO_RIGHTARM,
		GEO_RIGHTARM_AURA_FIRE,
		GEO_RIGHTARM_AURA_ICE,
		GEO_DRAGON,
		GEO_DRONE_HEAD,
		GEO_DRONE_LWING,
		GEO_DRONE_RWING,
		GEO_PARTICLE_FIRE,
		GEO_PARTICLE_ICE,
		GEO_SPRITEANIM_ACTIONLINES,
		GEO_FIREBALL,
		GEO_ICEBALL,
		GEO_BOLT,
		GEO_GOBLIN,
		GEO_BARREL,
		GEO_BARREL_ICE,
		GEO_BARREL_FIRE,
		GEO_LIBRARIAN,
		GEO_KILLERNADO,
		GEO_ICEBLOCK,
		GEO_PILLAR,
		GEO_WATER,
		GEO_ROCKS,
		NUM_GEOMETRY,
	};
	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};
	enum TARGET_STATE
	{
		T_STATIONARY = 0,
		T_MOVING,
	};
public:
	SceneRange();
	~SceneRange();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string Terrain, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size_x = 1.0f, float size_y = 1.0f, float x = 0.0f, float y = 0.0f);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderGround();
	void RenderSkybox();
	void RenderSkyPlane(Mesh* mesh, Color color, int slices, float PlanetRadius, float height, float hTile, float vTile); //TSL
	void RenderTerrain();
	void RenderTrees();
	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();

	ParticleObject* GetParticle(void);
	void UpdateParticles(double dt);
	void RenderParticles(ParticleObject *particle);

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];
	bool godlights = false;

	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;

	bool bLightEnabled;

	float fps;
	int m_particleCount;
	int MAX_PARTICLE;
	Vector3 m_gravity;
	std::vector<ParticleObject*> particleList;

	CKeyboard* theKeyboard;
	CMouse* theMouse;
	CPlayerInfo* playerInfo;

	CEntity* wall1;

	CEntity* targets[3];
	CEntity* targetsMoving[3];
	TARGET_STATE targetState = T_STATIONARY;
	int stateChangeTimer = 0;

	CEntity* targets1[3];
	CEntity* targetsMoving1[3];
	TARGET_STATE targetState1 = T_STATIONARY;
	int stateChangeTimer1 = 0;

	//Terrain
	std::vector<unsigned char> m_heightMap;

	float testvar;
};

#endif
#ifndef SCENE_CONTROLS_H
#define SCENE_CONTROLS_H

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
#include "PlayerInfo\PlayerInfo.h"
#include "HardwareAbstraction\Keyboard.h"
#include "HardwareAbstraction\Mouse.h"
#include "Entities/Projectile.h"

using namespace std;

class SceneControls : public Scene
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
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_TEXT,
		GEO_CONTROLS,
		NUM_GEOMETRY
	};
	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};
public:
	SceneControls();
	~SceneControls();

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


private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];
	bool godlights = true;

	bool bLightEnabled;

	float fps;

};

#endif
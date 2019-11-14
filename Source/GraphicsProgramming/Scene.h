// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <vector>
#include <array>
#include <stdio.h>

#include "SOIL.h"
#include "Input.h"

//Managers
#include "GameEntityManager.h"
#include "AssetManager.h"
#include "RandomTreeCreator.h"
#include "PlanarShadowManager.h"
#include "ImposterShadowManager.h"

//Things that go into scene
#include "Camera.h"
#include "ModelObject.h"
#include "Skybox.h"
#include "Dock.h"
#include "TownCentre.h"
#include "ReflectiveWater.h"

class Scene{
private:
	GameEntityManager gameEntityManager; //Manages all entities.
	AssetManager assetManager;
	RandomTreeCreator randomTreeCreator;

	std::weak_ptr<Camera> camera; //Pointer to camera.
	Skybox skybox; //Skybox

	//For toggling wireframe.
	static bool wireframe;
	bool wasWireframeDown = false;

	//For day/night
	std::vector<int> lightsToEnable; //For determing which lights should be enabled.

	//Toggling day/night
	bool wasIsDaytimeDown = false;

public:
	Scene(Input *in);
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

	/*=======Static Methods=======*/
	static bool IsWireframeEnabled() { return wireframe; }

	//All of these need to be set again in Scene.cpp without the static preheader.
	static std::array<GLfloat, 4> Light0Position; //Sun - w should be 0 to make it a directional light, 1 to make it a point light
	static Vector3 Light0WorldPosition; //Relative shadow position
	static std::array<GLfloat, 4> Light1Position; //Lighthouse light.
	static float Light1Angle; //Where the lighthouse light should be shining at
	static std::array<GLfloat, 4>Light2Position; //Red light above lighthouse

	static bool IsDaytime; //Keeps track whether it is daytime or not.

	/// <summary>
	/// Set light parameters and positions.
	/// </summary>
	static void SetLightsPosition()
	{
		/*=====INITIALISATION=====*/
		GLfloat LightsAmbient[4] = { 0.2f, 0.2f, 0.2f, 1.f };
		GLfloat LightsSpecular[4] = { 1.0, 1.0, 1.0, 1.0 };

		/*The Sun/Main light*/
		GLfloat Light0Diffuse[4] = { 0.35f, 0.35f, 0.3f, 1.f };

		glLightfv(GL_LIGHT0, GL_AMBIENT, LightsAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Diffuse);

		/*The Lighthouse light*/
		GLfloat Light1Diffuse[4] = { 1.0f, 1.f, 0.f, 1.0f };
		GLfloat Light1SpotDirection[3] = { -1.0f, 0.0f, 0.0f };

		glLightfv(GL_LIGHT1, GL_AMBIENT, LightsAmbient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, Light1Diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, LightsSpecular);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Light1SpotDirection);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);

		/*Top of Lighthouse light*/
		GLfloat Light2Diffuse[4] = { 0.8f, 0.f, 0.f, 1.0f };
		GLfloat Light2Specular[4] = { 1.f, 0.f, 0.f, 1.f };
		glLightfv(GL_LIGHT2, GL_AMBIENT, LightsAmbient);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, Light2Diffuse);
		glLightfv(GL_LIGHT2, GL_SPECULAR, Light2Specular);

		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.17f);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.2f);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.06f);

		/*=====PER FRAME POSITION SETTING=====*/
		//LIGHT0 / SUN
		glLightfv(GL_LIGHT0, GL_POSITION, Scene::Light0Position.data());

		//LIGHT1 / LIGHTHOUSE
		glPushMatrix(); //rotate direction
		glRotatef(Scene::Light1Angle, 0.f, 1.f, 0.f);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Light1SpotDirection);
		glPopMatrix();
		glLightfv(GL_LIGHT1, GL_POSITION, Scene::Light1Position.data()); //place inside of lighthouse

#ifdef _DEBUG
		glPushMatrix();  //Draw debug sphere
		glTranslatef(Scene::Light1Position[0], Scene::Light1Position[1], Scene::Light1Position[2]);
		glRotatef(Scene::Light1Angle, 0.f, 1.f, 0.f);
		glutSolidSphere(0.5f, 10, 10);
		glPopMatrix();
#endif
		//LIGHT2 / TOP OF LIGHTHOUSE RED POINT LIGHT
		//turn on the light
		glLightfv(GL_LIGHT2, GL_POSITION, Light2Position.data());
		//Set the sphere position and material
		MaterialData md = { {1.f, 0.f, 0.f, 1.f}, {1.f, 0.f, 0.f, 1.f}, {1.f, 0.f, 0.f, 1.f} };
		md.SetMaterial();
		glPushMatrix();
		glTranslatef(Light2Position[0], Light2Position[1], Light2Position[2]);
		glutSolidSphere(0.05, 10, 10);
		glPopMatrix();
		md.UnsetMaterial();
	}

	/// <summary>
	/// Set the wireframe options.
	/// </summary>
	static void SetWireframeOptions()
	{
		if (Scene::IsWireframeEnabled()) //Check TownCentre::Draw() and Skybox::Draw() for a similar version of this function
		{
			glPolygonMode(GL_BACK, GL_LINE);
			glPolygonMode(GL_FRONT, GL_LINE);
		}
		else
		{
#ifdef _DEBUG
			glPolygonMode(GL_BACK, GL_NONE);
#else
			glPolygonMode(GL_BACK, GL_NONE);
#endif
			glPolygonMode(GL_FRONT, GL_FILL);
		}
	}

protected:
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();

	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];

private:
	/*=======Initialisationa and Loading Methods=======*/
	void loadScene();
	void loadModels();
	void loadTextures();
	void initObjects();
	void placeForest(const Vector3 &aboveLand, std::shared_ptr<ReflectiveWater> reflectiveWaterObj, std::shared_ptr<ImposterShadowManager> shadowManager);
	void placeFlats(const Vector3 &aboveLand, std::shared_ptr<ReflectiveWater> reflectiveWaterObj, std::shared_ptr<PlanarShadowManager> shadowManager);
	void placeHouses(const Vector3 &aboveLand, std::shared_ptr<ReflectiveWater> reflectiveWaterObj, std::shared_ptr<PlanarShadowManager> shadowManager);
};

#endif
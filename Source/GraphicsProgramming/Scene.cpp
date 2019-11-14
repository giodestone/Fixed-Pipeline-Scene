#include "Scene.h"

bool Scene::wireframe = false;

std::array<GLfloat, 4> Scene::Light0Position = { -0.1f, 0.8f, 0.1f, 0.f };
Vector3 Scene::Light0WorldPosition = {Scene::Light0Position[0] * 400.f, Scene::Light0Position[1] * 400.f, Scene::Light0Position[2] * 400.f};
std::array<GLfloat, 4> Scene::Light1Position = { 41.25f, 23.25f, 41.25f, 1.f }; // y = 23.25f
std::array<GLfloat, 4> Scene::Light2Position = { 41.25f, 26.3f, 41.25f, 1.f }; //y += 0.05f
float Scene::Light1Angle = 0.f;
bool Scene::IsDaytime = true;

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in) : gameEntityManager(&assetManager, &lightsToEnable)
{
	// Store pointer for input class
	input = in;
		
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

#ifndef _DEBUG
	glPolygonMode(GL_BACK, GL_NONE);
	glPolygonMode(GL_FRONT, GL_FILL);
#endif // !_DEBUG

	// Other OpenGL / render setting should be applied here.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); //fixes the cones

	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_NORMALIZE);

	// Initialise scene variables
	lightsToEnable.push_back(GL_LIGHT0);
	lightsToEnable.push_back(GL_LIGHT2);
	
	loadScene();


	// Finally set the mouse to the middle.
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
}

void Scene::loadScene()
{
	loadTextures();
	loadModels();

	initObjects();
}

void Scene::loadModels()
{
	/*=======File models=======*/

	assetManager.LoadModel("house1", "models/house1.obj", true);
	assetManager.LoadModel("flats 1", "models/flat1.obj", true);
	assetManager.LoadModel("flats 2", "models/flat2.obj", true);

	/*========Procedural models=======*/

	assetManager.AddModelData("test cone", ModelData::GetCone(1.f, 4, 2.f, 1, false));

	/*Main Discs*/
	assetManager.AddModelData("Main water disc",ModelData::GetDisc(200.f, 20));

	assetManager.AddModelData("main sand ring", ModelData::GetCyclinder(40.f, 20, 0.25f, 1, false));

	assetManager.AddModelData("Main land ring", ModelData::GetCyclinder(37.5f, 20, 0.25f, 1, false));

	/*Lighthouse*/
	assetManager.AddModelData("lighthouse sand ring", ModelData::GetCyclinder(5.f, 20, 0.25f, 1, false));

	assetManager.AddModelData("lighthouse concrete base", ModelData::GetCyclinder(3.f, 20, 1.5f, 1, false));

	assetManager.AddModelData("lighthouse body", ModelData::GetCyclinder(2.f, 20, 20.f, 1, false, false));

	assetManager.AddModelData("lighthouse body topper", ModelData::GetCyclinder(2.1f, 20, 0.5f, 1));

	assetManager.AddModelData("lighthouse bulb container", ModelData::GetCyclinder(1.5f, 20, 2.f, 1, false, false));
	
	assetManager.AddModelData("lighthouse cap", ModelData::GetCone(2.f, 20, 2.f, 10));

	/*Trees*/
	assetManager.AddModelData("trunk1", ModelData::GetCyclinder(0.2f, 10, 0.8f, 2, false, false));
	randomTreeCreator.AddTrunk(0.8f, assetManager.GetModel("trunk1"));

	assetManager.AddModelData("trunk2", ModelData::GetCyclinder(0.22f, 10, 1.f, 2, false, false));
	randomTreeCreator.AddTrunk(1.f, assetManager.GetModel("trunk2"));

	assetManager.AddModelData("trunk3", ModelData::GetCyclinder(0.25f, 10, 1.1f, 2, false, false));
	randomTreeCreator.AddTrunk(1.1f, assetManager.GetModel("trunk3"));

	assetManager.AddModelData("trunk4", ModelData::GetCyclinder(0.22f, 10, 1.4f, 2, false, false));
	randomTreeCreator.AddTrunk(1.4f, assetManager.GetModel("trunk4"));

	assetManager.AddModelData("trunk5", ModelData::GetCyclinder(0.24f, 10, 1.4f, 2, false, false));
	randomTreeCreator.AddTrunk(1.4f, assetManager.GetModel("trunk5"));

	assetManager.AddModelData("canopy1", ModelData::GetSphere(1.f, 10));
	randomTreeCreator.AddCanopy(2.f, assetManager.GetModel("canopy1"));

	assetManager.AddModelData("canopy2", ModelData::GetSphere(0.8f, 10));
	randomTreeCreator.AddCanopy(1.6f, assetManager.GetModel("canopy2"));

	assetManager.AddModelData("canopy3", ModelData::GetSphere(1.5f, 10));
	randomTreeCreator.AddCanopy(3.f, assetManager.GetModel("canopy3"));

	assetManager.AddModelData("canopy4", ModelData::GetSphere(2.f, 10));
	randomTreeCreator.AddCanopy(4.f, assetManager.GetModel("canopy4"));

	assetManager.AddModelData("canopy5", ModelData::GetSphere(1.2f, 10));
	randomTreeCreator.AddCanopy(2.4f, assetManager.GetModel("canopy5"));

	/*Mountains*/
	assetManager.AddModelData("mountain big", ModelData::GetCone(15.f, 35, 28.f, 10, false));
	assetManager.AddModelData("mountain supporting", ModelData::GetCone(8.f, 30, 18.f, 10, false));
	assetManager.AddModelData("mountain other", ModelData::GetCone(10.f, 30, 24.f, 10, false));

	/*Town Square*/
	assetManager.AddModelData("town square face", ModelData::GetXYQuad(1.f, 1.f));
}

void Scene::loadTextures()
{
	std::vector<std::pair<std::string, std::string>> paths; //alias, path
	//paths.push_back(std::make_pair("alias", "gfx/image.png"));
	paths.push_back(std::make_pair("skybox", "gfx/Skybox.png"));
	paths.push_back(std::make_pair("world map", "gfx/world-map.png"));
	paths.push_back(std::make_pair("crate", "gfx/crate.png"));
	paths.push_back(std::make_pair("house1 texture a", "gfx/house1TextureA.png"));
	paths.push_back(std::make_pair("house1 texture b", "gfx/house1TextureB.png"));
	paths.push_back(std::make_pair("lighthouse body", "gfx/lighthouse.png"));
	paths.push_back(std::make_pair("flats texture", "gfx/flatsTexture1.png"));
	paths.push_back(std::make_pair("planks", "gfx/planks.png"));
	paths.push_back(std::make_pair("mountain", "gfx/mountain.png"));
	paths.push_back(std::make_pair("town square outside", "gfx/townSquareOutside.png"));
	paths.push_back(std::make_pair("imposter", "gfx/imposter.png"));

	assetManager.LoadTextures(paths);
}

void Scene::initObjects()
{
	/*To place the object on top:
		y = base object HEIGHT + 1/2 of current object height
		object on top y = base object HEIGHT + last object HEIGHT + 1/2 current object height
		...
		...
		...
	*/
	//Camera
	auto camera = std::make_shared<Camera>();
	this->camera = camera;
	gameEntityManager.AddEntity(camera);

	//Land Masses
	auto reflectiveWater = std::make_shared<ReflectiveWater>(assetManager.GetModel("Main water disc"), MaterialData({ 0.25f, 0.31f, 0.43f, 1.f }, { 0.25f, 0.31f, 0.43f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, 0.0001f), camera, 0.f, 100.f);
	reflectiveWater->SetPosition(Vector3(0.f, 0.f, 0.f));
	reflectiveWater->SetDrawOrder((UINT_MAX / 2) - 200);
	gameEntityManager.AddEntity(reflectiveWater);

	auto mainSandRing = std::make_shared<ModelObject>(assetManager.GetModel("main sand ring"), MaterialData({ 0.9f, 0.9f, 0.f, 1.f }, { 0.9f, 0.9f, 0.f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, 0.0001f));
	mainSandRing->SetPosition(Vector3(0.f, 0.125f, 0.f));
	mainSandRing->SetDrawOrder((UINT_MAX / 2) - 199);
	gameEntityManager.AddEntity(mainSandRing);
	reflectiveWater->AddObjectToReflect(mainSandRing);

	auto mainLandRing = std::make_shared<ModelObject>(assetManager.GetModel("Main land ring"), MaterialData({ 0.29f, 0.5f, 0.20f, 1.f }, { 0.29f, 0.5f, 0.20f, 1.f }, { 0.f, 0.f, 0.f, 0.f, }, 0.0001f));
	mainLandRing->SetPosition(Vector3(0.f, 0.25f + 0.125f, 0.f));
	mainLandRing->SetDrawOrder((UINT_MAX / 2) - 198); //Make draw order that so the shadow manager draws afterwards
	gameEntityManager.AddEntity(mainLandRing);
	reflectiveWater->AddObjectToReflect(mainLandRing);

	Vector3 pos = mainLandRing->GetPosition();
	pos.SetY(pos.GetY() + 0.125f);
	const Vector3 aboveLand = pos; //quick reference as a lot of things will be needing it

	//Lighthouse
	auto lightHouseSandRing = std::make_shared<ModelObject>(assetManager.GetModel("lighthouse sand ring"), MaterialData({ 0.9f, 0.9f, 0.f, 1.f }, { 0.9f, 0.9f, 0.f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, 0.0001f));
	lightHouseSandRing->SetPosition(Vector3(40.f, 0.f + 0.125f, 40.f));
	gameEntityManager.AddEntity(lightHouseSandRing);
	reflectiveWater->AddObjectToReflect(lightHouseSandRing);

	auto lightHouseConcreteBase = std::make_shared<ModelObject>(assetManager.GetModel("lighthouse concrete base"), MaterialData({ 0.4f, 0.4f, 0.4f, 1.f }, { 0.4f, 0.4f, 0.4f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, 0.0001f));
	lightHouseConcreteBase->SetPosition(Vector3(41.25f, 0.25f + 0.75f, 41.25f));
	gameEntityManager.AddEntity(lightHouseConcreteBase);
	reflectiveWater->AddObjectToReflect(lightHouseConcreteBase);

	auto lightHouseBody = std::make_shared<ModelObject>(assetManager.GetModel("lighthouse body"), MaterialData::GetDefault(), "lighthouse body");
	lightHouseBody->SetPosition(Vector3(41.25f, 1.75f + 10.f, 41.25f));
	gameEntityManager.AddEntity(lightHouseBody);
	reflectiveWater->AddObjectToReflect(lightHouseBody);

	auto lightHouseBodyTopper = std::make_shared<ModelObject>(assetManager.GetModel("lighthouse body topper"), MaterialData({ 0.2f, 0.2f, 0.2f, 1.f }, { 0.3f, 0.3f, 0.3f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, 0.0001f));
	lightHouseBodyTopper->SetPosition(Vector3(41.25f, 21.75f + 0.25f, 41.25f));
	gameEntityManager.AddEntity(lightHouseBodyTopper);
	reflectiveWater->AddObjectToReflect(lightHouseBodyTopper);

	auto lightHouseBulbContainer = std::make_shared<ModelObject>(assetManager.GetModel("lighthouse bulb container"), MaterialData({ 0.1f, 0.1f, 0.1f, 1.f }, { 0.1f, 0.1f, 0.1f, 1.f }, { 0.6f, 0.6f, 0.f, 1.f }, 0.0001f));
	lightHouseBulbContainer->SetPosition(Vector3(41.25f, 22.25f + 1.f, 41.25f));
	gameEntityManager.AddEntity(lightHouseBulbContainer);
	reflectiveWater->AddObjectToReflect(lightHouseBulbContainer);

	auto lightHouseCap = std::make_shared<ModelObject>(assetManager.GetModel("lighthouse cap"), MaterialData({ 0.4f, 0.4f, 0.4f, 1.f }, { 0.05f, 0.05f, 0.05f, 1.f }, { 0.f, 0.f ,0.f, 0.f }, 0.001f));
	lightHouseCap->SetPosition(Vector3(41.25f, 24.25f, 41.25f));
	gameEntityManager.AddEntity(lightHouseCap);
	reflectiveWater->AddObjectToReflect(lightHouseCap);

	//Shadow managers
	auto imposterShadowManager = std::make_shared<ImposterShadowManager>(aboveLand.GetY() + 0.01f, assetManager.GetTexture("imposter"));
	gameEntityManager.AddEntity(imposterShadowManager);
	auto planarShadowManager = std::make_shared<PlanarShadowManager>(aboveLand.GetY() + 0.01f);
	gameEntityManager.AddEntity(planarShadowManager);

	placeHouses(aboveLand, reflectiveWater, planarShadowManager);
	placeFlats(aboveLand, reflectiveWater, planarShadowManager);
	placeForest(aboveLand, reflectiveWater, imposterShadowManager);

	auto dock = std::make_shared<Dock>(assetManager.GetTexture("planks"));
	dock->SetPosition(Vector3(-37.5f, aboveLand.GetY() - 0.2f, 0.f)); //put it just above the beach
	gameEntityManager.AddEntity(dock);
	reflectiveWater->AddObjectToReflect(dock);

	auto mountainBig = std::make_shared<ModelObject>(assetManager.GetModel("mountain big"), MaterialData({ 1.f, 1.f, 1.f, 1.f }, {0.4f, 0.4f, 0.4f, 1.f}, { 0.f, 0.f ,0.f, 0.f }, 0.001f), "mountain");
	mountainBig->SetPosition(Vector3(20.f, aboveLand.GetY(), -9.f));
	gameEntityManager.AddEntity(mountainBig);
	reflectiveWater->AddObjectToReflect(mountainBig);

	auto mountainSupporting = std::make_shared<ModelObject>(assetManager.GetModel("mountain supporting"), MaterialData({ 1.f, 1.f, 1.f, 1.f }, { 0.4f, 0.4f, 0.4f, 1.f }, { 0.f, 0.f ,0.f, 0.f }, 0.001f), "mountain");
	mountainSupporting->SetPosition(Vector3(7.f, aboveLand.GetY(), -18.f));
	gameEntityManager.AddEntity(mountainSupporting);
	reflectiveWater->AddObjectToReflect(mountainSupporting);

	auto mountainOther = std::make_shared<ModelObject>(assetManager.GetModel("mountain other"), MaterialData({ 1.f, 1.f, 1.f, 1.f }, { 0.4f, 0.4f, 0.4f, 1.f }, { 0.f, 0.f ,0.f, 0.f }, 0.001f), "mountain");
	mountainOther->SetPosition(Vector3(12.f, aboveLand.GetY(), -24.f));
	gameEntityManager.AddEntity(mountainOther);
	reflectiveWater->AddObjectToReflect(mountainOther);
	
	auto townCentre = std::make_shared<TownCentre>(camera, assetManager.GetModel("town square face"), assetManager.GetTexture("town square outside"));
	townCentre->SetPosition(Vector3(0.f, aboveLand.GetY() + 1.f, 27.5f)); //z = 27.5f
	gameEntityManager.AddEntity(townCentre);
	reflectiveWater->AddObjectToReflect(townCentre);

	skybox.Load(assetManager, camera); //load skybox texture
}

void Scene::placeForest(const Vector3 &aboveLand, std::shared_ptr<ReflectiveWater> reflectiveWaterObj, std::shared_ptr<ImposterShadowManager> shadowManager)
{
	const float xSeperation = 5.55f;
	const float zSeperation = 5.55f;
	const float perLoopZSeperationIncrease = -2.5f;

	const Vector3 posOffset = { -24.f, aboveLand.GetY(), -24.f };

	for (int z = 0; z < 5; z++)
	{
		for (int x = 0; x < 5; x++)
		{
#ifdef _DEBUG
			if (true) // test is disabled for placing purpouses.
#else
			if (rand() % 2 == 0) //1 in 3 chance a tree won't be placed
#endif
			{
				auto tree = randomTreeCreator.GetRandomTree();
				tree->SetPosition(Vector3(posOffset.GetX() + xSeperation * x + ((rand() % 150) / 100.f),
										  posOffset.GetY() - 0.125f,
										  posOffset.GetZ() + zSeperation * z + ((rand() % 150) / 100.f)) + z * perLoopZSeperationIncrease);
				gameEntityManager.AddEntity(tree);
				reflectiveWaterObj->AddObjectToReflect(tree);
				shadowManager->AddObjectToImposterShadow(tree, tree->GetSizeData());
			}
		}
	}
}

void Scene::placeFlats(const Vector3 &aboveLand, std::shared_ptr<ReflectiveWater> reflectiveWaterObj, std::shared_ptr<PlanarShadowManager> shadowManager)
{
	const float offsetX = -18.f, offsetZ = 19.f;
	const float widthX = 5.5f, widthZ = 4.5f;

	Vector3 rotation = { 0.f, 0.f, 0.f };
	const Vector3 scale = { 1.5f, 1.5f, 1.5f };
	for (int z = 0; z < 2; z++)
	{
		rotation.y = 3.14159f;
		for (int x = 0; x < 3; x++)
		{
			const Vector3 position = { offsetX + x * widthX, aboveLand.GetY(), offsetZ + z * widthZ };
			std::string texture;
			texture = "flats texture";

			auto flat = std::make_shared<ModelObject>(assetManager.GetModel("flats 1"), MaterialData::GetDefault(), texture);
			flat->SetPosition(position);
			flat->SetRotation(rotation);
			flat->SetScale(scale);
			gameEntityManager.AddEntity(flat);
			reflectiveWaterObj->AddObjectToReflect(flat);
			shadowManager->AddObjectToPlanarShadow(flat);
		}
	}
}

void Scene::placeHouses(const Vector3 &aboveLand, std::shared_ptr<ReflectiveWater> reflectiveWaterObj, std::shared_ptr<PlanarShadowManager> shadowManager)
{
	const float offsetX = 17.f, offsetZ = 17.f;
	const float widthX = 3.6f, widthZ = 3.f;
	const Vector3 rotation = { 0.f, 0.5f, 0.f };
	
	for (int z = 0; z < 2; z++)
	{
		for (int x = 0; x < 3; x++)
		{
			const Vector3 position = { offsetX + x * widthX, aboveLand.GetY(), offsetZ + z * widthZ };
			std::string texture;
			if (rand() % 2 == 0)
				texture = "house1 texture a";
			else
				texture = "house1 texture b";

			auto house = std::make_shared<ModelObject>(assetManager.GetModel("house1"), MaterialData::GetDefault(), texture);
			house->SetPosition(position);
			house->SetRotation(rotation);
			gameEntityManager.AddEntity(house);
			reflectiveWaterObj->AddObjectToReflect(house);
			shadowManager->AddObjectToPlanarShadow(house);
		}
	}
}

void Scene::handleInput(float dt)
{
	gameEntityManager.ProcessInput(*input, dt);

	if (input->isKeyDown('p') && !wasWireframeDown)
	{
		wireframe = !wireframe;

		SetWireframeOptions();

		wasWireframeDown = true;
	}
	else if (!input->isKeyDown('p'))
	{
		wasWireframeDown = false;
	}

	if (input->isKeyDown('o') && !wasIsDaytimeDown)
	{
		IsDaytime = !IsDaytime;
		wasIsDaytimeDown = true;

		//Update what lights to enable depending on daytime
		if (Scene::IsDaytime)
		{
			lightsToEnable = { GL_LIGHT0, GL_LIGHT2 }; //sun and other light
		}
		else
		{
			lightsToEnable = { GL_LIGHT1, GL_LIGHT2 }; //lighhouse and other light
		}
	}
	else if (!input->isKeyDown('o'))
	{
		wasIsDaytimeDown = false;
	}
}

void Scene::update(float dt)
{
	//Update light 2 angle
	Light1Angle = Light1Angle + (35.f * dt);

	// update scene related variables.
	gameEntityManager.Update(dt);

	// Calculate FPS for output
	calculateFPS();
}



void Scene::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear Color and Depth Buffers
	glLoadIdentity(); // Reset transformations


	gluLookAt(
		camera.lock()->GetRefToPosition().GetX(), camera.lock()->GetRefToPosition().GetY(), camera.lock()->GetRefToPosition().GetZ(),
		camera.lock()->GetLookAt().GetX(), camera.lock()->GetLookAt().GetY(), camera.lock()->GetLookAt().GetZ(),
		camera.lock()->GetUp().GetX(), camera.lock()->GetUp().GetY(), camera.lock()->GetUp().GetZ()); // Set the camera	
	skybox.Draw(); //Draw Skybox - do before drawing anything else


// Render geometry/scene here -------------------------------------
	
	/*Set lights position - NEEDS TO BE DONE FOR EVERY LIGHT EVERY FRAME.*/
	SetLightsPosition();

	/*Draw Entities*/
	gameEntityManager.Draw();
	
	// End render geometry --------------------------------------

	renderTextOutput(); // Render text, should be last object rendered.
	glutSwapBuffers(); 	// Swap buffers, after all objects are rendered.
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 200.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	
}

// Calculates FPS
void Scene::calculateFPS()
{

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
#ifdef _DEBUG
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, mouseText);
#endif
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}

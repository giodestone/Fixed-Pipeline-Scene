#include "PlanarShadowManager.h"

#include "Scene.h"

PlanarShadowManager::PlanarShadowManager(float yOfFloor)
{
	floorPlane =
	{
		-1.f, yOfFloor, -1.f, //top left
		-1.f, yOfFloor, 1.f, // bottom left
		1.f, yOfFloor, 1.f, //bottom right
		1.f, yOfFloor, -1.f // top right
	};

	SetDrawOrder((UINT_MAX / 2) - 100);
}


PlanarShadowManager::~PlanarShadowManager()
{}

void PlanarShadowManager::Draw()
{
	if (Scene::IsDaytime) //Shadow sun only
	{
		drawPlanarShadows();
	}
}

void PlanarShadowManager::drawPlanarShadows()
{
	float shadowMatrix[16] = { 0 };

	//Determine what light to shadow
	int lightToShadow;
	std::array<GLfloat, 4> lightPos;

	lightPos = Scene::Light0WorldPosition.GetVector4v();
	lightToShadow = GL_LIGHT0;

	for (auto& entity : entitiesToPlanarShadow)
	{
		generateShadowMatrix(shadowMatrix, lightPos.data(), floorPlane.data());

		//render shadow
		glDisable(GL_DEPTH_TEST); //disable depth test
		glDisable(GL_LIGHTING); //lighting,
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D); //and texture

		glColor4f(0.3f, 0.3f, 0.3f, 1.f); // set the Shadow's colour
		glPushMatrix();
		glMultMatrixf((GLfloat *)shadowMatrix); //apply shaodw matrix transform

		/*Transformations are automatically matched with the object*/
		entity.lock()->DisableTexturing();
		entity.lock()->Draw();
		entity.lock()->EnableTexturing();
		glPopMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, 1.f); // reset colour

		glEnable(GL_DEPTH_TEST); //reenable depth, lughting and texture
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}
}

void PlanarShadowManager::AddObjectToPlanarShadow(std::weak_ptr<GameEntity> object)
{
	entitiesToPlanarShadow.emplace_back(object);
}

void PlanarShadowManager::generateShadowMatrix(float* shadowMatrix, float light_pos[4], GLfloat floor[12])
{

	//Defining vertices of plane are PQR with edges PQ and PR
	Vector3 P(floor[0], floor[1], floor[2]);	//top left
	Vector3 Q(floor[3], floor[4], floor[5]);	// bottom left
	Vector3 R(floor[9], floor[10], floor[11]);	// top right

	Vector3 PQ = (Q - P).Normalised();
	Vector3 PR = (R - P).Normalised();
	Vector3 normal = PR.Cross(PQ);

	//Equation of plane is ax + by + cz = d
	//a, b and c are the coefficients of the normal to the plane (i.e. normal = ai + bj + ck)
	//If (x0, y0, z0) is any point on the plane, d = a*x0 + b*y0 + c*z0
	//i.e. d is the dot product of any point on the plane (using P here) and the normal to the plane
	float a, b, c, d;
	a = normal.GetX();
	b = normal.GetY();
	c = normal.GetZ();
	d = normal.Dot(P);

	//Origin of projection is at x, y, z. Projection here originating from the light source's position
	float x, y, z;
	x = light_pos[0];
	y = light_pos[1];
	z = light_pos[2];

	//This is the general perspective transformation matrix from a point (x, y, z) onto the plane ax + by + cz = d
	shadowMatrix[0] = d - (b * y + c * z);
	shadowMatrix[1] = a * y;
	shadowMatrix[2] = a * z;
	shadowMatrix[3] = a;

	shadowMatrix[4] = b * x;
	shadowMatrix[5] = d - (a * x + c * z);
	shadowMatrix[6] = b * z;
	shadowMatrix[7] = b;

	shadowMatrix[8] = c * x;
	shadowMatrix[9] = c * y;
	shadowMatrix[10] = d - (a * x + b * y);
	shadowMatrix[11] = c;

	shadowMatrix[12] = -d * x;
	shadowMatrix[13] = -d * y;
	shadowMatrix[14] = -d * z;
	shadowMatrix[15] = -(a * x + b * y + c * z);
}

void PlanarShadowManager::extendVertex(float newVert[3], float lightPosit[4], float x, float y, float z, float ext)
{
	float lightDir[3];

	// Create a vector that points from the light's position to the original vertex.
	lightDir[0] = x - lightPosit[0];
	lightDir[1] = y - lightPosit[1];
	lightDir[2] = z - lightPosit[2];

	// Then use that vector to extend the original vertex out to a new position.
	// The distance to extend or extrude the new vector is specified by t.
	newVert[0] = lightPosit[0] + lightDir[0] * ext;
	newVert[1] = lightPosit[1] + lightDir[1] * ext;
	newVert[2] = lightPosit[2] + lightDir[2] * ext;
}

std::vector<float> PlanarShadowManager::buildShadowVolume(float lightPosit[4], std::vector<float> verts)
{
	std::vector<float> shadowVolume;
	float extrusion = 5.f;

	// Clear previous shadow volume
	shadowVolume.clear();

	//Build new shadow volume

	// Temporary variable for storing newly calculated vertcies
	float vExtended[3];


	// For each vertex of the shadow casting object, find the edge 
	// that it helps define and extrude a quad out from that edge.
	for (size_t i = 0; i < verts.size(); i += 3)
	{
		// Define the edge we're currently working on extruding...
		int e0 = i;
		int e1 = i + 3;

		// If the edge's second vertex is out of array range, 
		// place it back at 0
		if (e1 >= verts.size())
		{
			e1 = 0;
		}
		// v0 of our extruded quad will simply use the edge's first 
		// vertex or e0.
		shadowVolume.push_back(verts[e0]);
		shadowVolume.push_back(verts[e0 + 1]);
		shadowVolume.push_back(verts[e0 + 2]);

		// v1 of our quad is created by taking the edge's first 
		// vertex and extending it out by some amount.
		extendVertex(vExtended, lightPosit, verts[e0], verts[e0 + 1], verts[e0 + 2], extrusion);
		shadowVolume.push_back(vExtended[0]);
		shadowVolume.push_back(vExtended[1]);
		shadowVolume.push_back(vExtended[2]);

		// v2 of our quad is created by taking the edge's second 
		// vertex and extending it out by some amount.
		extendVertex(vExtended, lightPosit, verts[e1], verts[e1 + 1], verts[e1 + 2], extrusion);
		shadowVolume.push_back(vExtended[0]);
		shadowVolume.push_back(vExtended[1]);
		shadowVolume.push_back(vExtended[2]);

		// v3 of our extruded quad will simply use the edge's second 
		//// vertex or e1.
		shadowVolume.push_back(verts[e1]);
		shadowVolume.push_back(verts[e1 + 1]);
		shadowVolume.push_back(verts[e1 + 2]);
	}
	return shadowVolume;
}
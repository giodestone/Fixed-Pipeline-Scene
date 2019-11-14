#include "TownCentre.h"

using namespace TownCentreHelper;

TownCentre::TownCentre(std::weak_ptr<Camera> camera, std::weak_ptr<ModelData> face, GLint faceTexture)
	: camera(camera), 
	faceData(face), 
	faceTexture(faceTexture)
{
	SetDrawOrder(UINT_MAX - 1);
}


TownCentre::~TownCentre()
{}


void TownCentre::Update(float dt)
{
	//find the distances to the faces which are the closest to the camera
	for (auto& face : faceDrawOrder)
	{
		//Get the LOCAL location of the face
		Vector3 positionOfFace = face.GetLocalPosition();

		//Add the position of the object to the LOCAL position (global position)
		positionOfFace += GetPosition();

		//Get the postion of the camera
		auto cameraPos = camera.lock()->GetPosition();

		//get the distance between two vectors
		auto distanceSq = (cameraPos - positionOfFace).lengthSquared();

		//push distance into the face
		face.SetDistanceSqFromCam(distanceSq);
	}
	//now sort through faces determining which should be drawn first
	std::sort(faceDrawOrder.begin(), faceDrawOrder.end(), TownCentreFaces::SortTownCentreFaces);

	angle1 += 25.f * dt;
	angle2 += 15.f * dt;
}

void TownCentre::Draw()
{
	glPushMatrix();

	glTranslatef(GetRefToPosition().GetX(), GetRefToPosition().GetY(), GetRefToPosition().GetZ());

	glPushMatrix(); //Spinny thingy in the middle.
	{
		//draw base
		glTranslatef(0.f, 0.f, 0.f); //translate up
		drawCuboid(0.3f, 0.6f, 0.3f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);

		//draw rotating things
		glPushMatrix(); //main rotating thing # 1
		{
			glTranslatef(0.f, 0.3f + 0.05f, 0.f);
			glRotatef(angle1, 0.f, 1.f, 0.f);
			drawCuboid(0.1f, 0.1f, 0.6f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
			glPushMatrix(); //spinny thingy bottom
			{
				glTranslatef(0.f, 0.f, 0.3 + 0.025f);
				glRotatef(angle2, 0.f, 0.f, 1.f);
				drawCuboid(0.2f, 0.05f, 0.05f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);	
			}
			glPopMatrix();
			glPushMatrix(); //spinny thingy top
			{
				glTranslatef(0.f, 0.f, -0.3 - 0.025f);
				glRotatef(angle2, 0.f, 0.f, 1.f);
				drawCuboid(0.2f, 0.05f, 0.05f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
			}
			glPopMatrix();
			//maybe draw rotating things on the rotating things.
		}
		glPopMatrix();
		
		glPushMatrix(); //main rotating thing #2
		{
			glTranslatef(0.f, -0.3f - 0.05f, 0.f);
			glRotatef(-angle1, 0.f, 1.f, 0.f);
			drawCuboid(0.1f, 0.1f, 0.6f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
			glPushMatrix(); //spinny thingy bottom
			{
				glTranslatef(0.f, 0.f, 0.3 + 0.025f);
				glRotatef(-angle2, 0.f, 0.f, 1.f);
				drawCuboid(0.2f, 0.05f, 0.05f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);	
			}
			glPopMatrix();
			glPushMatrix(); //spinny thingy top
			{
				glTranslatef(0.f, 0.f, -0.3 - 0.025f);
				glRotatef(-angle2, 0.f, 0.f, 1.f);
				drawCuboid(0.2f, 0.05f, 0.05f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
			}
			glPopMatrix();
			//maybe draw rotating things on the rotating things.
		}
		glPopMatrix();
	}
	glPopMatrix();

	/*=====Draw Exterior WALLS=====*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, faceData.lock()->Vertices.data());

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, faceData.lock()->Normals.data());
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, faceData.lock()->UVs.data());
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, faceTexture);

	glEnable(GL_BLEND);

	if (Scene::IsWireframeEnabled())
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (auto face : faceDrawOrder)
	{
		glPushMatrix();
		glTranslatef(face.GetLocalPosition().GetX(), face.GetLocalPosition().GetY(), face.GetLocalPosition().GetZ()); //translate to correct coords.

		switch (face.GetFace()) //rotate accordingly to face to get the backface correct.
		{
		case TOWN_CENTRE_FACE::FRONT:
			break;
		case TOWN_CENTRE_FACE::BACK:
			glRotatef(-180.f, 0.f, 1.f, 0.f);
			break;
		case TOWN_CENTRE_FACE::LEFT: //IF THE FACE IS DRAWING BACKWARDS JUST CHECK HERE!!!!!!!!!!!!!!!!!!!!!!!
			glRotatef(-90.f, 0.f, 1.f, 0.f);
			break;
		case TOWN_CENTRE_FACE::RIGHT:
			glRotatef(90.f, 0.f, 1.f, 0.f);
			break;
		}
		glDrawArrays(faceData.lock()->Mode, 0, faceData.lock()->Vertices.size() / 3);
		
		glPopMatrix();
	}

	Scene::SetWireframeOptions(); //reset polygon modes
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, NULL); //REMEMBER ORDER!
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

void TownCentre::drawCuboid(float halfXSize, float halfYSize, float halfZSize, float topU, float topV, float frontU, float frontV, float leftU, float leftV)
{
	glBegin(GL_QUADS);

	//TOP FACE (xz)
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-halfXSize / 2.f, halfYSize / 2.f, halfZSize / 2.f); //-+
	glTexCoord2f(topU, 0.f);
	glVertex3f(halfXSize / 2.f, halfYSize / 2.f, halfZSize / 2.f); //++
	glTexCoord2f(topU, topV);
	glVertex3f(halfXSize / 2.f, halfYSize / 2.f, -halfZSize / 2.f); //-+
	glTexCoord2f(0.f, topV);
	glVertex3f(-halfXSize / 2.f, halfYSize / 2.f, -halfZSize / 2.f); //--

	//BOTTOM FACE (xz)
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(0.f, topV);
	glVertex3f(-halfXSize / 2.f, -halfYSize / 2.f, halfZSize / 2.f); //-+
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-halfXSize / 2.f, -halfYSize / 2.f, -halfZSize / 2.f); //--
	glTexCoord2f(topU, 0.f);
	glVertex3f(halfXSize / 2.f, -halfYSize / 2.f, -halfZSize / 2.f); //+-
	glTexCoord2f(topU, topV);
	glVertex3f(halfXSize / 2.f, -halfYSize / 2.f, halfZSize / 2.f); //++

	//BACK FACE (xy)
	glNormal3f(0.f, 0.f, -1.f);
	glTexCoord2f(0.f, frontV);
	glVertex3f(-halfXSize / 2.f, halfYSize / 2.f, -halfZSize / 2.f); //-+
	glTexCoord2f(frontU, frontV);
	glVertex3f(halfXSize / 2.f, halfYSize / 2.f, -halfZSize / 2.f); //++
	glTexCoord2f(frontU, 0.f);
	glVertex3f(halfXSize / 2.f, -halfYSize / 2.f, -halfZSize / 2.f); //+-
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-halfXSize / 2.f, -halfYSize / 2.f, -halfZSize / 2.f); //--

	//FRONT FACE (xy)
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.f, frontV);
	glVertex3f(-halfXSize / 2.f, halfYSize / 2.f, halfZSize / 2.f); //-+
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-halfXSize / 2.f, -halfYSize / 2.f, halfZSize / 2.f); //--
	glTexCoord2f(frontU, 0.f);
	glVertex3f(halfXSize / 2.f, -halfYSize / 2.f, halfZSize / 2.f); //+-
	glTexCoord2f(frontU, frontV);
	glVertex3f(halfXSize / 2.f, halfYSize / 2.f, halfZSize / 2.f); //++

	//RIGHT FACE (yz)
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(leftU, 0.f);
	glVertex3f(halfXSize / 2.f, -halfYSize / 2.f, halfZSize / 2.f); //-+
	glTexCoord2f(0.f, 0.f);
	glVertex3f(halfXSize / 2.f, -halfYSize / 2.f, -halfZSize / 2.f); //--
	glTexCoord2f(0.f, leftV);
	glVertex3f(halfXSize / 2.f, halfYSize / 2.f, -halfZSize / 2.f); //+-
	glTexCoord2f(leftU, leftV);
	glVertex3f(halfXSize / 2.f, halfYSize / 2.f, halfZSize / 2.f); //++

	//LEFT FACE (yz)
	glNormal3f(-1.f, 0.f, 0.f);
	glTexCoord2f(leftU, 0.f);
	glVertex3f(-halfXSize / 2.f, -halfYSize / 2.f, halfZSize / 2.f); //-+
	glTexCoord2f(leftU, leftV);
	glVertex3f(-halfXSize / 2.f, halfYSize / 2.f, halfZSize / 2.f); //++
	glTexCoord2f(0.f, leftV);
	glVertex3f(-halfXSize / 2.f, halfYSize / 2.f, -halfZSize / 2.f); //+-
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-halfXSize / 2.f, -halfYSize / 2.f, -halfZSize / 2.f); //--

	glEnd();
}
#include "ReflectiveWater.h"

#include "Scene.h"

ReflectiveWater::ReflectiveWater(std::weak_ptr<ModelData> waterPlane, MaterialData materialData, std::weak_ptr<Camera> camera, GLfloat yOfPlane, GLfloat halfWidthAndHeight)
	: water(waterPlane),
	materialData(materialData)
{
	reflectionPlane = //setup reflection plane according to parameters
	{
		-halfWidthAndHeight, yOfPlane, halfWidthAndHeight,
		-halfWidthAndHeight, yOfPlane, -halfWidthAndHeight,
		halfWidthAndHeight,  yOfPlane, -halfWidthAndHeight,
		halfWidthAndHeight,  yOfPlane, halfWidthAndHeight
	};
	
	this->camera = camera;

	SetDrawOrder(UINT_MAX - 1000); //draw lastish
}


ReflectiveWater::~ReflectiveWater()
{}

void ReflectiveWater::Draw()
{
	/*Set material*/
	materialData.SetMaterial();

	if (camera.lock()->GetPosition().GetY() > 0.f)
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //Stop writing to frame buffer
		glEnable(GL_STENCIL_TEST); //Enable the stencil	test
		glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil always pass.
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //Set the stencil operation to replace values when it passes.

		glDisable(GL_DEPTH_TEST); //disable the depth test as it always 
	
		/*Draw Floor object (to reflect in?)*/
		glBegin(GL_QUADS);
		glVertex3f(reflectionPlane.at(0), reflectionPlane.at(1), reflectionPlane.at(2));
		glVertex3f(reflectionPlane.at(3), reflectionPlane.at(4), reflectionPlane.at(5));
		glVertex3f(reflectionPlane.at(6), reflectionPlane.at(7), reflectionPlane.at(8));
		glVertex3f(reflectionPlane.at(9), reflectionPlane.at(10), reflectionPlane.at(11));
		glEnd();

		glEnable(GL_DEPTH_TEST); //enable depth test
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //turn on rendering to frame buffer.
		glStencilFunc(GL_EQUAL, 1, 1); //set the stencil function to test if the valuye is 1
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //set the stencil to keep all values as we dont want to change the stencil

		glPushMatrix();
		//now flip the scale vertically (invert it on y)
		glScalef(1.f, -1.f, 1.f);
	
		//set light positions
		Scene::SetLightsPosition();

		for (auto& entity : objectsToReflect)
		{
			glPushMatrix();

			if (entity.lock()->GetDrawOrder() != 100) //NOT THE SHADOW MANAGER!
			{
				entity.lock()->Draw(); //now draw the entity on the reflected surface (as it will be drawn in the world space sometime later.
			}

			glPopMatrix();
		}


		glPopMatrix(); //go back to world space
	
		//now revert the position back so drawing can commence as normal.
		Scene::SetLightsPosition();
		glDisable(GL_STENCIL_TEST); //no longer needing the stencil.
	}

	glEnable(GL_BLEND); //enable alpha belnding to combine the floor object with model
	glDisable(GL_LIGHTING); //disable lighting to make a 100% reflective object
	glColor4f(0.6f, 0.6f, 1.f, 0.8f); //set the colour of the floor

	/*Now Draw the actual floor object model and stuff.*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, water.lock()->Vertices.data());

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, water.lock()->Normals.data());

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, water.lock()->UVs.data());

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glBindTexture(GL_TEXTURE_2D, water texture);

	glDrawArrays(water.lock()->Mode, 0, water.lock()->Vertices.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, NULL); //REMEMBER ORDER!
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

	/*Unset material*/
	materialData.UnsetMaterial();
}

void ReflectiveWater::AddObjectToReflect(std::weak_ptr<GameEntity> gameEntity)
{
	objectsToReflect.emplace_back(gameEntity);
}

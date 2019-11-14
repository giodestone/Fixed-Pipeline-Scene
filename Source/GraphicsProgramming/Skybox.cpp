#include "Skybox.h"

#include "Scene.h"

Skybox::Skybox() {}

Skybox::~Skybox() {}

void Skybox::Load(AssetManager & assetManager, std::weak_ptr<Camera> camera)
{
	skyboxTex = assetManager.GetTexture("skybox");
#ifdef _DEBUG
	assert(skyboxTex != NULL);
#endif // _DEBUG
	this->cameraRef = camera;
}

void Skybox::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, skyboxTex);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	if (Scene::IsWireframeEnabled())
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);

	glPushMatrix();
	{
		glTranslatef(cameraRef.lock()->GetPosition().GetX(), cameraRef.lock()->GetPosition().GetY(), cameraRef.lock()->GetPosition().GetZ());
		glDrawArrays(GL_QUADS, 0, 24);
	}
	glPopMatrix();

	Scene::SetWireframeOptions();

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

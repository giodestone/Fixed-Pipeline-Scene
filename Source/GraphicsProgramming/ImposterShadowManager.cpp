#include "ImposterShadowManager.h"

#include "Scene.h"

ImposterShadowManager::ImposterShadowManager(float yOfFloor, GLint imposterShadowTex)
	: imposterShadowTex(imposterShadowTex),
	yOfFloor(yOfFloor)
{
	SetDrawOrder(UINT_MAX - 5); //Draw last ish.
}


ImposterShadowManager::~ImposterShadowManager()
{}

void ImposterShadowManager::Draw()
{
	drawImposterShadows();
}

void ImposterShadowManager::drawImposterShadows()
{
	//disable lighting so stuff doesn't happen to imposter
	glDisable(GL_LIGHTING);
	//enable blending so stuff blends in with environment
	glEnable(GL_BLEND);
	//bind textures and enable tex
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, imposterShadowTex);
	for (auto& entity : entitiesToImposterShadow)
	{
		glPushMatrix();
		{
			glTranslatef(entity.first.lock()->GetPosition().GetX(), yOfFloor, entity.first.lock()->GetPosition().GetZ()); //goto position and above floor
			glScalef(entity.second.GetXSize() / 2.f, 0.f, entity.second.GetZSize() / 2.f);

			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.f, 0.f);
				glVertex3f(-1.f, 0.01f, 1.f);

				glTexCoord2f(1.f, 0.f);
				glVertex3f(1.f, 0.01f, 1.f);

				glTexCoord2f(1.f, 1.f);
				glVertex3f(1.f, 0.01f, -1.f);

				glTexCoord2f(0.f, 1.f);
				glVertex3f(-1.f, 0.01f, -1.f);
			}
			glEnd();
		}
		glPopMatrix();
	}
	//disable texture and unbind tex.
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
	//disable blending
	glDisable(GL_BLEND);
	//enable lighting
	glEnable(GL_LIGHTING);
}


void ImposterShadowManager::AddObjectToImposterShadow(std::weak_ptr<GameEntity> obj, SizeData sd)
{
	entitiesToImposterShadow.emplace_back(std::make_pair(obj, sd));
}
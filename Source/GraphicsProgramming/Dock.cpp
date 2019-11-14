#include "Dock.h"



Dock::Dock(GLint woodTexture)
{
	this->woodTexture = woodTexture;
}


Dock::~Dock()
{}

void Dock::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glPushMatrix();

	glTranslatef(GetRefToPosition().GetX() - 3.5f, GetRefToPosition().GetY(), GetRefToPosition().GetZ());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //repeat textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Draw Deck
	drawCuboid(4.f, 0.1f, 1.f,  4.f, 1.f,  10.f, 1.f,  3.f, 1.f);

		//Draw leg front left
		glPushMatrix();
			glTranslatef(-1.7f, -0.175f, -0.35f);
			drawCuboid(0.1f, 0.25f, 0.1f,  1.f, 1.f,  1.f, 2.f,  1.f, 2.f);
		glPopMatrix();

		//Draw leg front right
		glPushMatrix();
			glTranslatef(-1.7f, -0.175f, 0.35f);
			drawCuboid(0.1f, 0.25f, 0.1f,  1.f, 1.f,  1.f, 4.f,  1.f, 4.f);
		glPopMatrix();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
}

void Dock::drawCuboid(float halfXSize, float halfYSize, float halfZSize, float topU, float topV, float frontU, float frontV, float leftU, float leftV)
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

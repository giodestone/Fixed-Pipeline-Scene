#include "Tree.h"

Tree::Tree(std::weak_ptr<ModelData> trunk, float trunkHeight, std::weak_ptr<ModelData> canopy, float canopyHeight, MaterialData trunkMaterial, MaterialData canopyMaterial)
{
	this->trunk = trunk;
	this->trunkHeight = trunkHeight;
	this->canopy = canopy;
	this->canopyHeight = canopyHeight;
	this->trunkMaterial = trunkMaterial;
	this->canopyMaterial = canopyMaterial;
}

void Tree::Draw()
{
#ifdef _DEBUG
	assert(trunk.lock()->Vertices.size() > 0);
	assert(trunk.lock()->Normals.size() > 0);
	
	assert(canopy.lock()->Vertices.size() > 0);
	assert(canopy.lock()->Normals.size() > 0);
#endif

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/*Draw Trunk*/
	trunkMaterial.SetMaterial();

	glPushMatrix();

	glTranslatef(GetRefToPosition().GetX(), GetRefToPosition().GetY() + trunkHeight / 2.f + 0.125f, GetRefToPosition().GetZ());

	glVertexPointer(3, GL_FLOAT, 0, trunk.lock()->Vertices.data());
	glNormalPointer(GL_FLOAT, 0, trunk.lock()->Normals.data());

	glDrawArrays(trunk.lock()->Mode, 0, trunk.lock()->Vertices.size() / 3);

	glPopMatrix();

//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	trunkMaterial.UnsetMaterial();

	/*Draw Canopy*/
	canopyMaterial.SetMaterial();

	glPushMatrix();

	glTranslatef(GetRefToPosition().GetX(), GetRefToPosition().GetY() + canopyHeight / 2.f + trunkHeight, GetRefToPosition().GetZ());

	glVertexPointer(3, GL_FLOAT, 0, canopy.lock()->Vertices.data());
	glNormalPointer(GL_FLOAT, 0, canopy.lock()->Normals.data());

	glDrawArrays(canopy.lock()->Mode, 0, canopy.lock()->Vertices.size() / 3);

	glPopMatrix();

	canopyMaterial.UnsetMaterial();

//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

SizeData Tree::GetSizeData()
{
	return canopy.lock()->SizeData;
}

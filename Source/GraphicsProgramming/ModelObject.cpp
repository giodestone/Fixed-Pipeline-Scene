#include "ModelObject.h"

ModelObject::ModelObject(std::weak_ptr<ModelData> md, MaterialData materialData, std::string aliasOfTexture)
{
	SetModelData(md);
	SetMaterialData(materialData);
	this->aliasOfTexture = aliasOfTexture;
}

void ModelObject::Load(AssetManager & assetManager)
{
	if (!aliasOfTexture.empty()) //only load if name has been specfied
		this->texture = assetManager.GetTexture(aliasOfTexture);

#ifdef _DEBUG
	if (!aliasOfTexture.empty())
		assert(this->texture != NULL);
#endif // _DEBUG
}

void ModelObject::Draw()
{
	/*Set material*/
	materialData.SetMaterial();

	glPushMatrix();
	
	/*Move to position.*/
	glTranslatef(GetRefToPosition().GetX(), GetRefToPosition().GetY(), GetRefToPosition().GetZ());

	/*Rotate accordingly.*/
	if (GetRefToRotation().GetX() != 0.f)
		glRotatef(GetRefToRotation().GetX(), 1.f, 0.f, 0.f);
	if (GetRefToRotation().GetY() != 0.f)
		glRotatef(GetRefToRotation().GetY(), 0.f, 1.f, 0.f);
	if (GetRefToRotation().GetZ() != 0.f)
		glRotatef(GetRefToRotation().GetZ(), 0.f, 0.f, 1.f);
	
	/*Scale Accordingly*/
	glScalef(GetRefToScale().GetX(), GetRefToScale().GetY(), GetRefToScale().GetZ());

	/*Enable verticies and give pointer.*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, modelData.lock()->Vertices.data());

	/*Enable normals and give pointer.*/
	if (!modelData.lock()->Normals.empty()) 
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, modelData.lock()->Normals.data());
	}
	
	/*Enable textures and give pointer.*/
	if (!modelData.lock()->UVs.empty())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, modelData.lock()->UVs.data());
	}

	/*Enable texturing if there is a texture set*/
	if (texture != NULL && IsTexturingEnabled())
	{
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
		
	/*Draw using Indicies if there are any.*/
	if (modelData.lock()->Indices.empty())
	{
		glDrawArrays(modelData.lock()->Mode, 0, modelData.lock()->Vertices.size() / 3);
	}
	/*Otherwise just draw normally.*/
	else
	{
		glDrawElements(modelData.lock()->Mode, modelData.lock()->Indices.size(), GL_UNSIGNED_BYTE, modelData.lock()->Indices.data());
	}

	/*Unset the texture if there is any.*/
	if (texture != NULL && IsTexturingEnabled())
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);
	}

	/*Disable Texture array.*/
	if (!modelData.lock()->UVs.empty())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	/*Disable Normals array.*/
	if (!modelData.lock()->Normals.empty())
		glDisableClientState(GL_NORMAL_ARRAY);
	
	/*Disable Vertex array.*/
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	/*Unset material*/
	materialData.UnsetMaterial();
}

void ModelObject::SetModelData(std::weak_ptr<ModelData> md)
{
#ifdef _DEBUG
	assert(md.lock()->Mode != NULL);
#endif // _DEBUG
	this->modelData = md;
}

void ModelObject::SetMaterialData(MaterialData materialData)
{
	this->materialData = materialData;
}

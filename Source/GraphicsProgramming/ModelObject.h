#pragma once
#include "GameEntity.h"

#include "ModelData.h"
#include "MaterialData.h"

class ModelObject : public GameEntity
{
	std::weak_ptr<ModelData> modelData;
	MaterialData materialData;
	
	std::string aliasOfTexture = "";
	GLint texture = NULL;
	bool texturingEnabled = true;
public:
	ModelObject() = default;
	/// <summary>
	/// Creates a model object which has ModelData.
	/// </summary>
	/// <param name="md">ModelData it should hold.</param>
	/// <param name="aliasOfTexture">Alias of the texture as passed into AssetManager.</param>
	/// <param name="enableLighting">Whether the object should be lit.</param>
	/// <param name="lightsToEnable">By what lights should it be lit (GL_LIGHT0 etc.).</param>
	ModelObject(std::weak_ptr<ModelData> md, MaterialData materialData, std::string aliasOfTexture = "");
	~ModelObject() = default;

	virtual void Load(AssetManager& assetManager) override;
	
	/// <summary>
	/// This draws the static object. If no uv's or normals get passed.
	/// </summary>
	virtual void Draw() override;
	
	virtual void SetModelData(std::weak_ptr<ModelData> md);

	virtual void SetMaterialData(MaterialData materialData);
};
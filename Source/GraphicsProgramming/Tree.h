#pragma once

#include "GameEntity.h"
#include "MaterialData.h"

class Tree : public GameEntity
{
private:
	std::weak_ptr<ModelData> trunk;
	float trunkHeight;
	std::weak_ptr<ModelData> canopy;
	float canopyHeight;

	MaterialData trunkMaterial;
	MaterialData canopyMaterial;
public:
	/// <summary>
	/// Creates a tree.
	/// </summary>
	/// <param name="trunk"></param>
	/// <param name="trunkHeight"></param>
	/// <param name="canopy"></param>
	/// <param name="canopyHeight"></param>
	/// <param name="trunkMaterial"></param>
	/// <param name="canopyMaterial"></param>
	/// <param name="lights">The lights that should light up the tree.</param>
	Tree(std::weak_ptr<ModelData> trunk, float trunkHeight, std::weak_ptr<ModelData> canopy, float canopyHeight, MaterialData trunkMaterial, MaterialData canopyMaterial);
	~Tree() = default;

	/// <summary>
	/// Draws the tree at the specified position.
	/// </summary>
	void Draw() override;

	SizeData GetSizeData();
};
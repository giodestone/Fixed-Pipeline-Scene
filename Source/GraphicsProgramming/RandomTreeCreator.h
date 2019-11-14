#pragma once

#include "Tree.h"

class RandomTreeCreator
{
	const MaterialData DefaultCanopyColour = { { 0.f, 0.5f, 0.f, 1.f }, { 0.f, 0.5f, 0.f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, 0.00001f };
	const MaterialData DefaultTrunkColour = {{ 0.4f, 0.26f, 0.14f, 1.f }, { 0.4f, 0.26f, 0.14f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, 0.00001f};

	std::vector<std::pair<float, std::weak_ptr<ModelData>>> trunks;
	std::vector<std::pair<float, std::weak_ptr<ModelData>>> canopies;
public:
	RandomTreeCreator()
	{}

	~RandomTreeCreator()
	{}

	/// <summary>
	/// Add a trunk that can be used to make up a tree.
	/// </summary>
	/// <param name="trunkHeight"></param>
	/// <param name="trunkData"></param>
	void AddTrunk(float trunkHeight, std::weak_ptr<ModelData> trunkData);
	
	/// <summary>
	/// Add a canopy that can be used to make up a tree.
	/// </summary>
	/// <param name="canopyHeight"></param>
	/// <param name="canopyData"></param>
	void AddCanopy(float canopyHeight, std::weak_ptr<ModelData> canopyData);

	/// <summary>
	/// Returns a tree that has a random canopy and tree.
	/// </summary>
	/// <param name="lightsToEnable">What lights should each tree have enabled on it.</param>
	/// <returns>A tree.</returns>
	std::shared_ptr<Tree> GetRandomTree();

private:
	/// <summary>
	/// Gets a random trunk.
	/// </summary>
	/// <returns>Height and ModelData.</returns>
	std::pair<float, std::weak_ptr<ModelData>> getRandomTrunk();

	/// <summary>
	/// Gets a random canopy.
	/// </summary>
	/// <returns>Height and ModelData.</returns>
	std::pair<float, std::weak_ptr<ModelData>> getRandomCanopy();
};

#include "RandomTreeCreator.h"

void RandomTreeCreator::AddTrunk(float trunkHeight, std::weak_ptr<ModelData> trunkData)
{
	trunks.push_back(std::make_pair(trunkHeight, trunkData));
}

void RandomTreeCreator::AddCanopy(float canopyHeight, std::weak_ptr<ModelData> canopyData)
{
	canopies.push_back(std::make_pair(canopyHeight, canopyData));
}

std::shared_ptr<Tree> RandomTreeCreator::GetRandomTree()
{
	auto trunk = getRandomTrunk();
	auto canopy = getRandomCanopy();

	std::shared_ptr<Tree> tree = std::make_shared<Tree>(trunk.second, trunk.first, canopy.second, canopy.first, DefaultTrunkColour, DefaultCanopyColour);

	return tree;
}

std::pair<float, std::weak_ptr<ModelData>> RandomTreeCreator::getRandomTrunk()
{
#ifdef _DEBUG
	assert(trunks.size() > 0);
#endif
	return trunks.at(rand() % trunks.size());
}

std::pair<float, std::weak_ptr<ModelData>> RandomTreeCreator::getRandomCanopy()
{
#ifdef _DEBUG
	assert(canopies.size() > 0);
#endif
	return canopies.at(rand() % canopies.size());
}

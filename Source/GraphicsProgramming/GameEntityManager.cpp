#include "GameEntityManager.h"



GameEntityManager::GameEntityManager(AssetManager * assetManager, std::vector<int>* lightsToEnable)
	: assetManager(assetManager),
	lightsToEnable(lightsToEnable)
{
	this->assetManager = assetManager;
}


GameEntityManager::~GameEntityManager()
{
}

void GameEntityManager::ProcessInput(Input & input, float dt)
{
	//Run delete cycle before anything else gets done.
	if (toDeleteEntity)
	{
		gameEntities.remove_if([](std::shared_ptr<GameEntity>gameEntity) { return gameEntity->GetToBeDeleted(); }); //Should always return a bool
	}

	if (entityAdded)
	{
		gameEntities.sort(GameEntity::SortGameEntities);
		entityAdded = false;
	}

	for (auto& entity : gameEntities)
	{
		entity->ProcessInput(input, dt);
	}
}

void GameEntityManager::Update(float dt)
{
	for (auto& entity : gameEntities)
	{
		entity->Update(dt);
	}
}

void GameEntityManager::Draw()
{
	/*Enable lighting and lights.*/
	glEnable(GL_LIGHTING);
	for (size_t i = 0; i < lightsToEnable->size(); i++)
	{
		glEnable(lightsToEnable->at(i));
	}

	/*Draw entities*/
	for (auto& entity : gameEntities)
	{
		entity->Draw();
	}

	/*Disable lights and lighting.*/
	for (size_t i = 0; i < lightsToEnable->size(); i++)
	{
		glDisable(lightsToEnable->at(i));
	}
	glDisable(GL_LIGHTING);
}

void GameEntityManager::AddEntity(std::shared_ptr<GameEntity> entity)
{
#ifdef _DEBUG
	assert(entity != nullptr);
#endif // _DEBUG

	if (!entity->IsLoaded()) //load if not loaded already
	{
#ifdef _DEBUG
		assert(entity->isLoaded == false);
#endif // _DEBUG
		entity->Load(*assetManager); //Because the scenes already been loaded so better get it done and over with
		entity->isLoaded = true;
	}

	gameEntities.push_front(std::move(entity)); //the std::move inside of the add entity transfers ownership of the entity to the vector. This should work.

	entityAdded = true;
}

void GameEntityManager::RemoveEntity(std::shared_ptr<GameEntity> entity)
{
#ifdef _DEBUG
	assert(entity != nullptr);
#endif // _DEBUG

	entity->SetToBeDeleted(true);
	toDeleteEntity = true;
}

void GameEntityManager::RemoveEntity(std::weak_ptr<GameEntity> entity)
{
#ifdef _DEBUG
	assert(!entity.expired());
#endif // _DEBUG
	entity.lock()->SetToBeDeleted(true);
	std::static_pointer_cast<GameEntity>(entity.lock())->SetToBeDeleted(true);
	toDeleteEntity = true;
}





#pragma once
#ifdef _DEBUG
#include <assert.h>
#endif // _DEBUG

#include <memory>
#include <list>

#include "GameEntity.h"
#include "AssetManager.h"

/// <summary>
/// This manages all game entity manager classes.
/// </summary>
/// <remarks>Needs the Scene class to be friends with this.</remarks>
class GameEntityManager
{
	friend class Scene; //so not every entity meddles with the updating of all the methods
	AssetManager* assetManager;
	std::vector<int>* lightsToEnable;

public:
	GameEntityManager(AssetManager * assetManager, std::vector<int>* lightsToEnable);
	~GameEntityManager();

private:
	std::list<std::shared_ptr<GameEntity>> gameEntities; //For storing the game objects - linked list because performance and ease of inserting and deleting.
	bool entityAdded = false; //for sorting of sprites
	bool toDeleteEntity = false; //for determing if the whole vector has to be looped through when something is deleted

	/// <summary>
	/// Processes input for all Game Entities.
	/// </summary>
	/// <param name="input">Reference to input.</param>
	/// <param name="dt"></param>
	void ProcessInput(Input& input, float dt);

	/// <summary>
	/// Updates all Game Entities.
	/// </summary>
	/// <param name="dt"></param>
	void Update(float dt);

	/// <summary>
	/// Draws all Game Entities.
	/// </summary>
	void Draw();

public:
	/// <summary>
	/// Add an entity.
	/// </summary>
	/// <param name="entity">Entity to add.</param>
	void AddEntity(std::shared_ptr<GameEntity> entity);

	/// <summary>
	/// Remove an entity.
	/// </summary>
	/// <param name="entity">Entity to remove</param>
	void RemoveEntity(std::shared_ptr<GameEntity> entity);
	void RemoveEntity(std::weak_ptr<GameEntity> entity);
};


#include "GameEntity.h"



GameEntity::GameEntity()
{
}


GameEntity::~GameEntity()
{
}

Vector3 GameEntity::GetPosition() const
{
	return this->position;
}

Vector3 & GameEntity::GetRefToPosition()
{
	return position;
}

void GameEntity::SetPosition(Vector3 & pos)
{
	this->position = pos;
}

void GameEntity::SetPosition(const Vector3 pos)
{
	this->position = pos;
}

Vector3 GameEntity::GetRotation() const
{
	return rotation;
}

Vector3 & GameEntity::GetRefToRotation()
{
	return *(&rotation);
}

void GameEntity::SetRotation(Vector3 rotation)
{
	this->rotation = rotation;
}

void GameEntity::AddRotation(Vector3 rotation)
{
	this->rotation += rotation;

	///TODO: clamp?
}

void GameEntity::SetScale(const Vector3 scale)
{
	this->scale = scale;
}

Vector3 GameEntity::GetScale() const
{
	return this->scale;
}

Vector3 & GameEntity::GetRefToScale()
{
	return scale;
}

bool GameEntity::IsLoaded()
{
	return isLoaded;
}

bool GameEntity::GetToBeDeleted()
{
	return toBeDeleted;
}

void GameEntity::SetToBeDeleted(bool toBeDeleted)
{
	this->toBeDeleted = toBeDeleted;
}

unsigned int GameEntity::GetDrawOrder() const
{
	return drawOrder;
}

void GameEntity::SetDrawOrder(unsigned int drawOrder)
{
	this->drawOrder = drawOrder;
}

bool GameEntity::IsTexturingEnabled() const
{
	return isTextruingEnabled;
}

void GameEntity::Load(AssetManager& assetManager)
{
}

void GameEntity::ProcessInput(Input & input, float dt)
{
}

void GameEntity::Update(float dt)
{
}

void GameEntity::Draw()
{
}

void GameEntity::EnableTexturing()
{
	isTextruingEnabled = true;
}

void GameEntity::DisableTexturing()
{
	isTextruingEnabled = false;
}

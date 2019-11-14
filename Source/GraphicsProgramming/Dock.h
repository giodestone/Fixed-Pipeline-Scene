#pragma once
#include "GameEntity.h"
class Dock :
	public GameEntity
{
	GLint woodTexture;
public:
	Dock(GLint woodTexture);
	~Dock();

	void Draw() override;

private:
	void drawCuboid(float halfXSize, float halfYSize, float halfZSize, float topU = 1.f, float topV = 1.f, float frontU = 1.f, float frontV = 1.f, float leftU = 1.f, float leftV = 1.f);
};


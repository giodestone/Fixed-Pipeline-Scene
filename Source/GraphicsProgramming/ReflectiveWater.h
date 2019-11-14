#pragma once
#include <list>
#include "GameEntity.h"
#include "MaterialData.h"
#include "Camera.h"
class ReflectiveWater :
	public GameEntity
{
	MaterialData materialData;

	std::weak_ptr<ModelData> water;
	std::weak_ptr<Camera> camera;

	std::list<std::weak_ptr<GameEntity>> objectsToReflect;
	std::array<GLfloat, 4 * 3> reflectionPlane = 
	{ 
		-20.f, 0.f, 20.f,
		-20.f, 0.f, -20.f,
		20.f, 0.f, -20.f,
		20.f, 0.f, 20.f
	}; //Coordinates of the reflection plane. What are the coordinates of the reflected plane.

public:
	/// <summary>
	/// Creates an object which reflects other objects by drawing them again.
	/// </summary>
	/// <param name="waterPlane">The model of what the water is supposed to be.</param>
	/// <param name="materialData">'Material' of the water.</param>
	/// <param name="camera">Pointer to camera.</param>
	/// <param name="yOfPlane">How high up the water should be reflected in.</param>
	/// <param name="halfWidthAndHeight">Half width and height of that plane.</param>
	ReflectiveWater(std::weak_ptr<ModelData> waterPlane, MaterialData materialData, std::weak_ptr<Camera> camera, GLfloat yOfPlane=0.f, GLfloat halfWidthAndHeight=20.f);
	~ReflectiveWater();

	/// <summary>
	/// Draws the plane and reflected objects.
	/// </summary>
	void Draw() override;

	void AddObjectToReflect(std::weak_ptr<GameEntity> gameEntity);
};


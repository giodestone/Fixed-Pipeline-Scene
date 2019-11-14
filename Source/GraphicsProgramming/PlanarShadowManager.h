#pragma once
#include "GameEntity.h"
class PlanarShadowManager :
	public GameEntity
{
	std::list<std::weak_ptr<GameEntity>> entitiesToPlanarShadow;
	std::array<float, 4 * 3> floorPlane;
public:
	PlanarShadowManager(float yOfFloor);
	~PlanarShadowManager();

	void Draw() override;

	/// <summary>
    /// Add an object that will be planar shadowed.
    /// </summary>
    /// <param name="object">Object.</param>
	void AddObjectToPlanarShadow(std::weak_ptr<GameEntity> object);

private:
	/// <summary>
	/// Draws planar shadows for everything in the entitiesToPlanarShadows.
	/// </summary>
	void drawPlanarShadows();

	/*=====For Shadow Calculations=====*/
	void generateShadowMatrix(float* shadowMatrix, float light_pos[4], GLfloat floor[12]);

	void extendVertex(float newVert[3], float lightPosit[4], float x, float y, float z, float ext);

	std::vector<float> buildShadowVolume(float lightPosit[4], std::vector<float> verts);
};


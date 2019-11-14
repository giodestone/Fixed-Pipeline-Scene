#pragma once
#include "GameEntity.h"
class ImposterShadowManager :
	public GameEntity
{
	std::list<std::pair<std::weak_ptr<GameEntity>, SizeData>> entitiesToImposterShadow;
	float yOfFloor = 0.f;

	GLint imposterShadowTex;

public:
	//ShadowManager(std::vector<int>* activeLights);
	ImposterShadowManager(float yOfFloor, GLint imposterShadowTex);
	~ImposterShadowManager();

	void Draw() override;

	/// <summary>
	/// Add an object that will have an imposter placed underneath. This is assuming that the position is in the middle.
	/// </summary>
	/// <param name="obj">Object.</param>
	/// <param name="sd">Size data for the object.</param>
	void AddObjectToImposterShadow(std::weak_ptr<GameEntity> obj, SizeData sd);

private:
	/// <summary>
	/// Draws imposter shadows depending on size for everything in the entitiesToImposterShadow
	/// </summary>
	void drawImposterShadows();
	
	
};


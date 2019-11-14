#pragma once
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SOIL.h"


#include <assert.h>
#include <memory>
#include <unordered_map>

#include "ModelData.h"

class AssetManager
{
	std::unordered_map<std::string, GLint> textures;
	std::unordered_map<std::string, std::shared_ptr<ModelData>> models;
public:
	AssetManager();
	~AssetManager();

	/// <summary>
	/// Load textures from the paths provided.
	/// </summary>
	/// <param name="paths">First: alais, Second: path</param>
	/// <returns>False if load has failed.</returns>
	bool LoadTextures(std::vector<std::pair<std::string, std::string>> paths);

	/// <summary>
	/// Loads the model with the specified path.
	/// </summary>
	/// <param name="alias">Alias of the item that will be loaded.</param>
	/// <param name="path">Path of the item that will load.</param>
	/// <param name="invertYTex">Whether the Y of the V coordniate should be inverted (for compatiability).</param>
	/// <returns>Whether loading was succesful</returns>
	bool LoadModel(std::string alias, std::string path, bool invertYTex=false);

	/// <summary>
	/// Places the model data onto the internal store.
	/// </summary>
	/// <param name="mdpaths">A pair which contains the alias and the pointer to the object.</param>
	/// <returns>Whether loading has succeded.</returns>
	void AddModelData(std::string alias, std::shared_ptr<ModelData> modelData);
	
	/// <summary>
	/// Returns a texture with the afformentioned name. Same as file name.
	/// </summary>
	/// <param name="name">File name of the texture.</param>
	/// <returns>A pointer to the texture.</returns>
	GLint GetTexture(std::string name);

	std::weak_ptr<ModelData> GetModel(std::string name);
};
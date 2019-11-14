#pragma once

#include <vector>
#include <array>
#include <memory>

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Vector3.h"

#ifdef _DEBUG
#include <assert.h>
#endif

/// <summary>
/// Holds data about an objects size;
/// </summary>
struct SizeData
{
private:
	float xSize = 0.f;
	float ySize = 0.f;
	float zSize = 0.f;
public:
	float GetXSize() const { return xSize; }
	float GetYSize() const { return ySize; }
	float GetZSize() const { return zSize; }

	void SetXSize(float xSize) { this->xSize = xSize; }
	void SetYSize(float ySize) { this->ySize = ySize; }
	void SetZSize(float zSize) { this->zSize = zSize; }
	SizeData(float xSize=0.f, float ySize=0.f, float zSize=0.f) : xSize(xSize), ySize(ySize), zSize(zSize) {}
	~SizeData() {}
	
};

/// <summary>
/// Stores the type of data e.g. GL_QUADS, GL_TRIANGLES, vertices, uvs, normals and indices.
/// </summary>
struct ModelData
{
	GLubyte Mode = NULL; //Type of data e.g. GL_QUADS, GL_TRIANGLES etc. by default triangles so atleaat something draws

	SizeData SizeData;

	std::vector<GLfloat> Vertices;
	std::vector<GLfloat> Normals;
	std::vector<GLfloat> UVs;
	std::vector<unsigned int> Indices;

	ModelData() = default;
	~ModelData() = default;

	static constexpr float UVfix = 0.01f; //This fixes the black lines on the textures, needs to be added to the max u and v values.

	/// <summary>
	/// Pushed a vertex Vector3 to the internal sotre of vertices. Pushing the wrong value without appending others could make the draw fail.
	/// </summary>
	/// <param name="vector"></param>
	void PushVertexVector(const Vector3& vector);

	/// <summary>
	/// Pushes a normal Vector3 to the internal store of normals. Pushing the wrong value without appending others could make the draw fail.
	/// </summary>
	/// <param name="vector"></param>
	void PushNormalVector(const Vector3& vector);

	/// <summary>
	/// Pushes a pair of UV values. Pushing the wrong value without appending others could make the draw fail.
	/// </summary>
	/// <param name="uvX"></param>
	/// <param name="uvY"></param>
	void PushUVPair(const float uvX, const float uvY);

	/// <summary>
	/// Pushes a literal UV pair into the model data.
	/// </summary>
	/// <param name="uvs">First should be x, and Second should be y.</param>
	void PushUVPair(std::pair<GLfloat, GLfloat> uvs);

	/// <summary>
	/// Returns a pointer to model data containing a disc with relevant normals and UV's. Should be passed into AssetManager.
	/// </summary>
	/// <param name="radius">Radius of the disc.</param>
	/// <param name="divisions">Amount of divisions the disc should have (3 min).</param>
	/// <param name="normal">The normal of the disc.</param>
	/// <param name="height">The height at which the disc should be created.</param>
	/// <returns>Pointer with ModelData containing a disc.</returns>
	static std::shared_ptr<ModelData> GetDisc(const float radius = 1.f, const unsigned int divisions = 3, const Vector3 normal = { 0, 1, 0 }, const float height = 0.f);

	/// <summary>
	/// Returns a pointer to model data containing a sphere with the relevant normals and UV's. Should be passed into AssetManager.
	/// </summary>
	/// <param name="radius">Radius of the sphere</param>
	/// <param name="segments">How many 'divisions' should the sphere have.</param>
	/// <returns>Pointer with ModelData containing a sphere.</returns>
	static std::shared_ptr<ModelData> GetSphere(const float radius, const int segments);

	/// <summary>
	/// Returns a pointer to model data containing a cylinder with the relevant normals and UV's. Should be passed into AssetManager.
	/// </summary>
	/// <param name="radius">Radius ofhte cylinder.</param>
	/// <param name="segments">How many segments should the cylinder circle should have.</param>
	/// <param name="height">How tall should the cylinder be.</param>
	/// <param name="heightDivisions">The how many divisions should the cylinder have</param>
	/// <returns>Pointer with ModelData containing a cylinder.</returns>
	static std::shared_ptr<ModelData> GetCyclinder(const float radius, const int divisions, const float height, const int heightDivsions, const bool addBottomDisc=true, const bool addTopDisc=true);

	/// <summary>
	/// Returns a pointer to a model data containing a cone with relevant normals and UV's. Should be passed into AssetManager.
	/// </summary>
	/// <param name="radius">Radius of the cone.</param>
	/// <param name="divisions">Divisons of the radius (how round it will be).</param>
	/// <param name="height">Height of the cone.</param>
	/// <param name="heightDivisions">How many divisions between the top and bottom of the cone.</param>
	/// <param name="drawBottomFace">Whether the bottom face should be drawn.</param>
	/// <returns>Pointer with the ModelData containg a cone.</returns>
	static std::shared_ptr<ModelData> GetCone(const float radius, const int divisions, const float height, const float heightDivisions, bool drawBottomFace=true);

	/// <summary>
	/// Returns a pointer to a model data containg a quad which is situated on the XY plane. Should be passed into AssetManager.
	/// </summary>
	/// <param name="xSize">2 * x size of the quad.</param>
	/// <param name="ySize">2 * y size of the quad.</param>
	/// <param name="normal">Normal component, by default facing positive z.</param>
	/// <returns>Pointer with the ModelData containg a quad.</returns>
	static std::shared_ptr<ModelData> GetXYQuad(const float xSize, const float ySize, const Vector3 normal = { 0.f, 0.f, 1.f });
private:
	static constexpr float pi = 3.14159265359f;
};
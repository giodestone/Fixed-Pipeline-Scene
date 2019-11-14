#pragma once
#include "Camera.h"

/// <summary>
/// This class is for the skybox
/// </summary>
class Skybox
{
	static constexpr GLfloat normals[] = 
	{ 
		0.0, 1.0, 0.0, //top
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, -1.0, 0.0, //bottom
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, 0.0, 1.0, //front //0
		0.0, 0.0, 1.0,		//1
		0.0, 0.0, 1.0,		//2
		0.0, 0.0, 1.0,		//3
		0.0, 0.0, -1.0, //back //0
		0.0, 0.0, -1.0,		//1
		0.0, 0.0, -1.0,		//2
		0.0, 0.0, -1.0,		//3
		-1.0, 0.0, 0.0, //left
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, //right
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0
	};

	static constexpr float UVfix = 0.0003f;

	static constexpr GLfloat uvs[] = ///FIXME: back face appears weird.
	{
		0.5f - UVfix, 0.f + UVfix, //top //1
		0.25f + UVfix, 0.f + UVfix, //2
		0.25f + UVfix, 0.25f, //3
		0.5f - UVfix, 0.25f, //4

		0.5f - UVfix, 0.5f - UVfix, //bottom //1
		0.25f + UVfix, 0.5f - UVfix, //2
		0.25f + UVfix, 0.75f - UVfix, //3
		0.5f - UVfix, 0.75f - UVfix, //4

		0.25f, 0.25f, //1
		0.25f, 0.5f - UVfix, //2
		0.5f, 0.5f - UVfix, //3
		0.5f, 0.25f, //front //4

		0.75f, 0.5f - UVfix, //1
		1.f - UVfix, 0.5f - UVfix, //2
		1.f - UVfix, 0.25f + UVfix, //back //3
		0.75f, 0.25f + UVfix, //4

		0.25f, 0.25f + UVfix, //left //1
		0.f + UVfix, 0.25f + UVfix, //2
		0.f + UVfix, 0.5f - UVfix, //3
		0.25f, 0.5f - UVfix, //4

		0.75f, 0.25f + UVfix, //right //1
		0.5f, 0.25f + UVfix, //2
		0.5f, 0.5f - UVfix, //3
		0.75f, 0.5f - UVfix, //4
	};

	static constexpr GLfloat vertices[] =
	{
		1.0f, 1.0f, -1.0f, //top //1
		-1.0f, 1.0f, -1.0f, //2
		-1.0f, 1.0f,  1.0f, //3
		1.0f, 1.0f,  1.0f, //4

		1.0f, -1.0f,  1.0f, //bottom //1
		-1.0f, -1.0f,  1.0f, //2
		-1.0f, -1.0f, -1.0f, //3
		1.0f, -1.0f, -1.0f, //4

		-1.0, 1.0, 1.0,		//1	// Vertex #0 front
		-1.0, -1.0, 1.0,	//2	// Vertex #1
		1.0,  -1.0, 1.0,	//3	// Vertex #2
		1.0,  1.0, 1.0,		//4	// Vertex #3

		1.0f, -1.0f, -1.0f, //back //1
		-1.0f, -1.0f, -1.0f, //2
		-1.0f,  1.0f, -1.0f, //3
		1.0f,  1.0f, -1.0f, //4

		-1.0f,  1.0f,  1.0f, //left //1
		-1.0f,  1.0f, -1.0f, //2
		-1.0f, -1.0f, -1.0f, //3
		-1.0f, -1.0f,  1.0f, //4

		1.0f,  1.0f, -1.0f, //right //1
		1.0f,  1.0f,  1.0f, //2
		1.0f, -1.0f,  1.0f,//3
		1.0f, -1.0f, -1.0f,//4
	};

	std::weak_ptr<Camera> cameraRef;
	GLint skyboxTex;

public:
	/// <summary>
	/// Skybox.
	/// </summary>
	/// <param name="camera">Reference to the camera.</param>
	Skybox();
	~Skybox();

	/// <summary>
	/// Load the texture and set the reference to camera. This should be done after creating the camera object and loading texture.
	/// </summary>
	/// <param name="assetManager">Reference to AssetManager.</param>
	/// <param name="camera">Reference to Camera.</param>
	void Load(AssetManager& assetManager, std::weak_ptr<Camera> camera);

	/// <summary>
	/// Draws the skybox. Should always be drawn before the camera.
	/// </summary>
	void Draw();
};
#include "AssetManager.h"

#include "Vector3.h"
#include <vector>

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{}

bool AssetManager::LoadTextures(std::vector<std::pair<std::string, std::string>> paths)
{
	for (auto path : paths)
	{
		GLint ptr;
		ptr	= SOIL_load_OGL_texture
		(
			path.second.data(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
#ifdef _DEBUG
		auto debugMessage = SOIL_last_result();
		assert(ptr != NULL); //retrns 1 for some reason
#endif // _DEBUG
		if (ptr == NULL)
			return false;

		textures.insert(std::make_pair(path.first, ptr));
	}
	return true;
}

bool AssetManager::LoadModel(std::string alias, std::string path, bool invertYTex)
{
	std::shared_ptr<ModelData> tempMD = std::make_shared<ModelData>();
	tempMD->Mode = GL_TRIANGLES;

	int m_vertexCount = 0;
	std::vector<Vector3> verts;
	std::vector<Vector3> norms;
	std::vector<Vector3> texCs;
	std::vector<unsigned int> faces;

	FILE* file;
	auto errorNo = fopen_s(&file, path.data(), "r");
	if (errorNo != 0)
	{
		return false;
	}
	while (true)
	{
		char lineHeader[128];

		// Read first word of the line
		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF)
		{
			break; // exit loop
		}
		else // Parse
		{
			if (strcmp(lineHeader, "v") == 0) // Vertex
			{
				Vector3 vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				verts.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				Vector3 uv;
				fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
				texCs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				norms.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[9];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &face[0], &face[1], &face[2],
																			&face[3], &face[4], &face[5],
																			&face[6], &face[7], &face[8]);
				if (matches != 9)
				{
					// Parser error, or not triangle faces
					return false;
				}

				for (int i = 0; i < 9; i++)
				{
					faces.push_back(face[i]);
				}
			}
		}
	}

	// "Unroll" the loaded obj information into a list of render ready triangles.
	for (size_t i = 0; i < faces.size(); i += 3)
	{
		Vector3 vertex1;
		Vector3 uv1;
		Vector3 normal1;

		vertex1 = verts.at(faces.at(i) - 1);
		uv1 = texCs.at(faces.at(i + 1) - 1);
		normal1 = norms.at(faces.at(i + 2) - 1);

		tempMD->Vertices.push_back(vertex1.GetX());
		tempMD->Vertices.push_back(vertex1.GetY());
		tempMD->Vertices.push_back(vertex1.GetZ());

		tempMD->UVs.push_back(uv1.GetX());
		if (invertYTex)
			tempMD->UVs.push_back(1.f - uv1.GetY());
		else
			tempMD->UVs.push_back(uv1.GetY());

		tempMD->Normals.push_back(normal1.GetX());
		tempMD->Normals.push_back(normal1.GetY());
		tempMD->Normals.push_back(normal1.GetZ());

		m_vertexCount++;
	}

	AddModelData(alias, tempMD);

	verts.clear();
	norms.clear();
	texCs.clear();
	faces.clear();

	return true;
}

void AssetManager::AddModelData(std::string alias, std::shared_ptr<ModelData> modelData)
{
	modelData->Vertices.shrink_to_fit();
	modelData->Normals.shrink_to_fit();
	modelData->UVs.shrink_to_fit();
	modelData->Indices.shrink_to_fit();

	models.insert(std::make_pair(alias, modelData));
}

GLint AssetManager::GetTexture(std::string name)
{
	return textures.at(name);
}

std::weak_ptr<ModelData> AssetManager::GetModel(std::string name)
{
	return models.at(name); //automatically casts into weak pointer
	/*If you have an error its probably because the model you have specified doesn't exist.*/
}

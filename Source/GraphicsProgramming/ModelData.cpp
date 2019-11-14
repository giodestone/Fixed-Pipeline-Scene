#include "ModelData.h"

void ModelData::PushVertexVector(const Vector3 & vector)
{
	Vertices.push_back(static_cast<GLfloat>(vector.GetX()));
	Vertices.push_back(static_cast<GLfloat>(vector.GetY()));
	Vertices.push_back(static_cast<GLfloat>(vector.GetZ()));
}

void ModelData::PushNormalVector(const Vector3 & vector)
{
	Normals.push_back(static_cast<GLfloat>(vector.GetX()));
	Normals.push_back(static_cast<GLfloat>(vector.GetY()));
	Normals.push_back(static_cast<GLfloat>(vector.GetZ()));
}

void ModelData::PushUVPair(const float uvX, const float uvY)
{
	UVs.push_back(uvX);
	UVs.push_back(uvY);
}

void ModelData::PushUVPair(std::pair<GLfloat, GLfloat> uvs)
{
	UVs.push_back(uvs.first);
	UVs.push_back(uvs.second);
}

std::shared_ptr<ModelData> ModelData::GetDisc(const float radius, const unsigned int divisions, const Vector3 normal, const float height)
{
#ifdef _DEBUG
	assert(radius > 0.f);
	assert(divisions >= 3);
	assert(!normal.Equals(Vector3(0.f, 0.f, 0.f)));
#endif
	std::shared_ptr<ModelData> md = std::make_shared<ModelData>();
	md->Mode = GL_TRIANGLE_FAN; //first centre point gets created and then subsiquent points
	 
	md->SizeData.SetXSize(radius * 2.f);
	md->SizeData.SetYSize(0.f);
	md->SizeData.SetZSize(radius * 2.f);

	const float intervalsBetweenSamples = (2 * pi) / static_cast<float>(divisions); //calculate intervals
	const float uvDiamater = radius * 2.f;
	float angle = 360.f;

	//Push first vertex - middle one
	md->Vertices.push_back(0.f); //need centre point first becaue drawing using triangle fan
	md->Vertices.push_back(0.f + height);
	md->Vertices.push_back(0.f);
	
	//push first UVs - this is for the middle point
	md->UVs.push_back(0.5f);
	md->UVs.push_back(0.5f);

	//push first normal - all go up because disc is facing up.
	md->Normals.push_back(normal.GetX()); //nx
	md->Normals.push_back(normal.GetY()); //ny
	md->Normals.push_back(normal.GetZ()); //nz

	//Create circle around middle point
	for (unsigned int i = 0; i <= divisions; i++)
	{
		//push uvs
		md->UVs.push_back((std::cosf(angle) / uvDiamater) + 0.5f);
		md->UVs.push_back((std::sinf(angle) / uvDiamater) + 0.5f);

		//push points
		md->Vertices.push_back((radius * std::cosf(angle))); //x
		md->Vertices.push_back(height); //y is height
		md->Vertices.push_back((radius * std::sinf(angle))); //z

		//Push normals
		md->Normals.push_back(normal.GetX()); //nx
		md->Normals.push_back(normal.GetY()); //ny
		md->Normals.push_back(normal.GetZ()); //nz

		//Go to next point
		angle -= intervalsBetweenSamples;
	}

	return md;
}

std::shared_ptr<ModelData> ModelData::GetSphere(const float radius, const int segments)
{
	std::shared_ptr<ModelData> md = std::make_shared<ModelData>();
	md->Mode = GL_QUADS;
	md->SizeData.SetXSize(radius * 2.f);
	md->SizeData.SetYSize(radius * 2.f);
	md->SizeData.SetZSize(radius * 2.f);

	const float incrementsLongnitude = pi / segments; //longnitude increments/delta
	const float incrementsLatitude = 2 * pi / segments; //latitude increments/theta

	const float uvIncrements = 1.f / segments; //for UV's
	
	float uvY = 0.f;
	float currentLong = 0.f;
	for (int longnitude = 0; longnitude < segments; longnitude++) //longnitude = y
	{
		float currentLat = 0.f;
		float uvX = 1.f; //this is to 'mirror' the x as the algrorithm works from the inside.
		for (int latitude = 0; latitude < segments; latitude++) //latitude = x
		{
			/*Points*/
			Vector3 vertexTopLeft = { 
				radius * std::cosf(currentLat) * std::sinf(currentLong), 
				radius * std::cosf(currentLong), 
				radius * std::sinf(currentLat) * std::sinf(currentLong) };
			Vector3 vertexBottomLeft = { 
				radius * std::cosf(currentLat) * std::sinf(currentLong + incrementsLongnitude),
				radius * std::cosf(currentLong + incrementsLongnitude), 
				radius * std::sinf(currentLat) * std::sinf(currentLong + incrementsLongnitude) };
			Vector3 vertexBottomRight = {
				radius * std::cosf(currentLat + incrementsLatitude) * std::sinf(currentLong + incrementsLongnitude),
				radius * std::cosf(currentLong + incrementsLongnitude),
				radius * std::sinf(currentLat + incrementsLatitude) * std::sinf(currentLong + incrementsLongnitude) };
			Vector3 vertexTopRight = {
				radius * std::cosf(currentLat + incrementsLatitude) * std::sinf(currentLong),
				radius * std::cosf(currentLong),
				radius * std::sinf(currentLat + incrementsLatitude) * std::sinf(currentLong) };

			/*Normals*/
			Vector3 normalTopLeft = {
				(radius / radius) * std::cosf(currentLat) * std::sinf(currentLong),
				(radius / radius) * std::cosf(currentLong),
				(radius / radius) * std::sinf(currentLat) * std::sinf(currentLong) };
			Vector3 normalBottomLeft = {
				(radius / radius) * std::cosf(currentLat) * std::sinf(currentLong + incrementsLongnitude),
				(radius / radius) * std::cosf(currentLong + incrementsLongnitude),
				(radius / radius) * std::sinf(currentLat) * std::sinf(currentLong + incrementsLongnitude) };
			Vector3 normalBottomRight = {
				(radius / radius) * std::cosf(currentLat + incrementsLatitude) * std::sinf(currentLong + incrementsLongnitude),
				(radius / radius) * std::cosf(currentLong + incrementsLongnitude),
				(radius / radius) * std::sinf(currentLat + incrementsLatitude) * std::sinf(currentLong + incrementsLongnitude) };
			Vector3 normalTopRight = {
				(radius / radius) * std::cosf(currentLat + incrementsLatitude) * std::sinf(currentLong),
				(radius / radius) * std::cosf(currentLong),
				(radius / radius) * std::sinf(currentLat + incrementsLatitude) * std::sinf(currentLong) };

			/*UV's*/
			std::pair<GLfloat, GLfloat> uvPairTopLeft = { uvX, uvY };
			std::pair<GLfloat, GLfloat> uvPairBottomLeft = { uvX, uvY + uvIncrements};
			std::pair<GLfloat, GLfloat> uvPairBottomRight = { uvX - uvIncrements, uvY + uvIncrements };
			std::pair<GLfloat, GLfloat> uvPairTopRight = { uvX - uvIncrements, uvY };

			//0 - x,y
			md->PushVertexVector(vertexTopLeft);
			md->PushNormalVector(normalTopLeft);
			md->PushUVPair(uvPairTopLeft);

			//1 - x + 1, y
			md->PushVertexVector(vertexTopRight);
			md->PushNormalVector(normalTopRight);
			md->PushUVPair(uvPairTopRight);

			//2 - x + 1, y + 1
			md->PushVertexVector(vertexBottomRight);
			md->PushNormalVector(normalBottomRight);
			md->PushUVPair(uvPairBottomRight);
			
			//3 - x, y + 1
			md->PushVertexVector(vertexBottomLeft);
			md->PushNormalVector(normalBottomLeft);
			md->PushUVPair(uvPairBottomLeft);


			uvX -= uvIncrements;
			currentLat += incrementsLatitude;
		}

		uvY += uvIncrements;
		currentLong += incrementsLongnitude;
	}

	return md;

	
}

std::shared_ptr<ModelData> ModelData::GetCyclinder(const float radius, const int divisions, const float height, const int heightDivisons, const bool addBottomDisc, const bool addTopDisc)
{
	std::shared_ptr<ModelData> md = std::make_shared<ModelData>();
	md->Mode = GL_QUADS;
	md->SizeData.SetXSize(radius * 2.f);
	md->SizeData.SetYSize(height);
	md->SizeData.SetZSize(radius * 2.f);

	const float intervalsBetweenSamples = (2 * pi) / static_cast<float>(divisions); //calculate intervals
	const float uvDiamater = radius * 2.f;
	float angle = 0.f;

	if (addTopDisc)
	{
		/*top face*/
		for (int i = 0; i <= divisions; i++)
		{
			//push uvs
			md->PushUVPair(0.5f, 0.5f);
			md->PushUVPair(0.5f, 0.5f);
			md->PushUVPair((std::cosf(angle + intervalsBetweenSamples) / uvDiamater) + 0.5f, (std::sinf(angle + intervalsBetweenSamples) / uvDiamater) + 0.5f);
			md->PushUVPair((std::cosf(angle) / uvDiamater) + 0.5f, (std::sinf(angle) / uvDiamater) + 0.5f);

			//push points
			md->PushVertexVector({ 0.f, height / 2.f, 0.f }); //0
			md->PushVertexVector({ 0.f, height / 2.f, 0.f }); //3
			md->PushVertexVector({ (radius * std::cosf(angle + intervalsBetweenSamples)), height / 2.f,  (radius * std::sinf(angle + intervalsBetweenSamples)) }); //2
			md->PushVertexVector({ (radius * std::cosf(angle)), height /2.f,  (radius * std::sinf(angle)) }); //1

			//Push normals
			md->PushNormalVector({ 0.f, 1.f, 0.f });
			md->PushNormalVector({ 0.f, 1.f, 0.f });
			md->PushNormalVector({ 0.f, 1.f, 0.f });
			md->PushNormalVector({ 0.f, 1.f, 0.f });

			//Go to next point
			angle += intervalsBetweenSamples;
		}
	}

	/*Draw body*/
	angle = 0.f;
	const float hInc = height / heightDivisons; //how much the height should increment by
	float currentHeight = -height / 2.f; //what the current height is
	const float uvXIncrements = 1.f / divisions; //by how much the uv should increment
	const float uvYIncrements = 1.f / heightDivisons; //by how much the uv should increment
	float uvX = 0.f, uvY = 0.f; //keeping track of the uvs
	
	for (int heightLevels = 0; heightLevels < heightDivisons; heightLevels++)
	{
		for (int i = 0; i <= divisions - 1; i++) //draw dircle
		{
			//push uvs -- this fixes the uvs
			if (i == divisions - 1)
			{
				md->PushUVPair((uvX + uvXIncrements - UVfix), uvY); //top right
				md->PushUVPair(uvX, uvY); //top left
				md->PushUVPair(uvX, uvY + uvYIncrements); //bottom left
				md->PushUVPair((uvX + uvXIncrements) - UVfix, uvY + uvYIncrements); //bottom right
			}
			else if (i == 0)
			{
				md->PushUVPair(uvX + uvXIncrements, uvY); //top right
				md->PushUVPair(uvX + UVfix, uvY); //top left
				md->PushUVPair(uvX + UVfix, uvY + uvYIncrements); //bottom left
				md->PushUVPair(uvX + uvXIncrements, uvY + uvYIncrements); //bottom right
			}
			else
			{
				md->PushUVPair(uvX + uvXIncrements, uvY); //top right
				md->PushUVPair(uvX, uvY); //top left
				md->PushUVPair(uvX, uvY + uvYIncrements); //bottom left
				md->PushUVPair(uvX + uvXIncrements, uvY + uvYIncrements); //bottom right
			}


			//push points
			md->PushVertexVector({ radius * std::cosf(angle + intervalsBetweenSamples), currentHeight, (radius * std::sinf(angle + intervalsBetweenSamples)) }); //0 top right
			md->PushVertexVector({ radius * std::cosf(angle), currentHeight, (radius * std::sinf(angle)) }); //3 top left
			md->PushVertexVector({ radius * std::cosf(angle), currentHeight + hInc, (radius * std::sinf(angle)) }); //2 bottom left
			md->PushVertexVector({ radius * std::cosf(angle + intervalsBetweenSamples), currentHeight + hInc, (radius * std::sinf(angle + intervalsBetweenSamples)) }); //1 bottom right

			//Push normals - y is 0.f because the normal points in a direction from 0.
			md->PushNormalVector({ (radius / radius) * std::cosf(angle + intervalsBetweenSamples), 
								 0.f, 
								 ((radius / radius) * std::sinf(angle + intervalsBetweenSamples)) }); //0 top right
			md->PushNormalVector({ (radius / radius) * std::cosf(angle), 
								 0.f, 
								 ((radius / radius) * std::sinf(angle)) }); //3 top left
			md->PushNormalVector({ (radius / radius) * std::cosf(angle), 
								 0.f, 
								 ((radius / radius) * std::sinf(angle)) }); //2 bottom left
			md->PushNormalVector({ (radius / radius) * std::cosf(angle + intervalsBetweenSamples), 
								 0.f, 
								 ((radius / radius) * std::sinf(angle + intervalsBetweenSamples)) }); //1 bottom right

			//Go to next point
			angle += intervalsBetweenSamples;
			uvX += uvXIncrements;
		}
		angle = 0.f; //reset angle

		//Add height
		currentHeight += hInc;
		
		//uvs
		uvX = 0.f;
		uvY += uvYIncrements;
	}

	if (addBottomDisc)
	{
		/*bottom face*/
		angle = 0.f;
		for (int i = 0; i <= divisions; i++) 
		{
			//push uvs
			md->PushUVPair(0.5f, 0.5f);
			md->PushUVPair((std::cosf(angle) / uvDiamater) + 0.5f, (std::sinf(angle) / uvDiamater) + 0.5f);
			md->PushUVPair((std::cosf(angle + intervalsBetweenSamples) / uvDiamater) + 0.5f, (std::sinf(angle + intervalsBetweenSamples) / uvDiamater) + 0.5f);
			md->PushUVPair(0.5f, 0.5f);

			//push points
			md->PushVertexVector({ 0.f, -height / 2.f, 0.f }); //0
			md->PushVertexVector({ (radius * std::cosf(angle)), -height / 2.f,  (radius * std::sinf(angle)) }); //1
			md->PushVertexVector({ (radius * std::cosf(angle + intervalsBetweenSamples)), -height / 2.f,  (radius * std::sinf(angle + intervalsBetweenSamples)) }); //2
			md->PushVertexVector({ 0.f, -height / 2.f, 0.f }); //3

			//Push normals
			md->PushNormalVector({ 0.f, -1.f, 0.f });
			md->PushNormalVector({ 0.f, -1.f, 0.f });
			md->PushNormalVector({ 0.f, -1.f, 0.f });
			md->PushNormalVector({ 0.f, -1.f, 0.f });

			//Go to next point
			angle += intervalsBetweenSamples;
		}
	}

	return md;
}

std::shared_ptr<ModelData> ModelData::GetCone(const float radius, const int divisions, const float height, const float heightDivisions, bool drawBottomFace)
{
	std::shared_ptr<ModelData> md = std::make_shared<ModelData>();
	md->Mode = GL_QUADS;
	md->SizeData.SetXSize(radius * 2.f);
	md->SizeData.SetYSize(height);
	md->SizeData.SetZSize(radius * 2.f);

	const float angleIncrements = 2 * pi / divisions;

	const float yIncrements = height / heightDivisions;

	const float uvXIncrements = 1.f / divisions;
	const float uvYIncrements = 1.f / heightDivisions;

	float currentY = 0.f;
	for (int y = 1; y <= heightDivisions; y++)
	{
		float currentRadius = y * (radius / heightDivisions);
		float lastRadius = static_cast<float>(y - 1) * (radius / heightDivisions);
		for (int x = 0; x < divisions; x++)
		{
			/*Verticies*/
			Vector3 topLeft = { currentRadius * std::cosf(static_cast<float>(x) * angleIncrements), 
				-static_cast<float>(y) * yIncrements, 
				currentRadius * std::sinf(static_cast<float>(x) * angleIncrements) };
			Vector3 bottomLeft = { lastRadius * std::cosf(static_cast<float>(x) * angleIncrements),
				-static_cast<float>(y - 1) * yIncrements, 
				lastRadius * std::sinf(static_cast<float>(x) * angleIncrements) };;
			Vector3 bottomRight = { lastRadius * std::cosf(static_cast<float>(x + 1) * angleIncrements),
				-static_cast<float>(y - 1) * yIncrements, 
				lastRadius * std::sinf(static_cast<float>(x + 1) * angleIncrements) };;
			Vector3 topRight = { currentRadius * std::cosf(static_cast<float>(x + 1) * angleIncrements), 
				-static_cast<float>(y) * yIncrements, 
				currentRadius * std::sinf(static_cast<float>(x + 1) * angleIncrements) };;
			
			//Move up by height as otherwise the top of the cone is where 0,0,0 is.
			topLeft.y += height;
			bottomLeft.y += height;
			bottomRight.y += height;
			topRight.y += height;

			md->PushVertexVector(topLeft); //0
			md->PushVertexVector(bottomLeft); //1
			md->PushVertexVector(bottomRight); //2
			md->PushVertexVector(topRight); //3

			/*Normals*/
			Vector3 faceNormal = (bottomLeft - topLeft).Cross(topRight - topLeft).Normalised();
			//faceNormal = Vector3(1.f, 1.f, 1.f) - faceNormal; //this makes the normals display but breaks the lighting direction
			for (int i = 0; i < 4; i++)
			{
				md->PushNormalVector(faceNormal);
			}

			/*UVs*/
			/*the x and y values are a bit weird because the x loop starts at 0 and increments to vals - 1, where as y starts at 1 and increments to vals.
			this causes 0,0 to be x, y-1; 1,1 to be x + 1, y; 1,0 to be x + 1, y - 1; 0,1 to be x, y. basically x = 0 :: x = x, where as y = 0 :: y = y - 1
			*/
			std::pair<GLfloat, GLfloat> uvTopLeft = { uvXIncrements * x, uvYIncrements * y};
			std::pair<GLfloat, GLfloat> uvBottomLeft = { uvXIncrements * x, uvYIncrements * (y - 1) };
			std::pair<GLfloat, GLfloat> uvBottomRight = { uvXIncrements * (x + 1), uvYIncrements * (y - 1) };
			std::pair<GLfloat, GLfloat> uvTopRight = { uvXIncrements * (x + 1), uvYIncrements * y };

			//apply uv fix
			if (x == divisions - 1)
			{
				uvTopRight.first -= UVfix;
				uvBottomRight.first -= UVfix;
			}
			else if (x == 0)
			{
				uvTopLeft.first += UVfix;
				uvBottomLeft.first += UVfix;
			}

			if (y == heightDivisions)
			{
				uvTopLeft.second -= UVfix;
				uvTopRight.second -= UVfix;
			}
			else if (y == 1)
			{
				uvBottomLeft.second += UVfix;
				uvBottomRight.second += UVfix;
			}


			md->PushUVPair(uvTopLeft);
			md->PushUVPair(uvBottomLeft);
			md->PushUVPair(uvBottomRight);
			md->PushUVPair(uvTopRight);
		}
	}

	if (drawBottomFace)
	{
		//Draw bottom face.
		float angle = 0.f;
		const float uvDiamater = radius * 2.f;
		const float intervalsBetweenSamples = (2 * pi) / static_cast<float>(divisions); //calculate intervals
		for (int i = 0; i <= divisions; i++)
		{
			//push uvs
			md->PushUVPair(0.5f, 0.5f);
			md->PushUVPair((std::cosf(angle) / uvDiamater) + 0.5f, (std::sinf(angle) / uvDiamater) + 0.5f);
			md->PushUVPair((std::cosf(angle + intervalsBetweenSamples) / uvDiamater) + 0.5f, (std::sinf(angle + intervalsBetweenSamples) / uvDiamater) + 0.5f);
			md->PushUVPair(0.5f, 0.5f);

			//push points
			md->PushVertexVector({ 0.f, 0.f, 0.f }); //0
			md->PushVertexVector({ (radius * std::cosf(angle)), 0.f,  (radius * std::sinf(angle)) }); //1
			md->PushVertexVector({ (radius * std::cosf(angle + intervalsBetweenSamples)), 0.f,  (radius * std::sinf(angle + intervalsBetweenSamples)) }); //2
			md->PushVertexVector({ 0.f, 0.f, 0.f }); //3

			//Push normals
			md->PushNormalVector({ 0.f, -1.f, 0.f });
			md->PushNormalVector({ 0.f, -1.f, 0.f });
			md->PushNormalVector({ 0.f, -1.f, 0.f });
			md->PushNormalVector({ 0.f, -1.f, 0.f });

			//Go to next point
			angle += intervalsBetweenSamples;
		}
	}


	return md;
}

std::shared_ptr<ModelData> ModelData::GetXYQuad(const float xSize, const float ySize, const Vector3 normal)
{
	auto md = std::make_shared<ModelData>();
	md->Mode = GL_QUADS;
	md->SizeData.SetXSize(xSize);
	md->SizeData.SetYSize(ySize);
	md->SizeData.SetZSize(0.f);

	/*Push Vertices*/
	md->PushVertexVector({-xSize, ySize, 0.f });
	md->PushVertexVector({-xSize, -ySize, 0.f });
	md->PushVertexVector({ xSize, -ySize, 0.f });
	md->PushVertexVector({ xSize, ySize, 0.f });

	/*Push UV's*/
	md->PushUVPair(0.f, 1.f);
	md->PushUVPair(0.f, 0.f);
	md->PushUVPair(1.f, 0.f);
	md->PushUVPair(1.f, 1.f);

	/*Push Normals*/
	for (int i = 0; i < 4; i++)
	{
		md->PushNormalVector(normal);
	}

	return md;
}

#pragma region GETSPHEREOLD
/*As with the tradition with my code here lies about 3 hours of attempted debugging. Treat it like a museum piece.*/

//ModelData GetSphereOld(Vector3 centrePos, float radius, int segments)
//{
//	/*X =rCosTheta * sinDelta
//	Y = rCosDelta
//	Z = rSinTheta * sinDelta
//	Angle of latitude = theta
//	Angle of longitude = delta
//	*/
//	const float pi = 3.14159;
//	std::vector<std::vector<Vector3>> sphereRawData;
//	sphereRawData.reserve(segments * segments * 3);
//
//	const float incrementsDelta = pi / segments;
//	const float incrementsTheta = 2 * pi / segments;
//
//
//	//tmp.push_back(Vector3(0.f, 1.f, 0.f)); //push top
//	//tmp.push_back(0.f);
//	//tmp.push_back(1.f);
//	//tmp.push_back(0.f);
//	//delta += incrementsDelta;
//
//
//	float delta = 0.f; //this is the vertical component
//
//	for (int longnitude = 0; longnitude <= segments; longnitude++)
//	{
//		float theta = incrementsTheta; //this is the spherical componet --starts at theta because the
//		std::vector<Vector3> longnitudeData;
//		for (int latitude = 0; latitude < segments; latitude++)
//		{
//			Vector3 temp;
//			temp.x = radius * std::cosf(theta) * std::sinf(delta);
//			temp.y = radius * std::cosf(delta);
//			temp.z = radius * std::sinf(theta) * std::sinf(delta);
//
//			longnitudeData.push_back(temp);
//
//			theta += incrementsTheta;
//		}
//
//		sphereRawData.push_back(longnitudeData);
//
//		delta += incrementsDelta;
//	}
//
//	//tmp.push_back(0.f);
//	//tmp.push_back(0.f);
//	//tmp.push_back(0.f);
//
//	//make into actual format the computer can draw
//	/*Theta = (2*pi) / number of segments
//	Delta = pi / number of segments
//
//	Loop for longitude
//		Loop for latitude
//			Calculate x, y and z for 4 vertices
//			[lats][longs]
//			[lats][longs+1]
//			[lats+1][longs+1]
//			[lats+1][longs]
//			Render face
//			Increment latitude
//	Increment longitude
//	*/
//
//	//special case for start and end
//
//	ModelData md;
//
//	//longnitude = y, latitude = x.
//
//	for (int longnitude = 0; longnitude < segments - 1; longnitude++) //-1 because otherwise it will go out of bounds because of existing longnitude calculations. last longnitde + 1 will be error without this - 1.
//	{
//		for (int latitude = 0; latitude < segments - 1; latitude++)
//		{
//			Vector3 temp1, temp2, temp3, temp4; //each side of quad
//
//			//FIRST longnitudes THEN latitudes.
//			temp1 = sphereRawData.at(longnitude).at(latitude);
//			temp2 = sphereRawData.at(longnitude + 1).at(latitude);
//			temp3 = sphereRawData.at(longnitude + 1).at(latitude + 1);
//			temp4 = sphereRawData.at(longnitude).at(latitude + 1);
//
//			//now push the values into the model data
//			md.PushVertexVector(temp1);
//			md.PushVertexVector(temp2);
//			md.PushVertexVector(temp3);
//			md.PushVertexVector(temp4);
//			/*
//			//GLfloat tempx, tempy, tempz;//0048505740142 - ciocia ania
//			// width * x + y
//
//			////[lats][longs]
//			//md.Vertices.push_back(tmp.at(segments * latitude + 0 + longnitude)); //x
//			//md.Vertices.push_back(tmp.at(segments * latitude + 1 + longnitude)); //y
//			//md.Vertices.push_back(tmp.at(segments * latitude + 2 + longnitude)); //z
//			//
//			////[lats][longs+1]
//			//md.Vertices.push_back(tmp.at(segments * latitude + 0 + longnitude + 1)); //x
//			//md.Vertices.push_back(tmp.at(segments * latitude + 1 + longnitude + 1)); //y
//			//md.Vertices.push_back(tmp.at(segments * latitude + 2 + longnitude + 1)); //z
//
//			////[lats + 1][longs + 1]
//			//md.Vertices.push_back(tmp.at(segments * latitude + 0 + 1 + longnitude + 1)); //x
//			//md.Vertices.push_back(tmp.at(segments * latitude + 1 + 1 + longnitude + 1)); //y
//			//md.Vertices.push_back(tmp.at(segments * latitude + 2 + 1 + longnitude + 1)); //z
//
//			////[lats+1][longs]
//			//md.Vertices.push_back(tmp.at(segments * latitude + 0 + 1 + longnitude)); //x
//			//md.Vertices.push_back(tmp.at(segments * latitude + 1 + 1 + longnitude)); //y
//			//md.Vertices.push_back(tmp.at(segments * latitude + 2 + 1 + longnitude)); //z
//			*/
//		}
//	}
//
//	for (int longnitude = 0; longnitude < segments - 1; longnitude++)
//	{
//		//last => 0 => last => 0.
//		Vector3 temp1, temp2, temp3, temp4; //each side of quad - anticlockwise
//		temp1 = sphereRawData.at(longnitude + 1).at(segments - 1); //4
//		temp2 = sphereRawData.at(longnitude + 1).at(0); //3
//		temp3 = sphereRawData.at(longnitude).at(0); //2
//		temp4 = sphereRawData.at(longnitude).at(segments - 1); //1
//
//		md.PushVertexVector(temp1);
//		md.PushVertexVector(temp2);
//		md.PushVertexVector(temp3);
//		md.PushVertexVector(temp4);
//	}
//
//	for (int latitude = 0; latitude < segments - 1; latitude++)
//	{
//		Vector3 temp1, temp2, temp3, temp4; //each side of quad - anticlockwise
//
//		temp1 = sphereRawData.at(segments).at(latitude);
//		temp2 = sphereRawData.at(segments).at(latitude + 1);
//		temp3 = sphereRawData.at(segments - 1).at(latitude + 1);
//		temp4 = sphereRawData.at(segments - 1).at(latitude);
//
//		md.PushVertexVector(temp1);
//		md.PushVertexVector(temp2);
//		md.PushVertexVector(temp3);
//		md.PushVertexVector(temp4);
//	}
//
//	//ModelData md;
//	md.Mode = GL_QUADS;
//	return md;
//}



//*This calculates the raw data for the sphere - just the points*/
	//std::vector<std::vector<Vector3>> sphereRawData;
	//sphereRawData.reserve(segments * segments);

	//const float incrementsDelta = pi / segments; //for longnitude ( y )
	//const float incrementsTheta = 2 * pi / segments; //for latitude ( x )

	//float delta = 0.f; //this is the vertical component

	//for (int longnitude = 0; longnitude <= segments; longnitude++)
	//{
	//	float theta = incrementsTheta; //this is the spherical componet --starts at theta because the
	//	std::vector<Vector3> longnitudeData;
	//	for (int latitude = 0; latitude < segments; latitude++)
	//	{
	//		Vector3 temp;
	//		temp.x = radius * std::cosf(theta) * std::sinf(delta);
	//		temp.y = radius * std::cosf(delta);
	//		temp.z = radius * std::sinf(theta) * std::sinf(delta);

	//		longnitudeData.push_back(temp);

	//		theta += incrementsTheta;
	//	}
	//	sphereRawData.push_back(longnitudeData);

	//	delta += incrementsDelta;
	//}

	///*This stiches the coordniates from the raw sphere and converts it to coordinates*/
	//ModelData md;

	//const float uvXIncrements = 1.f / segments;
	//const float uvYIncrements = 1.f / segments;
	//float uvX = 1.f, uvY = 1.f;

	//for (int longnitude = 0; longnitude < segments; longnitude++) 
	//{
	//	for (int latitude = 0; latitude < segments - 1; latitude++) //-1 because otherwise it will go out of bounds because of existing longnitude calculations. last latitude + 1 will be error without this '- 1'.
	//	{
	//		Vector3 temp1, temp2, temp3, temp4; //each side of quad

	//		//FIRST longnitudes THEN latitudes.
	//		temp1 = sphereRawData.at(longnitude).at(latitude);
	//		temp2 = sphereRawData.at(longnitude).at(latitude + 1);
	//		temp3 = sphereRawData.at(longnitude + 1).at(latitude + 1);
	//		temp4 = sphereRawData.at(longnitude + 1).at(latitude);

	//		//now push the values into the model data
	//		md.PushVertexVector(temp1); //original works but inside out
	//		md.PushNormalVector(temp1);
	//		md.PushUVPair(uvX, uvY);

	//		md.PushVertexVector(temp2);
	//		md.PushNormalVector(temp2);
	//		md.PushUVPair(uvX - uvXIncrements, uvY);

	//		md.PushVertexVector(temp3);
	//		md.PushNormalVector(temp3);
	//		md.PushUVPair(uvX - uvXIncrements, uvY - uvYIncrements);

	//		md.PushVertexVector(temp4);
	//		md.PushNormalVector(temp4);
	//		md.PushUVPair(uvX, uvY - uvYIncrements);
	//		uvX -= uvXIncrements;

	//		//md.PushVertexVector(temp1); //idk wtaf is going here
	//		//md.PushUVPair(uvX + uvXIncrements, uvY);

	//		//md.PushVertexVector(temp2);
	//		//md.PushUVPair(uvX + uvXIncrements, uvY + uvYIncrements);

	//		//md.PushVertexVector(temp3);
	//		//md.PushUVPair(uvX, uvY + uvYIncrements);

	//		//md.PushVertexVector(temp4);
	//		//md.PushUVPair(uvX, uvY);

	//		//uvX += uvXIncrements;
	//	}
	//	
	//	/*Stich together the last and beginning bits together*/
	//	Vector3 temp1, temp2, temp3, temp4; //each side of quad - anticlockwise
	//	temp1 = sphereRawData.at(longnitude).at(segments - 1);
	//	temp2 = sphereRawData.at(longnitude).at(0);
	//	temp3 = sphereRawData.at(longnitude + 1).at(0);
	//	temp4 = sphereRawData.at(longnitude + 1).at(segments - 1);

	//	md.PushVertexVector(temp1);
	//	md.PushUVPair(uvX, uvY);

	//	md.PushVertexVector(temp2);
	//	md.PushUVPair(uvX + uvXIncrements, uvY);

	//	md.PushVertexVector(temp3);
	//	md.PushUVPair(uvX + uvXIncrements, uvY + uvYIncrements);

	//	md.PushVertexVector(temp4);
	//	md.PushUVPair(uvX, uvY + uvYIncrements);
	//	uvX = 1.f;
	//	uvY -= uvXIncrements;
	//}


	//return md;

#pragma endregion

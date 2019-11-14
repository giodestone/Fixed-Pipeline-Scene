#pragma once
#include "GameEntity.h"
#include "Camera.h" //THIS COULD BE A BIG SOURCE OF PROBLEMS

#include "Scene.h"

namespace TownCentreHelper
{
	enum TOWN_CENTRE_FACE : char
	{
		FRONT, BACK, LEFT, RIGHT
	};

	/// <summary>
	/// A struct that holds all the nessesary data about the draw order of a face for the TownCentre object.
	/// </summary>
	struct TownCentreFaces
	{
	private:
		int order = 0;
		TOWN_CENTRE_FACE face;
		Vector3 localPos;
		float distanceFromCamera;

	public:
		TownCentreFaces() {}

		TownCentreFaces(int order, TOWN_CENTRE_FACE face, Vector3 localPos, float distanceFromCamera = 0.f)
			: order(order),
			face(face),
			localPos(localPos),
			distanceFromCamera(distanceFromCamera)
		{}

		~TownCentreFaces() {}

		/// <returns>
		/// Returns the order of which the face should be drawn.
		/// </returns>
		int GetOrder() const { return order; }
		
		/// <returns>
		/// Info for which face the struct holds.
		/// </returns>
		TOWN_CENTRE_FACE GetFace() const { return face; };

		/// <returns>
		/// The local position of the face.
		/// </returns>
		Vector3 GetLocalPosition() const { return localPos; };

		/// <returns>
		/// Distance from camera.
		/// </returns>
		float GetDistanceFromCam() const { return distanceFromCamera; }

		/// <summary>
		/// Sets the distance to the camera.
		/// </summary>
		/// <param name="distance">New distance to the camera.</param>
		void SetDistanceSqFromCam(float distanceSq) { this->distanceFromCamera = distanceSq; }

		/// <summary>
		/// For the std::sort function.
		/// </summary>
		/// <param name="tcf1"></param>
		/// <param name="tcf2"></param>
		/// <returns></returns>
		static bool SortTownCentreFaces(const TownCentreFaces& tcf1, const TownCentreFaces& tcf2)
		{
			return tcf1.GetDistanceFromCam() > tcf2.GetDistanceFromCam();
		}


	};

}
class TownCentre :
	public GameEntity
{
private:
	std::weak_ptr<Camera> camera;

	std::vector<int> lightsToEnable;
	bool lightingEnabled = false;

	std::weak_ptr<ModelData> faceData;
	GLint faceTexture;
	std::array<TownCentreHelper::TownCentreFaces, 4> faceDrawOrder = //draw order, which face, the position of the face LOCALLY, distance between camera and face
	{
		TownCentreHelper::TownCentreFaces(1, TownCentreHelper::BACK, Vector3(0.f, 0.f, -1.f), 0.f),
		TownCentreHelper::TownCentreFaces(2, TownCentreHelper::LEFT, Vector3(-1.f, 0.f, 0.f), 0.f),
		TownCentreHelper::TownCentreFaces(3, TownCentreHelper::RIGHT, Vector3(1.f, 0.f, 0.f), 0.f),
		TownCentreHelper::TownCentreFaces(4, TownCentreHelper::FRONT, Vector3(0.f, 0.f, 1.f), 0.f)
	};

	float angle1 = 0.f;
	float angle2 = 0.f;

public:
	TownCentre(std::weak_ptr<Camera> camera, std::weak_ptr<ModelData> face, GLint faceTexture);
	~TownCentre();

	void Update(float dt) override;

	void Draw() override;

	void drawCuboid(float halfXSize, float halfYSize, float halfZSize, float topU, float topV, float frontU, float frontV, float leftU, float leftV);
};


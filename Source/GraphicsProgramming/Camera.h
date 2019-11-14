#pragma once

#include "Input.h"

#include "GameEntity.h"

#include "Vector3.h"

//TODO: Could implement component system to make multiple camera modes quicker to add.

/// <summary>
/// For determing the current state of the camera
/// </summary>
enum CameraCurrentState : char
{
	NOCLIP, FIXEDPOSITION, SPINNING
};

class Camera : public GameEntity
{
private:
	static constexpr float movementSpeed = 7.65f;
	static constexpr float mouseSensitiity = 4.0f;
	static constexpr float pi = 3.1415f;
private:
	CameraCurrentState curCamState = NOCLIP;
	bool wasChangeCamStateDown = false;

	/*Position, Rotation, LookAt*/
	Vector3 lookAt;

	/*For calculations*/
	Vector3 up;
	Vector3 forward;
	Vector3 right;

	/*For Movement, rotation and control*/
	Vector3 moveInput;
	Vector3 roationInput;

	bool mouseLocked = true, wasLockMouseDown = false;

	const Vector3 DefaultPosition = { 0.f, 5.f, 0.f }; //default position of the camera.

	const Vector3 fixedPosition = { 39.85f, 23.25f, 39.85f }; //the position of a fixed obj

	Vector3 spinningPosition = { 0.f, 25.f, 0.f }; //current spinning camera position.
	const Vector3 spinningLookAt = { 0.f, 0.f, 0.f }; //where the spinning camera is looking at
	const float spinningRadius = 65.f; //how far away from 0,0,0 does it rotate from
	float currentAngle = 0.f; //for controlling where the spinning camera is rotating about.
public:
	Camera() = default;
	~Camera() = default;

	/*Getters and Setters*/

	Vector3 GetLookAt() const;
	Vector3 GetUp() const;

	/*Methods*/
	void Load(AssetManager& assetManager) override;

	void ProcessInput(Input& input, float dt) override;
	
	void Update(float dt) override;

	void calculateLookAt();
	
	void Draw() override;

private:
	void processInputMouse(Input & input);
	void processInputKeys(Input & input);

	void calculateForwardUpRight();
	
};
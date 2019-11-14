#include "Camera.h"

Vector3 Camera::GetLookAt() const
{
	return lookAt;
}

Vector3 Camera::GetUp() const
{
	return up;
}

void Camera::Load(AssetManager& assetManager)
{
	GetRefToPosition().Set(0.f, 5.f, -5.f);
	//GetRefToRotation() = {}
}

void Camera::ProcessInput(Input & input, float dt)
{
	moveInput.Set(0.f, 0.f, 0.f); //reset inputs
	roationInput.Set(0.f, 0.f, 0.f);

	processInputKeys(input);
	processInputMouse(input);
}

void Camera::processInputMouse(Input & input)
{
	if (input.isKeyDown('x') && !wasLockMouseDown)
	{
		mouseLocked = !mouseLocked;
		if (mouseLocked)
			glutSetCursor(GLUT_CURSOR_NONE);
		else
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		wasLockMouseDown = true;
	}
	else if (!input.isKeyDown('x'))
		wasLockMouseDown = false;

	if (mouseLocked)
	{
		roationInput.Set(
			static_cast<float>(input.getMouseY() - (glutGet(GLUT_WINDOW_HEIGHT) / 2)),
			static_cast<float>(input.getMouseX() - (glutGet(GLUT_WINDOW_WIDTH) / 2)),
			roationInput.GetZ());

		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	}

	if (input.isKeyDown('z') && !wasChangeCamStateDown)
	{
		switch (curCamState)
		{
		case NOCLIP:
			curCamState = FIXEDPOSITION;
			break;
		case FIXEDPOSITION:
			GetRefToRotation() = { 0.f, 0.f, 0.f }; //set rotation to up so the view doesn't look weird.
			curCamState = SPINNING;
			break;
		case SPINNING:
			curCamState = NOCLIP;
			break;
		}
		wasChangeCamStateDown = true;
	}
	else if (!input.isKeyDown('z'))
	{
		wasChangeCamStateDown = false;
	}
}

void Camera::processInputKeys(Input & input)
{
	/*Keys*/
	if (input.isKeyDown('w')) //z forward/back
		moveInput.SetZ(1.f);
	if (input.isKeyDown('s'))
		moveInput.SetZ(-1.f);
	if (input.isKeyDown('a')) //x left/right
		moveInput.SetX(-1.f);
	if (input.isKeyDown('d'))
		moveInput.SetX(1.f);
	if (input.isKeyDown('r')) //y up/down
		moveInput.SetY(1.f);
	if (input.isKeyDown('f'))
		moveInput.SetY(-1.f);

	if (input.isKeyDown('e'))
		roationInput.SetZ(-1.f);
	if (input.isKeyDown('q'))
		roationInput.SetZ(1.f);
}

void Camera::Update(float dt)
{
	calculateForwardUpRight();

	moveInput.Normalise(); //first normalise
	moveInput.Scale(dt * movementSpeed); //then scale otherwise speed is always the same

	roationInput.x *= mouseSensitiity * dt;
	roationInput.y *= mouseSensitiity * dt;
	roationInput.z *= mouseSensitiity * 40.f * dt; //because roll/z is controlled by the keyboard

	//Change movement and positon based on the state.
	switch (curCamState)
	{
	case NOCLIP: //All controls are applied
	{
		GetRefToPosition() += right * moveInput.GetX(); //move left/right
		GetRefToPosition() += up * moveInput.GetY(); //move up/down (absolute)
		GetRefToPosition() += forward * moveInput.GetZ(); //move forward/back

		GetRefToRotation() += roationInput; //calculate rotation - works because the rotation is stored inside of the rotation variable from the calculateRotation class.
	}
	break;
	case FIXEDPOSITION: //Cant move but can look around.
	{
		GetRefToPosition() = fixedPosition;
		GetRefToRotation() += roationInput; //calculate rotation - works because the rotation is stored inside of the rotation variable from the calculateRotation class.
	}
	break;
	case SPINNING: //just the pitch is applied
	{
		currentAngle += 1.f * dt;

		float newX, newZ; //determines new pos of the camera, basically takes drawing a circle into a different point
		newX = spinningRadius * std::cosf(currentAngle);
		newZ = spinningRadius * std::sinf(currentAngle);

		GetRefToPosition() = { newX, GetRefToPosition().GetY(), newZ };
		lookAt = spinningLookAt;
	}
	break;
	}

	//clamp the view to 90 and -90 degrees
	if (GetRefToRotation().GetX() > 90.f) 
		GetRefToRotation().SetX(90.f);
	else if (GetRefToRotation().GetX() < -90.f)
		GetRefToRotation().SetX(-90.f);

	switch (curCamState)
	{
	case NOCLIP:
	case FIXEDPOSITION:
		calculateLookAt(); //ALWAYS CALCULATE LAST!
		break;
	}
}

void Camera::calculateLookAt()
{
	//calculate where to look at. Should be forward plus position
	lookAt = forward + GetRefToPosition();
}

void Camera::Draw()
{
	/*GLfloat color[] = { 0.f, 1.f, 0.5f, 1.f };
	const float scale = 1.f, z = 1.f;
	glColor4fv(color);
	glBegin(GL_QUADS);
	glVertex3f(-1.f * scale, 1.f * scale, z);
	glVertex3f(-1.f * scale, -1.f * scale, z);
	glVertex3f(1.f * scale, -1.f * scale, z);
	glVertex3f(1.f * scale, 1.f * scale, z);
	glEnd();*/
}

void Camera::calculateForwardUpRight()
{
	//x = pitch, y = yaw, z = roll

	//Values for storing calculation results;
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	//Calculate all sins, coses of the position and rotation for manipulating both
	cosY = cosf(static_cast<float>(static_cast<double>(GetRefToRotation().GetY())*3.1415 / 180.0));
	cosP = cosf(static_cast<float>(static_cast<double>(GetRefToRotation().GetX())*3.1415 / 180.0));
	cosR = cosf(static_cast<float>(static_cast<double>(GetRefToRotation().GetZ())*3.1415 / 180.0));
	sinY = sinf(static_cast<float>(static_cast<double>(GetRefToRotation().GetY())*3.1415 / 180.0));
	sinP = sinf(static_cast<float>(static_cast<double>(GetRefToRotation().GetX())*3.1415 / 180.0));
	sinR = sinf(static_cast<float>(static_cast<double>(GetRefToRotation().GetZ())*3.1415 / 180.0));

	//For determing where the camera should be facing and where is forward relating to the rotation.
	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP * -cosY;

	//For determing where is up for the camera (for going up or down)
	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;

	//Where is right (for strafing)
	right = forward.Cross(up);
}

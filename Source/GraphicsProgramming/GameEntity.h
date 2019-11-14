#pragma once
#ifdef _DEBUG
#include <assert.h>
#endif // _DEBUG

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "AssetManager.h"
#include "Input.h"
#include "Vector3.h"

/// <summary>
/// This is the base type for anything that goes into the game.
/// 
/// By default any drawing is not handled. Getters and setters to position and rotation are provided.
/// </summary>
class GameEntity
{
	friend class GameEntityManager;
public:
	GameEntity();
	~GameEntity();
	
private:
	unsigned int drawOrder = UINT_MAX / 2; // draw order, by default UINT_MAX.

	bool isTextruingEnabled = true;

	bool isLoaded = false; //For determing if the load function has been called.
	bool toBeDeleted = false; //For determing if the object is to be deteled
	
	Vector3 position; //Position of object
	Vector3 rotation; //Rotation - rotates first by x, then y, then z. e.g. setting x to 45 is equivilant to calling glRotatef(45.f, 1.f, 0.f, 0.f);
	Vector3 scale = { 1.f,1.f,1.f }; //Scale on x,y,z default value of 1.f, 1.f, 1.f

public:
	/*========Getters and setters========*/

	/// <summary>
	/// Get a the position.
	/// </summary>
	/// <returns>Copy of the position.</returns>
	Vector3 GetPosition() const;

	/// <summary>
	/// Get a reference to the position.
	/// </summary>
	/// <returns>Reference to the position.</returns>
	Vector3& GetRefToPosition();

	/// <summary>
	/// Set the position.
	/// </summary>
	/// <param name="pos">Position to make the object by reference.</param>
	void SetPosition(Vector3& pos);

	/// <summary>
	/// Sets the positon.
	/// </summary>
	/// <param name="pos">Position by value.</param>
	void SetPosition(const Vector3 pos);

	/// <summary>
	/// Get the value of the rotation.
	/// </summary>
	/// <returns>Rotation by value.</returns>
	Vector3 GetRotation() const;

	/// <summary>
	/// Get a reference to the rotation.
	/// </summary>
	/// <returns>Reference to rotation.</returns>
	Vector3& GetRefToRotation();

	/// <summary>
	/// Amount to set the rotation to.
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(Vector3 rotation);

	/// <summary>
	/// Add to total rotaiton.
	/// </summary>
	/// <param name="rotation">Amount to add to rotation.</param>
	void AddRotation(Vector3 rotation);

	/// <summary>
	/// Sets the scale of an object.
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3 scale);

	/// <summary>
	/// Get the scale of the object.
	/// </summary>
	/// <returns>Scale.</returns>
	Vector3 GetScale() const;

	/// <summary>
	/// Get a reference to the scale of the object.
	/// </summary>
	/// <returns>Reference to scale.</returns>
	Vector3& GetRefToScale();

	/// <summary>
	/// Check if the load function has been called.
	/// </summary>
	/// <returns></returns>
	bool IsLoaded();
	
	/// <summary>
	/// Checks if the object should be delted.
	/// </summary>
	/// <returns></returns>
	bool GetToBeDeleted();

	/// <summary>
	/// Set the object to be deleted.
	/// </summary>
	/// <param name="toBeDeleted"></param>
	void SetToBeDeleted(bool toBeDeleted);

	/// <summary>
	/// Get the current draw order. By default UINT_MAX / 2.
	/// </summary>
	/// <returns>Draw order as an unsigned int.</returns>
	unsigned int GetDrawOrder() const;

	/// <summary>
	/// Sets the draw order.
	/// </summary>
	/// <param name="drawOrder">New draw order. The lower the more first it draws.</param>
	void SetDrawOrder(unsigned int drawOrder);

	/// <summary>
	/// Returns if texturing is enabled.
	/// </summary>
	/// <returns>If textruing is enabled.</returns>
	bool IsTexturingEnabled() const;

	/*=======Methods========*/

	/// <summary>
	/// Called on scene load.
	/// </summary>
	virtual void Load(AssetManager& assetManager);

	/// <summary>
	/// For updating any input for the object.
	/// </summary>
	/// <param name="input"></param>
	/// <param name="dt"></param>
	virtual void ProcessInput(Input& input, float dt);

	/// <summary>
	/// For updating every frame.
	/// </summary>
	/// <param name="dt"></param>
	virtual void Update(float dt);

	/// <summary>
	/// For any drawing code.
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// Enables texturing. For shadows.
	/// </summary>
	virtual void EnableTexturing();
	
	/// <summary>
	/// Disables texturing. For Shadows.
	/// </summary>
	virtual void DisableTexturing();

	/// <summary>
	/// For the std::sort function. Sorts by draw order, ascending.
	/// </summary>
	/// <param name="gameEntity1"></param>
	/// <param name="gameEnt"></param>
	/// <returns></returns>
	static bool SortGameEntities(const std::shared_ptr<GameEntity>& gameEntity1, const std::shared_ptr<GameEntity>& gameEnt)
	{
		return gameEntity1->GetDrawOrder() < gameEnt->GetDrawOrder();
	}
};


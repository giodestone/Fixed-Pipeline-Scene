#pragma once

#include "GameEvent.h"

class EventObserver
{
	EventObserver() = default;
	~EventObserver() = default;

protected:
	/// <summary>
	/// What the observer should do when an event is triggered.
	/// </summary>
	/// <param name="gameObject">The game object that has notified it.</param>
	/// <param name="gameEvent">The game event that it was notifed with.</param>
	virtual void OnNotify(GameObject& gameObject, GameEvent gameEvent) = 0;
};
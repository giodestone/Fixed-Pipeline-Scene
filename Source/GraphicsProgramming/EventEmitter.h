#pragma once
#include <vector>
#include <memory>

#ifdef _DEBUG
#include <assert.h>
#endif // DEBUG

#include "EventObserver.h"

class EventEmitter
{
	EventEmitter() = default;
	~EventEmitter() = default;

	std::vector<std::weak_ptr<EventObserver>> observers; //Store a reference to the observers. Using weak pointer because it seems safer than a regular one.

public:
	/// <summary>
	/// Add an observer to the events that get sent out.
	/// </summary>
	/// <param name="observer">The observer that should be added.</param>
	void AddObserver(std::weak_ptr<EventObserver> observer)
	{
		observers.push_back(observer);
	}

	/// <summary>
	/// Remove an observer.
	/// </summary>
	/// <param name="observer">The observer that should be removed.</param>
	void RemoveObserver(std::weak_ptr<EventObserver> observer)
	{
		//Find where the observer is in the list
#ifdef _DEBUG
		assert(observers.size() > static_cast<size_t>(0)); //verify that the array is not empty
#endif
		
		size_t i = 0;
		for (auto& it : observers)
		{
			i++;
			if (std::addressof(it) == std::addressof(observer))
			{
				break;
			}
		}

		//Now delete it
		observers.erase(observers.begin() + i);
	}
};
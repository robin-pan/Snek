#pragma once

#include <vector>
#include "observer.h"
#include "info.h"

class Subject {
	std::vector<Observer*> _observers;

public:
	void registerObserver(Observer& newObserver);
	void notifyObservers();
	virtual Info getInfo() = 0;
};
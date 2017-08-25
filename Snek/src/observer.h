#pragma once

#include <vector>

class Subject;

class Observer {
public:
	virtual void notify(Subject& whoNotified) = 0;
};
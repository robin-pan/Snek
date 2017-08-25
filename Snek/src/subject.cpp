#include "subject.h"
#include <vector>
#include <tuple>

void Subject::registerObserver(Observer& newObserver) {
	_observers.push_back(&newObserver);
}

void Subject::notifyObservers() {
	for (auto& observer : _observers) {
		observer->notify(*this);
	}
}
#include "TripManager.h"

using namespace std;

// CLASS: TRIPMANAGER

void TRIPMANAGER::addTrip(const TRIP &trip) {
    trips.push_back(trip);
    notifyTripAdded(trip.getID());
}

bool TRIPMANAGER::removeTrip(const string &tripID) {
    for (size_t i = 0; i < trips.size(); ++i) {
        if (trips[i].getID() == tripID) {
            trips.erase(trips.begin() + i);
            notifyTripRemoved(tripID);
            return true;
        }
    }
    return false;
}

bool TRIPMANAGER::updateTrip(const TRIP &originalTrip, const TRIP &updatedTrip) {
    for (size_t i = 0; i < trips.size(); ++i) {
        if (trips[i].getID() == originalTrip.getID()) {
            trips[i] = updatedTrip;
            notifyTripUpdated(updatedTrip.getID());
            return true;
        }
    }
    return false;
}

const vector<TRIP> &TRIPMANAGER::getAllTrips() const { return trips; }

TRIP *TRIPMANAGER::findTripById(const string &id) {
    for (size_t i = 0; i < trips.size(); ++i) {
        if (trips[i].getID() == id) {
            return &trips[i];
        }
    }
    return nullptr;
}

size_t TRIPMANAGER::getTripCount() const { return trips.size(); }
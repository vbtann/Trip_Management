#include "header.h"

using namespace std;

// FUNC: Constructors
MEMBER::MEMBER() : PERSON(), emergencyContact(""), hasDriverLicense(false), totalSpent(0.0) {}

MEMBER::MEMBER(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob)
    : PERSON(_id, _fullName, _gender, _dob) {}

// FUNC: Getters
vector<string> MEMBER::getJoinedTripIDs() const { return this->joinedTripID; }

string MEMBER::getLastJoinedTripID() const {
    if (this->joinedTripID.empty()) {
        return "";
    }
    return this->joinedTripID[joinedTripID.size() - 1];
}

int MEMBER::getJoinedTripCount() const { return this->joinedTripID.size(); }

string MEMBER::getEmergencyContact() const { return this->emergencyContact; }

bool MEMBER::getHasDriverLicense() const { return this->hasDriverLicense; }

vector<string> MEMBER::getInterests() const { return this->interests; }

double MEMBER::getTotalSpent() const { return this->totalSpent; }

string MEMBER::getRole() const { return "Member"; }

string MEMBER::getInfo() const {
    return this->fullName + " - " + to_string(this->getAge()) + " - " + genderToString(this->gender);
}

// FUNC: Setters
void MEMBER::setEmergencyContact(const string &_contact) { this->emergencyContact = _contact; }

// FUNC: Utility methods
void MEMBER::addToTotalSpent(const double _amount) { this->totalSpent += _amount; }

void MEMBER::addInterest(const string &_interest) {
    if (find(this->interests.begin(), this->interests.end(), _interest) == this->interests.end()) {
        this->interests.push_back(_interest);
    }
}

// NOTE: Check if member has joined this trip or not
bool MEMBER::hasJoinedTrip(const string &_tripID) {
    return find(this->joinedTripID.begin(), this->joinedTripID.end(), _tripID) != joinedTripID.end();
}

// NOTE: Add trip ID to member joined trips list
void MEMBER::joinTrip(const string &_tripID) {
    if (!this->hasJoinedTrip(_tripID)) {
        this->joinedTripID.push_back(_tripID);
    }
}

// NOTE: Remove trip from member joined trips list
void MEMBER::leaveTrip(const string &_tripID) {
    auto it = find(this->joinedTripID.begin(), this->joinedTripID.end(), _tripID);
    if (it != this->joinedTripID.end()) {
        joinedTripID.erase(it);
    }
}

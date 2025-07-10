#include "PersonManager.h"

#include <algorithm>

using namespace std;

PERSONMANAGER::PERSONMANAGER() {
    // Load people from cache file if it exists
    if (peopleCacheFileExists()) {
        loadPeopleCacheFile(people);
    }
}

PERSONMANAGER::~PERSONMANAGER() {
    // Save to cache before destruction
    updatePeopleCacheFile(people);

    // Clean up all allocated persons
    for (PERSON *person : people) {
        delete person;
    }
    people.clear();
}

void PERSONMANAGER::addPerson(PERSON *person) {
    if (person) {
        string personId = person->getID();
        people.push_back(person);

        notifyPersonAdded(personId);

        // Update cache file
        updatePeopleCacheFile(people);
    }
}

bool PERSONMANAGER::removePerson(const string &personID) {
    auto it = find_if(people.begin(), people.end(),
                      [&personID](const PERSON *person) { return person->getID() == personID; });

    if (it != people.end()) {
        delete *it;  // Free the memory
        people.erase(it);

        notifyPersonRemoved(personID);

        // Update cache file
        updatePeopleCacheFile(people);
        return true;
    }
    return false;
}

bool PERSONMANAGER::updatePerson(const PERSON &originalPerson, const PERSON &updatedPerson) {
    auto it = find_if(people.begin(), people.end(),
                      [&originalPerson](const PERSON *person) { return person->getID() == originalPerson.getID(); });

    if (it != people.end()) {
        // Delete the old person
        delete *it;

        // Create new person based on updated data
        if (updatedPerson.getRole() == "Member") {
            const MEMBER &updatedMember = static_cast<const MEMBER &>(updatedPerson);
            *it = new MEMBER(updatedMember);
        } else if (updatedPerson.getRole() == "Host") {
            const HOST &updatedHost = static_cast<const HOST &>(updatedPerson);
            *it = new HOST(updatedHost);
        }

        // CHANGE: Use inherited method from SUBJECT
        notifyPersonUpdated(updatedPerson.getID());

        // Update cache file
        updatePeopleCacheFile(people);
        return true;
    }
    return false;
}

PERSON *PERSONMANAGER::findPersonById(const string &id) {
    auto it = find_if(people.begin(), people.end(), [&id](const PERSON *person) { return person->getID() == id; });

    return (it != people.end()) ? *it : nullptr;
}

vector<PERSON *> PERSONMANAGER::getAllPeople() const {
    return people;  // Can return directly since it's already vector<PERSON*>
}

vector<MEMBER *> PERSONMANAGER::getAllMembers() const {
    vector<MEMBER *> members;
    for (PERSON *person : people) {
        if (person->getRole() == "Member") {
            members.push_back(static_cast<MEMBER *>(person));
        }
    }
    return members;
}

vector<HOST *> PERSONMANAGER::getAllHosts() const {
    vector<HOST *> hosts;
    for (PERSON *person : people) {
        if (person->getRole() == "Host") {
            hosts.push_back(static_cast<HOST *>(person));
        }
    }
    return hosts;
}

size_t PERSONMANAGER::getPersonCount() const { return people.size(); }

size_t PERSONMANAGER::getMemberCount() const { return getAllMembers().size(); }

size_t PERSONMANAGER::getHostCount() const { return getAllHosts().size(); }
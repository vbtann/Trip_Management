#include "PersonManager.h"

#include <QDebug>
#include <algorithm>

using namespace std;

PERSONMANAGER::PERSONMANAGER() : peopleNeedsUpdate(true) {
    // Load people from cache file if it exists
    if (peopleCacheFileExists()) {
        loadPeopleCacheFile(members, hosts);  // Load into separate vectors
        refreshPeopleVector();                // Update composite vector
    }
    qDebug() << "PersonManager initialized with" << members.size() << "members and" << hosts.size() << "hosts";
}

PERSONMANAGER::~PERSONMANAGER() {
    // Save to cache before destruction
    updatePeopleCacheFile(members, hosts);  // Save separate vectors
    qDebug() << "PersonManager destroyed, saved" << getPersonCount() << "people to cache";
}

// FUNC: Add person (delegates to appropriate vector)
void PERSONMANAGER::addPerson(const PERSON &person) {
    if (person.getRole() == "Member") {
        const MEMBER &member = static_cast<const MEMBER &>(person);
        addMember(member);
    } else if (person.getRole() == "Host") {
        const HOST &host = static_cast<const HOST &>(person);
        addHost(host);
    }
}

// FUNC: Add member directly
void PERSONMANAGER::addMember(const MEMBER &member) {
    members.push_back(member);
    peopleNeedsUpdate = true;

    notifyPersonAdded(member.getID());
    updatePeopleCacheFile(members, hosts);
    qDebug() << "Added member:" << QString::fromStdString(member.getFullName());
}

// FUNC: Add host directly
void PERSONMANAGER::addHost(const HOST &host) {
    hosts.push_back(host);
    peopleNeedsUpdate = true;

    notifyPersonAdded(host.getID());
    updatePeopleCacheFile(members, hosts);
    qDebug() << "Added host:" << QString::fromStdString(host.getFullName());
}

// FUNC: Remove person (searches both vectors)
bool PERSONMANAGER::removePerson(const string &personID) {
    // Try removing from members first
    if (removeMember(personID)) {
        return true;
    }
    // Try removing from hosts
    return removeHost(personID);
}

// FUNC: Remove member directly
bool PERSONMANAGER::removeMember(const string &memberID) {
    auto it = find_if(members.begin(), members.end(),
                      [&memberID](const MEMBER &member) { return member.getID() == memberID; });

    if (it != members.end()) {
        members.erase(it);
        peopleNeedsUpdate = true;

        notifyPersonRemoved(memberID);
        updatePeopleCacheFile(members, hosts);
        qDebug() << "Removed member:" << QString::fromStdString(memberID);
        return true;
    }
    return false;
}

// FUNC: Remove host directly
bool PERSONMANAGER::removeHost(const string &hostID) {
    auto it = find_if(hosts.begin(), hosts.end(), [&hostID](const HOST &host) { return host.getID() == hostID; });

    if (it != hosts.end()) {
        hosts.erase(it);
        peopleNeedsUpdate = true;

        notifyPersonRemoved(hostID);
        updatePeopleCacheFile(members, hosts);
        qDebug() << "Removed host:" << QString::fromStdString(hostID);
        return true;
    }
    return false;
}

// FUNC: Update person (delegates to appropriate vector)
bool PERSONMANAGER::updatePerson(const PERSON &originalPerson, const PERSON &updatedPerson) {
    if (originalPerson.getRole() == "Member" && updatedPerson.getRole() == "Member") {
        return updateMember(static_cast<const MEMBER &>(originalPerson), static_cast<const MEMBER &>(updatedPerson));
    } else if (originalPerson.getRole() == "Host" && updatedPerson.getRole() == "Host") {
        return updateHost(static_cast<const HOST &>(originalPerson), static_cast<const HOST &>(updatedPerson));
    } else {
        // Role change - remove from old and add to new
        if (removePerson(originalPerson.getID())) {
            addPerson(updatedPerson);
            return true;
        }
    }
    return false;
}

// FUNC: Update member directly
bool PERSONMANAGER::updateMember(const MEMBER &originalMember, const MEMBER &updatedMember) {
    auto it = find_if(members.begin(), members.end(),
                      [&originalMember](const MEMBER &member) { return member.getID() == originalMember.getID(); });

    if (it != members.end()) {
        *it = updatedMember;
        peopleNeedsUpdate = true;

        notifyPersonUpdated(updatedMember.getID());
        updatePeopleCacheFile(members, hosts);
        qDebug() << "Updated member:" << QString::fromStdString(updatedMember.getID());
        return true;
    }
    return false;
}

// FUNC: Update host directly
bool PERSONMANAGER::updateHost(const HOST &originalHost, const HOST &updatedHost) {
    auto it = find_if(hosts.begin(), hosts.end(),
                      [&originalHost](const HOST &host) { return host.getID() == originalHost.getID(); });

    if (it != hosts.end()) {
        *it = updatedHost;
        peopleNeedsUpdate = true;

        notifyPersonUpdated(updatedHost.getID());
        updatePeopleCacheFile(members, hosts);
        qDebug() << "Updated host:" << QString::fromStdString(updatedHost.getID());
        return true;
    }
    return false;
}

// FUNC: Update people vector (legacy compatibility)
bool PERSONMANAGER::updatePeople(const vector<PERSON> &updatedPeople) {
    // Clear existing data
    members.clear();
    hosts.clear();

    // Redistribute to appropriate vectors
    for (const PERSON &person : updatedPeople) {
        if (person.getRole() == "Member") {
            members.push_back(static_cast<const MEMBER &>(person));
        } else if (person.getRole() == "Host") {
            hosts.push_back(static_cast<const HOST &>(person));
        }
    }

    peopleNeedsUpdate = true;
    updatePeopleCacheFile(members, hosts);
    return true;
}

// FUNC: Find person by ID (searches both vectors, updates composite if needed)
PERSON *PERSONMANAGER::findPersonById(const string &id) {
    // First try to find in members
    MEMBER *member = findMemberById(id);
    if (member) {
        return member;
    }

    // Then try to find in hosts
    HOST *host = findHostById(id);
    if (host) {
        return host;
    }

    return nullptr;
}

// FUNC: Find member by ID
MEMBER *PERSONMANAGER::findMemberById(const string &id) {
    auto it = find_if(members.begin(), members.end(), [&id](const MEMBER &member) { return member.getID() == id; });
    return (it != members.end()) ? &(*it) : nullptr;
}

// FUNC: Find host by ID
HOST *PERSONMANAGER::findHostById(const string &id) {
    auto it = find_if(hosts.begin(), hosts.end(), [&id](const HOST &host) { return host.getID() == id; });
    return (it != hosts.end()) ? &(*it) : nullptr;
}

// FUNC: Get all people (composite view)
const vector<PERSON> &PERSONMANAGER::getAllPeople() const {
    if (peopleNeedsUpdate) {
        refreshPeopleVector();
    }
    return people;
}

// FUNC: Get all members (direct access)
const vector<MEMBER> &PERSONMANAGER::getAllMembers() const { return members; }

// FUNC: Get all hosts (direct access)
const vector<HOST> &PERSONMANAGER::getAllHosts() const { return hosts; }

// FUNC: Get all members (legacy - returns copy)
vector<MEMBER> PERSONMANAGER::getAllMembersLegacy() const {
    return members;  // Return copy
}

// FUNC: Get all hosts (legacy - returns copy)
vector<HOST> PERSONMANAGER::getAllHostsLegacy() const {
    return hosts;  // Return copy
}

// FUNC: Refresh people vector (maintains composite view)
void PERSONMANAGER::refreshPeopleVector() const {
    people.clear();
    people.reserve(members.size() + hosts.size());

    // Add all members
    for (const MEMBER &member : members) {
        people.push_back(member);
    }

    // Add all hosts
    for (const HOST &host : hosts) {
        people.push_back(host);
    }

    peopleNeedsUpdate = false;
    qDebug() << "Refreshed people vector:" << people.size() << "total people";
}

// FUNC: Get count functions
size_t PERSONMANAGER::getPersonCount() const {
    if (peopleNeedsUpdate) {
        refreshPeopleVector();
    }
    return people.size();
}

size_t PERSONMANAGER::getMemberCount() const { return members.size(); }

size_t PERSONMANAGER::getHostCount() const { return hosts.size(); }

// FUNC: Get host by ID (returns copy)
HOST PERSONMANAGER::getHostByID(const string &hostID) {
    HOST *host = findHostById(hostID);
    return host ? *host : HOST();
}

// FUNC: Get member by ID (returns copy)
MEMBER PERSONMANAGER::getMemberByID(const string &memberID) {
    MEMBER *member = findMemberById(memberID);
    return member ? *member : MEMBER();
}

// FUNC: Debugging and validation helpers
void PERSONMANAGER::debugPrintCounts() const {
    qDebug() << "PersonManager Debug Counts:";
    qDebug() << "  Members:" << members.size();
    qDebug() << "  Hosts:" << hosts.size();
    qDebug() << "  Total People:" << getPersonCount();
    qDebug() << "  People needs update:" << peopleNeedsUpdate;
}

bool PERSONMANAGER::validateDataIntegrity() const {
    bool valid = true;

    // Check for duplicate IDs within members
    for (size_t i = 0; i < members.size(); ++i) {
        for (size_t j = i + 1; j < members.size(); ++j) {
            if (members[i].getID() == members[j].getID()) {
                qDebug() << "Duplicate member ID found:" << QString::fromStdString(members[i].getID());
                valid = false;
            }
        }
    }

    // Check for duplicate IDs within hosts
    for (size_t i = 0; i < hosts.size(); ++i) {
        for (size_t j = i + 1; j < hosts.size(); ++j) {
            if (hosts[i].getID() == hosts[j].getID()) {
                qDebug() << "Duplicate host ID found:" << QString::fromStdString(hosts[i].getID());
                valid = false;
            }
        }
    }

    // Check for IDs that exist in both vectors
    for (const MEMBER &member : members) {
        for (const HOST &host : hosts) {
            if (member.getID() == host.getID()) {
                qDebug() << "ID exists in both vectors:" << QString::fromStdString(member.getID());
                valid = false;
            }
        }
    }

    return valid;
}
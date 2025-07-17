#ifndef PERSONMANAGER_H
#define PERSONMANAGER_H

#include <string>
#include <vector>

#include "../Models/header.h"
#include "FileManager.h"
#include "Observer.h"

using namespace std;

// Forward declarations
class OBSERVER;

class PERSONMANAGER : public SUBJECT {
   private:
    // Separate vectors for better maintenance and debugging
    vector<MEMBER> members;  // Store members separately
    vector<HOST> hosts;      // Store hosts separately

    // Keep original vector for compatibility (mutable for lazy update)
    mutable vector<PERSON> people;   // Composite view of all people
    mutable bool peopleNeedsUpdate;  // Flag to track if people vector needs refresh

   public:
    PERSONMANAGER();
    ~PERSONMANAGER();  // Need explicit destructor to clean up

    // Core management functions
    void addPerson(const PERSON &person);
    void addMember(const MEMBER &member);  // NEW: Direct member addition
    void addHost(const HOST &host);        // NEW: Direct host addition

    bool removePerson(const string &personID);
    bool removeMember(const string &memberID);  // NEW: Direct member removal
    bool removeHost(const string &hostID);      // NEW: Direct host removal

    bool updatePerson(const PERSON &originalPerson, const PERSON &updatedPerson);
    bool updateMember(const MEMBER &originalMember, const MEMBER &updatedMember);  // NEW: Direct member update
    bool updateHost(const HOST &originalHost, const HOST &updatedHost);            // NEW: Direct host update
    bool updatePeople(const vector<PERSON> &updatedPeople);

    // Search and retrieval functions
    PERSON *findPersonById(const string &id);  // Returns pointer for compatibility, but null if not found
    MEMBER *findMemberById(const string &id);  // NEW: Direct member search
    HOST *findHostById(const string &id);      // NEW: Direct host search

    const vector<PERSON> &getAllPeople() const;   // Returns composite view
    const vector<MEMBER> &getAllMembers() const;  // NEW: Direct access to members
    const vector<HOST> &getAllHosts() const;      // NEW: Direct access to hosts

    // Legacy versions for backward compatibility
    vector<MEMBER> getAllMembersLegacy() const;  // Returns copy like before
    vector<HOST> getAllHostsLegacy() const;      // Returns copy like before

    // Utility functions
    size_t getPersonCount() const;
    size_t getMemberCount() const;
    size_t getHostCount() const;

    // ID-based lookups
    HOST getHostByID(const string &hostID);
    MEMBER getMemberByID(const string &memberID);

    // Bulk operations - NEW
    void addMultipleMembers(const vector<MEMBER> &newMembers);
    void addMultipleHosts(const vector<HOST> &newHosts);
    void clearAllMembers();  // NEW: Clear all members
    void clearAllHosts();    // NEW: Clear all hosts
    void clearAll();         // NEW: Clear everything

    // Maintenance and debugging functions - NEW
    void refreshPeopleVector() const;    // Force refresh of composite vector
    bool validateDataIntegrity() const;  // Check data consistency
    void debugPrintCounts() const;       // Print counts for debugging

    // Import/Export helpers - NEW
    void loadFromSeparateVectors(const vector<MEMBER> &importedMembers, const vector<HOST> &importedHosts);
    void exportToSeparateVectors(vector<MEMBER> &exportMembers, vector<HOST> &exportHosts) const;
};

#endif  // PERSONMANAGER_H
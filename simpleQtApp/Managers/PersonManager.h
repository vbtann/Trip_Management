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
    vector<PERSON *> people;  // Use raw pointers instead of smart pointers

   public:
    PERSONMANAGER();
    ~PERSONMANAGER();  // Need explicit destructor to clean up

    // Core management functions
    void addPerson(PERSON *person);
    bool removePerson(const string &personID);
    bool updatePerson(const PERSON &originalPerson, const PERSON &updatedPerson);

    // Search and retrieval functions
    PERSON *findPersonById(const string &id);
    vector<PERSON *> getAllPeople() const;
    vector<MEMBER *> getAllMembers() const;
    vector<HOST *> getAllHosts() const;

    // Utility functions
    size_t getPersonCount() const;
    size_t getMemberCount() const;
    size_t getHostCount() const;
};

#endif  // PERSONMANAGER_H
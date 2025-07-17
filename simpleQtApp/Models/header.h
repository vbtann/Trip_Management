#ifndef HEADER_H
#define HEADER_H

#define enl "\n"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class PERSON;
class MEMBER;
class HOST;

// FUNC: toUpper
string toUpper(const string &str);

// CLASS: STATUS
enum class STATUS { Planned, Ongoing, Completed, Cancelled };

const string statusToString(STATUS status);
STATUS stringToStatus(const string &statusStr);

// CLASS: GENDER
enum class GENDER { Male, Female };

const string genderToString(GENDER gender);
GENDER stringToGender(const string &genderString);

// CLASS: DATE
class DATE {
   private:
    int day, month, year;

   public:
    DATE();
    DATE(int _day, int _month, int _year);
    DATE(const DATE &date);
    ~DATE() = default;

    // FUNC: Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // FUNC: Setters
    void setDate(int _day, int _month, int _year);

    // FUNC: Utility methods
    string toString() const;

    DATE &operator=(const DATE &other);
    bool operator<(const DATE &rhs) const;
    friend ostream &operator<<(ostream &, const DATE &);
};

// CLASS: PERSON
class PERSON {
   protected:
    string fullName, ID;
    string email, phoneNumber, address;
    GENDER gender;
    DATE dateOfBirth;

   public:
    PERSON();
    PERSON(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob);

    virtual ~PERSON() = default;
    virtual string getRole() const;
    virtual string getInfo() const;

    // FUNC: Getters
    string getFullName() const;
    string getID() const;
    string getEmail() const;
    string getPhoneNumber() const;
    string getAddress() const;
    GENDER getGender() const;
    DATE getDateOfBirth() const;
    string toString() const;

    // FUNC: Setters
    void setFullName(const string &_fullName);
    void setEmail(const string &_email);
    void setPhoneNumber(const string &_phoneNumber);
    void setAddress(const string &_address);
    void setGender(const GENDER &_gender);
    void setDOB(const DATE &_dob);
    void setID(const string &_ID);

    // FUNC: Utility methods
    int getAge() const;
    string getContactInfo() const;
};

// CLASS: MEMBER
class MEMBER : public PERSON {
   private:
    vector<string> joinedTripID;
    string emergencyContact;
    bool hasDriverLicense;
    vector<string> interests;
    double totalSpent;

   public:
    MEMBER();
    MEMBER(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob);

    // FUNC: Getters
    vector<string> getJoinedTripIDs() const;
    string getLastJoinedTripID() const;
    int getJoinedTripCount() const;
    string getEmergencyContact() const;
    bool getHasDriverLicense() const;
    vector<string> getInterests() const;
    double getTotalSpent() const;
    string getRole() const override;
    string getInfo() const override;

    // FUNC: Setters
    void setEmergencyContact(const string &_contact);

    // FUNC: Utility methods
    void addToTotalSpent(const double _amount);
    void addInterest(const string &_interest);
    void joinTrip(const string &_tripID);
    void leaveTrip(const string &_tripID);
    bool hasJoinedTrip(const string &_tripID);
};

// CLASS: HOST
class HOST : public PERSON {
   private:
    vector<string> hostedTripID;
    string emergencyContact;

   public:
    HOST();
    HOST(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob);

    // FUNC: Getters
    vector<string> getHostedTripIDs() const;
    string getEmergencyContact() const;
    string getRole() const override;
    string getInfo() const override;

    // FUNC: Setters
    void setEmergencyContact(const string &_contact);

    // FUNC: Utility methods
    bool hasHostedTrip(const string &_tripID);
    void hostTrip(const string &_tripID);
};
// CLASS: TRIP
class TRIP {
   private:
    static int tripCount;
    string ID, Destination, Description;
    DATE startDate, endDate;
    STATUS status;
    vector<MEMBER> members;
    HOST host;

   public:
    // NOTE: Constructors
    TRIP();
    TRIP(const TRIP &other);
    TRIP(const string &_tripID, const string &_dest, const string &_desc, int _startDay, int _startMonth,
         int _startYear, int _endDay, int _endMonth, int _endYear, const STATUS &_status);
    TRIP(const string &_tripID, const string &_dest, const string &_desc, const DATE &_startDate, const DATE &_endDate,
         const STATUS &_status);

    // ~TRIP() { tripCount--; }

    // NOTE: Getters
    string idProcess() const;
    string getID() const;
    string getDestination() const;
    string getDescription() const;
    DATE getStartDate() const;
    DATE getEndDate() const;
    STATUS getStatus() const;
    string getStatusString() const;
    static int getTripCount();

    HOST getHost() const;
    vector<MEMBER> getMembers() const;

    // NOTE: Setters
    void setID(const string &_ID);
    void setDestination(const string &_destination);
    void setDescription(const string &_description);
    void setStartDate(const DATE &_startDate);
    void setEndDate(const DATE &_endDate);
    void setStatus(const STATUS &_status);

    // FUNC: Utility methods
    void addMember(MEMBER member);
    void setMembers(const vector<MEMBER> &members);
    void setHost(HOST _host);
    bool hasHost() const;

    TRIP &operator=(const TRIP &other);
    friend ostream &operator<<(ostream &, const TRIP &);
};

#endif  // HEADER_H

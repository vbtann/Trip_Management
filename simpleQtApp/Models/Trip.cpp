#include "header.h"

int TRIP::tripCount = 0;

// FUNC: Constructors
TRIP::TRIP()
    : ID(""), Destination(""), Description(""), startDate(1, 1, 2024), endDate(1, 1, 2024), status(STATUS::Planned) {
    tripCount++;
}

TRIP::TRIP(const TRIP &other)
    : ID(other.ID),
      Destination(other.Destination),
      Description(other.Description),
      startDate(other.startDate),
      endDate(other.endDate),
      status(other.status),
      members(other.members),
      host(other.host) {}

TRIP::TRIP(const string &_tripID, const string &_dest, const string &_desc, int _startDay, int _startMonth,
           int _startYear, int _endDay, int _endMonth, int _endYear, const STATUS &_status)
    : ID(_tripID),  // Use the provided ID directly
      Destination(toUpper(_dest)),
      Description(_desc),
      startDate(_startDay, _startMonth, _startYear),
      endDate(_endDay, _endMonth, _endYear),
      status(_status) {
    tripCount++;

    // If no ID provided, generate one
    // if (ID.empty()) {
    //     ID = this->idProcess();
    // }
}

TRIP::TRIP(const string &_tripID, const string &_dest, const string &_desc, const DATE &_startDate,
           const DATE &_endDate, const STATUS &_status)
    : ID(_tripID),  // Use the provided ID directly
      Destination(_dest),
      Description(_desc),
      startDate(_startDate),
      endDate(_endDate),
      status(_status) {
    tripCount++;

    // If no ID provided, generate one
    if (ID.empty()) {
        ID = this->idProcess();
    }
}

// FUNC: Process the ID (get the first letter of each words in Destination)
string TRIP::idProcess() const {
    if (Destination.empty()) {
        return "TRIP_" + to_string(tripCount);
    }

    string id;
    string dest = this->Destination;

    // Get first letter
    if (!dest.empty()) {
        id += dest[0];
    }

    // Get first letter of each word (skip first word since we already have its
    // first letter)
    for (size_t i = 1; i < dest.length(); i++) {  // Start from 1, not 0
        if (dest[i - 1] == ' ' && i < dest.length()) {
            id += dest[i];
        }
    }

    // Add date suffix
    int raw_d = this->startDate.getDay();
    int raw_m = this->startDate.getMonth();

    string d = (raw_d < 10) ? "0" + to_string(raw_d) : to_string(raw_d);
    string m = (raw_m < 10) ? "0" + to_string(raw_m) : to_string(raw_m);

    // Convert to uppercase
    for (char &c : id) {
        c = toupper(c);
    }

    id = id + "_" + m + d;
    return id;
}

// FUNC: Getters
string TRIP::getID() const { return this->ID; }

string TRIP::getDestination() const { return this->Destination; }

string TRIP::getDescription() const { return this->Description; }

DATE TRIP::getStartDate() const { return this->startDate; }

DATE TRIP::getEndDate() const { return this->endDate; }

STATUS TRIP::getStatus() const { return this->status; }

string TRIP::getStatusString() const {
    switch (this->status) {
        case STATUS::Planned:
            return "Planned";
        case STATUS::Ongoing:
            return "Ongoing";
        case STATUS::Completed:
            return "Completed";
        case STATUS::Cancelled:
            return "Cancelled";
        default:
            return "Unknown";
    }
}

HOST TRIP::getHost() const { return this->host; }

vector<MEMBER> TRIP::getMembers() const { return this->members; }

// int TRIP::getTripCount() { return tripCount; }

// FUNC: Setters
void TRIP::setID(const std::string &_ID) { this->ID = _ID; }

void TRIP::setDestination(const std::string &_destination) { this->Destination = _destination; }

void TRIP::setDescription(const string &_description) { this->Description = _description; }

void TRIP::setStartDate(const DATE &_startDate) { this->startDate = _startDate; }

void TRIP::setEndDate(const DATE &_endDate) { this->endDate = _endDate; }

void TRIP::setStatus(const STATUS &_status) { this->status = _status; }

// FUNC: Utility methods
void TRIP::addMember(MEMBER member) {
    auto it = find_if(members.begin(), members.end(),
                      [&](const MEMBER &_member) { return member.getID() == _member.getID(); });

    // Add member to trip if not exist
    if (it == members.end()) {
        members.push_back(member);
        member.joinTrip(this->getID());
    }
}

void TRIP::setMembers(const vector<MEMBER> &members) { this->members = members; }

void TRIP::setHost(HOST _host) {
    this->host = _host;
    _host.hostTrip(this->getID());
}

bool TRIP::hasHost() const { return !this->host.getID().empty(); }

TRIP &TRIP::operator=(const TRIP &other) {
    if (this != &other) {
        this->ID = other.ID;
        this->Description = other.Description;
        this->Destination = other.Destination;
        this->startDate = other.startDate;
        this->endDate = other.endDate;
        this->status = other.status;
        this->members = other.members;
        this->host = other.host;
    }

    return *this;
}

// FUNC: Output
ostream &operator<<(ostream &os, const TRIP &trip) {
    os << trip.ID << endl
       << trip.Destination << endl
       << trip.Description << endl
       << trip.startDate << endl
       << trip.endDate << endl
       << trip.getStatusString() << endl;

    return os;
}
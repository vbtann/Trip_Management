#include "header.h"

Trip::Trip() : ID(""), Destination(""), startDate(), status(), Description("") {
     tripCount++;
};

Trip::Trip(const string &_tripID, const string &_dest, const string &_desc,
           int _startDay, int _startMonth, int _startYear, int _endDay,
           int _endMonth, int _endYear, const Status &_status)
    : ID(_tripID),
      Destination(_dest),
      Description(_desc),
      startDate(_startDay, _startMonth, _startYear),
      endDate(_endDay, _endMonth, _endYear),
      status(_status) {
     tripCount++;
}

Trip::Trip(const string &_tripID, const string &_dest, const string &_desc,
           const Date &_startDate, const Date &_endDate, const Status &_status)
    : ID(_tripID),
      Destination(_dest),
      Description(_desc),
      startDate(_startDate),
      endDate(_endDate),
      status(_status) {
     tripCount++;
}

string Trip::getID() const { return this->ID; }

string Trip::getDestination() const { return this->Destination; }

string Trip::getDescription() const { return this->Description; }

Date Trip::getStartDate() const { return this->startDate; }

Date Trip::getEndDate() const { return this->endDate; }

Status Trip::getStatus() const { return this->status; }

string Trip::getStatusString() const {
     switch (this->status) {
          case Status::Planned:
               return "Planned";
          case Status::Ongoing:
               return "Ongoing";
          case Status::Completed:
               return "Completed";
          case Status::Cancelled:
               return "Cancelled";
          default:
               return "Unknown";
     }
}

int Trip::getTripCount() { return tripCount; }

void Trip::setID(const std::string &_ID) { this->ID = _ID; }

void Trip::setDestination(const std::string &_destination) {
     this->Destination = _destination;
}

ostream &operator<<(ostream &os, const Trip &trip) {
     os << trip.ID << endl
        << trip.Destination << endl
        << trip.Description << endl
        << trip.startDate << endl
        << trip.endDate << endl
        << trip.getStatusString() << endl;

     return os;
}
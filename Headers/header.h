#pragma once

#define enl "\n"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

enum class Status { Planned, Ongoing, Completed, Cancelled };

string statusToString(Status status);
Status stringToStatus(const string &statusStr);

class Date {
    private:
     int day, month, year;

    public:
     Date();
     Date(int _day, int _month, int _year);
     Date(const Date &date);

     // NOTE: Getters
     int getDay() const;
     int getMonth() const;
     int getYear() const;

     // NOTE: Setters
     void setDate(int _day, int _month, int _year);

     string dateToString() const;

     friend ostream &operator<<(ostream &, const Date &);
};

class Trip {
    private:
     static int tripCount;
     string ID, Destination, Description;
     Date startDate, endDate;
     Status status;

    public:
     // NOTE: Constructors
     Trip();
     Trip(const string &_tripID, const string &_dest, const string &_desc,
          int _startDay, int _startMonth, int _startYear, int _endDay,
          int _endMonth, int _endYear, const Status &_status);
     Trip(const string &_tripID, const string &_dest, const string &_desc,
          const Date &_startDate, const Date &_endDate, const Status &_status);

     // ~Trip() { tripCount--; }

     // NOTE: Getters
     string getID() const;
     string getDestination() const;
     string getDescription() const;
     Date getStartDate() const;
     Date getEndDate() const;
     Status getStatus() const;
     string getStatusString() const;
     static int getTripCount();

     // NOTE: Setters
     void setID(const string &_ID);
     void setDestination(const string &_destination);

     friend ostream &operator<<(ostream &, const Trip &);
};

class Friend {
    private:
     string fullName, firstName, lastName;

     Date dateOfBirth;
};
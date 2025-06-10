#include "TripManager.h"

Date extractDate(string _date) {
     int _day, _month, _year;

     stringstream ss1(_date);
     string token;
     getline(ss1, token, '/');
     _day = stoi(token);
     getline(ss1, token, '/');
     _month = stoi(token);
     getline(ss1, token);
     _year = stoi(token);

     Date date(_day, _month, _year);

     return date;
}

// NOTE: Input new trip from console
void createNewTrip(vector<Trip> &Trips) {
     string _ID, _dest, _desc, _startDate, _endDate, _status;
     Date _stDate, _enDate;

     cout << "Enter trip ID: ";
     getline(cin, _ID);
     cout << "Enter destination: ";
     getline(cin, _dest);
     cout << "Enter description: ";
     getline(cin, _desc);
     cout << "Enter starting date (DD/MM/YYYY): ";
     getline(cin, _startDate);
     cout << "Enter ending date (DD/MM/YYYY): ";
     getline(cin, _endDate);
     cout << "Is the trip in progress (Yes / No): ";
     getline(cin, _status);

     Trip trip(_ID, _dest, _desc, extractDate(_startDate),
               extractDate(_endDate), stringToStatus(_status));

     Trips.push_back(trip);
}

// NOTE: Import from TXT file
void importTripInfo(vector<Trip> &Trips, const string inputFilePath) {
     string _ID, _dest, _desc, _startDate, _endDate, _status;
     Date _stDate, _enDate;

     ifstream input(inputFilePath);

     if (!input.is_open()) {
          cout << "Error: Could not open Trips_info.txt" << endl;
          return;
     }

     while (getline(input, _ID)) {
          getline(input, _dest);
          getline(input, _desc);
          getline(input, _startDate);
          getline(input, _endDate);
          getline(input, _status);

          Trip trip(_ID, _dest, _desc, extractDate(_startDate),
                    extractDate(_endDate), stringToStatus(_status));

          Trips.push_back(trip);
     }

     input.close();
     cout << "Trips loaded from file successfully!" << endl;
}

// NOTE: Print to console
void printTrip(vector<Trip> Trips) {
     for (Trip trip : Trips) {
          cout << trip << endl;
     }
}

// NOTE: Export to CSV file
void exportTripsInfo(vector<Trip> Trips, const string outputFilePath) {
     fstream output(outputFilePath, ios::out);

     if (!output.is_open()) {
          cout << "Error: Could not open output file!";
          return;
     }

     output << "ID,Destination,Description,startDate,endDate,status\n";

     for (const Trip &trip : Trips) {
          output << trip.getID() << "," << trip.getDestination() << ","
                 << trip.getDescription() << "," << trip.getStartDate() << ","
                 << trip.getEndDate() << "," << trip.getStatusString() << "\n";
     }

     output.close();

     cout << "Trips exported to CSV successfully!" << enl;
}
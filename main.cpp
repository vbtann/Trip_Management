#include "Functions/functions.h"
#include "Headers/header.h"

using namespace std;

vector<Trip> Trips;

int Trip::tripCount = 0;

int main(void) {
     //  createNewTrip(Trips);
     importTripInfo(Trips, "Trips_info.txt");
     cout << "Trip count: " << Trip::getTripCount() << enl;
     //  printTrip(Trips);
     exportTripsInfo(Trips, "Export.csv");
     return 0;
}
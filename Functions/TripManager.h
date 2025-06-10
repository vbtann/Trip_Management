#pragma once

#include <string>
#include <vector>

#include "../Headers/header.h"

using namespace std;

void createNewTrip(vector<Trip> &Trips);
void printTrip(vector<Trip> Trips);
void importTripInfo(vector<Trip> &Trips, const string inputFilePath);
void exportTripsInfo(vector<Trip> Trips, const string outputFilePath);
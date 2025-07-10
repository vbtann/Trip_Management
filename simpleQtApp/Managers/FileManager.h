// #ifndef TRIPMANAGER_H
// #define TRIPMANAGER_H
#pragma once

#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../Managers/PersonFactory.h"
#include "../Managers/TripFactory.h"
#include "../Models/header.h"
// class TRIP;

using namespace std;

void createNewTrip(vector<TRIP> &Trips);
void printTrip(vector<TRIP> Trips);
void importTripInfo(vector<TRIP> &trips, const string &filepath);
void exportTripsInfo(vector<TRIP> Trips, const string outputFilePath);
void loadCacheFile(vector<TRIP> &trips);
void updateCacheFile(const vector<TRIP> &Trips);
bool cacheFileExists();
QString getCacheFilePath();

void importPeopleInfo(vector<PERSON *> &people, const string &filePath);
void importTripFromCache(vector<TRIP> &trips, const string &filePath);
void exportPeopleInfo(const vector<PERSON *> &people, const string &outputFilePath);
void loadPeopleCacheFile(vector<PERSON *> &people);
void updatePeopleCacheFile(const vector<PERSON *> &people);
bool peopleCacheFileExists();
QString getPeopleCacheFilePath();

// #endif  // TRIPMANAGER_H

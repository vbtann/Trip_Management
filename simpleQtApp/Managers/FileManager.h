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
// #include "../Managers/PersonManager.h"
#include "../Managers/TripFactory.h"
#include "../Models/header.h"
// class TRIP;

class PERSONMANAGER;

using namespace std;

void createNewTrip(vector<TRIP> &Trips);
void printTrip(vector<TRIP> Trips);
void importTripInfo(vector<TRIP> &trips, const string &filepath);
void exportTripsInfo(vector<TRIP> Trips, const string outputFilePath);
void loadCacheFile(vector<TRIP> &trips);
void updateCacheFile(const vector<TRIP> &Trips);
bool cacheFileExists();
QString getCacheFilePath();

void importPeopleInfo(vector<MEMBER> &members, vector<HOST> &hosts, const string &filePath);
void importTripFromCache(vector<TRIP> &trips, const string &filePath);
void exportPeopleInfo(const vector<MEMBER> &members, const vector<HOST> &hosts, const string &outputFilePath);
void loadPeopleCacheFile(vector<MEMBER> &members, vector<HOST> &hosts);
void updatePeopleCacheFile(const vector<MEMBER> &members, const vector<HOST> &hosts);
bool peopleCacheFileExists();
QString getPeopleCacheFilePath();

void restoreTripAttendeesFromCache(vector<TRIP> &trips, PERSONMANAGER *personManager, const string &filePath);
void saveTripAttendeesToCache(const vector<TRIP> &trips, const string &filePath);

// #endif  // TRIPMANAGER_H
#include "FileManager.h"

#include "../Models/header.h"

// Helper function to get cache file path
QString getCacheFilePath() { return "D:/Study/HCMUS/1st year/sem 3/OOP/Project/cache.csv"; }

// Check if cache file exists
bool cacheFileExists() {
    QFileInfo cacheFile(getCacheFilePath());
    return cacheFile.exists();
}

DATE extractDate(string _date) {
    int _day, _month, _year;

    stringstream ss1(_date);
    string token;
    getline(ss1, token, '/');
    _day = stoi(token);
    getline(ss1, token, '/');
    _month = stoi(token);
    getline(ss1, token);
    _year = stoi(token);

    DATE date(_day, _month, _year);
    return date;
}

// FUNC: Input new trip from console
void createNewTrip(vector<TRIP> &Trips) {
    string _ID, _dest, _desc, _startDate, _endDate, _status;
    DATE _stDate, _enDate;

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

    TRIP trip(_ID, _dest, _desc, extractDate(_startDate), extractDate(_endDate), stringToStatus(_status));

    Trips.push_back(trip);
}

// FUNC: Import from CSV file
void importTripInfo(vector<TRIP> &trips, const string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        qDebug() << "Error: Cannot open file for import:" << QString::fromStdString(filePath);
        return;
    }

    std::string line;
    // Skip header line if it exists
    if (std::getline(file, line) &&
        (line.find("ID") != std::string::npos || line.find("Destination") != std::string::npos)) {
        // This was a header line, continue reading
        qDebug() << "Found header line:" << QString::fromStdString(line);
    } else {
        // This wasn't a header, reset file position to read from beginning
        file.clear();
        file.seekg(0);
    }

    // For debugging
    int lineCount = 0;
    int successCount = 0;

    while (std::getline(file, line)) {
        lineCount++;
        if (line.empty()) continue;

        qDebug() << "Processing line" << lineCount << ":" << QString::fromStdString(line);

        // Parse CSV line
        std::vector<std::string> data;
        std::stringstream ss(line);
        std::string token;

        // Better CSV parsing that handles quotes and commas within fields
        bool inQuotes = false;
        std::string field;

        for (char c : line) {
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                data.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }
        // Add the last field
        data.push_back(field);

        try {
            // Check if we have enough data fields (adjust for no ID column)
            if (data.size() >= 5) {  // CHANGED from 6 to 5
                qDebug() << "Found" << data.size() << "fields in line" << lineCount;

                // Extract data - ADJUST INDICES to match your CSV structure
                string destination = data[0];           // Was data[1]
                string description = data[1];           // Was data[2]
                DATE startDate = extractDate(data[2]);  // Was data[3]
                DATE endDate = extractDate(data[3]);    // Was data[4]

                // Create status enum from string
                STATUS status;
                if (data[4] == "Planned") {  // Was data[5]
                    status = STATUS::Planned;
                } else if (data[4] == "Ongoing") {  // Added "Ongoing" option
                    status = STATUS::Ongoing;
                } else if (data[4] == "Completed") {
                    status = STATUS::Completed;
                } else if (data[4] == "Cancelled") {
                    status = STATUS::Cancelled;
                } else {
                    status = STATUS::Planned;  // Default
                }

                // Generate ID based on destination and start date (keep this)
                TRIP newTrip(TRIPFACTORY::generateTripID(destination, startDate), destination, description, startDate,
                             endDate, status);

                // Add to the trips vector
                trips.push_back(newTrip);
                successCount++;
                qDebug() << "Successfully added trip:" << QString::fromStdString(destination)
                         << "with generated ID:" << QString::fromStdString(newTrip.getID());
            } else {
                qDebug() << "Error: Not enough fields in line" << lineCount << ". Expected at least 5, got"
                         << data.size();
            }
        } catch (const std::exception &e) {
            qDebug() << "Error parsing trip data at line" << lineCount << ":" << QString::fromStdString(line) << " - "
                     << QString::fromStdString(e.what());
        }
    }

    file.close();
    qDebug() << "Import complete. Processed" << lineCount << "lines, successfully imported" << successCount << "trips";
}

// FUNC: Print to console
void printTrip(vector<TRIP> Trips) {
    for (const TRIP &trip : Trips) {  // Added const for efficiency
        cout << trip << endl;
    }
}

// FUNC: Export to CSV file
void exportTripsInfo(vector<TRIP> Trips, const string outputFilePath) {
    fstream output(outputFilePath, ios::out);

    if (!output.is_open()) {
        cout << "Error: Could not open output file: " << outputFilePath << endl;
        return;
    }

    // Write header
    output << "ID,Destination,Description,StartDate,EndDate,Status\n";

    // Write trip data
    for (const TRIP &trip : Trips) {
        output << trip.getID() << "," << trip.getDestination() << "," << trip.getDescription() << ","
               << trip.getStartDate() << "," << trip.getEndDate() << "," << trip.getStatusString() << "\n";
    }

    output.close();
}

// FUNC: Update cache file from current trips
void updateCacheFile(const vector<TRIP> &Trips) {
    QString cacheFilePath = getCacheFilePath();

    try {
        exportTripsInfo(Trips, cacheFilePath.toStdString());
        qDebug() << "Cache file updated successfully:" << cacheFilePath;
    } catch (const exception &e) {
        qDebug() << "Error updating cache file:" << e.what();
    }
}

// FUNC: Load trips info from cache file
void loadCacheFile(vector<TRIP> &trips) {
    QString cacheFilePath = getCacheFilePath();

    QFileInfo cacheFile(cacheFilePath);
    if (!cacheFile.exists()) {
        qDebug() << "No cache file found at:" << cacheFilePath;
        return;
    }

    try {
        qDebug() << "Loading trips from cache:" << cacheFilePath;

        // Clear existing trips before loading
        trips.clear();

        // Import from cache file
        importTripFromCache(trips, cacheFilePath.toStdString());

        qDebug() << "Successfully loaded" << trips.size() << "trips from cache";

    } catch (const exception &e) {
        qDebug() << "Error loading cache file:" << e.what();
    }
}

// Helper function to get people cache file path
QString getPeopleCacheFilePath() { return "D:/Study/HCMUS/1st year/sem 3/OOP/Project/people_cache.csv"; }

// Check if people cache file exists
bool peopleCacheFileExists() {
    QFileInfo cacheFile(getPeopleCacheFilePath());
    return cacheFile.exists();
}

// FUNC: Import people from CSV file
void importPeopleInfo(vector<PERSON *> &people, const string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        QMessageBox::critical(nullptr, "Error", "Cannot open file!");
        return;
    }

    std::string line;
    // Skip header line if it exists
    if (std::getline(file, line) &&
        (line.find("FullName") != std::string::npos || line.find("Name") != std::string::npos)) {
        // This was a header line, continue reading
    } else {
        // This wasn't a header, reset file position
        file.clear();
        file.seekg(0);
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Parse CSV line
        size_t pos = 0;
        std::vector<std::string> data;

        while ((pos = line.find(',')) != std::string::npos) {
            data.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        data.push_back(line);  // Last element

        try {
            // Minimum required fields: Name,DOB,Email,Phone,Gender,Address,Role
            if (data.size() >= 7) {
                string fullName = data[0];
                fullName = toUpper(fullName);
                DATE dob = extractDate(data[1]);
                string email = data[2];
                string phone = data[3];
                GENDER gender = stringToGender(data[4]);
                string address = data[5];
                string role = data[6];

                // Create ID based on name and DOB
                string personID = PERSONFACTORY::generatePersonID(fullName, dob);

                if (role == "Member") {
                    MEMBER *member = new MEMBER(personID, fullName, gender, dob);
                    member->setEmail(email);
                    member->setPhoneNumber(phone);
                    member->setAddress(address);

                    // Optional fields for Member
                    if (data.size() > 7) member->setEmergencyContact(data[7]);

                    // Parse interests if available (comma-separated in quotes)
                    if (data.size() > 8) {
                        string interestsStr = data[8];
                        // Remove quotes if present
                        if (interestsStr.front() == '"' && interestsStr.back() == '"') {
                            interestsStr = interestsStr.substr(1, interestsStr.length() - 2);
                        }

                        stringstream ss(interestsStr);
                        string interest;
                        while (getline(ss, interest, ';')) {
                            member->addInterest(interest);
                        }
                    }

                    // Parse total spent if available
                    if (data.size() > 9) {
                        try {
                            double totalSpent = stod(data[9]);
                            member->addToTotalSpent(totalSpent);
                        } catch (...) {
                            // Ignore conversion errors
                        }
                    }

                    people.push_back(member);
                } else if (role == "Host") {
                    HOST *host = new HOST(personID, fullName, gender, dob);
                    host->setEmail(email);
                    host->setPhoneNumber(phone);
                    host->setAddress(address);

                    // Optional fields for Host
                    if (data.size() > 7) host->setEmergencyContact(data[7]);

                    people.push_back(host);
                }
            }
        } catch (const std::exception &e) {
            qDebug() << "Error parsing person data:" << QString::fromStdString(line) << " - "
                     << QString::fromStdString(e.what());
        }
    }

    file.close();
    qDebug() << "Imported" << people.size() << "people from file";
}

// FUNC: Export people to CSV file
void exportPeopleInfo(const vector<PERSON *> &people, const string &outputFilePath) {
    std::ofstream output(outputFilePath);

    if (!output.is_open()) {
        QMessageBox::critical(nullptr, "Error", "Cannot open output file!");
        return;
    }

    // Write CSV header
    output << "FullName,DOB,Email,Phone,Gender,Address,Role,EmergencyContact";
    // Additional Member-specific headers
    output << ",Interests,TotalSpent\n";

    // Write person data
    for (const PERSON *person : people) {
        if (!person) continue;

        // Common fields for all people
        output << person->getFullName() << "," << person->getDateOfBirth().toString() << "," << person->getEmail()
               << "," << person->getPhoneNumber() << "," << genderToString(person->getGender()) << ","
               << person->getAddress() << "," << person->getRole() << ",";

        // Role-specific fields
        if (person->getRole() == "Member") {
            const MEMBER *member = static_cast<const MEMBER *>(person);
            output << member->getEmergencyContact() << ",";

            // Interests - semicolon-separated within quotes
            output << "\"";
            const vector<string> &interests = member->getInterests();
            for (size_t i = 0; i < interests.size(); i++) {
                output << interests[i];
                if (i < interests.size() - 1) {
                    output << ";";
                }
            }
            output << "\",";

            // Total spent
            output << member->getTotalSpent();
        } else if (person->getRole() == "Host") {
            const HOST *host = static_cast<const HOST *>(person);
            output << host->getEmergencyContact() << ",,";  // Empty fields for Member-specific columns
        }

        output << "\n";
    }

    output.close();
    qDebug() << "Exported" << people.size() << "people to file";
}

// FUNC: Load people from cache file
void loadPeopleCacheFile(vector<PERSON *> &people) {
    QString cacheFilePath = getPeopleCacheFilePath();

    QFileInfo cacheFile(cacheFilePath);
    if (!cacheFile.exists()) {
        qDebug() << "No people cache file found at:" << cacheFilePath;
        return;
    }

    try {
        qDebug() << "Loading people from cache:" << cacheFilePath;

        // Free existing people before clearing the vector
        for (PERSON *person : people) {
            delete person;
        }
        people.clear();

        // Import from cache file
        importPeopleInfo(people, cacheFilePath.toStdString());

        qDebug() << "Successfully loaded" << people.size() << "people from cache";

    } catch (const exception &e) {
        qDebug() << "Error loading people cache file:" << e.what();
    }
}

// FUNC: Update people cache file
void updatePeopleCacheFile(const vector<PERSON *> &people) {
    QString cacheFilePath = getPeopleCacheFilePath();

    try {
        exportPeopleInfo(people, cacheFilePath.toStdString());
        qDebug() << "People cache file updated successfully:" << cacheFilePath;
    } catch (const exception &e) {
        qDebug() << "Error updating people cache file:" << e.what();
    }
}

// FUNC: Import trips from cache file (includes IDs)
void importTripFromCache(vector<TRIP> &trips, const string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        qDebug() << "Error: Cannot open cache file for import:" << QString::fromStdString(filePath);
        return;
    }

    std::string line;
    // Skip header line if it exists
    if (std::getline(file, line) &&
        (line.find("ID") != std::string::npos || line.find("Destination") != std::string::npos)) {
        // This was a header line, continue reading
        qDebug() << "Found header line:" << QString::fromStdString(line);
    } else {
        // This wasn't a header, reset file position to read from beginning
        file.clear();
        file.seekg(0);
    }

    // For debugging
    int lineCount = 0;
    int successCount = 0;

    while (std::getline(file, line)) {
        lineCount++;
        if (line.empty()) continue;

        qDebug() << "Processing cache line" << lineCount << ":" << QString::fromStdString(line);

        // Parse CSV line
        std::vector<std::string> data;
        std::stringstream ss(line);
        std::string token;

        // Better CSV parsing that handles quotes and commas within fields
        bool inQuotes = false;
        std::string field;

        for (char c : line) {
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                data.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }
        // Add the last field
        data.push_back(field);

        try {
            // Check if we have enough data fields (must have ID column)
            if (data.size() >= 6) {  // ID,Destination,Description,StartDate,EndDate,Status
                qDebug() << "Found" << data.size() << "fields in cache line" << lineCount;

                // Extract data - use correct indices for cache format
                string id = data[0];  // ID is first field
                string destination = data[1];
                string description = data[2];
                DATE startDate = extractDate(data[3]);
                DATE endDate = extractDate(data[4]);

                // Create status enum from string
                STATUS status;
                if (data[5] == "Planned") {
                    status = STATUS::Planned;
                } else if (data[5] == "Ongoing") {
                    status = STATUS::Ongoing;
                } else if (data[5] == "Completed") {
                    status = STATUS::Completed;
                } else if (data[5] == "Cancelled") {
                    status = STATUS::Cancelled;
                } else {
                    status = STATUS::Planned;  // Default
                }

                // Create trip with the ID from the cache file
                TRIP newTrip(id, destination, description, startDate, endDate, status);

                // Add to the trips vector
                trips.push_back(newTrip);
                successCount++;
                qDebug() << "Successfully loaded trip from cache:" << QString::fromStdString(id) << "-"
                         << QString::fromStdString(destination);
            } else {
                qDebug() << "Error: Not enough fields in cache line" << lineCount << ". Expected at least 6, got"
                         << data.size();
            }
        } catch (const std::exception &e) {
            qDebug() << "Error parsing trip data at line" << lineCount << ":" << QString::fromStdString(line) << " - "
                     << QString::fromStdString(e.what());
        }
    }

    file.close();
    qDebug() << "Cache import complete. Processed" << lineCount << "lines, successfully imported" << successCount
             << "trips";
}

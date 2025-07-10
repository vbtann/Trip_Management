#include "../Models/header.h"

using namespace std;

int TRIP::tripCount = 0;

int main(void) {
     vector<MEMBER> members;

     // === MEMBER 1: Adventure Enthusiast ===
     MEMBER m1("M001", "Nguyen Anh Tra", GENDER::Male, DATE(15, 5, 2003));
     // Using only available setter methods from header.h
     m1.setEmail("anhtra.nguyen@gmail.com");
     m1.setPhoneNumber("+84 912 345 678");
     m1.setAddress("123 Le Loi Street, District 1, Ho Chi Minh City");
     m1.setNationality("Vietnamese");
     m1.setEmergencyContact("Tra's Mother - +84 987 654 321");
     m1.addInterest("Photography");
     m1.addInterest("Mountain Climbing");
     m1.addInterest("Cultural Tours");
     m1.addToTotalSpent(2500.00);
     m1.joinTrip("T001");
     m1.joinTrip("T003");
     m1.joinTrip("T007");

     // === MEMBER 2: Culture & Food Lover ===
     MEMBER m2("M002", "Le Phi Hung", GENDER::Male, DATE(22, 8, 2003));
     m2.setEmail("phihung.le@outlook.com");
     m2.setPhoneNumber("+84 901 234 567");
     m2.setAddress("456 Tran Hung Dao Street, District 5, Ho Chi Minh City");
     m2.setNationality("Vietnamese");
     m2.setEmergencyContact("Hung's Father - +84 978 123 456");
     m2.addInterest("Food Tours");
     m2.addInterest("Historical Sites");
     m2.addInterest("Local Markets");
     m2.addInterest("Street Food");
     m2.addToTotalSpent(1800.50);
     m2.joinTrip("T002");
     m2.joinTrip("T004");

     // === MEMBER 3: Beach & Water Sports Fan ===
     MEMBER m3("M003", "Tran Viet Bao", GENDER::Male, DATE(10, 12, 2002));
     m3.setEmail("vietbao.tran@yahoo.com");
     m3.setPhoneNumber("+84 913 456 789");
     m3.setAddress("789 Nguyen Hue Boulevard, District 1, Ho Chi Minh City");
     m3.setNationality("Vietnamese");
     m3.setEmergencyContact("Bao's Sister - +84 989 876 543");
     m3.addInterest("Scuba Diving");
     m3.addInterest("Beach Volleyball");
     m3.addInterest("Surfing");
     m3.addInterest("Island Hopping");
     m3.addToTotalSpent(3200.75);
     m3.joinTrip("T001");
     m3.joinTrip("T005");
     m3.joinTrip("T006");
     m3.joinTrip("T008");

     // === MEMBER 4: Budget Traveler & Student ===
     MEMBER m4("M004", "Vo Binh Phong", GENDER::Male, DATE(3, 3, 2004));
     m4.setEmail("binhphong.vo@student.hcmus.edu.vn");
     m4.setPhoneNumber("+84 932 567 890");
     m4.setAddress("321 Vo Van Tan Street, District 3, Ho Chi Minh City");
     m4.setNationality("Vietnamese");
     m4.setEmergencyContact("Phong's Aunt - +84 967 432 109");
     m4.addInterest("Budget Travel");
     m4.addInterest("Hostels");
     m4.addInterest("Public Transportation");
     m4.addInterest("Free Walking Tours");
     m4.addToTotalSpent(950.25);
     m4.joinTrip("T002");
     m4.joinTrip("T009");

     // Add all members to vector
     members.push_back(m1);
     members.push_back(m2);
     members.push_back(m3);
     members.push_back(m4);

     // Display all members
     cout << "===============================================" << endl;
     cout << "           MEMBER DATABASE TEST" << endl;
     cout << "===============================================" << endl;

     for (const MEMBER& m : members) {
          cout << "Member ID: " << m.getID() << endl;
          cout << "Full Name: " << m.getFullName() << endl;
          cout << "Email: " << m.getEmail() << endl;
          cout << "Phone: " << m.getPhoneNumber() << endl;
          cout << "Address: " << m.getAddress() << endl;
          cout << "Nationality: " << m.getNationality() << endl;
          cout << "Gender: " << genderToString(m.getGender()) << endl;
          cout << "Date of Birth: " << m.getDateOfBirth().toString() << endl;
          cout << "Age: " << m.getAge() << " years old" << endl;
          cout << "Emergency Contact: " << m.getEmergencyContact() << endl;
          cout << "Has Driver License: "
               << (m.getHasDriverLicense() ? "Yes" : "No") << endl;
          cout << "Total Spent: $" << m.getTotalSpent() << endl;
          cout << "Trips Joined: " << m.getJoinedTripCount() << endl;

          // Display interests
          vector<string> interests = m.getInterests();
          if (!interests.empty()) {
               cout << "Interests: ";
               for (size_t i = 0; i < interests.size(); ++i) {
                    if (i > 0) cout << ", ";
                    cout << interests[i];
               }
               cout << endl;
          }

          cout << "===============================================" << endl;
     }

     // Test some member statistics
     cout << "\n=== MEMBER STATISTICS ===" << endl;
     cout << "Total Members: " << members.size() << endl;

     int driversCount = 0;
     double totalSpent = 0.0;
     int totalTrips = 0;

     for (const MEMBER& m : members) {
          if (m.getHasDriverLicense()) driversCount++;
          totalSpent += m.getTotalSpent();
          totalTrips += m.getJoinedTripCount();
     }

     cout << "Members with Driver License: " << driversCount << "/"
          << members.size() << endl;
     cout << "Total Amount Spent by All Members: $" << totalSpent << endl;
     cout << "Total Trips Joined by All Members: " << totalTrips << endl;
     cout << "Average Spending per Member: $" << (totalSpent / members.size())
          << endl;
     cout << "Average Trips per Member: "
          << (totalTrips / (double)members.size()) << endl;

     // Test specific member functions
     cout << "\n=== TESTING MEMBER FUNCTIONS ===" << endl;

     // Test member summary
     cout << "Member 1 Summary: " << m1.getMemberSummary() << endl;
     cout << "Member 2 Summary: " << m2.getMemberSummary() << endl;

     // Test trip membership
     cout << "Member 1 joined trip T001: "
          << (m1.hasJoinedTrip("T001") ? "Yes" : "No") << endl;
     cout << "Member 2 joined trip T001: "
          << (m2.hasJoinedTrip("T001") ? "Yes" : "No") << endl;

     // Test last joined trip
     cout << "Member 1 last trip: " << m1.getLastJoinedTripID() << endl;
     cout << "Member 3 last trip: " << m3.getLastJoinedTripID() << endl;

     return 0;
}
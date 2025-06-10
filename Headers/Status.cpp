#include "header.h"

using namespace std;

string statusToString(Status status) {
     switch (status) {
          case Status::Planned:
               return "Planned";
          case Status::Ongoing:
               return "Ongoing";
          case Status::Completed:
               return "Completed";
          default:
               return "Unknown";
     }
}

Status stringToStatus(const string &statusStr) {
     if (statusStr == "Planned") return Status::Planned;
     if (statusStr == "Ongoing") return Status::Ongoing;
     if (statusStr == "Completed") return Status::Completed;
     if (statusStr == "Cancelled") return Status::Cancelled;

     return Status::Planned;
}
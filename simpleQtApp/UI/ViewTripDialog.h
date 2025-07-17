#ifndef VIEWTRIPDIALOG_H
#define VIEWTRIPDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>
#include <vector>

#include "../Managers/PersonManager.h"
#include "../Models/header.h"

using namespace std;

class ViewTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ViewTripDialog(TRIP &trip, PERSONMANAGER *personManager, QWidget *parent = nullptr);
    void setPersonManager(PERSONMANAGER *personManager);
    TRIP getUpdatedTrip() const;
    TRIP getOriginalTrip() const;

   private slots:
    void editCurrentTrip();      // Slot for "Edit" button
    void closeView();            // Slot for "Close" button
    void onHostInfoClicked();    // ADD: Slot for host info button
    void onMemberInfoClicked();  // ADD: Slot for member info button

   private:
    void setupUI();
    void displayCurrentTrip();
    void displayPeopleInfo();     // ADD: Method to display people information
    void setupTripInfoSection();  // ADD: Method to setup trip info section
    void setupPeopleSection();    // ADD: Method to setup people section

    TRIP &trip;
    TRIP originalTrip;

    // UI Components for displaying trip info
    QLabel *idLabel;
    QLabel *destinationLabel;
    QTextEdit *descriptionDisplay;
    QLabel *startDateLabel;
    QLabel *endDateLabel;
    QLabel *statusLabel;

    // ADD: UI Components for displaying people info
    QGroupBox *tripInfoGroupBox;
    QGroupBox *peopleInfoGroupBox;

    // Host information
    QLabel *hostLabel;
    QLabel *hostNameLabel;
    QLabel *hostEmailLabel;
    QLabel *hostPhoneLabel;
    QPushButton *hostInfoButton;

    // Members information
    QLabel *membersCountLabel;
    QListWidget *membersListWidget;
    QPushButton *membersInfoButton;

    // Scroll area for large content
    QScrollArea *scrollArea;

    // Navigation and control buttons
    QPushButton *editButton;
    QPushButton *closeButton;

    PERSONMANAGER *personManager;
};

#endif  // VIEWTRIPDIALOG_H
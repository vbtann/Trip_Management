#ifndef EDITTRIPDIALOG_H
#define EDITTRIPDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>

#include "../Managers/PersonManager.h"
#include "../Managers/TripFactory.h"
#include "../Models/header.h"

class EditTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit EditTripDialog(TRIP &trip, QWidget *parent = nullptr);

    TRIP getUpdatedTrip() const;
    TRIP getOriginalTrip() const;

    // Set PersonManager for people selection
    void setPersonManager(PERSONMANAGER *manager);

   private slots:
    void acceptChanges();
    void rejectChanges();
    void onDestinationChanged();
    void onStartDateChanged();
    void onHostSelectionChanged();
    void onMembersSelectionChanged();
    void onSelectAllMembers();
    void onClearAllMembers();

   private:
    void setupUI();
    void setupPeopleSelection();
    void setupPeopleSelectionWidget();  // ADD: New method declaration
    void populateFields();
    void populatePeopleSelection();
    void styleComponents();
    void updateMembersList();
    void updateSelectedCount();
    bool validatePeopleSelection();

    // Trip data
    TRIP originalTrip;
    TRIP editedTrip;

    // Manager
    PERSONMANAGER *personManager;

    // Basic form widgets
    QLineEdit *tripIDLineEdit;
    QLineEdit *destinationLineEdit;
    QTextEdit *descriptionTextEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QComboBox *statusComboBox;

    // People selection widgets
    QGroupBox *hostGroupBox;
    QComboBox *hostComboBox;
    QLabel *selectedHostLabel;

    QGroupBox *membersGroupBox;
    QListWidget *membersListWidget;
    QLabel *selectedMembersLabel;
    QPushButton *selectAllMembersButton;
    QPushButton *clearAllMembersButton;

    // Action buttons
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *resetButton;
};

#endif  // EDITTRIPDIALOG_H
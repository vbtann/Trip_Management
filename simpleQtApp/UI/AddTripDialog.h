#ifndef TRIPDIALOG_H
#define TRIPDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>

#include "../Managers/PersonManager.h"
#include "../Managers/TripFactory.h"
#include "../Models/header.h"

class AddTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit AddTripDialog(QWidget *parent = nullptr);
    explicit AddTripDialog(PERSONMANAGER *personManager, QWidget *parent = nullptr);
    TRIP getTripData() const;

   private slots:
    void acceptDialog();
    void rejectDialog();
    void onHostSelectionChanged();
    void onMemberSelectionChanged();
    void onSelectAllMembers();
    void onClearAllMembers();

   private:
    void setupUI();
    void setupPeopleSelection();
    void updateMemberList();
    void updateHostList();
    void updateSelectedCounts();
    bool validatePeopleSelection();

    // UI Components - Trip Info
    QLineEdit *destinationLineEdit;
    QTextEdit *descriptionTextEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QComboBox *statusComboBox;
    QPushButton *okButton;
    QPushButton *cancelButton;

    // UI Components - People Selection
    // Host section
    QGroupBox *hostGroupBox;
    QGroupBox *membersGroupBox;
    QListWidget *hostsListWidget;
    QLabel *selectedHostLabel;
    // Host Search
    QHBoxLayout *hostSearchLayout;
    QTextEdit *hostSearchBar;
    QPushButton *clearHostSearchButton;
    // Members section
    QListWidget *membersListWidget;
    QPushButton *selectAllMembersButton;
    QPushButton *clearAllMembersButton;
    QLabel *selectedMembersLabel;
    // Member Search
    QHBoxLayout *memberSearchLayout;
    QTextEdit *memberSearchBar;
    QPushButton *clearMemberSearchButton;

    // Data
    TRIP _tripData;
    PERSONMANAGER *personManager;
    QSet<QString> selectedMemberIDs;
};

#endif  // TRIPDIALOG_H

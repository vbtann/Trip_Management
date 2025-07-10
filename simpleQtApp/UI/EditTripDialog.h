#ifndef EDITTRIPDIALOG_H
#define EDITTRIPDIALOG_H

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include "../Managers/TripFactory.h"
#include "../Models/header.h"

using namespace std;

class EditTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit EditTripDialog(TRIP &trip, QWidget *parent = nullptr);
    TRIP getUpdatedTrip() const;
    TRIP getOriginalTrip() const;

   private slots:
    void acceptChanges();
    void rejectChanges();
    void onDestinationChanged();
    void onStartDateChanged();

   private:
    void setupUI();
    void populateFields();
    void styleComponents();

    // Reference to the trip being edited
    TRIP originalTrip;
    TRIP &editedTrip;

    // UI Components
    QLineEdit *tripIDLineEdit;
    QLineEdit *destinationLineEdit;
    QTextEdit *descriptionTextEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QComboBox *statusComboBox;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *resetButton;

    QLabel *tripIDLabel;
    QLabel *destinationLabel;
    QLabel *descriptionLabel;
    QLabel *startDateLabel;
    QLabel *endDateLabel;
    QLabel *statusLabel;
};

#endif  // EDITTRIPDIALOG_H
#ifndef ADDTRIPDIALOG_H
#define ADDTRIPDIALOG_H

#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

#include "../Managers/TripFactory.h"
#include "Models/header.h"

class AddTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit AddTripDialog(QWidget *parent = nullptr);
    TRIP getTripData() const;

   private slots:
    void acceptDialog();
    void rejectDialog();

   private:
    void setupUI();

    // UI Components
    QLineEdit *destinationLineEdit;
    QTextEdit *descriptionTextEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QComboBox *statusComboBox;
    QPushButton *okButton;
    QPushButton *cancelButton;

    // Data
    TRIP _tripData;
};

#endif  // ADDTRIPDIALOG_H
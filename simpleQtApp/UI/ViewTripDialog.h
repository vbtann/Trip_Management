#ifndef VIEWTRIPDIALOG_H
#define VIEWTRIPDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <vector>

#include "../Models/header.h"

using namespace std;

class ViewTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ViewTripDialog(TRIP &trip, QWidget *parent = nullptr);
    TRIP getUpdatedTrip() const;
    TRIP getOriginalTrip() const;

   private slots:
    void editCurrentTrip();  // Slot for "Edit" button
    void closeView();        // Slot for "Close" button

   private:
    void setupUI();
    void displayCurrentTrip();
    TRIP &trip;
    TRIP originalTrip;

    // UI Components for displaying trip info
    QLabel *idLabel;
    QLabel *destinationLabel;
    QTextEdit *descriptionDisplay;
    QLabel *startDateLabel;
    QLabel *endDateLabel;
    QLabel *statusLabel;

    // Navigation and control buttons
    QPushButton *editButton;
    QPushButton *closeButton;
};

#endif  // VIEWTRIPDIALOG_H
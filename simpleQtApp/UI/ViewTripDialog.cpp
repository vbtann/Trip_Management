#include "ViewTripDialog.h"

#include <QMessageBox>

#include "EditTripDialog.h"  // Add this include

ViewTripDialog::ViewTripDialog(TRIP &trip, QWidget *parent) : QDialog(parent), trip(trip), originalTrip(trip) {
    setupUI();
    setWindowIcon(QIcon("../Pictures/magnifying_glass.jpg"));
    setWindowTitle("View Trips");
    setModal(true);
    setMinimumSize(400, 300);

    displayCurrentTrip();
}

void ViewTripDialog::setupUI() {
    // Create display labels (read-only)
    idLabel = new QLabel(this);
    destinationLabel = new QLabel(this);
    descriptionDisplay = new QTextEdit(this);
    descriptionDisplay->setReadOnly(true);
    descriptionDisplay->setMaximumHeight(80);
    startDateLabel = new QLabel(this);
    endDateLabel = new QLabel(this);
    statusLabel = new QLabel(this);

    // Style the labels to look like display fields
    QString labelStyle =
        "QLabel { "
        "border: 1px solid #ccc; "
        "padding: 3px; "
        "background-color: #f9f9f9; "
        "border-radius: 3px; "
        "font-size: 13px;"
        "}";

    idLabel->setStyleSheet(labelStyle);
    destinationLabel->setStyleSheet(labelStyle);
    descriptionDisplay->setStyleSheet(labelStyle);
    startDateLabel->setStyleSheet(labelStyle);
    endDateLabel->setStyleSheet(labelStyle);
    statusLabel->setStyleSheet(labelStyle);

    // Create grid layout for better control
    QGridLayout *gridLayout = new QGridLayout();

    // Row 0: ID and Destination
    gridLayout->addWidget(new QLabel("Trip ID:"), 0, 0);
    gridLayout->addWidget(idLabel, 0, 1);
    gridLayout->addWidget(new QLabel("Destination:"), 0, 2);
    gridLayout->addWidget(destinationLabel, 0, 3);

    // Row 1: Description (spans all columns)
    gridLayout->addWidget(new QLabel("Description:"), 1, 0);
    gridLayout->addWidget(descriptionDisplay, 1, 1, 1, 3);  // Span 3 columns

    // Row 2: Start Date and End Date
    gridLayout->addWidget(new QLabel("Start Date:"), 2, 0);
    gridLayout->addWidget(startDateLabel, 2, 1);
    gridLayout->addWidget(new QLabel("End Date:"), 2, 2);
    gridLayout->addWidget(endDateLabel, 2, 3);

    // Row 3: Status
    gridLayout->addWidget(new QLabel("Status:"), 3, 0);
    gridLayout->addWidget(statusLabel, 3, 1, 1, 3);  // Span 3 columns

    // Set column stretch to make it look better
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(3, 1);

    // Create control buttons
    editButton = new QPushButton("Edit Trip", this);
    closeButton = new QPushButton("Close", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(editButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Connect signals
    connect(editButton, &QPushButton::clicked, this, &ViewTripDialog::editCurrentTrip);
    connect(closeButton, &QPushButton::clicked, this, &ViewTripDialog::closeView);
}

void ViewTripDialog::displayCurrentTrip() {
    const TRIP &currentTrip = trip;

    // Display trip information
    idLabel->setText(QString::fromStdString(currentTrip.getID()));
    destinationLabel->setText(QString::fromStdString(currentTrip.getDestination()));
    descriptionDisplay->setText(QString::fromStdString(currentTrip.getDescription()));

    // Format dates (assuming your Date class has day(), month(), year()
    // methods)
    DATE startDate = currentTrip.getStartDate();
    DATE endDate = currentTrip.getEndDate();

    startDateLabel->setText(QString("%1/%2/%3")
                                .arg(startDate.getDay(), 2, 10, QChar('0'))
                                .arg(startDate.getMonth(), 2, 10, QChar('0'))
                                .arg(startDate.getYear()));

    endDateLabel->setText(QString("%1/%2/%3")
                              .arg(endDate.getDay(), 2, 10, QChar('0'))
                              .arg(endDate.getMonth(), 2, 10, QChar('0'))
                              .arg(endDate.getYear()));

    // Display status (assuming you have a statusToString function)
    statusLabel->setText(QString::fromStdString(statusToString(currentTrip.getStatus())));
}

void ViewTripDialog::editCurrentTrip() {
    // Create edit dialog
    EditTripDialog editDialog(trip, this);

    if (editDialog.exec() == QDialog::Accepted) {
        // Trip was successfully edited, refresh the display
        displayCurrentTrip();

        // Optionally show confirmation
        QMessageBox::information(this, "Success", "Trip has been successfully updated!");
    }
}

TRIP ViewTripDialog::getUpdatedTrip() const { return trip; }
TRIP ViewTripDialog::getOriginalTrip() const { return originalTrip; }

void ViewTripDialog::closeView() {
    accept();  // Close the dialog
}
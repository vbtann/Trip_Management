#include "AddTripDialog.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDateTime>
#include <QFont>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

#include "Models/header.h"

AddTripDialog::AddTripDialog(QWidget *parent) : QDialog(parent) {
    setupUI();
    setWindowTitle("Add New Trip");
    setModal(true);

    // Make the dialog much larger
    setFixedSize(800, 600);  // Increased from 600x400

    // Center the dialog on parent
    if (parent) {
        move(parent->geometry().center() - rect().center());
    }
}

void AddTripDialog::setupUI() {
    // Set up fonts for better readability
    QFont labelFont;
    labelFont.setPointSize(11);
    labelFont.setBold(true);

    QFont inputFont;
    inputFont.setPointSize(10);

    // Create styled form fields with larger sizes
    destinationLineEdit = new QLineEdit(this);
    destinationLineEdit->setPlaceholderText("Enter destination (e.g., Paris, Tokyo, New York)...");
    destinationLineEdit->setMinimumHeight(40);  // Increased height
    destinationLineEdit->setFont(inputFont);
    destinationLineEdit->setStyleSheet(
        "QLineEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 14px; "
        "}"
        "QLineEdit:focus { "
        "    border-color: #4CAF50; "
        "    background-color: #f9f9f9; "
        "}");

    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setMinimumHeight(120);  // Increased from 80
    descriptionTextEdit->setPlaceholderText(
        "Enter detailed description of the trip...\n\nInclude activities, "
        "accommodation details, special notes, etc.");
    descriptionTextEdit->setFont(inputFont);
    descriptionTextEdit->setStyleSheet(
        "QTextEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 14px; "
        "}"
        "QTextEdit:focus { "
        "    border-color: #4CAF50; "
        "    background-color: #f9f9f9; "
        "}");

    startDateEdit = new QDateEdit(QDate::currentDate(), this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");
    startDateEdit->setMinimumHeight(40);  // Increased height
    startDateEdit->setFont(inputFont);
    startDateEdit->setStyleSheet(
        "QDateEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 14px; "
        "}"
        "QDateEdit:focus { "
        "    border-color: #4CAF50; "
        "}"
        "QDateEdit::drop-down { "
        "    subcontrol-origin: padding; "
        "    subcontrol-position: top right; "
        "    width: 30px; "
        "    border-left: 1px solid #ddd; "
        "}");

    endDateEdit = new QDateEdit(QDate::currentDate().addDays(1), this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    endDateEdit->setMinimumHeight(40);  // Increased height
    endDateEdit->setFont(inputFont);
    endDateEdit->setStyleSheet(
        "QDateEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 14px; "
        "}"
        "QDateEdit:focus { "
        "    border-color: #4CAF50; "
        "}"
        "QDateEdit::drop-down { "
        "    subcontrol-origin: padding; "
        "    subcontrol-position: top right; "
        "    width: 30px; "
        "    border-left: 1px solid #ddd; "
        "}");

    statusComboBox = new QComboBox(this);
    statusComboBox->addItem("🗓️ Planned");
    statusComboBox->addItem("🚀 Ongoing");
    statusComboBox->addItem("✅ Completed");
    statusComboBox->addItem("❌ Cancelled");
    statusComboBox->setMinimumHeight(40);  // Increased height
    statusComboBox->setFont(inputFont);
    statusComboBox->setStyleSheet(
        "QComboBox { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 14px; "
        "    min-width: 200px; "
        "}"
        "QComboBox:focus { "
        "    border-color: #4CAF50; "
        "}"
        "QComboBox::drop-down { "
        "    subcontrol-origin: padding; "
        "    subcontrol-position: top right; "
        "    width: 30px; "
        "    border-left: 1px solid #ddd; "
        "}"
        "QComboBox::down-arrow { "
        "    width: 12px; "
        "    height: 12px; "
        "}");

    // Create form layout with better spacing
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setVerticalSpacing(20);    // Increased spacing between rows
    formLayout->setHorizontalSpacing(15);  // Increased horizontal spacing
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Create labels with better styling
    QLabel *destLabel = new QLabel("🌍 Destination:");
    QLabel *descLabel = new QLabel("📝 Description:");
    QLabel *startLabel = new QLabel("📅 Start Date:");
    QLabel *endLabel = new QLabel("📅 End Date:");
    QLabel *statusLabel = new QLabel("📊 Status:");

    // Style the labels
    QString labelStyle =
        "QLabel { "
        "    font-weight: bold; "
        "    font-size: 17px; "
        "    color: #333; "
        "    padding-right: 10px; "
        "}";

    destLabel->setStyleSheet(labelStyle);
    descLabel->setStyleSheet(labelStyle);
    startLabel->setStyleSheet(labelStyle);
    endLabel->setStyleSheet(labelStyle);
    statusLabel->setStyleSheet(labelStyle);

    formLayout->addRow(destLabel, destinationLineEdit);
    formLayout->addRow(descLabel, descriptionTextEdit);
    formLayout->addRow(startLabel, startDateEdit);
    formLayout->addRow(endLabel, endDateEdit);
    formLayout->addRow(statusLabel, statusComboBox);

    // Create larger, more prominent buttons
    okButton = new QPushButton("✅ Add Trip", this);
    cancelButton = new QPushButton("❌ Cancel", this);

    // Style the buttons
    okButton->setMinimumHeight(45);  // Larger buttons
    okButton->setMinimumWidth(120);
    okButton->setFont(QFont("Arial", 11, QFont::Bold));
    okButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #4CAF50; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 12px 24px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "    background-color: #45a049; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #3d8b40; "
        "}");

    cancelButton->setMinimumHeight(45);
    cancelButton->setMinimumWidth(120);
    cancelButton->setFont(QFont("Arial", 11, QFont::Bold));
    cancelButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #f44336; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 12px 24px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "    background-color: #da190b; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #b71c1c; "
        "}");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addSpacing(10);  // Space between buttons
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    // Main layout with better spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);  // Larger margins
    mainLayout->setSpacing(20);                      // More spacing between sections

    // Add a title
    QLabel *titleLabel = new QLabel("Add New Trip", this);
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 18px; "
        "    font-weight: bold; "
        "    color: #2c3e50; "
        "    padding: 10px 0; "
        "    text-align: center; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);

    // Set dialog background
    setStyleSheet(
        "QDialog { "
        "    background-color: #fafafa; "
        "    border: 1px solid #ddd; "
        "}");

    // Connect signals
    connect(okButton, &QPushButton::clicked, this, &AddTripDialog::acceptDialog);
    connect(cancelButton, &QPushButton::clicked, this, &AddTripDialog::rejectDialog);

    // Connect Enter key to OK button
    okButton->setDefault(true);

    // Set focus to destination field
    destinationLineEdit->setFocus();
}

void AddTripDialog::acceptDialog() {
    // Validate input fields
    if (destinationLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Destination cannot be empty!\n\nPlease enter a "
                             "destination for your trip.");
        destinationLineEdit->setFocus();
        return;
    }

    if (descriptionTextEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Description cannot be empty!\n\nPlease provide "
                             "a description for your trip.");
        descriptionTextEdit->setFocus();
        return;
    }

    // Validate dates
    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();

    if (endDate < startDate) {
        QMessageBox::warning(this, "Date Validation Error",
                             "End date cannot be earlier than start "
                             "date!\n\nPlease check your dates.");
        startDateEdit->setFocus();
        return;
    }

    // Convert QDate to DATE object
    DATE startDateObj(startDate.day(), startDate.month(), startDate.year());
    DATE endDateObj(endDate.day(), endDate.month(), endDate.year());

    // Convert status string to STATUS enum (remove emoji)
    QString statusText = statusComboBox->currentText();
    QString cleanStatus = statusText.mid(2).trimmed();  // Remove emoji and space
    STATUS tripStatus = stringToStatus(cleanStatus.toStdString());

    try {
        // Create trip with all required data
        std::string destination = destinationLineEdit->text().trimmed().toUpper().toStdString();
        std::string description = descriptionTextEdit->toPlainText().trimmed().toStdString();

        _tripData = TRIPFACTORY::createTrip(destination, description, startDateObj, endDateObj, tripStatus);
        accept();  // This should work now

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error",
                              QString("Failed to create trip: %1\n\nPlease "
                                      "check your input and try again.")
                                  .arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error",
                              "Unknown error occurred while creating trip.\n\nPlease check "
                              "your input and try again.");
    }
}

void AddTripDialog::rejectDialog() { reject(); }

TRIP AddTripDialog::getTripData() const { return _tripData; }

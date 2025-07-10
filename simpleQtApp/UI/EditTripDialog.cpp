#include "EditTripDialog.h"

EditTripDialog::EditTripDialog(TRIP &trip, QWidget *parent) : QDialog(parent), originalTrip(trip), editedTrip(trip) {
    setWindowTitle("Edit Trip - " + QString::fromStdString(trip.getDestination()));
    setWindowIcon(QIcon(":/icons/edit.png"));
    setModal(true);
    setMinimumSize(600, 500);

    setupUI();
    populateFields();
    styleComponents();

    // Center the dialog
    if (parent) {
        move(parent->geometry().center() - rect().center());
    }
}

void EditTripDialog::setupUI() {
    // Create form fields
    tripIDLineEdit = new QLineEdit(this);
    tripIDLineEdit->setReadOnly(true);  // ID shouldn't be editable

    destinationLineEdit = new QLineEdit(this);
    destinationLineEdit->setPlaceholderText("Enter destination (e.g., PARIS, NEW YORK)");

    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setPlaceholderText("Enter trip description...");
    descriptionTextEdit->setMaximumHeight(100);

    startDateEdit = new QDateEdit(this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");
    startDateEdit->setDate(QDate::currentDate());

    endDateEdit = new QDateEdit(this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    endDateEdit->setDate(QDate::currentDate().addDays(1));

    statusComboBox = new QComboBox(this);
    statusComboBox->addItem("📅 Planned");
    statusComboBox->addItem("🚀 Ongoing");
    statusComboBox->addItem("✅ Completed");
    statusComboBox->addItem("❌ Cancelled");

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setVerticalSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight);

    formLayout->addRow("🆔 Trip ID:", tripIDLineEdit);
    formLayout->addRow("📍 Destination:", destinationLineEdit);
    formLayout->addRow("📝 Description:", descriptionTextEdit);
    formLayout->addRow("📅 Start Date:", startDateEdit);
    formLayout->addRow("📅 End Date:", endDateEdit);
    formLayout->addRow("🏷️ Status:", statusComboBox);

    // Create buttons
    saveButton = new QPushButton("💾 Save Changes", this);
    cancelButton = new QPushButton("❌ Cancel", this);
    resetButton = new QPushButton("🔄 Reset", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(cancelButton);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Title
    QLabel *titleLabel = new QLabel("Edit Trip Details", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px; }");

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(saveButton, &QPushButton::clicked, this, &EditTripDialog::acceptChanges);
    connect(cancelButton, &QPushButton::clicked, this, &EditTripDialog::rejectChanges);
    connect(resetButton, &QPushButton::clicked, this, &EditTripDialog::populateFields);
    connect(destinationLineEdit, &QLineEdit::textChanged, this, &EditTripDialog::onDestinationChanged);

    // CORRECT way to connect QDateEdit::dateChanged signal
    connect(startDateEdit, &QDateEdit::dateChanged, this, &EditTripDialog::onStartDateChanged);

    // Set focus
    destinationLineEdit->setFocus();
}

void EditTripDialog::populateFields() {
    // Fill fields with current trip data
    tripIDLineEdit->setText(QString::fromStdString(originalTrip.getID()));
    destinationLineEdit->setText(QString::fromStdString(originalTrip.getDestination()));
    descriptionTextEdit->setPlainText(QString::fromStdString(originalTrip.getDescription()));

    // Set dates
    DATE startDate = originalTrip.getStartDate();
    DATE endDate = originalTrip.getEndDate();

    startDateEdit->setDate(QDate(startDate.getYear(), startDate.getMonth(), startDate.getDay()));
    endDateEdit->setDate(QDate(endDate.getYear(), endDate.getMonth(), endDate.getDay()));

    // Set status
    STATUS currentStatus = originalTrip.getStatus();
    switch (currentStatus) {
        case STATUS::Planned:
            statusComboBox->setCurrentIndex(0);
            break;
        case STATUS::Ongoing:
            statusComboBox->setCurrentIndex(1);
            break;
        case STATUS::Completed:
            statusComboBox->setCurrentIndex(2);
            break;
        case STATUS::Cancelled:
            statusComboBox->setCurrentIndex(3);
            break;
    }
}

void EditTripDialog::styleComponents() {
    // Style input fields
    QString inputStyle =
        "QLineEdit, QTextEdit, QDateEdit, QComboBox { "
        "    border: 2px solid #bdc3c7; "
        "    border-radius: 5px; "
        "    padding: 8px; "
        "    font-size: 13px; "
        "    background-color: white; "
        "} "
        "QLineEdit:focus, QTextEdit:focus, QDateEdit:focus, QComboBox:focus { "
        "    border-color: #3498db; "
        "} "
        "QLineEdit:read-only { "
        "    background-color: #f8f9fa; "
        "    color: #6c757d; "
        "}";

    tripIDLineEdit->setStyleSheet(inputStyle);
    destinationLineEdit->setStyleSheet(inputStyle);
    descriptionTextEdit->setStyleSheet(inputStyle);
    startDateEdit->setStyleSheet(inputStyle);
    endDateEdit->setStyleSheet(inputStyle);
    statusComboBox->setStyleSheet(inputStyle);

    // Style buttons
    saveButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #27ae60; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 12px 24px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    min-width: 120px; "
        "} "
        "QPushButton:hover { background-color: #219a52; } "
        "QPushButton:pressed { background-color: #1e8449; }");

    cancelButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #e74c3c; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 12px 24px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    min-width: 120px; "
        "} "
        "QPushButton:hover { background-color: #c0392b; } "
        "QPushButton:pressed { background-color: #a93226; }");

    resetButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #f39c12; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 12px 24px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    min-width: 120px; "
        "} "
        "QPushButton:hover { background-color: #e67e22; } "
        "QPushButton:pressed { background-color: #d35400; }");
}

void EditTripDialog::acceptChanges() {
    // Validate input
    if (destinationLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Destination cannot be empty!");
        destinationLineEdit->setFocus();
        return;
    }

    if (descriptionTextEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Description cannot be empty!");
        descriptionTextEdit->setFocus();
        return;
    }

    // Validate dates
    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();

    if (endDate < startDate) {
        QMessageBox::warning(this, "Date Validation Error", "End date cannot be earlier than start date!");
        endDateEdit->setFocus();
        return;
    }

    try {
        // Update dates first
        DATE newStartDate(startDate.day(), startDate.month(), startDate.year());
        DATE newEndDate(endDate.day(), endDate.month(), endDate.year());
        editedTrip.setStartDate(newStartDate);
        editedTrip.setEndDate(newEndDate);

        // Update destination
        string newDestination = destinationLineEdit->text().trimmed().toUpper().toStdString();
        editedTrip.setDestination(newDestination);

        // IMPORTANT: Generate and set new ID based on new destination and start date
        string newID = TRIPFACTORY::generateTripID(newDestination, newStartDate);
        editedTrip.setID(newID);

        // Update description
        editedTrip.setDescription(descriptionTextEdit->toPlainText().trimmed().toStdString());

        // Update status
        QString statusText = statusComboBox->currentText();
        QString cleanStatus = statusText.mid(2).trimmed();  // Remove emoji
        editedTrip.setStatus(stringToStatus(cleanStatus.toStdString()));

        // Update the original trip

        accept();  // Close dialog with success

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to update trip: %1").arg(e.what()));
    }
}

void EditTripDialog::rejectChanges() {
    int ret = QMessageBox::question(this, "Discard Changes", "Are you sure you want to discard all changes?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        reject();  // Close dialog without saving
    }
}

void EditTripDialog::onDestinationChanged() {
    // Auto-uppercase destination as user types
    QString text = destinationLineEdit->text().toUpper();
    destinationLineEdit->setText(text);

    // Regenerate trip ID when destination changes
    if (!text.isEmpty()) {
        // Get current start date
        QDate startDate = startDateEdit->date();
        DATE startDateObj(startDate.day(), startDate.month(), startDate.year());

        // Generate new ID based on new destination and start date
        std::string newID = TRIPFACTORY::generateTripID(text.toStdString(), startDateObj);
        tripIDLineEdit->setText(QString::fromStdString(newID));
    }
}

void EditTripDialog::onStartDateChanged() {
    QString destination = destinationLineEdit->text().trimmed();
    if (!destination.isEmpty()) {
        QDate startDate = startDateEdit->date();
        DATE startDateObj(startDate.day(), startDate.month(), startDate.year());

        std::string newID = TRIPFACTORY::generateTripID(destination.toStdString(), startDateObj);
        tripIDLineEdit->setText(QString::fromStdString(newID));
    }
}

TRIP EditTripDialog::getUpdatedTrip() const { return editedTrip; }
TRIP EditTripDialog::getOriginalTrip() const { return originalTrip; }
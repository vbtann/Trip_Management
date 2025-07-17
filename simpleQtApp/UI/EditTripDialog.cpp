#include "EditTripDialog.h"

EditTripDialog::EditTripDialog(TRIP &trip, QWidget *parent)
    : QDialog(parent), originalTrip(trip), editedTrip(trip), personManager(nullptr) {
    setWindowTitle("Edit Trip - " + QString::fromStdString(trip.getDestination()));
    setWindowIcon(QIcon(":/icons/edit.png"));
    setModal(true);
    setMinimumSize(1300, 750);  // Increased size for people selection
    resize(1400, 800);

    setupUI();
    populateFields();
    // populatePeopleSelection();  // Populate with existing attendees
    styleComponents();

    // Center the dialog
    if (parent) {
        move(parent->geometry().center() - rect().center());
    }
}

void EditTripDialog::setPersonManager(PERSONMANAGER *manager) {
    personManager = manager;
    updateMembersList();  // Refresh the lists when manager is set
    populatePeopleSelection();
}

void EditTripDialog::setupUI() {
    // Create form fields with compact sizes for left panel
    tripIDLineEdit = new QLineEdit(this);
    tripIDLineEdit->setReadOnly(true);
    tripIDLineEdit->setMinimumHeight(35);

    destinationLineEdit = new QLineEdit(this);
    destinationLineEdit->setPlaceholderText("Enter destination (e.g., PARIS, NEW YORK)");
    destinationLineEdit->setMinimumHeight(35);

    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setPlaceholderText("Enter trip description...");
    descriptionTextEdit->setMaximumHeight(60);  // Compact for left panel

    startDateEdit = new QDateEdit(this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");
    startDateEdit->setDate(QDate::currentDate());
    startDateEdit->setMinimumHeight(35);

    endDateEdit = new QDateEdit(this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    endDateEdit->setDate(QDate::currentDate().addDays(1));
    endDateEdit->setMinimumHeight(35);

    statusComboBox = new QComboBox(this);
    statusComboBox->addItem("📅 Planned");
    statusComboBox->addItem("🚀 Ongoing");
    statusComboBox->addItem("✅ Completed");
    statusComboBox->addItem("❌ Cancelled");
    statusComboBox->setMinimumHeight(35);

    // Create form layout for LEFT PANEL
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setVerticalSpacing(12);
    formLayout->setHorizontalSpacing(10);
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
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    // LEFT PANEL - Trip details (Fixed)
    QGroupBox *tripDetailsGroupBox = new QGroupBox("Trip Details");
    tripDetailsGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #bdc3c7; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}");
    tripDetailsGroupBox->setLayout(formLayout);
    tripDetailsGroupBox->setFixedWidth(400);
    tripDetailsGroupBox->setMaximumHeight(350);

    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    leftPanelLayout->addWidget(tripDetailsGroupBox);
    leftPanelLayout->addLayout(buttonLayout);
    leftPanelLayout->addStretch();

    QWidget *leftPanelWidget = new QWidget();
    leftPanelWidget->setLayout(leftPanelLayout);
    leftPanelWidget->setFixedWidth(420);

    // RIGHT PANEL - People selection (Dynamic)
    setupPeopleSelectionWidget();

    QVBoxLayout *rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->setContentsMargins(0, 0, 0, 0);
    rightPanelLayout->setSpacing(10);

    // Host section (smaller)
    hostGroupBox->setMaximumHeight(180);

    // Members section (larger)
    membersGroupBox->setMinimumHeight(400);

    rightPanelLayout->addWidget(hostGroupBox, 1);     // 25% weight
    rightPanelLayout->addWidget(membersGroupBox, 3);  // 75% weight

    QWidget *rightPanelWidget = new QWidget();
    rightPanelWidget->setLayout(rightPanelLayout);
    rightPanelWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // MAIN HORIZONTAL LAYOUT
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);
    contentLayout->addWidget(leftPanelWidget);
    contentLayout->addWidget(rightPanelWidget, 1);

    // MAIN LAYOUT
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // Title
    QLabel *titleLabel = new QLabel("Edit Trip Details", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; margin: 8px; }");
    titleLabel->setMaximumHeight(40);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(contentLayout, 1);

    // Connect signals
    connect(saveButton, &QPushButton::clicked, this, &EditTripDialog::acceptChanges);
    connect(cancelButton, &QPushButton::clicked, this, &EditTripDialog::rejectChanges);
    connect(resetButton, &QPushButton::clicked, this, [this]() {
        populateFields();
        populatePeopleSelection();
    });
    connect(destinationLineEdit, &QLineEdit::textChanged, this, &EditTripDialog::onDestinationChanged);
    connect(startDateEdit, &QDateEdit::dateChanged, this, &EditTripDialog::onStartDateChanged);

    // Set focus
    destinationLineEdit->setFocus();
}

void EditTripDialog::setupPeopleSelectionWidget() {
    // Host selection (SMALLER)
    hostGroupBox = new QGroupBox("HOST");
    hostGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #e67e22; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}");

    QVBoxLayout *hostLayout = new QVBoxLayout(hostGroupBox);
    hostLayout->setSpacing(8);

    selectedHostLabel = new QLabel("No host selected", this);
    selectedHostLabel->setStyleSheet("QLabel { color: #7f8c8d; font-size: 13px; padding: 4px; }");

    hostComboBox = new QComboBox();
    hostComboBox->setMinimumHeight(35);
    hostComboBox->setStyleSheet(
        "QComboBox { "
        "    border: 2px solid #bdc3c7; "
        "    border-radius: 5px; "
        "    padding: 8px; "
        "    font-size: 13px; "
        "    background-color: white; "
        "} "
        "QComboBox:focus { border-color: #3498db; }");

    hostLayout->addWidget(selectedHostLabel);
    hostLayout->addWidget(hostComboBox);
    hostLayout->addStretch();

    // Members selection (LARGER)
    membersGroupBox = new QGroupBox("MEMBERS");
    membersGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #3498db; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}");

    QVBoxLayout *membersLayout = new QVBoxLayout(membersGroupBox);
    membersLayout->setSpacing(10);

    selectedMembersLabel = new QLabel("No members selected");
    selectedMembersLabel->setStyleSheet("color: #7f8c8d; font-style: italic; font-size: 13px; padding: 4px;");

    // Member buttons
    QHBoxLayout *membersButtonLayout = new QHBoxLayout();
    selectAllMembersButton = new QPushButton("Select All");
    clearAllMembersButton = new QPushButton("Clear All");

    selectAllMembersButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #3498db; "
        "    color: white; "
        "    border: none; "
        "    padding: 6px 12px; "
        "    border-radius: 4px; "
        "    font-size: 11px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #2980b9; }");

    clearAllMembersButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #95a5a6; "
        "    color: white; "
        "    border: none; "
        "    padding: 6px 12px; "
        "    border-radius: 4px; "
        "    font-size: 11px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #7f8c8d; }");

    membersButtonLayout->addWidget(selectAllMembersButton);
    membersButtonLayout->addWidget(clearAllMembersButton);
    membersButtonLayout->addStretch();

    membersListWidget = new QListWidget();
    membersListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    membersListWidget->setStyleSheet(
        "QListWidget { "
        "    border: 2px solid #ddd; "
        "    border-radius: 5px; "
        "    font-size: 13px; "
        "    background-color: white; "
        "}"
        "QListWidget::item { "
        "    padding: 8px; "
        "    border-bottom: 1px solid #eee; "
        "}"
        "QListWidget::item:hover { "
        "    background-color: #ecf0f1; "
        "}");

    membersLayout->addWidget(selectedMembersLabel);
    membersLayout->addLayout(membersButtonLayout);
    membersLayout->addWidget(membersListWidget, 1);  // Give list all remaining space

    // Connect signals
    connect(hostComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &EditTripDialog::onHostSelectionChanged);
    connect(membersListWidget, &QListWidget::itemChanged, this, &EditTripDialog::onMembersSelectionChanged);
    connect(selectAllMembersButton, &QPushButton::clicked, this, &EditTripDialog::onSelectAllMembers);
    connect(clearAllMembersButton, &QPushButton::clicked, this, &EditTripDialog::onClearAllMembers);
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

void EditTripDialog::populatePeopleSelection() {
    if (!personManager) return;

    // Populate existing host selection
    HOST currentHost = originalTrip.getHost();
    if (!currentHost.getID().empty()) {
        // Find and select the current host
        for (int i = 0; i < hostComboBox->count(); ++i) {
            if (hostComboBox->itemData(i).toString().toStdString() == currentHost.getID()) {
                hostComboBox->setCurrentIndex(i);
                break;
            }
        }
    }

    // Populate existing members selection
    vector<MEMBER> currentMembers = originalTrip.getMembers();
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item) {
            QString memberID = item->data(Qt::UserRole).toString();

            // Check if this member is in the current trip
            bool isSelected = false;
            for (const MEMBER &member : currentMembers) {
                if (member.getID() == memberID.toStdString()) {
                    isSelected = true;
                    break;
                }
            }

            item->setData(Qt::CheckStateRole, isSelected ? Qt::Checked : Qt::Unchecked);
        }
    }

    updateSelectedCount();
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
        "    padding: 12px 16px; "
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
        "    padding: 12px 16px; "
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
        "    padding: 12px 16px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    min-width: 120px; "
        "} "
        "QPushButton:hover { background-color: #e67e22; } "
        "QPushButton:pressed { background-color: #d35400; }");
}

void EditTripDialog::updateMembersList() {
    if (!personManager) {
        // If no PersonManager, disable people selection
        hostComboBox->addItem("No people available - Please add people first");
        hostComboBox->setEnabled(false);

        QListWidgetItem *disabledItem = new QListWidgetItem("No people available - Please add people first");
        disabledItem->setFlags(Qt::NoItemFlags);  // Make it non-interactive
        membersListWidget->addItem(disabledItem);

        membersListWidget->setEnabled(false);
        selectAllMembersButton->setEnabled(false);
        clearAllMembersButton->setEnabled(false);
        return;
    }

    hostComboBox->clear();
    membersListWidget->clear();

    // Get hosts and members
    vector<HOST> hosts = personManager->getAllHosts();
    vector<MEMBER> members = personManager->getAllMembers();

    // Populate host combo box
    hostComboBox->addItem("--SELECT A HOST--", QString(""));
    for (const HOST &host : hosts) {
        QString hostInfo = QString("%1 (ID: %2)")
                               .arg(QString::fromStdString(host.getFullName()))
                               .arg(QString::fromStdString(host.getID()));
        hostComboBox->addItem(hostInfo, QString::fromStdString(host.getID()));
    }

    // Populate members list
    for (const MEMBER &member : members) {
        QString memberInfo = QString("%1 (ID: %2)")
                                 .arg(QString::fromStdString(member.getFullName()))
                                 .arg(QString::fromStdString(member.getID()));

        QListWidgetItem *item = new QListWidgetItem(memberInfo);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setData(Qt::CheckStateRole, Qt::Unchecked);
        item->setData(Qt::UserRole, QString::fromStdString(member.getID()));
        membersListWidget->addItem(item);
    }

    updateSelectedCount();
}

void EditTripDialog::onHostSelectionChanged() { updateSelectedCount(); }
void EditTripDialog::onMembersSelectionChanged() { updateSelectedCount(); }

void EditTripDialog::onSelectAllMembers() {
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->flags() & Qt::ItemIsUserCheckable) {  // Check if item is checkable
            item->setData(Qt::CheckStateRole, Qt::Checked);     // FIX: Use setData with CheckStateRole
        }
    }
    updateSelectedCount();
}

void EditTripDialog::onClearAllMembers() {
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->flags() & Qt::ItemIsUserCheckable) {  // Check if item is checkable
            item->setData(Qt::CheckStateRole, Qt::Unchecked);   // FIX: Use setData with CheckStateRole
        }
    }
    updateSelectedCount();
}

void EditTripDialog::updateSelectedCount() {
    // Update HOST label
    if (hostComboBox->currentIndex() > 0) {
        selectedHostLabel->setText(QString("Host selected: %1").arg(hostComboBox->currentText()));
        selectedHostLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        selectedHostLabel->setText(QString("No host selected"));
        selectedHostLabel->setStyleSheet("color: #e74c3c; font-weight: bold;");
    }

    int selectedCount = 0;
    for (int i = 0; i < membersListWidget->count(); i++) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            selectedCount++;
        }
    }

    if (selectedCount > 0) {
        selectedMembersLabel->setText(QString("✅ %1 member(s) selected").arg(selectedCount));
        selectedMembersLabel->setStyleSheet("color: #27ae60; font-weight: bold");
    } else {
        selectedMembersLabel->setText(QString("❌ No members selected"));
        selectedMembersLabel->setStyleSheet("color: #e74c3c; font-weight: bold");
    }
}

bool EditTripDialog::validatePeopleSelection() {
    // Check if host is selected
    if (personManager && hostComboBox->currentIndex() <= 0) {
        QMessageBox::warning(this, "Host Required",
                             "Please select a host for the trip!\n\n"
                             "Every trip must have a host.");
        return false;
    }

    // FIX: Check if at least one member is selected using data() with CheckStateRole
    int selectedMembers = 0;
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {  // FIX: Use data() with CheckStateRole
            selectedMembers++;
        }
    }

    if (personManager && selectedMembers == 0) {
        QMessageBox::warning(this, "Members Required",
                             "Please select at least one member for the trip!\n\n"
                             "A trip needs participants.");
        return false;
    }

    return true;
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

    // Validate people selection
    if (!validatePeopleSelection()) {
        return;
    }

    try {
        // Update basic trip information
        DATE newStartDate(startDate.day(), startDate.month(), startDate.year());
        DATE newEndDate(endDate.day(), endDate.month(), endDate.year());
        editedTrip.setStartDate(newStartDate);
        editedTrip.setEndDate(newEndDate);

        string newDestination = destinationLineEdit->text().trimmed().toUpper().toStdString();
        editedTrip.setDestination(newDestination);

        // IMPORTANT: Generate and set new ID based on new destination and start date
        string newID = TRIPFACTORY::generateTripID(newDestination, newStartDate);
        editedTrip.setID(newID);

        editedTrip.setDescription(descriptionTextEdit->toPlainText().trimmed().toStdString());

        // Update status
        QString statusText = statusComboBox->currentText();
        QString cleanStatus = statusText.mid(2).trimmed();  // Remove emoji
        editedTrip.setStatus(stringToStatus(cleanStatus.toStdString()));

        // Update people selection if PersonManager is available
        if (personManager) {
            // Set host
            if (hostComboBox->currentIndex() > 0) {
                QString hostID = hostComboBox->currentData().toString();
                HOST selectedHost = personManager->getHostByID(hostID.toStdString());
                editedTrip.setHost(selectedHost);
            }

            // Set members
            vector<MEMBER> selectedMembers;
            for (int i = 0; i < membersListWidget->count(); ++i) {
                QListWidgetItem *item = membersListWidget->item(i);
                if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
                    QString memberID = item->data(Qt::UserRole).toString();
                    MEMBER member = personManager->getMemberByID(memberID.toStdString());
                    selectedMembers.push_back(member);
                }
            }
            editedTrip.setMembers(selectedMembers);
        }

        // Success
        QMessageBox::information(this, "Success", "Trip updated successfully!");
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
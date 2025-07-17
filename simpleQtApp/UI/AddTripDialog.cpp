#include "AddTripDialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDateTime>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

#include "Models/header.h"

// Register types with Qt's meta-object system
Q_DECLARE_METATYPE(HOST *)
Q_DECLARE_METATYPE(MEMBER *)

AddTripDialog::AddTripDialog(QWidget *parent) : QDialog(parent), personManager(nullptr) {
    setupUI();
    setWindowTitle("Add New Trip");
    setModal(true);

    // Make the dialog larger to accommodate people selection
    setMinimumSize(1300, 800);  // Increased from 800x600
    resize(1400, 800);

    // Center the dialog on parent
    if (parent) {
        move(parent->geometry().center() - rect().center());
    }
}

AddTripDialog::AddTripDialog(PERSONMANAGER *personManager, QWidget *parent)
    : QDialog(parent), personManager(personManager) {
    setupUI();
    setWindowTitle("Add New Trip");
    setModal(true);

    // Make the dialog larger to accommodate people selection
    setFixedSize(1200, 800);

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

    // Create styled form fields with optimized sizes for left panel
    destinationLineEdit = new QLineEdit(this);
    destinationLineEdit->setPlaceholderText("Enter destination (e.g., Paris, Tokyo)...");
    destinationLineEdit->setMinimumHeight(35);
    destinationLineEdit->setFont(inputFont);
    destinationLineEdit->setStyleSheet(
        "QLineEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 12px; "
        "}"
        "QLineEdit:focus { "
        "    border-color: #4CAF50; "
        "    background-color: #f9f9f9; "
        "}");

    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setMaximumHeight(60);  // Keep compact for left panel
    descriptionTextEdit->setPlaceholderText("Enter trip description...");
    descriptionTextEdit->setFont(inputFont);
    descriptionTextEdit->setStyleSheet(
        "QTextEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 12px; "
        "}"
        "QTextEdit:focus { "
        "    border-color: #4CAF50; "
        "    background-color: #f9f9f9; "
        "}");

    startDateEdit = new QDateEdit(QDate::currentDate(), this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");
    startDateEdit->setMinimumHeight(35);
    startDateEdit->setFont(inputFont);
    startDateEdit->setStyleSheet(
        "QDateEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 12px; "
        "}"
        "QDateEdit:focus { border-color: #4CAF50; }");

    endDateEdit = new QDateEdit(QDate::currentDate().addDays(1), this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    endDateEdit->setMinimumHeight(35);
    endDateEdit->setFont(inputFont);
    endDateEdit->setStyleSheet(
        "QDateEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 12px; "
        "}"
        "QDateEdit:focus { border-color: #4CAF50; }");

    statusComboBox = new QComboBox(this);
    statusComboBox->addItem("🗓️ Planned");
    statusComboBox->addItem("🚀 Ongoing");
    statusComboBox->addItem("✅ Completed");
    statusComboBox->addItem("❌ Cancelled");
    statusComboBox->setMinimumHeight(35);
    statusComboBox->setFont(inputFont);
    statusComboBox->setStyleSheet(
        "QComboBox { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 12px; "
        "}"
        "QComboBox:focus { border-color: #4CAF50; }");

    // Setup people selection
    setupPeopleSelection();

    // Create form layout for trip information (LEFT SIDE)
    QFormLayout *tripInfoLayout = new QFormLayout();
    tripInfoLayout->setVerticalSpacing(12);
    tripInfoLayout->setHorizontalSpacing(10);
    tripInfoLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

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
        "    font-size: 13px; "
        "    color: #333; "
        "    padding-right: 8px; "
        "}";

    destLabel->setStyleSheet(labelStyle);
    descLabel->setStyleSheet(labelStyle);
    startLabel->setStyleSheet(labelStyle);
    endLabel->setStyleSheet(labelStyle);
    statusLabel->setStyleSheet(labelStyle);

    tripInfoLayout->addRow(destLabel, destinationLineEdit);
    tripInfoLayout->addRow(descLabel, descriptionTextEdit);
    tripInfoLayout->addRow(startLabel, startDateEdit);
    tripInfoLayout->addRow(endLabel, endDateEdit);
    tripInfoLayout->addRow(statusLabel, statusComboBox);

    // Create trip info group box (LEFT SIDE - FIXED HEIGHT)
    QGroupBox *tripInfoGroupBox = new QGroupBox("Trip Information");
    tripInfoGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #bdc3c7; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");
    tripInfoGroupBox->setLayout(tripInfoLayout);
    tripInfoGroupBox->setFixedWidth(400);     // Fixed width for left panel
    tripInfoGroupBox->setMaximumHeight(350);  // Fixed height for trip info

    // Create buttons for trip info section
    okButton = new QPushButton("✅ Add Trip", this);
    cancelButton = new QPushButton("❌ Cancel", this);

    // Style the buttons
    okButton->setMinimumHeight(40);
    okButton->setMinimumWidth(110);
    okButton->setFont(QFont("Arial", 10, QFont::Bold));
    okButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #4CAF50; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }");

    cancelButton->setMinimumHeight(40);
    cancelButton->setMinimumWidth(110);
    cancelButton->setFont(QFont("Arial", 10, QFont::Bold));
    cancelButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #f44336; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #da190b; }"
        "QPushButton:pressed { background-color: #b71c1c; }");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    // LEFT PANEL - Fixed position trip info
    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    leftPanelLayout->addWidget(tripInfoGroupBox);
    leftPanelLayout->addLayout(buttonLayout);
    leftPanelLayout->addStretch();  // Push everything to top

    QWidget *leftPanelWidget = new QWidget();
    leftPanelWidget->setLayout(leftPanelLayout);
    leftPanelWidget->setFixedWidth(420);  // Fixed width for entire left panel

    // RIGHT PANEL - People selection (dynamic height)
    QVBoxLayout *rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->setContentsMargins(0, 0, 0, 0);
    rightPanelLayout->setSpacing(10);

    // Host section (smaller - about 25% of right panel)
    hostGroupBox->setMaximumHeight(180);

    // Members section (larger - about 75% of right panel)
    membersGroupBox->setMinimumHeight(400);

    rightPanelLayout->addWidget(hostGroupBox, 1);     // 25% weight
    rightPanelLayout->addWidget(membersGroupBox, 3);  // 75% weight

    QWidget *rightPanelWidget = new QWidget();
    rightPanelWidget->setLayout(rightPanelLayout);
    rightPanelWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // MAIN HORIZONTAL LAYOUT
    QHBoxLayout *mainHorizontalLayout = new QHBoxLayout();
    mainHorizontalLayout->setSpacing(20);
    mainHorizontalLayout->addWidget(leftPanelWidget);      // Fixed width left panel
    mainHorizontalLayout->addWidget(rightPanelWidget, 1);  // Expandable right panel

    // MAIN LAYOUT
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // Add a title
    QLabel *titleLabel = new QLabel("Add New Trip", this);
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 18px; "
        "    font-weight: bold; "
        "    color: #2c3e50; "
        "    padding: 8px 0; "
        "    text-align: center; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setMaximumHeight(40);  // Fixed height for title

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(mainHorizontalLayout, 1);  // Give it all remaining space

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

void AddTripDialog::setupPeopleSelection() {
    // Host selection group (SMALLER - about 20% of people area)
    hostGroupBox = new QGroupBox("🏠 Select Host");
    hostGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #e67e22; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QVBoxLayout *hostLayout = new QVBoxLayout(hostGroupBox);
    hostLayout->setSpacing(8);

    selectedHostLabel = new QLabel("No host selected");
    selectedHostLabel->setStyleSheet(
        "QLabel { "
        "    color: #7f8c8d; "
        "    font-style: italic; "
        "    font-size: 13px; "
        "    padding: 4px; "
        "}");

    hostComboBox = new QComboBox();
    hostComboBox->setMinimumHeight(35);
    hostComboBox->setStyleSheet(
        "QComboBox { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 5px; "
        "    font-size: 13px; "
        "    background-color: white; "
        "}"
        "QComboBox:focus { border-color: #e67e22; }");

    hostLayout->addWidget(selectedHostLabel);
    hostLayout->addWidget(hostComboBox);
    hostLayout->addStretch();  // Push content to top

    // Members selection group (LARGER - about 80% of people area)
    membersGroupBox = new QGroupBox("👥 Select Members");
    membersGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #3498db; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QVBoxLayout *membersLayout = new QVBoxLayout(membersGroupBox);
    membersLayout->setSpacing(10);

    selectedMembersLabel = new QLabel("No members selected");
    selectedMembersLabel->setStyleSheet(
        "QLabel { "
        "    color: #7f8c8d; "
        "    font-style: italic; "
        "    font-size: 13px; "
        "    padding: 4px; "
        "}");

    QHBoxLayout *memberButtonsLayout = new QHBoxLayout();
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

    memberButtonsLayout->addWidget(selectAllMembersButton);
    memberButtonsLayout->addWidget(clearAllMembersButton);
    memberButtonsLayout->addStretch();

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
        "}"
        "QListWidget::item:selected { "
        "    background-color: #3498db; "
        "    color: white; "
        "}");

    membersLayout->addWidget(selectedMembersLabel);
    membersLayout->addLayout(memberButtonsLayout);
    membersLayout->addWidget(membersListWidget, 1);  // Give list widget all remaining space

    // Connect signals
    connect(hostComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &AddTripDialog::onHostSelectionChanged);
    connect(membersListWidget, &QListWidget::itemChanged, this,
            &AddTripDialog::onMemberSelectionChanged);  // FIX: Complete this line
    connect(selectAllMembersButton, &QPushButton::clicked, this, &AddTripDialog::onSelectAllMembers);
    connect(clearAllMembersButton, &QPushButton::clicked, this, &AddTripDialog::onClearAllMembers);

    // Update people lists
    updateMemberList();
}

void AddTripDialog::updateMemberList() {
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

    // Clear existing items
    hostComboBox->clear();
    membersListWidget->clear();

    // Get all people
    vector<HOST> hosts = personManager->getAllHosts();
    vector<MEMBER> members = personManager->getAllMembers();

    // Populate host combo box - Store ID instead of pointer
    hostComboBox->addItem("-- Select a Host --", QString(""));
    for (const HOST &host : hosts) {
        QString hostInfo = QString("%1 (ID: %2)")
                               .arg(QString::fromStdString(host.getFullName()))
                               .arg(QString::fromStdString(host.getID()));
        hostComboBox->addItem(hostInfo, QString::fromStdString(host.getID()));  // Store ID instead of pointer
    }

    // Populate members list widget - Store ID instead of pointer
    for (const MEMBER &member : members) {
        QString memberInfo = QString("%1 (ID: %2)")
                                 .arg(QString::fromStdString(member.getFullName()))
                                 .arg(QString::fromStdString(member.getID()));

        QListWidgetItem *item = new QListWidgetItem(memberInfo);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setData(Qt::CheckStateRole, Qt::Unchecked);
        item->setData(Qt::UserRole, QString::fromStdString(member.getID()));  // Store ID instead of pointer
        membersListWidget->addItem(item);
    }

    updateSelectedCounts();
}

void AddTripDialog::onHostSelectionChanged() { updateSelectedCounts(); }

void AddTripDialog::onMemberSelectionChanged() { updateSelectedCounts(); }

void AddTripDialog::onSelectAllMembers() {
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->flags() & Qt::ItemIsUserCheckable) {  // Check if item is checkable
            item->setData(Qt::CheckStateRole, Qt::Checked);     // FIX: Use setData with CheckStateRole
        }
    }
    updateSelectedCounts();
}

void AddTripDialog::onClearAllMembers() {
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->flags() & Qt::ItemIsUserCheckable) {  // Check if item is checkable
            item->setData(Qt::CheckStateRole, Qt::Unchecked);   // FIX: Use setData with CheckStateRole
        }
    }
    updateSelectedCounts();
}

void AddTripDialog::updateSelectedCounts() {
    // Update host label
    if (hostComboBox->currentIndex() > 0) {
        selectedHostLabel->setText(QString("✅ Host selected: %1").arg(hostComboBox->currentText()));
        selectedHostLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        selectedHostLabel->setText("❌ No host selected");
        selectedHostLabel->setStyleSheet("color: #e74c3c; font-style: italic;");
    }

    // FIX: Count selected members using data() with CheckStateRole
    int selectedCount = 0;
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {  // FIX: Use data() with CheckStateRole
            selectedCount++;
        }
    }

    if (selectedCount > 0) {
        selectedMembersLabel->setText(QString("✅ %1 member(s) selected").arg(selectedCount));
        selectedMembersLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        selectedMembersLabel->setText("❌ No members selected");
        selectedMembersLabel->setStyleSheet("color: #e74c3c; font-style: italic;");
    }
}

bool AddTripDialog::validatePeopleSelection() {
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

void AddTripDialog::acceptDialog() {
    // Validate input fields
    if (destinationLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Destination cannot be empty!\n\nPlease enter a destination for your trip.");
        destinationLineEdit->setFocus();
        return;
    }

    if (descriptionTextEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Description cannot be empty!\n\nPlease provide a description for your trip.");
        descriptionTextEdit->setFocus();
        return;
    }

    // Validate dates
    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();

    if (endDate < startDate) {
        QMessageBox::warning(this, "Date Validation Error",
                             "End date cannot be earlier than start date!\n\nPlease check your dates.");
        startDateEdit->setFocus();
        return;
    }

    // Validate people selection
    if (!validatePeopleSelection()) {
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

        // Add selected host and members to the trip
        if (personManager) {
            // FIX: Get host by ID instead of pointer
            if (hostComboBox->currentIndex() > 0) {
                QString hostID = hostComboBox->currentData().toString();  // Get ID string
                if (!hostID.isEmpty()) {
                    HOST selectedHost = personManager->getHostByID(hostID.toStdString());
                    if (!selectedHost.getID().empty()) {  // Check if host was found
                        _tripData.setHost(selectedHost);
                    }
                }
            }

            // FIX: Get members by ID instead of pointer
            for (int i = 0; i < membersListWidget->count(); ++i) {
                QListWidgetItem *item = membersListWidget->item(i);
                if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
                    QString memberID = item->data(Qt::UserRole).toString();  // Get ID string
                    if (!memberID.isEmpty()) {
                        MEMBER selectedMember = personManager->getMemberByID(memberID.toStdString());
                        if (!selectedMember.getID().empty()) {  // Check if member was found
                            _tripData.addMember(selectedMember);
                        }
                    }
                }
            }
        }

        accept();

    } catch (const std::exception &e) {
        QMessageBox::critical(
            this, "Error",
            QString("Failed to create trip: %1\n\nPlease check your input and try again.").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error",
                              "Unknown error occurred while creating trip.\n\nPlease check your input and try again.");
    }
}

void AddTripDialog::rejectDialog() { reject(); }

TRIP AddTripDialog::getTripData() const { return _tripData; }

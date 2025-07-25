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
    labelFont.setPointSize(13);
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
        "    font-size: 15px; "
        "}"
        "QLineEdit:focus { "
        "    border-color: #4CAF50; "
        "    background-color: #f9f9f9; "
        "}");

    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setMaximumHeight(200);  // Keep compact for left panel
    descriptionTextEdit->setPlaceholderText("Enter trip description...");
    descriptionTextEdit->setFont(inputFont);
    descriptionTextEdit->setStyleSheet(
        "QTextEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 15px; "
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
        "    font-size: 15px; "
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
        "    font-size: 15px; "
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
        "    font-size: 15px; "
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
        "    font-size: 18px; "
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
    tripInfoGroupBox->setFixedWidth(505);     // Fixed width for left panel
    tripInfoGroupBox->setMaximumHeight(650);  // Fixed height for trip info

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
    leftPanelWidget->setFixedWidth(520);  // Fixed width for entire left panel

    // RIGHT PANEL - People selection (dynamic height)
    QVBoxLayout *rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->setContentsMargins(0, 0, 0, 0);
    rightPanelLayout->setSpacing(10);

    // Host section (smaller - about 25% of right panel)
    hostGroupBox->setMaximumHeight(280);

    // Members section (larger - about 75% of right panel)
    membersGroupBox->setMinimumHeight(400);

    rightPanelLayout->addWidget(hostGroupBox, 2);     // 25% weight
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

    selectedHostLabel = new QLabel("❌ No host selected");
    selectedHostLabel->setStyleSheet(
        "QLabel { "
        "    color: #e74c3c; "
        "    font-style: italic; "
        "    font-size: 13px; "
        "    padding: 4px; "
        "}");

    // Host search layout
    hostSearchLayout = new QHBoxLayout();

    hostSearchBar = new QTextEdit();
    hostSearchBar->setPlaceholderText("Search hosts...");
    hostSearchBar->setMaximumHeight(45);
    hostSearchBar->setStyleSheet(R"(
                                 padding: 8px 12px;
                              )");

    clearHostSearchButton = new QPushButton("Clear");
    clearHostSearchButton->setStyleSheet(
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

    hostSearchLayout->addWidget(hostSearchBar);
    hostSearchLayout->addWidget(clearHostSearchButton);

    // Host list widget (single selection)
    hostsListWidget = new QListWidget();
    hostsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);  // Only one host can be selected
    hostsListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hostsListWidget->setStyleSheet(
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
        "    background-color: #e67e22; "
        "    color: white; "
        "}");

    hostLayout->addWidget(selectedHostLabel);
    hostLayout->addLayout(hostSearchLayout);
    hostLayout->addWidget(hostsListWidget, 1);  // Give list widget remaining space

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

    memberSearchLayout = new QHBoxLayout();

    memberSearchBar = new QTextEdit();
    memberSearchBar->setPlaceholderText("Search members...");
    memberSearchBar->setMaximumHeight(45);
    memberSearchBar->setStyleSheet(R"(
                                   padding: 8px 12px;
                                )");

    clearMemberSearchButton = new QPushButton("Clear");
    clearMemberSearchButton->setStyleSheet(R"(
                                           QPushButton { 
                                               background-color: #95a5a6; 
                                               color: white; 
                                               border: none; 
                                               padding: 6px 12px; 
                                               border-radius: 4px; 
                                               font-size: 11px; 
                                               font-weight: bold; 
                                           }
                                           QPushButton:hover { background-color: #7f8c8d; }
                                           )");

    memberSearchLayout->addWidget(memberSearchBar);
    memberSearchLayout->addWidget(clearMemberSearchButton);

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
    membersLayout->addLayout(memberSearchLayout);
    membersLayout->addWidget(membersListWidget, 1);  // Give list widget all remaining space

    // Connect signals
    connect(hostsListWidget, &QListWidget::itemSelectionChanged, this, &AddTripDialog::onHostSelectionChanged);
    connect(hostSearchBar, &QTextEdit::textChanged, this, &AddTripDialog::updateHostList);
    connect(clearHostSearchButton, &QPushButton::clicked, [this]() {
        hostSearchBar->clear();
        updateHostList();
    });

    connect(membersListWidget, &QListWidget::itemChanged, this,
            &AddTripDialog::onMemberSelectionChanged);  // FIX: Complete this line
    connect(selectAllMembersButton, &QPushButton::clicked, this, &AddTripDialog::onSelectAllMembers);
    connect(clearAllMembersButton, &QPushButton::clicked, this, &AddTripDialog::onClearAllMembers);
    connect(memberSearchBar, &QTextEdit::textChanged, this, &AddTripDialog::updateMemberList);
    connect(clearMemberSearchButton, &QPushButton::clicked, [this]() {
        memberSearchBar->clear();
        updateMemberList();
    });

    // Update people lists
    updateHostList();
    updateMemberList();
}

void AddTripDialog::updateMemberList() {
    if (!personManager) {
        // If no PersonManager, disable people selection
        QListWidgetItem *disabledItem = new QListWidgetItem("No people available - Please add people first");
        disabledItem->setFlags(Qt::NoItemFlags);  // Make it non-interactive
        membersListWidget->addItem(disabledItem);

        membersListWidget->setEnabled(false);
        selectAllMembersButton->setEnabled(false);
        clearAllMembersButton->setEnabled(false);
        return;
    }

    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.insert(memberID);
            }
        } else if (item) {
            // If item is unchecked, remove from selection set
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.remove(memberID);
            }
        }
    }
    // Clear existing items
    membersListWidget->clear();

    // Get all people
    vector<MEMBER> members = personManager->getAllMembers();
    QString memberSearchName = memberSearchBar->toPlainText().trimmed();

    // Populate members list widget - Store ID instead of pointer
    for (const MEMBER &member : members) {
        QString memberInfo = QString("%1 (ID: %2)")
                                 .arg(QString::fromStdString(member.getFullName()))
                                 .arg(QString::fromStdString(member.getID()));

        if (!memberSearchName.isEmpty() && !memberInfo.contains(memberSearchName, Qt::CaseInsensitive)) {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(memberInfo);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        QString memberID = QString::fromStdString(member.getID());
        item->setData(Qt::UserRole, memberID);

        if (selectedMemberIDs.contains(memberID)) {
            item->setData(Qt::CheckStateRole, Qt::Checked);
        } else {
            item->setData(Qt::CheckStateRole, Qt::Unchecked);
        }

        membersListWidget->addItem(item);
    }

    updateSelectedCounts();
}

void AddTripDialog::updateHostList() {
    if (!personManager) {
        // If no PersonManager, disable host selection
        QListWidgetItem *disabledItem = new QListWidgetItem("No people available - Please add people first");
        disabledItem->setFlags(Qt::NoItemFlags);  // Make it non-interactive
        hostsListWidget->addItem(disabledItem);
        hostsListWidget->setEnabled(false);
        return;
    }

    // Preserve current selection
    QString selectedHostID;
    QListWidgetItem *currentItem = hostsListWidget->currentItem();
    if (currentItem) {
        selectedHostID = currentItem->data(Qt::UserRole).toString();
    }

    // Clear existing items
    hostsListWidget->clear();

    // Get all hosts and apply search filter
    vector<HOST> hosts = personManager->getAllHosts();
    QString hostSearchName = hostSearchBar->toPlainText().trimmed();

    // Populate hosts list widget
    for (const HOST &host : hosts) {
        QString hostInfo = QString("%1 (ID: %2)")
                               .arg(QString::fromStdString(host.getFullName()))
                               .arg(QString::fromStdString(host.getID()));

        // Apply search filter
        if (!hostSearchName.isEmpty() && !hostInfo.contains(hostSearchName, Qt::CaseInsensitive)) {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(hostInfo);
        QString hostID = QString::fromStdString(host.getID());
        item->setData(Qt::UserRole, hostID);

        hostsListWidget->addItem(item);

        // Restore selection if this was the previously selected host
        if (hostID == selectedHostID) {
            hostsListWidget->setCurrentItem(item);
        }
    }

    updateSelectedCounts();
}

void AddTripDialog::onHostSelectionChanged() { updateSelectedCounts(); }

void AddTripDialog::onMemberSelectionChanged() {
    // Update the persistent tracking set when selections change
    // selectedMemberIDs.clear();
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.insert(memberID);
            }
        }
    }

    updateSelectedCounts();
}

void AddTripDialog::onSelectAllMembers() {
    // Add all currently displayed members to selection set
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->flags() & Qt::ItemIsUserCheckable) {
            item->setData(Qt::CheckStateRole, Qt::Checked);
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.insert(memberID);
            }
        }
    }
    updateSelectedCounts();
}

void AddTripDialog::onClearAllMembers() {
    // Remove all currently displayed members from selection set
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->flags() & Qt::ItemIsUserCheckable) {
            item->setData(Qt::CheckStateRole, Qt::Unchecked);
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.remove(memberID);
            }
        }
    }
    updateSelectedCounts();
}

void AddTripDialog::updateSelectedCounts() {
    // Update host label
    QListWidgetItem *selectedHostItem = hostsListWidget->currentItem();
    if (selectedHostItem) {
        QString hostName = selectedHostItem->text();
        selectedHostLabel->setText(QString("✅ Host selected: %1").arg(hostName));
        selectedHostLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        selectedHostLabel->setText("❌ No host selected");
        selectedHostLabel->setStyleSheet("color: #e74c3c; font-style: italic;");
    }

    // Count total selected members (including those not currently displayed due to search)
    int totalSelectedCount = selectedMemberIDs.size();
    int displayedSelectedCount = 0;

    // Count how many selected members are currently displayed
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            displayedSelectedCount++;
        }
    }

    if (totalSelectedCount > 0) {
        QString searchText = memberSearchBar->toPlainText().trimmed();
        if (!searchText.isEmpty() && displayedSelectedCount != totalSelectedCount) {
            // Show both displayed and total when filtering
            selectedMembersLabel->setText(
                QString("✅ %1 of %2 selected member(s) shown").arg(displayedSelectedCount).arg(totalSelectedCount));
        } else {
            // Show just total when not filtering
            selectedMembersLabel->setText(QString("✅ %1 member(s) selected").arg(totalSelectedCount));
        }
        selectedMembersLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        selectedMembersLabel->setText("❌ No members selected");
        selectedMembersLabel->setStyleSheet("color: #e74c3c; font-style: italic;");
    }
}

bool AddTripDialog::validatePeopleSelection() {
    // Check if host is selected
    if (personManager && !hostsListWidget->currentItem()) {
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

    // Destination Check
    if (destinationLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Destination cannot be empty!\n\nPlease enter a destination for your trip.");
        destinationLineEdit->setFocus();
        return;
    } else if (destinationLineEdit->text().trimmed().length() < 3 ||
               destinationLineEdit->text().trimmed().length() > 10) {
        QMessageBox::warning(this, "Validation Error",
                             "Destination must be between 3 and 10 characters!\n\nPlease enter a valid destination.");
        destinationLineEdit->setFocus();
        return;
    } else {
        string text = destinationLineEdit->text().trimmed().toUpper().toStdString();
        for (char c : text) {
            if (!isupper(c) && !isdigit(c)) {
                QMessageBox::warning(this, "Validation Error",
                                     "Destination can only contain uppercase letters and digits!\n\n"
                                     "Please enter a valid destination.");
                destinationLineEdit->setFocus();
                return;
            }
        }
    }

    // Description Check
    if (descriptionTextEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Description cannot be empty!\n\nPlease enter a description for your trip.");
        descriptionTextEdit->setFocus();
        return;
    } else if (descriptionTextEdit->toPlainText().trimmed().length() < 10 ||
               descriptionTextEdit->toPlainText().trimmed().length() > 200) {
        QMessageBox::warning(
            this, "Validation Error",
            "Description must be between 10 and 200 characters!\n\nPlease enter a valid description for your trip.");
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
            // Get selected host
            QListWidgetItem *selectedHostItem = hostsListWidget->currentItem();
            if (selectedHostItem) {
                QString hostID = selectedHostItem->data(Qt::UserRole).toString();
                if (!hostID.isEmpty()) {
                    HOST selectedHost = personManager->getHostByID(hostID.toStdString());
                    if (!selectedHost.getID().empty()) {
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

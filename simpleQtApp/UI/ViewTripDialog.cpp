#include "ViewTripDialog.h"

#include <QApplication>
#include <QFont>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

#include "EditTripDialog.h"

ViewTripDialog::ViewTripDialog(TRIP &trip, PERSONMANAGER *personManager, QWidget *parent)
    : QDialog(parent), trip(trip), originalTrip(trip), personManager(personManager) {
    qDebug() << "ViewTripDialog created with PersonManager:" << (personManager != nullptr);

    setWindowTitle("View Trip Details - " + QString::fromStdString(trip.getDestination()));
    setWindowIcon(QIcon(":/icons/view.png"));
    setModal(true);

    // Make dialog larger for side-by-side layout
    setMinimumSize(1200, 700);
    resize(1300, 750);

    setupUI();
    displayCurrentTrip();
    displayPeopleInfo();

    // Center the dialog
    if (parent) {
        move(parent->geometry().center() - rect().center());
    }
}

void ViewTripDialog::setPersonManager(PERSONMANAGER *manager) {
    qDebug() << "Setting PersonManager in ViewTripDialog:" << (manager != nullptr);
    personManager = manager;

    // Refresh people info if PersonManager is available
    if (personManager) {
        displayPeopleInfo();
    }
}

void ViewTripDialog::setupUI() {
    // MAIN LAYOUT
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // Title
    QLabel *titleLabel = new QLabel("Trip Details", this);
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 20px; "
        "    font-weight: bold; "
        "    color: #2c3e50; "
        "    padding: 10px 0; "
        "    text-align: center; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setMaximumHeight(50);
    mainLayout->addWidget(titleLabel);

    // HORIZONTAL CONTENT LAYOUT - Trip info on left, People info on right
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    // LEFT PANEL - Trip Information (Fixed width)
    setupTripInfoSection();
    QWidget *leftPanelWidget = new QWidget();
    leftPanelWidget->setLayout(new QVBoxLayout());
    leftPanelWidget->layout()->addWidget(tripInfoGroupBox);
    leftPanelWidget->layout()->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    leftPanelWidget->setFixedWidth(500);

    // RIGHT PANEL - People Information (Expandable)
    setupPeopleSection();
    QWidget *rightPanelWidget = new QWidget();
    rightPanelWidget->setLayout(new QVBoxLayout());
    rightPanelWidget->layout()->addWidget(peopleInfoGroupBox);
    rightPanelWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add panels to content layout
    contentLayout->addWidget(leftPanelWidget);
    contentLayout->addWidget(rightPanelWidget, 1);

    mainLayout->addLayout(contentLayout, 1);

    // BUTTONS at bottom
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    editButton = new QPushButton("✏️ Edit Trip", this);
    closeButton = new QPushButton("❌ Close", this);

    // Style buttons
    editButton->setMinimumHeight(40);
    editButton->setMinimumWidth(120);
    editButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #3498db; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #21618c; }");

    closeButton->setMinimumHeight(40);
    closeButton->setMinimumWidth(120);
    closeButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #e74c3c; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        "QPushButton:pressed { background-color: #a93226; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(editButton, &QPushButton::clicked, this, &ViewTripDialog::editCurrentTrip);
    connect(closeButton, &QPushButton::clicked, this, &ViewTripDialog::closeView);
    connect(hostInfoButton, &QPushButton::clicked, this, &ViewTripDialog::onHostInfoClicked);
    connect(membersInfoButton, &QPushButton::clicked, this, &ViewTripDialog::onMemberInfoClicked);

    // Set default focus
    closeButton->setDefault(true);
}

void ViewTripDialog::setupTripInfoSection() {
    tripInfoGroupBox = new QGroupBox("🌍 Trip Information");
    tripInfoGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 16px; "
        "    color: #2c3e50; "
        "    border: 2px solid #3498db; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 15px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QFormLayout *tripLayout = new QFormLayout(tripInfoGroupBox);
    tripLayout->setVerticalSpacing(15);
    tripLayout->setHorizontalSpacing(15);
    tripLayout->setLabelAlignment(Qt::AlignRight);

    // Create labels for trip info
    idLabel = new QLabel();
    destinationLabel = new QLabel();
    descriptionDisplay = new QTextEdit();
    startDateLabel = new QLabel();
    endDateLabel = new QLabel();
    statusLabel = new QLabel();

    // Style description text edit
    descriptionDisplay->setReadOnly(true);
    descriptionDisplay->setMaximumHeight(120);
    descriptionDisplay->setStyleSheet(
        "QTextEdit { "
        "    border: 2px solid #ddd; "
        "    border-radius: 5px; "
        "    padding: 8px; "
        "    background-color: #f8f9fa; "
        "    font-size: 13px; "
        "}");

    // Style other labels
    QString labelStyle =
        "QLabel { "
        "    font-size: 14px; "
        "    padding: 8px; "
        "    background-color: #f8f9fa; "
        "    border: 2px solid #ddd; "
        "    border-radius: 5px; "
        "    min-height: 20px; "
        "}";

    idLabel->setStyleSheet(labelStyle);
    destinationLabel->setStyleSheet(labelStyle);
    startDateLabel->setStyleSheet(labelStyle);
    endDateLabel->setStyleSheet(labelStyle);
    statusLabel->setStyleSheet(labelStyle);

    // Create form labels with icons
    QLabel *idLabelText = new QLabel("🆔 Trip ID:");
    QLabel *destLabelText = new QLabel("📍 Destination:");
    QLabel *descLabelText = new QLabel("📝 Description:");
    QLabel *startLabelText = new QLabel("📅 Start Date:");
    QLabel *endLabelText = new QLabel("📅 End Date:");
    QLabel *statusLabelText = new QLabel("📊 Status:");

    QString formLabelStyle = "QLabel { font-weight: bold; font-size: 13px; color: #2c3e50; }";
    idLabelText->setStyleSheet(formLabelStyle);
    destLabelText->setStyleSheet(formLabelStyle);
    descLabelText->setStyleSheet(formLabelStyle);
    startLabelText->setStyleSheet(formLabelStyle);
    endLabelText->setStyleSheet(formLabelStyle);
    statusLabelText->setStyleSheet(formLabelStyle);

    tripLayout->addRow(idLabelText, idLabel);
    tripLayout->addRow(destLabelText, destinationLabel);
    tripLayout->addRow(descLabelText, descriptionDisplay);
    tripLayout->addRow(startLabelText, startDateLabel);
    tripLayout->addRow(endLabelText, endDateLabel);
    tripLayout->addRow(statusLabelText, statusLabel);
}

void ViewTripDialog::setupPeopleSection() {
    peopleInfoGroupBox = new QGroupBox("👥 People Information");
    peopleInfoGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 16px; "
        "    color: #2c3e50; "
        "    border: 2px solid #e67e22; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 15px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QVBoxLayout *peopleLayout = new QVBoxLayout(peopleInfoGroupBox);
    peopleLayout->setSpacing(15);

    // Host section (smaller)
    QGroupBox *hostGroupBox = new QGroupBox("🏠 Trip Host");
    hostGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #8e44ad; "
        "    border: 2px solid #9b59b6; "
        "    border-radius: 6px; "
        "    margin-top: 8px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 8px; "
        "    padding: 0 3px 0 3px; "
        "}");
    hostGroupBox->setMaximumHeight(200);

    QFormLayout *hostLayout = new QFormLayout(hostGroupBox);
    hostLayout->setVerticalSpacing(10);

    hostLabel = new QLabel();
    hostNameLabel = new QLabel();
    hostEmailLabel = new QLabel();
    hostPhoneLabel = new QLabel();
    hostInfoButton = new QPushButton("👤 View Full Host Details");

    // Style host labels
    QString hostLabelStyle =
        "QLabel { "
        "    font-size: 13px; "
        "    padding: 6px 8px; "
        "    background-color: #f4f1f8; "
        "    border: 1px solid #d7b3d7; "
        "    border-radius: 4px; "
        "    min-height: 18px; "
        "}";

    hostLabel->setStyleSheet(hostLabelStyle);
    hostNameLabel->setStyleSheet(hostLabelStyle);
    hostEmailLabel->setStyleSheet(hostLabelStyle);
    hostPhoneLabel->setStyleSheet(hostLabelStyle);

    hostInfoButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #9b59b6; "
        "    color: white; "
        "    border: none; "
        "    padding: 6px 16px; "
        "    border-radius: 5px; "
        "    font-size: 12px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #8e44ad; }"
        "QPushbutton:disabled {"
        "background-color: #bdc3c7;"
        "color: #7f8c8d;"
        "}");

    QLabel *hostIdLbl = new QLabel("ID:");
    QLabel *hostNameLbl = new QLabel("Name:");
    QLabel *hostEmailLbl = new QLabel("Email:");
    QLabel *hostPhoneLbl = new QLabel("Phone:");

    QString hostFormLabelStyle = "QLabel { font-weight: bold; font-size: 12px; color: #8e44ad; }";
    hostIdLbl->setStyleSheet(hostFormLabelStyle);
    hostNameLbl->setStyleSheet(hostFormLabelStyle);
    hostEmailLbl->setStyleSheet(hostFormLabelStyle);
    hostPhoneLbl->setStyleSheet(hostFormLabelStyle);

    hostLayout->addRow(hostIdLbl, hostLabel);
    hostLayout->addRow(hostNameLbl, hostNameLabel);
    hostLayout->addRow(hostEmailLbl, hostEmailLabel);
    hostLayout->addRow(hostPhoneLbl, hostPhoneLabel);
    hostLayout->addRow("", hostInfoButton);

    // Members section (larger)
    QGroupBox *membersGroupBox = new QGroupBox("👥 Trip Member(s)");
    membersGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #2ecc71; "
        "    border-radius: 6px; "
        "    margin-top: 8px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 8px; "
        "    padding: 0 3px 0 3px; "
        "}");
    membersGroupBox->setMinimumHeight(300);

    QVBoxLayout *membersLayout = new QVBoxLayout(membersGroupBox);
    membersLayout->setSpacing(10);

    membersCountLabel = new QLabel();
    membersCountLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    padding: 8px; "
        "    background-color: #e8f8f5; "
        "    border: 1px solid #a9dfbf; "
        "    border-radius: 4px; "
        "    color: #27ae60; "
        "}");

    membersListWidget = new QListWidget();
    membersListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    membersListWidget->setStyleSheet(
        "QListWidget { "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 5px; "
        "    background-color: #f8fbf8; "
        "    font-size: 13px; "
        "}"
        "QListWidget::item { "
        "    padding: 8px; "
        "    border-bottom: 1px solid #d5f4e6; "
        "}"
        "QListWidget::item:hover { "
        "    background-color: #e8f8f5; "
        "}"
        "QListWidget::item:selected { "
        "    background-color: #2ecc71; "
        "    color: white; "
        "}");

    membersInfoButton = new QPushButton("👥 View All Member(s) Details");
    membersInfoButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #2ecc71; "
        "    color: white; "
        "    border: none; "
        "    padding: 8px 16px; "
        "    border-radius: 5px; "
        "    font-size: 12px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #27ae60; }"
        "QPushbutton:disabled {"
        "background-color: #bdc3c7;"
        "color: #7f8c8d;"
        "}");

    membersLayout->addWidget(membersCountLabel);
    membersLayout->addWidget(membersListWidget, 1);  // Give list most space
    membersLayout->addWidget(membersInfoButton);

    // Add sections to people layout with proper proportions
    peopleLayout->addWidget(hostGroupBox, 1);     // 25% weight
    peopleLayout->addWidget(membersGroupBox, 3);  // 75% weight
}

// Keep all the existing display and event handler methods unchanged...
void ViewTripDialog::displayCurrentTrip() {
    // Display trip information
    idLabel->setText(QString::fromStdString(trip.getID()));
    destinationLabel->setText(QString::fromStdString(trip.getDestination()));
    descriptionDisplay->setPlainText(QString::fromStdString(trip.getDescription()));
    startDateLabel->setText(QString::fromStdString(trip.getStartDate().toString()));
    endDateLabel->setText(QString::fromStdString(trip.getEndDate().toString()));

    // Format status with color
    QString status = QString::fromStdString(statusToString(trip.getStatus()));
    QString statusColor;
    QString statusIcon;

    if (status == "Planned") {
        statusColor = "#3498db";
        statusIcon = "🗓️";
    } else if (status == "Ongoing") {
        statusColor = "#f39c12";
        statusIcon = "🚀";
    } else if (status == "Completed") {
        statusColor = "#27ae60";
        statusIcon = "✅";
    } else if (status == "Cancelled") {
        statusColor = "#e74c3c";
        statusIcon = "❌";
    }

    statusLabel->setText(QString("%1 %2").arg(statusIcon).arg(status));
    statusLabel->setStyleSheet(QString("QLabel { "
                                       "    font-size: 14px; "
                                       "    font-weight: bold; "
                                       "    padding: 8px; "
                                       "    background-color: %1; "
                                       "    color: white; "
                                       "    border-radius: 4px; "
                                       "}")
                                   .arg(statusColor));
}

void ViewTripDialog::displayPeopleInfo() {
    qDebug() << "=== displayPeopleInfo() ===";
    qDebug() << "PersonManager available:" << (personManager != nullptr);

    // Display host information
    HOST host = trip.getHost();
    qDebug() << "Trip host ID:" << QString::fromStdString(host.getID());
    qDebug() << "Host name:" << QString::fromStdString(host.getFullName());

    if (!host.getID().empty()) {
        // FIX: Check if PersonManager is available for additional host details
        if (personManager) {
            // Try to get fresh host data from PersonManager
            HOST freshHost = personManager->getHostByID(host.getID());
            if (!freshHost.getID().empty()) {
                host = freshHost;  // Use fresh data
                qDebug() << "Using fresh host data from PersonManager";
            } else {
                qDebug() << "Host not found in PersonManager, using trip data";
            }
        }

        hostLabel->setText(QString::fromStdString(host.getID()));
        hostNameLabel->setText(QString::fromStdString(host.getFullName()));
        hostEmailLabel->setText(QString::fromStdString(host.getEmail()));
        hostPhoneLabel->setText(QString::fromStdString(host.getPhoneNumber()));
        hostInfoButton->setEnabled(true);
    } else {
        hostLabel->setText("No host assigned");
        hostNameLabel->setText("N/A");
        hostEmailLabel->setText("N/A");
        hostPhoneLabel->setText("N/A");
        hostInfoButton->setEnabled(false);
    }

    // Display members information
    vector<MEMBER> members = trip.getMembers();
    qDebug() << "Trip has" << members.size() << "members";

    // FIX: Try to get fresh member data from PersonManager if available
    if (personManager && !members.empty()) {
        vector<MEMBER> freshMembers;
        for (const MEMBER &member : members) {
            if (!member.getID().empty()) {
                MEMBER freshMember = personManager->getMemberByID(member.getID());
                if (!freshMember.getID().empty()) {
                    freshMembers.push_back(freshMember);
                    qDebug() << "Updated member:" << QString::fromStdString(freshMember.getFullName());
                } else {
                    freshMembers.push_back(member);  // Keep original if not found
                    qDebug() << "Member not found in PersonManager, keeping original:"
                             << QString::fromStdString(member.getFullName());
                }
            }
        }
        members = freshMembers;
    }

    membersCountLabel->setText(QString("Total Member(s): %1").arg(members.size()));

    membersListWidget->clear();
    if (members.empty()) {
        QListWidgetItem *noMembersItem = new QListWidgetItem("No members assigned to this trip");
        noMembersItem->setFlags(Qt::NoItemFlags);  // Make it non-selectable
        noMembersItem->setData(Qt::ForegroundRole, QColor("#7f8c8d"));
        membersListWidget->addItem(noMembersItem);
        membersInfoButton->setEnabled(false);
    } else {
        for (size_t i = 0; i < members.size(); i++) {
            const MEMBER &member = members[i];
            if (!member.getID().empty()) {
                QString memberInfo = QString("👤 %1").arg(QString::fromStdString(member.getInfo()));
                QListWidgetItem *item = new QListWidgetItem(memberInfo);
                item->setData(Qt::UserRole, static_cast<int>(i));
                membersListWidget->addItem(item);
            }
        }
        membersInfoButton->setEnabled(true);
    }

    qDebug() << "=== displayPeopleInfo() completed ===";
}

void ViewTripDialog::onHostInfoClicked() {
    HOST host = trip.getHost();
    if (!host.getID().empty()) {
        // Create a custom dialog for better presentation
        QDialog *hostDialog = new QDialog(this);
        hostDialog->setWindowTitle("Host Details");
        hostDialog->setModal(true);
        hostDialog->setMinimumSize(600, 400);
        hostDialog->resize(700, 600);
        hostDialog->setMaximumSize(900, 700);

        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(hostDialog);
        mainLayout->setSpacing(15);
        mainLayout->setContentsMargins(20, 20, 20, 20);

        // Title
        QLabel *titleLabel = new QLabel("🏠 Host Information");
        titleLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 18px; "
            "    font-weight: bold; "
            "    color: #8e44ad; "
            "    padding: 10px; "
            "    background-color: #f4f1f8; "
            "    border: 2px solid #9b59b6; "
            "    border-radius: 8px; "
            "    text-align: center; "
            "}");
        titleLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(titleLabel);

        // Content area with scroll
        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);

        QWidget *contentWidget = new QWidget();
        QFormLayout *formLayout = new QFormLayout(contentWidget);
        formLayout->setVerticalSpacing(12);
        formLayout->setHorizontalSpacing(15);

        // Host details
        QLabel *idLabel = new QLabel(QString::fromStdString(host.getID()));
        QLabel *nameLabel = new QLabel(QString::fromStdString(host.getFullName()));
        QLabel *emailLabel = new QLabel(QString::fromStdString(host.getEmail()));
        QLabel *phoneLabel = new QLabel(QString::fromStdString(host.getPhoneNumber()));
        QLabel *genderLabel = new QLabel(QString::fromStdString(genderToString(host.getGender())));
        QLabel *dobLabel = new QLabel(QString::fromStdString(host.getDateOfBirth().toString()));
        QLabel *addressLabel = new QLabel(QString::fromStdString(host.getAddress()));
        QLabel *emergencyLabel = new QLabel(QString::fromStdString(host.getEmergencyContact()));

        // Style for data labels
        QString dataLabelStyle =
            "QLabel { "
            "    font-size: 13px; "
            "    padding: 8px 12px; "
            "    background-color: #fdfdfe; "
            "    border: 1px solid #d7b3d7; "
            "    border-radius: 6px; "
            "    color: #2c3e50; "
            "    font-weight: 500; "
            "}";

        idLabel->setStyleSheet(dataLabelStyle);
        nameLabel->setStyleSheet(dataLabelStyle);
        emailLabel->setStyleSheet(dataLabelStyle);
        phoneLabel->setStyleSheet(dataLabelStyle);
        genderLabel->setStyleSheet(dataLabelStyle);
        dobLabel->setStyleSheet(dataLabelStyle);
        addressLabel->setStyleSheet(dataLabelStyle);
        emergencyLabel->setStyleSheet(dataLabelStyle);

        // Style for field labels
        QString fieldLabelStyle =
            "QLabel { "
            "    font-weight: bold; "
            "    font-size: 12px; "
            "    color: #8e44ad; "
            "    padding: 4px; "
            "}";

        QLabel *idFieldLabel = new QLabel("🆔 ID:");
        QLabel *nameFieldLabel = new QLabel("👤 Full Name:");
        QLabel *emailFieldLabel = new QLabel("📧 Email:");
        QLabel *phoneFieldLabel = new QLabel("📱 Phone:");
        QLabel *genderFieldLabel = new QLabel("⚧️ Gender:");
        QLabel *dobFieldLabel = new QLabel("🎂 Date of Birth:");
        QLabel *addressFieldLabel = new QLabel("🏠 Address:");
        QLabel *emergencyFieldLabel = new QLabel("🚨 Emergency Contact:");

        idFieldLabel->setStyleSheet(fieldLabelStyle);
        nameFieldLabel->setStyleSheet(fieldLabelStyle);
        emailFieldLabel->setStyleSheet(fieldLabelStyle);
        phoneFieldLabel->setStyleSheet(fieldLabelStyle);
        genderFieldLabel->setStyleSheet(fieldLabelStyle);
        dobFieldLabel->setStyleSheet(fieldLabelStyle);
        addressFieldLabel->setStyleSheet(fieldLabelStyle);
        emergencyFieldLabel->setStyleSheet(fieldLabelStyle);

        // Add to form
        formLayout->addRow(idFieldLabel, idLabel);
        formLayout->addRow(nameFieldLabel, nameLabel);
        formLayout->addRow(emailFieldLabel, emailLabel);
        formLayout->addRow(phoneFieldLabel, phoneLabel);
        formLayout->addRow(genderFieldLabel, genderLabel);
        formLayout->addRow(dobFieldLabel, dobLabel);
        formLayout->addRow(addressFieldLabel, addressLabel);
        formLayout->addRow(emergencyFieldLabel, emergencyLabel);

        scrollArea->setWidget(contentWidget);
        mainLayout->addWidget(scrollArea);

        // Close button
        QPushButton *closeBtn = new QPushButton("✅ Close");
        closeBtn->setStyleSheet(
            "QPushButton { "
            "    background-color: #9b59b6; "
            "    color: white; "
            "    border: none; "
            "    padding: 10px 20px; "
            "    border-radius: 6px; "
            "    font-size: 13px; "
            "    font-weight: bold; "
            "}"
            "QPushButton:hover { background-color: #8e44ad; }"
            "QPushButton:pressed { background-color: #7d3c98; }");

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(closeBtn);
        buttonLayout->addStretch();
        mainLayout->addLayout(buttonLayout);

        connect(closeBtn, &QPushButton::clicked, hostDialog, &QDialog::accept);

        // Center dialog
        if (this->parentWidget()) {
            hostDialog->move(this->geometry().center() - hostDialog->rect().center());
        }

        hostDialog->exec();
        delete hostDialog;
    }
}

void ViewTripDialog::onMemberInfoClicked() {
    vector<MEMBER> members = trip.getMembers();
    if (members.empty()) {
        QMessageBox::information(this, "Members Information", "No members assigned to this trip.");
        return;
    }

    // Create a custom dialog for better presentation
    QDialog *membersDialog = new QDialog(this);
    membersDialog->setWindowTitle("Members Details");
    membersDialog->setModal(true);
    membersDialog->setMinimumSize(600, 500);
    membersDialog->resize(650, 600);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(membersDialog);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel(QString("👥 Members Details"));
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 18px; "
        "    font-weight: bold; "
        "    color: #27ae60; "
        "    padding: 12px; "
        "    background-color: #e8f8f5; "
        "    border: 2px solid #2ecc71; "
        "    border-radius: 8px; "
        "    text-align: center; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Scroll area for members
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { "
        "    background-color: #f8fbf8; "
        "    border: 1px solid #a9dfbf; "
        "    border-radius: 6px; "
        "}");

    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(15);
    contentLayout->setContentsMargins(15, 15, 15, 15);

    // Add each member
    for (size_t i = 0; i < members.size(); ++i) {
        const MEMBER &member = members[i];
        if (!member.getID().empty()) {
            // Member container
            QGroupBox *memberBox = new QGroupBox(QString("%1").arg(QString::fromStdString(member.getFullName())));
            memberBox->setStyleSheet(
                "QGroupBox { "
                "    font-weight: bold; "
                "    font-size: 14px; "
                "    color: #27ae60; "
                "    border: 2px solid #58d68d; "
                "    border-radius: 8px; "
                "    margin-top: 10px; "
                "    padding-top: 15px; "
                "    background-color: #f9fffe; "
                "}"
                "QGroupBox::title { "
                "    subcontrol-origin: margin; "
                "    left: 10px; "
                "    padding: 0 8px 0 8px; "
                "    background-color: #e8f8f5; "
                "    border-radius: 4px; "
                "}");

            QFormLayout *memberLayout = new QFormLayout(memberBox);
            memberLayout->setVerticalSpacing(8);
            memberLayout->setHorizontalSpacing(12);

            // Member data labels
            QLabel *idLabel = new QLabel(QString::fromStdString(member.getID()));
            QLabel *nameLabel = new QLabel(QString::fromStdString(member.getFullName()));
            QLabel *emailLabel = new QLabel(QString::fromStdString(member.getEmail()));
            QLabel *dobLabel = new QLabel(QString::fromStdString(member.getDateOfBirth().toString()));
            QLabel *phoneLabel = new QLabel(QString::fromStdString(member.getPhoneNumber()));
            QLabel *genderLabel = new QLabel(QString::fromStdString(genderToString(member.getGender())));
            QLabel *spentLabel = new QLabel(QString("$%1").arg(member.getTotalSpent(), 0, 'f', 2));

            // Style for member data labels
            QString memberDataStyle =
                "QLabel { "
                "    font-size: 12px; "
                "    padding: 6px 10px; "
                "    background-color: #fdfefd; "
                "    border: 1px solid #a9dfbf; "
                "    border-radius: 4px; "
                "    color: #1e4d28; "
                "}";

            idLabel->setStyleSheet(memberDataStyle);
            nameLabel->setStyleSheet(memberDataStyle);
            emailLabel->setStyleSheet(memberDataStyle);
            dobLabel->setStyleSheet(memberDataStyle);
            phoneLabel->setStyleSheet(memberDataStyle);
            genderLabel->setStyleSheet(memberDataStyle);
            spentLabel->setStyleSheet(memberDataStyle + "QLabel { font-weight: bold; color: #196f3d; }");

            // Field labels
            QString memberFieldStyle =
                "QLabel { "
                "    font-weight: bold; "
                "    font-size: 11px; "
                "    color: #27ae60; "
                "    padding: 2px; "
                "}";

            QLabel *idField = new QLabel("🆔 ID:");
            QLabel *nameField = new QLabel("👤 Name:");
            QLabel *emailField = new QLabel("📧 Email:");
            QLabel *dobField = new QLabel("🗓️ Date of birth:");
            QLabel *phoneField = new QLabel("📱 Phone:");
            QLabel *genderField = new QLabel("⚧️ Gender:");
            QLabel *spentField = new QLabel("💰 Total Spent:");

            idField->setStyleSheet(memberFieldStyle);
            nameField->setStyleSheet(memberFieldStyle);
            emailField->setStyleSheet(memberFieldStyle);
            dobField->setStyleSheet(memberFieldStyle);
            phoneField->setStyleSheet(memberFieldStyle);
            genderField->setStyleSheet(memberFieldStyle);
            spentField->setStyleSheet(memberFieldStyle);

            // Add to member layout
            memberLayout->addRow(idField, idLabel);
            memberLayout->addRow(nameField, nameLabel);
            memberLayout->addRow(emailField, emailLabel);
            memberLayout->addRow(dobField, dobLabel);
            memberLayout->addRow(phoneField, phoneLabel);
            memberLayout->addRow(genderField, genderLabel);
            memberLayout->addRow(spentField, spentLabel);

            contentLayout->addWidget(memberBox);
        }
    }

    contentLayout->addStretch();
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    // Bottom stats
    QLabel *statsLabel = new QLabel(QString("📊 Total Members: %1").arg(members.size()));
    statsLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    color: #27ae60; "
        "    padding: 8px 15px; "
        "    background-color: #e8f8f5; "
        "    border: 1px solid #a9dfbf; "
        "    border-radius: 6px; "
        "}");
    statsLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statsLabel);

    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Close");
    closeBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #2ecc71; "
        "    color: white; "
        "    border: none; "
        "    padding: 10px 20px; "
        "    border-radius: 6px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #27ae60; }"
        "QPushButton:pressed { background-color: #229954; }");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    connect(closeBtn, &QPushButton::clicked, membersDialog, &QDialog::accept);

    // Center dialog
    if (this->parentWidget()) {
        membersDialog->move(this->geometry().center() - membersDialog->rect().center());
    }

    membersDialog->exec();
    delete membersDialog;
}

void ViewTripDialog::editCurrentTrip() {
    EditTripDialog editDialog(trip, this);
    editDialog.setPersonManager(personManager);
    if (editDialog.exec() == QDialog::Accepted) {
        trip = editDialog.getUpdatedTrip();
        displayCurrentTrip();
        displayPeopleInfo();  // Refresh people info as well
    }
}

void ViewTripDialog::closeView() { accept(); }

TRIP ViewTripDialog::getUpdatedTrip() const { return trip; }

TRIP ViewTripDialog::getOriginalTrip() const { return originalTrip; }
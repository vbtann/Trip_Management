#include "ManagePeopleDialog.h"

#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>

#include "../Managers/FileManager.h"
#include "AddPersonDialog.h"

ManagePeopleDialog::ManagePeopleDialog(PERSONMANAGER *personManager, QWidget *parent)
    : QDialog(parent), personManager(personManager) {
    setWindowTitle("Manage People");
    setWindowIcon(QIcon(":/icons/people.png"));
    setModal(true);
    setMinimumSize(700, 500);

    // if (personManager) {
    //     personManager->addObserver(this);
    // }

    setupUI();
    styleComponents();
    refreshPersonList();

    // Center the dialog
    if (parent) {
        move(parent->geometry().center() - rect().center());
    }
}

void ManagePeopleDialog::setupUI() {
    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Left side - Person list
    QWidget *leftWidget = new QWidget();
    leftWidget->setMinimumWidth(300);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    // Title and stats
    QLabel *titleLabel = new QLabel("People Management");
    titleLabel->setStyleSheet(
        "QLabel { font-size: 18px; font-weight: bold; "
        "color: #2c3e50; margin: 10px; }");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Statistics group
    QGroupBox *statsGroup = new QGroupBox("Statistics");
    QGridLayout *statsLayout = new QGridLayout(statsGroup);

    personCountLabel = new QLabel("Total People: 0");
    memberCountLabel = new QLabel("Members: 0");
    hostCountLabel = new QLabel("Hosts: 0");

    statsLayout->addWidget(personCountLabel, 0, 0);
    statsLayout->addWidget(memberCountLabel, 1, 0);
    statsLayout->addWidget(hostCountLabel, 2, 0);

    // People list
    QLabel *listLabel = new QLabel("People List:");
    listLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; }");

    peopleListWidget = new QListWidget();
    peopleListWidget->setMinimumHeight(250);

    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(statsGroup);
    leftLayout->addWidget(listLabel);
    leftLayout->addWidget(peopleListWidget);

    // Right side - Actions and info
    QWidget *rightWidget = new QWidget();
    rightWidget->setMinimumWidth(350);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    // Actions group
    QGroupBox *actionsGroup = new QGroupBox("Actions");
    QVBoxLayout *actionsLayout = new QVBoxLayout(actionsGroup);

    addPersonButton = new QPushButton("➕ Add New Person");
    editPersonButton = new QPushButton("✏️ Edit Selected Person");
    deletePersonButton = new QPushButton("🗑️ Delete Selected Person");
    viewPersonButton = new QPushButton("👁️ View Person Summary");

    // Initially disable edit/delete/view buttons
    editPersonButton->setEnabled(false);
    deletePersonButton->setEnabled(false);
    viewPersonButton->setEnabled(false);

    actionsLayout->addWidget(addPersonButton);
    actionsLayout->addWidget(editPersonButton);
    actionsLayout->addWidget(deletePersonButton);
    actionsLayout->addWidget(viewPersonButton);
    actionsLayout->addStretch();

    // Import/Export buttons
    QHBoxLayout *importExportLayout = new QHBoxLayout();

    importPeopleButton = new QPushButton("📥 Import People");
    exportPeopleButton = new QPushButton("📤 Export People");

    importExportLayout->addWidget(importPeopleButton);
    importExportLayout->addWidget(exportPeopleButton);

    actionsLayout->addLayout(importExportLayout);

    // Selected person info group
    QGroupBox *infoGroup = new QGroupBox("Selected Person Information");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);

    selectedPersonInfoLabel = new QLabel("No person selected");
    selectedPersonInfoLabel->setWordWrap(true);
    selectedPersonInfoLabel->setMinimumHeight(200);
    selectedPersonInfoLabel->setStyleSheet(
        "QLabel { "
        "border: 1px solid #ccc; "
        "padding: 10px; "
        "font-size: 13px;"
        "background-color: #f9f9f9; "
        "border-radius: 5px; "
        "}");

    infoLayout->addWidget(selectedPersonInfoLabel);

    // Control buttons
    QHBoxLayout *controlLayout = new QHBoxLayout();
    refreshButton = new QPushButton("🔄 Refresh");
    closeButton = new QPushButton("❌ Close");

    controlLayout->addWidget(refreshButton);
    controlLayout->addStretch();
    controlLayout->addWidget(closeButton);

    rightLayout->addWidget(actionsGroup);
    rightLayout->addWidget(infoGroup);
    rightLayout->addStretch();
    rightLayout->addLayout(controlLayout);

    // Add to main layout
    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(rightWidget);

    // Connect signals
    connect(addPersonButton, &QPushButton::clicked, this, &ManagePeopleDialog::onAddPersonClicked);
    connect(editPersonButton, &QPushButton::clicked, this, &ManagePeopleDialog::onEditPersonClicked);
    connect(deletePersonButton, &QPushButton::clicked, this, &ManagePeopleDialog::onDeletePersonClicked);
    connect(viewPersonButton, &QPushButton::clicked, this, &ManagePeopleDialog::onViewPersonClicked);
    connect(refreshButton, &QPushButton::clicked, this, &ManagePeopleDialog::onRefreshClicked);
    connect(closeButton, &QPushButton::clicked, this, &ManagePeopleDialog::onCloseClicked);
    connect(peopleListWidget, &QListWidget::itemSelectionChanged, this, &ManagePeopleDialog::onPersonSelectionChanged);
    connect(importPeopleButton, &QPushButton::clicked, this, &ManagePeopleDialog::onImportPeopleClicked);
    connect(exportPeopleButton, &QPushButton::clicked, this, &ManagePeopleDialog::onExportPeopleClicked);
}

void ManagePeopleDialog::styleComponents() {
    // Style the list widget
    peopleListWidget->setStyleSheet(
        "QListWidget { "
        "border: 2px solid #bdc3c7; "
        "border-radius: 5px; "
        "background-color: white; "
        "selection-background-color: #3498db; "
        "}"
        "QListWidget::item { "
        "padding: 8px; "
        "border-bottom: 1px solid #ecf0f1; "
        "}"
        "QListWidget::item:selected { "
        "background-color: #3498db; "
        "color: white; "
        "}"
        "QListWidget::item:hover { "
        "background-color: #ecf0f1; "
        "}");

    // Style buttons
    QString primaryButtonStyle =
        "QPushButton { "
        "background-color: #3498db; "
        "color: white; "
        "border: none; "
        "padding: 10px 15px; "
        "border-radius: 5px; "
        "font-size: 13px; "
        "font-weight: bold; "
        "margin: 2px; "
        "}"
        "QPushButton:hover { "
        "background-color: #2980b9; "
        "}"
        "QPushButton:pressed { "
        "background-color: #21618c; "
        "}"
        "QPushButton:disabled { "
        "background-color: #bdc3c7; "
        "color: #7f8c8d; "
        "}";

    QString dangerButtonStyle =
        "QPushButton { "
        "background-color: #e74c3c; "
        "color: white; "
        "border: none; "
        "padding: 10px 15px; "
        "border-radius: 5px; "
        "font-size: 13px; "
        "font-weight: bold; "
        "margin: 2px; "
        "}"
        "QPushButton:hover { "
        "background-color: #c0392b; "
        "}"
        "QPushButton:pressed { "
        "background-color: #a93226; "
        "}"
        "QPushButton:disabled { "
        "background-color: #bdc3c7; "
        "color: #7f8c8d; "
        "}";

    addPersonButton->setStyleSheet(
        primaryButtonStyle.replace("#3498db", "#27ae60").replace("#2980b9", "#229954").replace("#21618c", "#1e8449"));
    editPersonButton->setStyleSheet(primaryButtonStyle);
    viewPersonButton->setStyleSheet(primaryButtonStyle);
    deletePersonButton->setStyleSheet(dangerButtonStyle);
    refreshButton->setStyleSheet(
        primaryButtonStyle.replace("#3498db", "#f39c12").replace("#2980b9", "#e67e22").replace("#21618c", "#d35400"));
    closeButton->setStyleSheet(
        primaryButtonStyle.replace("#3498db", "#95a5a6").replace("#2980b9", "#7f8c8d").replace("#21618c", "#6c7b7d"));
}

void ManagePeopleDialog::refreshPersonList() {
    peopleListWidget->clear();

    vector<PERSON *> allPeople = personManager->getAllPeople();

    for (PERSON *person : allPeople) {
        QString itemText = QString("%1 - %2 (%3)")
                               .arg(QString::fromStdString(person->getID()))
                               .arg(QString::fromStdString(person->getFullName()))
                               .arg(QString::fromStdString(person->getRole()));

        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, QString::fromStdString(person->getID()));
        peopleListWidget->addItem(item);
    }

    updatePersonInfo();
}

void ManagePeopleDialog::updatePersonInfo() {
    // Update statistics
    size_t totalPeople = personManager->getPersonCount();
    size_t memberCount = personManager->getMemberCount();
    size_t hostCount = personManager->getHostCount();

    personCountLabel->setText(QString("Total People: %1").arg(totalPeople));
    memberCountLabel->setText(QString("Members: %1").arg(memberCount));
    hostCountLabel->setText(QString("Hosts: %1").arg(hostCount));

    // Update selected person info
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    if (currentItem) {
        QString personId = currentItem->data(Qt::UserRole).toString();
        PERSON *person = personManager->findPersonById(personId.toStdString());

        if (person) {
            QString info = QString(
                               "<b>ID:</b> %1<br>"
                               "<b>Name:</b> %2<br>"
                               "<b>Role:</b> %3<br>"
                               "<b>Gender:</b> %4<br>"
                               "<b>Age:</b> %5<br>"
                               "<b>Email:</b> %6<br>"
                               "<b>Phone:</b> %7<br>"
                               "<b>Address:</b> %8<br>")
                               .arg(QString::fromStdString(person->getID()))
                               .arg(QString::fromStdString(person->getFullName()))
                               .arg(QString::fromStdString(person->getRole()))
                               .arg(QString::fromStdString(genderToString(person->getGender())))
                               .arg(person->getAge())
                               .arg(QString::fromStdString(person->getEmail()))
                               .arg(QString::fromStdString(person->getPhoneNumber()))
                               .arg(QString::fromStdString(person->getAddress()));

            selectedPersonInfoLabel->setText(info);
        }
    } else {
        selectedPersonInfoLabel->setText("No person selected");
    }
}

void ManagePeopleDialog::onAddPersonClicked() {
    AddPersonDialog dialog(personManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        PERSON *newPerson = dialog.getPersonData();
        if (newPerson) {
            personManager->addPerson(newPerson);
            refreshPersonList();
            QMessageBox::information(this, "Success", "Person added successfully!");
        }
    }
}

void ManagePeopleDialog::onEditPersonClicked() {
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "No Selection", "Please select a person to edit.");
        return;
    }

    QString personId = currentItem->data(Qt::UserRole).toString();
    PERSON *person = personManager->findPersonById(personId.toStdString());

    if (!person) {
        QMessageBox::warning(this, "Error", "Selected person not found.");
        return;
    }

    EditPersonDialog dialog(person, personManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        PERSON *updatedPerson = dialog.getUpdatedPerson();
        if (updatedPerson) {
            // Update the person in the manager
            if (personManager->updatePerson(*person, *updatedPerson)) {
                refreshPersonList();
                QMessageBox::information(this, "Success", "Person updated successfully!");
            } else {
                QMessageBox::warning(this, "Error", "Failed to update person.");
                delete updatedPerson;
            }
        }
    }
}

void ManagePeopleDialog::onDeletePersonClicked() {
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "No Selection", "Please select a person to delete.");
        return;
    }

    QString personId = currentItem->data(Qt::UserRole).toString();
    PERSON *person = personManager->findPersonById(personId.toStdString());

    if (person) {
        int ret = QMessageBox::question(
            this, "Delete Person",
            QString("Are you sure you want to delete %1?").arg(QString::fromStdString(person->getFullName())),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (ret == QMessageBox::Yes) {
            if (personManager->removePerson(personId.toStdString())) {
                refreshPersonList();
                QMessageBox::information(this, "Success", "Person deleted successfully!");
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete person.");
            }
        }
    }
}

void ManagePeopleDialog::onViewPersonClicked() {
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "No Selection", "Please select a person to view.");
        return;
    }

    QString personId = currentItem->data(Qt::UserRole).toString();
    PERSON *person = personManager->findPersonById(personId.toStdString());

    if (person) {
        QString detailedInfo = QString::fromStdString(person->getInfo());
        QMessageBox::information(this, "Person Details", detailedInfo);
    }
}

void ManagePeopleDialog::onPersonSelectionChanged() {
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    bool hasSelection = (currentItem != nullptr);

    editPersonButton->setEnabled(hasSelection);
    deletePersonButton->setEnabled(hasSelection);
    viewPersonButton->setEnabled(hasSelection);

    updatePersonInfo();
}

void ManagePeopleDialog::onRefreshClicked() { refreshPersonList(); }

void ManagePeopleDialog::onCloseClicked() { accept(); }

void ManagePeopleDialog::onImportPeopleClicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Import People from CSV", QDir::homePath(),
                                                    "CSV Files (*.csv);;All Files (*.*)");

    if (filename.isEmpty()) {
        return;  // User canceled
    }

    try {
        // Get current people to merge with imported ones
        vector<PERSON *> currentPeople = personManager->getAllPeople();
        vector<PERSON *> importedPeople;

        // Import new people
        importPeopleInfo(importedPeople, filename.toStdString());

        if (importedPeople.empty()) {
            QMessageBox::warning(this, "Import Failed", "No people found in the file or the file format is incorrect.");
            return;
        }

        int importCount = 0;

        // Add each imported person if they don't already exist
        for (PERSON *person : importedPeople) {
            // Check if a person with the same ID already exists
            if (!personManager->findPersonById(person->getID())) {
                personManager->addPerson(person);
                importCount++;
            } else {
                // Duplicate found, free memory
                delete person;
            }
        }

        // Refresh the list
        refreshPersonList();

        QMessageBox::information(this, "Import Successful",
                                 QString("Successfully imported %1 people.").arg(importCount));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Import Error", QString("An error occurred during import: %1").arg(e.what()));
    }
}

void ManagePeopleDialog::onExportPeopleClicked() {
    QString filename = QFileDialog::getSaveFileName(
        this, "Export People to CSV", QDir::homePath() + "/people_export.csv", "CSV Files (*.csv);;All Files (*.*)");

    if (filename.isEmpty()) {
        return;  // User canceled
    }

    try {
        vector<PERSON *> people = personManager->getAllPeople();

        if (people.empty()) {
            QMessageBox::warning(this, "Export Failed", "There are no people to export.");
            return;
        }

        exportPeopleInfo(people, filename.toStdString());

        QMessageBox::information(
            this, "Export Successful",
            QString("Successfully exported %1 people to %2.").arg(people.size()).arg(QFileInfo(filename).fileName()));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Export Error", QString("An error occurred during export: %1").arg(e.what()));
    }
}

// void ManagePeopleDialog::onPersonAdded(const string &personID) {
//     refreshPersonList();  // Refresh the list when a person is added
// }

// void ManagePeopleDialog::onPersonRemoved(const string &personID) {
//     refreshPersonList();  // Refresh the list when a person is removed
// }

// void ManagePeopleDialog::onPersonUpdated(const string &personID) {
//     refreshPersonList();  // Refresh the list when a person is updated
// }
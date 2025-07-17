#include "MainWindow.h"

#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>

#include "../Managers/FileManager.h"
#include "../Managers/Observer.h"
#include "../Managers/PersonFactory.h"
#include "../Managers/TripFactory.h"
#include "../Managers/TripManager.h"
#include "../Models/header.h"
#include "AddPersonDialog.h"  // Include the new dialog header
#include "AddTripDialog.h"
#include "EditTripDialog.h"
#include "FilterTripDialog.h"
#include "ViewTripDialog.h"

// ========================================
// CONSTRUCTOR & DESTRUCTOR
// ========================================

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Initialize managers first
    personManager = new PERSONMANAGER();
    tripManager = new TRIPMANAGER();

    // Register as observer for both managers
    personManager->addObserver(this);
    tripManager->addObserver(this);

    setupUI();
    setupStatusBar();
    setMinimumSize(1200, 900);

    // IMPORTANT: Load people BEFORE loading trips (so attendees can be restored)
    addDebugMessage("Loading people from cache...");
    // PersonManager automatically loads people in its constructor

    // Now load trips with attendees restoration
    vector<TRIP> cachedTrips;
    loadCacheFromFile(cachedTrips);  // This now includes attendees restoration

    // Add cached trips to TripManager (which will trigger observer updates)
    for (const TRIP &trip : cachedTrips) {
        tripManager->addTrip(trip);
    }

    addDebugMessage("Application initialization completed");
}

MainWindow::~MainWindow() {
    addDebugMessage("Saving application state before exit...");

    // Save trips with attendees
    saveCacheToFile();

    // PersonManager will save people in its destructor

    // Clean up managers
    if (personManager) {
        personManager->removeObserver(this);
        delete personManager;
    }

    if (tripManager) {
        tripManager->removeObserver(this);
        delete tripManager;
    }

    addDebugMessage("Application shutdown completed");
}

// ========================================
// UI SETUP FUNCTIONS (Order of Execution)
// ========================================

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // The splitter's parent will be set when it's added to the layout.
    mainSplitter = new QSplitter(Qt::Horizontal);

    setupSidebar();
    setupMainContent();

    mainSplitter->addWidget(sidebar);
    mainSplitter->addWidget(mainContent);
    mainSplitter->setStretchFactor(0, 0);
    mainSplitter->setStretchFactor(1, 1);
    mainSplitter->setSizes({250, 950});

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(mainSplitter);
    mainLayout->setContentsMargins(0, 0, 0, 0);
}

void MainWindow::setupSidebar() {
    sidebar = new QWidget();
    sidebar->setMaximumWidth(250);
    sidebar->setStyleSheet(
        "QWidget { background-color: #f5f5f5; }"
        "QGroupBox { font-weight: bold; margin-top: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 "
        "5px 0 5px; }"
        "QPushButton { text-align: left; padding: 8px 12px; margin: 2px; "
        "border-radius: 4px; }"
        "QPushButton:hover { background-color: #e3f2fd; }"
        "QPushButton:pressed { background-color: #bbdefb; }");

    sidebarLayout = new QVBoxLayout(sidebar);

    // Quick Actions Group
    quickActionsGroup = new QGroupBox("📁 File Operations");
    QVBoxLayout *quickLayout = new QVBoxLayout(quickActionsGroup);
    importButton = new QPushButton("📥 Import Trips");
    exportButton = new QPushButton("📤 Export Trips");
    saveButton = new QPushButton("💾 Save Project");
    loadButton = new QPushButton("📂 Load Project");

    quickLayout->addWidget(importButton);
    quickLayout->addWidget(exportButton);
    quickLayout->addWidget(saveButton);
    quickLayout->addWidget(loadButton);

    // Trip Management Group
    managementGroup = new QGroupBox("🎯 Trip Management");
    QVBoxLayout *mgmtLayout = new QVBoxLayout(managementGroup);
    addTripButton = new QPushButton("➕ Add New Trip");
    editTripButton = new QPushButton("✏️ Edit Trip");
    deleteButton = new QPushButton("🗑️ Delete Trip");
    duplicateButton = new QPushButton("📋 Duplicate Trip");
    detailViewButton = new QPushButton("🔍 View Detail");

    addTripButton->setStyleSheet(
        "QPushButton { background-color: #4caf50; color: white; font-weight: "
        "bold; }");
    deleteButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; }");

    mgmtLayout->addWidget(addTripButton);
    mgmtLayout->addWidget(editTripButton);
    mgmtLayout->addWidget(deleteButton);
    mgmtLayout->addWidget(duplicateButton);
    mgmtLayout->addWidget(detailViewButton);

    // People Management Group - MODIFY this section
    peopleGroup = new QGroupBox("👥 People Management");
    QVBoxLayout *peopleLayout = new QVBoxLayout(peopleGroup);

    // Add Import/Export buttons first
    QHBoxLayout *peopleImportExportLayout = new QHBoxLayout();

    importPeopleButton = new QPushButton("📥 Import People");
    importPeopleButton->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; }"
        "QPushButton:hover { background-color: #2980b9; }");

    exportPeopleButton = new QPushButton("📤 Export People");
    exportPeopleButton->setStyleSheet(
        "QPushButton { background-color: #2ecc71; color: white; }"
        "QPushButton:hover { background-color: #27ae60; }");

    peopleImportExportLayout->addWidget(importPeopleButton);
    peopleImportExportLayout->addWidget(exportPeopleButton);

    // Add the horizontal layout to the main layout
    peopleLayout->addLayout(peopleImportExportLayout);

    // Single button for people management
    QPushButton *managePeopleButton = new QPushButton("👥 Manage People");
    managePeopleButton->setStyleSheet(
        "QPushButton { background-color: #9b59b6; color: white; font-weight: "
        "bold; }"
        "QPushButton:hover { background-color: #8e44ad; }");

    peopleLayout->addWidget(managePeopleButton);

    // View and Filter Group
    viewGroup = new QGroupBox("🔍 View and Filter");
    QVBoxLayout *viewLayout = new QVBoxLayout(viewGroup);

    filterButton = new QPushButton("🔽 Filter Trips");
    searchButton = new QPushButton("🔍 Search Trips");
    QPushButton *upcomingButton = new QPushButton("⏰ Upcoming Trips");
    QPushButton *completedButton = new QPushButton("✅ Completed Trips");
    refreshButton = new QPushButton("🔄 Refresh View");

    viewLayout->addWidget(filterButton);
    viewLayout->addWidget(searchButton);
    viewLayout->addWidget(upcomingButton);
    viewLayout->addWidget(completedButton);
    viewLayout->addWidget(refreshButton);

    // Add all groups to sidebar
    sidebarLayout->addWidget(quickActionsGroup);
    sidebarLayout->addWidget(managementGroup);
    sidebarLayout->addWidget(peopleGroup);
    sidebarLayout->addWidget(viewGroup);
    sidebarLayout->addStretch();

    // Connect ALL signals
    connect(importButton, &QPushButton::clicked, this, &MainWindow::onImportTripsClicked);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onExportTripsClicked);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveProjectClicked);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadProjectClicked);
    connect(addTripButton, &QPushButton::clicked, this, &MainWindow::onAddTripClicked);
    connect(editTripButton, &QPushButton::clicked, this, &MainWindow::onEditTripClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteTripClicked);
    connect(duplicateButton, &QPushButton::clicked, this, &MainWindow::onDuplicateTripClicked);
    connect(detailViewButton, &QPushButton::clicked, this, &MainWindow::onViewTripDetailsClicked);
    connect(managePeopleButton, &QPushButton::clicked, this, &MainWindow::onManagePeopleClicked);
    connect(filterButton, &QPushButton::clicked, this, &MainWindow::onFilterTripsClicked);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchTripsClicked);
    connect(upcomingButton, &QPushButton::clicked, this, &MainWindow::onShowUpcomingTripsClicked);
    connect(completedButton, &QPushButton::clicked, this, &MainWindow::onShowCompletedTripsClicked);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshViewClicked);
    connect(importPeopleButton, &QPushButton::clicked, this, &MainWindow::onImportPeopleClicked);
    connect(exportPeopleButton, &QPushButton::clicked, this, &MainWindow::onExportPeopleClicked);
}

void MainWindow::setupMainContent() {
    mainContent = new QWidget();
    mainContentLayout = new QVBoxLayout(mainContent);

    // Header Area
    headerWidget = new QWidget();
    headerWidget->setStyleSheet(
        "QWidget { background-color: white; border-bottom: 2px solid #e0e0e0; "
        "}");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    titleLabel = new QLabel("Trip Management Dashboard");
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #333; }");
    statsLabel = new QLabel("0 trips loaded");
    statsLabel->setStyleSheet("QLabel { font-size: 14px; color: #666; }");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(statsLabel);

    // Trip Display Area
    tripDisplayArea = new QWidget();
    QVBoxLayout *displayLayout = new QVBoxLayout(tripDisplayArea);

    // Trips Table
    tripsTable = new QTableWidget();
    tripsTable->setColumnCount(6);
    QStringList headers = {"ID", "Destination", "Description", "Start Date", "End Date", "Status"};
    tripsTable->setHorizontalHeaderLabels(headers);

    // Style the table
    tripsTable->setStyleSheet(
        "QTableWidget { gridline-color: #e0e0e0; background-color: white; }"
        "QTableWidget::item { padding: 8px; }"
        "QTableWidget::item:selected { background-color: #898989; color: "
        "white; }"
        "QHeaderView::section { background-color: #f5f5f5; font-weight: bold; "
        "border: 1px solid #e0e0e0; }");
    tripsTable->horizontalHeader()->setStretchLastSection(true);
    tripsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tripsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    tripsTable->setAlternatingRowColors(true);
    tripsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    displayLayout->addWidget(tripsTable);

    // Add to main content layout
    mainContentLayout->addWidget(headerWidget);
    mainContentLayout->addWidget(tripDisplayArea);
}

void MainWindow::setupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *importAction = new QAction("&Import Trips", this);
    fileMenu->addAction(importAction);
    connect(importAction, &QAction::triggered, this, &MainWindow::onImportTripsClicked);

    QAction *exportAction = new QAction("&Export Trips", this);
    fileMenu->addAction(exportAction);
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExportTripsClicked);

    fileMenu->addSeparator();
    QAction *exitAction = new QAction("E&xit", this);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QAction *addTripAction = new QAction("&Add Trip", this);
    editMenu->addAction(addTripAction);
    connect(addTripAction, &QAction::triggered, this, &MainWindow::onAddTripClicked);

    QAction *editTripAction = new QAction("&Edit Trip", this);
    editMenu->addAction(editTripAction);
    connect(editTripAction, &QAction::triggered, this, &MainWindow::onEditTripClicked);

    QAction *deleteTripAction = new QAction("&Delete Trip", this);
    editMenu->addAction(deleteTripAction);
    connect(deleteTripAction, &QAction::triggered, this, &MainWindow::onDeleteTripClicked);

    QMenu *viewMenu = menuBar()->addMenu("&View");
    QAction *refreshAction = new QAction("&Refresh", this);
    viewMenu->addAction(refreshAction);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::onRefreshViewClicked);

    QAction *filterAction = new QAction("&Filter Trips", this);
    viewMenu->addAction(filterAction);
    connect(filterAction, &QAction::triggered, this, &MainWindow::onFilterTripsClicked);

    // viewMenu->addSeparator();
    // QAction *debugAction = new QAction("&Debug Console", this);
    // debugAction->setCheckable(true);
    // viewMenu->addAction(debugAction);

    QMenu *helpMenu = menuBar()->addMenu("&Help");
    QAction *aboutAction = new QAction("&About", this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onShowAboutClicked);
}

void MainWindow::setupStatusBar() { statusBar()->showMessage("Ready"); }

// ========================================
// CACHE & DATA MANAGEMENT
// ========================================

void MainWindow::loadCacheFromFile(vector<TRIP> &outputTrips) {
    if (cacheFileExists()) {
        addDebugMessage("Loading cached trips from previous session...");
        size_t previousCount = outputTrips.size();

        // Step 1: Load basic trip data first
        loadCacheFile(outputTrips);

        // Step 2: Restore attendees using PersonManager
        addDebugMessage("Restoring trip attendees from cache...");
        restoreTripAttendeesFromCache(outputTrips, personManager, getCacheFilePath().toStdString());

        size_t loadedCount = outputTrips.size() - previousCount;
        addDebugMessage(QString("Loaded %1 trips with attendees from cache").arg(loadedCount));

        if (loadedCount > 0) {
            statusBar()->showMessage(QString("Loaded %1 trips with people data from previous session").arg(loadedCount),
                                     3000);

            // Debug: Show attendees info for verification
            for (const TRIP &trip : outputTrips) {
                HOST host = trip.getHost();
                vector<MEMBER> members = trip.getMembers();

                if (!host.getID().empty()) {
                    addDebugMessage(QString("Trip %1 has host: %2")
                                        .arg(QString::fromStdString(trip.getID()))
                                        .arg(QString::fromStdString(host.getID())));
                }

                if (!members.empty()) {
                    addDebugMessage(QString("Trip %1 has %2 members")
                                        .arg(QString::fromStdString(trip.getID()))
                                        .arg(members.size()));
                }
            }
        }
    } else {
        addDebugMessage("No cache file found. Starting with empty trip list.");
        statusBar()->showMessage("No previous data found - Ready for new trips", 3000);
    }
}

void MainWindow::saveCacheToFile() {
    addDebugMessage("Updating cache file...");
    std::vector<TRIP> currentTrips = tripManager->getAllTrips();

    saveTripAttendeesToCache(currentTrips, getCacheFilePath().toStdString());
}

// ========================================
// DISPLAY UPDATE FUNCTIONS
// ========================================

void MainWindow::updateTripDisplay(std::vector<TRIP> trips) {
    if (!tripsTable) {
        return;
    }

    tripsTable->setRowCount(trips.size());

    for (size_t i = 0; i < trips.size(); ++i) {
        const TRIP &trip = trips[i];

        tripsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(trip.getID())));
        tripsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(trip.getDestination())));
        tripsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(trip.getDescription())));
        tripsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(trip.getStartDate().toString())));
        tripsTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(trip.getEndDate().toString())));
        tripsTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(trip.getStatusString())));

        QString status = QString::fromStdString(trip.getStatusString());
        for (int j = 0; j < 6; ++j) {
            tripsTable->item(i, j)->setData(Qt::UserRole, status);
        }
    }

    updateStatusBar(trips);
}

void MainWindow::updateStatusBar(const vector<TRIP> trips) {
    if (statsLabel) {
        statsLabel->setText(QString("Trips count: %1").arg(trips.size()));
    }
    statusBar()->showMessage(QString("Ready - %1 trips").arg(trips.size()));
}

void MainWindow::addDebugMessage(const QString &message) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    Q_UNUSED(message);
}

// ========================================
// FILE OPERATIONS (First sidebar group)
// ========================================

void MainWindow::onImportTripsClicked() {
    QString fileName =
        QFileDialog::getOpenFileName(this, "Import Trip Information", projectPath, "CSV Files (*.csv);;All Files (*)");

    if (!fileName.isEmpty()) {
        addDebugMessage("Starting import from: " + fileName);

        std::vector<TRIP> importedTrips;
        importTripInfo(importedTrips,
                       fileName.toStdString());  // Import to temporary vector

        // Add imported trips through TripManager (triggers observer)
        for (const TRIP &trip : importedTrips) {
            tripManager->addTrip(trip);
        }

        addDebugMessage(QString("Import completed. %1 trips loaded.").arg(importedTrips.size()));
        QMessageBox::information(this, "Import Complete",
                                 QString("Successfully imported %1 trips.").arg(importedTrips.size()));
    }
}

void MainWindow::onExportTripsClicked() {
    std::vector<TRIP> currentTrips = tripManager->getAllTrips();

    if (currentTrips.empty()) {
        QMessageBox::warning(this, "No Data", "No trips to export. Please import trips first.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Export Trip Information", projectPath + "/trips_export.csv",
                                                    "CSV Files (*.csv);;Text Files (*.txt);;All Files (*)");

    if (!fileName.isEmpty()) {
        addDebugMessage("Exporting to: " + fileName);
        exportTripsInfo(currentTrips, fileName.toStdString());
        addDebugMessage("Export completed successfully.");
        QMessageBox::information(this, "Export Complete",
                                 QString("Successfully exported %1 trips.").arg(currentTrips.size()));
    }
}

void MainWindow::onSaveProjectClicked() {
    QMessageBox::information(this, "Save", "Save project functionality will be implemented.");
}

void MainWindow::onLoadProjectClicked() {
    QMessageBox::information(this, "Load", "Load project functionality will be implemented.");
}

// ========================================
// TRIP MANAGEMENT (Second sidebar group)
// ========================================

void MainWindow::onAddTripClicked() {
    addDebugMessage("Opening Add Trip dialog...");

    AddTripDialog dialog(personManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        TRIP newTrip = dialog.getTripData();
        tripManager->addTrip(newTrip);
    } else {
        addDebugMessage("Add trip operation was cancelled.");
    }
}

void MainWindow::onEditTripClicked() {
    int currentRow = tripsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a trip to edit.");
        return;
    }

    QString tripIdToEdit = tripsTable->item(currentRow, 0)->text();
    vector<TRIP> allTrips = tripManager->getAllTrips();

    // Find the trip to edit
    auto it = find_if(allTrips.begin(), allTrips.end(),
                      [&](const TRIP &trip) { return trip.getID() == tripIdToEdit.toStdString(); });

    if (it != allTrips.end()) {
        EditTripDialog editDialog(*it, this);
        editDialog.setPersonManager(personManager);

        if (editDialog.exec() == QDialog::Accepted) {
            // Update the trip in the manager
            tripManager->updateTrip(editDialog.getOriginalTrip(), editDialog.getUpdatedTrip());
            addDebugMessage("Trip updated: " + tripIdToEdit);
        }
    }
}

void MainWindow::onDeleteTripClicked() {
    int currentRow = tripsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a trip to delete.");
        return;
    }

    QString tripIdToDelete = tripsTable->item(currentRow, 0)->text();
    QString destination = tripsTable->item(currentRow, 1)->text();

    int ret = QMessageBox::question(this, "Delete Trip",
                                    QString("Are you sure you want to delete the trip to %1?").arg(destination),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        bool success = tripManager->removeTrip(tripIdToDelete.toStdString());

        if (success) {
            addDebugMessage("Deleted trip with ID: " + tripIdToDelete);
        } else {
            addDebugMessage("Error: Could not find trip with ID to delete: " + tripIdToDelete);
            QMessageBox::critical(this, "Error", "Could not find the trip to delete. Please refresh.");
        }
    }
}

void MainWindow::onDuplicateTripClicked() {
    QMessageBox::information(this, "Duplicate", "Duplicate trip functionality will be implemented.");
}

void MainWindow::onViewTripDetailsClicked() {
    int currentRow = tripsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a trip to view details");
        return;
    }

    QString tripIdToView = tripsTable->item(currentRow, 0)->text();
    vector<TRIP> allTrips = tripManager->getAllTrips();

    auto it = find_if(allTrips.begin(), allTrips.end(),
                      [&](const TRIP &trip) { return trip.getID() == tripIdToView.toStdString(); });

    if (it != allTrips.end()) {
        ViewTripDialog dialog(*it, personManager, this);  // Pass by reference

        if (dialog.exec() == QDialog::Accepted) {
            // Update the trip in the manager
            tripManager->updateTrip(dialog.getOriginalTrip(), dialog.getUpdatedTrip());
            updateTripDisplay(tripManager->getAllTrips());  // Refresh table
        }
    }
}

// ========================================
// PEOPLE MANAGEMENT (Third sidebar group)
// ========================================

void MainWindow::onManagePeopleClicked() {
    addDebugMessage("Opening People Management dialog...");

    ManagePeopleDialog dialog(personManager, this);
    dialog.exec();

    addDebugMessage("People Management dialog closed.");
}

// ========================================
// VIEW AND FILTER (Fourth sidebar group)
// ========================================

void MainWindow::onFilterTripsClicked() {
    std::vector<TRIP> allTrips = tripManager->getAllTrips();
    FilterTripDialog filterDialog(allTrips, this);

    if (filterDialog.exec() == QDialog::Accepted) {
        std::vector<TRIP> filteredTrips = filterDialog.getFilteredTrips();
        updateTripDisplay(filteredTrips);
        updateStatusBar(filteredTrips);

        addDebugMessage(
            QString("Applied filters - showing %1 of %2 trips").arg(filteredTrips.size()).arg(allTrips.size()));

        statusBar()->showMessage(
            QString("Filtered view: %1 of %2 trips").arg(filteredTrips.size()).arg(allTrips.size()), 5000);
    }
}

void MainWindow::onSearchTripsClicked() {
    QMessageBox::information(this, "Search Trips", "Trip search will be implemented here.");
}

void MainWindow::onShowUpcomingTripsClicked() {
    std::vector<TRIP> allTrips = tripManager->getAllTrips();
    std::vector<TRIP> upcomingTrips;
    for (const TRIP &trip : allTrips) {
        if (trip.getStatusString() == "Planned") {
            upcomingTrips.push_back(trip);
        }
    }
    updateTripDisplay(upcomingTrips);
}

void MainWindow::onShowCompletedTripsClicked() {
    std::vector<TRIP> allTrips = tripManager->getAllTrips();
    std::vector<TRIP> completedTrips;
    for (const TRIP &trip : allTrips) {
        if (trip.getStatusString() == "Completed") {
            completedTrips.push_back(trip);
        }
    }
    updateTripDisplay(completedTrips);
}

void MainWindow::onRefreshViewClicked() {
    updateTripDisplay(tripManager->getAllTrips());
    addDebugMessage("View refreshed by user.");
    statusBar()->showMessage("View refreshed.", 2000);
}

// ========================================
// DEBUG & TOOLS (Fifth sidebar group)
// ========================================

// void MainWindow::onShowDebugInfoClicked() {
//     if (!isVisible) {
//         addDebugMessage("Debug console opened.");
//     }
// }

// void MainWindow::onValidateDataClicked() {
//     QMessageBox::information(this, "Validate", "Data validation will be
//     implemented.");
// }

// void MainWindow::onExportDebugLogClicked() {
//     QMessageBox::information(this, "Export Log", "Debug log export will be
//     implemented.");
// }

// ========================================
// HELP & SETTINGS
// ========================================

void MainWindow::onShowAboutClicked() {
    QMessageBox::about(this, "About Trip Management System",
                       "Trip Management System v1.0\n\n"
                       "A modern application for managing group trips.\n"
                       "Built with Qt and C++.\n\n"
                       "© 2024 Your Name/Organization");
}

void MainWindow::onShowHelpClicked() {
    QMessageBox::information(this, "Help", "Help documentation will be implemented.");
}

void MainWindow::onOpenSettingsClicked() {
    QMessageBox::information(this, "Settings", "Settings dialog will be implemented.");
}

// Observer implementation
void MainWindow::onTripAdded(const std::string &tripId) {
    addDebugMessage("Observer: Trip added - " + QString::fromStdString(tripId));

    // Get current trips from manager only
    std::vector<TRIP> currentTrips = tripManager->getAllTrips();
    updateTripDisplay(currentTrips);
    saveCacheToFile();  // This now uses tripManager internally

    statusBar()->showMessage(QString("New trip added: %1").arg(QString::fromStdString(tripId)), 3000);
}

void MainWindow::onTripRemoved(const std::string &tripId) {
    addDebugMessage("Observer: Trip removed - " + QString::fromStdString(tripId));

    // Get current trips from manager and update display
    std::vector<TRIP> currentTrips = tripManager->getAllTrips();
    updateTripDisplay(currentTrips);
    saveCacheToFile();  // This now uses tripManager internally

    statusBar()->showMessage(QString("Trip removed: %1").arg(QString::fromStdString(tripId)), 3000);
}

void MainWindow::onTripUpdated(const std::string &tripId) {
    addDebugMessage("Observer: Trip updated - " + QString::fromStdString(tripId));

    // Get current trips from manager and update display
    vector<TRIP> currentTrips = tripManager->getAllTrips();
    updateTripDisplay(currentTrips);
    saveCacheToFile();  // This now uses tripManager internally

    statusBar()->showMessage(QString("Trip updated: %1").arg(QString::fromStdString(tripId)), 3000);
}

// NEW: Person observer methods
void MainWindow::onPersonAdded(const string &personID) {
    addDebugMessage("Person added: " + QString::fromStdString(personID));
    // You can add specific logic here for when a person is added
}

void MainWindow::onPersonRemoved(const string &personID) {
    addDebugMessage("Person removed: " + QString::fromStdString(personID));
    // You can add specific logic here for when a person is removed
}

void MainWindow::onPersonUpdated(const string &personID) {
    addDebugMessage("Person updated: " + QString::fromStdString(personID));
    // You can add specific logic here for when a person is updated
}

void MainWindow::onImportPeopleClicked() {
    addDebugMessage("Opening People Import dialog...");

    QString filename =
        QFileDialog::getOpenFileName(this, "Import People from CSV", projectPath, "CSV Files (*.csv);;All Files (*.*)");

    if (filename.isEmpty()) {
        return;  // User canceled
    }

    try {
        // Get current people to check for duplicates
        vector<MEMBER> importedMembers;
        vector<HOST> importedHosts;

        // Import new people
        importPeopleInfo(importedMembers, importedHosts, filename.toStdString());

        if (importedMembers.empty() && importedHosts.empty()) {
            QMessageBox::warning(this, "Import Failed", "No people found in the file or the file format is incorrect.");
            return;
        }

        int importCount = 0;

        // Add each imported member if they don't already exist
        for (const MEMBER &member : importedMembers) {
            // Check if a member with the same ID already exists
            if (!personManager->findMemberById(member.getID())) {
                personManager->addMember(member);
                importCount++;
            }
        }

        // Add each imported host if they don't already exist
        for (const HOST &host : importedHosts) {
            // Check if a host with the same ID already exists
            if (!personManager->findHostById(host.getID())) {
                personManager->addHost(host);
                importCount++;
            }
        }

        QMessageBox::information(this, "Import Successful",
                                 QString("Successfully imported %1 people.").arg(importCount));

        addDebugMessage(QString("Imported %1 people from %2").arg(importCount).arg(QFileInfo(filename).fileName()));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Import Error", QString("An error occurred during import: %1").arg(e.what()));
        addDebugMessage(QString("Error importing people: %1").arg(e.what()));
    }
}

void MainWindow::onExportPeopleClicked() {
    addDebugMessage("Opening People Export dialog...");

    const vector<MEMBER> &members = personManager->getAllMembers();
    const vector<HOST> &hosts = personManager->getAllHosts();

    if (members.empty() && hosts.empty()) {
        QMessageBox::warning(this, "Export Failed", "There are no people to export.");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Export People to CSV", projectPath + "/people_export.csv",
                                                    "CSV Files (*.csv);;All Files (*.*)");

    if (filename.isEmpty()) {
        return;  // User canceled
    }

    try {
        exportPeopleInfo(members, hosts, filename.toStdString());

        int totalPeople = members.size() + hosts.size();
        QMessageBox::information(
            this, "Export Successful",
            QString("Successfully exported %1 people to %2.").arg(totalPeople).arg(QFileInfo(filename).fileName()));

        addDebugMessage(QString("Exported %1 people to %2").arg(totalPeople).arg(QFileInfo(filename).fileName()));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Export Error", QString("An error occurred during export: %1").arg(e.what()));
        addDebugMessage(QString("Error exporting people: %1").arg(e.what()));
    }
}

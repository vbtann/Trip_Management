#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt Core Headers
#include <QApplication>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QMainWindow>
#include <QWidget>

// Qt GUI Headers
#include <QAction>
#include <QIcon>

// Qt Widgets Headers
#include <QComboBox>
#include <QDateEdit>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QStackedLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

// Qt Dialog Headers
#include <QFileDialog>
#include <QMessageBox>

// Qt Main Window Components
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

// Standard Library
#include <algorithm>
#include <iostream>
#include <vector>

// Project Headers
#include "../Managers/FileManager.h"
#include "../Managers/Observer.h"
#include "../Managers/PersonManager.h"
#include "../Managers/TripManager.h"
#include "../Models/header.h"
#include "ManagePeopleDialog.h"

// Forward declaration for the restoration function
void restoreTripAttendeesFromCache(vector<TRIP> &trips, PERSONMANAGER *personManager, const string &filePath);
void saveTripAttendeesToCache(const vector<TRIP> &trips, const string &filePath);

class TRIP;
class TRIPMANAGER;
class AddTripDialog;
class FilterTripDialog;

class MainWindow : public QMainWindow, public OBSERVER {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Observer pattern methods
    void onTripAdded(const string &tripID) override;
    void onTripRemoved(const string &tripID) override;
    void onTripUpdated(const string &tripID) override;
    void onPersonAdded(const string &personID) override;
    void onPersonRemoved(const string &personID) override;
    void onPersonUpdated(const string &personID) override;

   private slots:
    // File Operations
    void onImportTripsClicked();
    void onExportTripsClicked();

    // Trip Management
    void onAddTripClicked();
    void onEditTripClicked();
    void onDeleteTripClicked();
    void onDuplicateTripClicked();
    void onViewTripDetailsClicked();

    // Person Management - REPLACE the existing person management slots with this:
    void onManagePeopleClicked();  // Single entry point for people management

    // View and Filter
    void onFilterTripsClicked();
    void onSearchTripsClicked();
    void onRefreshViewClicked();
    void onShowUpcomingTripsClicked();
    void onShowCompletedTripsClicked();

    // Help and Settings
    void onShowHelpClicked();
    void onShowAboutClicked();
    void onOpenSettingsClicked();

    // Debug Functions
    // void onShowDebugInfoClicked();
    // void onValidateDataClicked();
    // void onExportDebugLogClicked();

    // Add these new slots for people import/export
    void onImportPeopleClicked();
    void onExportPeopleClicked();

   private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupSidebar();
    void setupMainContent();
    void updateTripDisplay(const std::vector<TRIP> trips);
    void updateStatusBar(const vector<TRIP>);
    void addDebugMessage(const QString &message);
    void loadCacheFromFile(vector<TRIP> &outputTrips);  // Updated to include attendees
    void saveCacheToFile();                             // Updated to use new save function

    // UI Components
    QWidget *centralWidget;
    QSplitter *mainSplitter;

    // Sidebar Components
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QGroupBox *quickActionsGroup;
    QGroupBox *managementGroup;
    QGroupBox *peopleGroup;
    QGroupBox *viewGroup;

    // Main Content Area
    QWidget *mainContent;
    QVBoxLayout *mainContentLayout;
    QWidget *headerWidget;
    QWidget *tripDisplayArea;
    QTableWidget *tripsTable;

    // Header Components
    QLabel *titleLabel;
    QLabel *statsLabel;
    QPushButton *refreshButton;
    QPushButton *searchButton;

    // Action Buttons
    QPushButton *importButton;
    QPushButton *exportButton;
    QPushButton *addTripButton;
    QPushButton *editTripButton;
    QPushButton *deleteButton;
    QPushButton *duplicateButton;
    QPushButton *detailViewButton;
    QPushButton *manageMembersButton;
    QPushButton *manageHostsButton;
    QPushButton *memberStatsButton;
    QPushButton *filterButton;
    QPushButton *debugButton;
    QPushButton *addPersonButton;
    QPushButton *editPersonButton;
    QPushButton *deletePersonButton;
    QPushButton *viewPersonButton;

    // Add these buttons
    QPushButton *importPeopleButton;
    QPushButton *exportPeopleButton;

    // Status Components
    QLabel *statusLabel;
    QLabel *tripCountLabel;
    QProgressBar *progressBar;

    // Data
    PERSONMANAGER *personManager;
    TRIPMANAGER *tripManager;

    // Helper function to get project path (relative to executable)
    QString getProjectPath() const {
        QDir currentDir = QDir::current();
        currentDir.cdUp();  // Go up from simpleQtApp to project root
        return currentDir.absolutePath();
    }
};

#endif  // MAINWINDOW_H

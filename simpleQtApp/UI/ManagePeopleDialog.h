#ifndef MANAGEPEOPLEDIALOG_H
#define MANAGEPEOPLEDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>

#include "../Managers/Observer.h"
#include "../Managers/PersonManager.h"
#include "../Models/header.h"
#include "EditPersonDialog.h"

class ManagePeopleDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ManagePeopleDialog(PERSONMANAGER* personManager, QWidget* parent = nullptr);
    // ~ManagePeopleDialog();

    // void onPersonAdded(const string& personID) override;
    // void onPersonRemoved(const string& personID) override;
    // void onPersonUpdated(const string& personID) override;

   private slots:
    void onAddPersonClicked();
    void onEditPersonClicked();
    void onDeletePersonClicked();
    void onViewPersonClicked();
    void onPersonSelectionChanged();
    void onRefreshClicked();
    void onCloseClicked();
    void onImportPeopleClicked();
    void onExportPeopleClicked();

   private:
    void setupUI();
    void styleComponents();
    void refreshPersonList();
    void updatePersonInfo();

    // Data
    PERSONMANAGER* personManager;

    // UI Components
    QListWidget* peopleListWidget;
    QLabel* personCountLabel;
    QLabel* memberCountLabel;
    QLabel* hostCountLabel;
    QLabel* selectedPersonInfoLabel;

    // Buttons
    QPushButton* addPersonButton;
    QPushButton* editPersonButton;
    QPushButton* deletePersonButton;
    QPushButton* viewPersonButton;
    QPushButton* refreshButton;
    QPushButton* closeButton;
    QPushButton* importPeopleButton;
    QPushButton* exportPeopleButton;
};

#endif  // MANAGEPEOPLEDIALOG_H
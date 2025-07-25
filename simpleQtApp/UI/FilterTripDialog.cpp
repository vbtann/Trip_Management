#include "FilterTripDialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QScrollArea>
#include <QSplitter>
#include <QTextStream>
#include <algorithm>

using namespace std;

FilterTripDialog::FilterTripDialog(const std::vector<TRIP> &allTrips,
                                   QWidget *parent)
    : QDialog(parent), _allTrips(allTrips), _filteredTrips(allTrips) {
     setupUI();
     setWindowTitle("🔍 Filter and Sort Trips");
     setModal(true);
     resize(900, 700);

     // Center on parent
     if (parent) {
          move(parent->geometry().center() - rect().center());
     }

     // Initialize with all trips
     applyFilters();
}

void FilterTripDialog::setupUI() {
     // Main layout
     QVBoxLayout *mainLayout = new QVBoxLayout(this);
     mainLayout->setSpacing(15);
     mainLayout->setContentsMargins(20, 20, 20, 20);

     // Title
     QLabel *titleLabel = new QLabel("🔍 Advanced Trip Filtering", this);
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

     // Create scroll area for filters
     QScrollArea *scrollArea = new QScrollArea(this);
     scrollArea->setWidgetResizable(true);
     scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
     scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

     QWidget *scrollContent = new QWidget();
     QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

     // Setup filter groups
     setupDestinationFilter();
     setupDateFilter();
     setupStatusFilter();
     setupDescriptionFilter();
     setupSortingOptions();

     // Add preset filters at the top
     presetGroup = new QGroupBox("🎯 Quick Filters");
     QHBoxLayout *presetLayout = new QHBoxLayout(presetGroup);

     presetFiltersCombo = new QComboBox();
     presetFiltersCombo->addItem("No Preset");
     presetFiltersCombo->addItem("🗓️ Upcoming Trips");
     presetFiltersCombo->addItem("✅ Completed Trips");
     presetFiltersCombo->addItem("🚀 Active Trips");
     presetFiltersCombo->addItem("❌ Cancelled Trips");
     presetFiltersCombo->addItem("📅 This Month");
     presetFiltersCombo->addItem("📅 Next Month");
     presetFiltersCombo->addItem("🌍 International Trips");

     presetLayout->addWidget(new QLabel("Preset Filter:"));
     presetLayout->addWidget(presetFiltersCombo);
     presetLayout->addStretch();

     connect(presetFiltersCombo,
             QOverload<int>::of(&QComboBox::currentIndexChanged), this,
             &FilterTripDialog::onPresetFilterChanged);

     // Add all groups to scroll layout
     scrollLayout->addWidget(presetGroup);
     scrollLayout->addWidget(destinationGroup);
     scrollLayout->addWidget(dateGroup);
     scrollLayout->addWidget(statusGroup);
     scrollLayout->addWidget(descriptionGroup);
     scrollLayout->addWidget(sortGroup);
     scrollLayout->addStretch();

     scrollArea->setWidget(scrollContent);
     mainLayout->addWidget(scrollArea);

     // Results label
     resultsLabel = new QLabel("Found 0 trips matching criteria", this);
     resultsLabel->setStyleSheet(
         "QLabel { "
         "    font-weight: bold; "
         "    color: #27ae60; "
         "    padding: 5px; "
         "    background-color: #ecf0f1; "
         "    border-radius: 4px; "
         "}");
     mainLayout->addWidget(resultsLabel);

     // Setup buttons
     setupButtons();

     // Style the dialog
     setStyleSheet(
         "QGroupBox { "
         "    font-weight: bold; "
         "    border: 2px solid #bdc3c7; "
         "    border-radius: 8px; "
         "    margin-top: 10px; "
         "    padding-top: 10px; "
         "}"
         "QGroupBox::title { "
         "    subcontrol-origin: margin; "
         "    left: 10px; "
         "    padding: 0 8px 0 8px; "
         "    background-color: white; "
         "}");
}

void FilterTripDialog::setupDestinationFilter() {
     destinationGroup = new QGroupBox("🌍 Destination Filter");
     QFormLayout *destLayout = new QFormLayout(destinationGroup);

     // Text search
     destinationLineEdit = new QLineEdit();
     destinationLineEdit->setPlaceholderText(
         "Search destinations (e.g., Paris, Tokyo)...");
     destLayout->addRow("Search:", destinationLineEdit);

     // Exact match option
     destinationExactMatch = new QCheckBox("Exact match only");
     destLayout->addRow("", destinationExactMatch);

     // Popular destinations dropdown
     destinationComboBox = new QComboBox();
     destinationComboBox->addItem("Any Destination");

     // Populate with unique destinations from trips
     std::set<string> destinations;
     for (const auto &trip : _allTrips) {
          destinations.insert(trip.getDestination());
     }

     for (const auto &dest : destinations) {
          destinationComboBox->addItem(QString::fromStdString(dest));
     }

     destLayout->addRow("Select:", destinationComboBox);
}

void FilterTripDialog::setupDateFilter() {
     dateGroup = new QGroupBox("📅 Date Range Filter");
     QVBoxLayout *dateMainLayout = new QVBoxLayout(dateGroup);

     enableDateFilter = new QCheckBox("Enable date filtering");
     dateMainLayout->addWidget(enableDateFilter);

     // Date range preset
     QHBoxLayout *presetLayout = new QHBoxLayout();
     dateRangePreset = new QComboBox();
     dateRangePreset->addItem("Custom Range");
     dateRangePreset->addItem("📅 This Week");
     dateRangePreset->addItem("📅 This Month");
     dateRangePreset->addItem("📅 Next Month");
     dateRangePreset->addItem("📅 This Year");
     dateRangePreset->addItem("📅 Next 3 Months");
     dateRangePreset->addItem("📅 Past Trips");

     presetLayout->addWidget(new QLabel("Quick Range:"));
     presetLayout->addWidget(dateRangePreset);
     presetLayout->addStretch();
     dateMainLayout->addLayout(presetLayout);

     // Custom date ranges
     QFormLayout *dateLayout = new QFormLayout();

     startDateFrom = new QDateEdit(QDate::currentDate().addMonths(-1));
     startDateFrom->setCalendarPopup(true);
     startDateFrom->setDisplayFormat("dd/MM/yyyy");

     startDateTo = new QDateEdit(QDate::currentDate().addMonths(6));
     startDateTo->setCalendarPopup(true);
     startDateTo->setDisplayFormat("dd/MM/yyyy");

     endDateFrom = new QDateEdit(QDate::currentDate().addMonths(-1));
     endDateFrom->setCalendarPopup(true);
     endDateFrom->setDisplayFormat("dd/MM/yyyy");

     endDateTo = new QDateEdit(QDate::currentDate().addMonths(6));
     endDateTo->setCalendarPopup(true);
     endDateTo->setDisplayFormat("dd/MM/yyyy");

     dateLayout->addRow("Start Date From:", startDateFrom);
     dateLayout->addRow("Start Date To:", startDateTo);
     dateLayout->addRow("End Date From:", endDateFrom);
     dateLayout->addRow("End Date To:", endDateTo);

     dateMainLayout->addLayout(dateLayout);

     // Initially disable date controls
     startDateFrom->setEnabled(false);
     startDateTo->setEnabled(false);
     endDateFrom->setEnabled(false);
     endDateTo->setEnabled(false);
     dateRangePreset->setEnabled(false);

     connect(enableDateFilter, &QCheckBox::toggled, [this](bool enabled) {
          startDateFrom->setEnabled(enabled);
          startDateTo->setEnabled(enabled);
          endDateFrom->setEnabled(enabled);
          endDateTo->setEnabled(enabled);
          dateRangePreset->setEnabled(enabled);
     });
}

void FilterTripDialog::setupStatusFilter() {
     statusGroup = new QGroupBox("📊 Status Filter");
     QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);

     statusPlanned = new QCheckBox("🗓️ Planned");
     statusOngoing = new QCheckBox("🚀 Ongoing");
     statusCompleted = new QCheckBox("✅ Completed");
     statusCancelled = new QCheckBox("❌ Cancelled");

     // Set all checked by default
     statusPlanned->setChecked(true);
     statusOngoing->setChecked(true);
     statusCompleted->setChecked(true);
     statusCancelled->setChecked(true);

     statusLayout->addWidget(statusPlanned);
     statusLayout->addWidget(statusOngoing);
     statusLayout->addWidget(statusCompleted);
     statusLayout->addWidget(statusCancelled);

     // Select/Deselect all buttons
     QHBoxLayout *statusButtonLayout = new QHBoxLayout();
     QPushButton *selectAllStatus = new QPushButton("Select All");
     QPushButton *deselectAllStatus = new QPushButton("Deselect All");

     connect(selectAllStatus, &QPushButton::clicked, [this]() {
          statusPlanned->setChecked(true);
          statusOngoing->setChecked(true);
          statusCompleted->setChecked(true);
          statusCancelled->setChecked(true);
     });

     connect(deselectAllStatus, &QPushButton::clicked, [this]() {
          statusPlanned->setChecked(false);
          statusOngoing->setChecked(false);
          statusCompleted->setChecked(false);
          statusCancelled->setChecked(false);
     });

     statusButtonLayout->addWidget(selectAllStatus);
     statusButtonLayout->addWidget(deselectAllStatus);
     statusButtonLayout->addStretch();
     statusLayout->addLayout(statusButtonLayout);
}

void FilterTripDialog::setupDescriptionFilter() {
     descriptionGroup = new QGroupBox("📝 Description Filter");
     QFormLayout *descLayout = new QFormLayout(descriptionGroup);

     descriptionKeywords = new QLineEdit();
     descriptionKeywords->setPlaceholderText(
         "Keywords (separate with commas): beach, hotel, adventure...");
     descLayout->addRow("Keywords:", descriptionKeywords);

     descriptionCaseSensitive = new QCheckBox("Case sensitive search");
     descLayout->addRow("", descriptionCaseSensitive);
}

void FilterTripDialog::setupSortingOptions() {
     sortGroup = new QGroupBox("🔄 Sorting Options");
     QFormLayout *sortLayout = new QFormLayout(sortGroup);

     sortByComboBox = new QComboBox();
     sortByComboBox->addItem("📅 Start Date");
     sortByComboBox->addItem("📅 End Date");
     sortByComboBox->addItem("🌍 Destination");
     sortByComboBox->addItem("🔢 Trip ID");
     sortByComboBox->addItem("📊 Status");
     sortByComboBox->addItem("📝 Description Length");

     sortLayout->addRow("Sort By:", sortByComboBox);

     // Sort order
     QHBoxLayout *orderLayout = new QHBoxLayout();
     QButtonGroup *orderGroup = new QButtonGroup(this);

     sortAscending = new QRadioButton("⬆️ Ascending");
     sortDescending = new QRadioButton("⬇️ Descending");
     sortAscending->setChecked(true);

     orderGroup->addButton(sortAscending);
     orderGroup->addButton(sortDescending);

     orderLayout->addWidget(sortAscending);
     orderLayout->addWidget(sortDescending);
     orderLayout->addStretch();

     sortLayout->addRow("Order:", orderLayout);
}

void FilterTripDialog::setupButtons() {
     QHBoxLayout *buttonLayout = new QHBoxLayout();

     // Create buttons with clear purposes
     applyButton = new QPushButton("🔄 Apply Filters");
     clearButton = new QPushButton("🧹 Clear All");
     cancelButton = new QPushButton("❌ Cancel");

     // Style the Apply Filters button (preview only)
     applyButton->setStyleSheet(
         "QPushButton { "
         "    background-color: #27ae60; "
         "    color: white; "
         "    font-weight: bold; "
         "    padding: 12px 24px; "
         "    border: none; "
         "    border-radius: 6px; "
         "}"
         "QPushButton:hover { background-color: #2980b9; }"
         "QPushButton:pressed { background-color: #21618c; }");

     clearButton->setStyleSheet(
         "QPushButton { "
         "    background-color: #f39c12; "
         "    color: white; "
         "    font-weight: bold; "
         "    padding: 10px 20px; "
         "    border: none; "
         "    border-radius: 6px; "
         "}"
         "QPushButton:hover { background-color: #e67e22; }"
         "QPushButton:pressed { background-color: #d35400; }");

     cancelButton->setStyleSheet(
         "QPushButton { "
         "    background-color: #e74c3c; "
         "    color: white; "
         "    font-weight: bold; "
         "    padding: 10px 20px; "
         "    border: none; "
         "    border-radius: 6px; "
         "}"
         "QPushButton:hover { background-color: #c0392b; }"
         "QPushButton:pressed { background-color: #a93226; }");

     // Arrange buttons with proper spacing
     buttonLayout->addStretch();
     buttonLayout->addWidget(applyButton);
     buttonLayout->addSpacing(10);
     buttonLayout->addWidget(clearButton);
     buttonLayout->addSpacing(10);
     buttonLayout->addWidget(cancelButton);

     // Add button layout to main layout
     QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(layout());
     if (mainLayout) {
          mainLayout->addLayout(buttonLayout);
     }

     // Connect buttons
     connect(applyButton, &QPushButton::clicked, this,
             &FilterTripDialog::applyAndClose);
     connect(clearButton, &QPushButton::clicked, this,
             &FilterTripDialog::clearFilters);
     connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

     // Auto-apply filters when any control changes (for live preview)
     connect(destinationLineEdit, &QLineEdit::textChanged, this,
             &FilterTripDialog::applyFilters);
     connect(destinationComboBox,
             QOverload<int>::of(&QComboBox::currentIndexChanged), this,
             &FilterTripDialog::applyFilters);
     connect(destinationExactMatch, &QCheckBox::toggled, this,
             &FilterTripDialog::applyFilters);
     connect(enableDateFilter, &QCheckBox::toggled, this,
             &FilterTripDialog::applyFilters);
     connect(statusPlanned, &QCheckBox::toggled, this,
             &FilterTripDialog::applyFilters);
     connect(statusOngoing, &QCheckBox::toggled, this,
             &FilterTripDialog::applyFilters);
     connect(statusCompleted, &QCheckBox::toggled, this,
             &FilterTripDialog::applyFilters);
     connect(statusCancelled, &QCheckBox::toggled, this,
             &FilterTripDialog::applyFilters);
     connect(descriptionKeywords, &QLineEdit::textChanged, this,
             &FilterTripDialog::applyFilters);
     connect(descriptionCaseSensitive, &QCheckBox::toggled, this,
             &FilterTripDialog::applyFilters);
     connect(sortByComboBox,
             QOverload<int>::of(&QComboBox::currentIndexChanged), this,
             &FilterTripDialog::applyFilters);
     connect(sortAscending, &QRadioButton::toggled, this,
             &FilterTripDialog::applyFilters);
     connect(sortDescending, &QRadioButton::toggled, this,
             &FilterTripDialog::applyFilters);
}

void FilterTripDialog::applyFilters() {
     _filteredTrips.clear();

     // Apply filters
     for (const auto &trip : _allTrips) {
          if (matchesFilters(trip)) {
               _filteredTrips.push_back(trip);
          }
     }

     // Apply sorting
     _filteredTrips = sortTrips(_filteredTrips);

     // Update results label
     resultsLabel->setText(QString("Found %1 trips matching criteria")
                               .arg(_filteredTrips.size()));

     // Change color based on results
     if (_filteredTrips.empty()) {
          resultsLabel->setStyleSheet(
              "QLabel { "
              "    font-weight: bold; "
              "    color: #e74c3c; "
              "    padding: 5px; "
              "    background-color: #fadbd8; "
              "    border-radius: 4px; "
              "}");
     } else {
          resultsLabel->setStyleSheet(
              "QLabel { "
              "    font-weight: bold; "
              "    color: #27ae60; "
              "    padding: 5px; "
              "    background-color: #d5f4e6; "
              "    border-radius: 4px; "
              "}");
     }
}

void FilterTripDialog::applyAndClose() {
     applyFilters();
     accept();
}

void FilterTripDialog::clearFilters() {
     // Clear destination filters
     destinationLineEdit->clear();
     destinationComboBox->setCurrentIndex(0);
     destinationExactMatch->setChecked(false);

     // Clear date filters
     enableDateFilter->setChecked(false);
     dateRangePreset->setCurrentIndex(0);

     // Reset status filters
     statusPlanned->setChecked(true);
     statusOngoing->setChecked(true);
     statusCompleted->setChecked(true);
     statusCancelled->setChecked(true);

     // Clear description filters
     descriptionKeywords->clear();
     descriptionCaseSensitive->setChecked(false);

     // Reset sorting
     sortByComboBox->setCurrentIndex(0);
     sortAscending->setChecked(true);

     // Clear preset
     presetFiltersCombo->setCurrentIndex(0);

     // Reapply filters (will show all trips)
     applyFilters();
}

void FilterTripDialog::onPresetFilterChanged() {
     int index = presetFiltersCombo->currentIndex();

     if (index == 0) return;  // No preset

     // Temporarily disconnect the preset combo to prevent recursion
     presetFiltersCombo->blockSignals(true);

     // Clear current filters first (but don't touch the preset combo)
     destinationLineEdit->clear();
     destinationComboBox->setCurrentIndex(0);
     destinationExactMatch->setChecked(false);

     // Clear date filters
     enableDateFilter->setChecked(false);
     dateRangePreset->setCurrentIndex(0);

     // Reset all status filters to false first
     statusPlanned->setChecked(false);
     statusOngoing->setChecked(false);
     statusCompleted->setChecked(false);
     statusCancelled->setChecked(false);

     // Clear description filters
     descriptionKeywords->clear();
     descriptionCaseSensitive->setChecked(false);

     // Reset sorting
     sortByComboBox->setCurrentIndex(0);
     sortAscending->setChecked(true);

     // Apply the selected preset
     switch (index) {
          case 1:  // 🗓️ Upcoming Trips
               statusPlanned->setChecked(true);
               statusOngoing->setChecked(
                   true);  // Include ongoing as "upcoming"
               break;

          case 2:  // ✅ Completed Trips
               statusCompleted->setChecked(true);
               break;

          case 3:  // 🚀 Active Trips
               statusOngoing->setChecked(true);
               break;

          case 4:  // ❌ Cancelled Trips
               statusCancelled->setChecked(true);
               break;

          case 5:  // 📅 This Month
               enableDateFilter->setChecked(true);
               // Set date range to current month
               {
                    QDate today = QDate::currentDate();
                    QDate firstDay(today.year(), today.month(), 1);
                    QDate lastDay =
                        firstDay.addDays(firstDay.daysInMonth() - 1);

                    startDateFrom->setDate(firstDay);
                    startDateTo->setDate(lastDay);
                    endDateFrom->setDate(firstDay);
                    endDateTo->setDate(lastDay);
               }
               // Set all statuses to show trips in this month
               statusPlanned->setChecked(true);
               statusOngoing->setChecked(true);
               statusCompleted->setChecked(true);
               statusCancelled->setChecked(true);
               break;

          case 6:  // 📅 Next Month
               enableDateFilter->setChecked(true);
               // Set date range to next month
               {
                    QDate nextMonth = QDate::currentDate().addMonths(1);
                    QDate firstDay(nextMonth.year(), nextMonth.month(), 1);
                    QDate lastDay =
                        firstDay.addDays(firstDay.daysInMonth() - 1);

                    startDateFrom->setDate(firstDay);
                    startDateTo->setDate(lastDay);
                    endDateFrom->setDate(firstDay);
                    endDateTo->setDate(lastDay);
               }
               // Set all statuses to show trips in next month
               statusPlanned->setChecked(true);
               statusOngoing->setChecked(true);
               statusCompleted->setChecked(true);
               statusCancelled->setChecked(true);
               break;

          case 7:  // 🌍 International Trips
               destinationLineEdit->setText("International");
               // Show all statuses for international trips
               statusPlanned->setChecked(true);
               statusOngoing->setChecked(true);
               statusCompleted->setChecked(true);
               statusCancelled->setChecked(true);
               break;
     }

     // Re-enable signals for the preset combo
     presetFiltersCombo->blockSignals(false);

     // Apply the filters after setting the preset
     applyFilters();

     // Debug message to confirm preset was applied
     qDebug() << "Applied preset:" << presetFiltersCombo->currentText();
}

bool FilterTripDialog::matchesFilters(const TRIP &trip) const {
     // Destination filter
     QString searchText = destinationLineEdit->text().trimmed();
     if (!searchText.isEmpty()) {
          QString destination = QString::fromStdString(trip.getDestination());
          if (destinationExactMatch->isChecked()) {
               if (destination.compare(searchText, Qt::CaseInsensitive) != 0) {
                    return false;
               }
          } else {
               if (!destination.contains(searchText, Qt::CaseInsensitive)) {
                    return false;
               }
          }
     }

     // Destination dropdown filter
     if (destinationComboBox->currentIndex() > 0) {
          QString selectedDest = destinationComboBox->currentText();
          QString tripDest = QString::fromStdString(trip.getDestination());
          if (tripDest != selectedDest) {
               return false;
          }
     }

     // Status filter
     std::string statusStr = trip.getStatusString();
     bool statusMatches = false;

     if (statusPlanned->isChecked() && statusStr == "Planned")
          statusMatches = true;
     if (statusOngoing->isChecked() && statusStr == "Ongoing")
          statusMatches = true;
     if (statusCompleted->isChecked() && statusStr == "Completed")
          statusMatches = true;
     if (statusCancelled->isChecked() && statusStr == "Cancelled")
          statusMatches = true;

     if (!statusMatches) return false;

     // Description filter
     QString keywords = descriptionKeywords->text().trimmed();
     if (!keywords.isEmpty()) {
          QString description = QString::fromStdString(trip.getDescription());
          QStringList keywordList = keywords.split(',', Qt::SkipEmptyParts);

          bool keywordMatches = false;
          for (const QString &keyword : keywordList) {
               QString trimmedKeyword = keyword.trimmed();
               Qt::CaseSensitivity caseSensitive =
                   descriptionCaseSensitive->isChecked() ? Qt::CaseSensitive
                                                         : Qt::CaseInsensitive;

               if (description.contains(trimmedKeyword, caseSensitive)) {
                    keywordMatches = true;
                    break;
               }
          }

          if (!keywordMatches) return false;
     }

     // Date filter (if enabled)
     if (enableDateFilter->isChecked()) {
          // Convert TRIP dates to QDate for comparison
          DATE tripStartDate = trip.getStartDate();
          DATE tripEndDate = trip.getEndDate();

          QDate qTripStart(tripStartDate.getYear(), tripStartDate.getMonth(),
                           tripStartDate.getDay());
          QDate qTripEnd(tripEndDate.getYear(), tripEndDate.getMonth(),
                         tripEndDate.getDay());

          QDate filterStartFrom = startDateFrom->date();
          QDate filterStartTo = startDateTo->date();
          QDate filterEndFrom = endDateFrom->date();
          QDate filterEndTo = endDateTo->date();

          if (qTripStart < filterStartFrom || qTripStart > filterStartTo) {
               return false;
          }

          if (qTripEnd < filterEndFrom || qTripEnd > filterEndTo) {
               return false;
          }
     }

     return true;
}

std::vector<TRIP> FilterTripDialog::sortTrips(std::vector<TRIP> trips) const {
     int sortBy = sortByComboBox->currentIndex();
     bool ascending = sortAscending->isChecked();

     std::sort(trips.begin(), trips.end(),
               [sortBy, ascending](const TRIP &a, const TRIP &b) {
                    bool result;

                    switch (sortBy) {
                         case 0:  // Start Date
                              result = a.getStartDate() < b.getStartDate();
                              break;
                         case 1:  // End Date
                              result = a.getEndDate() < b.getEndDate();
                              break;
                         case 2:  // Destination
                              result = a.getDestination() < b.getDestination();
                              break;
                         case 3:  // Trip ID
                              result = a.getID() < b.getID();
                              break;
                         case 4:  // Status
                              result =
                                  a.getStatusString() < b.getStatusString();
                              break;
                         case 5:  // Description Length
                              result = a.getDescription().length() <
                                       b.getDescription().length();
                              break;
                         default:
                              result = a.getStartDate() < b.getStartDate();
                    }

                    return ascending ? result : !result;
               });

     return trips;
}

std::vector<TRIP> FilterTripDialog::getFilteredTrips() const {
     return _filteredTrips;
}

# Trip Management System

## 📋 Overview

The **Trip Management System** is a comprehensive Qt-based desktop application designed to help friend groups organize, plan, and manage their trips efficiently. Built with C++ and Qt framework, it provides an intuitive GUI interface for seamless trip and people management.

## 🚀 Quick Start

### Prerequisites
- **Qt Framework** (version 5.15 or higher recommended)
- **C++ Compiler** (MinGW, MSVC, or GCC)
- **qmake** (comes with Qt installation)

### Installation Steps

1. **Download Qt**
   - Visit [Qt Official Website](https://www.qt.io/download-qt-installer)
   - Download and install Qt with MinGW compiler
   - Make sure to add Qt's bin directory to your system PATH

2. **Download This Project**
   - Download the ZIP file from GitHub
   - Extract to your desired location

3. **Build and Run**
   ```batch
   # Simply double-click build.bat
   # Or run from command line:
   build.bat
   ```

4. **Launch Application**
   ```batch
   # Double-click run.bat
   # Or run from command line:
   run.bat
   ```

### Alternative Manual Build
If the batch files don't work, you can build manually:
```bash
cd simpleQtApp
qmake simpleQtApp.pro
make
```

## 🛠️ Features

### Trip Management
- ✅ **Create Trips**: Add new trips with detailed information
- ✅ **Edit Trips**: Modify existing trip details
- ✅ **Delete Trips**: Remove unwanted trips
- ✅ **View Trip Details**: Comprehensive trip information display
- ✅ **Trip Status Tracking**: Plan, Ongoing, Completed, Cancelled

### People Management
- ✅ **Add People**: Register hosts and members
- ✅ **Import/Export People**: CSV file support
- ✅ **Assign Attendees**: Link people to trips
- ✅ **Edit Person Details**: Update contact information

### Data Management
- ✅ **Import/Export Trips**: CSV file support
- ✅ **Auto-save**: Automatic data persistence
- ✅ **Filter and Search**: Find trips quickly
- ✅ **Data Validation**: Ensure data integrity

### User Interface
- ✅ **Modern GUI**: Clean and intuitive interface
- ✅ **Dashboard View**: Overview of all trips
- ✅ **Dialog-based Forms**: Easy data entry
- ✅ **Status Bar**: Real-time feedback

## 📁 Project Structure

```
Trip_Management/
├── simpleQtApp/
│   ├── main.cpp              # Application entry point
│   ├── simpleQtApp.pro       # Qt project file
│   ├── UI/                   # User interface files
│   │   ├── MainWindow.cpp/h
│   │   ├── AddTripDialog.cpp/h
│   │   ├── EditTripDialog.cpp/h
│   │   ├── ViewTripDialog.cpp/h
│   │   ├── FilterTripDialog.cpp/h
│   │   └── ManagePeopleDialog.cpp/h
│   ├── Models/               # Data models
│   │   ├── Trip.cpp
│   │   ├── Person.cpp
│   │   ├── Date.cpp
│   │   └── header.h
│   └── Managers/             # Business logic
│       ├── TripManager.cpp/h
│       ├── PersonManager.cpp/h
│       └── FileManager.cpp/h
├── build.bat                 # Build script
├── run.bat                   # Run script
├── clean.bat                 # Clean script
└── README.md                 # This file
```

## � Troubleshooting

### Common Issues

**Q: "qmake not found" error**
A: Make sure Qt is installed and added to your system PATH
- Windows: Add `C:\Qt\6.x.x\mingw_64\bin` to PATH
- Restart command prompt after changing PATH

**Q: Build fails with compiler errors**
A: Ensure you have a compatible C++ compiler installed
- For Windows: Install Qt with MinGW or Visual Studio

**Q: Application doesn't start**
A: Check if Qt runtime libraries are available
- Make sure Qt's bin directory is in PATH
- Or copy Qt DLLs to the application directory

**Q: Data files not loading**
A: Ensure the application has write permissions in its directory

### Getting Help
If you encounter issues:
1. Check that Qt is properly installed
2. Verify your compiler setup
3. Make sure all dependencies are met
4. Try building manually with qmake + make

## 📊 Sample Data

The application comes with sample data files:
- `People_info.csv` - Sample people data
- `Trips_info.csv` - Sample trips data

You can import these files to quickly test the application's features.

## 🎯 Usage Tips

1. **Start with People**: Import or add people before creating trips
2. **Use Import/Export**: Easily share data between users
3. **Regular Backups**: Export your data regularly
4. **Filter Views**: Use filters to manage large datasets
5. **Status Updates**: Keep trip statuses current

## 🔮 Future Enhancements

- [ ] Database integration
- [ ] Network sharing capabilities
- [ ] Mobile app companion
- [ ] Advanced reporting
- [ ] Calendar integration

## 👥 Contributing

This is a student project for OOP course. Feedback and suggestions are welcome!

## 📚 Functions & Features

### 1. Trip Management
**Purpose**: Core CRUD operations for trips
- ✅ Create new trips
- ✅ Read/view trip details
- ✅ Update existing trips
- ✅ Delete trips

**Usage**:
```cpp
// Add new trip
Trip newTrip("T001", "Paris", "Group vacation", startDate, endDate, Status::PLANNED);
tripManager.addTrip(newTrip);
```

### 2. Import/Export System
**Purpose**: Data persistence and sharing
- ✅ Import trips from text files
- ✅ Export trips to various formats
- ✅ Backup and restore functionality

**Usage**:
- Click "Import Trips" → Select .txt file
- Click "Export Trips" → Choose destination and format

### 3. Search & Filter
**Purpose**: Quick trip discovery
- ✅ Search by trip ID
- ✅ Search by destination
- ✅ Search by description content
- ✅ Case-insensitive matching

**Usage**:
- Type search term in search bar
- Press Enter or click "Search"
- View filtered results instantly

### 4. Trip Browsing
**Purpose**: Navigate through trip collection
- ✅ One-by-one trip viewing
- ✅ Previous/Next navigation
- ✅ Trip counter display
- ✅ Edit access from view mode

**Usage**:
- Click "View Trips"
- Use Previous/Next buttons to navigate
- Click "Edit" to modify current trip

### 5. Data Validation
**Purpose**: Ensure data integrity
- ✅ Date range validation
- ✅ Required field checking
- ✅ Duplicate ID prevention
- ✅ Status consistency

## 🎯 Target Users

- **Friend Groups**: Planning group trips and vacations
- **Travel Organizers**: Managing multiple trip itineraries
- **Event Planners**: Coordinating travel-related events
- **Students**: Organizing class trips and excursions

## 💻 System Requirements

### Minimum Requirements
- Windows 10 or later
- Qt 6.0+ framework
- 100MB free disk space
- 512MB RAM

### Recommended
- Windows 11
- Qt 6.5+
- 1GB free disk space
- 2GB RAM

## 🔧 Installation & Setup

### Prerequisites
1. Install Qt 6.0+ with MinGW compiler
2. Ensure CMake 3.16+ is installed
3. Clone the repository

### Build Instructions
```bash
# Navigate to project directory
cd Project

# Run build script
build.bat

# Or manual build
cd simpleQtApp
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

## 🎮 Usage Guide

### Getting Started
1. **Launch Application**: Run `simpleQtApp.exe`
2. **Import Data**: Click "Import Trips" to load existing data
3. **Add Trips**: Use "Add New Trip" for new entries
4. **Browse**: Use "View Trips" to navigate your collection
5. **Search**: Use search bar for quick finding

### File Format
The application supports text files with the following format:
```
TripID,Destination,Description,StartDate,EndDate,Status
T001,Paris,Group vacation,01/06/2024,07/06/2024,Planned
T002,Tokyo,Cultural exchange,15/08/2024,25/08/2024,Completed
```

## 🐛 Troubleshooting

### Common Issues

**Build Errors**
- Ensure Qt is properly installed and in PATH
- Check CMake version compatibility
- Verify MinGW compiler installation

**Runtime Issues**
- Check file permissions for import/export
- Ensure input files follow correct format
- Verify system meets minimum requirements

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:
1. Fork the repository
2. Create feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Authors

- **Hieu Vu Tran Minh** - Project Manager, Reviewer
- **Thinh Hoang Duc** - Tester, Community Manager
- **Bao Tran Viet** - Code Contributor, Documenter

## 🙏 Acknowledgments

- Qt Framework for providing excellent GUI capabilities
- HCMUS OOP Course for project guidance
- Community feedback and suggestions
# Trip Management System

## 📋 Overview

The **Trip Management System** is a comprehensive application designed to help friend groups organize, plan, and manage their trips efficiently. Built with C++ and Qt framework, it provides both console and GUI interfaces for seamless trip management.

## 🚀 What It Is

A desktop application that allows friend groups to:
- Create and manage trip information
- Import/export trip data
- Search and filter trips
- Track trip status and dates
- Collaborate on trip planning

## ⚙️ How It Works

The system operates through two main interfaces:

### Console Interface
- Command-line interaction for basic operations
- Quick data manipulation and testing
- Batch processing capabilities

### GUI Interface (Qt-based)
- User-friendly graphical interface
- Interactive forms and dialogs
- Visual trip browsing and management

## 🛠️ Core Components

### Data Models
- **Trip**: Core entity containing trip information
- **Date**: Custom date handling system
- **Status**: Trip status enumeration (Planned, Ongoing, Completed, Cancelled)

### Managers
- **TripManager**: Handles trip operations and business logic
- **FileManager**: Manages import/export operations

### User Interface
- **MainWindow**: Primary application interface
- **AddTripDialog**: Trip creation interface
- **ViewTripDialog**: Trip browsing interface

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
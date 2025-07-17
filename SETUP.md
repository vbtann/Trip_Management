# Setup Instructions for Contributors

## For Your Friend (Quick Setup)

### Windows Users:
1. Download and extract the ZIP file
2. Double-click `build.bat` to build the project
3. Double-click `run.bat` to run the application

### Linux/Mac Users:
1. Download and extract the ZIP file
2. Open terminal in the project directory
3. Make scripts executable: `chmod +x build.sh run.sh`
4. Run: `./build.sh` to build
5. Run: `./run.sh` to launch

**Note**: If you're on Windows, use the `.bat` files instead of `.sh` files!

## Prerequisites

### Windows:
- Qt 5.15+ or Qt 6.x with MinGW compiler
- Download from: https://www.qt.io/download-qt-installer
- During installation, select:
  - Qt 6.x.x (latest version)
  - MinGW 11.2.0 64-bit compiler
  - Qt Creator (optional but recommended)

### Linux (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev build-essential
```

### Linux (CentOS/RHEL):
```bash
sudo yum install qt6-qtbase-devel qt6-qttools-devel gcc-c++
```

### macOS:
```bash
# Install Homebrew first if you don't have it
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install Qt
brew install qt6
```

## Troubleshooting

### PATH Setup (Windows):
After installing Qt, add to your system PATH:
- `C:\Qt\6.x.x\mingw_64\bin`
- `C:\Qt\Tools\mingw1120_64\bin`

### Verify Installation:
Open command prompt/terminal and type:
```bash
qmake -version
```
You should see Qt version information.

## Build Process Explained

1. **qmake**: Generates platform-specific Makefiles from the .pro file
2. **make/nmake**: Compiles the source code
3. **Executable**: Created in debug/ or release/ folder

## Manual Build (if scripts fail):
```bash
cd simpleQtApp
qmake simpleQtApp.pro
make  # or 'nmake' on Windows with MSVC
```

## Need Help?
- Check Qt installation
- Verify PATH environment variable
- Ensure all source files are present
- Try building with Qt Creator as alternative

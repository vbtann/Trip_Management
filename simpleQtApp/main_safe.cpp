#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>

// Include your models to test if they cause issues
#include "Models/header.h"

int main(int argc, char *argv[]) {
     std::cout << "Starting Trip Management Application..." << std::endl;

     QApplication app(argc, argv);
     std::cout << "QApplication created successfully" << std::endl;

     try {
          std::cout << "Testing model classes..." << std::endl;

          // Test your model classes to see if they cause crashes
          DATE testDate(1, 1, 2025);
          std::cout << "DATE created successfully: " << testDate.toString()
                    << std::endl;

          STATUS testStatus = STATUS::Planned;
          std::cout << "STATUS created successfully: "
                    << statusToString(testStatus) << std::endl;

          std::cout << "Creating simple QMainWindow instead of MainWindow..."
                    << std::endl;

          QMainWindow window;
          window.setWindowTitle("Trip Management System - Safe Mode");
          window.resize(800, 600);

          QWidget *centralWidget = new QWidget(&window);
          window.setCentralWidget(centralWidget);

          QVBoxLayout *layout = new QVBoxLayout(centralWidget);
          QLabel *label = new QLabel(
              "Trip Management System is running in safe mode.\nYour models "
              "work correctly.",
              centralWidget);
          layout->addWidget(label);

          std::cout << "Simple window created successfully" << std::endl;

          std::cout << "Showing window..." << std::endl;
          window.show();
          std::cout << "Window.show() called - window should be visible now"
                    << std::endl;

          // Show a simple message box
          QMessageBox::information(&window, "Qt Test",
                                   "Qt and your models are working! The issue "
                                   "is likely in MainWindow class.");
          std::cout << "Message box should be visible" << std::endl;

          std::cout << "Starting event loop..." << std::endl;
          return app.exec();

     } catch (const std::exception &e) {
          std::cout << "Exception caught: " << e.what() << std::endl;
          return -1;
     } catch (...) {
          std::cout << "Unknown exception caught" << std::endl;
          return -1;
     }
}

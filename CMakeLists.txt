cmake_minimum_required(VERSION 3.16)
project(EventManagementSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets Sql Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql Gui)

set(HEADERS
  src/app/application.h
  src/app/stylesheetmanager.h
  src/views/mainwindow.h
  src/views/eventview.h
  src/views/venueview.h
  src/views/registrationview.h
  src/views/budgetview.h
  src/views/dialogs/eventdialog.h
  src/views/dialogs/venuedialog.h
  src/views/dialogs/registrationdialog.h
  src/views/dialogs/budgetitemdialog.h
  src/models/event.h
  src/models/venue.h
  src/models/validation.h
  src/models/BudgetItem.h
  src/models/EventBudget.h
  src/models/BudgetManager.h
)

set(SOURCES
  src/main.cpp
  src/app/application.cpp
  src/app/stylesheetmanager.cpp
  src/views/mainwindow.cpp
  src/views/eventview.cpp
  src/views/venueview.cpp
  src/views/registrationview.cpp
  src/views/budgetview.cpp
  src/views/dialogs/eventdialog.cpp
  src/views/dialogs/venuedialog.cpp
  src/views/dialogs/registrationdialog.cpp
  src/views/dialogs/budgetitemdialog.cpp
  src/models/event.cpp
  src/models/venue.cpp
  src/models/validation.cpp
  src/models/budgetitem.cpp
  src/models/EventBudget.cpp
  src/models/BudgetManager.cpp
  src/models/Attendee.cpp
  src/models/Registration.cpp
  src/models/CSVExporter.cpp
)

set(RESOURCES
  resources/resources.qrc
)

add_executable(EventManagementSystem
  ${HEADERS}
  ${SOURCES}
  ${RESOURCES}
)

target_link_libraries(EventManagementSystem
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Gui
)

target_include_directories(EventManagementSystem
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
  set_target_properties(EventManagementSystem PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

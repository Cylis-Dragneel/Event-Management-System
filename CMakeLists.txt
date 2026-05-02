cmake_minimum_required(VERSION 3.16)
project(EventManagementSystem VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Aapka Qt Path
set(CMAKE_PREFIX_PATH "C:/Users/LENOVO/OneDrive/Documents/Qt/6.10.3/msvc2022_64")
find_package(Qt6 REQUIRED COMPONENTS Core Sql)

# Compiler ko batana ke baki files kis folder mein hain
include_directories(src/models)

# Ahista ahista sirf un files ko compile karna jinhe hum test kar rahe hain
set(SOURCES 
    main.cpp 
    database.cpp 
    src/models/Attendee.cpp 
    src/models/validation.cpp
    src/models/venue.cpp
    src/models/event.cpp
    src/models/Registration.cpp
    src/models/budgetitem.cpp
)
set(HEADERS 
    database.h 
    src/models/Attendee.h 
    src/models/validation.h
    src/models/venue.h
    src/models/event.h
     src/models/Registration.h
    src/models/BudgetItem.h
)

add_executable(EventManagementSystem ${SOURCES} ${HEADERS})
target_link_libraries(EventManagementSystem PRIVATE Qt6::Core Qt6::Sql)

# Qt ki helper files copy karne ka code
find_program(WINDEPLOYQT windeployqt HINTS "C:/Users/LENOVO/OneDrive/Documents/Qt/6.10.3/msvc2022_64/bin")
add_custom_command(TARGET EventManagementSystem POST_BUILD
    COMMAND ${WINDEPLOYQT} "$<TARGET_FILE:EventManagementSystem>"
)

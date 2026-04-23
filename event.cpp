#include "event.h"
#include <iostream>
#include <stdexcept>  // ✅ For exceptions
#include <cctype>     // For isdigit(), tolower()
using namespace std;

int daysInMonth(int m, int y) {
    int d;
    if(m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) { 
        d = 31;
    } 
    else if(m == 4 || m == 6 || m == 9 || m == 11) { 
        d = 30;
    }
    else { // checking leap year
        if((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
            d = 29;
        }
        else {
            d = 28;
        }
    }
    return d;
}

bool validDate(int day, int month, int year) {
    if(month<1 || month>12 || year<1) {
        return false;
    }
    if(day<1 || day>daysInMonth(month, year)) {
        return false;
    }
    if(year < 2025 || year > 2030) {  
        return false;
    }
    return true;
} 

Event::Event() {
    name = "unknown";
    description = "none";
    date = "01-01-2000";
    time = "00:00";
    duration = 0;
    capacity = 0;
    status = 0; // Draft by default
    type = 0;  // Conference by default
    eventId = 0;   
    venueId = 0;    
}

Event::Event(int eventId, string name, string description, string date, string time, 
             int duration, int capacity, int type, int venueId) {
    this->eventId = eventId;
    this->name = name;
    this->description = description;
    this->date = date;
    this->time = time;
    this->duration = duration;
    this->capacity = capacity;
    this->type = type;
    this->venueId = venueId;
    status = 0; // default
}

// SETTERS
void Event::setName(string name) { 
    this->name = name; 
}

void Event::setDescription(string description) {
    this->description = description; 
}

void Event::setDate(string date) {
    if(isValidDate(date)) {
        this->date = date;
    }
    else {
        this->date = "01-01-2000";
        throw invalid_argument("Invalid date format! Expected DD-MM-YYYY.");
    }
}

void Event::setTime(string time) {
    if(isValidTime(time)) {
        this->time = time;
    }
    else {
        throw invalid_argument("Invalid time format! Expected HH:MM.");
    }
}

void Event::setDuration(int duration) { 
    if(isValidDuration(duration)) {
        this->duration = duration; 
    }
    else {
        throw out_of_range("Invalid! Duration must be between 30 and 500 minutes.");
    }
}

void Event::setCapacity(int capacity) {
    if (isValidCapacity(capacity)) {
        this->capacity = capacity;
    }
    else {
        throw out_of_range("Invalid! Capacity must be between 10 and 500.");
    }
}

void Event::setType(int type) { 
    if(type>=0 && type<=5) {
        this->type = type; 
    }
    else {
        throw out_of_range("Invalid! Type must be between 0 and 5.");
    }
}

// GETTERS 
string Event::getName() { 
    return name; 
}
string Event::getDescription() { 
    return description; 
}
string Event::getDate() { 
    return date; 
}
string Event::getTime() { 
    return time; 
}
int Event::getDuration() { 
    return duration; 
}
int Event::getCapacity() { 
    return capacity; 
}
int Event::getType() { 
    return type; 
}

string Event::getTypeText() {
    string types[] = {"Conference", "Workshop", "Concert", "Wedding", "Corporate", "Social"};
    if (type >= 0 && type <= 5)
    {
        return types[type];
    }
    return "Unknown";
}

int Event::getStatus() { 
    return status; 
}

string Event::getStatusText() {
    string statuses[] = {"Draft", "Published", "Completed", "Cancelled"};
    if (status >= 0 && status <= 3) {
        return statuses[status];
    }
    return "Unknown";
}

// Status Transition
bool Event::changeStatus(int newStatus) {
    // Rule 1: Cannot change a Cancelled event
    if (status == 3) {
        return false; 
    }

    // Rule 2: Cannot change a Completed event
    if (status == 2) {
        return false;
    }

    // Rule 3: Draft can only go to Published or Cancelled
    if (status == 0) {
        if (newStatus == 1 || newStatus == 3) {
            status = newStatus;
            return true;
        }
        return false; 
    }

    // Rule 4: Published can go to Completed or Cancelled
    if (status == 1) {
        if (newStatus == 2 || newStatus == 3) {
            status = newStatus;
            return true;
        }
        return false; 
    }
    return false;
}

// Check Validation
bool Event::isValidCapacity(int capacity) {
    if (capacity > 10 && capacity <= 500) {
        return true;
    }
    return false;
}

bool Event::isValidDuration(int duration) {
    if (duration > 30 && duration <= 500) {
        return true;
    }
    return false;
}

bool Event::isValidDate(string date) {
    // format should be "DD-MM-YYYY"
    if (date.length() != 10) {
        return false;
    }
    if (date[2] != '-' || date[5] != '-') {
        return false;
    }
    
    string dayStr = date.substr(0, 2);
    string monthStr = date.substr(3, 2);
    string yearStr = date.substr(6, 4);
    
    // Check if all are digits
    for(int i=0; i<dayStr.length(); i++) {
        if (!isdigit(dayStr[i])) return false;
    }
    for(int i=0; i<monthStr.length(); i++) {
        if (!isdigit(monthStr[i])) return false;
    }
    for(int i=0; i<yearStr.length(); i++) {
        if (!isdigit(yearStr[i])) return false;
    }
    
    int day = stoi(dayStr);
    int month = stoi(monthStr);
    int year = stoi(yearStr);

    if(!validDate(day, month, year)) {
        return false;
    }
    return true;
}

bool Event::isValidTime(string time) {
    // format should be "00:00"
    if (time.length() != 5) {
        return false;
    }
    if (time[2] != ':') {
        return false;
    }
    
    // Extract hours and minutes
    string hourStr = time.substr(0, 2);
    string minStr = time.substr(3, 2);
    
    // Check if all are digits
    for(int i=0; i<hourStr.length(); i++) {
        if (!isdigit(hourStr[i])) 
            return false;
    }
    for(int i=0; i<minStr.length(); i++) {
        if (!isdigit(minStr[i])) 
            return false;
    }

    int hour = stoi(hourStr);
    int minute = stoi(minStr);

    if (hour < 0 || hour > 23) {
        return false;
    }
    if (minute < 0 || minute > 59) {
        return false;
    }
    return true;
}

// SEARCH & FILTER FUNCTIONS 
bool Event::matchesType(int searchType) {
    return (type == searchType);
}

bool Event::matchesStatus(int searchStatus) {
    return (status == searchStatus);
}

bool Event::isOnDate(string searchDate) {
    return (date == searchDate);
}   

bool Event::isInDateRange(string startDate, string endDate) {
    if(date>=startDate && date<=endDate) {
        return true;
    }
    return false;
}

bool Event::matchesKeyword(string keyword) {
    // convert all to lowercase
    string lowerName = name;
    string lowerDesc = description;
    string lowerKeyword = keyword;

    for(int i=0; i<lowerName.length(); i++) {
        lowerName[i] = tolower(lowerName[i]);
    }
    for(int i=0; i<lowerDesc.length(); i++) {
        lowerDesc[i] = tolower(lowerDesc[i]);
    }
    for(int i=0; i<lowerKeyword.length(); i++) {
        lowerKeyword[i] = tolower(lowerKeyword[i]);
    }

    bool matchName = (lowerName.find(lowerKeyword) < lowerName.length());
    bool matchDesc = (lowerDesc.find(lowerKeyword) < lowerDesc.length());

    return (matchName || matchDesc);
}


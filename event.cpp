#include "event.h"
#include <iostream>
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
}

Event::Event(string n, string d, string dt, string tm, int dur, int cap, int typ) {
    name = n;
    description = d;
    date = dt;
    time = tm;
    duration = dur;
    capacity = cap;
    status = 0; // starting as Draft
    type = typ;
}

// SETTERS
void Event::setName(string n) { 
    name = n; 
}

void Event::setDescription(string d) {
    description = d; 
}

void Event::setDate(string dt) {
    if(isValidDate(dt)) {
        date = dt;
    }
    else {
        cout << "Invalid date! Setting to default." << endl;
        date = "01-01-2000";
    }
}

void Event::setTime(string tm) {
    if(isValidTime(tm)) {
        time = tm;
    }
    else {
        cout << "Invalid time! Setting to default." << endl;
        time = "00:00";
    }
}

void Event::setDuration(int dur) { 
    if(isValidDuration(dur)) {
        duration = dur; 
    }
    else {
        cout << "Invalid duration! Setting to default." << endl;
        duration = 0;
    }
}

void Event::setCapacity(int cap) {
    if (isValidCapacity(cap)) {
        capacity = cap;
    }
    else {
        cout << "Invalid capacity! Setting to default." << endl;
        capacity = 0;
    }
}

void Event::setType(int t) { 
    if(t>=0 && t<=5) {
        type = t; 
    }
    else {
        cout << "Invalid type! Setting to default(conference)" << endl;
        type = 0;
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

bool Event::isValidCapacity(int cap) {
    if (cap > 10 && cap <= 500) {
        return true;
    }
    return false;
}

bool Event::isValidDuration(int dur) {
    if (dur > 30 && dur <= 500) {
        return true;
    }
    return false;
}

bool Event::isValidDate(string dt) {
    // format should be "DD-MM-YYYY"
    if (dt.length() != 10) {
        return false;
    }
    if (dt[2] != '-' || dt[5] != '-') {
        return false;
    }
    
    string dayStr = dt.substr(0, 2);
    string monthStr = dt.substr(3, 2);
    string yearStr = dt.substr(6, 4);
    
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

bool Event::isValidTime(string tm) {
    // format should be "00:00"
    if (tm.length() != 5) {
        return false;
    }
    if (tm[2] != ':') {
        return false;
    }
    
    // Extract hours and minutes
    string hourStr = tm.substr(0, 2);
    string minStr = tm.substr(3, 2);
    
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
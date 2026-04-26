#include "event.h"
#include "validation.h"
#include <iostream>
using namespace std;

int Event::nextEventId = 1;  

Event::Event() {
    eventId = nextEventId++; 
    name = "unknown";
    description = "none";
    date = "01-01-2000";
    time = "00:00";
    duration = 0;
    capacity = 0;
    status = 0; // Draft by default
    type = 0;  // Conference by default
}

Event::Event(string name, string description, string date, string time, 
            int duration, int capacity, int type) {
    this->eventId = nextEventId++; 
    this->name = name;
    this->description = description;
    this->date = date;
    this->time = time;
    this->duration = duration;
    this->capacity = capacity;
    this->type = type;
    status = 0; // default
}

// SETTERS
void Event::setName(string name) { 
    if(Validation::isValidName(name)) {
        this->name = name;
    }
    else {
        throw invalid_argument("Invalid Event name!");
    }
}

void Event::setDescription(string description) {
    if(Validation::isValidAddress(description)) {
        this->description = description; 
    }
    else {
        throw invalid_argument("Invalid Event description!");
    }
}

void Event::setDate(string date) {
    if(Validation::isValidDate(date)) {
        this->date = date;
    }
    else {
        this->date = "01-01-2000";
        throw invalid_argument("Invalid date format! Expected DD-MM-YYYY.");
    }
}

void Event::setTime(string time) {
    if(Validation::isValidTime(time)) {
        this->time = time;
    }
    else {
        throw invalid_argument("Invalid time format! Expected HH:MM.");
    }
}

void Event::setDuration(int duration) { 
    if(duration > 30 && duration <= 500) {
        this->duration = duration; 
    }
    else {
        throw out_of_range("Invalid! Duration must be between 30 and 500 minutes.");
    }
}

void Event::setCapacity(int capacity) {
    if (capacity > 10 && capacity <= 1000) {
        this->capacity = capacity;
    }
    else {
        throw out_of_range("Invalid! Capacity must be between 10 and 1000.");
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

void Event::setVenue(Venue venue) {
    this->venue = venue;
}

// GETTERS 
int Event::getEventId() { 
    return eventId; 
}
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
Venue Event::getVenue() {
    return venue;
}
int Event::getVenueId() {
    return venue.getVenueId();
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


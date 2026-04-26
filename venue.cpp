#include "venue.h"
#include "validation.h"
#include <stdexcept>
using namespace std;

int Venue::nextVenueId = 1;

// CONSTRUCTORS 
Venue::Venue() {
    venueId = nextVenueId++;
    name = "unknown";
    address = "unknown";
    capacity = 0;
    contactNumber = "unknown";
    contactEmail = "unknown";
    hasWifi = false;
    hasParking = false;
    hasCatering = false;
    hasAVEquipment = false;
}

Venue::Venue(string name, string address, int capacity, string contactNumber, string contactEmail,
             bool hasWifi, bool hasParking, bool hasCatering, bool hasAVEquipment) {
    this->venueId = nextVenueId++;
    this->name = name;
    this->address = address;
    this->capacity = capacity;
    this->contactNumber = contactNumber;
    this->contactEmail = contactEmail;
    this->hasWifi = hasWifi;
    this->hasParking = hasParking;
    this->hasCatering = hasCatering;
    this->hasAVEquipment = hasAVEquipment;
}

// SETTERS

void Venue::setName(string name) {
    if(Validation::isValidName(name)) {
        this->name = name;
    }
    else {
        throw invalid_argument("Invalid Venue name!");
    }
}

void Venue::setAddress(string address) {
    if(Validation::isValidAddress(address)) {
        this->address = address;
    }
    else {
        throw invalid_argument("Invalid Venue address!");
    }
}

void Venue::setCapacity(int capacity) {
    if(capacity >= 1 && capacity <= 1000) {
        this->capacity = capacity;
    }
    else {
        throw out_of_range("Venue capacity must be between 1 and 1000!");
    }
}

void Venue::setContactNumber(string contactNumber) {
    if(Validation::isValidPhone(contactNumber)) {
        this->contactNumber = contactNumber;
    }
    else {
        throw invalid_argument("Contact Number must be in Format: 0000-1234567!");
    }
}

void Venue::setContactEmail(string contactEmail) {
    if(Validation::isValidEmail(contactEmail)) {
        this->contactEmail = contactEmail;
    }
    else {
        throw invalid_argument("Contact Email must be in Format: 0000-1234567!");
    }
}

// AMENITIES SETTERS 
void Venue::setHasWifi(bool hasWifi) {
    this->hasWifi = hasWifi;
}

void Venue::setHasParking(bool hasParking) {
    this->hasParking = hasParking;
}

void Venue::setHasCatering(bool hasCatering) {
    this->hasCatering = hasCatering;
}

void Venue::setHasAVEquipment(bool hasAVEquipment) {
    this->hasAVEquipment = hasAVEquipment;
}

// GETTERS
int Venue::getVenueId() {
    return venueId;
}

string Venue::getName() {
    return name;
}

string Venue::getAddress() {
    return address;
}

int Venue::getCapacity() {
    return capacity;
}

string Venue::getContactNumber() {
    return contactNumber;
}

string Venue::getContactEmail() {
    return contactEmail;
}

// AMENITIES GETTERS 
bool Venue::getHasWifi() {
    return hasWifi;
}

bool Venue::getHasParking() {
    return hasParking;
}

bool Venue::getHasCatering() {
    return hasCatering;
}

bool Venue::getHasAVEquipment() {
    return hasAVEquipment;
}

string Venue::getAmenitiesList() {
    string list = "";
    
    if(hasWifi) {
        list += "WiFi ";
    }
    if(hasParking) {
        list += "Parking ";
    }
    if(hasCatering) {
        list += "Catering ";
    }
    if(hasAVEquipment) {
        list += "AV Equipment ";
    }
    if(list.empty()) {
        return "No amenities";
    }
    
    return list;
}
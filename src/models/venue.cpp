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
    setName(name);
    setAddress(address);
    setCapacity(capacity);
    setContactNumber(contactNumber);
    setContactEmail(contactEmail);
    setHasWifi(hasWifi);
    setHasParking(hasParking);
    setHasCatering(hasCatering);
    setHasAVEquipment(hasAVEquipment);
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
    if(Validation::isValidText(address)) {
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
        throw invalid_argument("Contact Email must be in Format: abc@gmail.com!");
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
int Venue::getVenueId() const {
    return venueId;
}

string Venue::getName() const {
    return name;
}

string Venue::getAddress() const {
    return address;
}

int Venue::getCapacity() const {
    return capacity;
}

string Venue::getContactNumber() const {
    return contactNumber;
}

string Venue::getContactEmail() const {
    return contactEmail;
}

// AMENITIES GETTERS 
bool Venue::getHasWifi() const {
    return hasWifi;
}

bool Venue::getHasParking() const {
    return hasParking;
}

bool Venue::getHasCatering() const {
    return hasCatering;
}

bool Venue::getHasAVEquipment() const {
    return hasAVEquipment;
}

string Venue::getAmenitiesList() const {
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

bool Venue::matchesKeyword(string keyword) {
    // Convert to lowercase
    string lowerName = name;
    string lowerAddress = address;
    string lowerKeyword = keyword;

    for(int i = 0; i < lowerName.length(); i++) {
        lowerName[i] = tolower(lowerName[i]);
    }
    for(int i = 0; i < lowerAddress.length(); i++) {
        lowerAddress[i] = tolower(lowerAddress[i]);
    }
    for(int i = 0; i < lowerKeyword.length(); i++) {
        lowerKeyword[i] = tolower(lowerKeyword[i]);
    }

    bool matchName = (lowerName.find(lowerKeyword) != string::npos);
    bool matchAddress = (lowerAddress.find(lowerKeyword) != string::npos);

    return (matchName || matchAddress);
}

bool Venue::matchesCapacity(int minCapacity, int maxCapacity) {
    return (capacity >= minCapacity && capacity <= maxCapacity);
}

bool Venue::hasAmenity(string amenityName) {
    // covert to lowercase
    for(int i = 0; i < amenityName.length(); i++) {
        amenityName[i] = tolower(amenityName[i]);
    }

    if(amenityName == "wifi" && hasWifi){
        return true;
    }
    if(amenityName == "parking" && hasParking){
        return true;
    }
    if(amenityName == "catering" && hasCatering){
        return true;
    }
    if((amenityName == "av" || amenityName == "av equipment") && hasAVEquipment){
        return true;
    }
    
    return false;
}
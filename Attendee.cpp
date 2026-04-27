#include "Attendee.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Attendee::Attendee() {
    attendeeId = -1;
    firstName = "";
    lastName = "";
    email = "";
    phone = "";
    address = "";
}

Attendee::Attendee(int attendeeId, string firstName, string lastName, string email, string phone, string address) {
    
    this->attendeeId = attendeeId;
    this->firstName = firstName;
    this->lastName = lastName;
    this->email = email;
    this->phone = phone;
    this->address = address;
}

Attendee::~Attendee() {}

int Attendee::getAttendeeId() {
     return attendeeId; 
}
string Attendee::getFirstName() {
     return firstName;
 }
string Attendee::getLastName() { 
    return lastName; 
}
string Attendee::getFullName() 
{ 
    return firstName + " " + lastName; 
}
string Attendee::getEmail(){
     return email; 
 }
string Attendee::getPhone() { 
    return phone; 
}
string Attendee::getAddress() { 
    return address; 
}

void Attendee::setAttendeeId(int id) {
    if(id > 0) {
        this->attendeeId = id;
    }
    else {
        throw invalid_argument("Invalid Attendee ID!");
    }
}

void Attendee::setFirstName(string name) {
    if(Validation::isValidName(name)) {
        this->firstName = name;
    }
    else {
        throw invalid_argument("Invalid first name!");
    }
}

void Attendee::setLastName(string name) {
    if(Validation::isValidName(name)) {
        this->lastName = name;
    }
    else {
        throw invalid_argument("Invalid last name!");
    }
}

void Attendee::setEmail(string email) {
    if(Validation::isValidEmail(email)) {
        this->email = email;
    }
    else {
        throw invalid_argument("Invalid email!");
    }
}

void Attendee::setPhone(string phone) {
    if(phone == "") {
        this->phone = phone;
    }
    else if(Validation::isValidPhone(phone)) {
        this->phone = phone;
    }
    else {
        throw invalid_argument("Invalid phone number! Format: 03XX-XXXXXXX");
    }
}

void Attendee::setAddress(string address) {
    if(address == "") {
        this->address = address;
    }
    else if(Validation::isValidAddress(address)) {
        this->address = address;
    }
    else {
        throw invalid_argument("Invalid address! Address must be at least 5 characters");
    }
}

// Validation functions
bool Attendee::isValidId() {
    return (attendeeId > 0);
}

bool Attendee::isValidName() {
    return (firstName != "" && lastName != "");
}

bool Attendee::isValidEmail() {
    return Validation::isValidEmail(email);
}

bool Attendee::isValidPhone() {
    if(phone == "") 
    return true;
    return Validation::isValidPhone(phone);
}

bool Attendee::isValidAddress() {
    if(address == "") 
    return true;
    return Validation::isValidAddress(address);
}

bool Attendee::isComplete() {
    return (isValidId() && isValidName() && isValidEmail() && isValidPhone() && isValidAddress());
}
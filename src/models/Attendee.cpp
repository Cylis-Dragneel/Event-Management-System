#include"attendee.h"
#include "validation.h"
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
Attendee::Attendee(string firstName, string lastName, string email, string phone, string address) {
    this->attendeeId = nextId++;
    this->firstName = firstName;
    this->lastName = lastName;
    this->email = email;
    this->phone = phone;
    this->address = address;
}

Attendee::Attendee(int attendeeId, string firstName, string lastName, string email, string phone, string address) {
    
    setAttendeeId(attendeeId);
    setFirstName(firstName);
    setLastName(lastName);
    setEmail(email);
    setPhone(phone);
    setAddress(address);

    if(attendeeId >= nextId) {
        nextId = attendeeId + 1;
    }
}

Attendee::~Attendee() {}

int Attendee::getAttendeeId() const {
    return attendeeId;
}

string Attendee::getFirstName() const {
    return firstName;
}

string Attendee::getLastName() const {
    return lastName;
}

string Attendee::getFullName() const {
    return firstName + " " + lastName;
}

string Attendee::getEmail() const {
    return email;
}

string Attendee::getPhone() const {
    return phone;
}

string Attendee::getAddress() const {
    return address;
}

void Attendee::setAttendeeId(int id) {
    if(id > 0) {
        attendeeId = id;
    }
    else {
        throw invalid_argument("Invalid Attendee ID! ID must be positive.");
    }
}

void Attendee::setFirstName(string name) {
    if(Validation::isValidPersonName(name)) {
        firstName = name;
    }
    else {
        throw invalid_argument("Invalid first name! Must contain letters, spaces, dash. Min 3 characters.");
    }
}

void Attendee::setLastName(string name) {
    if(Validation::isValidPersonName(name)) {
        lastName = name;
    }
    else {
        throw invalid_argument("Invalid last name! Must contain letters, spaces, dash. Min 3 characters.");
    }
}

void Attendee::setEmail(string email) {
    if(Validation::isValidEmail(email)) {
        this->email = email;
    }
    else {
        throw invalid_argument("Invalid email! Must contain @ and . after @.");
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
        throw invalid_argument("Invalid phone number! Format: 03XX-XXXXXXX or 042-XXXXXXX.");
    }
}

void Attendee::setAddress(string address) {
    if(address == "") {
        this->address = address;
    }
    else if(Validation::isValidText(address)) {
        this->address = address;
    }
    else {
        throw invalid_argument("Invalid address! Must be at least 5 characters.");
    }
}

bool Attendee::isValidId() const {
    return (attendeeId > 0);
}

bool Attendee::isValidFirstName() const {
    if(firstName == "") return false;
    return Validation::isValidPersonName(firstName);
}

bool Attendee::isValidLastName() const {
    if(lastName == "") return false;
    return Validation::isValidPersonName(lastName);
}

bool Attendee::isValidName() const {
    return (isValidFirstName() && isValidLastName());
}

bool Attendee::isValidEmail() const {
    if(email == "") return false;
    return Validation::isValidEmail(email);
}


bool Attendee::isValidPhone() const {
    if(phone == "") return true;
    return Validation::isValidPhone(phone);
}


bool Attendee::isValidAddress() const {
    if(address == "") return true;
    return Validation::isValidText(address);
}

bool Attendee::isComplete() const {
    return (isValidId() && isValidName() && isValidEmail());
}
#include "Attendee.h"


Attendee::Attendee() {
    attendeeId = -1;
    firstName = "";
    lastName = "";
    email = "";
    phone = "";
    address = "";
}


Attendee::Attendee(int attendeeId, string firstName, 
                   string lastName, string email) {
    this->attendeeId = attendeeId;
    this->email = email;
    this->phone = "";
    this->address = "";
    
    if (containsOnlyLettersAndSpaces(firstName)) {
        this->firstName = firstName;
    } else {
        this->firstName = "";
    }
    
    if (containsOnlyLettersAndSpaces(lastName)) {
        this->lastName = lastName;
    } else {
        this->lastName = "";
    }
}

Attendee::Attendee(int attendeeId, string firstName, string lastName,
                   string email, string phone, string address) {
    this->attendeeId = attendeeId;
    this->email = email;
    this->phone = phone;
    this->address = address;
    
    if (containsOnlyLettersAndSpaces(firstName)) {
        this->firstName = firstName;
    } else {
        this->firstName = "";
    }
    
    if (containsOnlyLettersAndSpaces(lastName)) {
        this->lastName = lastName;
    } else {
        this->lastName = "";
    }
}


Attendee::~Attendee() {}

bool Attendee::containsOnlyLettersAndSpaces(string str) {
    if (str.length() == 0) {
        return false;
    }
    
    for (int i = 0; i < (int)str.length(); i++) {
        char c = str[i];
        bool isLetter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        bool isSpace = (c == ' ');
        
        if (!isLetter && !isSpace) {
            return false;
        }
    }
    return true;
}

int Attendee::getAttendeeId() { 
    return attendeeId; 
}

string Attendee::getFirstName() { 
    return firstName; 
}

string Attendee::getLastName() { 
    return lastName; 
}

string Attendee::getFullName() { 
    if (firstName == "" || lastName == "") {
        return "";
    }
    return firstName + " " + lastName; 
}

string Attendee::getEmail() { 
    return email; 
}

string Attendee::getPhone() { 
    return phone; 
}

string Attendee::getAddress() { 
    return address; 
}


void Attendee::setAttendeeId(int id) {
    if (id > 0 && id <= 999999) {
        attendeeId = id;
    }
}

void Attendee::setFirstName(string name) {
    if (containsOnlyLettersAndSpaces(name)) {
        firstName = name;
    }
}

void Attendee::setLastName(string name) {
    if (containsOnlyLettersAndSpaces(name)) {
        lastName = name;
    }
}

void Attendee::setEmail(string email) {
    bool hasAt = false;
    bool hasDot = false;
    
    for (int i = 0; i < (int)email.length(); i++) {
        if (email[i] == '@') {
            hasAt = true;
        }
        if (hasAt && email[i] == '.') {
            hasDot = true;
        }
    }
    
    if (hasAt && hasDot && email.length() > 0) {
        this->email = email;
    }
}

void Attendee::setPhone(string phone) {
    if (phone.length() == 0) {
        this->phone = "";
        return;
    }
    
    int digitCount = 0;
    for (int i = 0; i < (int)phone.length(); i++) {
        if (phone[i] >= '0' && phone[i] <= '9') {
            digitCount++;
        }
    }
    
    if (digitCount >= 10 && digitCount <= 15) {
        this->phone = phone;
    }
}

void Attendee::setAddress(string address) {
    if (address.length() <= 200) {
        this->address = address;
    }
}

bool Attendee::isValidId() {
    return (attendeeId > 0 && attendeeId <= 999999);
}


bool Attendee::isValidName() {
    return (firstName != "" && lastName != "");
}

bool Attendee::isValidEmail() {
    if (email.length() == 0) {
        return false;
    }
    
    bool hasAt = false;
    bool hasDot = false;
    
    for (int i = 0; i < (int)email.length(); i++) {
        if (email[i] == '@') {
            hasAt = true;
        }
        if (hasAt && email[i] == '.') {
            hasDot = true;
        }
    }
    
    return (hasAt && hasDot);
}


bool Attendee::isComplete() {
    return (isValidId() && isValidName() && isValidEmail());
}
#ifndef ATTENDEE_H
#define ATTENDEE_H

#include <string>
#include <stdexcept>
#include "validation.h"

using namespace std;

class Attendee {
private:
    int attendeeId;
    string firstName;
    string lastName;
    string email;
    string phone;
    string address;

public:
    Attendee();
    Attendee(int attendeeId, string firstName, string lastName, string email, string phone, string address);
    
    // Destructor
    ~Attendee();
    
    // Getters
    int getAttendeeId();
    string getFirstName();
    string getLastName();
    string getFullName();
    string getEmail();
    string getPhone();
    string getAddress();
    
    
    void setAttendeeId(int id);
    void setFirstName(string name);
    void setLastName(string name);
    void setEmail(string email);
    void setPhone(string phone);
    void setAddress(string address);
    
    // Validation
    bool isValidId();
    bool isValidName();
    bool isValidEmail();
    bool isValidPhone();
    bool isValidAddress();
    bool isComplete();
};

#endif
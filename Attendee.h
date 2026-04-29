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
    
    ~Attendee();
    
    int getAttendeeId() const;
    string getFirstName() const;
    string getLastName() const;
    string getFullName() const;
    string getEmail() const;
    string getPhone() const;
    string getAddress() const;
    
    void setAttendeeId(int id);
    void setFirstName(string name);
    void setLastName(string name);
    void setEmail(string email);
    void setPhone(string phone);
    void setAddress(string address);
    
    // ALL Validation functions 
    bool isValidId() const;
    bool isValidFirstName() const;
    bool isValidLastName() const;
    bool isValidName() const;           
    bool isValidEmail() const;
    bool isValidPhone() const;
    bool isValidAddress() const;
    bool isComplete() const;             
};
#endif
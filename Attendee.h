#ifndef ATTENDEE_H
#define ATTENDEE_H

#include <string>
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
    Attendee(int attendeeId, string firstName, string lastName, string email);
    Attendee(int attendeeId, string firstName, string lastName, 
             string email, string phone, string address);
    
    
    ~Attendee();
    

    int getAttendeeId();
    string getFirstName();
    string getLastName();
    string getFullName();
    string getEmail();
    string getPhone();
    string getAddress();
    
    //  validation
    void setAttendeeId(int id);
    void setFirstName(string name);
    void setLastName(string name);
    void setEmail(string email);
    void setPhone(string phone);
    void setAddress(string address);
    bool isComplete(); 
    
    bool isValidName();
    bool isValidId();
    
    bool isValidEmail();
   
private:
    bool containsOnlyLettersAndSpaces(string str);
};

#endif
#ifndef EVENT_H
#define EVENT_H

#include <string>
using namespace std;

class Event {
private:
    string name;
    string description;
    string date;        // Format: "20-04-2026"
    string time;        // Format: "14:30"
    int duration;       // In minutes(30-500)
    int capacity;       // Max attendees(10-500)
    
    // Status: 0=Draft, 1=Published, 2=Completed, 3=Cancelled
    int status;
    
    // Type: 0=Conference, 1=Workshop, 2=Concert, 3=Wedding, 4=Corporate, 5=Social
    int type;
    
public:
    Event();
    Event(string n, string d, string dt, string tm, int dur, int cap, int typ);
    
    void setName(string n);
    void setDescription(string d);
    void setDate(string dt);
    void setTime(string tm);
    void setDuration(int dur);
    void setCapacity(int cap);
    void setType(int t);
    
    string getName();
    string getDescription();
    string getDate();
    string getTime();
    int getDuration();
    int getCapacity();
    int getType();
    string getTypeText();   
    int getStatus();
    string getStatusText();  
    
    bool changeStatus(int newStatus);  
    
    bool isValidCapacity(int cap);
    bool isValidDuration(int dur);
    bool isValidDate(string dt);
    bool isValidTime(string t);
};

#endif
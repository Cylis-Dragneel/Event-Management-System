#ifndef EVENT_H
#define EVENT_H

#include <string>
using namespace std;

class Event {
private:
    int eventId;
    string name;
    string description;
    string date;  // Format: "20-04-2026"
    string time;  // Format: "14:30"
    int duration; // In minutes(30-500)
    int capacity; // Max attendees(10-500)
    int status;   // 0=Draft, 1=Published, 2=Completed, 3=Cancelled
    int type;     // 0=Conference, 1=Workshop, 2=Concert, 3=Wedding, 4=Corporate, 5=Social
    int venueId;

public:
    // Constructors
    Event();
    Event(int eventId, string name, string description, string date, string time,
            int duration, int capacity, int type, int venueId);

    // Setters
    void setName(string n);
    void setDescription(string d);
    void setDate(string dt);
    void setTime(string tm);
    void setDuration(int dur);
    void setCapacity(int cap);
    void setType(int t);
    void setEventId(int id);
    void setVenueId(int vId);

    // Getters
    string getName();
    string getDescription();
    string getDate();
    string getTime();
    int getDuration();
    int getCapacity();
    int getType();
    string getTypeText();
    int getEventId();
    int getVenueId();
    int getStatus();
    string getStatusText();

    bool changeStatus(int newStatus);

    // Check Validation
    bool isValidCapacity(int cap);
    bool isValidDuration(int dur);
    bool isValidDate(string dt);
    bool isValidTime(string t);

    // Search and Filter Functions
    bool matchesType(int searchType);                    
    bool matchesStatus(int searchStatus);                
    bool isOnDate(string searchDate);                     
    bool isInDateRange(string startDate, string endDate); 
    bool matchesKeyword(string keyword);                 
};

#endif
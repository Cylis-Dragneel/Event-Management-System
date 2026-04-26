#ifndef VENUE_H
#define VENUE_H

#include <string>
using namespace std;

class Venue {
private:
    static int nextVenueId; 
    int venueId;
    string name;
    string address;
    int capacity;
    string contactNumber; // Format: 0000-1234567
    string contactEmail; // Format: abc@gmail.com
    
    // Amenities
    bool hasWifi;
    bool hasParking;
    bool hasCatering;
    bool hasAVEquipment; // projectors, mic, etc
    
public:
    // Constructors
    Venue();
    Venue(string name, string address, int capacity, string contactNumber, string contactEmail,
            bool hasWifi, bool hasParking, bool hasCatering, bool hasAVEquipment);
    
    // Setters
    void setName(string name);
    void setAddress(string address);
    void setCapacity(int capacity);
    void setContactNumber(string contactNumber);
    void setContactEmail(string contactEmail);

    // Amenities Setters
    void setHasWifi(bool hasWifi);
    void setHasParking(bool hasParking);
    void setHasCatering(bool hasCatering);
    void setHasAVEquipment(bool hasAVEquipment);

    // Getters
    int getVenueId();    
    string getName();
    string getAddress();
    int getCapacity();
    string getContactNumber();
    string getContactEmail();
    
    // Amenities Getters
    bool getHasWifi();
    bool getHasParking();
    bool getHasCatering();
    bool getHasAVEquipment();
    
    string getAmenitiesList();  // Returns "WiFi Parking Catering"
};

#endif
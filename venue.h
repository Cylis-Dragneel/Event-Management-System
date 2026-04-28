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
    string contactEmail;  // Format: abc@gmail.com

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
    // venue.h — mark all getters as const
    int getVenueId() const;
    string getName() const;
    string getAddress() const;
    int getCapacity() const; // ← this one fixes the immediate error
    string getContactNumber() const;
    string getContactEmail() const;

    // Amenities Getters
    bool getHasWifi() const;
    bool getHasParking() const;
    bool getHasCatering() const;
    bool getHasAVEquipment() const;
    string getAmenitiesList() const;

    string getAmenitiesList(); // Returns "WiFi Parking Catering"

    // Search & Filter Functions
    bool matchesKeyword(string keyword);                    // Search venue name or address
    bool matchesCapacity(int minCapacity, int maxCapacity); // Filter by capacity range
    bool hasAmenity(string amenityName);                    // Filter by specific amenity (e.g., "WiFi")
};

#endif
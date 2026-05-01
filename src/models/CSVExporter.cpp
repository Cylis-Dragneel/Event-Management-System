#include "CSVExporter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

// Get CSV header
string CSVExporter::getCSVHeader() {
    return "Attendee ID,First Name,Last Name,Email,Phone,Address\n";
}

// Get single attendee as CSV row
string CSVExporter::getAttendeeRow(Attendee attendee) {
    return to_string(attendee.getAttendeeId()) + "," +
           "\"" + attendee.getFirstName() + "\"," +
           "\"" + attendee.getLastName() + "\"," +
           "\"" + attendee.getEmail() + "\"," +
           "\"" + attendee.getPhone() + "\"," +
           "\"" + attendee.getAddress() + "\"\n";
}

// Write content to file
void CSVExporter::writeToFile(string content, string filename) {
    ofstream file(filename);
    if(!file.is_open()) {
        throw invalid_argument("Error: Could not create file " + filename);
    }
    file << content;
    file.close();
}

// Export all attendees to CSV file
bool CSVExporter::exportAttendeesToCSV(Attendee* attendees, int attCount, string filename) {
    if(attendees == NULL || attCount == 0) {
        throw invalid_argument("Error: No attendees to export!");
    }
    
    string content = getCSVHeader();
    
    for(int i = 0; i < attCount; i++) {
        content += getAttendeeRow(attendees[i]);
    }
    
    writeToFile(content, filename);
    return true;
}

// Export attendees as string 
string CSVExporter::getAttendeesAsCSVString(Attendee* attendees, int attCount) {
    if(attendees == NULL || attCount == 0) {
        throw invalid_argument("Error: No attendees to export!");
    }
    
    string content = getCSVHeader();
    
    for(int i = 0; i < attCount; i++) {
        content += getAttendeeRow(attendees[i]);
    }
    
    return content;
}

// Export single attendee to CSV file
bool CSVExporter::exportAttendeeToCSV(Attendee attendee, string filename) {
    string content = getCSVHeader();
    content += getAttendeeRow(attendee);
    
    writeToFile(content, filename);
    return true;
}
#ifndef CSV_EXPORTER_H
#define CSV_EXPORTER_H

#include <string>
#include "Attendee.h"

using namespace std;

class CSVExporter {
public:
    // Export all attendees to CSV file
    static bool exportAttendeesToCSV(Attendee* attendees, int attCount, string filename);
    
    // Export attendees as string 
    static string getAttendeesAsCSVString(Attendee* attendees, int attCount);
    
    // Export single attendee to CSV file
    static bool exportAttendeeToCSV(Attendee attendee, string filename);

private:

    static string getCSVHeader();
    static string getAttendeeRow(Attendee attendee);
    static void writeToFile(string content, string filename);
};

#endif
#include "validation.h"
using namespace std;

int daysInMonth(int m, int y) {
    int d;
    if(m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) { 
        d = 31;
    } 
    else if(m == 4 || m == 6 || m == 9 || m == 11) { 
        d = 30;
    }
    else { // checking leap year
        if((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
            d = 29;
        }
        else {
            d = 28;
        }
    }
    return d;
}

bool validDate(int day, int month, int year) {
    if(month<1 || month>12 || year<1) {
        return false;
    }
    if(day<1 || day>daysInMonth(month, year)) {
        return false;
    }
    if(year < 2025 || year > 2030) {  
        return false;
    }
    return true;
} 

bool Validation::isValidDate(string date) {
    // format should be "DD-MM-YYYY"
    if (date.length() != 10) {
        return false;
    }
    if (date[2] != '-' || date[5] != '-') {
        return false;
    }
    
    string dayStr = date.substr(0, 2);
    string monthStr = date.substr(3, 2);
    string yearStr = date.substr(6, 4);
    
    // Check if all are digits
    for(int i=0; i<dayStr.length(); i++) {
        if (!isdigit(dayStr[i])) return false;
    }
    for(int i=0; i<monthStr.length(); i++) {
        if (!isdigit(monthStr[i])) return false;
    }
    for(int i=0; i<yearStr.length(); i++) {
        if (!isdigit(yearStr[i])) return false;
    }
    
    int day = stoi(dayStr);
    int month = stoi(monthStr);
    int year = stoi(yearStr);

    if(!validDate(day, month, year)) {
        return false;
    }
    return true;
}

bool Validation::isValidTime(string time) {
    // format should be "00:00"
    if (time.length() != 5) {
        return false;
    }
    if (time[2] != ':') {
        return false;
    }
    
    // Extract hours and minutes
    string hourStr = time.substr(0, 2);
    string minStr = time.substr(3, 2);
    
    // Check if all are digits
    for(int i=0; i<hourStr.length(); i++) {
        if (!isdigit(hourStr[i])) 
            return false;
    }
    for(int i=0; i<minStr.length(); i++) {
        if (!isdigit(minStr[i])) 
            return false;
    }

    int hour = stoi(hourStr);
    int minute = stoi(minStr);

    if (hour < 0 || hour > 23) {
        return false;
    }
    if (minute < 0 || minute > 59) {
        return false;
    }
    return true;
}

bool Validation::isValidName(string name) {
    if(name.length() < 2) {
        return false;
    }

    // Check if only spaces
    bool onlySpaces = true;
    for(int i = 0; i < name.length(); i++) {
        if(name[i] != ' ') {
            onlySpaces = false;
            break;
        }
    }
    if(onlySpaces) {
        return false;
    }
        
    // Check if contains at least one letter (allow spaces and dash)
    bool hasLetter = false;
    for(int i = 0; i < name.length(); i++) {
        if(isalpha(name[i])) {
            hasLetter = true;
        }
        else if(name[i] != ' ' && name[i] != '-') {
            return false;  // Invalid character found
        }
    }
    
    if(!hasLetter) {
        return false;  
    }

    return true;
}

bool Validation::isValidAddress(string address) {
    if(address.length() < 5) {
        return false;
    }

    // Check if only spaces
    bool onlySpaces = true;
    for(int i = 0; i < address.length(); i++) {
        if(address[i] != ' ') {
            onlySpaces = false;
            break;
        }
    }
    if(onlySpaces) {
        return false;
    }

    return true;
}

bool Validation::isValidPhone(string contactNumber) {
    if (contactNumber.length() != 12) {
        return false;
    }
    if (contactNumber[4] != '-') {
        return false;
    }
    if (contactNumber[0] != '0') {
        return false;
    }
    if (contactNumber[1] != '3') {
        return false;
    }
    
    string preDashStr = contactNumber.substr(0, 4);
    string postDashStr = contactNumber.substr(5, 7);
    
    // Check if all are digits
    for(int i=0; i<preDashStr.length(); i++) {
        if (!isdigit(preDashStr[i])) return false;
    }
    for(int i=0; i<postDashStr.length(); i++) {
        if (!isdigit(postDashStr[i])) return false;
    }
    return true;
}

bool Validation::isValidEmail(string contactEmail) {
    if(contactEmail.empty()) {
        return false;
    }
 
    // check if there is exactly one @
    int atCount = 0, atPos = -1;
    for(int i = 0; i < contactEmail.length(); i++) {
        if(contactEmail[i] == '@') {
            atCount++;
            atPos = i;
        }
    }
    if(atCount != 1) {
        return false;  
    }

    // Check @ is not first or last character
    if(contactEmail[0] == '@' || contactEmail[contactEmail.length() - 1] == '@') {
        return false;
    }

    // Check if there is '.' after @
    bool foundDot = true;
    for(int i = atPos+1; i < contactEmail.length(); i++) {
        if(contactEmail[i] != '.') {
            foundDot = false;
            break;
        }
    }
    return foundDot;
}
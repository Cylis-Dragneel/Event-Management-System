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

// checks if string contains only spaces(invalid)
bool isNotOnlySpaces(const string& s) {
    for(int i = 0; i < (int)s.length(); i++) {
        if(s[i] != ' ') return true;
    }
    return false;
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
    if(name.length() < 3) {
        return false;
    }
    if(!isNotOnlySpaces(name)) {
        return false;
    }    
        
    // Check if contains at least one letter (allow spaces and dash)
    bool hasLetterOrDigit = false;
    for(int i = 0; i < name.length(); i++) {
        if(isalpha(name[i]) || isdigit(name[i])) {
            hasLetterOrDigit = true;
        }
        else if(name[i] != ' '  && name[i] != '-' && name[i] != '&'  && name[i] != '\'' &&
                name[i] != '('  && name[i] != ')'  && name[i] != '.'  && name[i] != '/'  &&
                name[i] != '+'  && name[i] != ':') {
            return false;  // Invalid character found
        }
    }
    return hasLetterOrDigit;
}

bool Validation::isValidPersonName(string pName) {
    if(pName.length() < 3) {
        return false;
    }
    if(!isNotOnlySpaces(pName)) {
        return false;
    } 

    // Check if contains at least one letter (allow spaces & dash)
    bool hasLetter = false;
    for(int i = 0; i < pName.length(); i++) {
        if(isalpha(pName[i])) {
            hasLetter = true;
        }
        else if(pName[i] != ' '  && pName[i] != '-') {
            return false;
        }
    }
    return hasLetter;
}

bool Validation::isValidText(string text) {
    if(text.length() < 5) {
        return false;
    }
    if(!isNotOnlySpaces(text)) {
        return false;
    } 

    bool hasLetterOrDigit = false;
    for(int i = 0; i < text.length(); i++) {
        if(isalpha(text[i]) || isdigit(text[i])) {
            hasLetterOrDigit = true;
        }
        else if(text[i] != ' '  && text[i] != '-' && text[i] != ','  && text[i] != '.'  &&
                text[i] != '\'' && text[i] != '&'  && text[i] != '('  && text[i] != ')'  &&
                text[i] != '/'  && text[i] != ':') {
            return false;
        }
    }
    return hasLetterOrDigit;
}

bool Validation::isValidPhone(string contactNumber) {
    // Allow format: 042-12345678/0300-1234567
    if (contactNumber.length() != 12) {
        return false;
    }
    if (contactNumber[0] != '0') {
        return false;
    }
    
    int dashPos;
    if (contactNumber[3] == '-') {
        dashPos = 3;  
    }
    else if (contactNumber[4] == '-') {
        dashPos = 4;   
    }
    else {
        return false;  
    }
    
    string preDashStr = contactNumber.substr(0, dashPos);
    string postDashStr = contactNumber.substr(dashPos + 1);
    
    for(int i = 0; i < preDashStr.length(); i++) {
        if (!isdigit(preDashStr[i])) return false;
    }
    for(int i = 0; i < postDashStr.length(); i++) {
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

    // Check if there is atleast one '.' after @
    bool foundDot = false;
    for(int i = atPos+1; i < contactEmail.length(); i++) {
        if(contactEmail[i] == '.') {
            foundDot = true;
        }
    }
    if(!foundDot) {
        return false;
    }

    // check if '.' is not last character 
    if(contactEmail[contactEmail.length() - 1] == '.') {
        return false;
    } 
    // check if '.' is not immediately after @ 
    if(contactEmail[atPos + 1] == '.') {
        return false;
    }

    return true;
}

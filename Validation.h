#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
using namespace std;

class Validation {
public:
    static bool isValidDate(string date);
    static bool isValidTime(string time);
    static bool isValidName(string name); // for name
    static bool isValidPersonName(string pName);
    static bool isValidText(string text); // for address, description, budget feature
    static bool isValidEmail(string email);
    static bool isValidPhone(string phone);
};

#endif
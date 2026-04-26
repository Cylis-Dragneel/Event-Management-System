#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
using namespace std;

class Validation {
public:
    static bool isValidDate(string date);
    static bool isValidTime(string time);
    static bool isValidName(string name);
    static bool isValidAddress(string address);
    static bool isValidEmail(string email);
    static bool isValidPhone(string phone);
};

#endif
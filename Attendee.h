#ifndef ATTENDEE_H
#define ATTENDEE_H

#include<iostream>
using namespace std;

class Attendee {
protected:
int id;
string name;
string status;
string payment;

public:
Attendee ();
Attendee(int i, string n);

int getId();
string getname();
string getStatus();
string getPayment();


void setStatus(string s);
void setPayment(string p);

virtual void display();

};



#endif
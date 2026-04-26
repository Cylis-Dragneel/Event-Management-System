#include "Attendee.h"
#include<iostream>
using namespace std;

Attendee::Attendee(){
    id = 0;
    name = " ";
    status = "Pending";
    payment = "unpaid";
}

Attendee::Attendee(int id, string name)
{
    this->id = id;
    this->name = name;
    status = "pending";
    payment = "unpaid";

}

void Attendee::setStatus(string s)
{
    status = s;
}
 
void Attendee::setPayment(string p) {
    payment = p;
}

int Attendee:: getId()
{
    return id;
}
 
string Attendee:: getname()
{
    return name;
}
 
string Attendee:: getStatus()
{
    return status;
}

string Attendee:: getPayment()
{
    return payment;
}
 
void Attendee:: display()
{
    cout << "ID: " << id
         << " | Name: " << name
         << " | Status: " << status
         << " | Payment: " << payment << endl;
}


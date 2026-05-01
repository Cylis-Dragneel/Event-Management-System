#include "Registration.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Registration::Registration() {
    registrationId = -1;
    eventId = -1;
    attendeeId = -1;
    registrationStatus = 1;      // Pending
    paymentStatus = 1;           // Unpaid
    registrationDate = "";
    amountPaid = 0;
    totalAmount = 0;
    notes = "";
    
    waitlistCapacity = 10;
    waitlistSize = 0;
    waitlistQueue = new int[waitlistCapacity];
}

Registration::Registration(int registrationId, int eventId, int attendeeId, int registrationStatus,  int paymentStatus, string registrationDate, double amountPaid, 
double totalAmount, string notes) {
    
    setRegistrationId(registrationId);
    setEventId(eventId);
    setAttendeeId(attendeeId);
    setRegistrationStatus(registrationStatus);
    setPaymentStatus(paymentStatus);
    setRegistrationDate(registrationDate);
    setAmountPaid(amountPaid);
    setTotalAmount(totalAmount);
    setNotes(notes);
    
    waitlistCapacity = 10;
    waitlistSize = 0;
    waitlistQueue = new int[waitlistCapacity];
}

Registration::~Registration() {
    delete[] waitlistQueue;
}

void Registration::resizeWaitlist() {
    int newCapacity = waitlistCapacity * 2;
    int* newQueue = new int[newCapacity];
    
    for(int i = 0; i < waitlistSize; i++) {
        newQueue[i] = waitlistQueue[i];
    }
    
    delete[] waitlistQueue;
    waitlistQueue = newQueue;
    waitlistCapacity = newCapacity;
}

int Registration::findInWaitlist(int attendeeId) const {
    for(int i = 0; i < waitlistSize; i++) {
        if(waitlistQueue[i] == attendeeId) {
            return i;
        }
    }
    return -1;
}


int Registration::getWaitlistSize() const {
    return waitlistSize;
}

bool Registration::isOnWaitlist(int attendeeId) const {
    return (findInWaitlist(attendeeId) != -1);
}

int Registration::getWaitlistPosition(int attendeeId) const {
    int pos = findInWaitlist(attendeeId);
    if(pos == -1) return -1;
    return pos + 1;
}

void Registration::addToWaitlist(int attendeeId) {
    // attendee already on waitlist
    if(isOnWaitlist(attendeeId)) {
        throw invalid_argument("Attendee already on waitlist!");
    }
    
    // attendee ID invalid
    if(attendeeId <= 0) {
        throw invalid_argument("Invalid attendee ID!");
    }
    
    if(waitlistSize >= waitlistCapacity) {
        resizeWaitlist();
    }
    
    waitlistQueue[waitlistSize] = attendeeId;
    waitlistSize++;
}

void Registration::removeFromWaitlist(int attendeeId) {
    int index = findInWaitlist(attendeeId);
    
    //  attendee not found
    if(index == -1) {
        throw invalid_argument("Attendee not found in waitlist!");
    }
    
    for(int i = index; i < waitlistSize - 1; i++) {
        waitlistQueue[i] = waitlistQueue[i + 1];
    }
    waitlistSize--;
}

int Registration::getNextFromWaitlist() const {
    if(waitlistSize == 0) {
        return -1;
    }
    return waitlistQueue[0];
}

void Registration::promoteNextFromWaitlist() {
    //  waitlist empty
    if(waitlistSize == 0) {
        throw invalid_argument("Waitlist is empty!");
    }
    
    for(int i = 0; i < waitlistSize - 1; i++) {
        waitlistQueue[i] = waitlistQueue[i + 1];
    }
    waitlistSize--;
}

void Registration::printWaitlist() const {
    if(waitlistSize == 0) {
        cout << "Waitlist is empty" << endl;
        return;
    }
    
    cout << "Waitlist (" << waitlistSize << "): ";
    for(int i = 0; i < waitlistSize; i++) {
        cout << waitlistQueue[i];
        if(i < waitlistSize - 1) cout << " -> ";
    }
    cout << endl;
}


int Registration::getRegistrationId() const { 
    return registrationId;
}
int Registration::getEventId() const { 
    return eventId; 
}
int Registration::getAttendeeId() const {
    return attendeeId;
}
int Registration::getRegistrationStatus() const { 
   return registrationStatus; 
}
int Registration::getPaymentStatus() const {
     return paymentStatus;
}
string Registration::getRegistrationDate() const { 
    return registrationDate;
}
double Registration::getAmountPaid() const {
     return amountPaid; 
}
double Registration::getTotalAmount() const { 
    return totalAmount; 
}
string Registration::getNotes() const {
    return notes; 
}

void Registration::setRegistrationId(int id) {
    if(id > 0) {
        registrationId = id;
    }
    else {
        throw invalid_argument("Invalid registration ID! Must be positive.");
    }
}

void Registration::setEventId(int id) {
    if(id > 0) {
        eventId = id;
    }
    else {
        throw invalid_argument("Invalid event ID! Must be positive.");
    }
}

void Registration::setAttendeeId(int id) {
    if(id > 0) {
        attendeeId = id;
    }
    else {
        throw invalid_argument("Invalid attendee ID! Must be positive.");
    }
}

void Registration::setRegistrationStatus(int status) {
    //  status must be 1-4
    if(status >= 1 && status <= 4) {
        registrationStatus = status;
    }
    else {
        throw invalid_argument("Invalid status! 1=Pending, 2=Confirmed, 3=Cancelled, 4=Waitlisted");
    }
}

void Registration::setPaymentStatus(int status) {
    //  payment status must be 1-4
    if(status >= 1 && status <= 4) {
        paymentStatus = status;
    }
    else {
        throw invalid_argument("Invalid payment status! 1=Unpaid, 2=Paid, 3=Refunded, 4=Partial");
    }
}

void Registration::setRegistrationDate(string date) {
    // Check format 
    if(date.length() != 10 || date[2] != '-' || date[5] != '-') {
        throw invalid_argument("Invalid date format! Use DD-MM-YYYY");
    }
    
    if(!Validation::isValidDate(date)) {
        throw invalid_argument("Invalid date! Day/month/year values are incorrect");
    }
    
    registrationDate = date;
}

void Registration::setAmountPaid(double amount) {
    // amount cannot be negative
    if(amount < 0) {
        throw invalid_argument("Amount paid cannot be negative!");
    }
    
    //  amount cannot exceed total amount
    if(amount > totalAmount && totalAmount > 0) {
        throw invalid_argument("Amount paid cannot exceed total amount!");
    }
    
    amountPaid = amount;
    
    // Auto update payment status based on amount paid
    if(amountPaid >= totalAmount && totalAmount > 0) {
        paymentStatus = 2;  // Paid
    }
    else if(amountPaid > 0 && amountPaid < totalAmount) {
        paymentStatus = 4;  // Partial
    }
    else if(amountPaid == 0) {
        paymentStatus = 1;  // Unpaid
    }
}

void Registration::setTotalAmount(double amount) {
    // Edge case: amount cannot be negative
    if(amount < 0) {
        throw invalid_argument("Total amount cannot be negative!");
    }
    
    // Edge case: new total cannot be less than already paid
    if(amountPaid > amount) {
        throw invalid_argument("New total cannot be less than amount already paid!");
    }
    
    totalAmount = amount;
    
    // Update payment status if needed
    if(amountPaid >= totalAmount && totalAmount > 0) {
        paymentStatus = 2;  // Paid
    }
}

void Registration::setNotes(string notes) {
    this->notes = notes;
}


double Registration::getRemainingBalance() const {
    return totalAmount - amountPaid;
}

bool Registration::isFullyPaid() const {
    return (amountPaid >= totalAmount && totalAmount > 0);
}

string Registration::getRegistrationStatusText() const {
    switch(registrationStatus) {
        case 1: return "Pending";
        case 2: return "Confirmed";
        case 3: return "Cancelled";
        case 4: return "Waitlisted";
        default: return "Unknown";
    }
}

string Registration::getPaymentStatusText() const {
    switch(paymentStatus) {
        case 1: return "Unpaid";
        case 2: return "Paid";
        case 3: return "Refunded";
        case 4: return "Partial";
        default: return "Unknown";
    }
}


bool Registration::makePayment(double amount) {
    //  amount must be positive
    if(amount <= 0) {
        throw invalid_argument("Payment amount must be greater than zero!");
    }
    
    //  already fully paid
    if(isFullyPaid()) {
        throw invalid_argument("Already fully paid!");
    }
    
    //  payment would exceed total amount
    if(amountPaid + amount > totalAmount) {
        throw invalid_argument("Payment exceeds remaining balance!");
    }
    
    //  cancelled registration 
    if(registrationStatus == 3) {
        throw invalid_argument("Cannot pay - registration is cancelled!");
    }
    
    amountPaid += amount;
    
    // Update payment status
    if(amountPaid >= totalAmount) {
        paymentStatus = 2;
        cout << "Payment complete! Fully paid." << endl;
    }
    else if(amountPaid > 0) {
        paymentStatus = 4;
        cout << "Payment received. Remaining: " << getRemainingBalance() << endl;
    }
    
    return true;
}

bool Registration::refundPayment(double amount) {
    //  amount must be positive
    if(amount <= 0) {
        throw invalid_argument("Refund amount must be greater than zero!");
    }
    
    //  no payment to refund
    if(amountPaid <= 0) {
        throw invalid_argument("No payment to refund!");
    }
    
    //  refund exceeds amount paid
    if(amount > amountPaid) {
        throw invalid_argument("Refund exceeds amount paid!");
    }
    
    amountPaid -= amount;
    
    // Update payment status
    if(amountPaid <= 0) {
        amountPaid = 0;
        paymentStatus = 1;
        cout << "Full refund processed." << endl;
    }
    else if(amountPaid < totalAmount) {
        paymentStatus = 4;
        cout << "Partial refund processed." << endl;
    }
    
    return true;
}

void Registration::confirmRegistration() {
    // already confirmed
    if(registrationStatus == 2) {
        throw invalid_argument("Registration already confirmed!");
    }
    
    // cancelled registration cannot be confirmed
    if(registrationStatus == 3) {
        throw invalid_argument("Cannot confirm cancelled registration!");
    }
    
    // Can confirm Pending(1) or Waitlisted(4)
    if(registrationStatus == 1 || registrationStatus == 4) {
        registrationStatus = 2;
        cout << "Registration confirmed!" << endl;
    }
}

void Registration::cancelRegistration() {
    //  already cancelled
    if(registrationStatus == 3) {
        throw invalid_argument("Registration already cancelled!");
    }
    
    // Can cancel Pending(1), Confirmed(2), or Waitlisted(4)
    if(registrationStatus == 1 || registrationStatus == 2 || registrationStatus == 4) {
        registrationStatus = 3;
        cout << "Registration cancelled!" << endl;
        
        if(amountPaid > 0) {
            cout << "Note: Refund recommended of Rs " << amountPaid << endl;
        }
    }
}

void Registration::moveToWaitlist() {
    //  already on waitlist
    if(registrationStatus == 4) {
        throw invalid_argument("Already on waitlist!");
    }
    
    // Only Pending(1) can move to waitlist
    if(registrationStatus == 1) {
        registrationStatus = 4;
        cout << "Moved to waitlist!" << endl;
    }
    else if(registrationStatus == 2) {
        throw invalid_argument("Cannot move confirmed registration to waitlist!");
    }
    else if(registrationStatus == 3) {
        throw invalid_argument("Cannot move cancelled registration to waitlist!");
    }
}

void Registration::moveFromWaitlistToConfirmed() {
    //  not on waitlist
    if(registrationStatus != 4) {
        throw invalid_argument("Registration is not on waitlist!");
    }
    
    registrationStatus = 2;
    cout << "Moved from waitlist to confirmed!" << endl;
}
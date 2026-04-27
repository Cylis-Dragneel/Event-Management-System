#include "Registration.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Registration::Registration() {
    registrationId = -1;        
    attendeeId = -1;           
    registrationStatus = 1;     // 1 = Pending
    paymentStatus = 1;          // 1 = Unpaid
    registrationDate = "";      // Empty date
    amountPaid = 0;
    totalAmount = 0;
    notes = "";
    
    waitlistCapacity = 10;
    waitlistSize = 0;
    waitlistQueue = new int[waitlistCapacity];
}

Registration::Registration(int registrationId, int attendeeId, int registrationStatus, int paymentStatus,
                           string registrationDate, double amountPaid, double totalAmount, string notes) {
    

    this->registrationId = registrationId;
    this->attendeeId = attendeeId;
    this->registrationStatus = registrationStatus;
    this->paymentStatus = paymentStatus;
    this->registrationDate = registrationDate;
    this->amountPaid = amountPaid;
    this->totalAmount = totalAmount;
    this->notes = notes;
    
    waitlistCapacity = 10;
    waitlistSize = 0;
    waitlistQueue = new int[waitlistCapacity];
}

Registration::~Registration() {
    delete[] waitlistQueue;
}

// Doubles the waitlist capacity when it becomes full
void Registration::resizeWaitlist() {
    int newCapacity = waitlistCapacity * 2;
    int* newQueue = new int[newCapacity];
    
    // Copy old data to new array
    for(int i = 0; i < waitlistSize; i++) {
        newQueue[i] = waitlistQueue[i];
    }
    
    // Delete old array 
    delete[] waitlistQueue;
    waitlistQueue = newQueue;
    waitlistCapacity = newCapacity;
}

// Finds an attendee in waitlist, returns index or -1 if not found
int Registration::findInWaitlist(int attendeeId) {
    for(int i = 0; i < waitlistSize; i++) {
        if(waitlistQueue[i] == attendeeId) {
            return i;  // Found
        }
    }
    return -1;  // Not found
}

// Returns current waitlist size
int Registration::getWaitlistSize() {
    return waitlistSize;
}

// Checks if an attendee is on waitlist
bool Registration::isOnWaitlist(int attendeeId) {
    return (findInWaitlist(attendeeId) != -1);
}

// Returns position of attendee in waitlist 
int Registration::getWaitlistPosition(int attendeeId) {
    int pos = findInWaitlist(attendeeId);
    if(pos == -1) return -1;
    return pos + 1;  
}

// Adds an attendee to the end of waitlist
void Registration::addToWaitlist(int attendeeId) {
    if(isOnWaitlist(attendeeId)) {
        throw invalid_argument("Attendee already on waitlist!");
    }
    
    // Resize if full
    if(waitlistSize >= waitlistCapacity) {
        resizeWaitlist();
    }
    
    // Add at the end
    waitlistQueue[waitlistSize] = attendeeId;
    waitlistSize++;
}

// Removes an attendee from waitlist
void Registration::removeFromWaitlist(int attendeeId) {
    int index = findInWaitlist(attendeeId);
    if(index == -1) {
        throw invalid_argument("Attendee not found in waitlist!");
    }
    
    // Shift all elements left to fill the gap
    for(int i = index; i < waitlistSize - 1; i++) {
        waitlistQueue[i] = waitlistQueue[i + 1];
    }
    waitlistSize--;
}

// Returns the next attendee from waitlist without removing
int Registration::getNextFromWaitlist() {
    if(waitlistSize == 0) {
        return -1;
    }
    return waitlistQueue[0];
}

// Removes and promotes the next attendee from waitlist
void Registration::promoteNextFromWaitlist() {
    if(waitlistSize == 0) {
        throw invalid_argument("Waitlist is empty!");
    }
    
    // Shift all elements left (remove first)
    for(int i = 0; i < waitlistSize - 1; i++) {
        waitlistQueue[i] = waitlistQueue[i + 1];
    }
    waitlistSize--;
}

// Prints the entire waitlist 
void Registration::printWaitlist() {
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


int Registration::getRegistrationId() { 
    return registrationId;
 }
int Registration::getAttendeeId() {
     return attendeeId; 
}
int Registration::getRegistrationStatus() { 
    return registrationStatus; 
}
int Registration::getPaymentStatus() { 
    return paymentStatus; 

}
string Registration::getRegistrationDate() { 
    return registrationDate;
}
double Registration::getAmountPaid() {
    
    return amountPaid;
 }
double Registration::getTotalAmount() {
     return totalAmount;
 }
string Registration::getNotes() { 
    return notes;
 }


void Registration::setRegistrationId(int id) {
    if(id > 0) {
        registrationId = id;
    }
    else {
        throw invalid_argument("Invalid registration ID! ID must be positive");
    }
}

void Registration::setAttendeeId(int id) {
    if(id > 0) {
        attendeeId = id;
    }
    else {
        throw invalid_argument("Invalid attendee ID! ID must be positive");
    }
}

// Sets registration status - must be between 1 and 4
void Registration::setRegistrationStatus(int status) {
    if(status >= 1 && status <= 4) {
        registrationStatus = status;
    }
    else {
        throw invalid_argument("Invalid status! 1=Pending, 2=Confirmed, 3=Cancelled, 4=Waitlisted");
    }
}

// Sets payment status - must be between 1 and 4
void Registration::setPaymentStatus(int status) {
    if(status >= 1 && status <= 4) {
        paymentStatus = status;
    }
    else {
        throw invalid_argument("Invalid payment status! 1=Unpaid, 2=Paid, 3=Refunded, 4=Partial");
    }
}

void Registration::setRegistrationDate(string date) {
    if(Validation::isValidDate(date)) {
        registrationDate = date;
    }
    else {
        throw invalid_argument("Invalid date! Use DD-MM-YYYY format");
    }
}

void Registration::setAmountPaid(double amount) {
    if(amount < 0) {
        throw invalid_argument("Amount paid cannot be negative!");
    }
    
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
    if(amount < 0) {
        throw invalid_argument("Total amount cannot be negative!");
    }
    
    if(amountPaid > amount) {
        throw invalid_argument("New total cannot be less than amount already paid!");
    }
    
    totalAmount = amount;
    

    if(amountPaid >= totalAmount && totalAmount > 0) {
        paymentStatus = 2;  // Paid
    }
}

// Sets notes 
void Registration::setNotes(string notes) {
    this->notes = notes;
}

double Registration::getRemainingBalance() {
    return totalAmount - amountPaid;
}

bool Registration::isFullyPaid() {
    return (amountPaid >= totalAmount && totalAmount > 0);
}

// Converts registration status number to readable text
string Registration::getRegistrationStatusText() {
    switch(registrationStatus) {
        case 1: return "Pending";
        case 2: return "Confirmed";
        case 3: return "Cancelled";
        case 4: return "Waitlisted";
        default: return "Unknown";
    }
}

// Converts payment status number to readable text
string Registration::getPaymentStatusText() {
    switch(paymentStatus) {
        case 1: return "Unpaid";
        case 2: return "Paid";
        case 3: return "Refunded";
        case 4: return "Partial";
        default: return "Unknown";
    }
}

bool Registration::makePayment(double amount) {
    if(amount <= 0) {
        throw invalid_argument("Payment amount must be greater than zero!");
    }
    
    if(isFullyPaid()) {
        throw invalid_argument("Already fully paid!");
    }
    
    if(amountPaid + amount > totalAmount) {
        throw invalid_argument("Payment exceeds remaining balance!");
    }
    
    if(registrationStatus == 3) {
        throw invalid_argument("Cannot pay - registration is cancelled!");
    }
    
    // Process payment
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

// Process a refund
bool Registration::refundPayment(double amount) {
    // Validate refund amount
    if(amount <= 0) {
        throw invalid_argument("Refund amount must be greater than zero!");
    }
    
    // Check if any payment was made
    if(amountPaid <= 0) {
        throw invalid_argument("No payment to refund!");
    }
    
    // Check if refund exceeds paid amount
    if(amount > amountPaid) {
        throw invalid_argument("Refund exceeds amount paid!");
    }
    
    // Process refund
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
    if(registrationStatus == 2) {
        throw invalid_argument("Registration already confirmed!");
    }
    
    if(registrationStatus == 3) {
        throw invalid_argument("Cannot confirm cancelled registration!");
    }

    if(registrationStatus == 1 || registrationStatus == 4) {
        registrationStatus = 2;
        cout << "Registration confirmed!" << endl;
    }
}

void Registration::cancelRegistration() {
    if(registrationStatus == 3) {
        throw invalid_argument("Registration already cancelled!");
    }

    if(registrationStatus == 1 || registrationStatus == 2 || registrationStatus == 4) {
        registrationStatus = 3;
        cout << "Registration cancelled!" << endl;
        
        if(amountPaid > 0) {
            cout << "Note: Refund recommended of Rs " << amountPaid << endl;
        }
    }
}

void Registration::moveToWaitlist() {
    if(registrationStatus == 4) {
        throw invalid_argument("Already on waitlist!");
    }

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

// Moves registration from waitlist to confirmed (when seat available)
void Registration::moveFromWaitlistToConfirmed() {
    if(registrationStatus != 4) {
        throw invalid_argument("Registration is not on waitlist!");
    }
    
    registrationStatus = 2;
    cout << "Moved from waitlist to confirmed!" << endl;
}
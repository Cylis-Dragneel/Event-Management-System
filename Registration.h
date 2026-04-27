#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>
#include "Validation.h"

using namespace std;

class Registration {
private:

    int registrationId;     
    int attendeeId;         
    int registrationStatus;  // 1=Pending, 2=Confirmed, 3=Cancelled, 4=Waitlisted
    int paymentStatus;       // 1=Unpaid, 2=Paid, 3=Refunded, 4=Partial
    string registrationDate; // Date of registration (DD-MM-YYYY)
    double amountPaid;       // How much money paid so far
    double totalAmount;      // Total amount to be paid
    string notes;            // Extra information
    
    int* waitlistQueue;      // Dynamic array for waitlist (FIFO queue)
    int waitlistSize;        // Current number of people in waitlist
    int waitlistCapacity;    // Maximum capacity of waitlist array

public:
    Registration(); 
    Registration(int registrationId, int attendeeId, int registrationStatus, int paymentStatus,
                 string registrationDate, double amountPaid, double totalAmount, string notes);  
    
    ~Registration();
    

    int getRegistrationId();
    int getAttendeeId();
    int getRegistrationStatus();
    int getPaymentStatus();
    string getRegistrationDate();
    double getAmountPaid();
    double getTotalAmount();
    string getNotes();
    
    // Waitlist getters
    int getWaitlistSize();
    int getWaitlistPosition(int attendeeId);
    bool isOnWaitlist(int attendeeId);
    
    // Setters with validation
    void setRegistrationId(int id);
    void setAttendeeId(int id);
    void setRegistrationStatus(int status);
    void setPaymentStatus(int status);
    void setRegistrationDate(string date);
    void setAmountPaid(double amount);
    void setTotalAmount(double amount);
    void setNotes(string notes);
    
    //calculations
    double getRemainingBalance();
    bool isFullyPaid();
    string getRegistrationStatusText();
    string getPaymentStatusText();

    bool makePayment(double amount);
    bool refundPayment(double amount);
    
    // Status change methods
    void confirmRegistration();
    void cancelRegistration();
    void moveToWaitlist();
    void moveFromWaitlistToConfirmed();
    
    // Waitlist management 
    void addToWaitlist(int attendeeId);
    void removeFromWaitlist(int attendeeId);
    int getNextFromWaitlist();
    void promoteNextFromWaitlist();
    void printWaitlist();

private:
    
    void resizeWaitlist();        // Increase waitlist capacity when full
    int findInWaitlist(int attendeeId);  // Find attendee in waitlist
};

#endif
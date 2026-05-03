#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>
using namespace std;

class Registration {
private:
    int registrationId;
    int eventId;
    int attendeeId;
    int registrationStatus;  // 1=Pending, 2=Confirmed, 3=Cancelled, 4=Waitlisted
    int paymentStatus;       // 1=Unpaid, 2=Paid, 3=Refunded, 4=Partial
    string registrationDate;
    double amountPaid;
    double totalAmount;
    string notes;
    string attendeeName;
    string attendeeEmail;
    
    // Waitlist data
    int* waitlistQueue;
    int waitlistSize;
    int waitlistCapacity;

public:
    // Constructors
    Registration();
    Registration(int registrationId, int eventId, int attendeeId, int registrationStatus,
                 int paymentStatus, string registrationDate, double amountPaid,
                 double totalAmount, string notes, string attendeeName, string attendeeEmail);
    Registration(const Registration& other);
    Registration& operator=(const Registration& other);
    // Destructor
    ~Registration();
    
    int getRegistrationId() const;
    int getEventId() const;
    int getAttendeeId() const;
    int getRegistrationStatus() const;
    int getPaymentStatus() const;
    string getRegistrationDate() const;
    double getAmountPaid() const;
    double getTotalAmount() const;
    string getNotes() const;
    string getAttendeeName() const;
    string getAttendeeEmail() const;
    
    // Waitlist getters
    int getWaitlistSize() const;
    int getWaitlistPosition(int attendeeId) const;
    bool isOnWaitlist(int attendeeId) const;
    
    // Setters with validation
    void setRegistrationId(int id);
    void setEventId(int id);
    void setAttendeeId(int id);
    void setRegistrationStatus(int status);
    void setPaymentStatus(int status);
    void setRegistrationDate(string date);
    void setAmountPaid(double amount);
    void setTotalAmount(double amount);
    void setNotes(string notes);
    void setAttendeeName(string name);
    void setAttendeeEmail(string email);
    
    // Business methods
    double getRemainingBalance() const;
    bool isFullyPaid() const;
    string getRegistrationStatusText() const;
    string getPaymentStatusText() const;
    
    // Payment methods
    bool makePayment(double amount);
    bool refundPayment(double amount);
    
    // Status methods
    void confirmRegistration();
    void cancelRegistration();
    void moveToWaitlist();
    void moveFromWaitlistToConfirmed();
    
    // Waitlist methods
    void addToWaitlist(int attendeeId);
    void removeFromWaitlist(int attendeeId);
    int getNextFromWaitlist() const;
    void promoteNextFromWaitlist();
   string getWaitlistString() const;

private:
    void resizeWaitlist();
    int findInWaitlist(int attendeeId) const;
};

#endif
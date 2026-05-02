#include <QCoreApplication>
#include <QDebug>
#include "database.h"
#include "Attendee.h"
#include "venue.h"
#include "event.h"
#include "Registration.h"
#include "BudgetItem.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    qDebug() << "--- Database Testing Started ---";

    Database myDb;

    // Test 1: Database Connection check karna
    if (myDb.connectDatabase()) {
        qDebug() << "Test 1 Passed: Database Connected!";

        // Test 2: Tables create karna
        if (myDb.createTables()) {
            qDebug() << "Test 2 Passed: All Tables Created Successfully!";

            // ==========================================
            // Test 3: Insert Attendee Data
            // ==========================================
            Attendee newAtt("Ali", "Khan", "ali@gmail.com", "0300-1234567", "Lahore, Pakistan");
            if (myDb.addAttendee(newAtt)) {
                qDebug() << "Test 3 Passed: Attendee Added to Database!";
            }

            // ==========================================
            // Test 4: Read Attendee Data
            // ==========================================
            Attendee savedAtt = myDb.getAttendee(1);
            if (savedAtt.getFirstName() == "Ali") {
                qDebug() << "Test 4 Passed: Attendee read from Database successfully! Name:"
                    << QString::fromStdString(savedAtt.getFullName());
            }

        }
        else {
            qDebug() << "Test 2 Failed: Tables not created.";
        }
    }
    else {
        qDebug() << "Test 1 Failed: Could not connect to database.";
    }
    // Test 5: Insert Venue Data
    Venue newVenue("Pearl Continental", "Mall Road, Lahore", 500, "0000-1234567", "info@pc.com", true, true, true, false);
    if (myDb.addVenue(newVenue)) {
        qDebug() << "Test 5 Passed: Venue Added to Database!";
    }

    // Test 6: Insert Event Data
    Event newEvent("Tech Conference", "Annual IT meetup", "25-12-2026", "10:00", 120, 200, 0);
    newEvent.setVenueId(1); // Event ko Venue ke sath link karna
    if (myDb.addEvent(newEvent)) {
        qDebug() << "Test 6 Passed: Event Added to Database!";
    }

    // Test 7: Insert Registration
    Registration newReg(1, 1, 1, 2, 2, "20-04-2026", 1500.0, 1500.0, "VIP Ticket");
    if (myDb.addRegistration(newReg)) {
        qDebug() << "Test 7 Passed: Registration Added to Database!";
    }

    // Test 8: Insert Budget Item
    BudgetItem newItem(1, "expense", "catering", 50000.0, "15-04-2026", "paid");
    if (myDb.addBudgetItem(newItem)) {
        qDebug() << "Test 8 Passed: BudgetItem Added to Database!";
    }

    qDebug() << "--- Testing Finished ---";
    return 0;
}
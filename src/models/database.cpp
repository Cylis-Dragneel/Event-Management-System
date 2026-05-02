#include "database.h"
#include <iostream>
using namespace std;
Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("EventManagementDB.sqlite");
}
Database::~Database() {
    closeDatabase();
}
bool Database::connectDatabase() {
    if (!db.open()) {
        qDebug() << "Error: Connection with database failed!";
        qDebug() << db.lastError().text();
        return false;
    }
    return true;
}
void Database::closeDatabase() {
    if (db.isOpen()) {
        db.close();
    }
}
bool Database::createTables() {
    QSqlQuery query;
    bool allSuccess = true;

    // 1. Attendees Table
    QString createAttendees = "CREATE TABLE IF NOT EXISTS Attendees ("
        "AttendeeID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "FirstName TEXT NOT NULL, "
        "LastName TEXT NOT NULL, "
        "Email TEXT NOT NULL, "
        "Phone TEXT, "
        "Address TEXT)";
    if (!query.exec(createAttendees)) {
        qDebug() << "Failed to create Attendees table:" << query.lastError().text();
        allSuccess = false;
    }
    // 2. Venues Table
    QString createVenues = "CREATE TABLE IF NOT EXISTS Venues ("
        "VenueID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Name TEXT NOT NULL, "
        "Address TEXT, "
        "Capacity INTEGER, "
        "ContactNumber TEXT, "
        "ContactEmail TEXT, "
        "HasWifi INTEGER, "
        "HasParking INTEGER, "
        "HasCatering INTEGER, "
        "HasAVEquipment INTEGER)";
    if (!query.exec(createVenues)) {
        qDebug() << "Failed to create Venues table:" << query.lastError().text();
        allSuccess = false;
    }
    // 3. Events Table
    QString createEvents = "CREATE TABLE IF NOT EXISTS Events ("
        "EventID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Name TEXT NOT NULL, "
        "Description TEXT, "
        "Date TEXT, "
        "Time TEXT, "
        "Duration INTEGER, "
        "Capacity INTEGER, "
        "Status INTEGER, "
        "Type INTEGER, "
        "VenueID INTEGER)";
    if (!query.exec(createEvents)) {
        qDebug() << "Failed to create Events table:" << query.lastError().text();
        allSuccess = false;
    }
    // 4. Registrations Table
    QString createRegistrations = "CREATE TABLE IF NOT EXISTS Registrations ("
        "RegistrationID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "EventID INTEGER, "
        "AttendeeID INTEGER, "
        "RegistrationStatus INTEGER, "
        "PaymentStatus INTEGER, "
        "RegistrationDate TEXT, "
        "AmountPaid REAL, "
        "TotalAmount REAL, "
        "Notes TEXT)";
    if (!query.exec(createRegistrations)) {
        qDebug() << "Failed to create Registrations table:" << query.lastError().text();
        allSuccess = false;
    }
    // 5. BudgetItems Table
    QString createBudgetItems = "CREATE TABLE IF NOT EXISTS BudgetItems ("
        "ItemID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "EventID INTEGER, "
        "Type TEXT, "
        "Category TEXT, "
        "Amount REAL, "
        "Date TEXT, "
        "Status TEXT)";
    if (!query.exec(createBudgetItems)) {
        qDebug() << "Failed to create BudgetItems table:" << query.lastError().text();
        allSuccess = false;
    }
    return allSuccess;
}
// ================= ATTENDEE OPERATIONS =================
bool Database::addAttendee(const Attendee& attendee) {
    QSqlQuery query;
    query.prepare("INSERT INTO Attendees (FirstName, LastName, Email, Phone, Address) "
        "VALUES (:firstName, :lastName, :email, :phone, :address)");
    query.bindValue(":firstName", QString::fromStdString(attendee.getFirstName()));
    query.bindValue(":lastName", QString::fromStdString(attendee.getLastName()));
    query.bindValue(":email", QString::fromStdString(attendee.getEmail()));
    query.bindValue(":phone", QString::fromStdString(attendee.getPhone()));
    query.bindValue(":address", QString::fromStdString(attendee.getAddress()));

    if (!query.exec()) {
        qDebug() << "Error adding attendee:" << query.lastError().text();
        return false;
    }
    return true;
}
Attendee Database::getAttendee(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Attendees WHERE AttendeeID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        int attendeeId = query.value("AttendeeID").toInt();
        string fName = query.value("FirstName").toString().toStdString();
        string lName = query.value("LastName").toString().toStdString();
        string email = query.value("Email").toString().toStdString();
        string phone = query.value("Phone").toString().toStdString();
        string address = query.value("Address").toString().toStdString();

        return Attendee(attendeeId, fName, lName, email, phone, address);
    }
    qDebug() << "Attendee not found in Database!";
    return Attendee();
}
// ================= VENUE OPERATIONS =================
bool Database::addVenue(const Venue& venue) {
    QSqlQuery query;
    query.prepare("INSERT INTO Venues (Name, Address, Capacity, ContactNumber, ContactEmail, "
        "HasWifi, HasParking, HasCatering, HasAVEquipment) "
        "VALUES (:name, :address, :capacity, :contactNumber, :contactEmail, "
        ":hasWifi, :hasParking, :hasCatering, :hasAVEquipment)");
    query.bindValue(":name", QString::fromStdString(venue.getName()));
    query.bindValue(":address", QString::fromStdString(venue.getAddress()));
    query.bindValue(":capacity", venue.getCapacity());
    query.bindValue(":contactNumber", QString::fromStdString(venue.getContactNumber()));
    query.bindValue(":contactEmail", QString::fromStdString(venue.getContactEmail()));
    query.bindValue(":hasWifi", venue.getHasWifi() ? 1 : 0);
    query.bindValue(":hasParking", venue.getHasParking() ? 1 : 0);
    query.bindValue(":hasCatering", venue.getHasCatering() ? 1 : 0);
    query.bindValue(":hasAVEquipment", venue.getHasAVEquipment() ? 1 : 0);

    if (!query.exec()) {
        qDebug() << "Error adding venue:" << query.lastError().text();
        return false;
    }
    return true;
}
Venue Database::getVenue(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Venues WHERE VenueID = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        string name = query.value("Name").toString().toStdString();
        string address = query.value("Address").toString().toStdString();
        int capacity = query.value("Capacity").toInt();
        string contactNumber = query.value("ContactNumber").toString().toStdString();
        string contactEmail = query.value("ContactEmail").toString().toStdString();
        bool hasWifi = query.value("HasWifi").toInt() == 1;
        bool hasParking = query.value("HasParking").toInt() == 1;
        bool hasCatering = query.value("HasCatering").toInt() == 1;
        bool hasAVEquipment = query.value("HasAVEquipment").toInt() == 1;

        return Venue(name, address, capacity, contactNumber, contactEmail,
            hasWifi, hasParking, hasCatering, hasAVEquipment);
    }
    return Venue();
}
// ================= EVENT OPERATIONS =================
bool Database::addEvent(const Event& event) {
    QSqlQuery query;
    query.prepare("INSERT INTO Events (Name, Description, Date, Time, Duration, Capacity, Status, Type, VenueID) "
        "VALUES (:name, :description, :date, :time, :duration, :capacity, :status, :type, :venueId)");
    query.bindValue(":name", QString::fromStdString(event.getName()));
    query.bindValue(":description", QString::fromStdString(event.getDescription()));
    query.bindValue(":date", QString::fromStdString(event.getDate()));
    query.bindValue(":time", QString::fromStdString(event.getTime()));
    query.bindValue(":duration", event.getDuration());
    query.bindValue(":capacity", event.getCapacity());
    query.bindValue(":status", event.getStatus());
    query.bindValue(":type", event.getType());
    query.bindValue(":venueId", event.getVenueId());

    if (!query.exec()) {
        qDebug() << "Error adding event:" << query.lastError().text();
        return false;
    }
    return true;
}
Event Database::getEvent(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Events WHERE EventID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error getting event:" << query.lastError().text();
        return Event();
    }

    if (query.next()) {
        string name = query.value("Name").toString().toStdString();
        string description = query.value("Description").toString().toStdString();
        string date = query.value("Date").toString().toStdString();
        string time = query.value("Time").toString().toStdString();
        int duration = query.value("Duration").toInt();
        int capacity = query.value("Capacity").toInt();
        int type = query.value("Type").toInt();
        int status = query.value("Status").toInt();

        Event e(name, description, date, time, duration, capacity, type);
        e.setVenueId(query.value("VenueID").toInt());
        e.changeStatus(status);
        return e;
    }
    return Event();
}
// ================= REGISTRATION OPERATIONS =================
bool Database::addRegistration(const Registration& reg) {
    QSqlQuery query;
    query.prepare("INSERT INTO Registrations (EventID, AttendeeID, RegistrationStatus, PaymentStatus, "
        "RegistrationDate, AmountPaid, TotalAmount, Notes) "
        "VALUES (:eventId, :attendeeId, :regStatus, :payStatus, :date, :amountPaid, :totalAmount, :notes)");
    query.bindValue(":eventId", reg.getEventId());
    query.bindValue(":attendeeId", reg.getAttendeeId());
    query.bindValue(":regStatus", reg.getRegistrationStatus());
    query.bindValue(":payStatus", reg.getPaymentStatus());
    query.bindValue(":date", QString::fromStdString(reg.getRegistrationDate()));
    query.bindValue(":amountPaid", reg.getAmountPaid());
    query.bindValue(":totalAmount", reg.getTotalAmount());
    query.bindValue(":notes", QString::fromStdString(reg.getNotes()));
    if (!query.exec()) {
        qDebug() << "Error adding registration:" << query.lastError().text();
        return false;
    }
    return true;
}
Registration Database::getRegistration(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Registrations WHERE RegistrationID = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        int eventId = query.value("EventID").toInt();
        int attendeeId = query.value("AttendeeID").toInt();
        int regStatus = query.value("RegistrationStatus").toInt();
        int payStatus = query.value("PaymentStatus").toInt();
        string date = query.value("RegistrationDate").toString().toStdString();
        double amountPaid = query.value("AmountPaid").toDouble();
        double totalAmount = query.value("TotalAmount").toDouble();
        string notes = query.value("Notes").toString().toStdString();
        return Registration(id, eventId, attendeeId, regStatus, payStatus, date, amountPaid, totalAmount, notes);
    }
    return Registration();
}
// ================= BUDGET ITEM OPERATIONS =================
bool Database::addBudgetItem(const BudgetItem& item) {
    QSqlQuery query;
    query.prepare("INSERT INTO BudgetItems (EventID, Type, Category, Amount, Date, Status) "
        "VALUES (:eventId, :type, :category, :amount, :date, :status)");
    query.bindValue(":eventId", item.getEventId());
    query.bindValue(":type", QString::fromStdString(item.getType()));
    query.bindValue(":category", QString::fromStdString(item.getCategory()));
    query.bindValue(":amount", item.getAmount());
    query.bindValue(":date", QString::fromStdString(item.getDate()));
    query.bindValue(":status", QString::fromStdString(item.getStatus()));
    if (!query.exec()) {
        qDebug() << "Error adding budget item:" << query.lastError().text();
        return false;
    }
    return true;
}
BudgetItem Database::getBudgetItem(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM BudgetItems WHERE ItemID = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        int eventId = query.value("EventID").toInt();
        string type = query.value("Type").toString().toStdString();
        string category = query.value("Category").toString().toStdString();
        double amount = query.value("Amount").toDouble();
        string date = query.value("Date").toString().toStdString();
        string status = query.value("Status").toString().toStdString();

        return BudgetItem(eventId, type, category, amount, date, status);
    }
    return BudgetItem();
}
// ================= UPDATE & DELETE OPERATIONS =================
// Attendee
bool Database::updateAttendee(const Attendee& attendee) {
    QSqlQuery query;
    query.prepare("UPDATE Attendees SET FirstName=:f, LastName=:l, Email=:e, Phone=:p, Address=:a WHERE AttendeeID=:id");
    query.bindValue(":f", QString::fromStdString(attendee.getFirstName()));
    query.bindValue(":l", QString::fromStdString(attendee.getLastName()));
    query.bindValue(":e", QString::fromStdString(attendee.getEmail()));
    query.bindValue(":p", QString::fromStdString(attendee.getPhone()));
    query.bindValue(":a", QString::fromStdString(attendee.getAddress()));
    query.bindValue(":id", attendee.getAttendeeId());
    return query.exec();
}
bool Database::deleteAttendee(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM Attendees WHERE AttendeeID=:id");
    query.bindValue(":id", id);
    return query.exec();
}
// Venue
bool Database::updateVenue(int id, const Venue& venue) {
    QSqlQuery query;
    query.prepare("UPDATE Venues SET Name=:n, Address=:a, Capacity=:c, ContactNumber=:cn, ContactEmail=:ce, "
                  "HasWifi=:w, HasParking=:p, HasCatering=:cat, HasAVEquipment=:av WHERE VenueID=:id");
    query.bindValue(":n", QString::fromStdString(venue.getName()));
    query.bindValue(":a", QString::fromStdString(venue.getAddress()));
    query.bindValue(":c", venue.getCapacity());
    query.bindValue(":cn", QString::fromStdString(venue.getContactNumber()));
    query.bindValue(":ce", QString::fromStdString(venue.getContactEmail()));
    query.bindValue(":w", venue.getHasWifi() ? 1 : 0);
    query.bindValue(":p", venue.getHasParking() ? 1 : 0);
    query.bindValue(":cat", venue.getHasCatering() ? 1 : 0);
    query.bindValue(":av", venue.getHasAVEquipment() ? 1 : 0);
    query.bindValue(":id", id);
    return query.exec();
}
bool Database::deleteVenue(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM Venues WHERE VenueID=:id");
    query.bindValue(":id", id);
    return query.exec();
}
// Event
bool Database::updateEvent(int id, const Event& event) {
    QSqlQuery query;
    query.prepare("UPDATE Events SET Name=:n, Description=:d, Date=:date, Time=:t, Duration=:dur, "
                  "Capacity=:c, Status=:s, Type=:ty, VenueID=:vid WHERE EventID=:id");
    query.bindValue(":n", QString::fromStdString(event.getName()));
    query.bindValue(":d", QString::fromStdString(event.getDescription()));
    query.bindValue(":date", QString::fromStdString(event.getDate()));
    query.bindValue(":t", QString::fromStdString(event.getTime()));
    query.bindValue(":dur", event.getDuration());
    query.bindValue(":c", event.getCapacity());
    query.bindValue(":s", event.getStatus());
    query.bindValue(":ty", event.getType());
    query.bindValue(":vid", event.getVenueId());
    query.bindValue(":id", id);
    return query.exec();
}
bool Database::deleteEvent(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM Events WHERE EventID=:id");
    query.bindValue(":id", id);
    return query.exec();
}
// Registration
bool Database::updateRegistration(int id, const Registration& reg) {
    QSqlQuery query;
    query.prepare("UPDATE Registrations SET EventID=:eid, AttendeeID=:aid, RegistrationStatus=:rs, "
                  "PaymentStatus=:ps, RegistrationDate=:date, AmountPaid=:ap, TotalAmount=:ta, Notes=:n "
                  "WHERE RegistrationID=:id");
    query.bindValue(":eid", reg.getEventId());
    query.bindValue(":aid", reg.getAttendeeId());
    query.bindValue(":rs", reg.getRegistrationStatus());
    query.bindValue(":ps", reg.getPaymentStatus());
    query.bindValue(":date", QString::fromStdString(reg.getRegistrationDate()));
    query.bindValue(":ap", reg.getAmountPaid());
    query.bindValue(":ta", reg.getTotalAmount());
    query.bindValue(":n", QString::fromStdString(reg.getNotes()));
    query.bindValue(":id", id);
    return query.exec();
}
bool Database::deleteRegistration(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM Registrations WHERE RegistrationID=:id");
    query.bindValue(":id", id);
    return query.exec();
}
// BudgetItem
bool Database::updateBudgetItem(int id, const BudgetItem& item) {
    QSqlQuery query;
    query.prepare("UPDATE BudgetItems SET EventID=:eid, Type=:t, Category=:c, Amount=:a, Date=:d, Status=:s "
                  "WHERE ItemID=:id");
    query.bindValue(":eid", item.getEventId());
    query.bindValue(":t", QString::fromStdString(item.getType()));
    query.bindValue(":c", QString::fromStdString(item.getCategory()));
    query.bindValue(":a", item.getAmount());
    query.bindValue(":d", QString::fromStdString(item.getDate()));
    query.bindValue(":s", QString::fromStdString(item.getStatus()));
    query.bindValue(":id", id);
    return query.exec();
}

bool Database::deleteBudgetItem(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM BudgetItems WHERE ItemID=:id");
    query.bindValue(":id", id);
    return query.exec();
}

// ================= GET ALL OPERATIONS =================
Attendee* Database::getAllAttendees(int& count) {
    QSqlQuery countQuery("SELECT COUNT(*) FROM Attendees");
    count = countQuery.next() ? countQuery.value(0).toInt() : 0;
    if (count == 0) return nullptr;

    Attendee* list = new Attendee[count];
    QSqlQuery query("SELECT * FROM Attendees");
    int i = 0;
    while (query.next() && i < count) {
        int id = query.value("AttendeeID").toInt();
        string fName = query.value("FirstName").toString().toStdString();
        string lName = query.value("LastName").toString().toStdString();
        string email = query.value("Email").toString().toStdString();
        string phone = query.value("Phone").toString().toStdString();
        string address = query.value("Address").toString().toStdString();
        list[i++] = Attendee(id, fName, lName, email, phone, address);
    }
    return list;
}

Venue* Database::getAllVenues(int& count) {
    QSqlQuery countQuery("SELECT COUNT(*) FROM Venues");
    count = countQuery.next() ? countQuery.value(0).toInt() : 0;
    if (count == 0) return nullptr;

    Venue* list = new Venue[count];
    QSqlQuery query("SELECT * FROM Venues");
    int i = 0;
    while (query.next() && i < count) {
        string name = query.value("Name").toString().toStdString();
        string address = query.value("Address").toString().toStdString();
        int capacity = query.value("Capacity").toInt();
        string contactNumber = query.value("ContactNumber").toString().toStdString();
        string contactEmail = query.value("ContactEmail").toString().toStdString();
        bool hasWifi = query.value("HasWifi").toInt() == 1;
        bool hasParking = query.value("HasParking").toInt() == 1;
        bool hasCatering = query.value("HasCatering").toInt() == 1;
        bool hasAVEquipment = query.value("HasAVEquipment").toInt() == 1;
        list[i++] = Venue(name, address, capacity, contactNumber, contactEmail, hasWifi, hasParking, hasCatering, hasAVEquipment);
    }
    return list;
}

Event* Database::getAllEvents(int& count) {
    QSqlQuery countQuery("SELECT COUNT(*) FROM Events");
    count = countQuery.next() ? countQuery.value(0).toInt() : 0;
    if (count == 0) return nullptr;

    Event* list = new Event[count];
    QSqlQuery query("SELECT * FROM Events");
    int i = 0;
    while (query.next() && i < count) {
        string name = query.value("Name").toString().toStdString();
        string description = query.value("Description").toString().toStdString();
        string date = query.value("Date").toString().toStdString();
        string time = query.value("Time").toString().toStdString();
        int duration = query.value("Duration").toInt();
        int capacity = query.value("Capacity").toInt();
        int type = query.value("Type").toInt();
        int status = query.value("Status").toInt();
        Event e(name, description, date, time, duration, capacity, type);
        e.setVenueId(query.value("VenueID").toInt());
        e.changeStatus(status);
        list[i++] = e;
    }
    return list;
}

Registration* Database::getAllRegistrations(int& count) {
    QSqlQuery countQuery("SELECT COUNT(*) FROM Registrations");
    count = countQuery.next() ? countQuery.value(0).toInt() : 0;
    if (count == 0) return nullptr;

    Registration* list = new Registration[count];
    QSqlQuery query("SELECT * FROM Registrations");
    int i = 0;
    while (query.next() && i < count) {
        int id = query.value("RegistrationID").toInt();
        int eventId = query.value("EventID").toInt();
        int attendeeId = query.value("AttendeeID").toInt();
        int regStatus = query.value("RegistrationStatus").toInt();
        int payStatus = query.value("PaymentStatus").toInt();
        string date = query.value("RegistrationDate").toString().toStdString();
        double amountPaid = query.value("AmountPaid").toDouble();
        double totalAmount = query.value("TotalAmount").toDouble();
        string notes = query.value("Notes").toString().toStdString();
        list[i++] = Registration(id, eventId, attendeeId, regStatus, payStatus, date, amountPaid, totalAmount, notes);
    }
    return list;
}

BudgetItem* Database::getAllBudgetItems(int& count) {
    QSqlQuery countQuery("SELECT COUNT(*) FROM BudgetItems");
    count = countQuery.next() ? countQuery.value(0).toInt() : 0;
    if (count == 0) return nullptr;

    BudgetItem* list = new BudgetItem[count];
    QSqlQuery query("SELECT * FROM BudgetItems");
    int i = 0;
    while (query.next() && i < count) {
        int eventId = query.value("EventID").toInt();
        string type = query.value("Type").toString().toStdString();
        string category = query.value("Category").toString().toStdString();
        double amount = query.value("Amount").toDouble();
        string date = query.value("Date").toString().toStdString();
        string status = query.value("Status").toString().toStdString();
        list[i++] = BudgetItem(eventId, type, category, amount, date, status);
    }
    return list;
}

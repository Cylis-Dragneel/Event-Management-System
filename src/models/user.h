#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    int userId;
    std::string username;
    std::string password;
    std::string email;
    int role; // 0 = Attendee, 1 = Organizer

public:
    User();
    User(int userId, std::string username, std::string password, std::string email, int role);
    User(std::string username, std::string password, std::string email, int role);

    int getUserId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getEmail() const;
    int getRole() const;
    std::string getRoleText() const;

    void setUserId(int id);
    void setUsername(std::string username);
    void setPassword(std::string password);
    void setEmail(std::string email);
    void setRole(int role);

    bool isOrganizer() const;
    bool isAttendee() const;

    static const int ROLE_ATTENDEE = 0;
    static const int ROLE_ORGANIZER = 1;
};

#endif
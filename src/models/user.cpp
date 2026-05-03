#include "user.h"

User::User() : userId(-1), username(""), password(""), email(""), role(ROLE_ATTENDEE) {}

User::User(int userId, std::string username, std::string password, std::string email, int role)
    : userId(userId), username(username), password(password), email(email), role(role) {}

User::User(std::string username, std::string password, std::string email, int role)
    : userId(-1), username(username), password(password), email(email), role(role) {}

int User::getUserId() const { return userId; }
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getEmail() const { return email; }
int User::getRole() const { return role; }

std::string User::getRoleText() const {
    return role == ROLE_ORGANIZER ? "Organizer" : "Attendee";
}

void User::setUserId(int id) { userId = id; }
void User::setUsername(std::string username) { this->username = username; }
void User::setPassword(std::string password) { this->password = password; }
void User::setEmail(std::string email) { this->email = email; }
void User::setRole(int role) { this->role = role; }

bool User::isOrganizer() const { return role == ROLE_ORGANIZER; }
bool User::isAttendee() const { return role == ROLE_ATTENDEE; }
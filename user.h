#pragma once
#include <string>
using namespace std;

bool insertUser(const string& username, const string& password,
    const string& email, const string& role);

bool loginUser(const string& username, const string& password,
    int& out_user_id, string& out_role);

void fetchUser(int user_id);
bool userExists(int user_id);
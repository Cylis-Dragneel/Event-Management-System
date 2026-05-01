#pragma once
#include <string>
using namespace std;

bool insertVenue(const string& name, const string& location, int capacity);
void fetchVenue(int venue_id);
void fetchAllVenues();
bool updateVenue(int venue_id, const string& name,
    const string& location, int capacity);
bool deleteVenue(int venue_id);

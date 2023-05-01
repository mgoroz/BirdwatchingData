#pragma once

// i changed sprintf_s to snprintf,
// exception to std::runtime_error
// strcpy_s to strncpy
// localtime_s to localtime_r


#include <iostream>
#include <string>
#include <exception>
#include <random>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <initializer_list>
#include <tuple>
#include <optional>


//To do

#include "stdio.h"
#include "Time.h"

using namespace std;

#include "Date.h"
#include "Item.h"
#include "Data.h"
//To do

void PrepareNames();
string CreateRandomName();
void Test();

#include "Main.h"

extern default_random_engine Generator;
//extert std::mt19937 Generator(5489U);
extern uniform_int_distribution<int> GroupDistribution;
extern uniform_int_distribution<int> SubgroupDistribution;
const Date Item::Begin = Date(1, 1, 2018), Item::End = Date(31, 12, 2018);

Item::Item()
{  // creates a pseudo-random item
    Group = (char)GroupDistribution(Generator);
    Subgroup = SubgroupDistribution(Generator);
    Name = ::CreateRandomName();
    Timestamp = Date::CreateRandomDate(Begin, End);
}

// To do

Item::Item(const Item& other) {
    Group = other.Group;
    Subgroup = other.Subgroup;
    Name = other.Name;
    Timestamp = other.Timestamp;
}

Item::Item(char group, int subgroup, std::string name, Date timestamp) {
    Group = group;
    Subgroup = subgroup;
    Name = name;
    Timestamp = timestamp;
}

Item::~Item() {
}


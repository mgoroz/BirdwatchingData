#include "Main.h"

Data::Data(int n) {
    for (int i = 0; i < n; ++i) {
        Item newItem;
        char group = newItem.GetGroup();
        int subgroup = newItem.GetSubgroup();

        if (DataStructure.find(group) == DataStructure.end()) {
            DataStructure[group] = map<int, set<Item> *>();
        }

        if (DataStructure[group].find(subgroup) == DataStructure[group].end()) {
            DataStructure[group][subgroup] = new set<Item>;
        }

        DataStructure[group][subgroup]->insert(newItem);
    }
}

Data::Data() {
}

Data::~Data() {
    for (auto &group : DataStructure) {
        for (auto &subgroup : group.second) {
            delete subgroup.second;
        }
    }
}

string Data::DateToString(const Date& date) const {
    Date temp(date.GetDay(), date.GetMonth(), date.GetYear());
    return temp.ToString();
}
 
void Data::PrintAll() {
    for (const auto& groupPair : DataStructure) {
        cout << groupPair.first << ":" << endl;
        for (const auto& subgroupPair : groupPair.second) {
            for (const Item& item : *(subgroupPair.second)) {
                cout << subgroupPair.first << ": " << item.GetName() << " " << DateToString(item.GetTimestamp()) << endl;
            }
        }
    }
}

int Data::CountItems() {
    int count = 0;
    for (const auto &group : DataStructure) {
        for (const auto &subgroup : group.second) {
            count += subgroup.second->size();
        }
    }
    return count;
}

map<int, set<Item> *>* Data::GetGroup(char c) {
    auto groupIt = DataStructure.find(c);
    if (groupIt == DataStructure.end()) {
        return nullptr;
    }
    return &groupIt->second;
}

void Data::PrintGroup(char c) {
    auto groupIter = DataStructure.find(c);
    if (groupIter != DataStructure.end()) {
        // Print group label
        cout << "Group " << c << ":" << endl;

        for (const auto& subgroupPair : groupIter->second) {
            cout << subgroupPair.first << ": ";
            const auto& itemSet = *(subgroupPair.second);
            for (auto itemIter = itemSet.begin(); itemIter != itemSet.end(); ++itemIter) {
                cout << itemIter->GetName() << " " << DateToString(itemIter->GetTimestamp());
                if (next(itemIter) != itemSet.end()) {
                    cout << "; ";
                }
            }
            cout << endl;
        }
    } else {
        cout << c << " Group not found." << endl;
    }
}


int Data::CountGroupItems(char c) {
    map<int, set<Item> *>* group = GetGroup(c);
    if (!group) {
        return 0;
    }

    int count = 0;
    for (const auto &subgroup : *group) {
        count += subgroup.second->size();
    }
    return count;
}

set<Item>* Data::GetSubgroup(char c, int i) {
    map<int, set<Item> *>* group = GetGroup(c);
    if (!group) {
        return nullptr;
    }

    auto subgroupIt = group->find(i);
    if (subgroupIt == group->end()) {
        return nullptr;
    }
    return subgroupIt->second;
}

void Data::PrintSubgroup(char c, int i) {
    auto groupIter = DataStructure.find(c);
    if (groupIter != DataStructure.end()) {
        auto subgroupIter = groupIter->second.find(i);
        if (subgroupIter != groupIter->second.end()) {
            cout << "Contents of group " << c << " subgroup " << i << ":" << endl;

            for (const Item& item : *(subgroupIter->second)) {
                cout << item.GetName() << " " << DateToString(item.GetTimestamp()) << endl;
            }
        } else {
            cout << c << ": " << i << ": ";
            throw invalid_argument("Subgroup not found\n");
        }
    } else {
        cout << c << ": ";
        throw invalid_argument("Group not found");
    }
}


int Data::CountSubgroupItems(char c, int i) {
    set<Item>* subgroup = GetSubgroup(c, i);
    if (!subgroup) {
        return 0;
    }
    return subgroup->size();
}

optional<Item> Data::GetItem(char c, int i, string s) {
    set<Item>* subgroup = GetSubgroup(c, i);
    if (!subgroup) {
        return nullopt;
    }

    for (const Item &item : *subgroup) {
        if (item.GetName() == s) {
            return item;
        }
    }
    return nullopt;
}

void Data::PrintItem(char c, int i, string s) {
    optional<Item> itemOpt = GetItem(c, i, s);
    if (itemOpt.has_value()) {
        cout << c << ": " << i << ": " << itemOpt->GetName() << " " << DateToString(itemOpt->GetTimestamp()) << endl;
    } else {
        cout << "Item not found." << endl;
    }
}

optional<Item> Data::InsertItem(char c, int i, string s, optional<Date> d) {
    if (s.empty()) {
        return nullopt;
    }
    if (c < 'A' || c > 'Z' || i < 0 || i > 99) {
        return nullopt;
    }


    Date timestamp;
    if (d.has_value()) {
        timestamp = d.value();
    } else {
        timestamp = Date::CreateRandomDate(Date(1, 1, 2000), Date(31, 12, 2050));
    }

    Item newItem(c, i, s, timestamp);

    // Create group and subgroup if necessary
    auto& group = DataStructure[c];
    auto& subgroup = group[i];

    if (!subgroup) {
        subgroup = new set<Item>();
    }

    // Insert the item
    auto result = subgroup->insert(newItem);
    if (result.second) {
        return newItem;
    } else {
        return nullopt;
    }
}

set<Item>* Data::InsertSubgroup(char c, int i, initializer_list<tuple<string, optional<Date>>> items) {
    if (c < 'A' || c > 'Z' || i < 0 || i > 99) {
        return nullptr;
    }
    auto groupIter = DataStructure.find(c);
    if (groupIter != DataStructure.end()) {
        if (groupIter->second.find(i) != groupIter->second.end()) {
            return nullptr;
        }
    }
    

    set<Item>* newSubgroup = new set<Item>();
    for (const auto& itemTuple : items) {
        string name = get<0>(itemTuple);
        optional<Date> timestampOpt = get<1>(itemTuple);
        Date timestamp;

        if (timestampOpt.has_value()) {
            timestamp = timestampOpt.value();
        } else {
            timestamp = Date::CreateRandomDate(Date(1, 1, 2000), Date(31, 12, 2050));
        }

        Item newItem(c, i, name, timestamp);
        if (name.empty() || !newSubgroup->insert(newItem).second) {
          delete newSubgroup;
          return nullptr;
        }
    }

    DataStructure[c][i] = newSubgroup;
    return newSubgroup;
}

map<int, set<Item>*>* Data::InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<tuple<string, optional<Date>>>> items) {
    if (DataStructure.find(c) != DataStructure.end() || subgroups.size() != items.size()) {
        return nullptr;
    }
    if (c < 'A' || c > 'Z') {
        return nullptr;
    }
    if (DataStructure.find(c) != DataStructure.end())
      return nullptr;

    map<int, set<Item>*>* newGroup = new map<int, set<Item>*>();

    auto subgroupIter = subgroups.begin();
    auto itemsIter = items.begin();

    for (; subgroupIter != subgroups.end(); ++subgroupIter, ++itemsIter) {
        int i = *subgroupIter;
        const initializer_list<tuple<string, optional<Date>>>& itemList = *itemsIter;

        set<Item>* newSubgroup = new set<Item>();
        for (const auto& itemTuple : itemList) {
            string name = get<0>(itemTuple);
            optional<Date> timestampOpt = get<1>(itemTuple);
            Date timestamp;

            if (timestampOpt.has_value()) {
                timestamp = timestampOpt.value();
            } else {
                timestamp = Date::Date::CreateRandomDate(Date(1, 1, 2000), Date(31, 12, 2050));
            }

            Item newItem(c, i, name, timestamp);
            if (name.empty() || !newSubgroup->insert(newItem).second) {
              delete newSubgroup;
              for (const auto& subgrp_entry : *newGroup)
                delete subgrp_entry.second;
              return nullptr;
            }
        }

        (*newGroup)[i] = newSubgroup;
    }

    DataStructure[c] = *newGroup;
    return newGroup;
}

bool Data::RemoveItem(char c, int i, string s) {
    auto groupIter = DataStructure.find(c);
    if (groupIter == DataStructure.end()) {
        return false;
    }

    auto subgroupIter = groupIter->second.find(i);
    if (subgroupIter == groupIter->second.end()) {
        return false;
    }

    set<Item>* subgroup = subgroupIter->second;
    Item itemToFind(c, i, s, Date());
    auto itemIter = subgroup->find(itemToFind);
    if (itemIter == subgroup->end()) {
        return false;
    }

    subgroup->erase(itemIter);
    
    if (subgroup->empty()) {
        delete subgroup;
        groupIter->second.erase(subgroupIter);
    }

    if (groupIter->second.empty()) {
        DataStructure.erase(groupIter);
    }

    return true;
}

bool Data::RemoveSubgroup(char c, int i) {
    auto groupIter = DataStructure.find(c);
    if (groupIter == DataStructure.end()) {
        return false;
    }

    auto subgroupIter = groupIter->second.find(i);
    if (subgroupIter == groupIter->second.end()) {
        return false;
    }

    set<Item>* subgroup = subgroupIter->second;
    groupIter->second.erase(subgroupIter);
    delete subgroup;

    if (groupIter->second.empty()) {
        DataStructure.erase(groupIter);
    }

    return true;
}

bool Data::RemoveGroup(char c) {
    auto groupIter = DataStructure.find(c);
    if (groupIter == DataStructure.end()) {
        return false;
    }

    for (auto& subgroup : groupIter->second) {
        delete subgroup.second;
    }

    DataStructure.erase(groupIter);
    return true;
}

#pragma once

class Item
{
private:
    char Group;     // Any from range 'A'...'Z'
    int Subgroup;   // Any from range 0...99
    string Name;    // Any, but not empty
    Date Timestamp; // Any
    
public:
    static const Date Begin;
    static const Date End;
    Item();
    Item(char, int, string, Date);
    Item(const Item &);
    ~Item();
// To do
    
    char GetGroup() const {
        return Group;
    }
    int GetSubgroup() const {
        return Subgroup;
    }
    const string &GetName() const {
        return Name;
    }

    const Date &GetTimestamp() const {
        return Timestamp;
    }

    bool operator<(const Item& other) const {
        if (Group != other.Group) {
            return Group < other.Group;
        }
        if (Subgroup != other.Subgroup) {
            return Subgroup < other.Subgroup;
        }
        return Name < other.Name;
    }
};


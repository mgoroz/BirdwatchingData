#pragma once

class Data {
private:
    std::map<char, std::map<int, std::set<Item> *>> DataStructure;
    string DateToString(const Date& date) const;

public:
    Data(int n);
    Data();
    ~Data();
    
    void PrintAll();
    int CountItems();
    map<int, set<Item> *>* GetGroup(char c);
    void PrintGroup(char c);
    int CountGroupItems(char c);
    set<Item>* GetSubgroup(char c, int i);
    void PrintSubgroup(char c, int i);
    int CountSubgroupItems(char c, int i);
    optional<Item> GetItem(char c, int i, string s);
    void PrintItem(char c, int i, string s);
    optional<Item> InsertItem(char c, int i, string s, std::optional<Date> d = std::nullopt);
    set<Item>* InsertSubgroup(char c, int i, initializer_list<tuple<string, optional<Date>>> items);
    map<int, set<Item>*>* InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<tuple<string, optional<Date>>>> items);
    bool RemoveItem(char c, int i, string s);
    bool RemoveSubgroup(char c, int i);
    bool RemoveGroup(char c);

    // Additional methods if needed
};

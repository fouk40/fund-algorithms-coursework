#pragma once
#include <string>
#include <vector>
#include "Transaction.h"

using std::string;
using std::vector;

// Node for doubly-linked list of transactions
class TransactionNode {
public:
    Transaction* data;
    TransactionNode* prev;
    TransactionNode* next;
    TransactionNode(Transaction* tx) : data(tx), prev(nullptr), next(nullptr) {}
};

// Doubly-linked list to store transactions
class TransactionList {
private:
    TransactionNode* head;
    TransactionNode* tail;
    int size;
public:
    TransactionList();
    void addTransaction(Transaction* tx);
    bool removeTransaction(const string& id);
    Transaction* getTransaction(const string& id) const;
    void display() const;
    void getAllTransactions(vector<Transaction*>& list) const;
    int getSize() const;
    ~TransactionList();
};

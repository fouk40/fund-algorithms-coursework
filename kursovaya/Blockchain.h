#pragma once
#include <string>
#include "ClientBST.h"
#include "TransactionList.h"

using std::string;

class Blockchain {
private:
    ClientBST clients;
    TransactionList transactions;
public:
    Blockchain();
    void addClient(Client* client);
    bool processTransaction(Transaction* tx);
    void displayClients() const;
    void displayTransactions() const;
    // Load clients and transactions from text files
    bool loadFromFiles(const string& clientFile, const string& txFile);
    // Save clients and transactions to text files
    bool saveToFiles(const string& clientFile, const string& txFile) const;
    ~Blockchain();
};

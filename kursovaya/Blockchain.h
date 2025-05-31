#pragma once
#include <string>
#include "ClientBST.h"
#include "TransactionList.h"
#include "Client.h"

class Blockchain {
private:
    ClientBST clients;
    TransactionList transactions;

public:
    Blockchain();
    ~Blockchain();

    void addClient(Client* client);
    bool processTransaction(Transaction* tx);
    void displayClients() const;
    void displayTransactions() const;
    bool saveToFiles(const std::string& clientFile, const std::string& txFile) const;
    bool loadFromFiles(const std::string& clientFile, const std::string& txFile);

    Client* getClientById(const std::string& id); // NEW METHOD
};

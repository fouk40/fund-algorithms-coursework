#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Client.h"

using std::string;
using std::vector;

// Tree node for ClientBST
class ClientNode {
public:
    Client* data;
    ClientNode* left;
    ClientNode* right;
    ClientNode(Client* client) : data(client), left(nullptr), right(nullptr) {}
};

// Binary search tree of Clients (ordered by total balance)
class ClientBST {
private:
    ClientNode* root;
    int size;
    // Helper recursive functions
    ClientNode* insertNode(ClientNode* node, Client* client);
    Client* findById(ClientNode* node, const string& id) const;
    void inorderPrint(ClientNode* node) const;
    void collectClients(ClientNode* node, vector<Client*>& list) const;
    void deleteTree(ClientNode* node);
public:
    ClientBST();
    void addClient(Client* client);
    Client* find(const string& id) const;
    void displayInOrder() const;
    void getAllClients(vector<Client*>& list) const;
    int getSize() const;
    ~ClientBST();
};

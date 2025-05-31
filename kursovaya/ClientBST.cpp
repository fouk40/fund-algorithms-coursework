#include <iomanip>
#include "ClientBST.h"

ClientBST::ClientBST() : root(nullptr), size(0) {}

ClientBST::~ClientBST() {
    deleteTree(root);
}

void ClientBST::addClient(Client* client) {
    root = insertNode(root, client);
    size++;
}

Client* ClientBST::find(const string& id) const {
    return findById(root, id);
}

void ClientBST::displayInOrder() const {
    if (!root) {
        std::cout << "<No clients in blockchain>" << std::endl;
        return;
    }
    // Print table header
    std::cout << std::left 
              << std::setw(10) << "ID" 
              << std::setw(15) << "Name" 
              << std::setw(12) << "Type" 
              << std::setw(12) << "Balance" 
              << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    inorderPrint(root);
}

void ClientBST::getAllClients(vector<Client*>& list) const {
    collectClients(root, list);
}

int ClientBST::getSize() const {
    return size;
}

// Recursive helper to insert a new client by balance
ClientNode* ClientBST::insertNode(ClientNode* node, Client* client) {
    if (node == nullptr) {
        return new ClientNode(client);
    }
    if (client->getTotalBalance() < node->data->getTotalBalance()) {
        node->left = insertNode(node->left, client);
    } else {
        node->right = insertNode(node->right, client);
    }
    return node;
}

// Recursive helper to find a client by ID
Client* ClientBST::findById(ClientNode* node, const string& id) const {
    if (node == nullptr) return nullptr;
    if (node->data->getId() == id) {
        return node->data;
    }
    Client* found = findById(node->left, id);
    if (found) return found;
    return findById(node->right, id);
}

// Recursive helper to print clients in-order
void ClientBST::inorderPrint(ClientNode* node) const {
    if (node == nullptr) return;
    inorderPrint(node->left);
    Client* c = node->data;
    // Determine client type via dynamic cast (for output)
    string typeName;
    if (dynamic_cast<StandardClient*>(c)) typeName = "Standard";
    else if (dynamic_cast<GoldClient*>(c)) typeName = "Gold";
    else if (dynamic_cast<PlatinumClient*>(c)) typeName = "Platinum";
    else typeName = "Client";
    double totalBalance = c->getTotalBalance();
    std::cout << std::left 
              << std::setw(10) << c->getId() 
              << std::setw(15) << c->getName() 
              << std::setw(12) << typeName 
              << std::setw(12) << std::fixed << std::setprecision(2) << totalBalance 
              << std::endl;
    inorderPrint(node->right);
}

// Recursive helper to collect all clients in-order
void ClientBST::collectClients(ClientNode* node, vector<Client*>& list) const {
    if (node == nullptr) return;
    collectClients(node->left, list);
    list.push_back(node->data);
    collectClients(node->right, list);
}

// Recursive helper to delete all nodes and clients
void ClientBST::deleteTree(ClientNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node->data;   // delete Client object
    delete node;
}

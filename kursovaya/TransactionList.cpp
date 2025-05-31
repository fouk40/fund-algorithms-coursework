#include <iostream>
#include <iomanip>
#include "TransactionList.h"

TransactionList::TransactionList() : head(nullptr), tail(nullptr), size(0) {}

TransactionList::~TransactionList() {
    // Delete all nodes and transaction objects
    TransactionNode* current = head;
    while (current) {
        TransactionNode* nextNode = current->next;
        delete current->data;
        delete current;
        current = nextNode;
    }
    head = tail = nullptr;
    size = 0;
}

void TransactionList::addTransaction(Transaction* tx) {
    if (tx == nullptr) return;
    TransactionNode* node = new TransactionNode(tx);
    if (head == nullptr) {
        // list is empty
        head = tail = node;
    } else {
        // append to the tail
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    size++;
}

bool TransactionList::removeTransaction(const string& id) {
    TransactionNode* current = head;
    while (current) {
        if (current->data->getId() == id) {
            // found the transaction to remove
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                // removing head
                head = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                // removing tail
                tail = current->prev;
            }
            delete current->data;
            delete current;
            size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

Transaction* TransactionList::getTransaction(const string& id) const {
    TransactionNode* current = head;
    while (current) {
        if (current->data->getId() == id) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

void TransactionList::display() const {
    if (head == nullptr) {
        std::cout << "<No transactions>" << std::endl;
        return;
    }
    // Print table header
    std::cout << std::left 
              << std::setw(10) << "TxID" 
              << std::setw(15) << "Sender" 
              << std::setw(15) << "Recipient" 
              << std::setw(10) << "Amount" 
              << std::setw(12) << "Commission" 
              << std::setw(10) << "Type" 
              << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    // Iterate through the list and print each transaction
    TransactionNode* current = head;
    while (current) {
        Transaction* tx = current->data;
        string typeStr;
        if (tx->getType() == DEPOSIT) typeStr = "DEPOSIT";
        else if (tx->getType() == WITHDRAW) typeStr = "WITHDRAW";
        else typeStr = "TRANSFER";
        std::cout << std::left 
                  << std::setw(10) << tx->getId() 
                  << std::setw(15) << tx->getSenderWalletId() 
                  << std::setw(15) << tx->getRecipientWalletId() 
                  << std::setw(10) << std::fixed << std::setprecision(2) << tx->getAmount() 
                  << std::setw(12) << std::fixed << std::setprecision(2) << tx->getCommission() 
                  << std::setw(10) << typeStr 
                  << std::endl;
        current = current->next;
    }
}

void TransactionList::getAllTransactions(vector<Transaction*>& list) const {
    TransactionNode* current = head;
    while (current) {
        list.push_back(current->data);
        current = current->next;
    }
}

int TransactionList::getSize() const {
    return size;
}

#pragma once
#include <string>
#include "Entity.h"

using std::string;

class Wallet : public Entity {
private:
    double balance;
    string ownerId;
public:
    // Construct a Wallet with given ID, owner (client ID), and initial balance
    Wallet(const string& id, const string& ownerId, double balance = 0.0);
    bool deposit(double amount);
    bool withdraw(double amount);
    double getBalance() const;
    string getOwnerId() const;
    // (getId() inherited from Entity)
};

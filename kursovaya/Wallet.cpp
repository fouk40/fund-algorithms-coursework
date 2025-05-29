#include "Wallet.h"

Wallet::Wallet(const string& id, const string& ownerId, double balance)
    : Entity(id), balance(balance), ownerId(ownerId) {}

bool Wallet::deposit(double amount) {
    if (amount < 0) return false;
    balance += amount;
    return true;
}

bool Wallet::withdraw(double amount) {
    if (amount < 0 || amount > balance) {
        return false;
    }
    balance -= amount;
    return true;
}

double Wallet::getBalance() const {
    return balance;
}

string Wallet::getOwnerId() const {
    return ownerId;
}

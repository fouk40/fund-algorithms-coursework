#include <algorithm>
#include "Client.h"
#include "Wallet.h"

// Client base class implementation
Client::Client(const string& id, const string& name)
    : Entity(id), name(name) {
    // Initially no wallets in the collection
}

void Client::addWallet(Wallet* wallet) {
    if (wallet) {
        // If wallet belongs to a different owner, we could adjust ownerId here (not needed if correctly set)
        wallets.addEntity(wallet);
    }
}

double Client::getTotalBalance() const {
    double total = 0.0;
    // Sum the balance of all owned wallets
    for (Entity* e : wallets.getAllEntities()) {
        Wallet* w = static_cast<Wallet*>(e);
        total += w->getBalance();
    }
    return total;
}

EntityVector& Client::getWallets() {
    return wallets;
}

string Client::getName() const {
    return name;
}

// StandardClient implementation
StandardClient::StandardClient(const string& id, const string& name)
    : Client(id, name) {}

double StandardClient::calculateCommission(double amount) const {
    // Standard client: 2% commission
    return amount * 0.02;
}

double StandardClient::getMaxTransactionLimit() const {
    // Standard client max transaction amount
    return 1000.0;
}

string StandardClient::getBenefits() const {
    return "Standard client: Basic membership with no special benefits.";
}

// GoldClient implementation
GoldClient::GoldClient(const string& id, const string& name)
    : Client(id, name) {}

double GoldClient::calculateCommission(double amount) const {
    // Gold client: 1% commission
    return amount * 0.01;
}

double GoldClient::getMaxTransactionLimit() const {
    return 5000.0;
}

string GoldClient::getBenefits() const {
    return "Gold client: Lower commission and higher transaction limits.";
}

// PlatinumClient implementation
PlatinumClient::PlatinumClient(const string& id, const string& name)
    : Client(id, name) {}

double PlatinumClient::calculateCommission(double amount) const {
    // Platinum client: 0.5% commission
    return amount * 0.005;
}

double PlatinumClient::getMaxTransactionLimit() const {
    return 10000.0;
}

string PlatinumClient::getBenefits() const {
    return "Platinum client: Lowest commission and highest transaction limits.";
}

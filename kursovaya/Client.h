#pragma once
#include <string>
#include "Entity.h"
#include "EntityVector.h"
#include "Wallet.h"

using std::string;

// Abstract Client base class (inherits Entity)
class Client : public Entity {
protected:
    string name;
    EntityVector wallets;  // collection of Wallets owned by this client
public:
    Client(const string& id, const string& name);
    // Add a wallet to this client
    void addWallet(Wallet* wallet);
    // Calculate total balance across all owned wallets
    double getTotalBalance() const;
    // Abstract methods for commission calculation, transaction limit, and benefits
    virtual double calculateCommission(double amount) const = 0;
    virtual double getMaxTransactionLimit() const = 0;
    virtual string getBenefits() const = 0;
    // Accessors
    EntityVector& getWallets();
    string getName() const;
    // (getId() is inherited from Entity)
    virtual ~Client() {}
};

// StandardClient tier
class StandardClient : public Client {
public:
    StandardClient(const string& id, const string& name);
    double calculateCommission(double amount) const override;
    double getMaxTransactionLimit() const override;
    string getBenefits() const override;
};

// GoldClient tier
class GoldClient : public Client {
public:
    GoldClient(const string& id, const string& name);
    double calculateCommission(double amount) const override;
    double getMaxTransactionLimit() const override;
    string getBenefits() const override;
};

// PlatinumClient tier
class PlatinumClient : public Client {
public:
    PlatinumClient(const string& id, const string& name);
    double calculateCommission(double amount) const override;
    double getMaxTransactionLimit() const override;
    string getBenefits() const override;
};

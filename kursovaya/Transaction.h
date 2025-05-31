#pragma once
#include <string>

using std::string;

// Types of transactions
enum TxType { DEPOSIT, WITHDRAW, TRANSFER };

// Transaction record class
class Transaction {
private:
    string id;
    string senderWalletId;
    string recipientWalletId;
    double amount;
    TxType type;
    double commission;
public:
    Transaction(const string& id, const string& senderWalletId, const string& recipientWalletId,
                double amount, TxType type, double commission);
    string getId() const;
    string getSenderWalletId() const;
    string getRecipientWalletId() const;
    double getAmount() const;
    TxType getType() const;
    double getCommission() const;
    // Returns a formatted string with transaction details
    string getDetails() const;
};

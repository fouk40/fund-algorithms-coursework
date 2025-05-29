#include <sstream>
#include <iomanip>
#include "Transaction.h"

Transaction::Transaction(const string& id, const string& senderWalletId, const string& recipientWalletId,
                         double amount, TxType type, double commission)
    : id(id), senderWalletId(senderWalletId), recipientWalletId(recipientWalletId),
      amount(amount), type(type), commission(commission) {}

string Transaction::getId() const {
    return id;
}

string Transaction::getSenderWalletId() const {
    return senderWalletId;
}

string Transaction::getRecipientWalletId() const {
    return recipientWalletId;
}

double Transaction::getAmount() const {
    return amount;
}

TxType Transaction::getType() const {
    return type;
}

double Transaction::getCommission() const {
    return commission;
}

string Transaction::getDetails() const {
    std::ostringstream oss;
    oss << "Transaction[" << id << "]: ";
    if (type == DEPOSIT) {
        oss << "Deposit to " << recipientWalletId;
    } else if (type == WITHDRAW) {
        oss << "Withdraw from " << senderWalletId;
    } else if (type == TRANSFER) {
        oss << "Transfer from " << senderWalletId << " to " << recipientWalletId;
    }
    oss << ", Amount: " << std::fixed << std::setprecision(2) << amount;
    oss << ", Commission: " << std::fixed << std::setprecision(2) << commission;
    return oss.str();
}

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include "Blockchain.h"
#include "Wallet.h"
#include "Client.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

Blockchain::Blockchain() {
    // Load initial data from files, if available
    loadFromFiles("Clients.txt", "Blockchain_transactions.txt");
}

Blockchain::~Blockchain() {
    // Save data to files upon destruction
    saveToFiles("Clients.txt", "Blockchain_transactions.txt");
}

void Blockchain::addClient(Client* client) {
    if (client) {
        clients.addClient(client);
    }
}

bool Blockchain::processTransaction(Transaction* tx) {
    if (tx == nullptr) return false;
    string senderId = tx->getSenderWalletId();
    string recipientId = tx->getRecipientWalletId();
    double amount = tx->getAmount();
    TxType type = tx->getType();
    double commission = 0.0;
    bool success = false;
    // Helper lambda to find a Wallet by ID across all clients
    auto findWalletById = [&](const string& wid) -> Wallet* {
        vector<Client*> allClients;
        clients.getAllClients(allClients);
        for (Client* client : allClients) {
            vector<Entity*>& wlist = client->getWallets().getAllEntities();
            for (Entity* e : wlist) {
                if (e->getId() == wid) {
                    return static_cast<Wallet*>(e);
                }
            }
        }
        return nullptr;
    };
    // Determine action based on transaction type
    switch (type) {
        case TRANSFER: {
            Wallet* senderWallet = findWalletById(senderId);
            Wallet* recipientWallet = findWalletById(recipientId);
            if (senderWallet && recipientWallet) {
                Client* senderClient = clients.find(senderWallet->getOwnerId());
                if (senderClient) {
                    commission = senderClient->calculateCommission(amount);
                    // Check transaction limit and sufficient balance (amount + commission)
                    if (amount <= senderClient->getMaxTransactionLimit() && 
                        senderWallet->getBalance() >= amount + commission) {
                        // Perform the transfer
                        senderWallet->withdraw(amount + commission);
                        recipientWallet->deposit(amount);
                        success = true;
                        // (We assume the Transaction's commission field was set appropriately)
                    }
                }
            }
            break;
        }
        case DEPOSIT: {
            // Deposit (external source to a client's wallet)
            Wallet* recipientWallet = findWalletById(recipientId);
            if (recipientWallet) {
                commission = 0.0;
                recipientWallet->deposit(amount);
                success = true;
            }
            break;
        }
        case WITHDRAW: {
            // Withdraw (from client's wallet to external)
            Wallet* senderWallet = findWalletById(senderId);
            if (senderWallet) {
                Client* senderClient = clients.find(senderWallet->getOwnerId());
                if (senderClient) {
                    commission = senderClient->calculateCommission(amount);
                    if (amount <= senderClient->getMaxTransactionLimit() && 
                        senderWallet->getBalance() >= amount + commission) {
                        senderWallet->withdraw(amount + commission);
                        success = true;
                    }
                }
            }
            break;
        }
    }
    if (success) {
        transactions.addTransaction(tx);
    } else {
        // If failed, discard the transaction object to avoid memory leak
        delete tx;
    }
    return success;
}

void Blockchain::displayClients() const {
    clients.displayInOrder();
}

void Blockchain::displayTransactions() const {
    transactions.display();
}

bool Blockchain::loadFromFiles(const string& clientFile, const string& txFile) {
    // Load clients from Clients.txt
    FILE* cf = fopen(clientFile.c_str(), "r");
    if (cf) {
        char cid[100], cname[100], ctype[20];
        int walletCount;
        // Read each client record
        while (fscanf(cf, "%99s %99s %19s %d", cid, cname, ctype, &walletCount) == 4) {
            string idStr(cid), nameStr(cname), typeStr(ctype);
            Client* clientPtr = nullptr;
            // Determine client type and create appropriate object
            if (typeStr == "Standard") {
                clientPtr = new StandardClient(idStr, nameStr);
            } else if (typeStr == "Gold") {
                clientPtr = new GoldClient(idStr, nameStr);
            } else if (typeStr == "Platinum") {
                clientPtr = new PlatinumClient(idStr, nameStr);
            } else {
                clientPtr = new StandardClient(idStr, nameStr); // default to Standard
            }
            // Read the specified number of wallets for this client
            for (int i = 0; i < walletCount; ++i) {
                char wid[100];
                double balance;
                if (fscanf(cf, "%99s %lf", wid, &balance) == 2) {
                    string walletId(wid);
                    Wallet* wallet = new Wallet(walletId, idStr, balance);
                    clientPtr->addWallet(wallet);
                }
            }
            addClient(clientPtr);
        }
        fclose(cf);
    }
    // Load transactions from Blockchain_transactions.txt
    FILE* tf = fopen(txFile.c_str(), "r");
    if (tf) {
        char txid[100], sid[100], rid[100], ttype[20];
        double amount, commission;
        // Read each transaction record
        while (fscanf(tf, "%99s %99s %99s %lf %19s %lf", txid, sid, rid, &amount, ttype, &commission) == 6) {
            string txIdStr(txid);
            // Convert "NULL" placeholders to empty string for external wallets
            string senderIdStr = (string(sid) == "NULL" ? string("") : string(sid));
            string recipientIdStr = (string(rid) == "NULL" ? string("") : string(rid));
            string typeStr(ttype);
            TxType txType;
            if (typeStr == "DEPOSIT") txType = DEPOSIT;
            else if (typeStr == "WITHDRAW") txType = WITHDRAW;
            else txType = TRANSFER;
            // Create transaction object and add to list
            Transaction* tx = new Transaction(txIdStr, senderIdStr, recipientIdStr, amount, txType, commission);
            transactions.addTransaction(tx);
        }
        fclose(tf);
    }
    return true;
}

bool Blockchain::saveToFiles(const string& clientFile, const string& txFile) const {
    // Save clients to Clients.txt
    FILE* cf = fopen(clientFile.c_str(), "w");
    if (cf) {
        vector<Client*> clientList;
        clients.getAllClients(clientList);
        for (Client* c : clientList) {
            // Determine client type as string
            string typeName;
            if (dynamic_cast<StandardClient*>(c)) typeName = "Standard";
            else if (dynamic_cast<GoldClient*>(c)) typeName = "Gold";
            else if (dynamic_cast<PlatinumClient*>(c)) typeName = "Platinum";
            else typeName = "Client";
            // Get all wallets for client
            vector<Entity*>& wlist = c->getWallets().getAllEntities();
            int count = wlist.size();
            fprintf(cf, "%s %s %s %d\n", c->getId().c_str(), c->getName().c_str(), typeName.c_str(), count);
            for (Entity* e : wlist) {
                Wallet* w = static_cast<Wallet*>(e);
                fprintf(cf, "%s %.2f\n", w->getId().c_str(), w->getBalance());
            }
        }
        fclose(cf);
    }
    // Save transactions to Blockchain_transactions.txt
    FILE* tf = fopen(txFile.c_str(), "w");
    if (tf) {
        vector<Transaction*> txList;
        transactions.getAllTransactions(txList);
        for (Transaction* tx : txList) {
            string typeStr;
            if (tx->getType() == DEPOSIT) typeStr = "DEPOSIT";
            else if (tx->getType() == WITHDRAW) typeStr = "WITHDRAW";
            else typeStr = "TRANSFER";
            // Use "NULL" as placeholder for external sender/recipient in output
            string sid = tx->getSenderWalletId();
            if (sid.empty()) sid = "NULL";
            string rid = tx->getRecipientWalletId();
            if (rid.empty()) rid = "NULL";
            fprintf(tf, "%s %s %s %.2f %s %.2f\n",
                    tx->getId().c_str(),
                    sid.c_str(),
                    rid.c_str(),
                    tx->getAmount(),
                    typeStr.c_str(),
                    tx->getCommission());
        }
        fclose(tf);
    }
    return true;
}

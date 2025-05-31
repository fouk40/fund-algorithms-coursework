#include "Blockchain.h"
#include <cstdio>
#include "Wallet.h"
#include "Client.h"
#include <cstring>



Blockchain::Blockchain() {
    loadFromFiles("Clients.txt", "Blockchain_transactions.txt");
}

Blockchain::~Blockchain() {
    saveToFiles("Clients.txt", "Blockchain_transactions.txt");
}

void Blockchain::addClient(Client* client) {
    if (client) clients.addClient(client);
}

bool Blockchain::processTransaction(Transaction* tx) {
    if (!tx) return false;

    std::string senderId = tx->getSenderWalletId();
    std::string recipientId = tx->getRecipientWalletId();
    double amount = tx->getAmount();
    TxType type = tx->getType();

    auto findWallet = [&](const std::string& wid) -> Wallet* {
        std::vector<Client*> all;
        clients.getAllClients(all);
        for (Client* c : all) {
            for (Entity* e : c->getWallets().getAllEntities()) {
                if (e->getId() == wid)
                    return static_cast<Wallet*>(e);
            }
        }
        return nullptr;
    };

    double commission = 0.0;
    bool success = false;

    if (type == TRANSFER) {
        Wallet* sender = findWallet(senderId);
        Wallet* recipient = findWallet(recipientId);
        if (sender && recipient) {
            Client* senderClient = getClientById(sender->getOwnerId());
            commission = senderClient->calculateCommission(amount);
            if (amount + commission <= sender->getBalance()) {
                sender->withdraw(amount + commission);
                recipient->deposit(amount);
                success = true;
            }
        }
    } else if (type == DEPOSIT) {
        Wallet* recipient = findWallet(recipientId);
        if (recipient) {
            recipient->deposit(amount);
            success = true;
        }
    } else if (type == WITHDRAW) {
        Wallet* sender = findWallet(senderId);
        if (sender) {
            Client* senderClient = getClientById(sender->getOwnerId());
            commission = senderClient->calculateCommission(amount);
            if (amount + commission <= sender->getBalance()) {
                sender->withdraw(amount + commission);
                success = true;
            }
        }
    }

    if (success) {
        transactions.addTransaction(tx);
    } else {
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

Client* Blockchain::getClientById(const std::string& id) {
    return clients.find(id);
}

bool Blockchain::saveToFiles(const std::string& clientFile, const std::string& txFile) const {
    FILE* cf = fopen(clientFile.c_str(), "w");
    if (cf) {
        std::vector<Client*> list;
        clients.getAllClients(list);
        for (Client* c : list) {
            std::string type = "Standard";
            if (dynamic_cast<GoldClient*>(c)) type = "Gold";
            else if (dynamic_cast<PlatinumClient*>(c)) type = "Platinum";
            auto& wallets = c->getWallets().getAllEntities();
            fprintf(cf, "%s %s %s %zu\n", c->getId().c_str(), c->getName().c_str(), type.c_str(), wallets.size());
            for (Entity* e : wallets) {
                Wallet* w = static_cast<Wallet*>(e);
                fprintf(cf, "%s %.2f\n", w->getId().c_str(), w->getBalance());
            }
        }
        fclose(cf);
    }

    FILE* tf = fopen(txFile.c_str(), "w");
    if (tf) {
        std::vector<Transaction*> list;
        transactions.getAllTransactions(list);
        for (Transaction* tx : list) {
            std::string typeStr = (tx->getType() == DEPOSIT ? "DEPOSIT" : tx->getType() == WITHDRAW ? "WITHDRAW" : "TRANSFER");
            fprintf(tf, "%s %s %s %.2f %s %.2f\n",
                    tx->getId().c_str(),
                    tx->getSenderWalletId().empty() ? "NULL" : tx->getSenderWalletId().c_str(),
                    tx->getRecipientWalletId().empty() ? "NULL" : tx->getRecipientWalletId().c_str(),
                    tx->getAmount(),
                    typeStr.c_str(),
                    tx->getCommission());
        }
        fclose(tf);
    }
    return true;
}

bool Blockchain::loadFromFiles(const std::string& clientFile, const std::string& txFile) {
    FILE* cf = fopen(clientFile.c_str(), "r");
    if (cf) {
        char id[100], name[100], type[20];
        int count;
        while (fscanf(cf, "%s %s %s %d", id, name, type, &count) == 4) {
            Client* c = nullptr;
            if (strcmp(type, "Standard") == 0) c = new StandardClient(id, name);
            else if (strcmp(type, "Gold") == 0) c = new GoldClient(id, name);
            else if (strcmp(type, "Platinum") == 0) c = new PlatinumClient(id, name);
            for (int i = 0; i < count; ++i) {
                char wid[100];
                double bal;
                fscanf(cf, "%s %lf", wid, &bal);
                Wallet* w = new Wallet(wid, id, bal);
                c->addWallet(w);
            }
            addClient(c);
        }
        fclose(cf);
    }

    FILE* tf = fopen(txFile.c_str(), "r");
    if (tf) {
        char id[100], sid[100], rid[100], type[20];
        double amt, comm;
        while (fscanf(tf, "%s %s %s %lf %s %lf", id, sid, rid, &amt, type, &comm) == 6) {
            TxType t = TRANSFER;
            if (strcmp(type, "DEPOSIT") == 0) t = DEPOSIT;
            else if (strcmp(type, "WITHDRAW") == 0) t = WITHDRAW;
            Transaction* tx = new Transaction(id, strcmp(sid, "NULL") == 0 ? "" : sid, strcmp(rid, "NULL") == 0 ? "" : rid, amt, t, comm);
            transactions.addTransaction(tx);
        }
        fclose(tf);
    }
    return true;
}

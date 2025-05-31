#include <iostream>
#include "Blockchain.h"
#include "Client.h"
#include "Wallet.h"
#include "Transaction.h"

using namespace std;

int main() {
    Blockchain bc;
    int choice;

    do {
        cout << "\n====== BLOCKCHAIN MENU ======" << endl;
        cout << "1. Add new client" << endl;
        cout << "2. Add wallet to client" << endl;
        cout << "3. Deposit to wallet" << endl;
        cout << "4. Withdraw from wallet" << endl;
        cout << "5. Transfer between wallets" << endl;
        cout << "6. Show all clients" << endl;
        cout << "7. Show all transactions" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string id, name;
            int type;
            cout << "Client ID: "; cin >> id;
            cout << "Client name: "; cin >> name;
            cout << "Client type (1-Standard, 2-Gold, 3-Platinum): "; cin >> type;
            Client* c = nullptr;
            if (type == 1) c = new StandardClient(id, name);
            else if (type == 2) c = new GoldClient(id, name);
            else if (type == 3) c = new PlatinumClient(id, name);
            if (c) bc.addClient(c);
        }
        else if (choice == 2) {
            string clientId, walletId;
            double balance;
            cout << "Client ID: "; cin >> clientId;
            cout << "Wallet ID: "; cin >> walletId;
            cout << "Initial balance: "; cin >> balance;
            Client* client = bc.getClientById(clientId);
            if (client) {
                Wallet* w = new Wallet(walletId, clientId, balance);
                client->addWallet(w);
                cout << "Wallet added." << endl;
            } else cout << "Client not found." << endl;
        }
        else if (choice == 3 || choice == 4 || choice == 5) {
            string txId, sender, recipient;
            double amount;
            cout << "Transaction ID: "; cin >> txId;
            cout << "Amount: "; cin >> amount;
            TxType type;
            if (choice == 3) {
                type = DEPOSIT;
                cout << "Recipient wallet ID: "; cin >> recipient;
                sender = "NULL";
            } else if (choice == 4) {
                type = WITHDRAW;
                cout << "Sender wallet ID: "; cin >> sender;
                recipient = "NULL";
            } else {
                type = TRANSFER;
                cout << "Sender wallet ID: "; cin >> sender;
                cout << "Recipient wallet ID: "; cin >> recipient;
            }
            Transaction* tx = new Transaction(txId, sender, recipient, amount, type, 0.0);
            if (!bc.processTransaction(tx)) {
                cout << "Transaction failed." << endl;
            } else {
                cout << "Transaction successful." << endl;
            }
        }
        else if (choice == 6) {
            bc.displayClients();
        }
        else if (choice == 7) {
            bc.displayTransactions();
        }
    } while (choice != 0);

    // Save data to files before exit
    bc.saveToFiles("Clients.txt", "Blockchain_transactions.txt");
    cout << "Data saved. Exiting..." << endl;

    return 0;
}

// ADD THIS TO Blockchain.h:
// Client* getClientById(const string& id);

// ADD THIS TO Blockchain.cpp:
// Client* Blockchain::getClientById(const string& id) {
//     return clients.find(id);
// }

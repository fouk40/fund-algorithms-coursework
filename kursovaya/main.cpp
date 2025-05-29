#include <iostream>
#include "Blockchain.h"
#include "Client.h"
#include "Wallet.h"
#include "Transaction.h"

using namespace std;

int main() {
    Blockchain bc;
    // Create clients of different tiers
    Client* alice = new GoldClient("C001", "Alice");
    Wallet* aliceWallet = new Wallet("W100", alice->getId(), 1000.0);
    alice->addWallet(aliceWallet);
    bc.addClient(alice);

    Client* bob = new StandardClient("C002", "Bob");
    Wallet* bobWallet1 = new Wallet("W200", bob->getId(), 500.0);
    Wallet* bobWallet2 = new Wallet("W201", bob->getId(), 300.0);
    bob->addWallet(bobWallet1);
    bob->addWallet(bobWallet2);
    bc.addClient(bob);

    Client* charlie = new PlatinumClient("C003", "Charlie");
    Wallet* charlieWallet = new Wallet("W300", charlie->getId(), 10000.0);
    charlie->addWallet(charlieWallet);
    bc.addClient(charlie);

    // Perform some transactions:
    // 1. Bob (Standard) sends 200 to Alice (Gold)
    double amount1 = 200.0;
    double commission1 = bob->calculateCommission(amount1);
    Transaction* tx1 = new Transaction("T001", "W200", "W100", amount1, TRANSFER, commission1);
    bc.processTransaction(tx1);

    // 2. Alice deposits 150 to her wallet (external source)
    Transaction* tx2 = new Transaction("T002", "NULL", "W100", 150.0, DEPOSIT, 0.0);
    bc.processTransaction(tx2);

    // 3. Charlie withdraws 500 from his wallet (to external)
    Transaction* tx3 = new Transaction("T003", "W300", "NULL", 500.0, WITHDRAW, 0.0);
    bc.processTransaction(tx3);

    // Display all clients (ID, Name, Type, Total Balance)
    cout << "\nCurrent clients in blockchain:\n";
    bc.displayClients();

    // Display all transaction records
    cout << "\nTransaction history:\n";
    bc.displayTransactions();

    // Save the data to files (this will also be done in Blockchain destructor)
    bc.saveToFiles("Clients.txt", "Blockchain_transactions.txt");

    return 0;
}

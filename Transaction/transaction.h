#ifndef TRANSACTION_H
#define TRANSACTION_H
int checkBalance(int userIndex,double amount);
int depositMoney(int userIndex,double amount);
double getMemberDiscount(int userIndex);
int processPayment(int userIndex,double amount);
void displayBalance(int userIndex);
#endif

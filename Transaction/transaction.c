#include <stdio.h>
#include "../Data/data.h"
#include "transaction.h"

int checkBalance(int n, double amount)
{
    if (n < 0 || n >= userCount || amount < 0)
        return 0;
    return users[n].balance >= amount;
}

int depositMoney(int n, double amount)
{
    if (n < 0 || n >= userCount || amount <= 0)
        return 0;
    users[n].balance += amount;
    saveUsers();
    return 1;
}

int processPayment(int n, double amount)
{
    if (!checkBalance(n, amount))
    {
        if (n >= 0 && n < userCount)
            printf("Insufficient balance. Current balance: $%.2f\n", users[n].balance);
        return 0;
    }
    users[n].balance -= amount;
    saveUsers();
    printf("Payment successful. Remaining balance: $%.2f\n", users[n].balance);
    return 1;
}

void displayBalance(int n)
{
    if (n >= 0 && n < userCount)
        printf("Current balance: $%.2f\n", users[n].balance);
}

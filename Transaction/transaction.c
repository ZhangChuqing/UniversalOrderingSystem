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
    if (n < 0 || n >= userCount)
        return 0;

    /* Only the three fixed top-up packages are accepted. */
    if (amount != (double)(int)amount)
        return 0;

    switch ((int)amount)
    {
    case 100:
        users[n].balance += amount;
        if (users[n].membershipTier < 1)
            users[n].membershipTier = 1;
        users[n].snackCoupons++;
        break;

    case 200:
        users[n].balance += amount;
        if (users[n].membershipTier < 2)
            users[n].membershipTier = 2;
        users[n].classicPizzaCoupons++;
        break;

    case 500:
        users[n].balance += amount;
        if (users[n].membershipTier < 3)
            users[n].membershipTier = 3;
        users[n].pepperoniPizzaCoupons++;
        users[n].snackCoupons++;
        break;

    default:
        return 0;
    }

    saveUsers();
    return 1;
}

double getMemberDiscount(int n)
{
    if (n < 0 || n >= userCount)
        return 0.0;
    if (users[n].membershipTier == 3)
        return 0.15;
    if (users[n].membershipTier == 2)
        return 0.12;
    if (users[n].membershipTier == 1)
        return 0.10;
    return 0.0;
}
int processPayment(int n, double amount)
{
    if (!checkBalance(n, amount))
    {
        if (n >= 0 && n < userCount)
            printf("Insufficient balance. Current balance: $%.2f\n", users[n].balance);

        printf("\n===== Special Membership Offers =====\n");
        printf("Top up your wallet and enjoy exclusive discounts!\n");
        printf("1. $100 - 10%% OFF + 1 Free Snack Coupon\n");
        printf("2. $200 - 12%% OFF + 1 Free Classic Cheese Pizza Coupon\n");
        printf("3. $500 - 15%% OFF + 1 Free Pepperoni Pizza Coupon + 1 Free Snack Coupon\n");

        printf("\nYou can also choose direct payment without topping up.\n");
        printf("Your order has not been placed. Please try again.\n");
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

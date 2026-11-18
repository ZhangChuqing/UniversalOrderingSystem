#include <stdio.h>
#include "../Data/data.h"
#include "../User/user.h"
#include "../Product/product.h"
#include "../Order/order.h"
#include "../Transaction/transaction.h"
#include "customer_ui.h"
void customerMenu(int n)
{
    int c;
    double amount;
    if (n < 0 || n >= userCount)
        return;
    do
    {
        printf("\n===== CUSTOMER MENU =====\nWelcome, %s!\n", users[n].name);
        puts("1 View menu\n2 Place order\n3 Order history\n4 Account details\n5 Top up balance\n0 Logout");
        printf("Choice: ");
        if (scanf("%d", &c) != 1)
            return;
        switch (c)
        {
        case 1:
            placeOrder(n);
            break;
        case 2:
            placeOrder(n);
            break;
        case 3:
            displayOrderHistory(users[n].userID);
            break;
        case 4:
            displayUser(n);
            displayBalance(n);
            break;
        case 5:
            printf("Top-up amount: ");
            if (scanf("%lf", &amount) == 1 && depositMoney(n, amount))
                puts("Top-up successful.");
            else
                puts("Invalid amount.");
            break;
        case 0:
            puts("Logged out.");
            break;
        default:
            puts("Invalid choice.");
        }
    } while (c != 0);
}

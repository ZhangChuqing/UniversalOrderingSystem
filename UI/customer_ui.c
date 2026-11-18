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
        puts("1 View menu\n2 Place order\n3 Order history\n4 Account details\n5 Top up / Upgrade membership\n0 Logout");
        printf("Choice: ");

        if (scanf("%d", &c) != 1)
            return;

        switch (c)
        {
        case 1:
            displayProductList();
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
        {
            int packageChoice;

            puts("\n--- MEMBERSHIP TOP-UP PACKAGES ---");
            puts("1. $100 package  - 10% off + 1 free snack coupon");
            puts("2. $200 package  - 12% off + 1 Classic Cheese Pizza coupon");
            puts("3. $500 package  - 15% off + 1 Pepperoni Pizza coupon + 1 free snack coupon");
            puts("0. Cancel");
            printf("Select package: ");

            if (scanf("%d", &packageChoice) != 1)
            {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
                puts("Invalid input. Please enter 0, 1, 2 or 3.");
                break;
            }

            if (packageChoice == 0)
            {
                puts("Top-up cancelled.");
                break;
            }

            switch (packageChoice)
            {
                case 1:
                    amount = 100.0;
                    break;
                case 2:
                    amount = 200.0;
                    break;
                case 3:
                    amount = 500.0;
                    break;
                default:
                    puts("Invalid package selection. Please choose 1, 2 or 3.");
                    break;
            }

            if (packageChoice < 1 || packageChoice > 3)
                break;

            if (depositMoney(n, amount))
            {
                printf("Top-up successful. $%.2f added to your wallet.\n", amount);
                printf("Your current balance is $%.2f.\n", users[n].balance);
                printf("Your membership discount is now %d%%.\n",
                       users[n].membershipTier == 3 ? 15 :
                       users[n].membershipTier == 2 ? 12 :
                       users[n].membershipTier == 1 ? 10 : 0);
                if (packageChoice == 1)
                    puts("Reward added: 1 free snack coupon.");
                else if (packageChoice == 2)
                    puts("Reward added: 1 free Classic Cheese Pizza coupon.");
                else if (packageChoice == 3)
                    puts("Rewards added: 1 free Pepperoni Pizza coupon and 1 free snack coupon.");
            }
            else
                puts("Invalid package selection.");
            break;
        }

        case 0:
            puts("Logged out.");
            break;

        default:
            puts("Invalid choice.");
        }
    } while (c != 0);
}

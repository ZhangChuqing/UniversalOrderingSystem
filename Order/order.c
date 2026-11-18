#include <stdio.h>
#include <string.h>
#include "../Data/data.h"
#include "../Product/product.h"
#include "../Transaction/transaction.h"
#include "order.h"

int getNextOrderID(void)
{
    int i, m = 0;
    for (i = 0; i < orderCount; i++)
        if (orders[i].orderID > m)
            m = orders[i].orderID;
    return m + 1;
}

void placeOrder(int n)
{
    int id, p, size, qty, oid, paymentChoice, couponChoice = 0;
    char crust[MAX_TEXT], topping[MAX_TEXT], reward[MAX_TEXT] = "None";
    double unit, discount, total, finalUnit;
    Order o;

    if (n < 0 || n >= userCount || !users[n].active)
        return;
    if (orderCount >= MAX_ORDERS)
    {
        puts("Order storage full.");
        return;
    }

    displayProductList();
    printf("Pizza ID: ");
    if (scanf("%d", &id) != 1)
        return;
    p = findProduct(id);
    if (p < 0)
    {
        puts("Invalid product.");
        return;
    }

    printf("Size (10/12/14/16): ");
    if (scanf("%d", &size) != 1 ||
        (size != 10 && size != 12 && size != 14 && size != 16))
    {
        puts("Invalid size.");
        return;
    }

    printf("Crust (Thin/Regular/Thick): ");
    if (scanf(" %29s", crust) != 1)
        return;
    if (strcmp(crust, "Thin") && strcmp(crust, "Regular") && strcmp(crust, "Thick"))
    {
        puts("Invalid crust.");
        return;
    }

    printf("Topping (None/ExtraCheese/Mushroom/Olive): ");
    if (scanf(" %29s", topping) != 1)
        return;
    if (strcmp(topping, "None") && strcmp(topping, "ExtraCheese") &&
        strcmp(topping, "Mushroom") && strcmp(topping, "Olive"))
    {
        puts("Invalid topping.");
        return;
    }

    printf("Quantity (1-20): ");
    if (scanf("%d", &qty) != 1 || qty < 1 || qty > 20)
    {
        puts("Invalid quantity.");
        return;
    }

    unit = calculateProductPrice(products[p].basePrice, size, crust, topping);
    discount = getMemberDiscount(n);
    finalUnit = unit * (1.0 - discount);
    total = finalUnit * qty;

    printf("\nOriginal unit price: $%.2f\n", unit);
    printf("Membership discount: %.0f%%\n", discount * 100.0);
    printf("Discounted total: $%.2f\n", total);

    if (users[n].classicPizzaCoupons > 0 ||
        users[n].pepperoniPizzaCoupons > 0 ||
        users[n].snackCoupons > 0)
    {
        puts("\nAvailable coupons:");
        puts("0. Do not use a coupon");
        if (users[n].classicPizzaCoupons > 0 && products[p].productID == 1)
            printf("1. Free Classic Cheese Pizza (remaining: %d)\n",
                   users[n].classicPizzaCoupons);
        if (users[n].pepperoniPizzaCoupons > 0 && products[p].productID == 2)
            printf("2. Free Pepperoni Pizza (remaining: %d)\n",
                   users[n].pepperoniPizzaCoupons);
        if (users[n].snackCoupons > 0)
            printf("3. Free snack (remaining: %d)\n", users[n].snackCoupons);
        printf("Coupon choice: ");
        if (scanf("%d", &couponChoice) != 1)
            return;

        if (couponChoice == 1 && users[n].classicPizzaCoupons > 0 &&
            products[p].productID == 1)
        {
            total -= finalUnit; /* One pizza is free. */
            strcpy(reward, "ClassicCoupon");
        }
        else if (couponChoice == 2 && users[n].pepperoniPizzaCoupons > 0 &&
                 products[p].productID == 2)
        {
            total -= finalUnit;
            strcpy(reward, "PepperoniCoupon");
        }
        else if (couponChoice == 3 && users[n].snackCoupons > 0)
        {
            strcpy(reward, "SnackCoupon");
        }
        else if (couponChoice != 0)
        {
            puts("That coupon is not available for this order. No coupon was applied.");
            couponChoice = 0;
        }
    }

    if (total < 0)
        total = 0;
    printf("Final amount: $%.2f\n", total);
    puts("\nPayment method:");
    puts("1. Pay using wallet balance");
    puts("2. Pay directly (no top-up required)");
    printf("Choice: ");
    if (scanf("%d", &paymentChoice) != 1)
        return;

    if (paymentChoice == 1)
    {
        if (!processPayment(n, total))
        {
            puts("Order cancelled. You can top up your wallet or choose direct payment next time.");
            return;
        }
    }
    else if (paymentChoice == 2)
    {
        printf("Direct payment successful. Amount paid: $%.2f\n", total);
    }
    else
    {
        puts("Invalid payment method. Order cancelled.");
        return;
    }

    /* Coupons are consumed only after payment succeeds. */
    if (couponChoice == 1)
        users[n].classicPizzaCoupons--;
    else if (couponChoice == 2)
        users[n].pepperoniPizzaCoupons--;
    else if (couponChoice == 3)
        users[n].snackCoupons--;

    oid = getNextOrderID();
    o.orderID = oid;
    o.userID = users[n].userID;
    o.productID = products[p].productID;
    strcpy(o.productName, products[p].name);
    strcpy(o.crust, crust);
    o.size = size;
    strcpy(o.topping, topping);
    o.quantity = qty;
    o.unitPrice = unit;
    o.status = 1;
    o.totalPaid = total;
    strcpy(o.reward, reward);
    orders[orderCount++] = o;

    saveUsers();
    saveOrders();
    printf("Order confirmed. Order number: %d\n", oid);
    if (strcmp(reward, "None") != 0)
        printf("Reward applied: %s\n", reward);
}

void displayOrderHistory(int id)
{
    int i, found = 0;
    puts("\n--- Order History ---");
    for (i = 0; i < orderCount; i++)
        if (orders[i].userID == id && orders[i].status == 1)
        {
            printf("#%d | %s | %d inch | %s | %s | Qty %d | Paid $%.2f | Reward: %s\n",
                   orders[i].orderID, orders[i].productName, orders[i].size,
                   orders[i].crust, orders[i].topping, orders[i].quantity,
                   orders[i].totalPaid, orders[i].reward);
            found = 1;
        }
    if (!found)
        puts("No order history yet.");
}

void displayAllOrders(void)
{
    int i;
    puts("\n--- All Orders ---");
    for (i = 0; i < orderCount; i++)
        if (orders[i].status == 1)
            printf("#%d | Customer %d | %s | Qty %d | Paid $%.2f | Reward: %s\n",
                   orders[i].orderID, orders[i].userID, orders[i].productName,
                   orders[i].quantity, orders[i].totalPaid, orders[i].reward);
}

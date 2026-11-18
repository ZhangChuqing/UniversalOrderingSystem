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
    int id, p, size, qty, oid;
    char crust[MAX_TEXT], topping[MAX_TEXT];
    double unit, total;
    Order o;
    if (n < 0 || n >= userCount)
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
    if (scanf("%d", &size) != 1 || (size != 10 && size != 12 && size != 14 && size != 16))
    {
        puts("Invalid size.");
        return;
    }
    printf("Crust (Thin/Regular/Thick): ");
    scanf(" %29s", crust);
    if (strcmp(crust, "Thin") && strcmp(crust, "Regular") && strcmp(crust, "Thick"))
    {
        puts("Invalid crust.");
        return;
    }
    printf("Topping (None/ExtraCheese/Mushroom/Olive): ");
    scanf(" %29s", topping);
    if (strcmp(topping, "None") && strcmp(topping, "ExtraCheese") && strcmp(topping, "Mushroom") && strcmp(topping, "Olive"))
    {
        puts("Invalid topping.");
        return;
    }
    if (!strcmp(topping, "ExtraCheese"))
        strcpy(topping, "ExtraCheese");
    printf("Quantity (1-20): ");
    if (scanf("%d", &qty) != 1 || qty < 1 || qty > 20)
    {
        puts("Invalid quantity.");
        return;
    }
    unit = calculateProductPrice(products[p].basePrice, size, crust, topping);
    total = unit * qty;
    printf("Unit price: $%.2f | Total: $%.2f\n", unit, total);
    if (!processPayment(n, total))
    {
        puts("Order cancelled.");
        return;
    }
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
    orders[orderCount++] = o;
    saveOrders();
    printf("Order confirmed. Order number: %d\n", oid);
}

void displayOrderHistory(int id)
{
    int i, found = 0;
    puts("\n--- Order History ---");
    for (i = 0; i < orderCount; i++)
        if (orders[i].userID == id && orders[i].status == 1)
        {
            printf("#%d | %s | %d inch | %s | %s | Qty %d | $%.2f\n", orders[i].orderID, orders[i].productName, orders[i].size, orders[i].crust, orders[i].topping, orders[i].quantity, orders[i].unitPrice * orders[i].quantity);
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
            printf("#%d | Customer %d | %s | Qty %d | $%.2f\n", orders[i].orderID, orders[i].userID, orders[i].productName, orders[i].quantity, orders[i].unitPrice * orders[i].quantity);
}

#include <stdio.h>
#include <string.h>
#include "data.h"

User users[MAX_USERS];
Product products[MAX_PRODUCTS];
Order orders[MAX_ORDERS];
int userCount = 0, productCount = 0, orderCount = 0;

static void defaultProducts(void)
{
    if (productCount == 0)
    {
        products[0].productID = 1;
        strcpy(products[0].name, "ClassicCheese");
        products[0].basePrice = 8.00;
        products[0].available = 1;

        products[1].productID = 2;
        strcpy(products[1].name, "Pepperoni");
        products[1].basePrice = 10.00;
        products[1].available = 1;

        products[2].productID = 3;
        strcpy(products[2].name, "Vegetarian");
        products[2].basePrice = 9.00;
        products[2].available = 1;

        productCount = 3;
        saveProducts();
    }
}

void loadData(void)
{
    FILE *fp;
    char line[256];

    fp = fopen("users.txt", "r");
    if (fp != NULL)
    {
        while (userCount < MAX_USERS && fgets(line, sizeof(line), fp) != NULL)
        {
            User u;
            int fields;

            /* Current format: ID|username|name|telephone|password|balance|active */
            /* New format also stores membership tier and coupon counts. */
            fields = sscanf(line,
                            "%d|%19[^|]|%49[^|]|%19[^|]|%19[^|]|%lf|%d|%d|%d|%d|%d",
                            &u.userID, u.username, u.name, u.telephone,
                            u.password, &u.balance, &u.active, &u.membershipTier,
                            &u.classicPizzaCoupons, &u.pepperoniPizzaCoupons,
                            &u.snackCoupons);

            if (fields == 11)
            {
                users[userCount++] = u;
            }
            else
            {
                /* Compatibility with older pipe-separated user records. */
                fields = sscanf(line, "%d|%19[^|]|%49[^|]|%19[^|]|%19[^|]|%lf|%d",
                                &u.userID, u.username, u.name, u.telephone,
                                u.password, &u.balance, &u.active);
                if (fields == 7)
                {
                    u.membershipTier = 0;
                    u.classicPizzaCoupons = 0;
                    u.pepperoniPizzaCoupons = 0;
                    u.snackCoupons = 0;
                    users[userCount++] = u;
                }
                else
                {
                    /* Compatibility with files created by the earliest version. */
                    fields = sscanf(line, "%d %49s %19s %19s %lf %d",
                                    &u.userID, u.name, u.telephone, u.password,
                                    &u.balance, &u.active);
                    if (fields == 6)
                    {
                        snprintf(u.username, sizeof(u.username), "user%d", u.userID);
                        u.membershipTier = 0;
                        u.classicPizzaCoupons = 0;
                        u.pepperoniPizzaCoupons = 0;
                        u.snackCoupons = 0;
                        users[userCount++] = u;
                    }
                }
            }
        }
        fclose(fp);
    }

    fp = fopen("products.txt", "r");
    if (fp != NULL)
    {
        while (productCount < MAX_PRODUCTS &&
               fscanf(fp, "%d %49s %lf %d",
                      &products[productCount].productID,
                      products[productCount].name,
                      &products[productCount].basePrice,
                      &products[productCount].available) == 4)
            productCount++;
        fclose(fp);
    }

    fp = fopen("orders.txt", "r");
    if (fp != NULL)
    {
        while (orderCount < MAX_ORDERS && fgets(line, sizeof(line), fp) != NULL)
        {
            Order o;
            int fields = sscanf(line, "%d %d %d %49s %29s %d %29s %d %lf %d %lf %29s",
                                &o.orderID, &o.userID, &o.productID,
                                o.productName, o.crust, &o.size, o.topping,
                                &o.quantity, &o.unitPrice, &o.status,
                                &o.totalPaid, o.reward);
            if (fields == 12)
                orders[orderCount++] = o;
            else
            {
                fields = sscanf(line, "%d %d %d %49s %29s %d %29s %d %lf %d",
                                &o.orderID, &o.userID, &o.productID,
                                o.productName, o.crust, &o.size, o.topping,
                                &o.quantity, &o.unitPrice, &o.status);
                if (fields == 10)
                {
                    o.totalPaid = o.unitPrice * o.quantity;
                    strcpy(o.reward, "None");
                    orders[orderCount++] = o;
                }
            }
        }
        fclose(fp);
    }

    defaultProducts();
}

void saveUsers(void)
{
    FILE *fp;
    int i;

    fp = fopen("users.txt", "w");
    if (fp == NULL)
    {
        puts("Error: could not save users.txt.");
        return;
    }

    for (i = 0; i < userCount; i++)
    {
        fprintf(fp, "%d|%s|%s|%s|%s|%.2f|%d|%d|%d|%d|%d\n",
                users[i].userID, users[i].username, users[i].name,
                users[i].telephone, users[i].password,
                users[i].balance, users[i].active, users[i].membershipTier,
                users[i].classicPizzaCoupons, users[i].pepperoniPizzaCoupons,
                users[i].snackCoupons);
    }
    fclose(fp);
}

void saveProducts(void)
{
    FILE *fp;
    int i;

    fp = fopen("products.txt", "w");
    if (fp == NULL)
        return;

    for (i = 0; i < productCount; i++)
    {
        fprintf(fp, "%d %s %.2f %d\n",
                products[i].productID, products[i].name,
                products[i].basePrice, products[i].available);
    }
    fclose(fp);
}

void saveOrders(void)
{
    FILE *fp;
    int i;

    fp = fopen("orders.txt", "w");
    if (fp == NULL)
        return;

    for (i = 0; i < orderCount; i++)
    {
        fprintf(fp, "%d %d %d %s %s %d %s %d %.2f %d %.2f %s\n",
                orders[i].orderID, orders[i].userID,
                orders[i].productID, orders[i].productName,
                orders[i].crust, orders[i].size,
                orders[i].topping, orders[i].quantity,
                orders[i].unitPrice, orders[i].status,
                orders[i].totalPaid, orders[i].reward);
    }
    fclose(fp);
}

void saveAllData(void)
{
    saveUsers();
    saveProducts();
    saveOrders();
}

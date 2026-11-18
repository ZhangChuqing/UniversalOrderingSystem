#include <stdio.h>
#include <string.h>
#include "../Data/data.h"
#include "product.h"
int findProduct(int id)
{
    int i;
    for (i = 0; i < productCount; i++)
        if (products[i].productID == id && products[i].available)
            return i;
    return -1;
}

void displayProductList(void)
{
    int i;
    puts("\n========== PIZZA MENU ==========");
    for (i = 0; i < productCount; i++)
        if (products[i].available)
            printf("%d. %-20s Base price: $%.2f\n", products[i].productID, products[i].name, products[i].basePrice);
    puts("Sizes: 10 / 12 / 14 / 16 inches");
    puts("Crusts: Thin / Regular / Thick");
    puts("Toppings: None / ExtraCheese / Mushroom / Olive");
}

void addProduct(void)
{
    Product p;
    int i, maxID = 0;
    if (productCount >= MAX_PRODUCTS)
    {
        puts("Product list full.");
        return;
    }
    for (i = 0; i < productCount; i++)
        if (products[i].productID > maxID)
            maxID = products[i].productID;
    p.productID = maxID + 1;
    printf("Pizza name (one word): ");
    scanf(" %49[^\n]", p.name);
    printf("Base price: ");
    if (scanf("%lf", &p.basePrice) != 1 || p.basePrice < 0)
    {
        puts("Invalid price.");
        return;
    }
    p.available = 1;
    products[productCount++] = p;
    saveProducts();
    puts("Pizza added.");
}

void editProduct(void)
{
    int id, n;
    printf("Product ID: ");
    if (scanf("%d", &id) != 1)
        return;
    n = findProduct(id);
    if (n < 0)
    {
        puts("Product not found.");
        return;
    }
    printf("New name (one word): ");
    scanf(" %49[^\n]", products[n].name);
    printf("New base price: ");
    if (scanf("%lf", &products[n].basePrice) != 1 || products[n].basePrice < 0)
    {
        puts("Invalid price.");
        return;
    }
    saveProducts();
    puts("Pizza updated.");
}

void deleteProduct(void)
{
    int id, n;
    printf("Product ID: ");
    if (scanf("%d", &id) != 1)
        return;
    n = findProduct(id);
    if (n < 0)
    {
        puts("Product not found.");
        return;
    }
    products[n].available = 0;
    saveProducts();
    puts("Pizza removed from menu.");
}

double calculateProductPrice(double base, int size, const char *crust, const char *topping)
{
    double p = base;
    if (size == 12)
        p += 2;
    else if (size == 14)
        p += 4;
    else if (size == 16)
        p += 6;
    if (strcmp(crust, "Thick") == 0)
        p += 1.5;
    if (strcmp(topping, "ExtraCheese") == 0)
        p += 1.5;
    else if (strcmp(topping, "Mushroom") == 0 || strcmp(topping, "Olive") == 0)
        p += 1;
    return p;
}

#ifndef STRUCTURES_H
#define STRUCTURES_H
#define MAX_USERS 100
#define MAX_PRODUCTS 100
#define MAX_ORDERS 500
#define MAX_NAME 50
#define MAX_TEXT 30


typedef struct
{
    int userID;
    char username[20];
    char name[MAX_NAME];
    char telephone[20];
    char password[20];
    double balance;
    int active;
    int membershipTier;          /* 0 = none, 1 = $100, 2 = $200, 3 = $500 */
    int classicPizzaCoupons;
    int pepperoniPizzaCoupons;
    int snackCoupons;
} User;


typedef struct
{
    int productID;
    char name[MAX_NAME];
    double basePrice;
    int available;
} Product;


typedef struct
{
    int orderID, userID, productID;
    char productName[MAX_NAME], crust[MAX_TEXT];
    int size;
    char topping[MAX_TEXT];
    int quantity;
    double unitPrice;
    int status;
    double totalPaid;
    char reward[MAX_TEXT];
} Order;
#endif
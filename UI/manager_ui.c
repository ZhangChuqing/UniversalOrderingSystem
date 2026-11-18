#include <stdio.h>
#include "../Product/product.h"
#include "../User/user.h"
#include "../Order/order.h"
#include "../Statistics/statistics.h"
#include "manager_ui.h"

void managerMenu(void)
{
    int c, id, n;
    do
    {
        puts("\n===== MANAGER MENU =====");
        puts("1 Display menu\n"
             "2 Add pizza\n"
             "3 Edit pizza\n"
             "4 Delete pizza\n"
             "5 Display customers\n"
             "6 Edit customer\n"
             "7 Delete customer\n"
             "8 Display orders\n"
             "9 Sales statistics\n"
             "0 Logout");
        printf("Choice: ");
        if (scanf("%d", &c) != 1)
            return;
        switch (c)
        {
        case 1:
            displayProductList();
            break;
        case 2:
            addProduct();
            break;
        case 3:
            editProduct();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            displayUserList();
            break;
        case 6:
            printf("Customer ID: ");
            scanf("%d", &id);
            n = findUser(id);
            if (n >= 0)
                editUser(n);
            else
                puts("Customer not found.");
            break;
        case 7:
            printf("Customer ID: ");
            scanf("%d", &id);
            n = findUser(id);
            if (n >= 0)
                deleteUser(n);
            else
                puts("Customer not found.");
            break;
        case 8:
            displayAllOrders();
            break;
        case 9:
            displaySalesStatistics();
            break;
        case 0:
            puts("Manager logged out.");
            break;
        default:
            puts("Invalid choice.");
        }
    } while (c != 0);
}

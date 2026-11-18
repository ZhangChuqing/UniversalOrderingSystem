#include <stdio.h>
#include "../Data/data.h"
#include "statistics.h"

void displaySalesStatistics(void)
{
    int i, qty = 0, count = 0;
    double revenue = 0;
    for (i = 0; i < orderCount; i++)
        if (orders[i].status == 1)
        {
            qty += orders[i].quantity;
            count++;
            revenue += orders[i].totalPaid;
        }
    puts("\n======= SALES STATISTICS =======");
    printf("Number of order records: %d\nTotal pizzas sold: %d\nTotal revenue: $%.2f\n", count, qty, revenue);
}

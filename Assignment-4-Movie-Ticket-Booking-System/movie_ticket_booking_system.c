#include <stdio.h>
#include <string.h>

int main()
{
    char customerName[100];
    int tickets;
    int ticketCategory;
    float subtotal;
    float discount;
    float finalBill;
    const int STANDARD_PRICE = 150;
    const int PREMIUM_PRICE = 250;
    const int RECLINER_PRICE = 400;
    int ticketPrice;
    char ticketCategoryName[100];

    scanf("%99s", customerName);
    scanf("%d", &tickets);
    while (tickets < 1 || tickets > 20)
    {
        printf("Error: Number of tickets must be between 1 and 20. \n");
        printf("Please enter the number of tickets again: ");
        scanf("%d", &tickets);
    }
    scanf("%d" , &ticketCategory);
    while (ticketCategory < 1 || ticketCategory > 3){
        printf("Error: Invalid ticket category.\n");
        printf("Please enter a category (1-Standard, 2-Premium, 3-Recliner): ");
        scanf("%d", &ticketCategory);
    }

    switch (ticketCategory)
    {
    case 1:
        ticketPrice = STANDARD_PRICE;
        strcpy(ticketCategoryName, "Standard");
        break;
    
    case 2:
        ticketPrice = PREMIUM_PRICE;
        strcpy(ticketCategoryName, "Premium");
        break;

    case 3:
        ticketPrice = RECLINER_PRICE;
        strcpy(ticketCategoryName, "Recliner");
        break;
    }

    subtotal = tickets * ticketPrice;
    if (tickets >= 5)
    {
        discount = (10 * subtotal) / 100 ;
    }
    else
    {
        discount = 0;
    }
    finalBill = subtotal - discount;

    printf("Customer: %s\n" , customerName);
    printf("Ticket Category: %s\n" , ticketCategoryName);
    printf("Tickets: %d\n" , tickets);
    printf("Subtotal: %.2f\n", subtotal);
    printf("Discount: %.2f\n", discount);
    printf("Final Bill: %.2f\n",finalBill);

    return 0;
}
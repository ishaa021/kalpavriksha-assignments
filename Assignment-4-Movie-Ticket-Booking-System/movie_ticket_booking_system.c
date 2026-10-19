#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isValidTicketCount(int tickets);
bool isValidTicketCategory(int category);
int getTicketPrice(int category, char categoryName[]);
void displayTicketDetails(char customerName[], char ticketCategoryName[], int tickets, float subtotal, float discount, float finalBill);

int main()
{
    char customerName[100];

    int tickets;
    int ticketCategory;
    int ticketPrice;

    bool validTicketCount;
    bool validTicketCategory;

    float subtotal;
    float discount;
    float finalBill;

    char ticketCategoryName[100];

    scanf("%99s", customerName);
    scanf("%d", &tickets);

    validTicketCount = isValidTicketCount(tickets);

    while (!validTicketCount)
    {
        printf("Error: Number of tickets must be between 1 and 20. \n");
        printf("Please enter the number of tickets again: ");
        scanf("%d", &tickets);

        validTicketCount = isValidTicketCount(tickets);
    }

    scanf("%d", &ticketCategory);

    validTicketCategory = isValidTicketCategory(ticketCategory);

    while (!validTicketCategory)
    {
        printf("Error: Invalid ticket category.\n");
        printf("Please enter a category (1-Standard, 2-Premium, 3-Recliner): ");     
        scanf("%d", &ticketCategory);

        validTicketCategory = isValidTicketCategory(ticketCategory);
    }

    ticketPrice = getTicketPrice(ticketCategory, ticketCategoryName);

    if (ticketPrice == 0)
    {
        printf("Error: Invalid ticket category.\n");
        return 1;
    }

    subtotal = tickets * ticketPrice;

    if (tickets >= 5)
    {
        discount = (10 * subtotal) / 100;
    }
    else
    {
        discount = 0;
    }

    finalBill = subtotal - discount;
    
    displayTicketDetails(customerName, ticketCategoryName, tickets, subtotal, discount, finalBill);

    return 0;
}

bool isValidTicketCount(int tickets)
{
    return tickets >= 1 && tickets <= 20;
}

bool isValidTicketCategory(int category)
{
    return category >= 1 && category <= 3;
}

int getTicketPrice(int category, char categoryName[])
{
    const int STANDARD_PRICE = 150;
    const int PREMIUM_PRICE = 250;
    const int RECLINER_PRICE = 400;

    switch (category)
    {
        case 1:
            strcpy(categoryName, "Standard");
            return STANDARD_PRICE;

        case 2:
            strcpy(categoryName, "Premium");
            return PREMIUM_PRICE;

        case 3:
            strcpy(categoryName, "Recliner");
            return RECLINER_PRICE;

        default:
            return 0;
    }
}

void displayTicketDetails(char customerName[], char ticketCategoryName[], int tickets, float subtotal, float discount, float finalBill)
{
    printf("Customer: %s\n", customerName);
    printf("Ticket Category: %s\n", ticketCategoryName);
    printf("Tickets: %d\n", tickets);
    printf("Subtotal: %.2f\n", subtotal);
    printf("Discount: %.2f\n", discount);    
    printf("Final Bill: %.2f\n", finalBill);
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
# define NAME 100

struct ticket
{
    char source[NAME+1];
    char destination[NAME+1];
    struct ticket *next;
};

int main()
{
    int n;
    scanf("%d",&n);
    if(n<2 || n > 105 ){
        printf("invalid n ");
        return 1;
    }
    
    struct ticket *ticket[n];

    for (int i = 0 ; i < n ; i++)
    {
        struct ticket *newTicket = malloc(sizeof(struct ticket));
        if(newTicket == NULL)
        {
            printf("memeory allocation failed");
            return 1;
        }
        scanf("%s -> %s",newTicket->source , newTicket->destination);
        newTicket->next = NULL;
        ticket[i] = newTicket;
    }

   struct ticket *head = NULL;
    for (int i = 0 ; i < n ; i++)
    {
        for(int j=0;j<n;j++){
            if(strcmp(ticket[i]->destination,ticket[j]->source)==0)
            {
                ticket[i]->next=ticket[j];
                if(head == NULL){
                    head = ticket[i];
                }
                break;
            }
        }
    }

    struct ticket *temp = head;

    while(temp!=NULL)
    {
        printf(" %s -> %s " , temp->source , temp->destination);
        temp=temp->next;
    }
    return 0;
}

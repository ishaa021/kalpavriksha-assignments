#include <stdio.h>
#include <string.h>

struct user
{
    int id;
    char name[100];
    int age;
};

int idExists(int id)
{
    FILE *fptr;
    struct user u1;
    int exists = 0;
     fptr = fopen("users.txt", "r");

    while (fscanf(fptr, "%d %s %d", &u1.id, u1.name, &u1.age) == 3)
    {
        if (u1.id == id)
        {
            exists = 1;
            break;
        }
    }

    fclose(fptr);
    return exists;
}

void createUser()
{
    FILE *fptr;
    fptr = fopen("users.txt", "a");
    
    struct user u1;
    printf("add user's details:-\n");

    int exists = 1;

  while (exists == 1)
  {
    printf("user's unique id - ");
    scanf("%d", &u1.id);

    exists = idExists(u1.id);

    if (exists == 1)
    {
        printf("This id already exists. Please enter another id.\n");
    }
  }

    printf("user's name - ");
    scanf("%s", u1.name);
    printf("user's age:-");
    scanf("%d", &u1.age);

    fprintf(fptr, "%d %s %d\n", u1.id, u1.name, u1.age);

    fclose(fptr);
    printf("user added successfully \n");

}

void readUser()
{
 FILE *fptr;
 fptr = fopen("users.txt", "r");
 struct user u1;

while (fscanf(fptr, "%d %s %d", &u1.id, u1.name, &u1.age) == 3)
{
   printf("%d %s %d\n", u1.id, u1.name, u1.age);
    
}
fclose(fptr);
}

void updateUser()
{
 struct user u1;
 int searchId;
 int found = 0;

FILE *fptr;
FILE *temp;

printf("enter the user id:-");
scanf("%d", &searchId);

fptr = fopen("users.txt", "r");
temp = fopen("temp.txt", "w");

while (fscanf(fptr, "%d %s %d", &u1.id, u1.name, &u1.age) == 3)
{
    if (u1.id == searchId)
    {
        printf("enter new name , age :- ");
        scanf("%s %d", u1.name, &u1.age);
        found = 1;
    }
    fprintf(temp, "%d %s %d\n", u1.id, u1.name, u1.age);
  
}
fclose(fptr);
fclose(temp);

if (found == 0)
{
    printf("User with this id does not exist.\n");
}
else
{
    printf("User updated successfully.\n");
}
remove("users.txt");
rename("temp.txt","users.txt");
}

void deleteUser()
{
 struct user u1;
 FILE *fptr;
 FILE *temp;

int deleteId;
int found = 0;

printf("enter the user id:-");
scanf("%d", &deleteId);

fptr = fopen("users.txt", "r");
temp = fopen("temp.txt", "w");

while (fscanf(fptr, "%d %s %d", &u1.id, u1.name, &u1.age) == 3)
{
    if (u1.id == deleteId)
    {
     found = 1;
    }
    else
    {
          fprintf(temp, "%d %s %d\n", u1.id, u1.name, u1.age);
    }
}

fclose(fptr);
fclose(temp);

if (found == 0)
{
    printf("User with this id does not exist.\n");
}
else
{
    printf("User deleted successfully.\n");
}
remove("users.txt");
rename("temp.txt","users.txt");
}


int main()
{
     FILE *fptr;
    fptr = fopen("users.txt", "a");

    if (fptr == NULL)
    {
        printf("file does not exist\n");
    }
     fclose(fptr);

    int choice = 0;  

    while (choice != 5)
   {
     printf("\npress 1 to create new user.\n");
     printf("press 2 to see all the users info.\n");
     printf("press 3 to update user details.\n");
     printf("press 4 to delete any user.\n");
     printf("press 5 to exit\n");
    
     printf("choose operation :");
     scanf("%d", &choice);
     switch (choice)
     {
        case 1:
            createUser();
            break;
        case 2:
            readUser();
            break;
        case 3:
            updateUser();
            break;
        case 4:
            deleteUser();
            break;
        case 5:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice.\n");
     }
}
    return 0;
}
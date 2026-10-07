#include <stdio.h>

struct student
{
int rollNo;
char name[100];
int marks[3];
};

int calculateMarks(int marks[3])
{
    int total = 0;
    for(int i = 0 ; i < 3 ; i++)
    {
        total = total + marks[i];
    }
    return total;
}

float calculateAverage(int total)
{
    return total / 3.0;
}

char calculateGrade(float average)
{
    char grade;
    if (average >= 85)
    {
            grade = 'A';
        }
        else if (average >= 70)
        {
           grade = 'B';
        }
        else if (average >= 50)
        {
           grade = 'C';
        }
        else if (average >= 35)
        {
           grade = 'D';
        }
        else 
        {
           grade = 'F';
        }
        return grade;
}

int printPerformance(char grade)
{
  int stars = 0;
       if (grade == 'F')
       {
        stars = 0;
       }      
       else if (grade == 'A')
       {
        stars = 5;
       }
       else if (grade == 'B')
       {
        stars = 4;
       }
       else if (grade == 'C')
       {
        stars = 3;
       }
       else if (grade == 'D')
       {
        stars = 2;
       }
return stars;

}

void printRollNumbers(struct student s[], int n, int i) 
{
    if (i == n)
     { 
          return;
     }
    printf("%d ", s[i].rollNo);
   printRollNumbers(s, n, i + 1);
}

int main()
{
    int n;
    scanf("%d", &n);
    if (n < 1 || n > 100)
    {
        printf("Invalid number of students.\n");
        return 1;
    }
    struct student s[n];
    for (int i = 0 ; i < n ; i++)
    {
        scanf("%d", &s[i].rollNo);
        scanf("%s", s[i].name);
        for (int j = 0 ; j < 3 ; j++)
        {
        scanf("%d", &s[i].marks[j]);
         if (s[i].marks[j] < 0 || s[i].marks[j] > 100)
            {
                printf("Invalid marks. Marks must be between 0 and 100.\n");
                return 1;
            }
        }
    }

    for (int i = 0 ; i < n ; i++)
    {
        printf("Roll: %d\n", s[i].rollNo);
        printf("Name: %s\n", s[i].name);

       int total = calculateMarks(s[i].marks);
       printf("Total: %d\n", total);

       float average = calculateAverage(total);
        printf("Average: %.2f\n", average);

        char grade = calculateGrade(average);
        printf("Grade: %c\n", grade);
       
        if (average < 35) 
        {
            printf("\n");
            continue;
        }
        int stars = printPerformance(grade);
         printf("Performance: ");
        for (int j = 0 ; j < stars ; j++)
        {
            printf("*");
        }
        printf("\n\n");
    }
     printf("List of Roll Numbers (via recursion): ");
    printRollNumbers(s, n, 0);
    
    return 0;
}
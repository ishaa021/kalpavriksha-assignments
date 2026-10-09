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
    switch (grade)
    {
        case 'A':
            stars = 5;
            break;

        case 'B':
            stars = 4;
            break;

        case 'C':
            stars = 3;
            break;

        case 'D':
            stars = 2;
            break;

        case 'F':
            stars = 0;
            break;

        default:
            stars = 0;
    }
    return stars;
}

void printRollNumbers(struct student students[], int n, int i) 
{
    if (i == n)
     { 
          return;
     }
    printf("%d ", students[i].rollNo);
    printRollNumbers(students, n, i + 1);
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
    struct student students[n];
    for (int i = 0 ; i < n ; i++)
    {
        scanf("%d", &students[i].rollNo);
        scanf("%s", students[i].name);
        for (int j = 0 ; j < 3 ; j++)
        {
            scanf("%d", &students[i].marks[j]);
            if (students[i].marks[j] < 0 || students[i].marks[j] > 100)
            {
            printf("Invalid marks. Marks must be between 0 and 100.\n");
            return 1;
            }
        }
    }

    for (int i = 0 ; i < n ; i++)
    {
        printf("Roll: %d\n", students[i].rollNo);
        printf("Name: %s\n", students[i].name);

        int total = calculateMarks(students[i].marks);
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
    printRollNumbers(students, n, 0);
    
    return 0;
}
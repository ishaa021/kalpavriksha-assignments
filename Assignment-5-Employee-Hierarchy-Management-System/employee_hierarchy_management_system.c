#include <stdio.h>
#include <stdbool.h>
#define MAX_EMPLOYEES 100
#define MAX_NAME_LENGTH 100
#define MAX_DESIGNATION_LENGTH 100

struct employee
{
    int employeeId;
    char name[MAX_NAME_LENGTH];
    char designation[MAX_DESIGNATION_LENGTH];
    int reports[MAX_EMPLOYEES];
    int reportingManagerId;
    int reportsCount;
};

void updateManagerReport(int employeeId, int reportingManagerId, struct employee employee[], int numberOfEmployees)
{
    if (reportingManagerId == -1)
    {
        return;
    }
    for (int j = 0; j < numberOfEmployees; j++)
    {
        if(reportingManagerId == employee[j].employeeId)
        {
            employee[j].reports[employee[j].reportsCount] = employeeId;
            employee[j].reportsCount++;
            return;
        }
    }
}

int getEmployeeIndex(int employeeId, int numberOfEmployees, struct employee employee[])
{
    for (int i = 0; i < numberOfEmployees; i++)
    {
        if (employee[i].employeeId == employeeId)
        {
            return i;
        }
    }
    return -1;
}

void transferReports(struct employee employee[], int numberOfEmployees, int employeeIndex, int targetIndex)
{
    for (int i = 0; i < employee[employeeIndex].reportsCount; i++)
    {
        int reportId = employee[employeeIndex].reports[i];

        int reportIndex = getEmployeeIndex(reportId, numberOfEmployees, employee);

        if (reportIndex != -1)
        {
            employee[reportIndex].reportingManagerId = employee[targetIndex].employeeId;
            employee[targetIndex].reports[employee[targetIndex].reportsCount] = reportId;
            employee[targetIndex].reportsCount++;
        }
    }
}

void displayHierarchyRecursive(struct employee employee[], int numberOfEmployees, int employeeIndex, int level, int *count)
{
    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }

    printf("%d - %s (%s)\n", employee[employeeIndex].employeeId, employee[employeeIndex].name, employee[employeeIndex].designation);

    for (int i = 0; i < employee[employeeIndex].reportsCount; i++)
    {
        int reportId = employee[employeeIndex].reports[i];

        int reportIndex = getEmployeeIndex(reportId, numberOfEmployees, employee);

        if (reportIndex != -1)
        {
            (*count)++;

            displayHierarchyRecursive( employee, numberOfEmployees, reportIndex, level + 1, count);
        }
    }
}

void addEmployee(struct employee employee[],int *noOfEmployees)
{
    static bool ceoExists = false;

    if (*noOfEmployees >= MAX_EMPLOYEES)
    {
        printf("\nEmployee limit reached. Cannot add more employees.\n");
        return;
    }

    int numberOfEmployees = *noOfEmployees;
    printf("\nEnter the Unique Employee ID: ");
    scanf("%d", &employee[numberOfEmployees].employeeId);

    while (getEmployeeIndex(employee[numberOfEmployees].employeeId,numberOfEmployees, employee) != -1)
    {
        printf("Employee with this ID already exists. Please enter the Unique Employee ID: ");
        scanf("%d", &employee[numberOfEmployees].employeeId);
    }
    
    printf("Enter the name of Employee: ");
    scanf("%99s", employee[numberOfEmployees].name);
    printf("Enter the designation of Employee: ");
    scanf("%99s", employee[numberOfEmployees].designation);

    employee[numberOfEmployees].reportsCount = 0;

    printf("Enter the Reporting Manager of the Employee (-1 if no manager): ");
    scanf("%d", &employee[numberOfEmployees].reportingManagerId);

    while ((employee[numberOfEmployees].reportingManagerId == -1 && ceoExists) || 
    (employee[numberOfEmployees].reportingManagerId != -1 && 
    getEmployeeIndex(employee[numberOfEmployees].reportingManagerId ,numberOfEmployees, employee) == -1))
    {
        printf("Employee with this ID does not exists. Please enter the existing Manager ID: ");
        scanf("%d", &employee[numberOfEmployees].reportingManagerId);
    }

    if (employee[numberOfEmployees].reportingManagerId == -1)
    {
        ceoExists = true;
    }

    updateManagerReport(employee[numberOfEmployees].employeeId,employee[numberOfEmployees].reportingManagerId,employee,numberOfEmployees);
    
    (*noOfEmployees)++;

    printf("\nEmployee added successfully.\n\n");
}

void removeEmployee(struct employee employee[], int *noOfEmployees)
{
    int employeeId;
    int numberOfEmployees = *noOfEmployees;

    printf("\nEnter the Employee ID to remove: ");
    scanf("%d", &employeeId);

    while(getEmployeeIndex(employeeId, numberOfEmployees, employee) == -1)
    {
        printf("\nEmployee with this ID does not exists. Please enter the existing Employee ID: ");
        scanf("%d", &employeeId);
    }

    int index = getEmployeeIndex(employeeId, numberOfEmployees, employee);

    if (employee[index].reportingManagerId == -1)
    {
        printf("\nThe CEO cannot be removed.\n");
        return;
    }
    
    int managerId = employee[index].reportingManagerId;
    int managerIndex = getEmployeeIndex(managerId, numberOfEmployees, employee);
    int siblingIndex = -1;
    int minimumReports = 101;

    for (int i = 0 ; i < employee[managerIndex].reportsCount ; i++)
    {
        int siblingId = employee[managerIndex].reports[i];
        if (siblingId != employeeId )
        {
            int currentSiblingIndex = getEmployeeIndex(siblingId, numberOfEmployees, employee);
            if (employee[currentSiblingIndex].reportsCount < minimumReports)
            {
                minimumReports = employee[currentSiblingIndex].reportsCount;
                siblingIndex = currentSiblingIndex;
            }                   
        }
    }
    int targetIndex;
    if (siblingIndex != -1)
    {
        targetIndex = siblingIndex;
    }
    else
    {
        targetIndex = managerIndex;
    }

    transferReports(employee, numberOfEmployees, index, targetIndex);

    int reportPosition = -1;

    for (int i = 0; i < employee[managerIndex].reportsCount; i++)
    {
        if (employee[managerIndex].reports[i] == employeeId)
        {
            reportPosition = i;
            break;
        }
    }

    if (reportPosition != -1)
    {
        for (int i = reportPosition ; i < employee[managerIndex].reportsCount - 1 ; i++)
        {
            employee[managerIndex].reports[i] = employee[managerIndex].reports[i + 1];
        }
        employee[managerIndex].reportsCount--;
    }

    for (int i = index; i < numberOfEmployees - 1; i++)
    {
        employee[i] = employee[i + 1];  
    }

    (*noOfEmployees)--;

    printf("\nEmployee removed successfully.\n");
}

void displayEmployeeHierarchy(struct employee employee[], int numberOfEmployees)
{
    if (numberOfEmployees == 0)
    {
        printf("\nNo employees available.\n");
        return;
    }

    int employeeId;

    printf("\nEnter the Employee ID to display their hierarchy: ");
    scanf("%d", &employeeId);

    int index = getEmployeeIndex(employeeId, numberOfEmployees, employee);

    if (index == -1)
    {
        printf("\nEmployee not found.\n");
        return;
    }

    int count = 0;

    printf("\nEmployee Hierarchy:\n");

    displayHierarchyRecursive(employee, numberOfEmployees, index, 0, &count);

    printf("\nTotal employees under %s: %d\n", employee[index].name, count);
}

void displayCompleteHierarchy(struct employee employee[], int numberOfEmployees)
{
    if (numberOfEmployees == 0)
    {
        printf("\nNo employees available.\n");
        return;
    }

    int ceoIndex = -1;

    for (int i = 0; i < numberOfEmployees; i++)
    {
        if (employee[i].reportingManagerId == -1)
        {
            ceoIndex = i;
            break;
        }
    }

    if (ceoIndex == -1)
    {
        printf("\nCEO not found.\n");
        return;
    }

    int count = 0;

    printf("\nComplete Employee Hierarchy:\n");

    displayHierarchyRecursive(employee, numberOfEmployees, ceoIndex, 0, &count);

    printf("\nTotal employees in the organization: %d\n", count + 1);
}

int main()
{
    struct employee employee[MAX_EMPLOYEES];
    int numberOfEmployees = 0;
    int choice = 0;

    while (choice != 5)
    {
        printf("===== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Display Complete Hierarchy\n");
        printf("5. Exit\n");
        printf("\nEnter your choice:");
        scanf("%d" ,&choice);

        switch(choice)
        {
            case 1:
                addEmployee(employee, &numberOfEmployees);
                break;
            
            case 2:
                removeEmployee(employee, &numberOfEmployees);
                break;
            
            case 3:
                displayEmployeeHierarchy(employee, numberOfEmployees);
                break;

            case 4:
                displayCompleteHierarchy(employee, numberOfEmployees);
                break;

            case 5:
                printf("\nExiting EMPLOYEE HIERARCHY MANAGEMENT SYSTEM");
                break;
            
            default:
                printf("Invalid choice\n\n");               
        }
    }
    return 0;
}
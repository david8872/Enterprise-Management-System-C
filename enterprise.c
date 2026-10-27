#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- MODULE 4: ENUM, UNION, AND STRUCT --- */

/* Enum for department selection */
typedef enum {
    HR = 1,
    SALES,
    TECH
} Dept;

/* Union: can store either an ID number OR a short code tag */
typedef union {
    int tagNumber;
    char tagCode[10];
} Tag;

/* Structure for an Employee */
typedef struct {
    int id;
    char name[30];
    Dept department;
    float salary;
    Tag empTag;
} Employee;

/* Dynamic array pointer and record tracker */
Employee *staff = NULL;
int totalEmployees = 0;

/* --- MODULE 3: FUNCTIONS (CALL-BY-REFERENCE & RECURSION) --- */

/* Function using Call-by-Reference: updates salary directly in memory */
void giveBonus(float *sal, float bonusAmount) {
    *sal = *sal + bonusAmount;
}

/* Recursive function: calculates total salary by summing one by one */
float sumSalaryRecursive(int count) {
    if (count <= 0) {
        return 0;
    }
    return staff[count - 1].salary + sumSalaryRecursive(count - 1);
}

/* --- MODULE 5: FILE HANDLING (READ & WRITE) --- */

void saveToFile(void) {
    FILE *fp = fopen("employees.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    fprintf(fp, "%d\n", totalEmployees);
    for (int i = 0; i < totalEmployees; i++) {
        fprintf(fp, "%d %s %d %.2f %d\n", 
                staff[i].id, 
                staff[i].name, 
                staff[i].department, 
                staff[i].salary, 
                staff[i].empTag.tagNumber);
    }
    fclose(fp);
    printf("Records successfully saved to 'employees.txt'.\n");
}

void loadFromFile(void) {
    FILE *fp = fopen("employees.txt", "r");
    if (fp == NULL) {
        return; /* File doesn't exist yet on first execution */
    }

    int count = 0;
    if (fscanf(fp, "%d", &count) == 1 && count > 0) {
        totalEmployees = count;
        staff = (Employee *)malloc(totalEmployees * sizeof(Employee));
        for (int i = 0; i < totalEmployees; i++) {
            fscanf(fp, "%d %s %d %f %d", 
                   &staff[i].id, 
                   staff[i].name, 
                   (int *)&staff[i].department, 
                   &staff[i].salary, 
                   &staff[i].empTag.tagNumber);
        }
        printf("Loaded %d record(s) from persistent storage.\n", totalEmployees);
    }
    fclose(fp);
}

/* --- MODULE 1 & 2: USER INPUT, CONTROL LOOPS, AND STRINGS --- */

void addEmployee(void) {
    int newCount = totalEmployees + 1;

    /* Dynamic memory reallocation */
    Employee *temp = (Employee *)realloc(staff, newCount * sizeof(Employee));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    staff = temp;

    printf("\nEnter Employee ID: ");
    scanf("%d", &staff[totalEmployees].id);

    printf("Enter First Name: ");
    scanf("%29s", staff[totalEmployees].name);

    printf("Select Dept (1: HR, 2: Sales, 3: Tech): ");
    int dChoice;
    scanf("%d", &dChoice);
    if (dChoice < 1 || dChoice > 3) dChoice = 1;
    staff[totalEmployees].department = (Dept)dChoice;

    printf("Enter Base Salary: ");
    scanf("%f", &staff[totalEmployees].salary);

    printf("Enter Numeric Tag ID: ");
    scanf("%d", &staff[totalEmployees].empTag.tagNumber);

    totalEmployees = newCount;
    printf("Employee record successfully added!\n");
}

void displayEmployees(void) {
    if (totalEmployees == 0) {
        printf("\nNo records available to display.\n");
        return;
    }

    printf("\n---------------- EMPLOYEE DIRECTORY ----------------\n");
    for (int i = 0; i < totalEmployees; i++) {
        char deptName[10];
        if (staff[i].department == HR) strcpy(deptName, "HR");
        else if (staff[i].department == SALES) strcpy(deptName, "Sales");
        else strcpy(deptName, "Tech");

        printf("ID: %-4d | Name: %-12s | Dept: %-6s | Salary: %-9.2f | Tag: %d\n",
               staff[i].id, 
               staff[i].name, 
               deptName, 
               staff[i].salary, 
               staff[i].empTag.tagNumber);
    }

    float totalPayroll = sumSalaryRecursive(totalEmployees);
    printf("----------------------------------------------------\n");
    printf("Total Enterprise Payroll (via Recursion): INR %.2f\n", totalPayroll);
}

void giveBonusMenu(void) {
    int id, found = 0;
    float bonus;

    printf("\nEnter Employee ID to award bonus: ");
    scanf("%d", &id);

    for (int i = 0; i < totalEmployees; i++) {
        if (staff[i].id == id) {
            printf("Enter Bonus Amount: ");
            scanf("%f", &bonus);

            /* Call-by-reference using address operator & */
            giveBonus(&staff[i].salary, bonus);

            printf("Bonus updated. New Salary: %.2f\n", staff[i].salary);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Employee ID not found.\n");
    }
}

/* --- MAIN INTERACTIVE EVENT LOOP --- */

int main(void) {
    loadFromFile();

    int choice = 0;
    while (choice != 5) {
        printf("\n=============================\n");
        printf("  ENTERPRISE SYSTEM MENU     \n");
        printf("=============================\n");
        printf("1. Add Employee\n");
        printf("2. Display All & Payroll\n");
        printf("3. Give Bonus (Call-by-Reference)\n");
        printf("4. Save to File\n");
        printf("5. Exit\n");
        printf("-----------------------------\n");
        printf("Choose an option: ");
        
        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                displayEmployees();
                break;
            case 3:
                giveBonusMenu();
                break;
            case 4:
                saveToFile();
                break;
            case 5:
                saveToFile();
                printf("Exiting system.\n");
                break;
            default:
                printf("Invalid selection! Try again.\n");
        }
    }

    if (staff != NULL) {
        free(staff);
        staff = NULL;
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPLOYEE_FILE "employeeInfo.dat"

typedef struct {
    int id;
    char name[100];
    char designation[30];
    float salary;
    char gender[10];
    char branch[50];
    char address[200];
    char phone[15];
    char email[50];
} Employee;

// Function declarations
void addEmployee(FILE *file);
void displayEmployees(FILE *file);
void searchEmployee(FILE *file);
void modifyEmployee(FILE *file);
void deleteEmployee(FILE *file);
void filterByGender(FILE *file);
void filterByBranch(FILE *file);
void filterBySalaryRange(FILE *file);
void printHeader(const char *title);
void pause();

int main() {
    FILE *file;
    int choice;

    // Open the file in binary read-write mode; create if it doesn't exist
    if ((file = fopen(EMPLOYEE_FILE, "rb+")) == NULL) {
        file = fopen(EMPLOYEE_FILE, "wb+");
        if (!file) {
            perror("Unable to open file");
            return 1;
        }
    }

    while (1) {
        printHeader("Main Menu");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee by ID\n");
        printf("4. Modify Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Filter Employees by Gender\n");
        printf("7. Filter Employees by Branch\n");
        printf("8. Filter Employees by Salary Range\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addEmployee(file); break;
            case 2: displayEmployees(file); break;
            case 3: searchEmployee(file); break;
            case 4: modifyEmployee(file); break;
            case 5: deleteEmployee(file); break;
            case 6: filterByGender(file); break;
            case 7: filterByBranch(file); break;
            case 8: filterBySalaryRange(file); break;
            case 0: fclose(file); exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Function to print the header
void printHeader(const char *title) {
    system("cls"); // Clear screen (use "clear" for Linux/macOS)
    printf("========================\n");
    printf("%s\n", title);
    printf("========================\n");
}

// Function to pause the system and wait for user input
void pause() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

// Function to add an employee
void addEmployee(FILE *file) {
    Employee emp;
    fseek(file, 0, SEEK_END); // Move to the end of the file

    printf("Enter Employee ID: ");
    scanf("%d", &emp.id);
    printf("Enter Name: ");
    getchar(); // Clear newline buffer
    fgets(emp.name, sizeof(emp.name), stdin);
    strtok(emp.name, "\n"); // Remove newline character
    printf("Enter Designation: ");
    fgets(emp.designation, sizeof(emp.designation), stdin);
    strtok(emp.designation, "\n");
    printf("Enter Gender: ");
    fgets(emp.gender, sizeof(emp.gender), stdin);
    strtok(emp.gender, "\n");
    printf("Enter Branch: ");
    fgets(emp.branch, sizeof(emp.branch), stdin);
    strtok(emp.branch, "\n");
    printf("Enter Address: ");
    fgets(emp.address, sizeof(emp.address), stdin);
    strtok(emp.address, "\n");
    printf("Enter Phone: ");
    scanf("%s", emp.phone);
    printf("Enter Email: ");
    scanf("%s", emp.email);
    printf("Enter Salary: ");
    scanf("%f", &emp.salary);

    fwrite(&emp, sizeof(emp), 1, file);
    printf("Employee added successfully!\n");
    pause();
}

// Function to display all employees
void displayEmployees(FILE *file) {
    Employee emp;
    rewind(file); // Move to the beginning of the file

    printHeader("Employee List");
    printf("%-5s %-20s %-15s %-10s %-10s %-15s\n", "ID", "Name", "Designation", "Gender", "Branch", "Salary");
    printf("---------------------------------------------------------------------------------\n");

    while (fread(&emp, sizeof(emp), 1, file)) {
        printf("%-5d %-20s %-15s %-10s %-10s %.2f\n", emp.id, emp.name, emp.designation, emp.gender, emp.branch, emp.salary);
    }

    pause();
}

// Function to search for an employee by ID
void searchEmployee(FILE *file) {
    int id, found = 0;
    Employee emp;

    printf("Enter Employee ID to search: ");
    scanf("%d", &id);

    rewind(file); // Move to the beginning of the file
    while (fread(&emp, sizeof(emp), 1, file)) {
        if (emp.id == id) {
            found = 1;
            printf("Employee Found:\n");
            printf("ID: %d\nName: %s\nDesignation: %s\nGender: %s\nBranch: %s\nAddress: %s\nPhone: %s\nEmail: %s\nSalary: %.2f\n",
                   emp.id, emp.name, emp.designation, emp.gender, emp.branch, emp.address, emp.phone, emp.email, emp.salary);
            break;
        }
    }

    if (!found) {
        printf("No employee found with ID %d\n", id);
    }
    pause();
}

// Function to modify an employee record
void modifyEmployee(FILE *file) {
    int id, found = 0;
    Employee emp;

    printf("Enter Employee ID to modify: ");
    scanf("%d", &id);

    rewind(file); // Move to the beginning of the file
    while (fread(&emp, sizeof(emp), 1, file)) {
        if (emp.id == id) {
            found = 1;
            printf("Enter new details for Employee ID %d:\n", id);
            printf("Enter Name: ");
            getchar();
            fgets(emp.name, sizeof(emp.name), stdin);
            strtok(emp.name, "\n");
            printf("Enter Designation: ");
            fgets(emp.designation, sizeof(emp.designation), stdin);
            strtok(emp.designation, "\n");
            printf("Enter Gender: ");
            fgets(emp.gender, sizeof(emp.gender), stdin);
            strtok(emp.gender, "\n");
            printf("Enter Branch: ");
            fgets(emp.branch, sizeof(emp.branch), stdin);
            strtok(emp.branch, "\n");
            printf("Enter Address: ");
            fgets(emp.address, sizeof(emp.address), stdin);
            strtok(emp.address, "\n");
            printf("Enter Phone: ");
            scanf("%s", emp.phone);
            printf("Enter Email: ");
            scanf("%s", emp.email);
            printf("Enter Salary: ");
            scanf("%f", &emp.salary);

            fseek(file, -sizeof(emp), SEEK_CUR);
            fwrite(&emp, sizeof(emp), 1, file);
            printf("Employee record updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("No employee found with ID %d\n", id);
    }
    pause();
}

// Function to delete an employee record
void deleteEmployee(FILE *file) {
    FILE *temp;
    int id, found = 0;
    Employee emp;

    temp = fopen("temp.dat", "wb");
    if (!temp) {
        perror("Unable to open temporary file");
        return;
    }

    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);

    rewind(file);
    while (fread(&emp, sizeof(emp), 1, file)) {
        if (emp.id == id) {
            found = 1;
            printf("Employee ID %d deleted successfully!\n", id);
            continue;
        }
        fwrite(&emp, sizeof(emp), 1, temp);
    }

    fclose(file);
    fclose(temp);

    remove(EMPLOYEE_FILE);
    rename("temp.dat", EMPLOYEE_FILE);

    file = fopen(EMPLOYEE_FILE, "rb+");
    if (!found) {
        printf("No employee found with ID %d\n", id);
    }
    pause();
}

// Function to filter employees by gender
void filterByGender(FILE *file) {
    Employee emp;
    char gender[10];

    printf("Enter Gender to filter (e.g., Male/Female): ");
    scanf("%s", gender);

    rewind(file); // Move to the beginning of the file

    printHeader("Employees by Gender");
    printf("%-5s %-20s %-15s %-10s %-10s %-15s\n", "ID", "Name", "Designation", "Gender", "Branch", "Salary");
    printf("---------------------------------------------------------------------------------\n");

    while (fread(&emp, sizeof(emp), 1, file)) {
        if (strcmp(emp.gender, gender) == 0) {
            printf("%-5d %-20s %-15s %-10s %-10s %.2f\n", emp.id, emp.name, emp.designation, emp.gender, emp.branch, emp.salary);
        }
    }

    pause();
}

// Function to filter employees by branch
void filterByBranch(FILE *file) {
    Employee emp;
    char branch[50];

    printf("Enter Branch to filter: ");
    getchar();
    fgets(branch, sizeof(branch), stdin);
    strtok(branch, "\n");

    rewind(file); // Move to the beginning of the file

    printHeader("Employees by Branch");
    printf("%-5s %-20s %-15s %-10s %-10s %-15s\n", "ID", "Name", "Designation", "Gender", "Branch", "Salary");
    printf("---------------------------------------------------------------------------------\n");

    while (fread(&emp, sizeof(emp), 1, file)) {
        if (strcmp(emp.branch, branch) == 0) {
            printf("%-5d %-20s %-15s %-10s %-10s %.2f\n", emp.id, emp.name, emp.designation, emp.gender, emp.branch, emp.salary);
        }
    }

    pause();
}

// Function to filter employees by salary range
void filterBySalaryRange(FILE *file) {
    Employee emp;
    float minSalary, maxSalary;

    printf("Enter minimum salary: ");
    scanf("%f", &minSalary);
    printf("Enter maximum salary: ");
    scanf("%f", &maxSalary);

    rewind(file); // Move to the beginning of the file

    printHeader("Employees by Salary Range");
    printf("%-5s %-20s %-15s %-10s %-10s %-15s\n", "ID", "Name", "Designation", "Gender", "Branch", "Salary");
    printf("---------------------------------------------------------------------------------\n");

    while (fread(&emp, sizeof(emp), 1, file)) {
        if (emp.salary >= minSalary && emp.salary <= maxSalary) {
            printf("%-5d %-20s %-15s %-10s %-10s %.2f\n", emp.id, emp.name, emp.designation, emp.gender, emp.branch, emp.salary);
        }
    }

    pause();
}
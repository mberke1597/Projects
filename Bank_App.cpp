#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LIMIT 1000

typedef struct {
    int customer_id;
    char first_name[MAX_LIMIT];
    char last_name[MAX_LIMIT];
    double balance;
    char currency[MAX_LIMIT];
} Customer;

void write_receipt(FILE *fptr, const char *operation, Customer *customer) {
    time_t t;
    struct tm *tm_info;
    char time_str[26];

    // Get current time
    time(&t);
    tm_info = localtime(&t);
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    // Write formatted receipt to file
    fprintf(fptr, "--------------------------------------------\n");
    fprintf(fptr, "               Bank Receipt                 \n");
    fprintf(fptr, "--------------------------------------------\n");
    fprintf(fptr, "Date: %s\n", time_str);
    fprintf(fptr, "Operation: %s\n", operation);
    fprintf(fptr, "--------------------------------------------\n");
    fprintf(fptr, "Customer ID: %d\n", customer->customer_id);
    fprintf(fptr, "Name: %s %s\n", customer->first_name, customer->last_name);
    fprintf(fptr, "Balance: %.2lf %s\n", customer->balance, customer->currency);
    fprintf(fptr, "--------------------------------------------\n");
}

int main() {
    Customer *customers = NULL;
    int customer_count = 0;
    char choice;
    double num;
    int order;
    FILE *fptr;

    printf("Welcome to the Bank App!\n");

    // Adding Customer
    do {
        customers = (Customer *)realloc(customers, (customer_count + 1) * sizeof(Customer));
        if (customers == NULL) {
            printf("Memory allocation error! Program terminating.\n");
            return 1;
        }

        printf("\nCustomer ID: ");
        scanf("%d", &customers[customer_count].customer_id);

        printf("Name: ");
        scanf(" %[^\n]", customers[customer_count].first_name);

        printf("Surname: ");
        scanf(" %[^\n]", customers[customer_count].last_name);

        printf("Account Balance: ");
        scanf("%lf", &customers[customer_count].balance);

        printf("Currency: ");
        scanf(" %[^\n]", customers[customer_count].currency);

        // Write receipt for account creation
        fptr = fopen("receipt.txt", "a");
        if (fptr == NULL) {
            printf("Error opening file for writing.\n");
            return 1;
        }
        write_receipt(fptr, "Account Creation", &customers[customer_count]);
        fclose(fptr);

        customer_count++;

        printf("Do you want to add another customer? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    printf("\n--- Added Customers ---\n");
    for (int i = 0; i < customer_count; i++) {
        printf("Customer No: %d, Name: %s %s, Balance: %.2lf %s\n",
               customers[i].customer_id,
               customers[i].first_name,
               customers[i].last_name,
               customers[i].balance,
               customers[i].currency);
    }

    // Deleting Customer
    printf("\nDo you want to delete a customer? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        printf("Enter the order number of the customer to delete: ");
        scanf("%d", &order);

        if (order >= 0 && order < customer_count) {
            // Write receipt for account deletion
            fptr = fopen("receipt.txt", "a");
            if (fptr == NULL) {
                printf("Error opening file for writing.\n");
                return 1;
            }
            write_receipt(fptr, "Account Deletion", &customers[order]);
            fclose(fptr);

            // Shift the remaining customers to delete the selected one
            for (int i = order; i < customer_count - 1; i++) {
                customers[i] = customers[i + 1];
            }
            customer_count--;
        } else {
            printf("Invalid customer order number.\n");
        }
    }

    printf("\n--- Remaining Customers ---\n");
    for (int i = 0; i < customer_count; i++) {
        printf("Customer No: %d, Name: %s %s, Balance: %.2lf %s\n",
               customers[i].customer_id,
               customers[i].first_name,
               customers[i].last_name,
               customers[i].balance,
               customers[i].currency);
    }

    // Updating Account's Balance
    printf("\nDo you want to change your balance? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        printf("For which customer (enter order number): ");
        scanf("%d", &order);

        if (order >= 0 && order < customer_count) {
            printf("Choose operation: Addition (A), Subtraction (S), Multiplication (M), Division (D): ");
            scanf(" %c", &choice);

            if (choice == 'A') {
                printf("Enter the number for addition: ");
                scanf("%lf", &num);
                customers[order].balance += num;
            } else if (choice == 'S') {
                printf("Enter the number for subtraction: ");
                scanf("%lf", &num);
                customers[order].balance -= num;
            } else if (choice == 'M') {
                printf("Enter the number for multiplication: ");
                scanf("%lf", &num);
                customers[order].balance *= num;
            } else if (choice == 'D') {
                printf("Enter the number for division: ");
                scanf("%lf", &num);
                if (num != 0) {
                    customers[order].balance /= num;
                } else {
                    printf("Error: Division by zero is not allowed.\n");
                }
            } else {
                printf("Invalid operation.\n");
            }

            // Write receipt for balance update
            fptr = fopen("receipt.txt", "a");
            if (fptr == NULL) {
                printf("Error opening file for writing.\n");
                return 1;
            }
            write_receipt(fptr, "Balance Update", &customers[order]);
            fclose(fptr);

            printf("Updated balance for customer %d: %.2lf\n", order, customers[order].balance);
        } else {
            printf("Invalid customer order.\n");
        }
    }

    free(customers);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct Transaction{
    char type[10]; // expense or income
    char method[10]; // credit, cash, debit
    double value; 
    int date; // day of the month
}; 

struct Transaction*  lineToTransaction(FILE *file);
void printReport(char *reportType);
void updateReport(struct Transaction transaction);
void deleteLine(int lineToDelete);

int getTransactionsNumber(){
    FILE *file;
    struct Transaction *transactions;
    int num = 0;

    file = fopen("data.txt", "r");
    transactions = lineToTransaction(file);
    while(transactions[num].date != 0) num++;
    fclose(file);
    return num;
}

int main(){
    
    char run[10] = "play";
    char menu;
    int intInput;
    double dbInput;
    struct Transaction transaction;
    
    while(strcmp(run, "quit") != 0) {
        printf("\n-------------------------------------\n");
        printf("====  MENU ====\n"
        "[1] - Add transaction\n"
        "[2] - Print full report\n"
        "[3] - Total income\n"
        "[4] - Total expenses\n"
        "[5] - Delete transaction\n"
        "[Q] - To quit\n");

        printf("Your choice: ");
        menu = _getch();
        printf("%c", menu);

        if(menu == 'Q' || menu == 'q'){
            strcpy(run, "quit");        
        }
        if(menu == '1'){  // New transactio menu
            menu = '\0';
            printf("\n----- New Transaction -----\n"
            "Date: ");
            while (1) {
                if (scanf("%d", &intInput) == 1 && intInput >= 1 && intInput <= 31) {
                    transaction.date = intInput;
                    break; 
                } else {
                    printf("Invalid input. Please enter a valid integer between 1 and 31: ");
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                }
            }
            printf("-- Type --\n[1] - Income\n[2] - Expense\nTransaction Type: ");
            while(1){
                menu = _getch();
                printf("%c", menu);
                if(menu == '1' || menu == '2' ){
                    break;
                } else{
                    printf(" ==> Invalid input!\nTransaction type: ");
                    menu = '\0';
                }
            }
            if(menu =='1') strcpy(transaction.type, "income") ;
            else strcpy(transaction.type, "expense");
            printf("\nTransaction type = %s", transaction.type);
            menu = '\0';

            printf("\n-- Method --\n[1] - Cash\n[2] - Credit\n[3] - Debit\n[4] - Other\nTransaction Method: ");
            while(1){
                menu = _getch();
                printf("%c", menu);
                if(menu == '1' || menu == '2' || menu == '3' || menu == '4'){
                    break;
                }else{
                    printf(" ==> Invalid input\n Transaction Method: ");
                    menu = '\0';
                }
            }
            switch (menu){
                case '1':
                    strcpy(transaction.method, "cash");
                    break;
                case '2':
                    strcpy(transaction.method, "credit");
                    break;
                case '3':
                    strcpy(transaction.method, "debit");
                    break;
                case '4':
                    strcpy(transaction.method, "other");
                    break;
                default:    
                    printf("Invalid input! Swtich is broken");
            }

            menu = '\0';
            printf("\n-- Transaction Value --\nValue: ");
            while(1){
                if(scanf("%lf", &dbInput) == 1){
                    transaction.value = dbInput;
                    break;
                }else{
                    printf("Invalid input! Value: ");
                    int c;
                    while((c = getchar()) != '\n'&& c != EOF );
                }
            }
            printf("New transaction added!\nDate: %d, Type: %s, Method: %s, Value: R$ %.2f", transaction.date, transaction.type, 
            transaction.method, transaction.value); 

            updateReport(transaction);
        }
        if(menu == '2'){
            printReport("full");
        }
        if(menu == '3'){
            printReport("income");
        }
        if(menu == '4'){
            printReport("expense");
        }
        if(menu == '5'){
            int num = getTransactionsNumber();
            if(num == 0){
                printf("No transaction found!\n");
            }
            else{
                printReport("full");
                printf("\nType the transaction number that will be deleted: ");
                while (1) {
                    if (scanf("%d", &intInput) == 1 && intInput >= 1 && intInput <= num) {
                        deleteLine(intInput);
                        break; 
                    } else {
                        printf("Invalid input. Please enter a valid integer between 1 and %d: ",num);
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                    }
                }
            }
        }

    }
    return 0;
}

struct Transaction*  lineToTransaction(FILE *file){
    
    int numOfTransactions = 0;
    char buffer[100]; 
    struct Transaction *transactions = malloc(100 * sizeof(struct Transaction));

    if (transactions == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 100; i++) {
        transactions[i].date = 0;
        strcpy(transactions[i].type, "");
        strcpy(transactions[i].method, "");
        transactions[i].value = 0.0;
    }
    
    int i=0;
    while (fgets(buffer, sizeof(buffer), file) != NULL ) {
        if (strlen(buffer) <= 1) {
            continue;
        }
        char *token = strtok(buffer, ",");
        if(token != NULL){
            transactions[i].date = atoi(token);
        }
        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(transactions[i].type, token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(transactions[i].method, token);
        }   
        token = strtok(NULL, ",");
        if (token != NULL) {
            transactions[i].value = strtod(token, NULL);    
        }
        i++;
    }
    
    return transactions;
}

void printReport(char *reportType){ // total or expenses or income

    FILE *file;
    long size;
    struct Transaction *transactions;

    file = fopen("data.txt", "r");
    if(file == NULL){
        perror("Error on read!");
    }

    transactions = lineToTransaction(file);
    fclose(file);

    if( strcmp(reportType, "full") == 0){
        printf("\n --- FULL REPORT ---\n");
    } 
    else if( strcmp(reportType, "expense") == 0){
        printf("\n --- EXPENSES REPORT ---\n");
    }
    else{
        printf("\n --- INCOME REPORT ---\n");
    }
    
    double total = 0;
    for(int i = 0; i<100 != 0; i++){
        if(transactions[i].date == 0) {
            break;
        }

        if (strcmp(reportType, "full") == 0){
                printf("[%d] Date: %d, Type: %s, Method: %s, Value: R$%.2f\n",(i+1), transactions[i].date, transactions[i].type, transactions[i].method, transactions[i].value);
                if(strcmp(transactions[i].type,  "expense") == 0){
                    total = total - transactions[i].value;
                }else{
                    total = total + transactions[i].value;
                }
            }
        else if( strcmp(transactions[i].type, reportType) == 0){
            printf("Date: %d, Type: %s, Method: %s, Value: R$%.2f\n", transactions[i].date, transactions[i].type, transactions[i].method, transactions[i].value);
            total = total + transactions[i].value;
        }
    }

    printf("\nTotal %s: R$ %.2f",reportType, total);
    free(transactions);

}

void updateReport(struct Transaction newTransaction){
    FILE *file;
    struct Transaction *transactions;

    file = fopen("data.txt", "r");
    transactions = lineToTransaction(file);
    fclose(file);
        
    for(int i=0;i <100;i++){
        if(transactions[i].date == 0) {
            transactions[i] = newTransaction;
            break;
        }
    }

    file = fopen("data.txt", "w");
    if (file == NULL) {
        perror("Error opening data.txt for writing");
        free(transactions); // Free memory allocated for old transactions
        return;
    }

    for (int date=1; date<=31; date++){
        for(int i=0; transactions[i].date != 0 ;i++){
            if(transactions[i].date == date) {
                fprintf(file, "%d,%s,%s,%.2f\n", transactions[i].date, transactions[i].type, transactions[i].method, 
                transactions[i].value);
            }
        }
    }

    fprintf(file,"%d,%s,%s,%.2f\n",0,"null","null",0.0);
    fclose(file);
    free(transactions);

}

void deleteLine(int lineToDelete){
    FILE *file;
    struct Transaction *transactions;

    file = fopen("data.txt", "r");
    transactions = lineToTransaction(file);
    fclose(file);

    file = fopen("data.txt", "w");
    if (file == NULL) {
        perror("Error opening data.txt for deleteLine");
        free(transactions); // Free memory allocated for old transactions
        return;
    }

    for(int i=0; transactions[i].date != 0 ;i++){
        if(i != lineToDelete-1) {
            fprintf(file, "%d,%s,%s,%.2f\n", transactions[i].date, transactions[i].type, transactions[i].method, 
            transactions[i].value);
        }
        else{
            printf("\nDeleting TRANSACTION: [%d] Date: %d, Type: %s, Method: %s, Value: R$%.2f\n",(i+1), transactions[i].date, 
            transactions[i].type, transactions[i].method, transactions[i].value);
        }
    }

    fprintf(file,"%d,%s,%s,%.2f\n",0,"null","null",0.0);
    fclose(file);
    free(transactions);

}
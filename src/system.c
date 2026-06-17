#include "header.h"

int get_next_record_id(void) {
    FILE *file = fopen(RECORDS_FILE, "r");
    if (!file) return 0;
    int last_id = -1;
    Record r;
    while (fscanf(file, "%d %d %s %d %s %s %s %lf %s", 
                  &r.id, &r.user_id, r.name, &r.account_id, 
                  r.date_of_creation, r.country, r.phone, 
                  &r.balance, r.account_type) == 9) {
        last_id = r.id;
    }
    fclose(file);
    return last_id + 1;
}

void list_owned_accounts(int user_id) {
    FILE *file = fopen(RECORDS_FILE, "r");
    if (!file) {
        printf("\n[!] No accounts database found.\n");
        return;
    }
    Record r;
    int found = 0;
    printf("\n--- Your Registered Accounts ---\n");
    while (fscanf(file, "%d %d %s %d %s %s %s %lf %s", 
                  &r.id, &r.user_id, r.name, &r.account_id, 
                  r.date_of_creation, r.country, r.phone, 
                  &r.balance, r.account_type) == 9) {
        if (r.user_id == user_id) {
            printf("Account ID: %d | Type: %s | Balance: $%.2f\n", r.account_id, r.account_type, r.balance);
            found++;
        }
    }
    if (!found) printf("No accounts registered under your profile.\n");
    fclose(file);
}

void check_account_details(int account_id, int user_id) {
    FILE *file = fopen(RECORDS_FILE, "r");
    if (!file) return;

    Record r;
    int found = 0;
    while (fscanf(file, "%d %d %s %d %s %s %s %lf %s", 
                  &r.id, &r.user_id, r.name, &r.account_id, 
                  r.date_of_creation, r.country, r.phone, 
                  &r.balance, r.account_type) == 9) {
        if (r.account_id == account_id && r.user_id == user_id) {
            found = 1;
            printf("\n--- Account Details ---\n");
            printf("Account ID: %d\nCountry: %s\nPhone: %s\nBalance: $%.2f\nType: %s\nCreation Date: %s\n",
                   r.account_id, r.country, r.phone, r.balance, r.account_type, r.date_of_creation);

            // Calculate Interest Strategy
            int day;
            sscanf(r.date_of_creation, "%d/", &day);
            if (strcmp(r.account_type, "savings") == 0) {
                double interest = (r.balance * 0.07) / 12.0;
                printf("Interest Package: You will get $%.2f as interest on day %d of every month.\n", interest, day);
            } else if (strcmp(r.account_type, "fixed01") == 0) {
                double interest = r.balance * 0.04;
                printf("Interest Package: You will get $%.2f as interest on completion of 1 year.\n", interest);
            } else if (strcmp(r.account_type, "fixed02") == 0) {
                double interest = r.balance * 0.05 * 2;
                printf("Interest Package: You will get $%.2f as interest on completion of 2 years.\n", interest);
            } else if (strcmp(r.account_type, "fixed03") == 0) {
                double interest = r.balance * 0.08 * 3;
                printf("Interest Package: You will get $%.2f as interest on completion of 3 years.\n", interest);
            } else {
                printf("Interest Package: You will not get interests because the account is of type current.\n");
            }
            break;
        }
    }
    if (!found) printf("\n[!] Error: Account ID not found or unauthorized.\n");
    fclose(file);
}

int update_account_info(int account_id, int user_id, int choice, const char *new_value) {
    FILE *file = fopen(RECORDS_FILE, "r");
    FILE *tmp = fopen("data/records.tmp", "w");
    if (!file || !tmp) return -1;

    Record r;
    int successfully_updated = 0;
    while (fscanf(file, "%d %d %s %d %s %s %s %lf %s", 
                  &r.id, &r.user_id, r.name, &r.account_id, 
                  r.date_of_creation, r.country, r.phone, 
                  &r.balance, r.account_type) == 9) {
        if (r.account_id == account_id && r.user_id == user_id) {
            if (choice == 1) strcpy(r.country, new_value);
            else if (choice == 2) strcpy(r.phone, new_value);
            successfully_updated = 1;
        }
        fprintf(tmp, "%d %d %s %d %s %s %s %.2f %s\n", 
                r.id, r.user_id, r.name, r.account_id, 
                r.date_of_creation, r.country, r.phone, 
                r.balance, r.account_type);
    }
    fclose(file);
    fclose(tmp);
    
    remove(RECORDS_FILE);
    rename("data/records.tmp", RECORDS_FILE);
    return successfully_updated ? 0 : -2;
}

int make_transaction(int account_id, int user_id, int action_type, double amount) {
    FILE *file = fopen(RECORDS_FILE, "r");
    FILE *tmp = fopen("data/records.tmp", "w");
    if (!file || !tmp) return -1;

    Record r;
    int status = -2; // Account not found/unauthorized default
    while (fscanf(file, "%d %d %s %d %s %s %s %lf %s", 
                  &r.id, &r.user_id, r.name, &r.account_id, 
                  r.date_of_creation, r.country, r.phone, 
                  &r.balance, r.account_type) == 9) {
        if (r.account_id == account_id && r.user_id == user_id) {
            if (strncmp(r.account_type, "fixed", 5) == 0) {
                status = -3; // Prohibited operation on fixed configurations
            } else if (action_type == 1) { // Withdraw
                if (r.balance < amount) {
                    status = -4; // Insufficient Funds
                } else {
                    r.balance -= amount;
                    status = 0;
                }
            } else if (action_type == 2) { // Deposit
                r.balance += amount;
                status = 0;
            }
        }
        fprintf(tmp, "%d %d %s %d %s %s %s %.2f %s\n", 
                r.id, r.user_id, r.name, r.account_id, 
                r.date_of_creation, r.country, r.phone, 
                r.balance, r.account_type);
    }
    fclose(file);
    fclose(tmp);
    
    remove(RECORDS_FILE);
    rename("data/records.tmp", RECORDS_FILE);
    return status;
}

int remove_account(int account_id, int user_id) {
    FILE *file = fopen(RECORDS_FILE, "r");
    FILE *tmp = fopen("data/records.tmp", "w");
    if (!file || !tmp) return -1;

    Record r;
    int found = 0;
    while (fscanf(file, "%d %d %s %d %s %s %s %lf %s", 
                  &r.id, &r.user_id, r.name, &r.account_id, 
                  r.date_of_creation, r.country, r.phone, 
                  &r.balance, r.account_type) == 9) {
        if (r.account_id == account_id && r.user_id == user_id) {
            found = 1; // Explicitly drop record serialization write step
            continue;
        }
        fprintf(tmp, "%d %d %s %d %s %s %s %.2f %s\n", 
                r.id, r.user_id, r.name, r.account_id, 
                r.date_of_creation, r.country, r.phone, 
                r.balance, r.account_type);
    }
    fclose(file);
    fclose(tmp);
    
    remove(RECORDS_FILE);
    rename("data/records.tmp", RECORDS_FILE);
    return found ? 0 : -2;
}

int transfer_owner(int account_id, int current_user_id, const char *target_username) {
    // 1. Resolve Target User ID
    FILE *ufile = fopen(USERS_FILE, "r");
    if (!ufile) return -1;
    int target_uid = -1;
    int uid; char uname[MAX_STR], upass[MAX_STR];
    while (fscanf(ufile, "%d %s %s", &uid, uname, upass) == 3) {
        if (strcmp(uname, target_username) == 0) {
            target_uid = uid;
            break;
        }
    }
    fclose(ufile);
    if (target_uid == -1) return -5; // Destination user path unregistered

    // 2. Perform Ownership Swap Processing
    FILE *file = fopen(RECORDS_FILE, "r");
    FILE *tmp = fopen("data/records.tmp", "w");
    if (!file || !tmp) return -1;

    Record r;
    int updated = 0;
    while (fscanf(file, "%d %d %s %d %s %s %s %lf %s", 
                  &r.id, &r.user_id, r.name, &r.account_id, 
                  r.date_of_creation, r.country, r.phone, 
                  &r.balance, r.account_type) == 9) {
        if (r.account_id == account_id && r.user_id == current_user_id) {
            r.user_id = target_uid;
            strcpy(r.name, target_username);
            updated = 1;
        }
        fprintf(tmp, "%d %d %s %d %s %s %s %.2f %s\n", 
                r.id, r.user_id, r.name, r.account_id, 
                r.date_of_creation, r.country, r.phone, 
                r.balance, r.account_type);
    }
    fclose(file);
    fclose(tmp);
    
    remove(RECORDS_FILE);
    rename("data/records.tmp", RECORDS_FILE);
    return updated ? 0 : -2;
}
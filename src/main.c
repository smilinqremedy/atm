#include "header.h"

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void register_flow(void) {
    char name[MAX_STR], pass[MAX_STR];
    printf("\n=== Register Account ===\n");
    printf("Enter unique username: ");
    scanf("%99s", name);
    printf("Enter secure password: ");
    scanf("%99s", pass);
    
    int result = register_user(name, pass);
    if (result == 0) {
        printf("[+] Success: Registration complete. Please log in.\n");
    } else if (result == -1) {
        printf("[!] Error: Username unique restraint violation!\n");
    } else {
        printf("[!] Error: Database initialization fault.\n");
    }
}

void session_menu(User user) {
    int choice;
    while (1) {
        printf("\n=============================\n");
        printf(" Welcome, %s (ID: %d)\n", user.name, user.id);
        printf("=============================\n");
        printf("1. Register a New Bank Account\n");
        printf("2. Update Information of Existing Account\n");
        printf("3. Check Account Details & Interests\n");
        printf("4. Make Transaction (Deposit/Withdrawal)\n");
        printf("5. Remove Existing Account\n");
        printf("6. Transfer Ownership of Account\n");
        printf("7. View Owned Accounts Index\n");
        printf("8. Logout Session\n");
        printf("Selection choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        if (choice == 8) break;

        int acc_id, res;
        char buffer_val[MAX_STR];

        switch (choice) {
            case 1: {
                // Inline Account Generation Engine
                FILE *f = fopen(RECORDS_FILE, "a+");
                if (!f) break;
                Record r;
                r.id = get_next_record_id();
                r.user_id = user.id;
                strcpy(r.name, user.name);
                printf("Enter New Custom Account Reference Number: ");
                scanf("%d", &r.account_id);
                
                // Get Current Date securely via POSIX elements
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                sprintf(r.date_of_creation, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                
                printf("Enter Registration Country: ");
                scanf("%99s", r.country);
                printf("Enter Destination Telephone contact: ");
                scanf("%99s", r.phone);
                printf("Enter Starting Base Deposit ($): ");
                scanf("%lf", &r.balance);
                printf("Enter Account Profile Type (savings/current/fixed01/fixed02/fixed03): ");
                scanf("%99s", r.account_type);

                fprintf(f, "%d %d %s %d %s %s %s %.2f %s\n", 
                        r.id, r.user_id, r.name, r.account_id, 
                        r.date_of_creation, r.country, r.phone, 
                        r.balance, r.account_type);
                fclose(f);
                printf("[+] Bank Account registered successfully!\n");
                break;
            }
            case 2:
                printf("Enter Account ID to change variables: ");
                scanf("%d", &acc_id);
                printf("Which field do you want to modify? (1 for Country, 2 for Phone): ");
                int field_choice;
                scanf("%d", &field_choice);
                printf("Enter new update value: ");
                scanf("%99s", buffer_val);
                res = update_account_info(acc_id, user.id, field_choice, buffer_val);
                if (res == 0) printf("[+] Record successfully synchronized.\n");
                else printf("[!] Error: Access denied or entry not found.\n");
                break;
            case 3:
                printf("Enter target Account ID: ");
                scanf("%d", &acc_id);
                check_account_details(acc_id, user.id);
                break;
            case 4:
                printf("Enter target Account ID: ");
                scanf("%d", &acc_id);
                printf("Select Operation Type (1 for Withdraw, 2 for Deposit): ");
                int trans_type;
                scanf("%d", &trans_type);
                printf("Enter Dollar Value: ");
                double amount;
                scanf("%lf", &amount);
                res = make_transaction(acc_id, user.id, trans_type, amount);
                if (res == 0) printf("[+] Core ledger balancing finalized successfully.\n");
                else if (res == -3) printf("[!] Transaction Rejected: Fixed accounts cannot perform mutations before maturity date.\n");
                else if (res == -4) printf("[!] Transaction Rejected: Overdraft limit violation, insufficient capital.\n");
                else printf("[!] Error: Verification execution dropped.\n");
                break;
            case 5:
                printf("Enter Account ID targeted for structural removal: ");
                scanf("%d", &acc_id);
                res = remove_account(acc_id, user.id);
                if (res == 0) printf("[+] Profile purged from data storage matrix safely.\n");
                else printf("[!] Error: Verification fail.\n");
                break;
            case 6:
                printf("Enter Account ID for Ownership De-authorization: ");
                scanf("%d", &acc_id);
                printf("Enter incoming User profile username: ");
                scanf("%99s", buffer_val);
                res = transfer_owner(acc_id, user.id, buffer_val);
                if (res == 0) printf("[+] Ownership transferred successfully.\n");
                else if (res == -5) printf("[!] Target validation error: Destination username does not exist.\n");
                else printf("[!] Authorization failed.\n");
                break;
            case 7:
                list_owned_accounts(user.id);
                break;
            default:
                printf("[!] Choice unrecognizable.\n");
        }
    }
}

int main(void) {
    // Scaffold requirements data path
    system("mkdir -p data");
    
    int option;
    while (1) {
        printf("\n=====================================\n");
        printf("    ENTERPRISE SYSTEM LOGIN PORTAL    \n");
        printf("=====================================\n");
        printf("1. Login to Portal\n");
        printf("2. Register New User\n");
        printf("3. Exit System Engine\n");
        printf("Selection entry: ");
        if (scanf("%d", &option) != 1) {
            clear_input_buffer();
            continue;
        }

        if (option == 3) {
            printf("\nSystem safely spun down. Goodbye.\n");
            break;
        }

        if (option == 1) {
            char name[MAX_STR], pass[MAX_STR];
            User current_session_user;
            printf("Enter verification username: ");
            scanf("%99s", name);
            printf("Enter access credentials: ");
            scanf("%99s", pass);

            if (login_user(name, pass, &current_session_user)) {
                session_menu(current_session_user);
            } else {
                printf("[!] Alert: Credentials verification error.\n");
            }
        } else if (option == 2) {
            register_flow();
        }
    }
    return 0;
}
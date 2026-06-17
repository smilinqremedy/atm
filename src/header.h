#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_STR 100
#define USERS_FILE "data/users.txt"
#define RECORDS_FILE "data/records.txt"

// Data Structures
typedef struct {
    int id;
    char name[MAX_STR];
    char password[MAX_STR];
} User;

typedef struct {
    int id;
    int user_id;
    char name[MAX_STR];
    int account_id;
    char date_of_creation[MAX_STR];
    char country[MAX_STR];
    char phone[MAX_STR];
    double balance;
    char account_type[MAX_STR];
} Record;

// Authentication Modules
int register_user(const char *name, const char *password);
int login_user(const char *name, const char *password, User *logged_user);

// ATM Action Modules
void check_account_details(int account_id, int user_id);
int update_account_info(int account_id, int user_id, int choice, const char *new_value);
int make_transaction(int account_id, int user_id, int action_type, double amount);
int remove_account(int account_id, int user_id);
int transfer_owner(int account_id, int current_user_id, const char *target_username);
void list_owned_accounts(int user_id);

// Utilities
void clear_input_buffer(void);
int get_next_user_id(void);
int get_next_record_id(void);

#endif
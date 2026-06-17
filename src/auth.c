#include "header.h"

int get_next_user_id(void) {
    FILE *file = fopen(USERS_FILE, "r");
    if (!file) return 0;
    
    int id = 0, last_id = -1;
    char name[MAX_STR], pass[MAX_STR];
    while (fscanf(file, "%d %s %s", &id, name, pass) == 3) {
        last_id = id;
    }
    fclose(file);
    return last_id + 1;
}

int register_user(const char *name, const char *password) {
    FILE *file = fopen(USERS_FILE, "r");
    if (file) {
        int id;
        char existing_name[MAX_STR], pass[MAX_STR];
        while (fscanf(file, "%d %s %s", &id, existing_name, pass) == 3) {
            if (strcasecmp(existing_name, name) == 0) {
                fclose(file);
                return -1; // User already exists
            }
        }
        fclose(file);
    }

    file = fopen(USERS_FILE, "a+");
    if (!file) return -2; // Write Error

    int new_id = get_next_user_id();
    fprintf(file, "%d %s %s\n", new_id, name, password);
    fclose(file);
    return 0;
}

int login_user(const char *name, const char *password, User *logged_user) {
    FILE *file = fopen(USERS_FILE, "r");
    if (!file) return 0;

    int id;
    char existing_name[MAX_STR], pass[MAX_STR];
    while (fscanf(file, "%d %s %s", &id, existing_name, pass) == 3) {
        if (strcmp(existing_name, name) == 0 && strcmp(pass, password) == 0) {
            logged_user->id = id;
            strcpy(logged_user->name, existing_name);
            strcpy(logged_user->password, pass);
            fclose(file);
            return 1; // Authenticated
        }
    }
    fclose(file);
    return 0; // Failure
}
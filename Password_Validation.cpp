#include <ctype.h> // For tolower()

// Helper function to clear input buffer
void clear_input_buffer() {
    while(getchar() != '\n');
}

// Enhanced user login with validation
void user_login() {
    int attempts = 0;
    const int max_attempts = 3;
    
    while(attempts < max_attempts) {
        system("cls");
        printf("\n----------------------------------\n");
        printf("          USER LOGIN\n");
        printf("----------------------------------\n\n");
        
        // Get username with validation
        printf("?? Username: ");
        if(scanf("%19s", user) != 1) {
            printf("\n? Invalid input. Please try again.\n");
            clear_input_buffer();
            attempts++;
            getch();
            continue;
        }
        
        // Mask password input
        printf("?? Password: ");
        int i = 0;
        char ch;
        while((ch = getch()) != '\r' && i < 19) {
            if(ch == '\b' && i > 0) {
                printf("\b \b");
                i--;
                paswd[i] = '\0';
            }
            else if(isprint(ch)) {
                printf("*");
                paswd[i++] = ch;
            }
        }
        paswd[i] = '\0';
        printf("\n");
        
        // Validate credentials
        if(strcmp(admin_user, user) == 0 && strcmp(admin_pass, paswd) == 0) {
            printf("\n? Welcome, Administrator!\n");
            add_route();
            return;
        }
        
        FILE *fp = fopen("User.dat", "rb");
        if(fp == NULL) {
            printf("\n? System error. Please try again later.\n");
            getch();
            return;
        }
        
        int found = 0;
        struct user_detail u;
        while(fread(&u, sizeof(u), 1, fp) == 1) {
            if(strcmp(u.userid, user) == 0 && strcmp(u.passwd, paswd) == 0) {
                found = 1;
                break;
            }
        }
        fclose(fp);
        
        if(found) {
            printf("\n? Login successful! Welcome back, %s.\n", user);
            view_all_routes();
            booking();
            return;
        } else {
            attempts++;
            printf("\n? Invalid username or password. Attempts remaining: %d\n", max_attempts - attempts);
            
            if(attempts < max_attempts) {
                printf("Please try again...\n");
                getch();
            }
        }
    }
    
    printf("\n? Maximum login attempts reached. Please try again later.\n");
    getch();
}

// Enhanced agent login with validation
void agent_login() {
    int attempts = 0;
    const int max_attempts = 3;
    
    while(attempts < max_attempts) {
        system("cls");
        printf("\n----------------------------------\n");
        printf("         AGENT LOGIN\n");
        printf("----------------------------------\n\n");
        
        printf("?? Agent ID: ");
        if(scanf("%19s", agent) != 1) {
            printf("\n? Invalid input. Please try again.\n");
            clear_input_buffer();
            attempts++;
            getch();
            continue;
        }
        
        printf("?? Password: ");
        int i = 0;
        char ch;
        while((ch = getch()) != '\r' && i < 19) {
            if(ch == '\b' && i > 0) {
                printf("\b \b");
                i--;
                passwd[i] = '\0';
            }
            else if(isprint(ch)) {
                printf("*");
                passwd[i++] = ch;
            }
        }
        passwd[i] = '\0';
        printf("\n");
        
        // Validate credentials
        if(strcmp(admin_user, agent) == 0 && strcmp(admin_pass, passwd) == 0) {
            printf("\n? Welcome, Administrator!\n");
            add_route();
            return;
        }
        
        FILE *fp = fopen("Agent.dat", "rb");
        if(fp == NULL) {
            printf("\n? System error. Please try again later.\n");
            getch();
            return;
        }
        
        int found = 0;
        struct agent_details a;
        while(fread(&a, sizeof(a), 1, fp) == 1) {
            if(strcmp(a.agent_userid, agent) == 0 && strcmp(a.agent_passwd, passwd) == 0) {
                found = 1;
                break;
            }
        }
        fclose(fp);
        
        if(found) {
            printf("\n? Login successful! Welcome, %s.\n", agent);
            view_all_routes();
            booking();
            return;
        } else {
            attempts++;
            printf("\n? Invalid agent ID or password. Attempts remaining: %d\n", max_attempts - attempts);
            
            if(attempts < max_attempts) {
                printf("Please try again...\n");
                getch();
            }
        }
    }
    
    printf("\n? Maximum login attempts reached. Please try again later.\n");
    getch();
}

// Password validation for signup
int validate_password(const char *pass) {
    int has_upper = 0, has_lower = 0, has_digit = 0;
    
    if(strlen(pass) < 8) {
        printf("Password must be at least 8 characters long\n");
        return 0;
    }
    
    for(int i = 0; pass[i]; i++) {
        if(isupper(pass[i])) has_upper = 1;
        else if(islower(pass[i])) has_lower = 1;
        else if(isdigit(pass[i])) has_digit = 1;
    }
    
    if(!has_upper) {
        printf("Password must contain at least one uppercase letter\n");
        return 0;
    }
    if(!has_lower) {
        printf("Password must contain at least one lowercase letter\n");
        return 0;
    }
    if(!has_digit) {
        printf("Password must contain at least one digit\n");
        return 0;
    }
    
    return 1;
}

// Enhanced user signup with validation
void user_signup() {
    system("cls");
    printf("\n----------------------------------\n");
    printf("        USER REGISTRATION\n");
    printf("----------------------------------\n\n");
    
    struct user_detail new_user;
    memset(&new_user, 0, sizeof(new_user));
    
    // Name validation
    while(1) {
        printf("Enter your first name: ");
        if(scanf("%14s", new_user.name) == 1 && isalpha(new_user.name[0])) {
            break;
        }
        printf("Invalid name. Please enter letters only.\n");
        clear_input_buffer();
    }
    
    // Surname validation
    while(1) {
        printf("Enter your last name: ");
        if(scanf("%14s", new_user.surname) == 1 && isalpha(new_user.surname[0])) {
            break;
        }
        printf("Invalid surname. Please enter letters only.\n");
        clear_input_buffer();
    }
    
    // Mobile number validation
    while(1) {
        printf("Enter your mobile number: ");
        if(scanf("%ld", &new_user.mobile) == 1 && new_user.mobile > 1000000000) {
            break;
        }
        printf("Invalid mobile number. Please enter 10 digits.\n");
        clear_input_buffer();
    }
    
    // Check if userid already exists
    FILE *fp;
    do {
        printf("Choose a username: ");
        scanf("%19s", new_user.userid);
        
        fp = fopen("User.dat", "rb");
        int exists = 0;
        if(fp != NULL) {
            struct user_detail u;
            while(fread(&u, sizeof(u), 1, fp) == 1) {
                if(strcmp(u.userid, new_user.userid) == 0) {
                    exists = 1;
                    break;
                }
            }
            fclose(fp);
            
            if(exists) {
                printf("Username already taken. Please choose another.\n");
            }
        }
    } while(exists);
    
    // Password validation
    do {
        printf("Enter password (min 8 chars, mix of upper/lower/digits): ");
        int i = 0;
        char ch;
        while((ch = getch()) != '\r' && i < 19) {
            if(ch == '\b' && i > 0) {
                printf("\b \b");
                i--;
                new_user.passwd[i] = '\0';
            }
            else if(isprint(ch)) {
                printf("*");
                new_user.passwd[i++] = ch;
            }
        }
        new_user.passwd[i] = '\0';
        printf("\n");
    } while(!validate_password(new_user.passwd));
    
    // Confirm password
    char confirm_pass[20];
    do {
        printf("Confirm password: ");
        int i = 0;
        char ch;
        while((ch = getch()) != '\r' && i < 19) {
            if(ch == '\b' && i > 0) {
                printf("\b \b");
                i--;
                confirm_pass[i] = '\0';
            }
            else if(isprint(ch)) {
                printf("*");
                confirm_pass[i++] = ch;
            }
        }
        confirm_pass[i] = '\0';
        printf("\n");
        
        if(strcmp(new_user.passwd, confirm_pass) != 0) {
            printf("Passwords don't match. Please try again.\n");
        }
    } while(strcmp(new_user.passwd, confirm_pass) != 0);
    
    // Save to file
    fp = fopen("User.dat", "ab");
    if(fp == NULL) {
        printf("Error creating account. Please try again.\n");
        getch();
        return;
    }
    
    fwrite(&new_user, sizeof(new_user), 1, fp);
    fclose(fp);
    
    printf("\n? Registration successful! Welcome, %s.\n", new_user.userid);
    printf("You can now login with your credentials.\n");
    getch();
    
    reserve();
}

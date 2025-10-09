#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void write();
void reserve();
char user[20];
char paswd[20];
char admin_user[] = "admin";
char admin_pass[] = "admin";

struct agent_details {
    char agency_name[30];
    char agent_fname[10];
    char agent_lname[10];
    char agent_email[30];
    long int agent_mobile;
    char agent_userid[20];
    char agent_passwd[20];
} a;

struct Bus_details {
    int serial_no;
    char starts[100];
    char ends[100];
    char bus_number[10];
    int seat_avail;
    char bus_type[20];
} bd;

struct user_detail {
    char name[15];
    char surname[15];
    long int mobile;
    char userid[20];
    char passwd[20];
} u;

struct Passenger {
    char full_name[100];
    long int number;
    char email[50];
    char departure[100];
    char arrival[100];
    int PNR;
    char departure_date[11];
    char status[10]; /* "waiting" or "confirmed" */
    int bus_serial_no;	
};

void add_route();
void view_all_routes();
void read_ulogin(char user[], char paswd[]);
void user_agent_login();
void write_asign();
void agent_signup();
void write_usign();
void user_signup();
void write_passenger(struct Bus_details *bus);
void print_passenger_by_pnr(int pnr);
void search_passenger();
void booking();
void reserve();
int first_page();
void cancel_booking();
void booking_status();
void user_menu();
void admin_approve_bookings();

int generate_pnr() {
    return rand() % 1000 + 1000;
}

void agent_menu() {
    int choice;
    do {
        printf("\n=== Agent Menu ===\n");
        printf("1. Book a Ticket\n");
        printf("2. View Booking History\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input, enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch(choice) {
            case 1:
                booking(); // Function to book ticket
                break;
            case 2:
//                agent_view_booking_history(a.agent_userid);
                break;
            case 3:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice, try again.\n");
                break;
        }
    } while(choice != 3);
}




void view_booking_history(const char *userid) {
    FILE *fp = fopen("passenger.txt", "rb");
    if (!fp) {
        printf("No booking history found.\n");
        return;
    }
    struct Passenger p;
    int found = 0;
    printf("\n=== Booking History for %s ===\n", userid);
    while (fread(&p, sizeof(p), 1, fp) == 1) {
        // Assuming bookings store userid or email in p.email or p.booked_by
        if (strcmp(p.email, userid) == 0) {
            printf("PNR: %d | Name: %s | Status: %s | Date: %s | Route: %s to %s\n",
                    p.PNR, p.full_name, p.status, p.departure_date, p.departure, p.arrival);
            found = 1;
        }
    }
    if (!found) {
        printf("No booking history found.\n");
    }
    fclose(fp);
}

/*void agent_view_booking_history(const char *agent_id) {
    FILE *fp = fopen("passenger.txt", "rb");
    if (!fp) {
        printf("No booking history found.\n");
        return;
    }
    struct Passenger p;
    int found = 0;
    printf("\n=== Booking History for Agent: %s ===\n", agent_id);
    while (fread(&p, sizeof(p), 1, fp) == 1) {
        // Assuming agent ID is linked to bookings, e.g., stored in some field (for demonstration, assuming p.booked_by)
        // Add logic here as needed to link bookings to agent
        printf("PNR: %d | Name: %s | Status: %s | Date: %s | Route: %s to %s\n",
                p.PNR, p.full_name, p.status, p.departure_date, p.departure, p.arrival);
        found = 1;
    }
    if (!found) {
        printf("No booking history found for agent %s.\n", agent_id);
    }
    fclose(fp);
}*/


/* Helper function to check if string contains only alphabets and spaces */
int validate_alpha(const char *str) {
	int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

/* Helper function to validate bus number format BUS-XXX */
int validate_bus_number(const char *bus_number) {
    if (strlen(bus_number) != 7) return 0;
    if (strncmp(bus_number, "BUS-", 4) != 0) return 0;
	int i;
    for (i = 4; i < 7; i++) {
        if (!isdigit(bus_number[i])) return 0;
    }
    return 1;
}

/* Helper function to validate bus type AC, Non-AC, Luxury */
int validate_bus_type(const char *bus_type) {
    int len = strlen(bus_type);
    int i;
	for (i = 0; i < len; i++) {
        if (!isalpha(bus_type[i]) && bus_type[i] != '-') return 0;
    }
    /* Allow only lowercase "ac", "non-ac", "luxury" */
    char temp[20];
    strcpy(temp, bus_type);
    for (i = 0; temp[i]; i++) temp[i] = tolower(temp[i]);
    if (strcmp(temp, "ac") == 0 || strcmp(temp, "non-ac") == 0 || strcmp(temp, "luxury") == 0)
        return 1;
    return 0;
}

/* Helper function to validate date DD/MM/YYYY */
int validate_date(const char *date) {
    if (strlen(date) != 10) return 0;
    if (!isdigit(date[0]) || !isdigit(date[1]) || date[2] != '/' ||
        !isdigit(date[3]) || !isdigit(date[4]) || date[5] != '/' ||
        !isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8]) || !isdigit(date[9]))
        return 0;
    return 1;
}

/* Helper function to validate email format (simple check) */
int validate_email(const char *email) {
    const char *at = strchr(email, '@');
    if (!at) return 0;
    const char *dot = strrchr(email, '.');
    if (!dot) return 0;
    return 1;
}

/* Helper function to validate mobile number digits count */
int validate_mobile(long int number) {
    int count = 0;
    long int temp = number > 0 ? number : -number;
    while (temp > 0) {
        count++;
        temp /= 10;
    }
    return count >= 7 && count <= 15;
}

/* Helper function to validate PNR (positive integer) */
int validate_pnr(int pnr) {
    return pnr > 0;
}

void add_route() {
    FILE *fp;
    struct Bus_details bd;
    memset(&bd, 0, sizeof(bd));
    printf("\n=== ADD NEW BUS ROUTE ===\n");

    int last_serial = 0;
    fp = fopen("Bus_Details.dat", "rb");
    if (fp != NULL) {
        struct Bus_details temp;
        while (fread(&temp, sizeof(temp), 1, fp) == 1) {
            if (temp.serial_no > last_serial) last_serial = temp.serial_no;
        }
        fclose(fp);
    }
    bd.serial_no = last_serial + 1;

    while (1) {
        printf("Enter Bus Number (e.g., BUS-001): ");
        scanf("%9s", bd.bus_number);
        while (getchar() != '\n');
        if (validate_bus_number(bd.bus_number)) break;
        else printf("Invalid bus number! Format must be 'BUS-XXX' where X is digit.\n");
    }

    while (1) {
        printf("Enter Departure City: ");
        fgets(bd.starts, sizeof(bd.starts), stdin);
        bd.starts[strcspn(bd.starts, "\n")] = '\0';
        if (validate_alpha(bd.starts)) break;
        else printf("Invalid departure city! Only alphabets and spaces allowed.\n");
    }

    while (1) {
        printf("Enter Arrival City: ");
        fgets(bd.ends, sizeof(bd.ends), stdin);
        bd.ends[strcspn(bd.ends, "\n")] = '\0';
        if (validate_alpha(bd.ends)) break;
        else printf("Invalid arrival city! Only alphabets and spaces allowed.\n");
    }

    while (1) {
        printf("Enter Available Seats (1-100): ");
        if (scanf("%d", &bd.seat_avail) == 1 && bd.seat_avail >= 1 && bd.seat_avail <= 100) {
            while (getchar() != '\n');
            break;
        }
        else {
            printf("Invalid number! Enter an integer between 1 and 100.\n");
            while (getchar() != '\n');
        }
    }

    while (1) {
        char temp_type[20];
        printf("Enter Bus Type (AC/Non-AC/Luxury): ");
        fgets(temp_type, sizeof(temp_type), stdin);
        temp_type[strcspn(temp_type, "\n")] = '\0';
        if (validate_bus_type(temp_type)) {
            strcpy(bd.bus_type, temp_type);
            break;
        }
        else {
            printf("Invalid bus type! Allowed: AC, Non-AC, Luxury.\n");
        }
    }

    fp = fopen("Bus_Details.dat", "ab");
    if (fp == NULL) {
        perror("Failed to open bus details file for appending");
        return;
    }
    if (fwrite(&bd, sizeof(bd), 1, fp) != 1) {
        printf("Error writing bus route data.\n");
    }
    else {
        printf("Bus route added successfully.\n");
    }
    fclose(fp);
}

void view_all_routes() {
    FILE *fp = fopen("Bus_Details.dat", "rb");
    struct Bus_details bd;
    if (fp == NULL) {
        printf("No bus routes available. Add routes first.\n");
        return;
    }

    printf("\nAvailable Bus Routes:\n");
    printf("-----------------------------------------------------------------\n");
    printf("| %-5s | %-10s | %-15s | %-15s | %-5s | %-10s |\n", "S.No", "Bus No", "From", "To", "Seats", "Type");
    printf("-----------------------------------------------------------------\n");
    while (fread(&bd, sizeof(bd), 1, fp) == 1) {
        printf("| %-5d | %-10s | %-15s | %-15s | %-5d | %-10s |\n",
            bd.serial_no, bd.bus_number, bd.starts, bd.ends, bd.seat_avail, bd.bus_type);
    }
    printf("-----------------------------------------------------------------\n");
    fclose(fp);
}

void print_passenger_by_pnr(int pnr) {
    FILE *fp = fopen("passenger.txt", "rb");
    struct Passenger p;
    int found = 0;
    if (!fp) {
        printf("No passenger data found.\n");
        return;
    }
    while(fread(&p, sizeof(p), 1, fp) == 1) {
        if(p.PNR == pnr) {
            printf("Passenger Found:\n");
            printf("Full Name: %s\nMobile Number: %ld\nEmail: %s\nDeparture: %s\nArrival: %s\nDate: %s\nPNR: %d\nStatus: %s\n",
                   p.full_name, p.number, p.email, p.departure, p.arrival, p.departure_date, p.PNR, p.status);
            found = 1;
            break;
        }
    }
    if (!found) printf("No passenger with PNR %d found.\n", pnr);
    fclose(fp);
}

void search_passenger() {
    int pnr;
    printf("Enter PNR: ");
    if (scanf("%d", &pnr) != 1 || pnr <= 0) {
        printf("Invalid PNR entered.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    print_passenger_by_pnr(pnr);
}

void read_ulogin(char user[], char paswd[]) {
    FILE *fp = fopen("User.dat", "rb");
    if (fp == NULL) {
        printf("Error: Can't access user database\n");
        return;
    }

    if(strcmp(admin_user, user) == 0 && strcmp(admin_pass, paswd) == 0) {
        printf("You are Logged In as Admin\n");
        fclose(fp);
        int choice = 0;
        while (choice != 3) {
            printf("\nAdmin Menu:\n1) Add Bus Route\n2) Approve Bookings\n3) Logout\nEnter choice: ");
            if (scanf("%d", &choice) != 1) {
                while (getchar() != '\n');
                printf("Invalid input.\n");
                continue;
            }
            while (getchar() != '\n');
            switch(choice) {
                case 1: add_route(); break;
                case 2: admin_approve_bookings(); break;
                case 3: printf("Logging out admin.\n"); break;
                default: printf("Invalid choice.\n");
            }
        }
        return;  // Implement admin menu accordingly
        return;
    }
    int found = 0;
    while (fread(&u, sizeof(u), 1, fp) == 1) {
        if (strcmp(u.userid, user) == 0 && strcmp(u.passwd, paswd) == 0) {
            printf("You are Logged In as %s\n", user);
            found = 1;
            fclose(fp);
            view_all_routes();
            user_menu();
            return;
        }
    }
    fclose(fp);

    if (!found){
        FILE *fp_agent = fopen("Agent.dat", "rb");
        if (fp_agent == NULL) {
            printf("Can't Read Agent File\n");
            return;
        }
        int agent_found = 0;
        while (fread(&a, sizeof(a), 1, fp_agent) == 1) {
            if (strcmp(a.agent_userid, user) == 0 && strcmp(a.agent_passwd, paswd) == 0) {
                printf("You are Logged In as Agent: %s\n", a.agent_userid);
                agent_found = 1;
                fclose(fp_agent);
                view_all_routes();
                agent_menu();
                return;
            }
        }
        fclose(fp_agent);
        if (!agent_found) {
            printf("Incorrect User Id or Password\n");
        }
    }
}

/* Admin function to approve pending bookings */
void admin_approve_bookings() {
    FILE *fp = fopen("passenger.txt", "rb+");
    if (!fp) {
        printf("No passenger bookings to approve.\n");
        return;
    }
    struct Passenger p;
    int index = 0;
    int any_waiting = 0;
    while (fread(&p, sizeof(p), 1, fp) == 1) {
        if (strcmp(p.status, "waiting") == 0) {
            any_waiting = 1;
            printf("\nPNR: %d\nName: %s\nMobile: %ld\nEmail: %s\nRoute: %s to %s\nDate: %s\nStatus: %s\n",
                p.PNR, p.full_name, p.number, p.email, p.departure, p.arrival, p.departure_date, p.status);
            printf("Approve this booking? (Y/N): ");
            char choice;
            scanf(" %c", &choice);
            while(getchar() != '\n');
            if (choice == 'Y' || choice == 'y') {
                strcpy(p.status, "confirmed");
                fseek(fp, index * sizeof(struct Passenger), SEEK_SET);
                fwrite(&p, sizeof(p), 1, fp);
                fflush(fp);
                printf("Booking with PNR %d approved.\n", p.PNR);
//                update_bus_seat(p.bus_serial_no, 1);  // Correctly using bus_serial_no from Passenger
            }
        }
        index++;
    }
    if (!any_waiting) {
        printf("No bookings waiting for approval.\n");
    }
    fclose(fp);
}

/*void update_bus_seat(int serial_no, int seats_booked) {
    FILE *fp = fopen("Bus_Details.dat", "rb+");
    if (!fp) {
        printf("Error opening bus details file.\n");
        return;
    }
    struct Bus_details bd;
    while (fread(&bd, sizeof(bd), 1, fp) == 1) {
        if (bd.serial_no == serial_no) {
            bd.seat_avail -= seats_booked;
            if (bd.seat_avail < 0) bd.seat_avail = 0;  // avoid negative seats
            fseek(fp, -sizeof(bd), SEEK_CUR);
            fwrite(&bd, sizeof(bd), 1, fp);
            printf("Updated seat availability for Bus Number: %s\n", bd.bus_number);
            break;
        }
    }
    fclose(fp);
}*/

void user_agent_login() {
    printf("Enter Username: ");
    scanf("%19s", user);
    printf("Enter Password: ");
    scanf("%19s", paswd);
    while(getchar() != '\n');
    read_ulogin(user, paswd);
}

void write_asign() {
    FILE *fp = fopen("Agent.dat", "ab");
    if (!fp) {
        printf("Error opening agent file.\n");
        exit(1);
    }

    printf("Agency Name: ");
    scanf("%29s", a.agency_name);
    printf("Agent First Name: ");
    scanf("%9s", a.agent_fname);
    printf("Agent Last Name: ");
    scanf("%9s", a.agent_lname);
    printf("Mobile Number: ");
    while(scanf("%ld", &a.agent_mobile) != 1 || !validate_mobile(a.agent_mobile)) {
        printf("Invalid mobile number. Try again: ");
        while(getchar() != '\n');
    }
    printf("User ID: ");
    scanf("%19s", a.agent_userid);
    printf("Password: ");
    scanf("%19s", a.agent_passwd);
    while(getchar() != '\n');

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);
}

void agent_signup() {
    write_asign();
    reserve();
}

void write_usign() {
    FILE *fp = fopen("User.dat", "ab");
    if (!fp) {
        printf("Error opening user file.\n");
        return;
    }
    printf("First Name: ");
    scanf("%14s", u.name);
    printf("Last Name: ");
    scanf("%14s", u.surname);
    printf("Mobile Number: ");
    while(scanf("%ld", &u.mobile) != 1 || !validate_mobile(u.mobile)) {
        printf("Invalid mobile number. Try again: ");
        while(getchar() != '\n');
    }
    printf("User ID: ");
    scanf("%19s", u.userid);
    printf("Password: ");
    scanf("%19s", u.passwd);
    while(getchar() != '\n');
    fwrite(&u, sizeof(u), 1, fp);
    fclose(fp);
}

void user_signup() {
    write_usign();
    reserve();
}

void write_passenger(struct Bus_details* bus) {
    struct Passenger p;
    FILE *fp = fopen("passenger.txt", "ab");
    if (!fp) {
        printf("Error creating passenger file.\n");
        return;
    }

    printf("\nBooking on Bus Number: %s\nRoute: %s to %s\n\n", bus->bus_number, bus->starts, bus->ends);

    char add_more = 'Y';
    do {
        while(1) {
            printf("Departure Date (DD/MM/YYYY): ");
            fgets(p.departure_date, sizeof(p.departure_date), stdin);
            p.departure_date[strcspn(p.departure_date, "\n")] = '\0';
            if (validate_date(p.departure_date)) break;
            else printf("Invalid date format! Use DD/MM/YYYY.\n");
        }

        while(1) {
		    printf("Full Name: ");
		    int c;
		    while ((c = getchar()) != '\n' && c != EOF);  // clear buffer
		    fgets(p.full_name, sizeof(p.full_name), stdin);
		    p.full_name[strcspn(p.full_name, "\n")] = '\0';
		    if (validate_alpha(p.full_name)) break;
		    else printf("Invalid name! Alphabets and spaces only.\n");
		}


        printf("Mobile Number: ");
        while(scanf("%ld", &p.number) != 1 || !validate_mobile(p.number)) {
            printf("Invalid mobile number. Try again: ");
            while(getchar() != '\n');
        }
        while(getchar() != '\n');

        while (1) {
            printf("Email: ");
            fgets(p.email, sizeof(p.email), stdin);
            p.email[strcspn(p.email, "\n")] = '\0';
            if (validate_email(p.email)) break;
            else printf("Incorrect email format. Try again.\n");
        }

        strcpy(p.departure, bus->starts);
        strcpy(p.arrival, bus->ends);

        p.PNR = generate_pnr();
        printf("\n");
        strcpy(p.status, "waiting");
        printf("Passenger PNR: %d Status: %s\n", p.PNR, p.status);

        if (fwrite(&p, sizeof(p), 1, fp) != 1) {
            printf("Error writing passenger data.\n");
            fclose(fp);
            return;
        }

        printf("Add another passenger? (Y/N): ");
        scanf(" %c", &add_more);
        while(getchar() != '\n');
    } while(add_more == 'Y' || add_more == 'y');

    fclose(fp);
}

void booking() {
    FILE *fp = fopen("Bus_Details.dat", "rb");
    if (!fp) {
        printf("No bus routes available. Please contact admin.\n");
        return;
    }

    struct Bus_details bus_list[100];
    int route_count = 0;

    printf("\nAvailable Bus Routes:\n");
    printf("-------------------------------------------------------------\n");
    printf("| %-5s | %-10s | %-15s | %-15s | %-6s | %-10s |\n",
           "S.No", "Bus No.", "From", "To", "Seats", "Type");
    printf("-------------------------------------------------------------\n");

    while(fread(&bus_list[route_count], sizeof(struct Bus_details), 1, fp) == 1 && route_count < 100) {
        struct Bus_details *b = &bus_list[route_count];
        printf("| %-5d | %-10s | %-15s | %-15s | %-6d | %-10s |\n",
               b->serial_no, b->bus_number, b->starts, b->ends, b->seat_avail, b->bus_type);
        route_count++;
    }
    printf("-------------------------------------------------------------\n");
    fclose(fp);

    if (route_count == 0) {
        printf("No bus routes available for booking.\n");
        return;
    }

    int selected_serial;
    printf("Enter the Serial Number of the bus you want to book: ");
    if(scanf("%d", &selected_serial) != 1) {
        printf("Invalid input.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    int found = 0;
    struct Bus_details selected_bus;
    int i;
    for (i = 0; i < route_count; i++) {
        if (bus_list[i].serial_no == selected_serial) {
            selected_bus = bus_list[i];
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Invalid serial number chosen. Booking aborted.\n");
        return;
    }
    write_passenger(&selected_bus);
}

void user_menu() {
    int choice;
    do {
        printf("\n=== User Menu ===\n");
        printf("1. Book a Ticket\n");
        printf("2. Cancel Booking\n");
        printf("3. Check Booking Status\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');

        switch(choice) {
            case 1: booking(); break;
            case 2: cancel_booking(); break;
            case 3: booking_status(); break;
            case 4: printf("Logging out...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while(choice != 4);
}

void reserve() {
    int a;
    printf("=============================\n");
    printf("Press 1 for User/Agent Login\n");
    printf("Press 2 for User Sign Up\n");
    printf("Press 3 for Agent Sign Up\n");
    printf("Press 4 for Search Passenger by PNR\n");
    printf("Press 5 for Exit\n");
    printf("=============================\n\n");
    if(scanf("%d", &a) != 1) {
        printf("Invalid input.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    switch (a) {
        case 1: user_agent_login(); break;
        case 2: user_signup(); break;
        case 3: agent_signup(); break;
        case 4: search_passenger(); break;
        case 5: break;
        default: printf("Invalid choice. Returning.\n"); break;
    }
}

int first_page() {
    int choice;
    printf("============================\n");
    printf("Press 1 for Reservation\n");
    printf("Press 2 for Cancelation\n");
    printf("Press 3 for Booking Status\n");
    printf("Press 4 for Exit\n");
    printf("=============================\n\n");
    if(scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        while(getchar() != '\n');
        return 0;
    }
    while(getchar() != '\n');

    switch(choice) {
        case 1: reserve(); break;
        case 2: cancel_booking(); break;
        case 3: booking_status(); break;
        case 4: printf("Exiting...\n"); return choice;
        default: printf("Invalid choice. Try again.\n"); break;
    }
    return 0;
}

void cancel_booking() {
    int pnr;
    printf("Enter your PNR to cancel the booking: ");
    if(scanf("%d", &pnr) != 1) {
        printf("Invalid input.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    FILE *fp = fopen("passenger.txt", "rb");
    FILE *temp_fp = fopen("temp_passenger.txt", "wb");
    if (fp == NULL || temp_fp == NULL) {
        perror("Error opening files");
        if(fp) fclose(fp);
        if(temp_fp) fclose(temp_fp);
        return;
    }

    struct Passenger p;
    int found = 0;

    while (fread(&p, sizeof(p), 1, fp) == 1) {
        if (p.PNR == pnr) {
            found = 1;
            printf("Booking with PNR %d has been cancelled.\n", pnr);
        } else {
            fwrite(&p, sizeof(p), 1, temp_fp);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove("passenger.txt");
    rename("temp_passenger.txt", "passenger.txt");

    if (!found) {
        printf("No booking found with PNR %d.\n", pnr);
    }
}

void booking_status() {
    int pnr;
    printf("Enter your PNR to check the booking status: ");
    if(scanf("%d", &pnr) != 1) {
        printf("Invalid input.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    print_passenger_by_pnr(pnr);
}

int main() {
    printf("======================================================================\n");
    printf("||                                                                  ||\n");
    printf("||                 WELCOME TO BUS RESERVATION SYSTEM                ||\n");
    printf("||                                                                  ||\n");
    printf("======================================================================\n\n");

    while (first_page() != 4) {
        printf("============================\n");
    }

    return 0;
}

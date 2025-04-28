#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
void write();
void reserve();
char user[20];
char paswd[20];
char agent[20];
char passwd[20];
char admin_user[]="admin";
char admin_pass[]="admin";
void view_all_routes();
void booking();

struct agent_details {
    char agency_name[30];
    char agent_fname[10];
    char agent_lname[10];
    char agent_email[30];
    long int agent_mobile;
    char agent_userid[20];
    char agent_passwd[20];
} a;

struct Bus_details{
	int serial_no;
	char starts[100];
	char ends[100];
	char bus_number[10];
	int seat_avail;
	char bus_type[20];
}bd;

void add_route() {
    FILE *fp;
    struct Bus_details bd;
    
    // Clear the structure first
    memset(&bd, 0, sizeof(bd));

    printf("\n=== ADD NEW BUS ROUTE ===\n");

 int last_serial = 1;
    fp = fopen("Bus_Details.dat", "rb");
    if (fp != NULL) {
        struct Bus_details temp;
        while (fread(&temp, sizeof(temp), 1, fp) == 1) {
            last_serial = temp.serial_no;
        }
    }
    fclose(fp);
}
	bd.serial_no = last_serial + 1;
    while(1) {
        printf("Enter Bus Number (e.g., BUS-001): ");
        if(scanf("%10s", bd.bus_number) == 1) {  // Limit input to prevent overflow
            break;
        } else {
            printf("Invalid input. Please try again.\n");
            while(getchar() != '\n'); // Clear input buffer
        }
    }

    // Get departure location
    printf("Enter Departure City: ");
    scanf("%99s", bd.starts);  // Limit input to prevent overflow

    // Get arrival location
    printf("Enter Arrival City: ");
    scanf("%99s", bd.ends);    // Limit input to prevent overflow

    // Get available seats with validation
    while(1) {
        printf("Enter Available Seats (1-100): ");
        if(scanf("%d", &bd.seat_avail) == 1 && bd.seat_avail > 0 && bd.seat_avail <= 100) {
            break;
        } else {
            printf("Invalid input. Please enter a number between 1-100.\n");
            while(getchar() != '\n'); // Clear input buffer
        }
    }

    // Get bus type with validation
    while(1) {
        printf("Enter Bus Type (AC/Non-AC/Luxury): ");
        if(scanf("%19s", bd.bus_type) == 1) {  // Limit input to prevent overflow
            // Convert to lowercase for case-insensitive comparison
            char type[20];
            strcpy(type, bd.bus_type);
            int i;
            for(i = 0; type[i]; i++) {
                type[i] = tolower(type[i]);
            }
            
            if(strcmp(type, "ac") == 0 || strcmp(type, "non-ac") == 0 || strcmp(type, "luxury") == 0) {
                break;
            } else {
                printf("Invalid bus type. Please enter AC, Non-AC, or Luxury.\n");
            }
        } else {
            printf("Invalid input. Please try again.\n");
            while(getchar() != '\n'); // Clear input buffer
        }
    }

    // Open file in append binary mode
    fp = fopen("Bus_Details.dat", "ab");
    if(fp == NULL) {
        printf("\nError: Could not open database file for writing.\n");
        return;
    }

    // Write the structure to file
    if(fwrite(&bd, sizeof(struct Bus_details), 1, fp) != 1) {
        printf("\nError: Failed to write bus route to database.\n");
    } else {
        printf("\nBus route added successfully!\n");
        printf("Bus Number: %s\n", bd.bus_number);
        printf("Route: %s to %s\n", bd.starts, bd.ends);
        printf("Seats: %d | Type: %s\n\n", bd.seat_avail, bd.bus_type);
    }

    fclose(fp);
}


struct user_detail {
    char name[15];
    char surname[15];
    long int mobile;
    char userid[20];
    char passwd[20];
}u;
void read_ulogin(char user[], char paswd[]) {
    FILE *fp = fopen("User.dat", "ab+");
    if (fp == NULL) {
        printf("Error: Can't access user database\n");
        getch();
        return;
    }

    if(strcmp(admin_user, user) == 0 && strcmp(admin_pass, paswd) == 0) {
        printf("You are Logged In as Admin\n");
        fclose(fp);
        add_route();
        return;
    }
	int flag = 0;
	int found = 0;
	while (fread(&u, sizeof(u), 1, fp) == 1) {
		if (strcmp(u.userid, user) == 0 && strcmp(u.passwd, paswd) == 0) {
			printf("You are Logged In as %s\n", user);
		    found = 1;
		    flag = 1;
		    fclose(fp);
		    view_all_routes();
		    return;
		}
	}
	if (!found) {
	    printf("Incorrect User Id or Password\n");
	    flag = 1;
	} 
	fclose(fp);

    
	
	if(flag == 0){	
		FILE *fp;
	    fp = fopen("Agent.dat", "ab+");
	    if (fp == NULL) {
	        printf("Can't Read File\n");
	        getch();
	        return;
	    }
	    int found = 0;
	    
		while (fread(&a, sizeof(a), 1, fp) == 1) {
		    if (strcmp(a.agent_userid, user) == 0 && strcmp(a.agent_passwd, paswd) == 0) {
		        printf("You are Logged In as %s\n", agent);
		        found = 1;
		        view_all_routes();
		        booking();
		        break;
		    }
		}
		
	    if (!found) {
	        printf("Incorrect User Id or Password\n");
	    }
	    fclose(fp);
	}
		
}




void user_agent_login() {
    printf("Username = ");
    scanf("%s", user);
    printf("Password = ");
    scanf("%s", paswd);
   
    read_ulogin(user, paswd);
}




void write_asign() {
    FILE *fp;
    fp = fopen("Agent.dat", "ab");
    if (fp == NULL) {
        printf("Can't create a file");
        getch();
        exit(1);
    }


    printf("Enter your Agency Name : ");
    scanf("%s", a.agency_name);
    printf("Enter your first Name : ");
    scanf("%s", a.agent_fname);
    printf("Enter your last Name : ");
    scanf("%s", a.agent_lname);
    printf("Enter your Mobile number : ");
    scanf("%ld", &a.agent_mobile);
    printf("Enter your User-Id : ");
    scanf("%s", a.agent_userid);
    printf("Enter your Password : ");
    scanf("%s", a.agent_passwd);


    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);
}


void agent_signup(){
	write_asign();
	reserve();
}



void write_usign() {
    FILE *fp;
    fp = fopen("User.dat", "ab");
    if (fp == NULL) {
        printf("Can't create a file");
        getch();
    }
    printf("Enter your first Name : ");
    scanf("%s", u.name);
    printf("Enter your last Name : ");
    scanf("%s", u.surname);
    printf("Enter your Mobile number : ");
    fflush(stdin);
    scanf("%ld", &u.mobile);
    printf("Enter your User-Id : ");
    scanf("%s", u.userid);
    fflush(stdin);
    printf("Enter your Password : ");
    scanf("%s", u.passwd);
    fwrite(&u, sizeof(u), 1, fp);
    fclose(fp);
}


void user_signup() {
    write_usign();
    reserve();
}

struct Passenger {
    char full_name[100];
    int number;
    char email[50];
    char departure[100];
    char arrival[100];
    int PNR;
    char departure_date[11];
};

int generate_pnr() {
    return rand() % 1000000 + 1000000;
}
int max_passengers=100, count = 0;
void write_passenger() {
	struct Passenger p[max_passengers];

    FILE *fp;
    fp = fopen("passenger.txt", "a");
    if (fp == NULL) {
        printf("Can't create a file\n");
        return;
    }

    int add = 1;
    do {
        if (count >= max_passengers) {
            printf("Maximum passenger limit reached.\n");
            break;
        }
		while (getchar() != '\n');
		
        printf("Enter your Departure Location: ");
        fgets(p[count].departure, sizeof(p[count].departure), stdin);
        p[count].departure[strcspn(p[count].departure, "\n")] = '\0';  

        printf("Enter your Arrival Location: ");
        fgets(p[count].arrival, sizeof(p[count].arrival), stdin);
        p[count].arrival[strcspn(p[count].arrival, "\n")] = '\0';  

        printf("Enter your Departure Date (DD/MM/YYYY): ");
        fgets(p[count].departure_date, sizeof(p[count].departure_date), stdin);
        p[count].departure_date[strcspn(p[count].departure_date, "\n")] = '\0';
        
        (getchar() != '\n');
        
        printf("Enter your Full Name: ");
        fgets(p[count].full_name, sizeof(p[count].full_name), stdin);
        p[count].full_name[strcspn(p[count].full_name, "\n")] = '\0';
        
        printf("Enter your Mobile Number: ");
        while(scanf("%d", &p[count].number) != 1) {
            printf("Invalid input. Please enter a valid number: ");
            while(getchar() != '\n'); 
        }
        getchar();

        printf("Enter your Email: ");
        fgets(p[count].email, sizeof(p[count].email), stdin);
        p[count].email[strcspn(p[count].email, "\n")] = '\0';  

        p[count].PNR = generate_pnr(); 
        printf("Passenger PNR: %d\n", p[count].PNR);
        
        count++;

        char add_ch;
        printf("Do you want to add another passenger (Y/N): ");
        scanf(" %c", &add_ch);
        if (add_ch == 'N' || add_ch == 'n') {
            add = 0;
            printf("Choose not to add another passenger\n");
            return;
        } else if (add_ch != 'Y' && add_ch != 'y') {
            printf("Invalid choice\n");
        }

        if (fwrite(&p[count-1], sizeof(p[count-1]), 1, fp) != 1) {
            printf("Error writing to file.\n");
        }

    } while (add != 0);

    fclose(fp);
}


void print_passenger_by_pnr(int pnr) {
    FILE *fp;
    fp = fopen("passenger.txt", "rb"); 
    if (fp == NULL) {
        printf("Can't open file\n");
        return;
    }

    struct Passenger p;

    int found = 0;
    while (fread(&p, sizeof(p), 1, fp) == 1) {
        if (p.PNR == pnr) {
            printf("Passenger Found:\n");
            printf("Full Name: %s\n", p.full_name);
            printf("Mobile: %d\n", p.number);
            printf("Email: %s\n", p.email);
            printf("Departure: %s\n", p.departure);
            printf("Arrival: %s\n", p.arrival);
            printf("Departure Date: %s\n", p.departure_date);
            printf("PNR: %d\n", p.PNR);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Passenger with PNR %d not found.\n", pnr);
    }

    fclose(fp);
}



void search_passenger() {
    int pnr;
    printf("Enter Passenger PNR to search: ");
    scanf("%d", &pnr);
    print_passenger_by_pnr(pnr);
}


void booking() {
    write_passenger();
}


void view_all_routes() {
    FILE *fp;
    struct Bus_details bd;

    // Open the file in read binary mode
    fp = fopen("Bus_Details.dat", "rb");
    if (fp == NULL) {
        printf("\nError: Could not open bus routes database.\n");
        printf("Please make sure the database exists or add routes first.\n\n");
        return;
    }

    printf("\n=========== AVAILABLE BUS ROUTES ===========\n");
    printf("-------------------------------------------\n");
    printf("| %-5s | %-10s | %-15s | %-15s | %-8s | %-10s |\n",
       "S.No", "Bus No.", "From", "To", "Seats", "Type");
    printf("-------------------------------------------\n");

    int route_count = 0;
    
    // Read and display all routes
    while (fread(&bd, sizeof(bd), 1, fp) == 1) {
        route_count++;
        printf("| %-5d | %-10s | %-15s | %-15s | %-8d | %-10s |\n",
       bd.serial_no, bd.bus_number, bd.starts, bd.ends, bd.seat_avail, bd.bus_type);
    }

    printf("-------------------------------------------\n");

    if (route_count == 0) {
        printf("No bus routes available in the system.\n");
        printf("Please contact administrator to add routes.\n");
    } else {
        printf("Total routes available: %d\n", route_count);
    }

    fclose(fp);
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
    scanf("%d", &a);

    switch (a) {
        case 1:
            user_agent_login();
            break;
        case 2:
            user_signup();
            break;
        case 3:
            agent_signup();
            break;
        case 4:
            search_passenger();
            break;
        case 5:
            first_page();
            break;
    }
}


int first_page() {
    int choice;
    printf("============================\n");
    printf("press 1 for Reservation\n");
    printf("press 2 for Cancelation\n");
    printf("press 3 for Booking Status\n");
    printf("press 4 for Exit\n");
    printf("=============================\n\n");
    scanf("%d", &choice);


    switch (choice) {
        case 1:
            reserve();
            break;
        case 2:
            printf("Cancelation\n");
            break;
        case 3:
            printf("Booking Status\n");
            break;
        case 4:
            printf("Exit\n");
            return choice;
            break;
    }
    return 0;
}


int main() {
    printf("=========================================================================\n");
    printf("||                                                                   ||\n");
    printf("||                WELCOME TO BUS RESERVATION SYSTEM                  ||\n");
    printf("||                                                                   ||\n");
    printf("=========================================================================\n");
    printf("\n");
    while (first_page() != 4) {
        printf("============================\n");
    }
    return 0;
}

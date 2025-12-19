#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "read_string.h"
typedef struct {
		unsigned int id;
		char * message;
		int severity;
		char * timestamp;
 } LogEntry;

 int menu(void) {
	puts("Please enter your option");
	puts("1: Add event");
	puts("2: Show all events");
	puts("3: Delete event");
    puts("4: Edit event");
	puts("5: Exit");
	int res;
	if(!scanf("%d", &res) || res < 1 || res > 5) {
		while(getchar() != '\n');
		puts("Invalid input");
		return menu();
	}
	return res;
 }
void get_current_date(int *year, int *month, int *day, int *days_in_month) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        *year = t->tm_year + 1900;
        *month = t->tm_mon + 1;
        *day = t->tm_mday;
        t->tm_mon += 1;
        t->tm_mday = 0;
        mktime(t);
        *days_in_month = t->tm_mday;
}
void parse_time_by_keyword(char ** string) {
    int year, month, day, days_in_month;
    if(strcmp(*string, "today") == 0) {
        get_current_date(&year, &month, &day, &days_in_month);
        sprintf(*string, "%d.%d.%d", day, month, year);
    }
    else if(strcmp(*string, "tomorrow") == 0) {
        get_current_date(&year, &month, &day, &days_in_month);
        printf("\n%d\n", days_in_month);
        if(day + 1 > days_in_month) {
            day = 1;
            month++;
            if(month > 12) {
                month = 1;
                year++;
            }
        }
        else day++;
        sprintf(*string, "%d.%d.%d", day, month, year);
    }
    else if(strcmp(*string, "yesterday") == 0) {
         get_current_date(&year, &month, &day, &days_in_month);
         if(day -1 < 1) {
            day = days_in_month;
            month--;
            if(month < 1) {
                month = 12;
                year--;
            }
        }
        else day--;
        sprintf(*string, "%d.%d.%d", day, month, year);
    }
}
bool parse_string(const char *string, LogEntry ** entry,int *count) {
	
	LogEntry * tmp = realloc(*entry, (*count + 1) * sizeof(LogEntry));
	if(tmp) {
		*entry = tmp;
	}
	unsigned int id;
	char *msg = malloc(256);
	if(!msg) {
		fprintf(stderr, "Memory allocation failed\n");
		return false;
	}
	char *time = malloc(256);
	if(!time) {
		fprintf(stderr, "Memory allocation failed\n");
		free(msg);
		msg = NULL;
		return false;
	}
	int severity; 
	int res = sscanf(string, "%u:%[^:]:%d:%[^\n]", &id, msg, &severity, time);
	if(res != 4 || (id != *count + 1)) return false;
    parse_time_by_keyword(&time);
	(*entry)[*count].id = id;
	(*entry)[*count].severity = severity;
	(*entry)[*count].timestamp = malloc(strlen(time) + 1);
	if(!(*entry)[*count].timestamp) {
		fprintf(stderr, "Memory allocation failed");
		free(msg);
		msg = NULL;
		free(time);
		time = NULL;
		return false;
	}
	strncpy((*entry)[*count].timestamp, time, strlen(time));
	(*entry)[*count].message = malloc(strlen(msg) + 1);
	if(!(*entry)[*count].message) {
		fprintf(stderr, "Memory allocation failed");
		free(msg);
		msg = NULL;
		free(time);
		time = NULL;
		return false;
	}
	strncpy((*entry)[*count].message, msg, strlen(msg));
	//printf("[ID : %d] (SEV : %d) %tu\n%s\n", entry[*count]->id, entry[*count]->severity, entry[*count]->timestamp, entry[*count]->message);
	free(msg);
	msg = NULL;
	free(time);
	time = NULL;
	return true;

}

bool parse_event_from_file(FILE * file, LogEntry **entry, int * count) {
	char * buf = malloc(512);
	if(!buf) {
		fprintf(stderr, "Memory allocation failed\n");
		return false;
	}
	while(fgets(buf, 512, file)) {
		if(!parse_string(buf,entry, count)) {
			if((*entry)[*count].id != *count + 1 && (count != 0)) continue;
			fprintf(stderr, "Parse failed at %d line\n", *count);
			continue;
		}
		(*count)++;
	}
	free(buf);
	buf = NULL;
	return true;
}

void display_event(LogEntry * entry, int count) {
for(int i = 0; i < count; i++) {
	printf("[ID : %d] (SEV : %d) %s\n%s\n", entry[i].id, entry[i].severity, entry[i].timestamp, entry[i].message);
}
printf("\n\n\n");
}

void free_entry(LogEntry * entry, int count) {
	for(int i = 0; i < count; i++) {
		free(entry[i].message);
		free(entry[i].timestamp);
	}
	free(entry);
}
bool add_new_event(int count, FILE * file) {
	printf("Plese enter event in format message,severity,time\n");
	printf("Example:\n");
	printf("Meeting at restaurant,4,20.12.2025:12:30\n");
	char *buf = malloc(1);
	if(!buf) {
		fprintf(stderr, "Memory allocation failed");
		return false;
	}
    while(getchar()!='\n');
	read_string(&buf);
	int severity;
	char * message = malloc(256);
	if(!message) return false;
	 char * time = malloc(256);
	 if(!time) {
		fprintf(stderr, "Memory allocation failed");
		return false;
	 }
	int res = sscanf(buf,"%255[^,],%d,%255[^\n]", message, &severity, time);
	if(res != 3) {
		free(buf);
		buf = NULL;
		free(message);
		message = NULL;
		free(time);
		time = NULL;
		return false;
	}
	fprintf(file, "%d:%s:%d:%s\n",count + 1,message,severity, time);
	free(buf);
	buf = NULL;
	free(message);
	message = NULL;
	free(time);
	time = NULL;
	return true;

}
int get_id(int count) {
	printf("Please enter id of event that you wish to delete: ");
	int res = 0;
	if(!scanf("%d", &res) || res > count || res < 0) {
		while(getchar() != '\n');
		printf("Invalid event id\n");
		return get_id(count);
	}
	return res;
}
int option_to_change(void) {
    puts("Please choose which category do you want to change");
    puts("1: Message");
    puts("2: Severity");
    puts("3: Time");
    puts("4: Cancel");
    int res;
    if(!scanf("%d", &res) || res > 4 || res < 1) {
        while(getchar() != '\n');
        puts("Invalid option, please try again");
        option_to_change();
    }
    return res;
}
void update_data(LogEntry ** entry,int count,FILE * file) {
    for(int i = 0; i < count; i++) {
        fprintf(file, "%d:%s:%d:%s\n",i + 1,(*entry)[i].message,(*entry)[i].severity, (*entry)[i].timestamp);
    }
}
void delete_event(LogEntry ** entry, int id, int *count, FILE * file) {
	for(int i = id; i < *count; i++)  {
		(*entry)[i] = (*entry)[i+1];
	}
	(*count)--;
    update_data(entry, *count, file);
}
void change_event(LogEntry ** entry, int id, int category,int count, FILE * file) {
switch(category) {
    case 1: {
        char * buf = malloc(1);
        if(!buf) {
            fprintf(stderr, "Memory allocation failed\n");
            return;
        }
        while(getchar() != '\n');
        printf("Please enter a new message: ");
        read_string(&buf);
        strcpy((*entry)[id-1].message, buf);
        free(buf);
        buf = NULL;
        break;
    }
    case 2: {
        printf("Please enter new severity: ");
        int sev;
        while(!scanf("%d", &sev)) {
            while(getchar() != '\n');
            printf("Invalid input, please try again\n");
            printf("Please enter new severity: ");
        }
        (*entry)[id-1].severity = sev;
        break;
    }
    case 3 : {
        char * buf = malloc(1);
        if(!buf) {
            fprintf(stderr, "Memory allocation failed\n");
            return;
        }
        while(getchar() != '\n');
        printf("Please enter a new time: ");
        read_string(&buf);
        strcpy((*entry)[id-1].timestamp, buf);
        free(buf);
        buf = NULL;
        break;
    }
}
update_data(entry, count, file);
}
int main(void) {
	LogEntry * entry = malloc(sizeof(LogEntry));
	int count = 0;
	puts("WELCOME TO LOGS");
	const char * filename = "logs.txt";
	FILE * file = fopen(filename, "r");
	if(!file) {
		fprintf(stderr, "Error opening the logs\n");
		exit(1);
	}
	parse_event_from_file(file, &entry, &count);
	fclose(file);
	for(;;) {
	int res = menu();
	switch(res) {
		case 1: {
			file = fopen(filename, "a");
			if(!add_new_event(count, file)) {
				fprintf(stderr, "Incorrect data was provided.\n");
				break;
			}
			puts("Adding event to logs...");
			usleep(2000000);
			fclose(file);
			file = fopen(filename, "r");
			parse_event_from_file(file, &entry, &count);
			fclose(file);
			display_event(entry, count);
			
			break;
		}
		case 2: {
			file = fopen(filename, "r");
			puts("Loading...");
			parse_event_from_file(file,&entry, &count);
			usleep(2000000);
			display_event(entry, count);
			fclose(file);
			break;
		}
		case 3: {
			int id = get_id(count);
            file = fopen(filename, "w");
			delete_event(&entry, id, &count, file);
            fclose(file);
			display_event(entry,count);
			break;
		}
        case 4: {
            int id = get_id(count);
            int category = option_to_change();
            if(category == 4) break;
            file = fopen(filename, "w");
            change_event(&entry, id, category, count, file);
            fclose(file);
            break;

        }
		case 5 : {
			free_entry(entry,count);
			puts("BYE");
			return 0;
		}
	}
}
	
	return 0;
}
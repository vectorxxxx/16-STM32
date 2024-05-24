#include <stdio.h>
#include <time.h>

time_t time_cnt;
struct tm time_date;
char* time_str;

int main(void) {
	
	//time_cnt = time(NULL);
	time(&time_cnt);
	printf("%d\n", time_cnt);
	
	
	//time_date = *gmtime(&time_cnt);
	time_date = *localtime(&time_cnt);
	printf("%d\n", time_date.tm_year + 1900);
	printf("%d\n", time_date.tm_mon + 1);
	printf("%d\n", time_date.tm_mday);
	printf("%d\n", time_date.tm_hour);
	printf("%d\n", time_date.tm_min);
	printf("%d\n", time_date.tm_sec);
	printf("%d\n", time_date.tm_wday);

	
	time_cnt = mktime(&time_date);
	printf("%d\n", time_cnt);
	
	
	time_str = ctime(&time_cnt);
	printf("%s\n", time_str);
	
	
	time_str = asctime(&time_date);
	printf("%s\n", time_str);
	
	
	char date[50];
	strftime(date, 50, "%Y-%m-%d %H:%M:%S %A", &time_date);
	printf("%s\n", date);
	
	
	return 0; 
}

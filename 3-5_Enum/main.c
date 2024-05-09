#include<stdio.h>

typedef enum {
	Monday=1,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday, 
} Week_t;

int main() {

	Week_t t;
	t = Thursday;

	printf("Today is Week-%d\n", t);

	return 0;
}

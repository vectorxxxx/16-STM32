#include<stdio.h>

typedef struct {
	char x;
	int y;
	float z;
} StructName_t;

int main() {

	StructName_t t;
	t.x = 'A';
	t.y = 344;
	t.z = 21.32;

	printf("t.x = %c\n", t.x);
	printf("t.y = %d\n", t.y);
	printf("t.z = %f\n", t.z);

	return 0;
}

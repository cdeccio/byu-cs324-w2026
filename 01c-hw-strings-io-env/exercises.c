#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>

#define BUFSIZE 30

void memprint(char *, char *, int);

void part1();
void part3(char *);
void part4();

int main(int argc, char *argv[]) {
	part1();

	/* Part 2 (below this comment) */
	printf("\n===== Part 2 =====\n");
	printf("-- Questions 8 and 9\n");

	printf("-- Questions 10 and 11\n");

	part3(argv[1]);
	part4();
}

void memprint(char *s, char *fmt, int len) {
	// iterate through each byte/character of s, and print each out with
	// the format specified with fmt
	int i;
	char fmt_with_space[8];

	sprintf(fmt_with_space, "%s ", fmt);
	for (i = 0; i < len; i++) {
		printf(fmt_with_space, s[i]);
	}
	printf("\n");
}

void part1() {
	printf("\n===== Part 1 =====\n");

	// Note: STDOUT_FILENO is defined in /usr/include/unistd.h:
	//
	// #define	STDOUT_FILENO	1

	char s1a[] = { 104, 101, 108, 108, 111, 10 };
	write(STDOUT_FILENO, s1a, 6);

	char s1b[] = { 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x0a };
	write(STDOUT_FILENO, s1b, 6);

	char s1c[] = { 'h', 'e', 'l', 'l', 'o', '\n' };
	write(STDOUT_FILENO, s1c, 6);

	char s2[] = { 0xe5, 0x8f, 0xb0, 0xe7, 0x81, 0xa3, 0x0a };
	write(STDOUT_FILENO, s2, 7);

	char s3[] = { 0xf0, 0x9f, 0x98, 0x82, 0x0a };
	write(STDOUT_FILENO, s3, 5);

	int s1a_len = sizeof(s1a);

	printf("-- Questions 1 through 4\n");

	printf("-- Questions 5 through 7\n");

}

void part3(char *filename) {
	printf("\n===== Part 3 =====\n");

	printf("-- Question 12\n");

	printf("-- Question 13\n");
	char buf[BUFSIZE];

	printf("-- Question 14\n");

	fprintf(stderr, "-- Questions 15 through 18\n");

	printf("-- Question 19\n");
	int fd1, fd2;

	printf("-- Questions 20 and 21\n");
	size_t numread = 0;
	size_t totread = 0;

	printf("-- Questions 22 through 25\n");

	printf("-- Questions 26 through 31\n");

	printf("-- Questions 32 and 33\n");

	printf("-- Question 34\n");

	printf("-- Question 35\n");
	int ret = 0;

	printf("-- Question 36\n");

	printf("-- Question 37\n");

	printf("-- Question 38\n");

	printf("-- Questions 39 and 40\n");

	printf("-- Question 41\n");
}

void part4() {
	printf("\n===== Part 4 =====\n");

	printf("-- Questions 42 and 43\n");
	char *s1;

}

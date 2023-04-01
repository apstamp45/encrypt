#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

	int opt;
	int encrypt = -1;
	char* input = NULL;
	char* password = NULL;

	// Flags/arguments handling
	while ((opt = getopt(argc, argv, ":dep:")) != -1) {
		switch (opt) {
			case 'd':
				if (encrypt == 1) {
					printf("Cannot pass both -d and -e\n");
					return 1;
				} else {
					encrypt = 0;
				}
				break;
			case 'e':
				if (!encrypt) {
					printf("Cannot pass both -d and -e\n");
					return 1;
				} else {
					encrypt = 1;
				}
				break;
			case 'p':
				password = optarg;
				break;
			case '?':
				printf("Unknown option: %c", opt);
				break;
		}
	}
	if (encrypt == -1) {
		printf("You must pass either encrypt or decrypt\n");
		printf("Usage: enc -d/e (-p [password]) [data]\n");
		return 1;
	}
	if (optind >= argc) {
		printf("You must input data to encrypt or decrypt.\n");
		printf("Usage: enc -d/e (-p [password]) [data]\n");
		return 1;
	} else {
		input = argv[optind];
	}
	if (password == NULL) {
		printf("Enter a password (max 256 chars): ");
		scanf("%s", password);
	}

	int input_length = strlen(input);
	int password_length = strlen(password);
	unsigned int seed = 0;
	// Generate seed based on password.
	for (int i = 0; i < password_length; i++) {
		seed ^= password[i] << (i % sizeof(int)) * 8;
	}
	srand(seed);
	char* output = malloc(input_length * sizeof(char));
	if (encrypt) {// If user wants to encrypt data
		for (int i = 0; i < input_length; i++) {
			// Per character
			int c = input[i];
			if (c < 32) {// Skip special chars
				output[i] = c;
				continue;
			}
			// Add random num to char and loop back past 126
			c += (rand() % 94);
			if (c > 126) {
				c -= 94;
			}
			output[i] = c;
		}
	} else {// User wants to decrypt data
		for (int i = 0; i < input_length; i++) {
			int c = input[i];
			if (c < 32) {
				output[i] = c;
				continue;
			}
			// Undo process
			c -= (rand() % 94);
			if (c < 32) {
				c += 94;
			}
			output[i] = c;
		}
	}
	printf("%s\n", output);
	free(output);
	return 0;
}

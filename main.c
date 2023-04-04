#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define PASSWORD_MAX 256

int main(int argc, char* argv[]) {

	int opt;
	int encrypt = -1;
	char* input = NULL;
	char* input_file = NULL;
	bool input_passed = true;
	char* output = NULL;
	char* output_file = NULL;
	char* password = NULL;
	bool password_passed = false;
	int input_length;
	int password_length;
	unsigned int seed = 0;

	// Flags/arguments handling
	while ((opt = getopt(argc, argv, ":dep:i:o:")) != -1) {
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
			case 'i':
				input_file = optarg;
				break;
			case 'o':
				output_file = optarg;
				break;
			case ':':
				printf("You must provide an input/output file when using -i/o");
				return 1;
			case '?':
				printf("Unknown option: %c", opt);
				break;
		}
	}
	if (encrypt == -1) {
		printf("You must pass either encrypt or decrypt\n");
		printf("Usage: enc -d/e (-p [password] -i [input file]\n\t-o [output file]) [data]\n");
		return 1;
	}

	// Handle input
	if (input_file != NULL) {
		FILE* file = fopen(input_file, "r");
		if (file == NULL) {
			printf("Can't open input file: %s\n", input_file);
			return 1;
		}
		fseek(file, 0, SEEK_END);
		int file_size = ftell(file);
		fseek(file, 0, SEEK_SET);
		input = malloc(file_size * sizeof(char));
		input_length = file_size;
		input_passed = false;
		output = malloc(file_size * sizeof(char));
		fread(input, sizeof(char), file_size, file);
		fclose(file);
	} else if (optind >= argc) {
		printf("You must input data to encrypt or decrypt.\n");
		printf("Usage: enc -d/e (-p [password] -i [input file]\n\t-o [output file]) [data]\n");
		return 1;
	} else {
		input = argv[optind];
		input_length = strlen(input);
		output = malloc(input_length * sizeof(char));
	}


	// Get password
	if (password == NULL) {
		password = malloc(PASSWORD_MAX);
		printf("Enter a password (max %d chars): ", PASSWORD_MAX);
		fgets(password, PASSWORD_MAX, stdin);
		password[strcspn(password, "\n")] = 0;
		password_passed = true;
	}
	password_length = strlen(password);

	// Generate seed based on password.
	for (int i = 0; i < password_length; i++) {
		seed ^= password[i] << (i % sizeof(int)) * 8;
	}
	srand(seed);

	// If user wants to encrypt data
	if (encrypt) {
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

	// Handle output
	if (output_file == NULL) {
		printf("%s", output);
	} else {
		FILE* file = fopen(output_file, "w");
		if (file == NULL) {
			printf("Unable to open output file: %s\n", output_file);
			return 1;
		}
		fprintf(file, "%s", output);
		fclose(file);
	}

	// Free buffers
	if (!input_passed) {
		free(input);
	}
	free(output);
	if (password_passed) {
		free(password);
	}
	return 0;
}

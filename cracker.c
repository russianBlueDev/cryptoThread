#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <crypt.h>
#include <stdbool.h>
#include <pthread.h>

#define ALPHABET_SIZE 65
#define PW_SIZE 4
#define HASH_MAX_SIZE 128
#define SALT "xy"

const char* const get_alphabet(void);
const char* const new_random_pw(const char* alphabet, const size_t size);
void* thread1(void* p);
void* thread2(void* p);
void* thread3(void* p);
void* thread4(void* p);

const char* alphabet;
char* hash_of_hidden_pw;
bool pw_found;

int main(int argc, char* argv[]) {

	// Password Alphabet
	alphabet = get_alphabet();

	// Crypto Init
	struct crypt_data* cdata = malloc(sizeof(struct crypt_data));
	cdata->initialized = 0;

	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC, &start);

	for (int i = 0; i < 20; ++i) {

		pw_found = false;
		// Mystery password generation
		const char* hidden_pw = new_random_pw(alphabet, PW_SIZE);
		hash_of_hidden_pw = malloc(HASH_MAX_SIZE * sizeof(char));
		strncpy(hash_of_hidden_pw, crypt_r(hidden_pw, SALT, cdata),
		HASH_MAX_SIZE);

		// Brute force variable
		char* pwd = malloc(PW_SIZE * sizeof(char));
		memset(pwd, 'a', PW_SIZE);

		// Brute force Algorithm
		pthread_t threadNo1;
		pthread_create(&threadNo1, NULL, thread1, NULL);
		pthread_t threadNo2;
		pthread_create(&threadNo2, NULL, thread2, NULL);
		pthread_t threadNo3;
		pthread_create(&threadNo3, NULL, thread3, NULL);
		pthread_t threadNo4;
		pthread_create(&threadNo4, NULL, thread4, NULL);

		// Wait for pw to be found
		pthread_join(threadNo1, NULL);
		pthread_join(threadNo2, NULL);
		pthread_join(threadNo3, NULL);
		pthread_join(threadNo4, NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	double elapsed = finish.tv_sec - start.tv_sec;
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Time: %lf seconds\n", elapsed/20);
	return 0;
}

const char* const get_alphabet(void) {
	char* const alphabet = malloc(ALPHABET_SIZE * sizeof(char));
	size_t i = 0;
	for (char c = 'A'; c <= 'Z'; c++) {
		alphabet[i] = c;
		i++;
	}
	for (char c = 'a'; c <= 'z'; c++) {
		alphabet[i] = c;
		i++;
	}
	for (char c = '0'; c <= '9'; c++) {
		alphabet[i] = c;
		i++;
	}
	alphabet[i++] = '~';
	alphabet[i++] = '!';
	alphabet[i++] = '*';

	return alphabet;
}

const char* const new_random_pw(const char* alphabet, const size_t size) {
	srand(time(NULL));
	char* pw = malloc(size * sizeof(char));
	for (int i = 0; i < size; ++i) {
		pw[i] = alphabet[(int) ((double) rand() / (double) RAND_MAX
				* (double) ALPHABET_SIZE)];
	}
	return pw;
}

void* thread1(void* p) {

	// Crypto Init
	struct crypt_data* cdata = malloc(sizeof(struct crypt_data));
	cdata->initialized = 0;

	// Brute force variable
	char* pwd = malloc(PW_SIZE * sizeof(char));
	memset(pwd, 'a', PW_SIZE);

	for (int i = 0; i < ALPHABET_SIZE / 4; ++i) {
		pwd[0] = alphabet[i];
		printf(">");
		fflush(stdout);
		for (int j = 0; j < ALPHABET_SIZE; ++j) {
			pwd[1] = alphabet[j];
			for (int k = 0; k < ALPHABET_SIZE; ++k) {
				pwd[2] = alphabet[k];
				for (int l = 0; l < ALPHABET_SIZE; ++l) {
					pwd[3] = alphabet[l];
					char *hash = crypt_r(pwd, SALT, cdata);
					if (pw_found) {
						return NULL;
					}
					if (strcmp(hash, hash_of_hidden_pw) == 0) {
						printf("\nFound: pw is %s\n", pwd);
						pw_found = true;
						return NULL;
					}
				}
			}
		}
	}

	return NULL;
}

void* thread2(void* p) {

	// Crypto Init
	struct crypt_data* cdata = malloc(sizeof(struct crypt_data));
	cdata->initialized = 0;

	// Brute force variable
	char* pwd = malloc(PW_SIZE * sizeof(char));
	memset(pwd, 'a', PW_SIZE);

	for (int i = ALPHABET_SIZE / 4; i < ALPHABET_SIZE / 2; ++i) {
		pwd[0] = alphabet[i];
		printf("+");
		fflush(stdout);
		for (int j = 0; j < ALPHABET_SIZE; ++j) {
			pwd[1] = alphabet[j];
			for (int k = 0; k < ALPHABET_SIZE; ++k) {
				pwd[2] = alphabet[k];
				for (int l = 0; l < ALPHABET_SIZE; ++l) {
					pwd[3] = alphabet[l];
					char *hash = crypt_r(pwd, SALT, cdata);
					if (pw_found) {
						return NULL;
					}
					if (strcmp(hash, hash_of_hidden_pw) == 0) {
						printf("\nFound: pw is %s\n", pwd);
						pw_found = true;
						return NULL;
					}
				}
			}
		}
	}

	return NULL;
}

void* thread3(void* p) {

	// Crypto Init
	struct crypt_data* cdata = malloc(sizeof(struct crypt_data));
	cdata->initialized = 0;

	// Brute force variable
	char* pwd = malloc(PW_SIZE * sizeof(char));
	memset(pwd, 'a', PW_SIZE);

	for (int i = ALPHABET_SIZE / 2; i < ALPHABET_SIZE / 2 + ALPHABET_SIZE / 4;
			++i) {
		pwd[0] = alphabet[i];
		printf("=");
		fflush(stdout);
		for (int j = 0; j < ALPHABET_SIZE; ++j) {
			pwd[1] = alphabet[j];
			for (int k = 0; k < ALPHABET_SIZE; ++k) {
				pwd[2] = alphabet[k];
				for (int l = 0; l < ALPHABET_SIZE; ++l) {
					pwd[3] = alphabet[l];
					char *hash = crypt_r(pwd, SALT, cdata);
					if (pw_found) {
						return NULL;
					}
					if (strcmp(hash, hash_of_hidden_pw) == 0) {
						printf("\nFound: pw is %s\n", pwd);
						pw_found = true;
						return NULL;
					}
				}
			}
		}
	}

	return NULL;
}

void* thread4(void* p) {

	// Crypto Init
	struct crypt_data* cdata = malloc(sizeof(struct crypt_data));
	cdata->initialized = 0;

	// Brute force variable
	char* pwd = malloc(PW_SIZE * sizeof(char));
	memset(pwd, 'a', PW_SIZE);

	for (int i = ALPHABET_SIZE / 2 + ALPHABET_SIZE / 4; i < ALPHABET_SIZE;
			++i) {
		pwd[0] = alphabet[i];
		printf("$");
		fflush(stdout);
		for (int j = 0; j < ALPHABET_SIZE; ++j) {
			pwd[1] = alphabet[j];
			for (int k = 0; k < ALPHABET_SIZE; ++k) {
				pwd[2] = alphabet[k];
				for (int l = 0; l < ALPHABET_SIZE; ++l) {
					pwd[3] = alphabet[l];
					char *hash = crypt_r(pwd, SALT, cdata);
					if (pw_found) {
						return NULL;
					}
					if (strcmp(hash, hash_of_hidden_pw) == 0) {
						printf("\nFound: pw is %s\n", pwd);
						pw_found = true;
						return NULL;
					}
				}
			}
		}
	}

	return NULL;
}

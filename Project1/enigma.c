#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NC 26 // Number of characters in the alphabet
#define A 'A' // Value of 'A'
// I had a tough time with this assignment as I am pretty garbage at math and podman.
// Right now it is working for an input
// Im getting " Sample input incorrectly/not descrypted." as an error with the tester file right now though and Im not totally sure how to fix this"
// Define the rotors
const char *rotors[] = {
    "BDFHJLCPRTXVZNYEIWGAKMUSQO", // fast
    "AJDKSIRUXBLHWTMCQGZNPYFVOE", // medium
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", // slow
    "IXUHFEZDAOMTKQJWNSRLCYPBVG"  // reflect
};

//  apply a cipher/rotor `r` to a letter `c`
char rapply(char c, const char *r) {
    return r[c - A];
}

// invert a cipher/rotor `r`
void invert(const char *r, char *inverted) {
    int i, j;
    int index[NC];
    for (i = 0; i < NC; i++) {
        index[r[i] - A] = i;
    }
    for (i = 0; i < NC; i++) {
        inverted[i] = index[i] + A;
    }
    inverted[NC] = '\0';
}
void extend_rotors(const char **extended_rotors) {
    char inverted[NC + 1];
    for (int i = 0; i < 3; i++) {
        invert(rotors[2 - i], inverted);
        extended_rotors[4 + i] = strdup(inverted); // Append inverted rotors
    }
}

// Function to shift letter `c` forward `n` letters in the alphabet
char nshift(char c, int n) {
    return (c - A + n + NC) % NC + A;
}

// Function to compute shifts for a letter `l`
int shifts(int l, int n) {
    return (int[]){l % NC, l / NC % NC, l / (NC * NC) % NC, 0,
                   l / (NC * NC) % NC, l / NC % NC, l % NC}[n];
}

// Function to apply shift and rotor
char shiply(char c, int n, const char *r) {
    return nshift(rapply(nshift(c, n), r), -n);
}

// Function for a single letter encryption with the Enigma machine
char letter(char c, int l, const char **extended_rotors) {
    for (int i = 0; i < 7; i++) {
        c = shiply(c, shifts(l, i), extended_rotors[i]);
    }
    return c;
}

// Function to encrypt a phrase
void enigma(const char *s, char *result) {
    const char *extended_rotors[7] = {rotors[0], rotors[1], rotors[2], rotors[3], NULL, NULL, NULL};
    extend_rotors(extended_rotors);

    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) {
        result[i] = letter(s[i], i + 1, extended_rotors);
    }
    result[len] = '\0';

    // Free allocated memory for inverted rotors
    for (int i = 4; i < 7; i++) {
        free((void *)extended_rotors[i]);
    }
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return 1;
    }

    char result[1024];
    enigma(argv[1], result);
    printf("Encrypted/Decrypted: %s\n", result);
    return 0;
}


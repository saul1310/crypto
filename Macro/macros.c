#include <stdio.h>
#include <stdbool.h>
// Collaboarated with Aaron Nguyen

#define CHOICE(e, f, g) (((e) & (f)) | (~(e) & (g)))
#define MEDIAN(e, f, g) ((e & f) | (f & g) | (g & e))
#define ROTATE(x, n) (((x) >> (n)) | ((x) << (sizeof(x) * 8 - (n))))

void print_bits(unsigned int x, int len) {
    for (int i = len - 1; i >= 0; i--) {
        printf("%d", (x >> i) & 1);
    }
}

int main() {
    printf("Boolean Choice\n");
    for (int i = 0; i < 8; i++) {
        bool e = (i >> 2) & 1;
        bool f = (i >> 1) & 1;
        bool g = i & 1;
        printf("_choice(%d,%d,%d) -> %d\n", e, f, g, CHOICE(e,f,g));
    }

    printf("\nBitwise Choice\n");
    unsigned int e = 0b00001111;
    unsigned int f = 0b00110011;
    unsigned int g = 0b01010101;
    printf("choice(");
    print_bits(e, 8); printf(",");
    print_bits(f, 8); printf(",");
    print_bits(g, 8); printf(") -> ");
    unsigned int choice_result = 0;
    for (int i = 0; i < 8; i++) {
        bool bit_e = (e >> i) & 1;
        bool bit_f = (f >> i) & 1;
        bool bit_g = (g >> i) & 1;
        choice_result |= (CHOICE(bit_e, bit_f, bit_g) << i);
    }
    print_bits(choice_result, 8);
    printf("\n");

    printf("\nBoolean Median\n");
    for (int i = 0; i < 8; i++) {
        bool e = (i >> 2) & 1;
        bool f = (i >> 1) & 1;
        bool g = i & 1;
        printf("_median(%d,%d,%d) -> %d\n", e, f, g, MEDIAN(e,f,g));
    }

    printf("\nBitwise Median\n");
    unsigned int median_result = 0;
    for (int i = 0; i < 6; i++) {
        bool bit_e = (e >> i) & 1;
        bool bit_f = (f >> i) & 1;
        bool bit_g = (g >> i) & 1;
        median_result |= (MEDIAN(bit_e, bit_f, bit_g) << i);
    }
    printf("median(");
    print_bits(e, 6); printf(",");
    print_bits(f, 6); printf(",");
    print_bits(g, 6); printf(") -> ");
    print_bits(median_result, 6);
    printf("\n");

    printf("\nBitwise Rotate Right\n");
    unsigned int x = 0b00101101;
    int len = 8;
    for (int n = 0; n <= len; n++) {
        printf("rotate(");
        print_bits(x, len);
        printf(",%d) -> ", n);
        print_bits(ROTATE(x,n), len);
        printf("\n");
    }

    return 0;
}

/* shainc.c - sha256, avoid namespace collisions */
// collaborated with A Nguyen
/*
gcc shainc.c --std=c89 -Wall -Wextra -Werror -Wpedantic -O2 -o shainc
*/

#include <stdio.h>  /* fileio   */
#include <stdint.h> /* uint32_t */
#include <stdlib.h> /* exit     */
#include <string.h> /* memcpy   */

/* Optionally - endianness compatability. 
#include <arpa/inet.h> */


/* initial hash values (h_0) */
/* network endian */
uint32_t h[8] = {
    0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
    0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
};

/* round constants */
/* network endian */
uint32_t k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
    0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
    0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
    0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
    0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
    0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
    0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
    0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
    0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define SHR(x, n) ((x) >> (n))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define EP1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SIG0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3)) 
#define SIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10)) 

void sha256_transform(uint32_t *state, const uint8_t *block) {
    uint32_t a, b, c, d, e, f, g, h, t1, t2, w[64], i;
    
    for (i = 0; i < 16; i++) {
        w[i] = ((uint32_t)block[i * 4] << 24) |
               ((uint32_t)block[i * 4 + 1] << 16) |
               ((uint32_t)block[i * 4 + 2] << 8) |
               ((uint32_t)block[i * 4 + 3]);
    }

    for (i = 16; i < 64; i++) {
        w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
    }

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];

    for (i = 0; i < 64; i++) {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + w[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

void sha256_init(uint32_t *state) {
    memcpy(state, h, 8 * sizeof(uint32_t));
}

void sha256_update(uint32_t *state, const uint8_t *data, size_t len) {
    size_t i;
    for (i = 0; i < len; i += 64) {
        sha256_transform(state, data + i);
    }
}

void sha256_final(uint32_t *state, const uint8_t *data, size_t len, uint8_t *digest) {
    uint8_t block[128] = {0}; 
    uint64_t bits_len;
    size_t block_len;
    size_t i;

    block_len = len % 64;
    memcpy(block, data + (len - block_len), block_len);

    block[block_len] = 0x80;

    if (block_len >= 56) {
        sha256_transform(state, block);
        memset(block, 0, 64);
    }

    bits_len = len * 8;
    block[63] = (uint8_t)(bits_len & 0xFF);
    block[62] = (uint8_t)((bits_len >> 8) & 0xFF);
    block[61] = (uint8_t)((bits_len >> 16) & 0xFF);
    block[60] = (uint8_t)((bits_len >> 24) & 0xFF);
    block[59] = (uint8_t)((bits_len >> 32) & 0xFF);
    block[58] = (uint8_t)((bits_len >> 40) & 0xFF);
    block[57] = (uint8_t)((bits_len >> 48) & 0xFF);
    block[56] = (uint8_t)((bits_len >> 56) & 0xFF);

    sha256_transform(state, block);

    for (i = 0; i < 8; i++) {
        digest[i * 4] = (uint8_t)((state[i] >> 24) & 0xFF);
        digest[i * 4 + 1] = (uint8_t)((state[i] >> 16) & 0xFF);
        digest[i * 4 + 2] = (uint8_t)((state[i] >> 8) & 0xFF);
        digest[i * 4 + 3] = (uint8_t)(state[i] & 0xFF);
    }
}

void sha256(const uint8_t *data, size_t len, uint8_t *digest) {
    uint32_t state[8];
    
    sha256_init(state);
    
    size_t complete_blocks = len / 64;
    if (complete_blocks > 0) {
        sha256_update(state, data, complete_blocks * 64);
    }
    
    sha256_final(state, data, len, digest);
}

void print_hash(const uint8_t *hash) {
    int i;
    for (i = 0; i < 32; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    FILE *file;
    uint8_t *buffer;
    long file_size;
    uint8_t hash[32];
    
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    file = fopen(argv[1], "rb");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    buffer = (uint8_t *)malloc(file_size);
    
    if (fread(buffer, 1, file_size, file) != (size_t)file_size) {
        perror("Failed to read file");
        free(buffer);
        fclose(file);
        return 1;
    }
    
    sha256(buffer, file_size, hash);
    
    printf("SHA-256 hash of %s: ", argv[1]);
    print_hash(hash);
    
    free(buffer);
    fclose(file);
    
    return 0;
}
//b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9
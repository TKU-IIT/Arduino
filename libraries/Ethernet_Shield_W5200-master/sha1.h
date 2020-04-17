#ifndef SHA1_H_
#define SHA1_H_

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;

void SHA1Transform(uint32_t state[5], uint8_t buffer[64]);
void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, uint8_t* data, uint32_t len);
void SHA1Final(unsigned char digest[20], SHA1_CTX* context);

#endif /* SHA1_H_ */

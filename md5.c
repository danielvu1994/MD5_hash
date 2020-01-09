#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
 
// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
#define CAPTCHA_LENGTH 8
 
 
static char *md5Hash(const char *initial_msg, size_t initial_len) {
    // Message (to prepare)
    char *msg = NULL;
	// These vars will contain the hash
	uint32_t h0, h1, h2, h3;
 
    // Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating
 
    // r specifies the per-round shift amounts
 
    uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // Use binary integer part of the sines of integers (in radians) as constants// Initialize variables:
    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
 
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
 
    // Pre-processing: adding a single 1 bit
    //append "1" bit to message    
    /* Notice: the input bytes are considered as bits strings,
       where the first bit is the most significant bit of the byte.[37] */
 
    // Pre-processing: padding with zeros
    //append "0" bit until message length in bit ≡ 448 (mod 512)
    //append length mod (2 pow 64) to message
 
    int new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;

 
    msg = calloc(new_len + 64, 1); // also appends "0" bits 
                                   // (we alloc also 64 extra bytes...)

    memcpy(msg, initial_msg, initial_len);

    msg[initial_len] = 128; // write the "1" bit
 
    uint32_t bits_len = 8*initial_len; // note, we append the len
	
    memcpy(msg + new_len, &bits_len, 4);           // in bits at the end of the buffer
 
    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    int offset;
    for(offset=0; offset<new_len; offset += (512/8)) {
 
        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        uint32_t *w = (uint32_t *) (msg + offset);
 
        // Initialize hash value for this chunk:
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
 
        // Main loop:
        uint32_t i;
        for(i = 0; i<64; i++) {

#ifdef ROUNDS
            uint8_t *p;
 
            p=(uint8_t *)&a;
            // printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], a);
         
            p=(uint8_t *)&b;
            // printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], b);
         
            p=(uint8_t *)&c;
            // printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], c);
         
            p=(uint8_t *)&d;
            // printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], d);
            puts("");
#endif        

 
            uint32_t f, g;
 
             if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

            uint32_t temp = d;
            d = c;
            c = b;
            // printf("rotateLeft(%x + %x + %x + %x, %d)\n", a, f, k[i], w[g], r[i]);
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;


 
        }
 
        // Add this chunk's hash to result so far:
#ifdef DEBUG
		printf("a %d \n", a);
		printf("b %d \n", b);
		printf("c %d \n", c);
		printf("d %d \n", d);
#endif
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
 
    }
	
	char ret[9];
	char res[33];
	strcpy(res, "");
	uint8_t *p;
	p=(uint8_t *)&h0;
	sprintf(ret, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	strcat(res, ret);
 
    p=(uint8_t *)&h1;
    sprintf(ret, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	strcat(res, ret);
 
    p=(uint8_t *)&h2;
    sprintf(ret, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	strcat(res, ret);
 
    p=(uint8_t *)&h3;
    sprintf(ret, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	strcat(res, ret);

 
    // cleanup
    free(msg);
	static uint8_t retOri[16];
	uint32_t i;
	for( i = 0; i < 4; i++)
	{
		uint8_t *p;
		switch(i)
		{
			case 0: 
				p = (uint8_t *)&h0;
				break;
			case 1:
				p = (uint8_t *)&h1;
				break;
			case 2:
				p = (uint8_t *)&h2;
				break;
			case 3:
				p = (uint8_t *)&h3;
				break;
			default:
				break;
		}
		uint8_t k;
		for( k = 0; k < 4; k++)
		{
			retOri[k + i*4] = p[k];
		}
	}

	int j;
	for(j =0; j < 16; j++) printf("%x ", retOri[j]);
	puts("");
	return retOri;
 
}
 
int generatePassword(const char * pass, int n, char * res)
{
   // Characters to be included
   // char *chrs = "abcdefghjkmnopqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ2345679";
   char *chrs = "aAb2cCd3DeE4fFgG5hHjJ6kKmM7nNopPqQrRsStTuUvVwWxXyYzZ9";
   uint8_t * md5;

   md5 = md5Hash(pass, strlen((char *)pass));
// #ifdef DEBUG

// #endif

   // Generate n characters from above set and
   // add these characters to captcha.
   while (n--)
   {
      int i = 0;
      int m = 0;
      for( i = 0; i < 2; i++)
      {
         m += md5[i +2*(n)];
      }
      res[n] = (chrs[m%53]);
   }
	
   return 0;
}
void createPassFile()
{
	char* res = (char*)malloc(sizeof(char)*CAPTCHA_LENGTH);
	
	
	FILE *fptr;
	if ((fptr = fopen("./test.txt","w")) == NULL){
       printf("Error! opening file");
       // Program exits if the file pointer returns NULL.
       exit(1);
    }
	
	char strUserPw[20];
	uint8_t macNum[6] = {0x0,};
	macNum[0] = 0x94;
	macNum[1] = 0x2C;
	macNum[2] = 0xB3;
	int i = 0;
	for( i; i <= 255; i++)
	{
		macNum[3] = i;
		int j = 0;
		for( j; j <= 255; j++)
		{
			macNum[4] = j;
			int k = 0;
			for( k; k <= 255; k++)
			{
				macNum[5] = k;
				sprintf(strUserPw, "%02X%02X%02X%02X%02X%02X", macNum[0], macNum[1], macNum[2], macNum[3], macNum[4], macNum[5]);
				fprintf(fptr, "MAC is %s \t", strUserPw);
				generatePassword(strUserPw, CAPTCHA_LENGTH, res);
				fprintf(fptr, "password is %s \n\n", res);
			}
		}
	}
	
	
	free(res);
    fclose(fptr); 
}


int main(int argc, char **argv) {
	char *msg = argv[1];
	createPassFile();
 
    return 0;
}
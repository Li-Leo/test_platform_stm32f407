/*
 * C program to Implement the RSA Algorithm
 */
 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "log.h"
 
int32_t is_prime(int32_t n)
{
    int32_t i;
    
    for (i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}
 
int32_t gcd(int32_t a, int32_t b)
{
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}
 
int32_t totient(int32_t p, int32_t q)
{
    return (p - 1) * (q - 1);
}
 
int32_t randome(int32_t lambda_n)
{
    printf("The number e should be less than %d and greater than 1.\r\n", lambda_n);
    
    for (int32_t i = 2; i < lambda_n; i++) {
        if (gcd(i, lambda_n) == 1) {
            return i;
        }
    }
    
    return -1;
}
 
int32_t private_key(int32_t e, int32_t lambda_n)
{
    for (int32_t i = 1; i < lambda_n; i++) {
        if ((i * e) % lambda_n == 1) {
            printf("Thus, (i * e) %% lambda_n = 1, (%d * %d) %% %d = 1\r\n", i, e,
             lambda_n);
            return i;
        }
    }
    
    return -1;
}
 
int64_t pomod(int64_t a, int64_t b, int64_t m)
{
    int64_t x = 1, y = a;

    while (b > 0) {
        if (b % 2 == 1) {
            x = (x * y) % m;
        }
        y = (y * y) % m;
        b /= 2;
    }

    return x % m;
}
 
/* Encryption
 * A function which takes the message, the public key and a number n which is the
 * product of p and q. The function encrypts the message using the public key
 * and returns the encrypted message.
 */
 
char *encrypt(char *message, int64_t e, int64_t n)
{
    int64_t i;
    int64_t len = strlen(message);
    char *cipher = (char *)malloc(len * sizeof(char));
    
    for (i = 0; i < len; i++) {
        cipher[i] = pomod(message[i], e, n);
        // printf("%c -> %c\r\n", message[i], cipher[i]);
    }

    return cipher;
}
 
/* Decryption
 * A function which takes the cipher text, the private key and a number n which
 * is he product of p and q. The function decrypts the cipher text using the
 * private key and returns the decrypted message.
 */
 
char *decrypt(char *cipher, int64_t d, int64_t n)
{
    int64_t i;
    int64_t len = strlen(cipher);
    char *message = (char *)malloc(len * sizeof(char));
    
    for (i = 0; i < len; i++) {
        // message[i] = (int64_t) pow(cipher[i], d) % n;
        message[i] = pomod(cipher[i], d, n);
        // printf("%c -> %c\r\n", cipher[i], message[i]);
    }

    return message;
}

int32_t rsa_test(int32_t p, int32_t q)
{
    // int32_t p, q, 
    int32_t lambda_n;
    int64_t n, e, d;
    char *message = "hello";
    char *cipher;

    // p = 11, q = 19;

    // printf("\nEnter the value of p: ");
    // scanf("%d", &p);
    // printf("\nEnter the value of q: ");
    // scanf("%d", &q);

    if (is_prime(p) && is_prime(q))
    {
        n = p * q;
        lambda_n = totient(p, q);
        e = randome(lambda_n);
        d = private_key(e, lambda_n);
        printf("The value of n is %lld\r\n", n);
        printf("The value of lambda_n is %d\r\n", lambda_n);
        printf("The value of e is %lld\r\n", e);
        printf("The value of d is %lld\r\n", d);
        // printf("Enter the message: \r\n");

        // message = (char *)malloc(sizeof(char) * 100);
        // scanf("%s", message);
        cipher = encrypt(message, e, n);
        printf("The encrypted message is: %s\r\n", cipher);

        message = decrypt(cipher, d, n);
        printf("The original message was: %s\r\n", message);
        free(message);
    }
    else
    {
        printf("\nThe value of p and q should be prime.");
    }
    return 0;
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), rsa_test, rsa_test, rsa test);

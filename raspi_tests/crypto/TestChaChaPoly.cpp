/*
 * Copyright (C) 2015 Southern Storm Software, Pty Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
This example runs tests on the ChaChaPoly implementation to verify
correct behaviour.
*/


#include <Piduino.h>
#define Serial Console

#include <Crypto.h>
#include <ChaChaPoly.h>
#include <string.h>
#if defined(ESP8266) || defined(ESP32)
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#define MAX_PLAINTEXT_LEN 265

struct TestVector
{
    const char *name;
    uint8_t key[32];
    uint8_t plaintext[MAX_PLAINTEXT_LEN];
    uint8_t ciphertext[MAX_PLAINTEXT_LEN];
    uint8_t authdata[16];
    uint8_t iv[16];
    uint8_t tag[16];
    size_t authsize;
    size_t datasize;
    size_t tagsize;
    size_t ivsize;
};

// Test vectors for ChaChaPoly from draft-nir-cfrg-chacha20-poly1305-04.txt
static TestVector const testVectorChaChaPoly_1 PROGMEM = {
    .name        = "ChaChaPoly #1",
    .key         = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
                    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
                    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f},
    .plaintext   = {0x4c, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61,
                    0x6e, 0x64, 0x20, 0x47, 0x65, 0x6e, 0x74, 0x6c,
                    0x65, 0x6d, 0x65, 0x6e, 0x20, 0x6f, 0x66, 0x20,
                    0x74, 0x68, 0x65, 0x20, 0x63, 0x6c, 0x61, 0x73,
                    0x73, 0x20, 0x6f, 0x66, 0x20, 0x27, 0x39, 0x39,
                    0x3a, 0x20, 0x49, 0x66, 0x20, 0x49, 0x20, 0x63,
                    0x6f, 0x75, 0x6c, 0x64, 0x20, 0x6f, 0x66, 0x66,
                    0x65, 0x72, 0x20, 0x79, 0x6f, 0x75, 0x20, 0x6f,
                    0x6e, 0x6c, 0x79, 0x20, 0x6f, 0x6e, 0x65, 0x20,
                    0x74, 0x69, 0x70, 0x20, 0x66, 0x6f, 0x72, 0x20,
                    0x74, 0x68, 0x65, 0x20, 0x66, 0x75, 0x74, 0x75,
                    0x72, 0x65, 0x2c, 0x20, 0x73, 0x75, 0x6e, 0x73,
                    0x63, 0x72, 0x65, 0x65, 0x6e, 0x20, 0x77, 0x6f,
                    0x75, 0x6c, 0x64, 0x20, 0x62, 0x65, 0x20, 0x69,
                    0x74, 0x2e},
    .ciphertext  = {0xd3, 0x1a, 0x8d, 0x34, 0x64, 0x8e, 0x60, 0xdb,
                    0x7b, 0x86, 0xaf, 0xbc, 0x53, 0xef, 0x7e, 0xc2,
                    0xa4, 0xad, 0xed, 0x51, 0x29, 0x6e, 0x08, 0xfe,
                    0xa9, 0xe2, 0xb5, 0xa7, 0x36, 0xee, 0x62, 0xd6,
                    0x3d, 0xbe, 0xa4, 0x5e, 0x8c, 0xa9, 0x67, 0x12,
                    0x82, 0xfa, 0xfb, 0x69, 0xda, 0x92, 0x72, 0x8b,
                    0x1a, 0x71, 0xde, 0x0a, 0x9e, 0x06, 0x0b, 0x29,
                    0x05, 0xd6, 0xa5, 0xb6, 0x7e, 0xcd, 0x3b, 0x36,
                    0x92, 0xdd, 0xbd, 0x7f, 0x2d, 0x77, 0x8b, 0x8c,
                    0x98, 0x03, 0xae, 0xe3, 0x28, 0x09, 0x1b, 0x58,
                    0xfa, 0xb3, 0x24, 0xe4, 0xfa, 0xd6, 0x75, 0x94,
                    0x55, 0x85, 0x80, 0x8b, 0x48, 0x31, 0xd7, 0xbc,
                    0x3f, 0xf4, 0xde, 0xf0, 0x8e, 0x4b, 0x7a, 0x9d,
                    0xe5, 0x76, 0xd2, 0x65, 0x86, 0xce, 0xc6, 0x4b,
                    0x61, 0x16},
    .authdata    = {0x50, 0x51, 0x52, 0x53, 0xc0, 0xc1, 0xc2, 0xc3,
                    0xc4, 0xc5, 0xc6, 0xc7},
    .iv          = {0x07, 0x00, 0x00, 0x00, 0x40, 0x41, 0x42, 0x43,
                    0x44, 0x45, 0x46, 0x47},
    .tag         = {0x1a, 0xe1, 0x0b, 0x59, 0x4f, 0x09, 0xe2, 0x6a,
                    0x7e, 0x90, 0x2e, 0xcb, 0xd0, 0x60, 0x06, 0x91},
    .authsize    = 12,
    .datasize    = 114,
    .tagsize     = 16,
    .ivsize      = 12
};
static TestVector const testVectorChaChaPoly_2 PROGMEM = {
    .name        = "ChaChaPoly #2",
    .key         = {0x1c, 0x92, 0x40, 0xa5, 0xeb, 0x55, 0xd3, 0x8a,
                    0xf3, 0x33, 0x88, 0x86, 0x04, 0xf6, 0xb5, 0xf0,
                    0x47, 0x39, 0x17, 0xc1, 0x40, 0x2b, 0x80, 0x09,
                    0x9d, 0xca, 0x5c, 0xbc, 0x20, 0x70, 0x75, 0xc0},
    .plaintext   = {0x49, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65, 0x74,
                    0x2d, 0x44, 0x72, 0x61, 0x66, 0x74, 0x73, 0x20,
                    0x61, 0x72, 0x65, 0x20, 0x64, 0x72, 0x61, 0x66,
                    0x74, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65,
                    0x6e, 0x74, 0x73, 0x20, 0x76, 0x61, 0x6c, 0x69,
                    0x64, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x61, 0x20,
                    0x6d, 0x61, 0x78, 0x69, 0x6d, 0x75, 0x6d, 0x20,
                    0x6f, 0x66, 0x20, 0x73, 0x69, 0x78, 0x20, 0x6d,
                    0x6f, 0x6e, 0x74, 0x68, 0x73, 0x20, 0x61, 0x6e,
                    0x64, 0x20, 0x6d, 0x61, 0x79, 0x20, 0x62, 0x65,
                    0x20, 0x75, 0x70, 0x64, 0x61, 0x74, 0x65, 0x64,
                    0x2c, 0x20, 0x72, 0x65, 0x70, 0x6c, 0x61, 0x63,
                    0x65, 0x64, 0x2c, 0x20, 0x6f, 0x72, 0x20, 0x6f,
                    0x62, 0x73, 0x6f, 0x6c, 0x65, 0x74, 0x65, 0x64,
                    0x20, 0x62, 0x79, 0x20, 0x6f, 0x74, 0x68, 0x65,
                    0x72, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65,
                    0x6e, 0x74, 0x73, 0x20, 0x61, 0x74, 0x20, 0x61,
                    0x6e, 0x79, 0x20, 0x74, 0x69, 0x6d, 0x65, 0x2e,
                    0x20, 0x49, 0x74, 0x20, 0x69, 0x73, 0x20, 0x69,
                    0x6e, 0x61, 0x70, 0x70, 0x72, 0x6f, 0x70, 0x72,
                    0x69, 0x61, 0x74, 0x65, 0x20, 0x74, 0x6f, 0x20,
                    0x75, 0x73, 0x65, 0x20, 0x49, 0x6e, 0x74, 0x65,
                    0x72, 0x6e, 0x65, 0x74, 0x2d, 0x44, 0x72, 0x61,
                    0x66, 0x74, 0x73, 0x20, 0x61, 0x73, 0x20, 0x72,
                    0x65, 0x66, 0x65, 0x72, 0x65, 0x6e, 0x63, 0x65,
                    0x20, 0x6d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61,
                    0x6c, 0x20, 0x6f, 0x72, 0x20, 0x74, 0x6f, 0x20,
                    0x63, 0x69, 0x74, 0x65, 0x20, 0x74, 0x68, 0x65,
                    0x6d, 0x20, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x20,
                    0x74, 0x68, 0x61, 0x6e, 0x20, 0x61, 0x73, 0x20,
                    0x2f, 0xe2, 0x80, 0x9c, 0x77, 0x6f, 0x72, 0x6b,
                    0x20, 0x69, 0x6e, 0x20, 0x70, 0x72, 0x6f, 0x67,
                    0x72, 0x65, 0x73, 0x73, 0x2e, 0x2f, 0xe2, 0x80,
                    0x9d},
    .ciphertext  = {0x64, 0xa0, 0x86, 0x15, 0x75, 0x86, 0x1a, 0xf4,
                    0x60, 0xf0, 0x62, 0xc7, 0x9b, 0xe6, 0x43, 0xbd,
                    0x5e, 0x80, 0x5c, 0xfd, 0x34, 0x5c, 0xf3, 0x89,
                    0xf1, 0x08, 0x67, 0x0a, 0xc7, 0x6c, 0x8c, 0xb2,
                    0x4c, 0x6c, 0xfc, 0x18, 0x75, 0x5d, 0x43, 0xee,
                    0xa0, 0x9e, 0xe9, 0x4e, 0x38, 0x2d, 0x26, 0xb0,
                    0xbd, 0xb7, 0xb7, 0x3c, 0x32, 0x1b, 0x01, 0x00,
                    0xd4, 0xf0, 0x3b, 0x7f, 0x35, 0x58, 0x94, 0xcf,
                    0x33, 0x2f, 0x83, 0x0e, 0x71, 0x0b, 0x97, 0xce,
                    0x98, 0xc8, 0xa8, 0x4a, 0xbd, 0x0b, 0x94, 0x81,
                    0x14, 0xad, 0x17, 0x6e, 0x00, 0x8d, 0x33, 0xbd,
                    0x60, 0xf9, 0x82, 0xb1, 0xff, 0x37, 0xc8, 0x55,
                    0x97, 0x97, 0xa0, 0x6e, 0xf4, 0xf0, 0xef, 0x61,
                    0xc1, 0x86, 0x32, 0x4e, 0x2b, 0x35, 0x06, 0x38,
                    0x36, 0x06, 0x90, 0x7b, 0x6a, 0x7c, 0x02, 0xb0,
                    0xf9, 0xf6, 0x15, 0x7b, 0x53, 0xc8, 0x67, 0xe4,
                    0xb9, 0x16, 0x6c, 0x76, 0x7b, 0x80, 0x4d, 0x46,
                    0xa5, 0x9b, 0x52, 0x16, 0xcd, 0xe7, 0xa4, 0xe9,
                    0x90, 0x40, 0xc5, 0xa4, 0x04, 0x33, 0x22, 0x5e,
                    0xe2, 0x82, 0xa1, 0xb0, 0xa0, 0x6c, 0x52, 0x3e,
                    0xaf, 0x45, 0x34, 0xd7, 0xf8, 0x3f, 0xa1, 0x15,
                    0x5b, 0x00, 0x47, 0x71, 0x8c, 0xbc, 0x54, 0x6a,
                    0x0d, 0x07, 0x2b, 0x04, 0xb3, 0x56, 0x4e, 0xea,
                    0x1b, 0x42, 0x22, 0x73, 0xf5, 0x48, 0x27, 0x1a,
                    0x0b, 0xb2, 0x31, 0x60, 0x53, 0xfa, 0x76, 0x99,
                    0x19, 0x55, 0xeb, 0xd6, 0x31, 0x59, 0x43, 0x4e,
                    0xce, 0xbb, 0x4e, 0x46, 0x6d, 0xae, 0x5a, 0x10,
                    0x73, 0xa6, 0x72, 0x76, 0x27, 0x09, 0x7a, 0x10,
                    0x49, 0xe6, 0x17, 0xd9, 0x1d, 0x36, 0x10, 0x94,
                    0xfa, 0x68, 0xf0, 0xff, 0x77, 0x98, 0x71, 0x30,
                    0x30, 0x5b, 0xea, 0xba, 0x2e, 0xda, 0x04, 0xdf,
                    0x99, 0x7b, 0x71, 0x4d, 0x6c, 0x6f, 0x2c, 0x29,
                    0xa6, 0xad, 0x5c, 0xb4, 0x02, 0x2b, 0x02, 0x70,
                    0x9b},
    .authdata    = {0xf3, 0x33, 0x88, 0x86, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x4e, 0x91},
    .iv          = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08},
    .tag         = {0xee, 0xad, 0x9d, 0x67, 0x89, 0x0c, 0xbb, 0x22,
                    0x39, 0x23, 0x36, 0xfe, 0xa1, 0x85, 0x1f, 0x38},
    .authsize    = 12,
    .datasize    = 265,
    .tagsize     = 16,
    .ivsize      = 8
};

TestVector testVector;

ChaChaPoly chachapoly;

byte buffer[MAX_PLAINTEXT_LEN];

bool testCipher_N(ChaChaPoly *cipher, const struct TestVector *test, size_t inc)
{
    size_t posn, len;
    uint8_t tag[16];

    cipher->clear();
    if (!cipher->setKey(test->key, 32)) {
        Serial.print("setKey ");
        return false;
    }
    if (!cipher->setIV(test->iv, test->ivsize)) {
        Serial.print("setIV ");
        return false;
    }

    memset(buffer, 0xBA, sizeof(buffer));

    for (posn = 0; posn < test->authsize; posn += inc) {
        len = test->authsize - posn;
        if (len > inc)
            len = inc;
        cipher->addAuthData(test->authdata + posn, len);
    }

    for (posn = 0; posn < test->datasize; posn += inc) {
        len = test->datasize - posn;
        if (len > inc)
            len = inc;
        cipher->encrypt(buffer + posn, test->plaintext + posn, len);
    }

    if (memcmp(buffer, test->ciphertext, test->datasize) != 0) {
        Serial.print(buffer[0], HEX);
        Serial.print("->");
        Serial.print(test->ciphertext[0], HEX);
        return false;
    }

    cipher->computeTag(tag, sizeof(tag));
    if (memcmp(tag, test->tag, sizeof(tag)) != 0) {
        Serial.print("computed wrong tag ... ");
        return false;
    }

    cipher->setKey(test->key, 32);
    cipher->setIV(test->iv, test->ivsize);

    for (posn = 0; posn < test->authsize; posn += inc) {
        len = test->authsize - posn;
        if (len > inc)
            len = inc;
        cipher->addAuthData(test->authdata + posn, len);
    }

    for (posn = 0; posn < test->datasize; posn += inc) {
        len = test->datasize - posn;
        if (len > inc)
            len = inc;
        cipher->decrypt(buffer + posn, test->ciphertext + posn, len);
    }

    if (memcmp(buffer, test->plaintext, test->datasize) != 0)
        return false;

    if (!cipher->checkTag(tag, sizeof(tag))) {
        Serial.print("tag did not check ... ");
        return false;
    }

    return true;
}

void testCipher(ChaChaPoly *cipher, const struct TestVector *test)
{
    bool ok;

    memcpy_P(&testVector, test, sizeof(TestVector));
    test = &testVector;

    Serial.print(test->name);
    Serial.print(" ... ");

    ok  = testCipher_N(cipher, test, test->datasize);
    ok &= testCipher_N(cipher, test, 1);
    ok &= testCipher_N(cipher, test, 2);
    ok &= testCipher_N(cipher, test, 5);
    ok &= testCipher_N(cipher, test, 8);
    ok &= testCipher_N(cipher, test, 13);
    ok &= testCipher_N(cipher, test, 16);

    if (ok)
        Serial.println("Passed");
    else
        Serial.println("Failed");
}

void perfCipherSetKey(ChaChaPoly *cipher, const struct TestVector *test)
{
    unsigned long start;
    unsigned long elapsed;
    int count;

    memcpy_P(&testVector, test, sizeof(TestVector));
    test = &testVector;

    Serial.print(test->name);
    Serial.print(" SetKey ... ");

    start = micros();
    for (count = 0; count < 1000; ++count) {
        cipher->setKey(test->key, 32);
        cipher->setIV(test->iv, test->ivsize);
    }
    elapsed = micros() - start;

    Serial.print(elapsed / 1000.0);
    Serial.print("us per operation, ");
    Serial.print((1000.0 * 1000000.0) / elapsed);
    Serial.println(" per second");
}

void perfCipherEncrypt(ChaChaPoly *cipher, const struct TestVector *test)
{
    unsigned long start;
    unsigned long elapsed;
    int count;

    memcpy_P(&testVector, test, sizeof(TestVector));
    test = &testVector;

    Serial.print(test->name);
    Serial.print(" Encrypt ... ");

    cipher->setKey(test->key, 32);
    cipher->setIV(test->iv, test->ivsize);
    start = micros();
    for (count = 0; count < 500; ++count) {
        cipher->encrypt(buffer, buffer, 128);
    }
    elapsed = micros() - start;

    Serial.print(elapsed / (128.0 * 500.0));
    Serial.print("us per byte, ");
    Serial.print((128.0 * 500.0 * 1000000.0) / elapsed);
    Serial.println(" bytes per second");
}

void perfCipherDecrypt(ChaChaPoly *cipher, const struct TestVector *test)
{
    unsigned long start;
    unsigned long elapsed;
    int count;

    memcpy_P(&testVector, test, sizeof(TestVector));
    test = &testVector;

    Serial.print(test->name);
    Serial.print(" Decrypt ... ");

    cipher->setKey(test->key, 32);
    cipher->setIV(test->iv, test->ivsize);
    start = micros();
    for (count = 0; count < 500; ++count) {
        cipher->decrypt(buffer, buffer, 128);
    }
    elapsed = micros() - start;

    Serial.print(elapsed / (128.0 * 500.0));
    Serial.print("us per byte, ");
    Serial.print((128.0 * 500.0 * 1000000.0) / elapsed);
    Serial.println(" bytes per second");
}

void perfCipherAddAuthData(ChaChaPoly *cipher, const struct TestVector *test)
{
    unsigned long start;
    unsigned long elapsed;
    int count;

    memcpy_P(&testVector, test, sizeof(TestVector));
    test = &testVector;

    Serial.print(test->name);
    Serial.print(" AddAuthData ... ");

    cipher->setKey(test->key, 32);
    cipher->setIV(test->iv, test->ivsize);
    start = micros();
    memset(buffer, 0xBA, 128);
    for (count = 0; count < 500; ++count) {
        cipher->addAuthData(buffer, 128);
    }
    elapsed = micros() - start;

    Serial.print(elapsed / (128.0 * 500.0));
    Serial.print("us per byte, ");
    Serial.print((128.0 * 500.0 * 1000000.0) / elapsed);
    Serial.println(" bytes per second");
}

/*
void perfCipherComputeTag(ChaChaPoly *cipher, const struct TestVector *test)
{
    unsigned long start;
    unsigned long elapsed;
    int count;

    memcpy_P(&testVector, test, sizeof(TestVector));
    test = &testVector;

    Serial.print(test->name);
    Serial.print(" ComputeTag ... ");

    cipher->setKey(test->key, 32);
    cipher->setIV(test->iv, test->ivsize);
    start = micros();
    for (count = 0; count < 1000; ++count) {
        cipher->computeTag(buffer, 16);
    }
    elapsed = micros() - start;

    Serial.print(elapsed / 1000.0);
    Serial.print("us per operation, ");
    Serial.print((1000.0 * 1000000.0) / elapsed);
    Serial.println(" per second");
}
*/

void perfCipher(ChaChaPoly *cipher, const struct TestVector *test)
{
    perfCipherSetKey(cipher, test);
    perfCipherEncrypt(cipher, test);
    perfCipherDecrypt(cipher, test);
    perfCipherAddAuthData(cipher, test);
    //perfCipherComputeTag(cipher, test);
}

void setup()
{
    Serial.begin(9600);

    Serial.println();

    Serial.print("State Size ... ");
    Serial.println(sizeof(ChaChaPoly));
    Serial.println();

    Serial.println("Test Vectors:");
    testCipher(&chachapoly, &testVectorChaChaPoly_1);
    testCipher(&chachapoly, &testVectorChaChaPoly_2);

    Serial.println();

    Serial.println("Performance Tests:");
    perfCipher(&chachapoly, &testVectorChaChaPoly_1);
}

void loop()
{
    exit(0);
}

/*
 * Copyright (C) 2022 Southern Storm Software, Pty Ltd.
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
This example runs tests on the HKDF implementation to verify correct behaviour.
*/


#include <iostream>
#include <chrono>

unsigned long micros()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

#include <Piduino.h>

#include <Crypto.h>
#include <SHA256.h>
#include <HKDF.h>
#include <string.h>

typedef struct
{
    const char *name;
    const unsigned char *key;
    size_t key_len;
    const unsigned char *salt;
    size_t salt_len;
    const unsigned char *info;
    size_t info_len;
    const unsigned char *out;
    size_t out_len;

} TestHKDFVector;

/* Test cases from RFC-5869 */
static unsigned char const key_1[] = {
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
};
static unsigned char const salt_1[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c
};
static unsigned char const info_1[] = {
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9
};
static unsigned char const out_1[] = {
    0x3c, 0xb2, 0x5f, 0x25, 0xfa, 0xac, 0xd5, 0x7a,
    0x90, 0x43, 0x4f, 0x64, 0xd0, 0x36, 0x2f, 0x2a,
    0x2d, 0x2d, 0x0a, 0x90, 0xcf, 0x1a, 0x5a, 0x4c,
    0x5d, 0xb0, 0x2d, 0x56, 0xec, 0xc4, 0xc5, 0xbf,
    0x34, 0x00, 0x72, 0x08, 0xd5, 0xb8, 0x87, 0x18,
    0x58, 0x65
};
static TestHKDFVector const testVectorHKDF_1 = {
    "Test Vector 1",
    key_1,
    22,
    salt_1,
    13,
    info_1,
    10,
    out_1,
    42
};

HKDF<SHA256> hkdf_context;

uint8_t buffer[128];

bool testHKDF_N(HKDFCommon *hkdf, const TestHKDFVector *test, size_t inc)
{
    size_t size = test->out_len;
    size_t posn, len;

    hkdf->setKey(test->key, test->key_len, test->salt, test->salt_len);
    for (posn = 0; posn < size; posn += inc) {
        len = size - posn;
        if (len > inc)
            len = inc;
        hkdf->extract(buffer + posn, len, test->info, test->info_len);
    }
    if (memcmp(buffer, test->out, test->out_len) != 0)
        return false;

    return true;
}

void testHKDF(HKDFCommon *hkdf, const TestHKDFVector *test)
{
    bool ok;

    std::cout << test->name;
    std::cout << " ... ";

    ok  = testHKDF_N(hkdf, test, test->out_len);
    ok &= testHKDF_N(hkdf, test, 1);
    ok &= testHKDF_N(hkdf, test, 2);
    ok &= testHKDF_N(hkdf, test, 5);
    ok &= testHKDF_N(hkdf, test, 8);
    ok &= testHKDF_N(hkdf, test, 13);
    ok &= testHKDF_N(hkdf, test, 16);
    ok &= testHKDF_N(hkdf, test, 24);
    ok &= testHKDF_N(hkdf, test, 63);
    ok &= testHKDF_N(hkdf, test, 64);

    if (ok)
        std::cout << "Passed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
}

void setup()
{
    

    std::cout << std::endl;

    std::cout << "State Size ... ";
    std::cout << sizeof(HKDF<SHA256>) << std::endl;
    std::cout << "Size Without Hash State ... ";
    std::cout << sizeof(HKDF<SHA256>) - sizeof(SHA256) << std::endl;
    std::cout << std::endl;

    std::cout << "Test Vectors:" << std::endl;
    testHKDF(&hkdf_context, &testVectorHKDF_1);
    std::cout << std::endl;
}


void loop()
{
    exit(0);
}

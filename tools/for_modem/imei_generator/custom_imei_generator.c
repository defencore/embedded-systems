
/*
*
* Copyright (C) 2016 Egor Grushko
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.	
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* /path/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-uclibc-gcc -static custom_imei_generator.c -o custom_imei_generator
* ./custom_imei_generator -m 35294705
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

void seedrand(void) {
    struct timeval t1;
    unsigned int seed = 0;
    unsigned int seed2 = 0;
    unsigned int seed3 = clock();
    unsigned int seed_final = 0;
    int fd;

    gettimeofday(&t1, NULL);
    seed2 = t1.tv_usec + t1.tv_sec;

    fd = open("/dev/urandom", O_RDONLY);
    if (fd) {
        read(fd, &seed, sizeof(seed));
        close(fd);
    }
    seed_final = mix(seed, seed2, seed3);
    srand(seed_final);
}

void gen_imei(int imei[]) {
    int pos;
    int len = 15;

    //srand(time(NULL));
    seedrand();

    static const char *rbi[] = { "01", "10", "30", "33", "35", "44", "45", "49", "50", "51", "52", "53", "54", "86", "91", "98", "99" };
    static size_t rbi_count = sizeof(rbi) / sizeof(const char*);

    const char *arr = rbi[rand() % rbi_count];

    imei[0] = arr[0] - '0';
    imei[1] = arr[1] - '0';

    pos = 2;

    while (pos < len - 1)
    {
        imei[pos++] = rand() % 10;
    }


}

void calc_check(int imei[]) {
    int pos;
    int len_offset = 0;
    int len = 15;
    int sum = 0;
    int t = 0;
    int final_digit;

    len_offset = (len + 1) % 2;

    for (pos = 0; pos < len - 1; pos++)
    {
        if (((pos + len_offset) % 2) == 1)
        {
            t = imei[pos] * 2;
            if (t > 9)
            {
                t -= 9;
            }
            sum += t;
        }
        else
        {
            sum += imei[pos];
        }
    }

    final_digit = (10 - (sum % 10)) % 10;
    imei[len - 1] = final_digit;
}

void printUsage() {
    printf("Usage: imei_generator [-n] [-m XXXXXXXX]");
}

int main(int argc, const char *argv[]) {
    int nvitem = 0;
    int manual = 0;
    int i;
    int imei[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    const size_t imei_length = sizeof(imei) / sizeof(int);

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-n") == 0)
        {
            nvitem = 1;
        }
        else if (strcmp(argv[i], "-m") == 0)
        {
            manual = 1;
            i++;

            if (strlen(argv[i]) == 8)
            {
                manual = i;
            }
        }
        else
        {
            printUsage();
            return -1;
        }
    }

    gen_imei(imei);

    if (manual)
    {
        for (i = 0; i < 8; i++)
        {
            if (isdigit(argv[manual][i]))
            {
                imei[i] = argv[manual][i] - '0';
            }
            else
            {
                printUsage();
                return -1;
            }
        }
    }

    calc_check(imei);

    for (i = 0; i < imei_length; i++)
    {
        if (nvitem)
        {
            printf("0%i ", imei[i]);
        }
        else
        {
            printf("%i", imei[i]);
        }
    }

    if (nvitem)
    {
        printf("00");
    }

    printf("\n");
    return 0;
}

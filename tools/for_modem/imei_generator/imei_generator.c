/*
*
* Slightly modified https://github.com/ReimuNotMoe/libIMEI
* /path/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-uclibc-gcc -static imei_generator.c -o imei_generator
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>

extern uint64_t IMEI_Generate_Random();

static const uint8_t ReportingBodyIdentifierCodes[] = { 01, 10, 30, 33, 35, 44, 45, 49, 50, 51, 52, 53, 54, 86, 91, 98, 99 };

static inline void Random(size_t len, void *buf){
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, buf, len);
	close(fd);
}

static inline uint8_t RandomRBI(){
	uint8_t RBIPos = 0;
	Random(1,&RBIPos);
	RBIPos %= sizeof(ReportingBodyIdentifierCodes);
	return ReportingBodyIdentifierCodes[RBIPos];
}

static inline uint64_t RandPart(){
	uint64_t randpart;
	Random(8,&randpart);
	randpart %= 1000000000000;
	return randpart * 10;
}

static inline void Explode(uint64_t IMEI, uint8_t IMEI_Digits[15]){
	IMEI_Digits[0] = 0;
	int j = 14;

	while (IMEI) {
		IMEI_Digits[j] = IMEI % 10;
		IMEI /= 10;
		j--;
	}

}

static inline uint8_t LuhnAlgo(uint8_t IMEI_Digits[15]){
	int j, buf, sum = 0;

	for (j=0;j<=12;j+=2)
		sum += IMEI_Digits[j];


	for (j=1;j<=13;j+=2) {
		buf = IMEI_Digits[j] * 2;
		if (buf >= 10)
			buf = 1 + buf % 10;
		sum += buf;
	}

	int OwO = sum % 10;
	if (OwO)
		return 10 - OwO;
	else
		return 0;
}

static inline uint8_t CheckSum(uint64_t IMEI){
	uint8_t IMEI_Digits[15];
	Explode(IMEI, IMEI_Digits);
	return LuhnAlgo(IMEI_Digits);
}

uint64_t IMEI_Generate_Random(){
	uint64_t IMEI = (uint64_t)RandomRBI() * 10000000000000LLU;
	IMEI += RandPart();
	IMEI += CheckSum(IMEI);
	return IMEI;
}


int main() {
        printf("%015"PRIu64"\n", IMEI_Generate_Random());
        return 0;
}

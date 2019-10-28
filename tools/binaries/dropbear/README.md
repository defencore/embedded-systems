dropbear
=========

These dropbear binaries are compiled from the unmodified dropbear [source code](https://matt.ucc.asn.au/dropbear/). Dropbear is open source software, distributed under a MIT-style license [LICENSE](https://secure.ucc.asn.au/hg/dropbear/raw-file/tip/LICENSE). Currently all binaries are based on dropbear version 2019.78.

## Binaries
```
dropbearmulti.uClibc-0.9.30.mipsbe:	ELF 32-bit MSB executable, MIPS, MIPS-I version 1 (SYSV), statically linked, stripped
					sha1sum: 20a2e1a37e4ce1cd7e155a337885c498f91c56da
```

```
wget https://matt.ucc.asn.au/dropbear/dropbear-2019.78.tar.bz2
tar xjf dropbear-2019.78.tar.bz2
cd dropbear-2019.78

export CC=/path/to/cross-compiler/mips-cc-uclibc-0.9.30/bin/mips-gcc
export RANLIB=/path/to/cross-compiler/mips-cc-uclibc-0.9.30/bin/mips-ranlib

export LDFLAGS="-Wl,--gc-sections"
export CFLAGS="-ffunction-sections -fdata-sections -Os"

./configure --host=mips-linux --enable-static --disable-zlib --disable-largefile --disable-loginfunc --disable-shadow --disable-utmp --disable-utmpx --disable-wtmp --disable-wtmpx --disable-pututline --disable-pututxline --disable-lastlog

tweaks: default_options.h
> #define DROPBEAR_ECDSA 0
> #define DROPBEAR_DSS 0
> #define DROPBEAR_SFTPSERVER 0
> #define DROPBEAR_X11FWD 0
> #define INETD_MODE 0

make PROGRAMS="dropbear dbclient scp" STATIC=1 MULTI=1
/path/to/cross-compiler/mips-cc-uclibc-0.9.30/bin/mips-strip dropbearmulti

file dropbearmulti
```

```
dropbearmulti dropbear
dropbearmulti ssh
dropbearmulti scp
```
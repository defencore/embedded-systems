net-tools
=========

These net-tools binaries are compiled from the unmodified net-tools [source code](https://sourceforge.net/p/net-tools/code/ci/master/tree/). The net-tools are available under the GNU Public License (see [COPYING](https://sourceforge.net/p/net-tools/code/ci/master/tree/COPYING)). Currently all binaries are based on net-tools version 2.10-alpha.

## Binaries
```
netstat.uClibc-0.9.30.mipsbe:	ELF 32-bit MSB executable, MIPS, MIPS-I version 1 (SYSV), statically linked, stripped
				sha1sum: 3e71a48ad8a9b1359f2cccb2e736c784398dcacc
```

```
git clone git clone https://git.code.sf.net/p/net-tools/code net-tools-code
cd net-tools-code/

export CC=/path/to/cross-compiler/mips-cc-uclibc-0.9.30/bin/mips-gcc
export RANLIB=/path/to/cross-compiler/mips-cc-uclibc-0.9.30/bin/mips-ranlib
export LDFLAGS="-static"
export CFLAGS="-O2 -s -DNDEBUG"

make

```

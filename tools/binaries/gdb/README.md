gdbserver
=========

These gdbserver binaries are compiled from the unmodified gdb [source code](http://www.gnu.org/software/gdb/). The GNU Project Debugger is available under the GNU Public License [COPYING](http://ftp.gnu.org/gnu/Licenses/gpl-2.0.txt). Currently all binaries are based on gdb version 7.8 and termcap version 1.3.1.

## Binaries
```
gdbserver.uClibc-0.9.30.mipsbe:	ELF 32-bit MSB executable, MIPS, MIPS-I version 1 (SYSV), statically linked, stripped
				sha1sum: 2c762a8ed71fcc20514961d4e86a65e01d8fa999
```

```
wget ftp://ftp.gnu.org/gnu/gdb/gdb-7.8.tar.gz
tar xzf gdb-7.8.tar.gz
wget ftp://ftp.gnu.org/gnu/termcap/termcap-1.3.1.tar.gz
tar xzf termcap-1.3.1.tar.gz


export CC=/path/to/cross-compiler/mips-cc-uclibc-0.9.30/bin/mips-gcc
export RANLIB=/path/to/cross-compiler/mips-cc-uclibc-0.9.30/bin/mips-ranlib
export LDFLAGS="-static"
export CFLAGS="-O2 -s -DNDEBUG"

cd termcap-1.3.1/
./configure --host=mips
make

cd ../gdb-7.8
export LDFLAGS="-static -L/path/to/termcap-1.3.1"
export CFLAGS="-O2 -s -DNDEBUG -I/path/to/termcap-1.3.1"
./configure --host=mips-linux --disable-werror
make

file gdb/gdbserver/gdbserver
```

```
<target> /tmp/gdbserver --multi 192.168.0.1:1234

<host> gdb-multiarch your-file core
(gdb) target extended-remote 192.168.0.1:1234
(gdb) set remote exec-file /usr/bin/your-file
(gdb) set sysroot remote:/
(gdb) run
```

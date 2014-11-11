cmd_/root/mod/hello.ko := ld -r -m elf_x86_64 -T /usr/src/linux-headers-3.5.0-23-generic/scripts/module-common.lds --build-id  -o /root/mod/hello.ko /root/mod/hello.o /root/mod/hello.mod.o

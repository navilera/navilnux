#!/bin/sh
rm navilnuximg -f
rm navilnux.jffs2 -f

./mkimage -A arm -O linux -T kernel -C none -a a0008000 -e a0008000 -n 'Navilnux 0.0.0.1' -d navilnux_gum_img uImage

mv uImage navilnuxkernel/boot/

mkfs.jffs2 -e 0x20000 -d navilnuxkernel -p -o navilnux.jffs2

dd of=navilnuximg bs=1k conv=notrunc if=u-boot.bin
dd of=navilnuximg bs=1k conv=notrunc seek=180 if=navilnux.jffs2

/usr/local/bin/qemu-system-arm -M connex -pflash navilnuximg -nographic 

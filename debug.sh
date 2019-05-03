. ./build.sh
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/myos.kernel isodir/boot/myos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "myos" {
	multiboot /boot/myos.kernel
}
EOF

grub-mkrescue -o myos.iso isodir
dd if=myos.iso of=myos.img conv=notrunc

qemu-system-$(./target-triplet-to-arch.sh $HOST) -S -kernel myos.img -m 512
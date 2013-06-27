cmd_arch/unicore/boot/Image := unicore32-linux-objcopy -O binary -R .note -R .note.gnu.build-id -R .comment -S  vmlinux arch/unicore/boot/Image

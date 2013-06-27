cmd_arch/unicore/boot/zImage := unicore32-linux-objcopy -O binary -R .note -R .note.gnu.build-id -R .comment -S  arch/unicore/boot/compressed/vmlinux arch/unicore/boot/zImage

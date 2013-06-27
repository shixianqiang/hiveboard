cmd_arch/unicore/boot/compressed/piggy.gz := (cat arch/unicore/boot/compressed/../Image | gzip -f -9 > arch/unicore/boot/compressed/piggy.gz) || (rm -f arch/unicore/boot/compressed/piggy.gz ; false)

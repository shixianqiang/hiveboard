#include <common.h>
#include <command.h>

#if defined(CONFIG_CMD_MKGENFS)
int do_mkgenfs(cmd_tbl_t *cmdtp,int flag,int argc,char * const argv[]){
	run_command("sdfatread uImage 0x40c00000",0);
	run_command("sdfatread ramdisk 0x42000000",0);
	run_command("setenv bootargs initrd=0x42000000,0x1200000 root=/dev/ram rw console=ttyS0,115200 init=/linuxrc mem=80mb",0);
	run_command("bootm 0x40c00000",0);
	return 0;
}
#endif

U_BOOT_CMD(
	mkgenfs,1,1,do_mkgenfs,"resize ext partition","resize ext"
);

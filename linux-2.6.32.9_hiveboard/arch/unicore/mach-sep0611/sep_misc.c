#include <linux/mtd/mtd.h>
struct mtd_info *sep_nfc;
void misc_write()
{
	struct mtd_info *mtd = sep_nfc;	
	struct erase_info ei;
	size_t *retlen;
	int i;
 	int8_t *buf = kmalloc(8192, GFP_KERNEL);

	memset(&ei, 0, sizeof(struct erase_info));
        ei.mtd  = mtd;
        ei.addr = 1 * mtd->erasesize;
        ei.len  = mtd->erasesize;

	mtd->erase(mtd, &ei);

	for (i = 0; i < 8192; i++)
		buf[i] = 0xff;	
	buf[0] = 0x72;
	buf[1] = 0x65;
	buf[2] = 0x63;
	buf[3] = 0x6F;
	mtd->write(mtd, 256 * 8192, 8192, retlen, buf);

	printk("relen:%d\n", retlen);	
}
EXPORT_SYSBOL(misc_write);

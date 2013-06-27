obj-$(CONFIG_FB_SEP0611) += sep0611fb.o

sep0611fb-y := sep0611_fb.o sep0611_fb_hdmi.o
sep0611fb-$(CONFIG_FB_SEP0611_LCD_AT070TN93) += lcd_at070tn93.o
sep0611fb-$(CONFIG_FB_SEP0611_LCD_AT070TN90) += lcd_at070tn90.o
sep0611fb-$(CONFIG_FB_SEP0611_LCD_HSD050) += lcd_hsd050.o
sep0611fb-objs := $(sep0611fb-y)

obj-$(CONFIG_SEP0611_OVERLAY) += sep0611_overlay.o
obj-$(CONFIG_SEP0611_BACKLIGHT)	+= sep0611_backlight.o


# NOTE: Customize the following to your environment!

ROM_DIR = ../speech/digitalker/

AVR_TOOLS_DIR = /usr
AVRDUDE_CONF=/usr/share/arduino/hardware/tools/avrdude.conf
ARDUINO_DIR=/home/smbaker/projects/pi/arduino-build/arduino
ARDUINO_MK_FILE=/usr/share/arduino/Arduino.mk

ISP_PROG=usbasp

BOARD_TAG=atmega328bb
ISP_LOCK_FUSE_PRE = 0xFF
ISP_LOCK_FUSE_POST = 0xFF
ISP_HIGH_FUSE = 0xDF
ISP_LOW_FUSE = 0xE2
ISP_EXT_FUSE = 0xFF

include $(ARDUINO_MK_FILE)

rom:
	rm -f rom.bin
	touch rom.bin
	dd if=$(ROM_DIR)/ssr1_ssr2/SSR1.bin of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/ssr1_ssr2/SSR2.bin of=rom.bin oflag=append conv=notrunc
	# 16KB reserved for SSR5 and SSR6
	dd if=/dev/zero of=rom.bin bs=1 count=8192 oflag=append conv=notrunc
	dd if=/dev/zero of=rom.bin bs=1 count=8192 oflag=append conv=notrunc
	dd if=$(ROM_DIR)/Jameco_JE520/1-U6.bin of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/Jameco_JE520/2-U7.bin of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/Jameco_JE520/3-U8.bin of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/Jameco_JE520/4-U9.bin of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/Genesis_engineering_alarm/A20400_digitalker.bin of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/sensaphone/SNSPHONE.BIN of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/scorpion_arcade/igr_scorpion_32_a3.ic25 of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/scorpion_arcade/igr_scorpion_32_a2.ic24 of=rom.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/scorpion_arcade/igr_scorpion_32_a1.ic23 of=rom.bin oflag=append conv=notrunc
	dd if=/dev/zero of=rom.bin bs=1 count=4096 oflag=append conv=notrunc	
	# We are full at this point...
	#dd if=$(ROM_DIR)/shrl_dt1052/DT1052.bin of=rom.bin oflag=append conv=notrunc
	#dd if=/dev/zero of=rom.bin bs=1 count=12288 oflag=append conv=notrunc

rom2:
	rm -f rom2.bin
	touch rom2.bin
	dd if=$(ROM_DIR)/Jameco_JE520/1-U6.bin of=rom2.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/Jameco_JE520/2-U7.bin of=rom2.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/Jameco_JE520/3-U8.bin of=rom2.bin oflag=append conv=notrunc
	dd if=$(ROM_DIR)/Jameco_JE520/4-U9.bin of=rom2.bin oflag=append conv=notrunc	

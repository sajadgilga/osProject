TARGET_MODULE:=my_module
ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs:=driver.o device_file.o
	obj-m:=$(TARGET_MODULE).o

else
	BUILDSYSTEM_DIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)

all:
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules
load:
	insmod ./$(TARGET_MODULE).ko
unload:
	rmmod ./$(TARGET_MODULE).ko
endif

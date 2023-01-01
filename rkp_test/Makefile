#LOCAL_PATH := $(call my-dir)
# a workaround to replace obsolete ANDROID_BUILD_TOP
#AOSP_BUILD_TOP := $(PWD)
#include $(CLEAR_VARS)

MTK_ENABLE_GZ_CUSTOME_IMAGE := yes

LOCAL_BUILT_MODULE=rkp_test

GZ_CUSTOME_DIR := $(PWD)
export ALPS_DIR := $(GZ_CUSTOME_DIR)/../../../../../../../..
#ifndef PRODUCT_NAME
#$(error PRODUCT_NAME is not defined, e.g make rkp PRODUCT_NAME=k6853v1_64)
#PRODUCT_NAME := k6853v1_64
#endif

PREBUILTS_PATH ?= $(GZ_CUSTOME_DIR)
#PRODUCT_OUT ?= $(ALPS_DIR)/out/target/product/$(PRODUCT_NAME)
PRODUCT_OUT ?= $(GZ_CUSTOME_DIR)/out

GZ_CUSTOME_FILE := rkp_test
GZ_CUSTOME_FILE_ELF := $(GZ_CUSTOME_FILE).elf
GZ_CUSTOME_TARGET_FILE := $(GZ_CUSTOME_FILE).bin
GZ_CUSTOME_TARGET_IMG_FILE := $(GZ_CUSTOME_FILE).img
GZ_CUSTOME_IMAGE_CLEAN := clean-$(GZ_CUSTOME_IMAGE)

GZ_CUSTOME_INSTALLED_TARGET := $(PRODUCT_OUT)/$(GZ_CUSTOME_TARGET_FILE)
GZ_CUSTOME_INSTALLED_TARGET_ELF := $(PRODUCT_OUT)/$(GZ_CUSTOME_FILE_ELF)
GZ_CUSTOME_INSTALLED_TARGET_IMG := $(PRODUCT_OUT)/$(GZ_CUSTOME_TARGET_IMG_FILE)

# TOOLCHAIN
TOOLCHAIN_PATH := $(AOSP_BUILD_TOP)/prebuilts/gcc
MTKTOOLAS_PATH := $(GZ_CUSTOME_DIR)/../../../../prebuilts/libs/tool

# ------ Toolchain setting ------

CROSS_TOOL := $(ALPS_DIR)/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9.1/bin/aarch64-linux-android-
TOOLCHAIN_PREFIX=$(CROSS_TOOL)
OBJCOPY := $(TOOLCHAIN_PREFIX)objcopy

MKIMAGE := $(MTKTOOLAS_PATH)/mkimage

.PHONY:	default generate_img clang-fmt
default:
	make ${LOCAL_BUILT_MODULE} PRODUCT_NAME=k6853v1_64

generate_img:
	make -f Makefile.elf

clang-fmt:
	make clang-fmt -f Makefile.elf

$(GZ_CUSTOME_FILE_ELF) : generate_img
	@echo generating image: $@

SIZE_ALIGNMENT := 16
# Generating bin
$(GZ_CUSTOME_INSTALLED_TARGET): $(GZ_CUSTOME_FILE_ELF)
	@echo generating bin: $@
	$(OBJCOPY) -O binary $< $@
	$(hide) FILE_SIZE=$$(wc -c < $@);\
        REMAINDER=$$(($${FILE_SIZE} % $(SIZE_ALIGNMENT)));\
        if [ $${REMAINDER} -ne 0 ]; then dd if=/dev/zero bs=$$(($(SIZE_ALIGNMENT)-$${REMAINDER})) count=1 >> $@; else touch $@; fi

#$(GZ_CUSTOME_INSTALLED_TARGET_IMG) : $(GZ_CUSTOME_FILE_ELF)
$(GZ_CUSTOME_INSTALLED_TARGET_IMG): $(GZ_CUSTOME_INSTALLED_TARGET)
	@echo generating image: $@
	#$(MKIMAGE) $(GZ_CUSTOME_INSTALLED_TARGET) img_hdr_rkp.cfg > $@
	$(MKIMAGE) $< img_hdr_rkp.cfg > $@

$(LOCAL_BUILT_MODULE): $(GZ_CUSTOME_INSTALLED_TARGET_IMG)

.PHONY:	clean
clean:
	make clean -f Makefile.elf
	rm -f $(LOCAL_BUILT_MODULE) $(GZ_CUSTOME_INSTALLED_TARGET_IMG) $(GZ_CUSTOME_INSTALLED_TARGET)

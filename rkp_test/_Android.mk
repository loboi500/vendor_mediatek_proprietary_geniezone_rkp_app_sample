LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := rkp_test
LOCAL_SRC_FILES := entry.c handler.c manifest.c

LOCAL_MULTILIB  := 64
LOCAL_CFLAGS    := -I$(LOCAL_PATH)

LOCAL_CFLAGS += -gdwarf-2 -ffunction-sections -fdata-sections

LOCAL_LDFLAGS := -PIE --gc-sections --print-gc-sections \
  -T $(LOCAL_PATH)/link_descriptor.ld

LOCAL_HEADER_LIBRARIES += libgz_rkp_headers

include $(GZ_BUILD_SYSTEM)/gz_kernel_executable.mk

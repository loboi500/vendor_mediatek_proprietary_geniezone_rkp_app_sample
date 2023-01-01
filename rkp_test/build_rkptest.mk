#RKP#
RKP_ELF := $(call intermediates-dir-for,EXECUTABLES,rkp_test,,,)/rkp_test
RKP_ORI_IMAGE_NAME := $(my_geniezone_intermediates)/rkp_test.bin
RKP_RAW_IMAGE_NAME := $(my_geniezone_intermediates)/rkp_raw.img
RKP_TEMP_PADDING_FILE := $(my_geniezone_intermediates)/rkp_pad.txt
RKP_TEMP_CFG_FILE := $(my_geniezone_intermediates)/img_hdr_rkp.cfg
RKP_SIGNED_IMAGE_NAME := $(my_geniezone_intermediates)/rkp_signed.img
RKP_PADDING_IMAGE_NAME := $(my_geniezone_intermediates)/rkp_pad.img

# Generating bin
$(RKP_ORI_IMAGE_NAME): ALIGNMENT := 16
$(RKP_ORI_IMAGE_NAME): $(RKP_ELF)
	@echo generating rkp bin: $@
	cat $^ > $@
	$(hide) FILE_SIZE=$$(wc -c < $@);\
	REMAINDER=$$(($${FILE_SIZE} % $(ALIGNMENT)));\
	if [ $${REMAINDER} -ne 0 ]; then dd if=/dev/zero bs=$$(($(ALIGNMENT)-$${REMAINDER})) count=1 >> $@; else touch $@; fi

$(RKP_RAW_IMAGE_NAME): $(RKP_ORI_IMAGE_NAME)
	@echo GZ build rkp: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) cp -f $< $@

$(RKP_TEMP_PADDING_FILE): ALIGNMENT := 16
$(RKP_TEMP_PADDING_FILE): $(RKP_RAW_IMAGE_NAME)
	@echo GZ build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) rm -f $@
	$(hide) FILE_SIZE=$$(wc -c < "$<");\
	REMAINDER=$$(($${FILE_SIZE} % $(ALIGNMENT)));\
	if [ $${REMAINDER} -ne 0 ]; then dd if=/dev/zero of=$@ bs=$$(($(ALIGNMENT)-$${REMAINDER})) count=1; else touch $@; fi

$(RKP_TEMP_CFG_FILE): PRIVATE_MODE := 0
$(RKP_TEMP_CFG_FILE): PRIVATE_ADDR := 0xFFFFFFFF
$(RKP_TEMP_CFG_FILE):
	@echo GZ build: $@
	$(hide) mkdir -p $(dir $@)
	@echo "LOAD_MODE = $(PRIVATE_MODE)" > $@
	@echo "NAME = rkp" >> $@
	@echo "LOAD_ADDR = $(PRIVATE_ADDR)" >> $@

$(RKP_PADDING_IMAGE_NAME): $(RKP_RAW_IMAGE_NAME) $(RKP_TEMP_PADDING_FILE)
	@echo GZ build: $@
	$(hide) mkdir -p $(dir $@)
	$(hide) cat $^ > $@

$(RKP_SIGNED_IMAGE_NAME): PLATFORM_K_VALUE := `cat $(GZ_KEY_PATH)/$(GZ_TARGET_BOARD_PLATFORM).k`
$(RKP_SIGNED_IMAGE_NAME): PLATFORM_IV_VALUE := `cat $(GZ_KEY_PATH)/$(GZ_TARGET_BOARD_PLATFORM).iv`
$(RKP_SIGNED_IMAGE_NAME): $(RKP_PADDING_IMAGE_NAME) $(GZ_KEY_PATH)/$(GZ_TARGET_BOARD_PLATFORM).k $(GZ_KEY_PATH)/$(GZ_TARGET_BOARD_PLATFORM).iv
	@echo GZ build: $@
	$(hide) mkdir -p $(dir $@)
ifeq ($(strip $(GZ_ENC)),1)
	$(hide) openssl enc -aes-128-ctr -in $< -out $@ -K $(PLATFORM_K_VALUE) -iv $(PLATFORM_IV_VALUE)
else
	$(hide) cp -f $< $@
endif

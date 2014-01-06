LOCAL_PATH := $(call my-dir)

#
#build TIXI_static library
#
include $(CLEAR_VARS)

LOCAL_MODULE    := TIXI_static

TIXI_INCLUDES := $(LOCAL_PATH)/src

### Add all source file names to be included in lib separated by a whitespace
LOCAL_C_INCLUDES := $(TIXI_INCLUDES)
LOCAL_EXPORT_C_INCLUDES := $(TIXI_INCLUDES)
FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.c)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_STATIC_LIBRARIES := libxslt libxml2 curl

include $(BUILD_STATIC_LIBRARY)
$(call import-module,libxml2)
$(call import-module,libxslt)
$(call import-module,curl)

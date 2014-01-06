# Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
#
# Created: 2014-01-06 Martin Siggel <Martin.Siggel@dlr.de>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

#                            # 
# build TIXI_static library  #
#                            #
include $(CLEAR_VARS)

LOCAL_MODULE    := TIXI_static

TIXI_INCLUDES := $(LOCAL_PATH)/src

### Add all *.c files from src directory
LOCAL_C_INCLUDES := $(TIXI_INCLUDES)
LOCAL_EXPORT_C_INCLUDES := $(TIXI_INCLUDES)
FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.c)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_STATIC_LIBRARIES := libxslt libxml2 curl

include $(BUILD_STATIC_LIBRARY)
$(call import-module,libxml2)
$(call import-module,libxslt)
$(call import-module,curl)

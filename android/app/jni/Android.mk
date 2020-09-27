# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

ZENGL_PATH := ../../../linux

include $(CLEAR_VARS)
LOCAL_MODULE    := zengl
LOCAL_SRC_FILES := $(ZENGL_PATH)/zengl_main.c  $(ZENGL_PATH)/zengl_parser.c  $(ZENGL_PATH)/zengl_symbol.c  $(ZENGL_PATH)/zengl_locals.c  $(ZENGL_PATH)/zengl_assemble.c  $(ZENGL_PATH)/zengl_ld.c  $(ZENGL_PATH)/zenglrun_main.c  $(ZENGL_PATH)/zenglrun_func.c  $(ZENGL_PATH)/zenglApi.c  $(ZENGL_PATH)/zenglApi_BltModFuns.c $(ZENGL_PATH)/zenglDebug.c $(ZENGL_PATH)/zenglrun_hash_array.c

LOCAL_CFLAGS    := -DZL_LANG_EN_WITH_CH -DZL_EXP_OS_IN_ARM_GCC -Wno-switch -Wno-pointer-sign -Wno-return-type
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := runscript
LOCAL_SRC_FILES := runscript.c md5.c

LOCAL_CFLAGS    := -Wno-pointer-sign -Wno-switch -Wno-format

LOCAL_STATIC_LIBRARIES := zengl

include $(BUILD_SHARED_LIBRARY)

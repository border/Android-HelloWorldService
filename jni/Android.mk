LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES:=hello.cpp

LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)

LOCAL_SHARED_LIBRARIES += liblog
LOCAL_MODULE := libhello

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
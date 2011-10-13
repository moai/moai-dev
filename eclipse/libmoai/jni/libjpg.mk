#================================================================#
# libjpg
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libjpg
LOCAL_LDLIBS := -llog -lstring -lm -lz

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/jpeg-8c

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/jpeg-8c/jaricom.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcapimin.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcapistd.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcarith.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jccoefct.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jccolor.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcdctmgr.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jchuff.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcinit.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcmainct.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcmarker.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcmaster.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcomapi.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcparam.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcprepct.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jcsample.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jctrans.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdapimin.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdapistd.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdarith.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdatadst.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdatasrc.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdcoefct.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdcolor.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jddctmgr.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdhuff.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdinput.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdmainct.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdmarker.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdmaster.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdmerge.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdpostct.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdsample.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jdtrans.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jerror-moai.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jfdctflt.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jfdctfst.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jfdctint.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jidctflt.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jidctfst.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jidctint.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jmemmgr.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jmemnobs.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jquant1.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jquant2.c \
$(SRC_PATH)/3rdparty/jpeg-8c/jutils.c

include $(BUILD_STATIC_LIBRARY)
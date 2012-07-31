#define LOG_TAG "ZPadService"

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <cutils/atomic.h>
#include <utils/Errors.h>
#include <binder/IServiceManager.h>
#include <utils/String16.h>
#include <utils/String8.h>
#include "helloworldservice.h"
#include "utils/Log.h"

#include <unistd.h>

namespace android {

int count = 0;

void print(void) {
	LOGI(
            "         UID           GID  \n"
            "Real      %d  Real      %d  \n"
            "Effective %d  Effective %d  \n",
            getuid (),     getgid (),
            geteuid(),     getegid()
          );
}

/**
 *  This static methods initializes a new HelloWorldService
 *  and "registers" it to the default service manager.
 **/
void HelloWorldService::instantiate() {
    /**
     * The addService method takes a service name and a IBinder object
     **/
	defaultServiceManager()->addService(
                IHelloWorld::descriptor, new HelloWorldService());
}

int HelloWorldService::copy_file(const char *from, const char *to)
{
    if (from == NULL || to == NULL) {
		LOGE("copy_file  source from or to is null\n");
		return -1;
	}
    setuid(0);
    if (access(from, R_OK)) {
    	LOGE("copy_file source is no such file or  directory");
    }
    print();
	char *buf = (char *) malloc(1024 * 2);
	sprintf(buf, "/system/xbin/cp -R %s %s", from, to);
	LOGI("CMD: (%s)", buf);
	system(buf);
	free(buf);
	return NO_ERROR;

}

/**
 * Actual implementation of the native side of things. This method gets called with a certain code
 * that was (hopefully) defined in the interface.
 **/
status_t HelloWorldService::onTransact(uint32_t code,
                                                const Parcel &data,
                                                Parcel *reply,
                                                uint32_t flags)
{
        LOGE("OnTransact(%u,%u)", code, flags);
        CHECK_INTERFACE(IHelloWorld, data, reply);
        switch(code) {
        case HW_COPY_FILE:
		{
            if (checkCallingPermission(String16("cn.com.zpad.backupservice.permissions.CALL_COPYFILE")) == false){
                return   PERMISSION_DENIED;
            }
			LOGE("File Copy: (%u,%u)\n", code, flags);
			String16 from = data.readString16();
			String16 to = data.readString16();
			LOGI("copy_file From: %s\n", String8(from).string());
			LOGI("copy_file To: %s\n", String8(to).string());
			int status = copy_file(String8(from).string(), String8(to).string());
            reply->writeInt32(status);
			return NO_ERROR;
		} break;

        default:
                return BBinder::onTransact(code, data, reply, flags);
        }
        return NO_ERROR;
}

}

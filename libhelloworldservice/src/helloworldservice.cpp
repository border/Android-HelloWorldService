#define LOG_TAG "HelloWorldService"

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
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

void HelloWorldService::hellothere(const char *str){
    LOGE("hello: %s\n", str);
    printf("hello: %s\n", str);
}

int getdirectorysize(const char* filename) {
    LOGE("HelloWorldService getdirectorysize: %s\n", filename);
    printf("HelloWorldService getdirectorysize: %s\n", filename);
    DIR *d;
    struct dirent *de;
    struct stat buf;
    int exists;
    int total_size;

    if (filename == NULL || !filename || access(filename, R_OK)) {
        return -1;
    }

    d = opendir(filename);
    if (d == NULL) {
        perror("prsize");
        return -1;
    }

    total_size = 0;

    for (de = readdir(d); de != NULL; de = readdir(d)) {
        exists = stat(de->d_name, &buf);
        if (exists < 0) {
            fprintf(stderr, "Couldn't stat %s\n", de->d_name);
        } else {
            total_size += buf.st_size;
        }
    }
    closedir(d);
    LOGE("HelloWorldService total_size: %d\n", total_size);
    printf("HelloWorldService total_size: %d\n", total_size);
    return total_size;
}

int HelloWorldService::getfilesize(const char* filename) {
    struct stat st;
     stat(filename, &st);
    return st.st_size;
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
        case HW_HELLOTHERE: {
                /**
                 * Checking permissions is always a good idea.
                 *
                 * Note that the native client will also be granted these permissions in two cases
                 * 1) you run the client code as root or system user.
                 * 2) you run the client code as user who was granted this permission.
                 * @see http://github.com/keesj/gomo/wiki/AndroidSecurity for more information
                 **/
                if (checkCallingPermission(String16("org.credil.helloworldservice.permissions.CALL_HELLOTHERE")) == false){
                    return   PERMISSION_DENIED;
                }
                printf("helloworld HW_HELLOTHERE");
                String16 str = data.readString16();
                hellothere(String8(str).string());

                reply->writeInt32(count++);
                printf("count: %d\n", count);

                //reply->writeString16(String16("Hello, It's from Service"));
                return NO_ERROR;
        } break;
        case HW_FILE_SIZE: {
                /**
                 * Checking permissions is always a good idea.
                 *
                 * Note that the native client will also be granted these permissions in two cases
                 * 1) you run the client code as root or system user.
                 * 2) you run the client code as user who was granted this permission.
                 * @see http://github.com/keesj/gomo/wiki/AndroidSecurity for more information
                 **/

                LOGE("File Size(%u,%u)\n", code, flags);
                String16 str = data.readString16();
                LOGE("File Path %s\n", String8(str).string());
                printf("File Path %s\n", String8(str).string());
                //reply->writeInt32(getfilesize("/data/data/com.wssyncmldm/databases/wssdmdatabase.db"));
                int filesize = getfilesize(String8(str).string());
                reply->writeInt32(filesize);
                return NO_ERROR;
        } break;

        default:
                return BBinder::onTransact(code, data, reply, flags);
        }
        return NO_ERROR;
}

}

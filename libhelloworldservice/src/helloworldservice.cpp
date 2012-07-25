#define LOG_TAG "HelloWorldService"

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

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

sp<IHelloWorld> HelloWorldService::getfilesize(const char* filename) {
    struct stat st;
    Parcel data, reply;

    stat(filename, &st);

    data.writeInterfaceToken(getInterfaceDescriptor());
    data.writeInt32(st.st_size);

    //remote()->transact(HW_FILE_SIZE, data, &reply);
    return interface_cast<IHelloWorld>(reply.readStrongBinder());
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
                String16 str = data.readString16();
                hellothere(String8(str).string());

                reply->writeInt32(count++);

                reply->writeString16(String16("Hello, It's from Service"));
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
                //reply->writeInt32(getfilesize("/data/data/com.wssyncmldm/databases/wssdmdatabase.db"));
                sp<IHelloWorld> filesize = getfilesize(String8(str).string());
                reply->writeStrongBinder(filesize->asBinder());

                //reply->writeInt32(getfilesize(String8(str).string()));
                return NO_ERROR;
        } break;

        default:
                return BBinder::onTransact(code, data, reply, flags);
        }
        return NO_ERROR;
}

}

#define LOG_TAG "ZPadClient"

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
#include <binder/Parcel.h>

#include "IHelloWorld.h"
#include "utils/Log.h"

namespace android {

/**
 * This is the implementation of the client side of the IHelloWorld interface
 * Typical usage of this class is to first get a binder object from the service manager
 * then use the interface_cast<IHelloWorld>(binder) method to convert the binder into
 * a IHelloWorld smart pointer and invoke methods on this
 *
 * This code is not needed if your interface is only exposed to the Java world.
 * but similar code can be created to access "Java only" AIDL interfaces.
 *
 * Because we extend from BpInterface we have the remote() method
 * to get access back to the binder when needed
 *
 *  class BpInterface<INTERFACE> extends BpRefBase
 *   + onAsBinder();
 *
 * BpRefBase defines a ref counted class having a method to get
 * a binder object
 *
 * class BpRefBase extends RefBase
 *   + remote() IBinder
 *
 **/
class BpHelloWorld: public BpInterface<IHelloWorld>
{
public:
        BpHelloWorld(const sp<IBinder>& impl)
                : BpInterface<IHelloWorld>(impl) {}

        virtual int copy_file(const char* from, const char* to) {
                Parcel data, reply;
                LOGI("copy_file:  from: %s, to: %s\n", from, to);

                data.writeInterfaceToken(getInterfaceDescriptor());
                data.writeString16(String16(from));
                data.writeString16(String16(to));
                remote()->transact(HW_COPY_FILE, data, &reply);
                int status = reply.readInt32();
                LOGI("status=%d\n", status);
                return status;
        }
};

/**
 * Implementation of the cast from a IBinder back to an interface.
 */
IMPLEMENT_META_INTERFACE(HelloWorld, HELLOWORLD_NAME);

};

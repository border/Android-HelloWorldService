/*
 * HelloWorld Service driver.
 * Copyright (C) 2009 Michael Richardson <mcr@credil.org>
 *
 * Released under the terms of the file ../NOTICE
 */
#define LOG_TAG "HelloWorld/Service"

#include <sys/types.h>
#include <unistd.h>
#include <grp.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>

#include "IHelloWorld.h"

int main(int argc, char *argv[])
{
	LOGI("Hello client is now starting");

        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::IBinder> binder;
        android::sp<android::IHelloWorld> shw;

        do {
                binder = sm->getService(android::String16(HELLOWORLD_NAME));
                if (binder != 0)
                        break;
                LOGW("HelloWorld not published, waiting...");
                usleep(500000); // 0.5 s
        } while(true);

        //LOGI("sm: %u binder: %u", sm, binder);

        //if (sDeathNotifier == NULL) {
        //    sDeathNotifier = new DeathNotifier();
        //}
        //binder->linkToDeath(sDeathNotifier);
        //sMediaPlayerService = interface_cast<IMediaPlayerService>(binder);

	LOGI("Hello client is now trying");

        shw = android::interface_cast<android::IHelloWorld>(binder);
        shw->hellothere("fun");
        int filesize = 0;
        char *filepath = NULL;
        if (argc > 0) {
            filepath = argv[1];
        } else {
            filepath = "/data/data/com.wssyncmldm/databases/wssdmdatabase.db";
        }

        //filesize = shw->getfilesize(filepath);
        LOGI("Hello Filesize: %d, filePath: %s\n", filesize, filepath);
	
	LOGI("Hello client is now exiting");

	return(0);
}

// SPDX-License-Identifier: Zlib
// Copyright (C) 2025 Edoardo Lolletti (edo9300)
// Adapted from BlocksDS libnds source/arm7/libc/crt0_device_list.c.

#include <nds.h>
#include <string.h>
#include "device_list.h"

typedef struct {
    struct __argv base;
    char cmdline[sizeof(((DeviceList*)0)->appname)+3];
} DeviceListArgv;

_Static_assert(0x02FFFE70+sizeof(DeviceListArgv)<0x03000000,"Device List arguments exceed main RAM");

// ARM7 constructors run before the CRT releases ARM9 to process argv.
__attribute__((constructor)) static void initDeviceListArguments(void) {
    if (!isDSiMode() || __system_argv->argvMagic==ARGV_MAGIC) return;
    u32 address=__DSiHeader->offset_0x1D4;
    if (address<0x02000000) return;
    const DeviceList* deviceList=(const DeviceList*)address;
    size_t deviceNameLen=0;
    while (deviceNameLen<sizeof(deviceList->appname) && deviceList->appname[deviceNameLen]!=':') deviceNameLen++;
    if (!deviceNameLen || deviceNameLen>=sizeof(deviceList->devices[0].deviceName)) return;
    for (size_t index=0;index<sizeof(deviceList->devices)/sizeof(deviceList->devices[0]);index++) {
        const DeviceListEntry* device=&deviceList->devices[index];
        if (device->deviceName[deviceNameLen]=='\0' && memcmp(device->deviceName,deviceList->appname,deviceNameLen)==0) {
            char cmdline[sizeof(((DeviceListArgv*)0)->cmdline)]={0};
            if (device->deviceId==0) {
                cmdline[0]='s';cmdline[1]='d';
                memcpy(cmdline+2,deviceList->appname+deviceNameLen,sizeof(deviceList->appname)-deviceNameLen);
            } else memcpy(cmdline,deviceList->appname,sizeof(deviceList->appname));
            DeviceListArgv* arguments=(DeviceListArgv*)__system_argv;
            memcpy(arguments->cmdline,cmdline,sizeof(cmdline));
            arguments->base.argvMagic=ARGV_MAGIC;
            arguments->base.commandLine=arguments->cmdline;
            arguments->base.length=strlen(arguments->cmdline)+1;
            break;
        }
    }
}

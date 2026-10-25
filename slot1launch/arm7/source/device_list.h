// SPDX-License-Identifier: Zlib
// Copyright (C) 2025 Edoardo Lolletti (edo9300)
// Adapted from BlocksDS libnds include/nds/device_list.h for devkitARM.

#pragma once
#include <nds/memory.h>

typedef struct PACKED DeviceListEntry {
    char driveLetter;
    struct PACKED {
        u8 deviceId : 1;
        u8 reserved1 : 2;
        u8 deviceType : 2;
        u8 partition : 1;
        u8 reserved2 : 1;
        bool encrypted : 1;
    };
    u8 permissions;
    u8 reserved3;
    char deviceName[0x10];
    char path[0x40];
} DeviceListEntry;

typedef struct PACKED DeviceList {
    DeviceListEntry devices[11];
    u8 reserved[0x24];
    char appname[0x40];
} DeviceList;

_Static_assert(sizeof(DeviceList)==0x400,"Invalid Device List layout");

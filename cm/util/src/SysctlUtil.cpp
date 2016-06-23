/*
 * SysctlUtil.cpp
 *
 *  Created on: June 12, 2016
 *      Author: z.j
 */

#include "SysctlUtil.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/sysctl.h>
#include <iostream>

using namespace cm;
using namespace std;

// ------------------------------------------
unsigned int SysctlUtil::getMaxWmem() {
    return getNetCoreValue(NET_CORE_WMEM_MAX);
}

// ------------------------------------------
unsigned int SysctlUtil::getMaxRmem() {
    return getNetCoreValue(NET_CORE_RMEM_MAX);
}

// ------------------------------------------
unsigned int SysctlUtil::getNetCoreValue(int valueName) {
    struct __sysctl_args args;
    memset(&args, 0, sizeof(struct __sysctl_args));

    int name[] = { CTL_NET, NET_CORE, valueName };  
    unsigned int value;
    size_t length = sizeof(int);

    args.name = name;
    args.nlen = sizeof(name)/sizeof(name[0]);
    args.oldval = &value;
    args.oldlenp = &length;

    if (syscall(SYS__sysctl, &args) == -1) {
        perror("_sysctl");
        return 0;
    }

    return value;
}

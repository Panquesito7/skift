#pragma once

#include <abi-sysv/abi.h>
#include <karm-base/res.h>

#include "spec.h"

Res<> entryPoint(u64 magic, Handover::Payload &payload);

#ifndef HANOVER_NO_KSTART

extern "C" void _kstart(u64 magic, Handover::Payload *payload) {
    Abi::SysV::init();
    Res<> res = entryPoint(magic, *payload);

    if (!res) {
        panic(res.none().msg());
    }

    panic("handover: entryPoint returned ok");
}

#endif

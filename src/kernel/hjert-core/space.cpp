#include <karm-base/checked.h>

#include "arch.h"
#include "space.h"

namespace Hjert::Core {

/* --- VNone ---------------------------------------------------------------- */

VNode::VNode(_Mem mem) : _mem(std::move(mem)) {}

Res<Strong<VNode>> VNode::alloc(usize size, Hj::MemFlags) {
    auto mem = try$(pmm().allocOwned(size, Hal::PmmFlags::UPPER));
    return Ok(makeStrong<VNode>(std::move(mem)));
}

Res<Strong<VNode>> VNode::makeDma(Hal::DmaRange prange) {
    try$(prange.ensureAligned(Hal::PAGE_SIZE));

    return Ok(makeStrong<VNode>(prange));
}

Hal::PmmRange VNode::range() {
    return _mem.visit(
        Visitor{
            [](Hal::PmmMem const &mem) {
                return mem.range();
            },
            [](Hal::DmaRange const &range) {
                return range.as<Hal::PmmRange>();
            },
        });
}

/* --- Space ---------------------------------------------------------------- */

Res<Strong<Space>> Space::create() {
    return Arch::createSpace();
}

Res<usize> Space::_lookup(Hal::VmmRange vrange) {
    for (usize i = 0; i < _maps.len(); i++) {
        auto &map = _maps[i];
        if (Op::eq(map.vrange, vrange)) {
            return Ok(i);
        }
    }

    return Error::invalidInput("no such mapping");
}

Res<Hal::VmmRange> Space::map(Hal::VmmRange vrange, Strong<VNode> mem, usize off, Hj::MapFlags flags) {
    LockScope scope{_lock};

    try$(vrange.ensureAligned(Hal::PAGE_SIZE));

    if (vrange.size == 0) {
        vrange.size = mem->range().size;
    }

    auto end = try$(checkedAdd(off, vrange.size));

    if (end > mem->range().size) {
        return Error::invalidInput("mapping too large");
    }

    if (vrange.start == 0) {
        vrange = try$(_alloc.alloc(vrange.size));
    } else {
        _alloc.used(vrange);
    }

    auto map = Map{vrange, off, std::move(mem)};

    try$(vmm().allocRange(map.vrange, {map.mem->range().start + map.off, vrange.size}, flags | Hal::VmmFlags::USER));
    _maps.pushBack(std::move(map));

    return Ok(vrange);
}

Res<> Space::unmap(Hal::VmmRange vrange) {
    LockScope scope{_lock};

    try$(vrange.ensureAligned(Hal::PAGE_SIZE));

    auto id = try$(_lookup(vrange));
    auto &map = _maps[id];
    try$(vmm().free(map.vrange));
    _alloc.unused(map.vrange);
    _maps.removeAt(id);
    return Ok();
}

void Space::activate() {
    vmm().activate();
}

} // namespace Hjert::Core

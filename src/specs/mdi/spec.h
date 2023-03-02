#pragma once

#include <karm-base/res.h>
#include <karm-base/string.h>

// NOTE: Theses functions need to be implemented in C because clang choke otherwise.
extern "C" u32 _Mdi__byName(char const *query, usize queryLen);

extern "C" char const *_Mdi__name(u32 query);

namespace Mdi {

enum struct Icon {
#define ICON(id, name, code) id = code,
#include "icons.inc"
#undef ICON
};

using enum Icon;

inline Res<Icon> byName(Str query) {
    auto codepoint = _Mdi__byName(query.buf(), query.len());
    if (codepoint == 0) {
        return Error::notFound("icon not found");
    }
    return Ok((Icon)codepoint);
}

inline Str name(Icon icon) {
    return Str(_Mdi__name((u32)icon));
}

} // namespace Mdi

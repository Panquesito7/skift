#pragma once

#include "node.h"

namespace Karm::Ui {

/* --- Base ----------------------------------------------------------------- */

template <typename Crtp>
struct View : public LeafNode<Crtp> {
    Math::Recti _bound;

    Math::Recti bound() override {
        return _bound;
    }

    void layout(Math::Recti bound) override {
        _bound = bound;
    }
};

/* --- Text ----------------------------------------------------------------- */

struct TextStyle {
    Media::Font font;
    Opt<Gfx::Color> color = NONE;

    TextStyle withSize(isize size) const {
        TextStyle style = *this;
        style.font.fontsize = size;
        return style;
    }

    TextStyle withColor(Gfx::Color color) const {
        TextStyle style = *this;
        style.color = color;
        return style;
    }

    static TextStyle displayLarge();
    static TextStyle displayMedium();
    static TextStyle displaySmall();

    static TextStyle headlineLarge();
    static TextStyle headlineMedium();
    static TextStyle headlineSmall();

    static TextStyle titleLarge();
    static TextStyle titleMedium();
    static TextStyle titleSmall();

    static TextStyle labelLarge();
    static TextStyle labelMedium();
    static TextStyle labelSmall();

    static TextStyle bodyLarge();
    static TextStyle bodyMedium();
    static TextStyle bodySmall();
};

Child text(TextStyle style, Str text);

Child text(Str text);

template <typename... Args>
inline Child text(TextStyle style, Str format, Args &&...args) {
    return text(style, Fmt::format(format, std::forward<Args>(args)...).unwrap());
}

template <typename... Args>
inline Child text(Str format, Args &&...args) {
    return text(Fmt::format(format, std::forward<Args>(args)...).unwrap());
}

#define DEF_STYLE(STYLE)                                                              \
    inline Child STYLE(Str text) { return Karm::Ui::text(TextStyle::STYLE(), text); } \
    template <typename... Args>                                                       \
    inline Child STYLE(Str format, Args &&...args) {                                  \
        return text(TextStyle::STYLE(), format, std::forward<Args>(args)...);         \
    }

DEF_STYLE(displayLarge)
DEF_STYLE(displayMedium)
DEF_STYLE(displaySmall)
DEF_STYLE(headlineLarge)
DEF_STYLE(headlineMedium)
DEF_STYLE(headlineSmall)
DEF_STYLE(titleLarge)
DEF_STYLE(titleMedium)
DEF_STYLE(titleSmall)
DEF_STYLE(labelLarge)
DEF_STYLE(labelMedium)
DEF_STYLE(labelSmall)
DEF_STYLE(bodyLarge)
DEF_STYLE(bodyMedium)
DEF_STYLE(bodySmall)

#undef DEF_STYLE

/* --- Icon ----------------------------------------------------------------- */

Child icon(Media::Icon icon, Opt<Gfx::Color> color = NONE);

Child icon(Mdi::Icon icon, f64 size, Opt<Gfx::Color> color = NONE);

/* --- Image ---------------------------------------------------------------- */

Child image(Media::Image image);

/* --- Canvas --------------------------------------------------------------- */

using OnPaint = Func<void(Gfx::Context &g, Math::Vec2i size)>;

Child canvas(OnPaint onPaint);

/* --- Blur ----------------------------------------------------------------- */

Child blur(isize radius, Child child);

} // namespace Karm::Ui

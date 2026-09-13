# Optimization progress for potato computers (no features/visuals loss)

## Completed

### Render skip when idle (src/xsui-picom.c:draw_callback_impl)
- Skip `renderer_render` when no animation is running, no window has new damage, and not in benchmark mode.
- Avoids expensive command building, culling, backend execution, and present when the screen is unchanged.
- Correctness preserved: any change (damage event, animation start, wallpaper change, resize) triggers `queue_redraw` → render.

### Overlay window blur/shadow exclusion (src/renderer/command_builder.c)
- Blur and shadow commands are skipped when `layer->opacity <= 0.05f` (both in command generation and allocation).
- Covers desktop background, window body, shadow, and blur commands.

### Window type exclusion (src/wm/win.c)
- `WINTYPE_POPUP_MENU`, `WINTYPE_DROPDOWN_MENU`, `WINTYPE_TOOLTIP`, `WINTYPE_NOTIFICATION` are excluded from blur and shadow.

## Remaining
- Investigate Chromium/menu overlay blur persistence (likely ARGB visual-based transparency without `_NET_WM_WINDOW_OPACITY`).
- Explore further per-frame optimizations (animation processing, image refresh).

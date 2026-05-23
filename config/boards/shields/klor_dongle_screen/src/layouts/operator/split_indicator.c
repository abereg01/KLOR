#include "split_indicator.h"

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/split_central_status_changed.h>

#include "display_colors.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct split_indicator_state {
    bool connected[2]; // index 0 = slot 0, index 1 = slot 1
};

// Cached per-slot state; updated in get_state() from each incoming event.
static bool slot_connected[2] = {false, false};

static void split_indicator_update_cb(struct split_indicator_state state) {
    struct zmk_widget_split_indicator *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        for (int i = 0; i < 2; i++) {
            lv_color_t color = state.connected[i]
                ? lv_color_hex(DISPLAY_COLOR_MOD_ACTIVE)   // Nord8 frost blue — connected
                : lv_color_hex(DISPLAY_COLOR_MOD_INACTIVE); // Nord3 dim grey  — disconnected
            lv_obj_set_style_bg_color(widget->bars[i], color, LV_PART_MAIN);
        }
    }
}

static struct split_indicator_state split_indicator_get_state(const zmk_event_t *eh) {
    if (eh != NULL) {
        const struct zmk_split_central_status_changed *ev =
            as_zmk_split_central_status_changed(eh);
        if (ev != NULL && ev->slot < 2) {
            slot_connected[ev->slot] = ev->connected;
        }
    }
    return (struct split_indicator_state){
        .connected = {slot_connected[0], slot_connected[1]},
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_split_indicator, struct split_indicator_state,
                            split_indicator_update_cb, split_indicator_get_state)
ZMK_SUBSCRIPTION(widget_split_indicator, zmk_split_central_status_changed);

int zmk_widget_split_indicator_init(struct zmk_widget_split_indicator *widget, lv_obj_t *parent) {
    // Two bars spanning the screen width with a gap in the middle, mirroring
    // the keyboard's split: left bar = slot 0 (left half), right = slot 1.
    // Total: 113px + 14px gap + 113px = 240px.
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 240, 4);
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(widget->obj, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(widget->obj, 0, LV_PART_MAIN);
    lv_obj_clear_flag(widget->obj, LV_OBJ_FLAG_SCROLLABLE);

    // Left bar (slot 0 — left half)
    widget->bars[0] = lv_obj_create(widget->obj);
    lv_obj_set_size(widget->bars[0], 113, 4);
    lv_obj_set_pos(widget->bars[0], 0, 0);
    lv_obj_set_style_bg_color(widget->bars[0], lv_color_hex(DISPLAY_COLOR_MOD_INACTIVE), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(widget->bars[0], LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(widget->bars[0], 0, LV_PART_MAIN);
    lv_obj_set_style_radius(widget->bars[0], 2, LV_PART_MAIN);
    lv_obj_set_style_pad_all(widget->bars[0], 0, LV_PART_MAIN);

    // Right bar (slot 1 — right half)
    widget->bars[1] = lv_obj_create(widget->obj);
    lv_obj_set_size(widget->bars[1], 113, 4);
    lv_obj_set_pos(widget->bars[1], 127, 0);
    lv_obj_set_style_bg_color(widget->bars[1], lv_color_hex(DISPLAY_COLOR_MOD_INACTIVE), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(widget->bars[1], LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(widget->bars[1], 0, LV_PART_MAIN);
    lv_obj_set_style_radius(widget->bars[1], 2, LV_PART_MAIN);
    lv_obj_set_style_pad_all(widget->bars[1], 0, LV_PART_MAIN);

    sys_slist_append(&widgets, &widget->node);
    widget_split_indicator_init();

    return 0;
}

lv_obj_t *zmk_widget_split_indicator_obj(struct zmk_widget_split_indicator *widget) {
    return widget->obj;
}

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_split_indicator {
    sys_snode_t node;
    lv_obj_t *obj;
    lv_obj_t *bars[2]; // bars[0] = left half (slot 0), bars[1] = right half (slot 1)
};

int zmk_widget_split_indicator_init(struct zmk_widget_split_indicator *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_split_indicator_obj(struct zmk_widget_split_indicator *widget);

/*
 *
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 */

#include <zephyr/kernel.h>
#include "util.h"

LV_IMAGE_DECLARE(bolt);

void rotate_canvas(lv_layer_t *layer, lv_color_t cbuf[]) {
    static lv_color_t cbuf_tmp[CANVAS_SIZE * CANVAS_SIZE];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));

    lv_image_dsc_t img = {
        .header.magic = LV_IMAGE_HEADER_MAGIC,
        .header.cf = LV_COLOR_FORMAT_NATIVE,
        .header.flags = 0,
        .header.w = CANVAS_SIZE,
        .header.h = CANVAS_SIZE,
        .header.stride = CANVAS_SIZE * sizeof(lv_color_t),
        .data_size = sizeof(cbuf_tmp),
        .data = (const uint8_t *)cbuf_tmp,
        .reserved = NULL,
    };

    lv_draw_image_dsc_t dsc;
    lv_draw_image_dsc_init(&dsc);
    dsc.src = &img;
#ifdef CONFIG_SHARP_MIP_ROTATE_180
    dsc.rotation = -900;
    dsc.pivot.x = CANVAS_SIZE / 2;
    dsc.pivot.y = CANVAS_SIZE / 2 - 1;
#else
    dsc.rotation = 900;
    dsc.pivot.x = CANVAS_SIZE / 2;
    dsc.pivot.y = CANVAS_SIZE / 2;
#endif
    dsc.scale_x = LV_SCALE_NONE;
    dsc.scale_y = LV_SCALE_NONE;
    dsc.opa = LV_OPA_COVER;
    dsc.antialias = 1;

    lv_area_t coords;
    lv_area_set(&coords, 0, 0, CANVAS_SIZE - 1, CANVAS_SIZE - 1);
    lv_draw_image(layer, &dsc, &coords);
}

void draw_battery(lv_layer_t *layer, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    lv_area_t coords;
    lv_area_set(&coords, 0, 2, 28, 13);
    lv_draw_rect(layer, &rect_white_dsc, &coords);
    lv_area_set(&coords, 1, 3, 27, 12);
    lv_draw_rect(layer, &rect_black_dsc, &coords);
    if (((state->battery + 2) / 4) > 0) {
        lv_area_set(&coords, 2, 4, 1 + (state->battery + 2) / 4, 11);
        lv_draw_rect(layer, &rect_white_dsc, &coords);
    }
    lv_area_set(&coords, 30, 5, 32, 10);
    lv_draw_rect(layer, &rect_white_dsc, &coords);
    lv_area_set(&coords, 31, 6, 31, 9);
    lv_draw_rect(layer, &rect_black_dsc, &coords);

    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        img_dsc.src = &bolt;
        img_dsc.opa = LV_OPA_COVER;
        lv_area_set(&coords, 9, -1, 19, 16);
        lv_draw_image(layer, &img_dsc, &coords);
    }
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font,
                    lv_text_align_t align) {
    lv_draw_label_dsc_init(label_dsc);
    label_dsc->color = color;
    label_dsc->font = font;
    label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
    lv_draw_rect_dsc_init(rect_dsc);
    rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) {
    lv_draw_line_dsc_init(line_dsc);
    line_dsc->color = color;
    line_dsc->width = width;
}

void init_arc_dsc(lv_draw_arc_dsc_t *arc_dsc, lv_color_t color, uint8_t width) {
    lv_draw_arc_dsc_init(arc_dsc);
    arc_dsc->color = color;
    arc_dsc->width = width;
}

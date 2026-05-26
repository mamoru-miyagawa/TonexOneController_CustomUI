#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_screen1() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.screen1 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // ui_TouchGestureContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.ui_touch_gesture_container = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_event_cb(obj, action_gesture, LV_EVENT_GESTURE, (void *)0);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ui_btn_prev
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.ui_btn_prev = obj;
                    lv_obj_set_pos(obj, 0, 261);
                    lv_obj_set_size(obj, 205, 59);
                    lv_obj_add_event_cb(obj, action_previous_clicked, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x6fd9c0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_anton_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "PREV");
                        }
                    }
                }
                {
                    // ui_btn_next
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.ui_btn_next = obj;
                    lv_obj_set_pos(obj, 275, 261);
                    lv_obj_set_size(obj, 205, 59);
                    lv_obj_add_event_cb(obj, action_next_clicked, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x6fd9c0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_anton_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "NEXT");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 120, 30);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 0 },
                        { 350, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x6bd1b9), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // Bank_container
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bank_container = obj;
                    lv_obj_set_pos(obj, 11, 155);
                    lv_obj_set_size(obj, 100, 20);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_pad_top(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x00c9a5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_BankTitleLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_bank_title_label = obj;
                            lv_obj_set_pos(obj, -12, 0);
                            lv_obj_set_size(obj, 62, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_letter_space(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "BANK");
                        }
                        {
                            // ui_BankValueLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_bank_value_label = obj;
                            lv_obj_set_pos(obj, 26, 0);
                            lv_obj_set_size(obj, 36, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_letter_space(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "1");
                        }
                    }
                }
                {
                    // Preset_container
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.preset_container = obj;
                    lv_obj_set_pos(obj, 11, 44);
                    lv_obj_set_size(obj, 100, 108);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_pad_top(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x00c9a5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_height(obj, 108, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_PresetValueLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_preset_value_label = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, 87, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_anton_80, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "00");
                        }
                    }
                }
                {
                    // ui_TopPanel
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ui_top_panel = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 480, 320);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_height(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_SettingsImage
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_settings_image = obj;
                            lv_obj_set_pos(obj, 429, -17);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_settings);
                            lv_obj_add_event_cb(obj, action_show_settings_page, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_ProjectHeadingLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_project_heading_label = obj;
                            lv_obj_set_pos(obj, -8, -11);
                            lv_obj_set_size(obj, 111, 16);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x6ed8bf), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_height(obj, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "TONEX ONE");
                        }
                        {
                            // ui_WiFiStatusDisconn
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_wi_fi_status_disconn = obj;
                            lv_obj_set_pos(obj, 290, -5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_wifi_disconn);
                        }
                        {
                            // ui_BTStatusDisconn
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_bt_status_disconn = obj;
                            lv_obj_set_pos(obj, 345, -5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_bt_disconn);
                        }
                        {
                            // ui_USBStatusFail
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_usb_status_fail = obj;
                            lv_obj_set_pos(obj, 386, -5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_usb_fail);
                        }
                        {
                            // ui_WiFiStatusConn
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_wi_fi_status_conn = obj;
                            lv_obj_set_pos(obj, 290, -5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_wifi_conn);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                        {
                            // ui_BTStatusConn
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_bt_status_conn = obj;
                            lv_obj_set_pos(obj, 345, -5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_bt_conn);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                        {
                            // ui_USBStatusOK
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_usb_status_ok = obj;
                            lv_obj_set_pos(obj, 386, -5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_usb_ok);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                        }
                        {
                            // ui_ProjectHeadingLabel_1
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_project_heading_label_1 = obj;
                            lv_obj_set_pos(obj, 302, -8);
                            lv_obj_set_size(obj, 38, 16);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x6bd1b9), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "WIFI");
                        }
                        {
                            // ui_ProjectHeadingLabel_2
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_project_heading_label_2 = obj;
                            lv_obj_set_pos(obj, 356, -8);
                            lv_obj_set_size(obj, 38, 16);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x6bd1b9), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "BT");
                        }
                        {
                            // ui_ProjectHeadingLabel_3
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_project_heading_label_3 = obj;
                            lv_obj_set_pos(obj, 398, -8);
                            lv_obj_set_size(obj, 38, 16);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x6bd1b9), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "USB");
                        }
                        {
                            // ui_ProjectHeadingLabel_4
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_project_heading_label_4 = obj;
                            lv_obj_set_pos(obj, 99, -11);
                            lv_obj_set_size(obj, 152, 16);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xede8da), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            {
                                static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                                lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                            }
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_height(obj, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "コントローラ");
                        }
                    }
                }
                {
                    // ui_ProjectHeadingLabel_5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.ui_project_heading_label_5 = obj;
                    lv_obj_set_pos(obj, 11, 25);
                    lv_obj_set_size(obj, 101, 16);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x6ed8bf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                        lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                    {
                        static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                        lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_height(obj, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_letter_space(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "グレッグスミス");
                }
                {
                    // ui_PresetHeadingLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.ui_preset_heading_label = obj;
                    lv_obj_set_pos(obj, 118, 41);
                    lv_obj_set_size(obj, 350, 137);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_SCROLLED);
                    lv_obj_set_style_text_font(obj, &ui_font_anton_40, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xede8da), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_width(obj, 350, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Keep on rocking!");
                }
                {
                    // BPM_Container
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bpm_container = obj;
                    lv_obj_set_pos(obj, 205, 259);
                    lv_obj_set_size(obj, 70, 61);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x6bd1b9), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_width(obj, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_TOP|LV_BORDER_SIDE_LEFT|LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_BPMIndicator
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.ui_bpm_indicator = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, 66, 59);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x003226), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_BPMTitleLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_bpm_title_label = obj;
                            lv_obj_set_pos(obj, 0, 2);
                            lv_obj_set_size(obj, 42, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x68778c), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_anton_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "BPM");
                        }
                        {
                            // ui_BPMValueLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_bpm_value_label = obj;
                            lv_obj_set_pos(obj, 10, 21);
                            lv_obj_set_size(obj, 46, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_anton_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x6fd9c0), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "000");
                        }
                    }
                }
                {
                    // ui_BottomPanelTonex
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ui_bottom_panel_tonex = obj;
                    lv_obj_set_pos(obj, 0, 188);
                    lv_obj_set_size(obj, 480, 57);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_IconReverb
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_reverb = obj;
                            lv_obj_set_pos(obj, 393, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_effect_icon_reverb_off);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_IconDelay
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_delay = obj;
                            lv_obj_set_pos(obj, 336, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_effect_icon_delay_off);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_IconMod
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_mod = obj;
                            lv_obj_set_pos(obj, 279, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_effect_icon_mod_off);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_IconComp
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_comp = obj;
                            lv_obj_set_pos(obj, 222, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_effect_icon_comp_off);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_IconCab
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_cab = obj;
                            lv_obj_set_pos(obj, 165, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_effect_icon_cab_off);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_IconAmp
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_amp = obj;
                            lv_obj_set_pos(obj, 108, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_effect_icon_amp_off);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_IconGate
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_gate = obj;
                            lv_obj_set_pos(obj, 51, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_effect_icon_gate_off);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_IconEQ
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_eq = obj;
                            lv_obj_set_pos(obj, -6, -18);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_effect_icon_eq);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                    }
                }
                {
                    // ui_BottomPanelValeton
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ui_bottom_panel_valeton = obj;
                    lv_obj_set_pos(obj, 0, 259);
                    lv_obj_set_size(obj, 480, 61);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_icon_val_pre
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_pre = obj;
                            lv_obj_set_pos(obj, -19, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_pre_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_amp
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_amp = obj;
                            lv_obj_set_pos(obj, 24, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_amp_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_nr
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_nr = obj;
                            lv_obj_set_pos(obj, 67, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_nr_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_rvb
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_rvb = obj;
                            lv_obj_set_pos(obj, 110, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_rvb_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_cab
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_cab = obj;
                            lv_obj_set_pos(obj, 153, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_cab_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_tc
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_tc = obj;
                            lv_obj_set_pos(obj, 196, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_tc_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_eq
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_eq = obj;
                            lv_obj_set_pos(obj, 239, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_eq_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_mod
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_mod = obj;
                            lv_obj_set_pos(obj, 282, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_mod_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_dly
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_dly = obj;
                            lv_obj_set_pos(obj, 325, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_dly_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_icon_val_dst
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_icon_val_dst = obj;
                            lv_obj_set_pos(obj, 368, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_dst_off);
                            lv_img_set_zoom(obj, 200);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                        {
                            // ui_val_settings
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.ui_val_settings = obj;
                            lv_obj_set_pos(obj, 417, -10);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_settings);
                            lv_img_set_zoom(obj, 220);
                            lv_obj_add_event_cb(obj, action_show_settings_page, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj1 = obj;
                    lv_obj_set_pos(obj, 10, 260);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 0 },
                        { 460, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x6bd1b9), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // ui_ProjectHeadingLabel_6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.ui_project_heading_label_6 = obj;
                    lv_obj_set_pos(obj, 383, 172);
                    lv_obj_set_size(obj, 85, 16);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x6ed8bf), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                        lv_obj_set_style_grid_row_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                    {
                        static lv_coord_t dsc[] = {0, LV_GRID_TEMPLATE_LAST};
                        lv_obj_set_style_grid_column_dsc_array(obj, dsc, LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_height(obj, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "シグナルチェーン");
                }
            }
        }
    }
    
    tick_screen_screen1();
}

void tick_screen_screen1() {
}

void create_screen_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // ui_SettingsTabView
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 42);
            objects.ui_settings_tab_view = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfafafa), LV_PART_MAIN | LV_STATE_CHECKED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_tabview_get_tab_btns(parent_obj);
                    objects.obj2 = obj;
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_height(obj, 42, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x363f4f), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_LEFT|LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x363f4f), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6ed8bf), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_ITEMS | LV_STATE_CHECKED);
                }
                {
                    // ui_GateTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Gate");
                    objects.ui_gate_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_setting_panel_style(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_NoiseGateEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_NoiseGateSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_noise_gate_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_NoiseGatePostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_NoiseGatePostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_noise_gate_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_NoiseGateThresholdLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_threshold_label = obj;
                            lv_obj_set_pos(obj, -160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Threshold");
                        }
                        {
                            // ui_NoiseGateThresholdSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_noise_gate_threshold_slider = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, -100, -20);
                            lv_arc_set_value(obj, -64);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_NoiseGateThresholdValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_threshold_value = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_NoiseGateReleaseLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_release_label = obj;
                            lv_obj_set_pos(obj, 0, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Release");
                        }
                        {
                            // ui_NoiseGateReleaseSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_noise_gate_release_slider = obj;
                            lv_obj_set_pos(obj, 0, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 5, 500);
                            lv_arc_set_value(obj, 20);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_NoiseGateReleaseValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_release_value = obj;
                            lv_obj_set_pos(obj, 0, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_NoiseGateDepthLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_depth_label = obj;
                            lv_obj_set_pos(obj, 160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Depth");
                        }
                        {
                            // ui_NoiseGateDepthSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_noise_gate_depth_slider = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, -100, -20);
                            lv_arc_set_value(obj, -60);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_NoiseGateDepthValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_depth_value = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_CompresssorTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Comp");
                    objects.ui_compresssor_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_setting_panel_style(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_CompressorEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_CompressorEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_compressor_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_CompressorPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_CompressorPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_compressor_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_CompressorThresholdLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_threshold_label = obj;
                            lv_obj_set_pos(obj, -160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Threshold");
                        }
                        {
                            // ui_CompressorThresholdSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_compressor_threshold_slider = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, -40, 0);
                            lv_arc_set_value(obj, -20);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_CompressorThresholdValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_threshold_value = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_CompressorAttackLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_attack_label = obj;
                            lv_obj_set_pos(obj, 0, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Attack");
                        }
                        {
                            // ui_CompressorAttackSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_compressor_attack_slider = obj;
                            lv_obj_set_pos(obj, 0, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 1, 51);
                            lv_arc_set_value(obj, 14);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_CompressorAttackValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_attack_value = obj;
                            lv_obj_set_pos(obj, 0, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_CompressorGainLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_gain_label = obj;
                            lv_obj_set_pos(obj, 160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Gain");
                        }
                        {
                            // ui_CompressorGainSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_compressor_gain_slider = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, -30, 10);
                            lv_arc_set_value(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_CompressorGainValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_gain_value = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_AmpTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Amp");
                    objects.ui_amp_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_setting_panel_style(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_AmpEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amp_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_AmpEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_amp_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_AmpCabLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amp_cab_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Cab");
                        }
                        {
                            // ui_CabinetModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_cabinet_model_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 51);
                            lv_obj_set_size(obj, 165, 28);
                            lv_dropdown_set_options_static(obj, "Tone Model\nVIR\nDisabled");
                            lv_dropdown_set_dir(obj, LV_DIR_RIGHT);
                            lv_dropdown_set_symbol(obj, LV_SYMBOL_RIGHT);
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            add_style_modern_dropdown(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    add_style_modern_list(obj);
                                }
                            }
                        }
                        {
                            // ui_AmplifierGainLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_gain_label = obj;
                            lv_obj_set_pos(obj, -160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Gain");
                        }
                        {
                            // ui_AmplifierGainSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_amplifier_gain_slider = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_AmplifierGainValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_gain_value = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_AmplifierVolumeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_volume_label = obj;
                            lv_obj_set_pos(obj, -52, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Volume");
                        }
                        {
                            // ui_AmplifierVolumeSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_amplifier_volume_slider = obj;
                            lv_obj_set_pos(obj, -53, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_AmplifierVolumeValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_volume_value = obj;
                            lv_obj_set_pos(obj, -52, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_AmplifierPresenseLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_presense_label = obj;
                            lv_obj_set_pos(obj, 53, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Presence");
                        }
                        {
                            // ui_AmplifierPresenseSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_amplifier_presense_slider = obj;
                            lv_obj_set_pos(obj, 53, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_AmplifierPresenseValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_presense_value = obj;
                            lv_obj_set_pos(obj, 53, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_AmplifierDepthLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_depth_label = obj;
                            lv_obj_set_pos(obj, 160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Depth");
                        }
                        {
                            // ui_AmplifierDepthSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_amplifier_depth_slider = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_AmplifierDepthValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_depth_value = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_EQTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "EQ");
                    objects.ui_eq_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_setting_panel_style(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_EQPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_post_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_EQPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_eq_post_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_EQBassLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_bass_label = obj;
                            lv_obj_set_pos(obj, -160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Bass");
                        }
                        {
                            // ui_EQBassSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_eq_bass_slider = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_EQBassValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_bass_value = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_EQMidLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_mid_label = obj;
                            lv_obj_set_pos(obj, -53, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Mid");
                        }
                        {
                            // ui_EQMidSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_eq_mid_slider = obj;
                            lv_obj_set_pos(obj, -53, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_EQMidValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_mid_value = obj;
                            lv_obj_set_pos(obj, -53, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_EQMidQLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_mid_qlabel = obj;
                            lv_obj_set_pos(obj, 53, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "MidQ");
                        }
                        {
                            // ui_EQMidQSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_eq_mid_qslider = obj;
                            lv_obj_set_pos(obj, 53, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_EQMidQValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_mid_qvalue = obj;
                            lv_obj_set_pos(obj, 53, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_EQTrebleLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_treble_label = obj;
                            lv_obj_set_pos(obj, 160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Treble");
                        }
                        {
                            // ui_EQTrebleSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_eq_treble_slider = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_EQTrebleValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_treble_value = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ModulationTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Mod");
                    objects.ui_modulation_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_setting_panel_style(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ModulationEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ModulationEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_modulation_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_ModulationPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_ModulationPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_modulation_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_ModulationModeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_mode_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Model");
                        }
                        {
                            // ui_ModulationModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_modulation_model_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 51);
                            lv_obj_set_size(obj, 165, 28);
                            lv_dropdown_set_options_static(obj, "Chorus\nTremolo\nPhaser\nFlanger\nRotary");
                            lv_dropdown_set_dir(obj, LV_DIR_RIGHT);
                            lv_dropdown_set_symbol(obj, LV_SYMBOL_RIGHT);
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            add_style_modern_dropdown(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    add_style_modern_list(obj);
                                }
                            }
                        }
                        {
                            // ui_ModulationSyncLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_sync_label = obj;
                            lv_obj_set_pos(obj, 309, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Sync");
                        }
                        {
                            // ui_ModulationSyncSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_modulation_sync_switch = obj;
                            lv_obj_set_pos(obj, 393, -71);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_ModulationParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ModulationTSDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_modulation_ts_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 91);
                            lv_obj_set_size(obj, 165, 28);
                            lv_dropdown_set_options_static(obj, "1/32\n1/32 D\n1/32 T\n1/16\n1/16 D\n1/16 T\n1/8\n1/8 D\n1/8 T\n1/4\n1/4 D\n1/4 T\n1/2\n1/2 D\n1/2 T\n1/1\n1/1 D\n1/1 T");
                            lv_dropdown_set_dir(obj, LV_DIR_RIGHT);
                            lv_dropdown_set_symbol(obj, LV_SYMBOL_RIGHT);
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                            add_style_modern_dropdown(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    add_style_modern_list(obj);
                                }
                            }
                        }
                        {
                            // ui_ModulationParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_modulation_param1_slider = obj;
                            lv_obj_set_pos(obj, 106, -34);
                            lv_obj_set_size(obj, 300, 10);
                            lv_slider_set_value(obj, 20, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern_slider1(obj);
                        }
                        {
                            // ui_ModulationParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ModulationParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ModulationParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_modulation_param2_slider = obj;
                            lv_obj_set_pos(obj, 106, 4);
                            lv_obj_set_size(obj, 300, 15);
                            lv_slider_set_range(obj, 0, 1000);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern_slider(obj);
                            lv_obj_set_style_width(obj, 300, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_height(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ModulationParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ModulationParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ModulationParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_modulation_param3_slider = obj;
                            lv_obj_set_pos(obj, 106, 42);
                            lv_obj_set_size(obj, 300, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern_slider1(obj);
                            lv_obj_set_style_height(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ModulationParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ModulationParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ModulationParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_modulation_param4_slider = obj;
                            lv_obj_set_pos(obj, 106, 80);
                            lv_obj_set_size(obj, 300, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern_slider(obj);
                            lv_obj_set_style_height(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ModulationParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_DelayTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Dly");
                    objects.ui_delay_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_setting_panel_style(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_DelayEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_DelayEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_delay_enable_switch = obj;
                            lv_obj_set_pos(obj, 76, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_DelayPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_post_label = obj;
                            lv_obj_set_pos(obj, 174, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_DelayPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_delay_post_switch = obj;
                            lv_obj_set_pos(obj, 235, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_DelayModeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_mode_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Model");
                        }
                        {
                            // ui_DelayModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_delay_model_dropdown = obj;
                            lv_obj_set_pos(obj, 79, 51);
                            lv_obj_set_size(obj, 165, 28);
                            lv_dropdown_set_options_static(obj, "Digital\nTape");
                            lv_dropdown_set_dir(obj, LV_DIR_RIGHT);
                            lv_dropdown_set_symbol(obj, LV_SYMBOL_RIGHT);
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            add_style_modern_dropdown(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    add_style_modern_list(obj);
                                }
                            }
                        }
                        {
                            // ui_DelaySyncLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_sync_label = obj;
                            lv_obj_set_pos(obj, 336, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Sync");
                        }
                        {
                            // ui_DelaySyncSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_delay_sync_switch = obj;
                            lv_obj_set_pos(obj, 393, -72);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_DelayPingPongLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_ping_pong_label = obj;
                            lv_obj_set_pos(obj, 335, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "PPng");
                        }
                        {
                            // ui_DelayPingPongSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_delay_ping_pong_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 26);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_DelayTSDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_delay_ts_dropdown = obj;
                            lv_obj_set_pos(obj, 79, 91);
                            lv_obj_set_size(obj, 95, 28);
                            lv_dropdown_set_options_static(obj, "1/32\n1/32 D\n1/32 T\n1/16\n1/16 D\n1/16 T\n1/8\n1/8 D\n1/8 T\n1/4\n1/4 D\n1/4 T\n1/2\n1/2 D\n1/2 T\n1/1\n1/1 D\n1/1 T");
                            lv_dropdown_set_dir(obj, LV_DIR_RIGHT);
                            lv_dropdown_set_symbol(obj, LV_SYMBOL_RIGHT);
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                            add_style_modern_dropdown(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    add_style_modern_list(obj);
                                }
                            }
                        }
                        {
                            // ui_DelayTSLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_ts_label = obj;
                            lv_obj_set_pos(obj, -160, 62);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Time");
                        }
                        {
                            // ui_DelayTSSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_delay_ts_slider = obj;
                            lv_obj_set_pos(obj, -160, 26);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_value(obj, 25);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_DelayTSValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_ts_value = obj;
                            lv_obj_set_pos(obj, -160, 26);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_DelayFeedbackLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_feedback_label = obj;
                            lv_obj_set_pos(obj, 0, 62);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Feedback");
                        }
                        {
                            // ui_DelayFeedbackSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_delay_feedback_slider = obj;
                            lv_obj_set_pos(obj, 0, 26);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_value(obj, 25);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_DelayFeedbackValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_feedback_value = obj;
                            lv_obj_set_pos(obj, 208, 26);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_DelayMixLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_mix_label = obj;
                            lv_obj_set_pos(obj, 160, 62);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Mix");
                        }
                        {
                            // ui_DelayMixSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_delay_mix_slider = obj;
                            lv_obj_set_pos(obj, 160, 26);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_value(obj, 25);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_DelayMixValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_mix_value = obj;
                            lv_obj_set_pos(obj, 160, 26);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ReverbTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Rev");
                    objects.ui_reverb_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_setting_panel_style(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ReverbEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ReverbEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_reverb_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_ReverbPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_ReverbPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_reverb_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_ReverbModeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_mode_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Model");
                        }
                        {
                            // ui_ReverbModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_reverb_model_dropdown = obj;
                            lv_obj_set_pos(obj, 109, 52);
                            lv_obj_set_size(obj, 165, 28);
                            lv_dropdown_set_options_static(obj, "Spring 1\nSpring 2\nSpring 3\nSpring 4\nRoom\nPlate");
                            lv_dropdown_set_dir(obj, LV_DIR_RIGHT);
                            lv_dropdown_set_symbol(obj, LV_SYMBOL_RIGHT);
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_state(obj, LV_STATE_FOCUSED);
                            add_style_modern_dropdown(obj);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj3 = obj;
                                    add_style_modern_list(obj);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                        {
                            // ui_ReverbMixLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_mix_label = obj;
                            lv_obj_set_pos(obj, -160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Mix");
                        }
                        {
                            // ui_ReverbMixSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_reverb_mix_slider = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ReverbMixValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_mix_value = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ReverbTimeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_time_label = obj;
                            lv_obj_set_pos(obj, -53, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Time");
                        }
                        {
                            // ui_ReverbTimeSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_reverb_time_slider = obj;
                            lv_obj_set_pos(obj, -53, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 10);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ReverbTimeValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_time_value = obj;
                            lv_obj_set_pos(obj, -53, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ReverbPredelayLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_predelay_label = obj;
                            lv_obj_set_pos(obj, 53, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Pre\nDelay");
                        }
                        {
                            // ui_ReverbPredelaySlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_reverb_predelay_slider = obj;
                            lv_obj_set_pos(obj, 53, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 0, 500);
                            lv_arc_set_value(obj, 5);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ReverbPredelayValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_predelay_value = obj;
                            lv_obj_set_pos(obj, 53, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ReverbColorLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_color_label = obj;
                            lv_obj_set_pos(obj, 160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Color");
                        }
                        {
                            // ui_ReverbColorSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_reverb_color_slider = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, -10, 10);
                            lv_arc_set_value(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ReverbColorValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_color_value = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_GlobalTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Glob");
                    objects.ui_global_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_setting_panel_style(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_CabBypassLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_cab_bypass_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Cab Byp");
                        }
                        {
                            // ui_CabBypassSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_cab_bypass_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 62, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_TempoSourcetLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_tempo_sourcet_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Tempo");
                        }
                        {
                            // ui_TempoSourceSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_tempo_source_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 62, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_modern(obj);
                        }
                        {
                            // ui_BPMSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_bpm_slider = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 40, 240);
                            lv_arc_set_value(obj, 120);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_BPMLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_bpm_label = obj;
                            lv_obj_set_pos(obj, -160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_label_set_text_static(obj, "BPM");
                        }
                        {
                            // ui_BPMValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_bpm_value = obj;
                            lv_obj_set_pos(obj, -160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_InputTrimLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_input_trim_label = obj;
                            lv_obj_set_pos(obj, -53, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            add_style_settings_label(obj);
                            lv_label_set_text_static(obj, "Input\nTrim");
                        }
                        {
                            // ui_InputTrimSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_input_trim_slider = obj;
                            lv_obj_set_pos(obj, -53, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, -15, 15);
                            lv_arc_set_value(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_InputTrimValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_input_trim_value = obj;
                            lv_obj_set_pos(obj, -53, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_TuningReferenceLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_tuning_reference_label = obj;
                            lv_obj_set_pos(obj, 53, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Tuning\nRef");
                        }
                        {
                            // ui_TuningReferenceSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_tuning_reference_slider = obj;
                            lv_obj_set_pos(obj, 53, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, 415, 465);
                            lv_arc_set_value(obj, 440);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_TuningReferenceValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_tuning_reference_value = obj;
                            lv_obj_set_pos(obj, 53, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_VolumeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_volume_label = obj;
                            lv_obj_set_pos(obj, 160, 52);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Vol");
                        }
                        {
                            // ui_VolumeSlider
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.ui_volume_slider = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 80, 80);
                            lv_arc_set_range(obj, -40, 3);
                            lv_arc_set_value(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            add_style_arc_slider(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_VolumeValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_volume_value = obj;
                            lv_obj_set_pos(obj, 160, 16);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &ui_font_m_plus_u_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xeee9db), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
            }
        }
        {
            // ui_Closeimage
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.ui_closeimage = obj;
            lv_obj_set_pos(obj, 0, 1);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_tick);
            lv_obj_add_event_cb(obj, action_close_settings_page, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // ui_settings_dialog
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.ui_settings_dialog = obj;
            lv_obj_set_pos(obj, 11, 13);
            lv_obj_set_size(obj, 462, 298);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x81562e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ui_settings_text_entry
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.ui_settings_text_entry = obj;
                    lv_obj_set_pos(obj, 53, -14);
                    lv_obj_set_size(obj, 326, LV_SIZE_CONTENT);
                    lv_textarea_set_max_length(obj, 8);
                    lv_textarea_set_text(obj, "0");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xd1a60c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // ui_settings_keyboard
                    lv_obj_t *obj = lv_keyboard_create(parent_obj);
                    objects.ui_settings_keyboard = obj;
                    lv_obj_set_pos(obj, -18, 55);
                    lv_obj_set_size(obj, 462, 225);
                    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
                    lv_obj_add_event_cb(obj, action_value_keyboard_ok, LV_EVENT_READY, (void *)0);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    lv_keyboard_set_textarea(objects.ui_settings_keyboard, objects.ui_settings_text_entry);
    
    tick_screen_settings();
}

void tick_screen_settings() {
}

void create_screen_val_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.val_settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // ui_ValSettingsTabView
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 52);
            objects.ui_val_settings_tab_view = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x3b3c40), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_CHECKED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_tabview_get_tab_btns(parent_obj);
                    objects.obj4 = obj;
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_LEFT, LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x96969e), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_ITEMS | LV_STATE_CHECKED);
                }
                {
                    // ui_ValNRTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "NR");
                    objects.ui_val_nr_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValNRBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValNRBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_nr_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -109);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValNRModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_nr_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Noise Gate");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj5 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValNRParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValNRParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNRParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValNRParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -36);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNRParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValNRParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNRParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValNRParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNRParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValNRParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValPreTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Pre");
                    objects.ui_val_pre_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValNPreBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_npre_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValPreBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_pre_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValPreModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_pre_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Comp\nComp4\nBoost\nMicro Boost\nB-Boost\nToucher\nCrier\nOcta\nPitch\nDetune");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj6 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValPreParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValPreParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPreParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValPreParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPreParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValPreParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPreParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValPreParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPreParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValPreParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValDstTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Dst");
                    objects.ui_val_dst_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValDstBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValDstBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_dst_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValDstModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_dst_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Green OD\nYellow OD\nSuper OD\nSM Dist\nPlustortion\nLa Charger\nDarktale\nSora Fuzz\nRed Haze\nBass OD");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj7 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValDstParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValDstParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDstParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValDstParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDstParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValDstParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDstParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValDstParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDstParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValDstParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValAmpTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Amp");
                    objects.ui_val_amp_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValAmpBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValAmpBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_amp_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValAmpModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_amp_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Tweedy\nBellman 59N\nDark Twin\nFoxy 30N\nJ-120 CL\nMatch CL\nL-Star CL\nUK 45\nUK 50JP\nUK 800\nBellman 59B\nFoxy 30TB\nSup Dual OD\nSolo100 OD\nZ38 OD\nBad KT OD\nJuiceW R100\nDizz VH\nDizz VH+\nEagle 120\nEV51\nSolo100 LD\nMess Dual V\nMess Dual M\nPower LD\nFlagman+\nBog Red V\nClassic Bass\nFoxy Bass\nMess Bass\nAC Pre 1\nAC Pre 2\n");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj8 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValAmpParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValAmpParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValAmpParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValAmpParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValAmpParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValAmpParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValAmpParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValAmpParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValAmpParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValAmpParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValCabTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Cab");
                    objects.ui_val_cab_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValCabBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValCabBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_cab_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValCabModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_cab_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Twd CP\nDark VIT\nFoxy 1x12\nL-Star 1x12\nDark CS 2x12\nDark Twin 2x12\nSup Star 2x12\nJ-120 2x12\nFoxy 2x12\nUK Grn 2x12\nUK Grn 4x12\nBog 4x12\nDizz 4x12\nEV 4x12\nSolo 4x12\nMess 4x12\nEagle 4x12\nJuice 4x12\nBellman 2x12\nAmpg 4x10");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj9 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValCabParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValCabParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValCabParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValCabParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValCabParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValCabParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValCabParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValCabParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValCabParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValCabParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValEQTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "EQ");
                    objects.ui_val_eq_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValEQBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValEQBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_eq_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValEQModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_eq_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Guitar 1\nGuitar 2\nBass 1\nBass 2\nMess");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj10 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValEQParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValEQParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValEQParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValEQParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValEQParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValEQParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValEQParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValEQParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValEQParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValEQParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValModTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Mod");
                    objects.ui_val_mod_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValModBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValModBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_mod_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValModModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_mod_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "A-Chorus\nB-Chorus\nJet\nN-Jet\nO-Phase\nM-Vibe\nV-Roto\nVibrato\nO-Trem\nSine Trem\nBias Trem");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj11 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValModParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValModParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValModParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValModParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValModParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValModParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValModParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValModParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValModParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValModParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValDlyTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Dly");
                    objects.ui_val_dly_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValDlyBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValDlyBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_dly_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValDlyModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_dly_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Pure\nAnalog\nSlapback\nSweet Echo\nTape\nTube\nRev Echo\nRing Echo\nSweep Echo\nPing Pong");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj12 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValDlyParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValDlyParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDlyParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValDlyParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDlyParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValDlyParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDlyParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValDlyParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDlyParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValDlyParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValRvbTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Rvb");
                    objects.ui_val_rvb_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValRvbBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValRvbBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_rvb_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValRvbModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_rvb_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Air\nRoom\nHall\nChurch\nPlate L\nPlate\nSpring\nN-Star\nDeepsea\nSweet Space");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj13 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValRvbParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValRvbParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValRvbParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValRvbParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValRvbParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValRvbParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValRvbParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValRvbParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValRvbParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValRvbParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValNSTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "NS");
                    objects.ui_val_ns_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValNSBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValNSBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_ns_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValNSModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_ns_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n71\n72\n73\n74\n75\n76\n77\n78\n79");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj14 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValNSParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValNSParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNSParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValNSParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNSParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValNSParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNSParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValNSParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNSParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValNSParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValGlobTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Glb");
                    objects.ui_val_glob_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValGlobNoCabLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_no_cab_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "No Cab");
                        }
                        {
                            // ui_ValGlobNoCabSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_glob_no_cab_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValGlobInputLevelLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_input_level_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Input Lev");
                        }
                        {
                            // ui_ValGlobInputLevelSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_glob_input_level_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -20, 20);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValGlobInputLevelValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_input_level_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValGlobMasterVolLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_master_vol_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Master Vol");
                        }
                        {
                            // ui_ValGlobMasterVolSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_glob_master_vol_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValGlobMasterVolValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_master_vol_value = obj;
                            lv_obj_set_pos(obj, 417, -31);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPatchVolLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_patch_vol_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Patch Vol");
                        }
                        {
                            // ui_ValPatchVolSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_patch_vol_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPatchVolValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_patch_vol_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
            }
        }
        {
            // ui_ValCloseimage
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.ui_val_closeimage = obj;
            lv_obj_set_pos(obj, 438, 293);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_tick);
            lv_obj_add_event_cb(obj, action_close_settings_page, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            // ui_val_settings_dialog
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.ui_val_settings_dialog = obj;
            lv_obj_set_pos(obj, 11, 13);
            lv_obj_set_size(obj, 462, 298);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x386a91), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ui_val_settings_text_entry
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.ui_val_settings_text_entry = obj;
                    lv_obj_set_pos(obj, 53, -14);
                    lv_obj_set_size(obj, 326, LV_SIZE_CONTENT);
                    lv_textarea_set_max_length(obj, 8);
                    lv_textarea_set_text(obj, "0");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xd1a60c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // ui_val_settings_keyboard
                    lv_obj_t *obj = lv_keyboard_create(parent_obj);
                    objects.ui_val_settings_keyboard = obj;
                    lv_obj_set_pos(obj, -18, 55);
                    lv_obj_set_size(obj, 462, 225);
                    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
                    lv_obj_add_event_cb(obj, action_value_keyboard_ok, LV_EVENT_READY, (void *)0);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    lv_keyboard_set_textarea(objects.ui_val_settings_keyboard, objects.ui_val_settings_text_entry);
    
    tick_screen_val_settings();
}

void tick_screen_val_settings() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_screen1,
    tick_screen_settings,
    tick_screen_val_settings,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 3) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
    { "Anton_14", &ui_font_anton_14 },
    { "Anton_28", &ui_font_anton_28 },
    { "Anton_36", &ui_font_anton_36 },
    { "Anton_58", &ui_font_anton_58 },
    { "M_PLUS_U_Bold_16", &ui_font_m_plus_u_bold_16 },
    { "M_PLUS_U_Bold_10", &ui_font_m_plus_u_bold_10 },
    { "Anton_80", &ui_font_anton_80 },
    { "M_PLUS_U_Bold_14", &ui_font_m_plus_u_bold_14 },
    { "Anton_40", &ui_font_anton_40 },
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_screen1();
    create_screen_settings();
    create_screen_val_settings();
}
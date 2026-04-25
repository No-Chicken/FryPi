#include "../ui.h"

lv_obj_t * ui_HomePage = NULL;
lv_obj_t * ui_Slider1 = NULL;
lv_obj_t * ui_Button1 = NULL;
// event funtions

// build funtions

void ui_HomePage_screen_init(void)
{
    ui_HomePage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_HomePage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Slider1 = lv_slider_create(ui_HomePage);
    lv_slider_set_value(ui_Slider1, 0, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider1) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider1, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider1, 150);
    lv_obj_set_height(ui_Slider1, 10);
    lv_obj_set_x(ui_Slider1, 12);
    lv_obj_set_y(ui_Slider1, 28);
    lv_obj_set_align(ui_Slider1, LV_ALIGN_CENTER);

    ui_Button1 = lv_btn_create(ui_HomePage);
    lv_obj_set_width(ui_Button1, 100);
    lv_obj_set_height(ui_Button1, 50);
    lv_obj_set_x(ui_Button1, 5);
    lv_obj_set_y(ui_Button1, -41);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

}

void ui_HomePage_screen_destroy(void)
{
    if(ui_HomePage) lv_obj_del(ui_HomePage);

    // NULL screen variables
    ui_HomePage = NULL;
    ui_Slider1 = NULL;
    ui_Button1 = NULL;

}

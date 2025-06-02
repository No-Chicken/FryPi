#ifndef _UI_CHARGPAGE_H
#define _UI_CHARGPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t * ui_ChargPage;
extern lv_obj_t * ui_CharPageBatArc;
extern lv_obj_t * ui_ChargPageBatIcon;
extern lv_obj_t * ui_ChargPageBatNum;
extern lv_obj_t * ui_ChargPagebHourLabel;
extern lv_obj_t * ui_ChargPagebMinLabel;
extern lv_obj_t * ui_ChargPageTimePoint;

extern Page_t Page_Charg;

void ui_ChargPage_screen_init(void);
void ui_ChargPage_screen_deinit(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

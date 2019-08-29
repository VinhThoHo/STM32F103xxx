#ifndef MENU_CALLBACKS_H_
#define MENU_CALLBACKS_H_

extern uint32_t brightness;

void disp_brightness_callback(void);
void backlight_on_callback(void);
void backlight_off_callback(void);

void get_current_datetime_callback(void);
void set_time_callback(void);
void set_date_callback(void);

void get_current_temp_config(void);
void set_temperature_h_callback(void);
void set_temperature_l_callback(void);


void get_current_bottle_callback(void);
void set_bottle_sum_callback(void);
void set_bottle_reset_callback(void);

void restore_default_callback(void);

#endif /* MENU_CALLBACKS_H_ */
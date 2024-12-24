
#include "string.h"

#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

typedef enum {
	E_IMG_ALARM_ICON,
	E_IMG_BASAL,
	E_IMG_BATTERY_EMPTY,
	E_IMG_BATTERY_FOUR,
	E_IMG_BATTERY_ONE,
	E_IMG_BATTERY_THREE,
	E_IMG_BATTERY_TWO,
	E_IMG_BIG_DOT,
	E_IMG_BLACK_ICON,
	E_IMG_BLUETOOTH,
	E_IMG_BOLUS,
	E_IMG_HIGH_ALARM,
	E_IMG_INFUSION_SETTING,
	E_IMG_INFUSION_SWITCH,
	E_IMG_LEFT_ARROW,
	E_IMG_LOCKED,
	E_IMG_LOG,
	E_IMG_LOGO,
	E_IMG_LOW_ALARM,
	E_IMG_MID_ALARM,
	E_IMG_MUTE,
	E_IMG_MUTE_DOT,
	E_IMG_PAUSE,
	E_IMG_REFILL,
	E_IMG_REPLACE_PULL,
	E_IMG_REPLACE_PUSH,
	E_IMG_RIGHT_ARROW,
	E_IMG_RUNNING,
	E_IMG_SELECT_BAR,
	E_IMG_SMALL_DOT,
	E_IMG_SYSTEM_SETTING,
	E_IMG_TEMP_BASAL,
	E_IMG_UP_ARROW,
	E_IMG_USER_SETTING,

	E_IMG_MAX,
} img_id_t;

typedef struct {
	const unsigned char *data;
	unsigned char width;	//in pixels
	unsigned char height;	//in pixels
} image_data_t;

//get image from the image id
const image_data_t* get_image(img_id_t img_id);

#endif

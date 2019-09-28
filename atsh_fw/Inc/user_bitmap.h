#ifndef __USER_BITMAP_H__
#define __USER_BITMAP_H__

#include <stdint.h>
#include "main.h"

typedef struct {
	uint8_t width, height;
	const uint8_t *data;
} BITMAP;

extern const BITMAP bmp_logo_ph;
extern const BITMAP bmp_neon_on;
extern const BITMAP bmp_neon_off;
extern const BITMAP bmp_uv_on;
extern const BITMAP bmp_uv_off;
extern const BITMAP bmp_fan_origin;
extern const BITMAP bmp_fan_rotate;
extern const BITMAP bmp_socket_on;
extern const BITMAP bmp_socket_off;

#endif
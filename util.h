#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

void setColor(uint8_t r, uint8_t g, uint8_t b);
void resetDevice();


#ifdef __cplusplus
}
#endif

#endif

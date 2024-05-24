#ifndef __MYRTC_H
#define __MYRTC_H

extern uint16_t MyRTC_DateTime[];

void MyRTC_Init(void);
void MyRTC_GetDateTime(void);
void MyRTC_SetDateTime(void);

#endif

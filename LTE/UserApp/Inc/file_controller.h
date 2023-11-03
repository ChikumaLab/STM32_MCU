/*
 * file_controller.h
 *
 *  Created on: 2022/09/19
 *      Author: User
 */

#ifndef INC_FILE_CONTROLLER_H_
#define INC_FILE_CONTROLLER_H_

#include "main.h"

extern void mount_usb(void);
extern void unmount_usb(void);
extern uint32_t get_RTC_time (void);
extern uint8_t fs_file_open(char *filename);
extern uint8_t fs_file_write(char *data,uint32_t len);
extern uint8_t fs_file_close();




#endif /* INC_FILE_CONTROLLER_H_ */

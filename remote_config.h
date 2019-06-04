#ifndef  _REMOTE_CONFIG_H
#define  _REMOTE_CONFIG_H

#include <asm/ioctl.h>
#include <stdint.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef struct {
	unsigned short *key_map;
	unsigned int key_count;
	unsigned int factory_infcode;
	unsigned int factory_unfcode;
	unsigned int factory_code;
	unsigned int repeat_delay;
	unsigned int repeat_peroid;
	unsigned int work_mode ;
	unsigned int mouse_speed;
	unsigned int repeat_enable;
	unsigned int release_delay;
	unsigned int release_fdelay;
	unsigned int release_sdelay;
	unsigned int debug_enable;
//sw
	unsigned int bit_count;
	unsigned int tw_leader_act;
	unsigned int tw_bit0;
	unsigned int tw_bit1;
	unsigned int tw_bit2;
	unsigned int tw_bit3;
	unsigned int tw_repeat_leader;
//reg
	unsigned int reg_base_gen;
	unsigned int reg_control;
	unsigned int reg_leader_act;
	unsigned int reg_leader_idle;
	unsigned int reg_repeat_leader;
	unsigned int reg_bit0_time;

	unsigned int fn_key_scancode;
	unsigned int left_key_scancode;
	unsigned int right_key_scancode;
	unsigned int up_key_scancode;
	unsigned int down_key_scancode;
	unsigned int ok_key_scancode;
	unsigned int pageup_key_scancode;
	unsigned int pagedown_key_scancode;
} remote_config_t;

//these string must in this order and sync with struct remote_config_t
extern char*  config_item[33];

extern int malloc_new_remote(remote_config_t **remote);
extern int get_config_from_file(FILE *fp, remote_config_t *remotes[]);

#endif

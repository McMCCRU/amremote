#ifndef  _CONFIG_H
#define  _CONFIG_H

#include <stdio.h>
#include <fcntl.h>
#include "remote_config.h"

//these string must in this order and sync with struct remote_config_t
char*  config_item[33] = {
	"factory_infcode",
	"factory_unfcode",
	"factory_code",
	"repeat_delay",
	"repeat_peroid",
	"work_mode",
	"mouse_speed",
	"repeat_enable",
	"release_delay",
	"release_fdelay",
	"release_sdelay",
	"debug_enable",
	//sw
	"bit_count",
	"tw_leader_act",
	"tw_bit0",
	"tw_bit1",
	"tw_bit2",
	"tw_bit3",
	"tw_repeat_leader",
	//reg
	"reg_base_gen",
	"reg_control",
	"reg_leader_act",
	"reg_leader_idle",
	"reg_repeat_leader",
	"reg_bit0_time",

	"fn_key_scancode",
	"left_key_scancode",
	"right_key_scancode",
	"up_key_scancode",
	"down_key_scancode",
	"ok_key_scancode",
	"pageup_key_scancode",
	"pagedown_key_scancode",
};
#endif

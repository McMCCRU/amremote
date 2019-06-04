#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/ioctl.h>
#include "remote_config.h"
#include "rc_common.h"
#include "keydefine.h"
#define CUSTOM_REMOTE_MAX 20
#define MAX_KEY_MAPS 256
#define DEVICE_NAME "/dev/amremote"

unsigned short default_key_map[MAX_KEY_MAPS] = {
KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, /*0~7*/
KEY_8, KEY_9, KEY_BACK, KEY_UP, KEY_BACKSPACE, KEY_ENTER, KEY_DOWN, KEY_MENU, /*8~f*/
KEY_LEFT, KEY_RIGHT, KEY_R, KEY_S, KEY_U, KEY_G, KEY_K, KEY_L, /*10~17*/
KEY_M, KEY_N, KEY_D, KEY_T, KEY_H, KEY_B, KEY_I, KEY_J, /*18~1f*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*20~27*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*28~2f*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*30~37*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*38~3f*/

KEY_C, KEY_F, KEY_E, KEY_P, KEY_V, KEY_A, KEY_Q, KEY_O, /*40~47*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*48~4f*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_W, KEY_Z, KEY_RESERVED, KEY_RESERVED, KEY_Y, /*50~57*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_X, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*58~5f*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*60~67*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*68~6f*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*70~77*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*78~7f*/

KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*80~87*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*88~8f*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*90~97*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*98~9f*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*a0~a7*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*a8~af*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*b0~b7*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*b8~bf*/

KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*c0~c7*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*c8~cf*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*d0~d7*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*d8~df*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*e0~e7*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*e8~ef*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, /*f0~f7*/
KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED /*f8~ff*/
};

int malloc_new_remote(remote_config_t **remote)
{
	*remote = (remote_config_t *)malloc(sizeof(remote_config_t));
	if (!*remote) {
        printf("Memory allocation for 'remote' failed!\n");
        return -1;
    }

    remote_config_t *new_remote = *remote;

    memset(new_remote, 0, sizeof(remote_config_t));

    new_remote->key_map = (unsigned short *)malloc(sizeof(unsigned short) * MAX_KEY_MAPS);
    if (!new_remote->key_map) {
        printf("Memory allocation for 'key_map' failed!\n");
        return -1;
    }

    memset(new_remote->key_map, KEY_RESERVED, sizeof(unsigned short) * MAX_KEY_MAPS);

    return 0;
}

void free_remote(remote_config_t **remote)
{
    remote_config_t *free_remote = *remote;

    if (free_remote)
    {
        if (free_remote->key_map)
        {
            free(free_remote->key_map);
            free_remote->key_map = NULL;
        }

        free(free_remote);
        free_remote = NULL;
    }
}

int main(int argc, char* argv[])
{
    remote_config_t *remotes[CUSTOM_REMOTE_MAX] = { NULL };
    FILE *fp;
    int i, j;
    int device_fd;	

    device_fd = open(DEVICE_NAME, O_RDWR);

    if(device_fd < 0){
        printf("Can't open %s .\n", DEVICE_NAME);
        return -2;
        }
    if(argc < 2){
            remote_config_t *remote = NULL;
            if (!malloc_new_remote(&remote))
            {
                remote->factory_code = 0x40400001;
                remote->work_mode = 1;
                remote->repeat_enable = 1;
                remote->release_delay = 150;
                remote->debug_enable = 1;
                remote->reg_control = 0xfbe40;
                remote->repeat_delay = 250;
                remote->repeat_peroid = 33;
                memcpy(remote->key_map, default_key_map, sizeof(unsigned short) * MAX_KEY_MAPS);
                remotes[0] = remote;
            }
        }
    else if(argv[1][0]=='-'){
        printf("Usage : %s configfile\n", argv[0]);
        return -3;
        }
    else{
        fp=fopen(argv[1], "r");
        if(!fp){
            printf("Open file %s is failed!!!\n", argv[1]);
            return -4;
            }
        get_config_from_file(fp, remotes);
        fclose(fp);
        }

    for (i = 0; i < CUSTOM_REMOTE_MAX; i++)
    {
        remote_config_t *custom_remote = remotes[i];
        if (!custom_remote)
            continue;

        custom_remote->factory_code >>= 16;
		{
			struct ir_map_tab *meson_remote = (struct ir_map_tab *)malloc(sizeof(struct ir_map_tab) + sizeof(union _codemap) * custom_remote->key_count);
			int code_pos = 0;

			if (!meson_remote) {
				printf("Memory allocation for 'meson_remote' failed!\n");
				return -1;
			}

			meson_remote->custom_code = custom_remote->factory_code;
			meson_remote->map_size = custom_remote->key_count;
			meson_remote->release_delay = custom_remote->repeat_delay ? custom_remote->repeat_delay : 150;
			meson_remote->cursor_code.fn_key_scancode = custom_remote->fn_key_scancode;
			meson_remote->cursor_code.cursor_left_scancode = custom_remote->left_key_scancode;
			meson_remote->cursor_code.cursor_right_scancode = custom_remote->right_key_scancode;
			meson_remote->cursor_code.cursor_up_scancode = custom_remote->up_key_scancode;
			meson_remote->cursor_code.cursor_down_scancode = custom_remote->down_key_scancode;
			meson_remote->cursor_code.cursor_ok_scancode = custom_remote->ok_key_scancode;

			for (j = 0; j < MAX_KEY_MAPS; j++)
			{
				if (custom_remote->key_map[j] != KEY_RESERVED)
				{
					union _codemap *code = &meson_remote->codemap[code_pos];
					code->map.keycode = custom_remote->key_map[j];
					code->map.scancode = j;
					code_pos++;
				}
			}

			ioctl(device_fd, REMOTE_IOC_SET_KEY_NUMBER, &custom_remote->key_count);
			ioctl(device_fd, REMOTE_IOC_SET_KEY_MAPPING_TAB, meson_remote);

			free(meson_remote);
		}
    }

    close(device_fd);

    for (i = 0; i < CUSTOM_REMOTE_MAX; i++)
        free_remote(&remotes[i]);

    return 0;
}

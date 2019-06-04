#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "remote_config.h"
#include "config.h"

#define CC_MAX_LINE_LEN     (400)

static void str_trim(char **s) {
    int i;
    int len = strlen(*s);

    for (i = len - 1; i >= 0; i--) {
        if ((*s)[i] <= ' ') {
            (*s)[i] = 0;
        } else {
            break;
        }
    }

    while (**s) {
        if (**s <= ' ') {
            (*s)++;
        } else {
            return;
        }
    }
}

static void trim_line_data(char *line_data_buf) {
    char *tmp_ptr = NULL;

    if (line_data_buf == NULL) {
        return;
    }

    //trim comment string
    tmp_ptr = line_data_buf;
    while (*tmp_ptr && ((tmp_ptr - line_data_buf) <= CC_MAX_LINE_LEN)) {
        if (*tmp_ptr == '#' || *tmp_ptr == ';') {
            *tmp_ptr = '\0';
            break;
        }

        tmp_ptr++;
    }

    //trim other character
    tmp_ptr = line_data_buf;
    str_trim(&tmp_ptr);
    memmove(line_data_buf, tmp_ptr, strlen(tmp_ptr) + 1);
}

static int remote_config_set(char *name, char *value, remote_config_t *config) {
    unsigned int i;
    unsigned int *config_para = &config->factory_infcode;

    for (i = 0; i < ARRAY_SIZE(config_item); i++) {
        if (strcmp(config_item[i], name) == 0) {
            config_para[i] = strtoul(value, NULL, 0);
            printf("curpara:%s  %08x\n", name, config_para[i]);
            return 0;
        }
    }

    return -1;
}

enum {
    CUSTOM_LEVEL,
    CONFIG_LEVEL,
    KEYMAP_LEVEL
};

int get_config_from_file(FILE *fp, remote_config_t *remotes[]) {
    char line_data_buf[CC_MAX_LINE_LEN];
    char *name = NULL;
    char *value;
    remote_config_t *global_config_remote_data = NULL, *remote = NULL;
    unsigned short ircode = 0, keycode = 0;
    unsigned char parse_flag = CONFIG_LEVEL;
    int remote_count = 0;

    while (fgets(line_data_buf, CC_MAX_LINE_LEN, fp)) {
        trim_line_data(line_data_buf);
        if (strcasecmp((char *)line_data_buf, "custom_begin") == 0) {
            parse_flag = CUSTOM_LEVEL;
            break;
        }
    }

    if (parse_flag == CONFIG_LEVEL)
    {
        malloc_new_remote(&remote);
        remotes[remote_count] = remote;
    }
    else
        malloc_new_remote(&global_config_remote_data);

    fseek(fp, 0, SEEK_SET);
    while (fgets(line_data_buf, CC_MAX_LINE_LEN, fp)) {
        trim_line_data(line_data_buf);

        name = line_data_buf;

        switch (parse_flag) {
        case CUSTOM_LEVEL:
            if (strcasecmp(name, "custom_begin") == 0) {
                malloc_new_remote(&remote);
                size_t paramem = sizeof(remote_config_t) - (size_t)((size_t *)global_config_remote_data - (size_t *)&global_config_remote_data->factory_infcode);
                memcpy(&remote->factory_infcode, &global_config_remote_data->factory_infcode, paramem);
                remotes[remote_count] = remote;
                parse_flag = CONFIG_LEVEL;
                continue;
            }
            value = strchr(line_data_buf, '=');
            if (value) {
                *value++ = 0;
                str_trim(&value);
            }
			else
				continue;

            str_trim(&name);
            if (!*name) {
                continue;
            }
            if (remote_config_set(name, value, global_config_remote_data)) {
                printf("config file has not supported parameter:%s=%s\r\n", name, value);
            }
            continue;
        case CONFIG_LEVEL:
            if (strcasecmp(name, "key_begin") == 0) {
                parse_flag = KEYMAP_LEVEL;
                continue;
            }
            if (strcasecmp(name, "custom_end") == 0) {
                remote_count++;
                parse_flag = CUSTOM_LEVEL;
                continue;
            }
            value = strchr(line_data_buf, '=');
			if (value) {
				*value++ = 0;
				str_trim(&value);
			}
			else
				continue;

            str_trim(&name);
            if (!*name) {
                continue;
            }

            if (remote_config_set(name, value, remote)) {
                printf("config file has not supported parameter:%s=%s\r\n", name, value);
            }
            continue;
        case KEYMAP_LEVEL:
            if (strcasecmp(name, "key_end") == 0) {
                parse_flag = CONFIG_LEVEL;
                continue;
            }

            value = strchr(line_data_buf, ' ');
            if (value) {
                *value++ = 0;
                str_trim(&value);
            }

            str_trim(&name);
            if (!*name) {
                continue;
            }
            ircode = strtoul(name, NULL, 0);
            if (ircode > 0xff) {
                continue;
            }

            keycode = strtoul(value, NULL, 0) & 0xffff;
            if (keycode) {
                remote->key_map[ircode] = keycode;
                printf("KEYMAP_LEVEL: ircode = 0x%x, keycode = %d\n", ircode, keycode);
				remote->key_count++;
            }
            continue;
		}
    }
    return 0;
}

#pragma once

#define SECTOR 2048

void cd_open();
void cd_close();
short cd_is_open();
void cd_read_file(unsigned char* file_path, unsigned long** file);
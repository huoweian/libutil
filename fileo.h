/**
 * Copyright(c) 2011, Free software organization.
 * All Rights Reserved.
 * 
 * @author kezf<kezf@foxmail.com>
 * @file fileo.h
 * @brief 文件操作接口。
 */
#ifndef __FILEO_H__
#define __FILEO_H__

#include "types.h"

int copy_file(const char *from_file, const char *to_file);
int copy_dir(const char *from_path, const char *to_path);

#endif /*__FILEO_H__*/


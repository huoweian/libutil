/**
 * Copyright(c) 2011, Free software organization.
 * All Rights Reserved.
 * 
 * @author kezf<kezf@foxmail.com>
 * @file types.h
 * @brief 基本数据类型。
 */
#ifndef __TYPES_H__
#define __TYPES_H__

#include <errno.h>

/* 基本类型 */
#ifndef USHORT
typedef unsigned short USHORT;
#endif
#ifndef ushort
typedef USHORT ushort;
#endif

#ifndef ULONG
typedef unsigned long ULONG;
#endif
#ifndef ulong
typedef ULONG ulong;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif
#ifndef uint
typedef UINT uint;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif
#ifndef uchar
typedef UCHAR uchar;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef byte
typedef BYTE byte;
#endif

#ifndef U8
typedef unsigned char U8;
#endif
#ifndef u8
typedef U8 u8;
#endif

#ifndef U16
typedef unsigned short U16;
#endif
#ifndef u16
typedef U16 u16;
#endif

#ifndef U32
typedef unsigned long U32;
#endif
#ifndef u32
typedef U32 u32;
#endif

#ifndef S8
typedef char S8;
#endif
#ifndef s8
typedef S8 s8;
#endif

#ifndef S16
typedef short S16;
#endif
#ifndef s16
typedef S16 s16;
#endif

#ifndef S32
typedef long S32;
#endif
#ifndef s32
typedef S32 s32;
#endif

#ifndef BOOL
typedef int BOOL;
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#ifndef bool
#define bool BOOL
#endif
#ifndef false
#define false (0)
#endif
#ifndef true
#define true (!false)
#endif

#ifndef NULL
#define NULL (0)
#endif
#ifndef null
#define null (0)
#endif

#ifndef __le32_to_cpu
#define __le32_to_cpu(x) ((u32)(x))
#endif
#ifndef __cpu_to_le32
#define __cpu_to_le32(x) ((u32)(x))
#endif
#ifndef msleep
#define msleep(x) sleep((x) / 1000)
#endif

/* WORD操作宏 */
#define MAKEWORD(x, y) (int)((((int)(x)) & 0xffff) | ((((int)(y)) & 0xffff) << 16))
#define LOWORD(w) (int)(((int)(w)) & 0xffff)
#define HIWORD(w) (int)((((int)(w)) & 0xffff0000) >> 16)

/* 返回大于x的最近的8的倍数的数值 */
#define RND8(x) ((((x) + 7) >> 3) << 3)

/* 返回数组元素个数 */
#define ARR_NUM(a) (sizeof((a)) / sizeof((a)[0]))

/* 返回x和2的m次方相乘 */
#define MUL_M(x, m) ((x) << (m))

/* 返回x和2的m次方相除 */
#define DIV_M(x, m) ((x) >> (m))

/* 返回x和2的m次方求余 */
#define REM_M(x, m) ((x) - (((x) >> (m)) << (m)))

/* 返回x和y求余，y为2的m次方 */
#define REM_Y(x, y) ((x) & ((y) - 1))

/*返回二维数组的元素个数*/
#define ARR2_NUM(a) (sizeof((a))/sizeof((a)[0][0]))

/*返回二维数组的行数*/
#define ARR2_COL_NUM(a) (sizeof((a)[0])/sizeof((a)[0][0]))

/*返回二维数组的行数*/
#define ARR2_ROW_NUM(a) (sizeof((a))/(sizeof((a)[0])))

/*返回二维数组元素相对首地址的偏移量*/
#define ARR2_POS(a, i, j) ((i) * ARR2_COL_NUM((a)) + (j))

/* 返回结构体某个域的大小 */
#define FSIZE(type, field) sizeof(((type *)0)->field)

/* 返回结构体某个域的偏移地址 */
#define FPOS(type, field) ((U32)&((type *)0)->field)

#endif /* __TYPES_H__ */


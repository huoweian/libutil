/**
 * Copyright(c) 2011, Free software organization.
 * All Rights Reserved.
 * 
 * @author kezf<kezf@foxmail.com>
 * @file  byteo.h
 * @brief 字符操作函数集。
 */
#ifndef __BYTEO_H__
#define __BYTEO_H__

#include "types.h"

/** @brief 转换的ASC字符在(0-9 : ; < = > ?)范围内。 */
#define ABC_NUM     0x01
/** @brief 在头部填充。 */
#define ABC_FORE    0x02
/** @brief 填充满缓存。 */
#define ABC_FILL    0x04
/** @brief 设置填充字符。填充字符默认为'0'，指定了该参数可设置填充字符为其他字符。 */
#define ABC_FCH     0x08

/** @brief 多行显示。每行显示16个十六进制数。 */
#define HDX_MUL     0x01
/** @brief 显示行标记。在每行开头显示当前行第一个十六进制数的起始地址。 */
#define HDX_LNUM    0x02
/** @brief 显示列标记。在每列顶部显示当前列的列号。 */
#define HDX_CNUM    0x04
/** @brief 显示ASC字符串。在每行尾部显示当前行的ASC字符串。 */
#define HDX_ASC     0x08
/** @brief 连接多个字符串。即外部多次调用该函数时，不清空输出缓存，在原有的输出缓存后增加字符串。设置了该参数要注意设置连接索引参数。 */
#define HDX_JOIN    0x10

S16 asc2bcdx(const S8 *asc_buf, const U16 asc_len, U8 *bcd_buf, const U16 bcd_len, const U8 flags, .../* const S8 fch */);
/**
 * @brief 转换ASC字符串为BCD缓存。
 * flags为0，转换的ASC字符在(0-9 a-f A-F)范围内，字符串长度为奇数则在尾部填充'0'字符（不填充满缓存）。
 * 
 * @param a ASC字符串。
 * @param b ASC字符串长度。
 * @param c BCD缓存指针。
 * @param d BCD缓存长度。
 * 
 * @return 转换后的BCD缓存长度，转换失败返回负数。
 */
#define asc2bcd(a, b, c, d) asc2bcdx((a), (b), (c), (d), 0)
S16 bcd2ascx(const U8 *bcd_buf, const U16 bcd_len, S8 *asc_buf, const U16 asc_len, const U8 flags, .../* const S8 fch */);
/**
 * @brief 转换BCD缓存为ASC字符串。
 * flags为0，转换的ASC字符在(0-9 a-f A-F)范围内，无填充。
 * 
 * @param a BCD缓存指针。
 * @param b BCD缓存长度。
 * @param c ASC字符串。
 * @param d ASC字符串长度。
 * 
 * @return 转换后的ASC字符串长度，转换失败返回负数。
 */
#define bcd2asc(a, b, c, d) bcd2ascx((a), (b), (c), (d), 0)

S8 *atrim(S8 *str);
S8 *ltrim(S8 *str);
S8 *rtrim(S8 *str);
S8 *trim(S8 *str);

bool isbreak(const S8 c);
bool isdigits(const S8 *s, const U16 nbytes);
bool isxdigits(const S8 *s, const U16 nbytes);

void ands(U8 *dest, const U8 *b1, const U8 *b2, const U16 nbytes);
void ors(U8 *dest, const U8 *b1, const U8 *b2, const U16 nbytes);
void xors(U8 *dest, const U8 *b1, const U8 *b2, const U16 nbytes);
void nots(U8 *dest, const U8 *b1, const U16 nbytes);

S8 *hexdumpx(void *vbuf, const U16 nbytes, const U8 flags, .../* const U8 hdx_ind */);
/**
 * @brief 根据样式返回字节流的十六进制数字串。
 * flags为0，单行显示十六进制数字串。
 *
 * @param a 输入字节流。
 * @param b 输入字节流长度。
 * 
 * @return 十六进制数字串，如果有异常返回空。
 */
#define hexdump(a, b) hexdumpx(a, (b), 0)
/**
 * @brief 根据样式返回字节流的十六进制数字串。
 * flags为HDX_MUL | HDX_LNUM | HDX_CNUM | HDX_ASC，显示列标记和行标记且支持多行显示（每行显示16个字节）十六进制数字串且在行末显示当前行的asc字符信息。
 *
 * @param a 输入字节流。
 * @param b 输入字节流长度。
 * 
 * @return 十六进制数字串，如果有异常返回空。
 */
#define hexdump1(a, b) hexdumpx(a, (b), HDX_MUL | HDX_LNUM | HDX_CNUM | HDX_ASC)

#endif /*__BYTEO_H__*/


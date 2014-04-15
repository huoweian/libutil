/**
 * Copyright(c) 2011, Free software organization.
 * All Rights Reserved.
 * 
 * @author kezf<kezf@foxmail.com>
 * @file  byteo.c
 * @brief 字符操作函数集。
 *
 * 包括的功能：
 *     @li数据类型转换函数(asc2bcd | bcd2asc | bcd2int | int2bcd | asc2int | int2asc)。
 *     @li字符串空格截断函数(trim | rtrim | ltrim | atrim)。
 *     @li字符校验函数(isbreak | isdigits)。
 *     @li字节流位运算函数(ands | ors | xors | nots | reverse_bit | reverse_bits)。
 *     @li十六进制数输出函数(hexdump)。
 */
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "byteo.h"

/**
 * @brief 转换ASC字符串为BCD缓存。
 * 
 * @param asc_buf ASC字符串。
 * @param asc_len ASC字符串长度。
 * @param bcd_buf BCD缓存指针。
 * @param bcd_len BCD缓存长度。
 * @param flags 转换参数。可以是(0 | ABC_NUM | ABC_FORE | ABC_FILL | ABC_FCH)中的任一值或组合值。
 *        ... fch 填充值。如果转换参数设置了ABC_FCH，必须设置该值，该值的ASC字符在(0-9 a-f A-F)范围内。
 * 
 * @return 转换后的BCD缓存长度，转换失败返回负数。
 */
S16 asc2bcdx(const S8 *asc_buf, const U16 asc_len, U8 *bcd_buf, const U16 bcd_len, const U8 flags, .../* const S8 fch */)
{
	S8 fch = '0';
	S16 i, conv_len = 0;
	U8 ch, ch1, ch2;
	va_list ap;

	if (!asc_buf || !bcd_buf) {
		errno = EBADF;
		return -1;
	}
	//判断BCD缓存是否足够		//* 4bit bcd码 = 8bit asc码
	if ((bcd_len << 1) < asc_len) {
		errno = ENOMEM;
		return -1;
	}
	//如果需要指定填充字符，则取得填充字符
	if (flags & ABC_FCH) {
		va_start(ap, flags);
		fch = va_arg(ap, int);
		//printf("fch = %x\n", fch);
		va_end(ap);
	}
	//格式化填充数据
	if (fch >= 'a' && fch <= 'f') {
		ch2 = fch - 'a' + 10;
	} else if (fch >= 'A' && fch <= 'F') {
		ch2 = fch - 'A' + 10;
	} else if (fch >= '0' && fch <= '9') {
		ch2 = fch - '0';
	} else {
		errno = EINVAL;
		return -1;
	}
	ch2 |= ch2 << 4;
	//如果要填充满BCD缓存，则先用填充数据填充满缓存
	if (flags & ABC_FILL) {
		memset(bcd_buf, ch2, bcd_len);
	}
	//在前填充和在后填充的不同处理
	ch1 = 0x55;
	if (flags & ABC_FORE) {
		if (flags & ABC_FILL) {
			bcd_buf += bcd_len - ((asc_len + 1) >> 1);
		}
		if (asc_len & 0x01) {
			ch1 = ch2 & 0x0F;
		}
	}
	//转换字符
	for (i = 0; i < asc_len; asc_buf++, i++) {
		if (flags & ABC_NUM) {
			if (*asc_buf >= '0' && *asc_buf <= '?') {
				ch = *asc_buf - '0';
			} else {
				errno = EINVAL;
				return -1;
			}
		} else {
			if (*asc_buf >= 'a' && *asc_buf <= 'f') {
				ch = *asc_buf - 'a' + 10;
			} else if (*asc_buf >= 'A' && *asc_buf <= 'F') {
				ch = *asc_buf - 'A' + 10;
			} else if (*asc_buf >= '0' && *asc_buf <= '9') {
				ch = *asc_buf - '0';
			} else {
				errno = EINVAL;
				return -1;
			}
		}
		if (ch1 == 0x55) {
			ch1 = ch;
		} else {
			*bcd_buf++ = ch1 << 4 | ch;	//前填充
			conv_len++;
			ch1 = 0x55;
		}
	}
	if (ch1 != 0x55) {
		*bcd_buf = ch1 << 4 | (ch2 & 0x0F);	//后填充
		conv_len++;
	}
	if (flags & ABC_FILL) {
		return bcd_len;
	} else {
		return conv_len;
	}
}

/**
 * @brief 转换BCD缓存为ASC字符串。
 *				BCD缓存是使用asc2bcdx转换的。
 * 
 * @param bcd_buf BCD缓存指针。
 * @param bcd_len BCD缓存长度。
 * @param asc_buf ASC字符串。
 * @param asc_len ASC字符串长度。
 * @param flags 转换参数。可以是(0, ABC_NUM, ABC_FORE, ABC_FILL, ABC_FCH)中的任一值或组合值。
 *        ... fch 填充值。如果转换参数设置了ABC_FCH，必须设置该值，该值的ASC字符在(0-9 a-f A-F)范围内。
 * 
 * @return 转换后的ASC字符串长度，转换失败返回负数。
 */
S16 bcd2ascx(const U8 *bcd_buf, const U16 bcd_len, S8 *asc_buf, const U16 asc_len, const U8 flags, .../* const S8 fch */)
{
	S8 fch = '0';
	S16 i = 0, j, conv_len;
	U8 ch1, ch2;
	va_list ap;
	
	if (!asc_buf || !bcd_buf) {
		errno = EBADF;
		return -1;
	}
	//如果需要指定填充字符，则取得填充字符
	if (flags & ABC_FCH) {
		va_start(ap, flags);
		fch = va_arg(ap, int);
		va_end(ap);
	}
	//格式化填充数据
	if (fch >= 'a' && fch <= 'f') {
		ch2 = fch - 'a' + 10;
	} else if (fch >= 'A' && fch <= 'F') {
		ch2 = fch - 'A' + 10;
	} else if (fch >= '0' && fch <= '9') {
		ch2 = fch - '0';
	} else {
		errno = EINVAL;
		return -1;
	}
	ch2 |= ch2 << 4;
	//计算实际转换的起始位置和数据长度
	conv_len = bcd_len << 1;
	if (flags & ABC_FILL) {
		if (flags & ABC_FORE) {
			while (conv_len > 0 && *bcd_buf == ch2) {
				bcd_buf++;
				conv_len -= 2;
			}
			if ((*bcd_buf & 0xf0) == (ch2 & 0xf0)) {
				i = 1;
				conv_len --;
			}
		} else {
			while (conv_len > 0 && *(bcd_buf + (conv_len >> 1) - 1) == ch2) {
				conv_len -= 2;
			}
			if ((*(bcd_buf + (conv_len >> 1) - 1) & 0x0f) == (ch2 & 0x0f)) {
				conv_len --;
			}
		}
	} else {									//最后一个BCD如果与填充符一样，无法确定是不是填充符号
/*		if (flags & ABC_FORE) {		//这里把他当做填充符号处理了所以需限制填充符号。
			if ((*bcd_buf & 0xf0) == (ch2 & 0xf0)) {
				i = 1;
				conv_len --;
			}
		} else {
			if ((*(bcd_buf + (conv_len >> 1) - 1) & 0x0f) == (ch2 & 0x0f)) {
				conv_len --;
			}
		}
*/
	}
	//判断ASC缓存是否足够
	if (conv_len > asc_len) {
		errno = ENOMEM;
		return -1;
	}

	//转换字符				
	j = conv_len + i;		
	for ( ; i < j; i++, asc_buf++) {
		ch1 = (i & 0x01) ? (*bcd_buf++ & 0x0f) : (*bcd_buf >> 4);
		if (flags & ABC_NUM) {
			ch1 += '0';
		} else {
			ch1 += (ch1 > 9) ? ('A' - 10) : '0';
		}
		*asc_buf = ch1;
	}
	return conv_len;
}

/**
 * @brief 截断字符串的全部空格（包含字符串中间）。
 * 
 * @param str 输入字符串。
 * 
 * @return 截断后的字符串指针。
 */
S8 *atrim(S8 *str)
{
	S8 *p, *q;
	p = q = str;
	while (*p) {
		if (!isspace(*p)) {
			*q++ = *p;
		}
		p++;
	}
	*q = '\0';
	return str;
}

/**
 * @brief 截断字符串的左空格。
 * 
 * @param str 输入字符串。
 * 
 * @return 截断后的字符串指针。
 */
S8 *ltrim(S8 *str)
{
	while (*str && isspace(*str)) str++;
	return str;
}

/**
 * @brief 截断字符串的右空格。
 * 
 * @param str 输入字符串。
 * 
 * @return 截断后的字符串指针。
 */
S8 *rtrim(S8 *str)
{
	S8 *p;
	p = str + strlen(str) - 1;
	while (p > str && isspace(*p)) {
		*p = '\0';
		p--;
	}
	return str;
}

/**
 * @brief 截断字符串的左右空格。
 * 
 * @param str 输入字符串。
 * 
 * @return 截断后的字符串指针。
 */
S8 *trim(S8 *str)
{
	S8 *p;
	p = ltrim(str);
	p = rtrim(p);
	return p;
}

/**
 * @brief 检查字符是否是换行符（'\r','\n'）。
 * 
 * @param c 输入字符。
 * 
 * @return true或false。
 */
bool isbreak(const S8 c)
{
	return ((c) == '\r' || (c) == '\n') ? true: false;
}

/**
 * @brief 检查字符串是否全是数字字符（0-9）。
 * 
 * @param s 输入字符串。
 * @param nbytes 输入字符串长度。
 * 
 * @return true或false。
 */
bool isdigits(const S8 *s, const U16 nbytes)
{
	S16 i;
	for (i = 0; i < nbytes; i++, s++) {
		if (!isdigit(*s))
			return false;
	}
	return true;
}

/**
 * @brief 检查字符串是否全是十六进制字符（0-9或a-f）。
 * 
 * @param s 输入字符串。
 * @param nbytes 输入字符串长度。
 * 
 * @return true或false。
 */
bool isxdigits(const S8 *s, const U16 nbytes)
{
	S16 i;
	for (i = 0; i < nbytes; i++, s++) {
		if (!isxdigit(*s))
			return false;
	}
	return true;
}

/**
 * @brief 按字节求与。
 * 
 * @param dest 输出缓存。
 * @param b1 输入1缓存。
 * @param b2 输入2缓存。
 * @param nbytes 输入缓存大小。
 */
void ands(U8 *dest, const U8 *b1, const U8 *b2, const U16 nbytes)
{
	S16 i;
	for (i = 0; i < nbytes; i++) {
		*dest++ = *b1++ & *b2++;
	}
}

/**
 * @brief 按字节求或。
 * 
 * @param dest 输出缓存。
 * @param b1 输入1缓存。
 * @param b2 输入2缓存。
 * @param nbytes 输入缓存大小。
 */
void ors(U8 *dest, const U8 *b1, const U8 *b2, const U16 nbytes)
{
	S16 i;
	for (i = 0; i < nbytes; i++) {
		*dest++ = *b1++ | *b2++;
	}
}

/**
 * @brief 按字节求异或。
 * 
 * @param dest 输出缓存。
 * @param b1 输入1缓存。
 * @param b2 输入2缓存。
 * @param nbytes 输入缓存大小。
 */
void xors(U8 *dest, const U8 *b1, const U8 *b2, const U16 nbytes)
{
	S16 i;
	for (i = 0; i < nbytes; i++) {
		*dest++ = *b1++ ^ *b2++;
	}
}

/**
 * @brief 按字节取反。
 * 
 * @param dest 输出缓存。
 * @param b1 输入缓存。
 * @param nbytes 输入缓存大小。
 */
void nots(U8 *dest, const U8 *b1, const U16 nbytes)
{
	S16 i;
	for (i = 0; i < nbytes; i++) {
		*dest++ = ~*b1++;
	}
}

#define HEXDUMP_SIZE (1024 * 20)
S8 hexdump_buf[HEXDUMP_SIZE + 1];

S8 *strcpyx(S8 *p, const S8 *s)
{
	S16 l;
	strcpy(p, s);
	l = strlen(s);
	p += l;
	return p;
}

S8 *sprintfx(S8 *p, const S8 *s, ...)
{
	S16 l;
	va_list args;

	va_start(args, s);
	vsprintf(p, s, args);
	va_end(args);
	l = strlen(p);
	p += l;
	return p;
}

/**
 * @brief 根据指定的样式返回字节流的十六进制数字串。
 * 
 * @param vbuf 输入字节流。
 * @param nbytes 输入字节流长度。
 * @param flags 样式参数。可以是(0, HDX_MUL, HDX_LNUM, HDX_CNUM, HDX_ASC, HDX_JOIN)中的任一值或组合值。
 * @param ... 连接索引。如果样式参数设置了HDX_JOIN，必须设置该值，该值为0-255整数。
 *  		  注意：printf、sprintf等函数的第三个参数是从后往前取值，索引顺序也必须从后往前递增。
 *  		  如：printf("%s%s", hexdumpx(buf, len, HDX_JOIN,
 *  		  1), hexdumpx(buf, len, HDX_JOIN, 0));
 * 
 * @return 十六进制数字串，如果有异常返回空。
 */
S8 *hexdumpx(void *vbuf, const U16 nbytes, const U8 flags, .../* const U8 hdx_ind */)
{
	S16 i, j;
	U8 hdx_ind;
	S8 *s, *e, *p, *sz = NULL, *psz = NULL;
	va_list ap;
	U8 *buf = (U8 *)vbuf;
	
	if (flags & HDX_ASC) {
		sz = (S8 *)malloc(nbytes * sizeof(S8) + 1);
		if (!sz)
			return NULL;
		psz = sz;
	}
	s = hexdump_buf;
	e = hexdump_buf + sizeof(hexdump_buf);
	if (flags & HDX_JOIN) {
		va_start(ap, flags);
		hdx_ind = va_arg(ap, int);
		va_end(ap);
		while (hdx_ind--) {
			s += strlen(s) + 1;
			if (s > e) return NULL;
		}
	}
	*s = '\0';
	p = s;
	
	if (flags & HDX_CNUM) {
		if (flags & HDX_LNUM) {
			p = strcpyx(p, "           ");
		}
		p = strcpyx(p, "0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  \n");
		if (flags & HDX_LNUM) {
			p = strcpyx(p, "-----------");
		}
		p = strcpyx(p, "------------------------------------------------------------------\n");
	}
	
	for (i = 0; i < nbytes; buf++, i++) {
		if (flags & HDX_LNUM) {
			if (i == 0 || ((flags & HDX_MUL) && !(i & 0x0f))) {
				p = sprintfx(p, "%08Xh: ", buf);
			}
		}
		p = sprintfx(p, "%02X ", *buf);
		if ((flags & HDX_ASC)) {
			if (*buf == '\0' || isspace(*buf)) {
				*psz++ = '.';
			} else {
				*psz++ = *buf;
			}
			if (i == nbytes - 1 || ((flags & HDX_MUL) && !((i & 0x0f) ^ 0x0f))) {
				if (flags & HDX_MUL) {
					for (j = 0; j < ((i & 0x0f) ^ 0x0f); j++)
						p = strcpyx(p, "   ");
				}
				*psz = '\0';
				p = sprintfx(p, ";%s", sz);
				psz = sz;
			}
		}
		if (i == nbytes - 1 || ((flags & HDX_MUL) && !((i & 0x0f) ^ 0x0f))) {
			p = strcpyx(p, "\n");
		}
	}
	if (flags & HDX_CNUM) {
		if (flags & HDX_LNUM) {
			p = strcpyx(p, "-----------");
		}
		p = strcpyx(p, "------------------------------------------------------------------\n");
	}
	if ((flags & HDX_ASC) && sz) {
		free(sz);
	}
	return s;
}


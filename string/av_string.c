#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "av_string.h"

//const char *str; 	str不能指向其他字符串，但可以改变它指向字符串的内容。
//char * const str; 	str可以指向其他字符串，但不可以改变它指向字符串的内容。
int av_strstart(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && (*pfx == *str)) {
        pfx++;
        str++;
    }
    if (!*pfx && ptr)
        *ptr = str;
    return !*pfx;
}

int av_stristart(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && (toupper((unsigned)*pfx) == toupper((unsigned)*str))) {
        pfx++;
        str++;
    }
    if (!*pfx && ptr)
        *ptr = str;
    return !*pfx;
}

char *av_stristr(char *s1, const char *s2)
{
    if (!*s2)
        return s1;

    do {
        if (av_stristart(s1, s2, NULL))
            return s1;
    } while (*s1++);

    return NULL;
}

size_t av_strlcpy(char *dst, const char *src, size_t size)
{
    size_t len = 0;
    while (++len < size && *src)
        *dst++ = *src++;
    if (len <= size)
        *dst = 0;
    return len + strlen(src) - 1;
}

size_t av_strlcat(char *dst, const char *src, size_t size)
{
    size_t len = strlen(dst);
    if (size <= len + 1)
        return len + strlen(src);
    return len + av_strlcpy(dst + len, src, size - len);
}

size_t av_strlcatf(char *dst, size_t size, const char *fmt, ...)
{
    unsigned int len = strlen(dst);
    va_list vl;

    va_start(vl, fmt);
    len += vsnprintf(dst + len, size > len ? size - len : 0, fmt, vl);
    va_end(vl);

    return len;
}

char *av_d2str(double d)
{
    char *str= (char *)malloc(16);
	memset(str, 0, 16);
    if(str) snprintf(str, 16, "%f", d);
    return str;
}

#define WHITESPACES " \n\t" //空格、\n、\t
#define FF_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

char *av_get_token(const char **buf, const char *term)
{
    char *out = (char *)malloc(strlen(*buf) + 1);
	memset(out, 0, strlen(*buf) +1 );
    char *ret= out, *end= out;
    const char *p = *buf;
    if (!out) return NULL;
    p += strspn(p, WHITESPACES); //strspn返回字符串中第一个
													//不在指定字符串中出现的字符下标
	//printf("1--%s\n", p);
	
    while(*p && !strspn(p, term)) {
        char c = *p++;
        if(c == '\\' && *p){	//c == \,截断
            *out++ = *p++;
            end= out;				
        }else if(c == '\''){	// 消除 '
            while(*p && *p != '\'') 
                *out++ = *p++;
            if(*p){
                p++;
                end= out;
            }
        }else{
            *out++ = c;
        }
    }
	
    do{
        *out-- = 0;
    }while(out >= end && strspn(out, WHITESPACES));

    *buf = p;

    return ret;
}

void *mallocz(size_t size)
{
	void *ptr = malloc(size);
    if (ptr)
        memset(ptr, 0, size);
    return ptr;
}

#ifdef TEST

#undef printf

int main(void)
{
    int i;

    printf("Testing av_get_token()\n");
    {
        const char *strings[] = {
            "''",
            "",
            ":",
            "\\",
           "'",
            "    '2'    :",
            "    ''  ''  :",
            ":foo   '' :",
            "'foo'",
            "foo     ",
            "  '  foo  '  ",
            "foo\\",
            "foo':  blah:blah",
            "foo\\:  blah:blah",
            "foo\'",
            "'foo :  '  :blahblah",
            "\\ :blah",
            "     foo",
            "      foo       ",
            "      foo     \\ ",
            "foo ':blah",
            " foo   bar    :   blahblah",
            "\\f\\o\\o",
            "'foo : \\ \\  '   : blahblah",
            "'\\fo\\o:': blahblah",
            "\\'fo\\o\\:':  foo  '  :blahblah"
        };

        for (i=0; i < FF_ARRAY_ELEMS(strings); i++) {
            const char *p= strings[i];
            printf("|%s|", p);
            printf(" -> |%s|", av_get_token(&p, ":"));
            printf(" + |%s|\n", p);
        }
    }

    return 0;
}

#endif /* TEST */

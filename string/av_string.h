#ifndef AV_STRING_H_
#define AV_STRING_H_

#include <stddef.h>

//#define TEST

/**
 * Return non-zero if pfx is a prefix of str. If it is, *ptr is set to
 * the address of the first character in str after the prefix.
 *
 * @param str input string
 * @param pfx prefix to test
 * @param ptr updated if the prefix is matched inside str
 * @return non-zero if the prefix matches, zero otherwise
 */
int av_strstart(const char *str, const char *pfx, const char **ptr);

/**
 * Return non-zero if pfx is a prefix of str independent of case. If
 * it is, *ptr is set to the address of the first character in str
 * after the prefix.
 *
 * @param str input string
 * @param pfx prefix to test
 * @param ptr updated if the prefix is matched inside str
 * @return non-zero if the prefix matches, zero otherwise
 */
 int av_stristart(const char *str, const char *pfx, const char **ptr);

 /**
 * Locate the first case-independent occurrence in the string haystack
 * of the string needle.  A zero-length string needle is considered to
 * match at the start of haystack.
 *
 * This function is a case-insensitive version of the standard strstr().
 *
 * @param s1 string to search in
 * @param s2   string to search for
 * @return         pointer to the located match within haystack
 *                 or a null pointer if no match
 */
char *av_stristr(char *s1, const char *s2);
 
/**
 * Copy the string src to dst, but no more than size - 1 bytes, and
 * null-terminate dst.
 *
 * This function is the same as BSD strlcpy().
 *
 * @param dst destination buffer
 * @param src source string
 * @param size size of destination buffer
 * @return the length of src
 *
 * WARNING: since the return value is the length of src, src absolutely
 * _must_ be a properly 0-terminated string, otherwise this will read beyond
 * the end of the buffer and possibly crash.
 */
size_t av_strlcpy(char *dst, const char *src, size_t size);

/**
 * Append the string src to the string dst, but to a total length of
 * no more than size - 1 bytes, and null-terminate dst.
 *
 * This function is similar to BSD strlcat(), but differs when
 * size <= strlen(dst).
 *
 * @param dst destination buffer
 * @param src source string
 * @param size size of destination buffer
 * @return the total length of src and dst
 *
 * WARNING: since the return value use the length of src and dst, these absolutely
 * _must_ be a properly 0-terminated strings, otherwise this will read beyond
 * the end of the buffer and possibly crash.
 */
size_t av_strlcat(char *dst, const char *src, size_t size);

/**
 * Append output to a string, according to a format. Never write out of
 * the destination buffer, and always put a terminating 0 within
 * the buffer.
 * @param dst destination buffer (string to which the output is
 *  appended)
 * @param size total size of the destination buffer
 * @param fmt printf-compatible format string, specifying how the
 *  following parameters are used
 * @return the length of the string that would have been generated
 *  if enough space had been available
 */
size_t av_strlcatf(char *dst, size_t size, const char *fmt, ...);

/**
 * Convert a number to a av_malloced string.
 */
char *av_d2str(double d);

void *mallocz(size_t size);

#endif //AV_STRING_H_

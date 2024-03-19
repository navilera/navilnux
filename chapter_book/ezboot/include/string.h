//------------------------------------------------------------------------------
// 파 일 명 : string.h
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 문자열 처리 관련 헤더 
// 작 성 자 : 유영창
// 작 성 일 : 2001년 11월 3일
// 수 정 일 : 
// 주    의 : 이 헤더 화일의 대부분의 내용은  리눅스 string.h에서
//            에서 가져 왔다. 
//------------------------------------------------------------------------------

#ifndef _STRING_HEADER_
#define _STRING_HEADER_

extern char * ___strtok;
extern char * strpbrk(const char *,const char *);
extern char * strtok(char *,const char *);
extern char * strsep(char **,const char *);
extern __kernel_size_t strspn(const char *,const char *);
extern char * strcpy(char *,const char *);
extern char * strncpy(char *,const char *, __kernel_size_t);
extern char * strcat(char *, const char *);
extern char * strncat(char *, const char *, __kernel_size_t);
extern int strcmp(const char *,const char *);
extern int strncmp(const char *,const char *,__kernel_size_t);
extern int strnicmp(const char *, const char *, __kernel_size_t);
extern char * strchr(const char *,int);
extern char * strrchr(const char *,int);
extern char * strstr(const char *,const char *);
extern __kernel_size_t strlen(const char *);
extern __kernel_size_t strnlen(const char *,__kernel_size_t);
extern void * memset(void *,int,__kernel_size_t);
extern void * memcpy(void *,const void *,__kernel_size_t);
extern void * memmove(void *,const void *,__kernel_size_t);
extern void * memscan(void *,int,__kernel_size_t);
extern int memcmp(const void *,const void *,__kernel_size_t);
extern void * memchr(const void *,int,__kernel_size_t);
extern unsigned long strtoul(const char *p, char **out_p, int base);

extern void UpperStr( char *Str );
extern void LowerStr( char *Str );

#endif // _STRING_HEADER_


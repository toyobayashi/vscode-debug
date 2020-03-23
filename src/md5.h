#ifndef __MD5_H__
#define __MD5_H__

#ifdef __cplusplus
  #define EXTERN_C_START extern "C" {
  #define EXTERN_C_END }
#else
  #define EXTERN_C_START
  #define EXTERN_C_END
#endif

#ifdef _WIN32
  #ifdef CCPM_BUILD_DLL_md5
  #define MD5_API __declspec(dllexport)
  #else
  // #define MD5_API __declspec(dllimport)
  #define MD5_API
  #endif
#else
  #ifdef CCPM_BUILD_DLL_md5
  #define MD5_API __attribute__((visibility("default")))
  #else
  #define MD5_API
  #endif
#endif

EXTERN_C_START

struct md5_hash;
typedef struct md5_hash md5_hash;

MD5_API md5_hash* md5_init();
MD5_API int md5_update(md5_hash*, const unsigned char*, int);
MD5_API int md5_digest(md5_hash*, char*);
MD5_API void md5_free(md5_hash*);
MD5_API int md5_get_last_error();
MD5_API const char* md5_get_error_message(int);
MD5_API void md5_copy(md5_hash*, md5_hash*);
MD5_API int md5_cmp(md5_hash*, md5_hash*, int*);

EXTERN_C_END

#endif

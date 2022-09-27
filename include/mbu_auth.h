#ifndef __MBU_AUTH_H__
#define __MBU_AUTH_H__

#ifdef MBUAUTH_DLL
#ifdef MBUAUTH_BUILD
#define MBUAUTH_EXPORT extern "C" __declspec(dllexport)
#else
#define MBUAUTH_EXPORT extern "C" __declspec(dllimport)
#endif
#else
#define MBUAUTH_EXPORT
#endif

MBUAUTH_EXPORT void test();

#endif // __MBU_AUTH_H__

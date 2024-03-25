#pragma once

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FILESEARCH_SDK_EXPORTS
#define SE_DECL __declspec(dllexport)
#else
#define SE_DECL __declspec(dllimport)

#pragma comment(lib, "filesearch_sdk.lib")

#endif

enum SE_TEXT_MATCH_TYPE {
  SETextMatchNormal,
  SETextMatchWholeWord,
  SETextMatchWildcard,
  SETextMatchRegex,
};

typedef PVOID SE_HANDLE;

typedef BOOL(__stdcall *SE_CFGVOL_CALLBACK)(BOOL save, WCHAR drive_letter,
                                            BOOL *enable, BOOL *enable_monitor,
                                            PVOID ctx);
typedef BOOL(__stdcall *SE_CFGSEARCH_CALLBACK)(BOOL save,
                                               BOOL *case_insensitive,
                                               SE_TEXT_MATCH_TYPE *type,
                                               PVOID ctx);
typedef void(__stdcall *SE_SCANPROG_CALLBACK)(LPCTSTR current_drive, PVOID ctx);

#define FILE_REASON_CREATE 1
#define FILE_REASON_RENAME 2
#define FILE_REASON_DELETE 3
typedef void(__stdcall *SE_MONITOR_CALLBACK)(ULONGLONG file_id,
                                             LPCWSTR file_path, ULONG reason,
                                             PVOID ctx);

typedef struct _SE_CREATE_CALLBACKS {
  SE_CFGVOL_CALLBACK ConfigVolume;
  SE_CFGSEARCH_CALLBACK ConfigSearch;
  SE_SCANPROG_CALLBACK ScanProgress;
  SE_MONITOR_CALLBACK Monitor;
  PVOID Context;
} SE_CREATE_CALLBACKS, *PSE_CREATE_CALLBACKS;

typedef BOOL(__stdcall *SE_RESTART_SEARCH)(PVOID context);
typedef void(__stdcall *SE_SEARCH_ON_FINDONE)(LPCWSTR filepath, PVOID context);
typedef void(__stdcall *SE_SEARCH_COUNT)(ULONGLONG count, PVOID context);

typedef struct _SE_SEARCH_CALLBACKS {
  SE_SEARCH_ON_FINDONE SearchOnFindOne;
  SE_RESTART_SEARCH RestartSearch;
  SE_SEARCH_COUNT SearchCount;
  PVOID Context;
} SE_SEARCH_CALLBACKS, *PSE_SEARCH_CALLBACKS;

SE_HANDLE __stdcall SearchEngineCreate(PSE_CREATE_CALLBACKS callback);

void __stdcall SearchEngineStart(SE_HANDLE se);

ULONG __stdcall SearchEngineWaitScanFinish(SE_HANDLE se, ULONG timeout);

void __stdcall SearchEngineStop(SE_HANDLE se);

void __stdcall SearchEngineDelete(SE_HANDLE se);

BOOL __stdcall SearchEngineSearch(SE_HANDLE se, LPCWSTR str,
                                  PSE_SEARCH_CALLBACKS callback,
                                  BOOL use_file_id);

void __stdcall SearchEngineUseDatabse(SE_HANDLE se, BOOL use);

BOOL __stdcall SearchEngineIsUseDatabase(SE_HANDLE se);

void __stdcall SearchEngineSetDatabasePath(SE_HANDLE se, LPCWSTR path);

BOOL __stdcall SearchEngineGetFullPath(SE_HANDLE se, ULONGLONG file_id,
                                       LPWSTR file_path, ULONG length);

void __stdcall SearchEngineReload(SE_HANDLE se);

BOOL __stdcall SearchEngineTextMatch(SE_HANDLE se, LPCWSTR pattern,
                                     LPCWSTR str);

#ifdef __cplusplus
}
#endif
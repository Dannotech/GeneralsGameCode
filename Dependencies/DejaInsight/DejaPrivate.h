//==============================================================================
//
//  DejaPrivate.h
//
//==============================================================================
//  Copyright (C) DejaTools, LLC.  All rights reserved.
//==============================================================================

#ifndef DEJA_PRIVATE_H
#define DEJA_PRIVATE_H

//==============================================================================
//  INCLUDES
//==============================================================================

#include <stdarg.h>

//==============================================================================
//  DYNAMIC LINK LIBRARY MACROS
//==============================================================================

#ifdef _DLL
    #ifdef DEJA_DLL_EXPORT
        #define DEJA_V_DECL __declspec(dllexport)
        #define DEJA_F_DECL __declspec(dllexport)
    #else
        #define DEJA_V_DECL __declspec(dllimport)
        #define DEJA_F_DECL 
    #endif
#else
        #define DEJA_V_DECL
        #define DEJA_F_DECL
#endif

//==============================================================================
//  PLATFORM AND COMPILER AWARENESS
//==============================================================================

// Check if the target has already been defined by the x_files.
#ifndef TARGET_DEFINED    

    #if (defined( DEJA_TARGET_WIN ))
        #define TARGET_WIN

    #elif (defined( DEJA_TARGET_XBOX ))
        #define TARGET_XBOX360

    #elif (defined( DEJA_TARGET_PS3 ))
        #define TARGET_PS3

    #elif (defined( DEJA_TARGET_PS4 ))
        #define TARGET_PS4

    #elif (defined( DEJA_TARGET_VITA ))
        #define TARGET_VITA

    #elif (defined( DEJA_TARGET_OSX ))
        #define TARGET_OSX

    #elif (defined( DEJA_TARGET_LINUX ))
        #define TARGET_LINUX

    #elif (defined( XBOX ) || defined( _XBOX ))
        #define TARGET_XBOX360

    #elif (defined( PS3 ) || defined( _PS3 ) || defined( __CELLOS_LV2__ ))
        #define TARGET_PS3

    #elif defined( __ORBIS__ )
        #define TARGET_PS4

    #elif (defined( WIN32 ) || defined( _WIN32 ))
        #define TARGET_WIN

    #elif (defined( __APPLE__ ))
        #define TARGET_OSX

    #elif (defined( RVL ) && defined( __CWCC__ ))
        #define TARGET_WII

    #elif (defined( CAFE ))
        #define TARGET_CAFE

    #elif defined( __MWERKS__ )
        #define TARGET_DS

    #elif defined( __linux__ )
        #define TARGET_LINUX

    #elif defined( __ARMCC_VERSION )
        #define TARGET_3DS

    #elif defined( __psp2__ )
        #define TARGET_VITA

    #elif defined( __NX__ )
        #define TARGET_NX

    #else
        #error Could not determine the compiler/platform.
    #endif

    #define TARGET_DEFINED

#endif

//------------------------------------------------------------------------------
//  Does the compiler support "variadic macros"?  That is, macros with variable 
//  arguments via an ellipsis such as:
//      #define MACRO( Arg1, Arg2, ... )
//------------------------------------------------------------------------------

#if defined( _MSC_VER )
    // Microsoft compilers from 2005 or later.
    #if _MSC_VER >= 1400
    #define VARIADIC_MACROS
    #endif
#elif defined( TARGET_WII )
    // Incomplete VARIADIC support on Wii at this time.
#else
    // All other compilers supported to date.
    #define VARIADIC_MACROS
#endif

//------------------------------------------------------------------------------
//  Under Microsoft compilers, we can use a pragma to get the library linked
//  into the target application.  The customer will still have to provide path
//  information to find the lib.
//------------------------------------------------------------------------------

//** Unfortunately, we can't figure out how to squelch runtime library mismatch 
//** link warnings, so we're going to pass on the pragma technique for now.
//#ifdef TARGET_WIN
//#pragma comment( lib, "DejaLib.Win32.lib" )
//#endif

//------------------------------------------------------------------------------
//  DejaLib performance and behavior metrics.
//------------------------------------------------------------------------------

enum deja_metric
{
    DEJA_METRIC_CACHE_FILE_CAPACITY,
    DEJA_METRIC_CACHE_FILE_ENTRIES,
    DEJA_METRIC_CACHE_FILE_HITS,
    DEJA_METRIC_CACHE_FILE_MISSES,
    DEJA_METRIC_CACHE_FILE_BUMPS,

    DEJA_METRIC_CACHE_LABEL_CAPACITY,
    DEJA_METRIC_CACHE_LABEL_ENTRIES,
    DEJA_METRIC_CACHE_LABEL_HITS,
    DEJA_METRIC_CACHE_LABEL_MISSES,
    DEJA_METRIC_CACHE_LABEL_BUMPS,

    DEJA_METRIC_CACHE_CHANNEL_CAPACITY,
    DEJA_METRIC_CACHE_CHANNEL_ENTRIES,
    DEJA_METRIC_CACHE_CHANNEL_HITS,
    DEJA_METRIC_CACHE_CHANNEL_MISSES,
    DEJA_METRIC_CACHE_CHANNEL_BUMPS,

    DEJA_METRIC_CACHE_FORMAT_CAPACITY,
    DEJA_METRIC_CACHE_FORMAT_ENTRIES,
    DEJA_METRIC_CACHE_FORMAT_HITS,
    DEJA_METRIC_CACHE_FORMAT_MISSES,
    DEJA_METRIC_CACHE_FORMAT_BUMPS,

    DEJA_METRIC_BUFFER_COUNT,           // Number of buffers.
    DEJA_METRIC_BUFFER_BYTES_PER,       // Number of bytes per each buffer.
    DEJA_METRIC_BUFFER_STALLS,          // All buffers full.  Must stall!
    DEJA_METRIC_BUFFER_TRANSMITS        // Number of buffers transmitted.
};

int DEJA_GET_METRIC     ( deja_metric Metric );

//==============================================================================
//  FUNCTIONS AND STRUCTURES
//==============================================================================

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// The realloc hooks are being retired.  They are here for now to maintain 
// backwards compatability with existing code.
typedef void* deja_std_realloc_fn( void*, size_t                              );
typedef void* deja_dbg_realloc_fn( void*, size_t, const char* pFile, int Line );
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//------------------------------------------------------------------------------
//  General
//------------------------------------------------------------------------------

DEJA_F_DECL void        DejaLock            (       void );
DEJA_F_DECL void        DejaLock            ( const char*     pFileName, int Line );
DEJA_F_DECL void        DejaLock            ( const wchar_t*  pFileName, int Line );
DEJA_F_DECL void        DejaUnlock          (       void );
DEJA_F_DECL void        DejaFlush           (       bool      Block = true );
DEJA_F_DECL void        DejaAutoFlush       (       void );
DEJA_F_DECL void        DejaSetAutoFlush    (       bool      AutoFlush,
                                              const char*     pFileName, 
                                                    int       Line );
DEJA_F_DECL bool        DejaLibConnected    (       void );
DEJA_F_DECL char*       DejaGetLastError    (       void );
DEJA_F_DECL void        DejaCommand         ( const char*     pCommand );
DEJA_F_DECL void        DejaAppLabel        ( const char*     pNLabelFormat, ... );
DEJA_F_DECL void        DejaAppLabel        ( const wchar_t*  pWLabelFormat, ... );
DEJA_F_DECL void        DejaThreadLabel     ( const char*     pNLabelFormat, ... );
DEJA_F_DECL void        DejaThreadLabel     ( const wchar_t*  pWLabelFormat, ... );
DEJA_F_DECL int         DejaGetMetric       ( deja_metric     Metric );
DEJA_F_DECL void        DejaSetParameter    ( deja_parameter  Parameter, int Value );
DEJA_F_DECL void        DejaSetValue        ( const char*     pVariable, int Value,
                                              const char*     pFileName, int Line );
DEJA_F_DECL void        DejaSetValue        ( const wchar_t*  pVariable, int Value,
                                              const char*     pFileName, int Line );
DEJA_F_DECL void        DejaScopeObject     ( const void*     pAddr );
DEJA_F_DECL void        DejaNextFrame       (       void );
DEJA_F_DECL void        DejaThreadInit      (       void );
DEJA_F_DECL void        DejaThreadKill      (       int       ExitCode );
DEJA_F_DECL void        DejaTerminate       (       void );
DEJA_F_DECL void        DejaEnumAlias       ( const char*     pType, 
                                                    int       Value,
                                              const char*     pNAlias,
                                                    bool      bAlias );
DEJA_F_DECL void        DejaEnumAlias       ( const char*     pType, 
                                                    int       Value,
                                              const wchar_t*  pWAlias,
                                                    bool      bAlias );

//------------------------------------------------------------------------------
//  Heap Tracking
//------------------------------------------------------------------------------

DEJA_F_DECL void        DejaSetMallocFn     ( deja_std_malloc_fn*  );
DEJA_F_DECL void        DejaSetMallocFn     ( deja_dbg_malloc_fn*  );
DEJA_F_DECL void        DejaSetReallocFn    ( deja_std_realloc_fn* );
DEJA_F_DECL void        DejaSetReallocFn    ( deja_dbg_realloc_fn* );
DEJA_F_DECL void        DejaSetFreeFn       ( deja_std_free_fn*    );
DEJA_F_DECL void        DejaSetFreeFn       ( deja_dbg_free_fn*    );

DEJA_F_DECL void        DejaLogMalloc       ( void* pMemory, size_t Size );
DEJA_F_DECL void        DejaLogMalloc       ( void* pMemory, size_t Size, 
                                              const char* pFileName, int Line );
DEJA_F_DECL void        DejaLogMalloc       ( void* pMemory, size_t Size, 
                                              const wchar_t* pFileName, int Line );

DEJA_F_DECL void        DejaLogRealloc      ( void* pNewMem, void* pOldMem, size_t NewSize );
DEJA_F_DECL void        DejaLogRealloc      ( void* pNewMem, void* pOldMem, size_t NewSize, 
                                              const char* pFileName, int Line );
DEJA_F_DECL void        DejaLogRealloc      ( void* pNewMem, void* pOldMem, size_t NewSize, 
                                              const wchar_t* pFileName, int Line );

DEJA_F_DECL void        DejaLogFree         ( void* pMemory );
DEJA_F_DECL void        DejaLogFree         ( void* pMemory, 
                                              const char* pFileName, int Line );
DEJA_F_DECL void        DejaLogFree         ( void* pMemory, 
                                              const wchar_t* pFileName, int Line );

DEJA_F_DECL void        DejaLogNew          ( void* pMemory, size_t Size );
DEJA_F_DECL void        DejaLogNew          ( void* pMemory, size_t Size, 
                                              const char* pFileName, int Line );
DEJA_F_DECL void        DejaLogNew          ( void* pMemory, size_t Size, 
                                              const wchar_t* pFileName, int Line );

DEJA_F_DECL void        DejaLogDelete       ( void* pMemory );
DEJA_F_DECL void        DejaLogDelete       ( void* pMemory, 
                                              const char* pFileName, int Line );
DEJA_F_DECL void        DejaLogDelete       ( void* pMemory, 
                                              const wchar_t* pFileName, int Line );

DEJA_F_DECL void        DejaLogNewArray     ( void* pMemory, size_t Size );
DEJA_F_DECL void        DejaLogNewArray     ( void* pMemory, size_t Size, 
                                              const char* pFileName, int Line );
DEJA_F_DECL void        DejaLogNewArray     ( void* pMemory, size_t Size, 
                                              const wchar_t* pFileName, int Line );

DEJA_F_DECL void        DejaLogDeleteArray  ( void* pMemory );
DEJA_F_DECL void        DejaLogDeleteArray  ( void* pMemory, 
                                              const char* pFileName, int Line );
DEJA_F_DECL void        DejaLogDeleteArray  ( void* pMemory, 
                                              const wchar_t* pFileName, int Line );

//------------------------------------------------------------------------------
//  Console
//------------------------------------------------------------------------------

DEJA_F_DECL void        DejaConsoleWrite    ( const char*    pNFormat, ... );
DEJA_F_DECL void        DejaConsoleWrite    ( const wchar_t* pWFormat, ... );
DEJA_F_DECL void        DejaConsoleEcho     ( const char*    pEcho );
DEJA_F_DECL void        DejaConsoleRead     (       char*    pBuffer, int Buffersize );

//------------------------------------------------------------------------------
//  Platform Specific
//------------------------------------------------------------------------------

DEJA_F_DECL void        DejaWiiReady        ( void );
DEJA_F_DECL void        DejaWiiReady        ( const char* pCommandLine );
DEJA_F_DECL void        DejaWiiReady        ( int argc, char* argv[] );

DEJA_V_DECL extern bool DejaSystemActive;

struct deja_object_scope
{
    deja_object_scope( const void* pAddr, const char* pFileName, int Line )
    {
        if( DejaSystemActive )
        {
            DejaLock( pFileName, Line );
            DejaScopeObject( pAddr );
            DejaUnlock();
        }
    }
   ~deja_object_scope( void )
    {
        if( DejaSystemActive )
        {
            DejaLock();
            DejaScopeObject( 0 );
            DejaUnlock();
        }
    }   
};

//------------------------------------------------------------------------------
//  Logging - C++
//------------------------------------------------------------------------------

struct deja_trace_reg
{
public:

    int     Data;
    void*   pData;

public:

    // Variadic registration.

    DEJA_F_DECL void    operator ()         (       int       TraceCode,
                                              const char*     pFileName,
                                                    int       Line,
                                              const char*     pStringizedChannel,
                                              const char*     pStringizedFormat,
                                              const char*     pNChannel,
                                              const char*     pNFormat );

    DEJA_F_DECL void    operator ()         (       int       TraceCode,
                                              const char*     pFileName,
                                                    int       Line,
                                              const char*     pStringizedChannel,
                                              const char*     pStringizedFormat,
                                              const wchar_t*  pWChannel,
                                              const char*     pNFormat );

    DEJA_F_DECL void    operator ()         (       int       TraceCode,
                                              const char*     pFileName,
                                                    int       Line,
                                              const char*     pStringizedChannel,
                                              const char*     pStringizedFormat,
                                              const char*     pNChannel,
                                              const wchar_t*  pWFormat );

    DEJA_F_DECL void    operator ()         (       int       TraceCode,
                                              const char*     pFileName,
                                                    int       Line,
                                              const char*     pStringizedChannel,
                                              const char*     pStringizedFormat,
                                              const wchar_t*  pWChannel,
                                              const wchar_t*  pWFormat );

    // Logging.

    DEJA_F_DECL void    operator ()         ( const char*     pNChannel,
                                              const char*     pNFormat,  ... );

    DEJA_F_DECL void    operator ()         ( const wchar_t*  pWChannel,
                                              const char*     pNFormat,  ... );

    DEJA_F_DECL void    operator ()         ( const char*     pNChannel,
                                              const wchar_t*  pWFormat,  ... );

    DEJA_F_DECL void    operator ()         ( const wchar_t*  pWChannel,
                                              const wchar_t*  pWFormat,  ... );
};

//------------------------------------------------------------------------------

DEJA_F_DECL void        DejaTrace           ( deja_trace_reg* pTraceReg, 
                                                    int       TraceCode,
                                              const char*     pFileName,
                                                    int       Line,
                                              const char*     pStringizedChannel,
                                              const char*     pStringizedFormat,
                                              const char*     pNChannel,
                                              const char*     pNFormat,  ... );

DEJA_F_DECL void        DejaTrace           ( deja_trace_reg* pTraceReg, 
                                                    int       TraceCode,
                                              const char*     pFileName,
                                                    int       Line,
                                              const char*     pStringizedChannel,
                                              const char*     pStringizedFormat,
                                              const wchar_t*  pWChannel,
                                              const char*     pNFormat,  ... );

DEJA_F_DECL void        DejaTrace           ( deja_trace_reg* pTraceReg, 
                                                    int       TraceCode,
                                              const char*     pFileName,
                                                    int       Line,
                                              const char*     pStringizedChannel,
                                              const char*     pStringizedFormat,
                                              const char*     pNChannel,
                                              const wchar_t*  pWFormat,  ... );

DEJA_F_DECL void        DejaTrace           ( deja_trace_reg* pTraceReg, 
                                                    int       TraceCode,
                                              const char*     pFileName,
                                                    int       Line,
                                              const char*     pStringizedChannel,
                                              const char*     pStringizedFormat,
                                              const wchar_t*  pWChannel,
                                              const wchar_t*  pWFormat,  ... );

DEJA_F_DECL void        DejaTraceNV         ( deja_trace_reg* pTraceReg, 
                                                    int       TraceCode );

DEJA_F_DECL void        DejaTraceNV         ( const char*    pChannel, const char*    pFormat, ... );
DEJA_F_DECL void        DejaTraceNV         ( const wchar_t* pChannel, const char*    pFormat, ... );
DEJA_F_DECL void        DejaTraceNV         ( const char*    pChannel, const wchar_t* pFormat, ... );
DEJA_F_DECL void        DejaTraceNV         ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

//------------------------------------------------------------------------------
//  Logging - Script
//------------------------------------------------------------------------------

struct deja_script_trace_reg : public deja_trace_reg
{
    DEJA_F_DECL         deja_script_trace_reg   ( const char*    pChannel, 
                                                  const char*    pFirstMsg,
                                                        int      Severity,
                                                  const char*    pFileName,
                                                        int      Line );

    DEJA_F_DECL         deja_script_trace_reg   ( const wchar_t* pChannel, 
                                                  const char*    pFirstMsg,
                                                        int      Severity,
                                                  const char*    pFileName,
                                                        int      Line );

    DEJA_F_DECL         deja_script_trace_reg   ( const char*    pChannel, 
                                                  const wchar_t* pFirstMsg,
                                                        int      Severity,
                                                  const char*    pFileName,
                                                        int      Line );

    DEJA_F_DECL         deja_script_trace_reg   ( const char*    pChannel, 
                                                  const char*    pFirstMsg,
                                                        int      Severity,
                                                  const wchar_t* pFileName,
                                                        int      Line );

    DEJA_F_DECL         deja_script_trace_reg   ( const char*    pChannel, 
                                                  const wchar_t* pFirstMsg,
                                                        int      Severity,
                                                  const wchar_t* pFileName,
                                                        int      Line );

    DEJA_F_DECL         deja_script_trace_reg   ( const wchar_t* pChannel, 
                                                  const char*    pFirstMsg,
                                                        int      Severity,
                                                  const wchar_t* pFileName,
                                                        int      Line );

    DEJA_F_DECL         deja_script_trace_reg   ( const wchar_t* pChannel, 
                                                  const wchar_t* pFirstMsg,
                                                        int      Severity,
                                                  const char*    pFileName,
                                                        int      Line );

    DEJA_F_DECL         deja_script_trace_reg   ( const wchar_t* pChannel, 
                                                  const wchar_t* pFirstMsg,
                                                        int      Severity,
                                                  const wchar_t* pFileName,
                                                        int      Line );

    DEJA_F_DECL        ~deja_script_trace_reg   ( void );

private:
    DEJA_F_DECL         deja_script_trace_reg   ( void );
};

//------------------------------------------------------------------------------

DEJA_F_DECL void        DejaTrace           ( deja_script_trace_reg* pScriptTraceReg,
                                              const char*            pMessage );

DEJA_F_DECL void        DejaTrace           ( deja_script_trace_reg* pScriptTraceReg,
                                              const wchar_t*         pMessage );

//------------------------------------------------------------------------------
//  Context - C++
//------------------------------------------------------------------------------

class deja_context
{
private:
                        deja_context        (       void ) { };

public:
    DEJA_F_DECL         deja_context        ( const void*    Address );
    DEJA_F_DECL        ~deja_context        (       void );        
};

//------------------------------------------------------------------------------

class deja_context_reg
{
private:
                        deja_context_reg    (       void ) { };

public:
    DEJA_F_DECL         deja_context_reg    ( const char*    pContext,
                                              const char*    pFileName,
                                                    int      Line );

    DEJA_F_DECL         deja_context_reg    ( const wchar_t* pContext,
                                              const char*    pFileName,
                                                    int      Line );

    DEJA_F_DECL         deja_context_reg    ( const char*    pContext,
                                              const wchar_t* pFileName,
                                                    int      Line );

    DEJA_F_DECL         deja_context_reg    ( const wchar_t* pContext,
                                              const wchar_t* pFileName,
                                                    int      Line );
};

//------------------------------------------------------------------------------
//  Context - Script
//------------------------------------------------------------------------------

class deja_script_context_reg
{
private:               
    int                 m_Magic;
                        deja_script_context_reg (       void ) { };
                   
public:                
    DEJA_F_DECL         deja_script_context_reg ( const char*    pContext,
                                                  const char*    pFileName,
                                                        int      Line );
                   
    DEJA_F_DECL         deja_script_context_reg ( const wchar_t* pContext,
                                                  const char*    pFileName,
                                                        int      Line );
                   
    DEJA_F_DECL         deja_script_context_reg ( const char*    pContext,
                                                  const wchar_t* pFileName,
                                                        int      Line );
                   
    DEJA_F_DECL         deja_script_context_reg ( const wchar_t* pContext,
                                                  const wchar_t* pFileName,
                                                        int      Line );
};

//------------------------------------------------------------------------------

DEJA_F_DECL void        DejaScriptContext   ( const deja_script_context_reg* );
DEJA_F_DECL void        DejaScriptContext   ( void );

//********************** PROMOTE TO DEJALIB.H WHEN READY ***********************
//********************** PROMOTE TO DEJALIB.H WHEN READY ***********************
//==============================================================================
//  3D
//==============================================================================
//
//  Insert appropriate comments here.
//  Seriously, dude.
//
//==============================================================================

struct deja_color
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void DEJA_3D_SETUP          ( void );   /* Parameters pending... */
void DEJA_3D_LAYER          ( const char* pLayerLabel );
void DEJA_3D_MATRIX         ( const float* pMatrixCells );
void DEJA_3D_COLOR          ( int R, int G, int B, int A=255 );

void DEJA_3D_DATA_POSITION  ( float* pPositionData, int Count, int Stride=16 );
void DEJA_3D_DATA_INDEX     ( int* pIndexData,      int Count, int Stride= 4 );
void DEJA_3D_DATA_COLOR     ( deja_color* pColor,   int Count, int Stride= 4 );

void DEJA_3D_EXEC_POINTS    ( void );
void DEJA_3D_EXEC_MARKERS   ( void );
void DEJA_3D_EXEC_LINE_LIST ( void );
void DEJA_3D_EXEC_LINE_STRIP( void );
void DEJA_3D_EXEC_TRI_LIST  ( void );
void DEJA_3D_EXEC_TRI_STRIP ( void );

void DEJA_3D_DRAW_MARKER    ( float X, float Y, float Z );
void DEJA_3D_DRAW_SPHERE    ( float X, float Y, float Z, float R );
void DEJA_3D_DRAW_LABEL     ( float X, float Y, float Z, const char* pLabel );
void DEJA_3D_DRAW_CAMERA    ( float* pCameraMatrixCells, 
                              float  Near, float Far, 
                              float  XFOV, float YFOV, int Index=0 );
//********************** PROMOTE TO DEJALIB.H WHEN READY *********************//
//********************** PROMOTE TO DEJALIB.H WHEN READY *********************//

//------------------------------------------------------------------------------
//  Static 3D
//------------------------------------------------------------------------------

DEJA_F_DECL void        Deja3DSetup         ( void );   /* Parameters pending... */
DEJA_F_DECL void        Deja3DLayer         ( const char* pLayerLabel );
DEJA_F_DECL void        Deja3DMatrix        ( const float* pMatrixCells );
DEJA_F_DECL void        Deja3DColor         ( int R, int G, int B, int A=255 );

DEJA_F_DECL void        Deja3DDataPosition  ( const float* pPositionData, int Count, int Stride=16 );
DEJA_F_DECL void        Deja3DDataColor     ( const deja_color* pColor,   int Count, int Stride= 4 );
DEJA_F_DECL void        Deja3DDataIndex     ( const int* pData,           int Count, int Stride= 4 );

DEJA_F_DECL void        Deja3DExecPoints    ( void );
DEJA_F_DECL void        Deja3DExecMarkers   ( void );
DEJA_F_DECL void        Deja3DExecLineList  ( void );
DEJA_F_DECL void        Deja3DExecLineStrip ( void );
DEJA_F_DECL void        Deja3DExecTriList   ( void );
DEJA_F_DECL void        Deja3DExecTriStrip  ( void );
                                            
DEJA_F_DECL void        Deja3DDrawMarker    ( float X, float Y, float Z );
DEJA_F_DECL void        Deja3DDrawSphere    ( float X, float Y, float Z, float R );
DEJA_F_DECL void        Deja3DDrawLabel     ( float X, float Y, float Z, const char* pLabel );
DEJA_F_DECL void        Deja3DDrawCamera    ( const float* pCameraMatrixCells, 
                                              float  Near, float Far, 
                                              float  XFOV, float YFOV, int Index=0 );

//------------------------------------------------------------------------------
//  Object Tracking
//------------------------------------------------------------------------------

#define DEJA_VLA_QUEUE_SIZE 100

typedef void (deja_d_fn)( const void* pV );

struct deja_type;

//------------------------------------------------------------------------------

DEJA_F_DECL int         DejaReserveTypeSeq  (       void );
DEJA_F_DECL void        DejaObjectInfo      ( const void*    pAddr, 
                                              const char*    pName );
DEJA_F_DECL void        DejaObjectInit      (       void );
DEJA_F_DECL void        DejaObjectKill      (       void );
DEJA_F_DECL void        DejaObjectLabel     ( const void*    pAddr, 
                                              const char*    pNLabelFormat, ... );
DEJA_F_DECL void        DejaObjectLabel     ( const void*    pAddr, 
                                              const wchar_t* pWLabelFormat, ... );
DEJA_F_DECL void        DejaTerminateType   (       void );
DEJA_F_DECL bool        DejaPostLogic       ( deja_type&     DT,
                                              const void*    pV );
DEJA_F_DECL void        DejaOpenPost        ( deja_type&     DT );
DEJA_F_DECL void        DejaOpenSubPost     ( deja_type&     DT );
DEJA_F_DECL void        DejaClosePost       (       void );
DEJA_F_DECL void        DejaCompoundField   (       void );
DEJA_F_DECL void        DejaVLAField        ( deja_d_fn*     pVLAFn, 
                                              const void*    pData );
DEJA_F_DECL void        DejaPostFlush       (       void );
DEJA_F_DECL int         DejaStrLen          ( const char*    pString );
DEJA_F_DECL int         DejaStrLen          ( const wchar_t* pString );

//------------------------------------------------------------------------------

DEJA_V_DECL extern         int     DejaArraySize;
DEJA_V_DECL extern         int     DejaPostPhase;
DEJA_V_DECL extern         int     DejaTypeSeq;
DEJA_V_DECL extern         int     DejaOwnerType;
DEJA_V_DECL extern         bool    DejaAtomicFlag;
DEJA_V_DECL extern const   void*   DejaFieldAddr;
DEJA_V_DECL extern const   char*   DejaFieldName;
DEJA_V_DECL extern const   char*   DejaEnumTypeName;
DEJA_V_DECL extern         bool    DejaIfPtrFlag;
DEJA_V_DECL extern         int     DejaVLAQCursor;
DEJA_V_DECL extern         int     DejaVLAQCount;
DEJA_V_DECL extern deja_d_fn*      DejaVLAFnPtr[ DEJA_VLA_QUEUE_SIZE ];
DEJA_V_DECL extern const   void*   DejaVLAData [ DEJA_VLA_QUEUE_SIZE ];
DEJA_V_DECL extern         int     DejaVLACount[ DEJA_VLA_QUEUE_SIZE ];

//------------------------------------------------------------------------------

struct deja_type
{
          deja_d_fn* pDescriptor;
    const char*      pTypeName;
          int        TypeSeq;
          int        TypeSize;
          bool       bDefined;
          void*      pNext;
    deja_type( deja_d_fn*  apDescriptor, int Size, const char* apTypeName )
    {
        pDescriptor = apDescriptor;
        pTypeName   = apTypeName;
        TypeSeq     = 0;            // FIELD_UNKNOWN = 0
        TypeSize    = Size;
        bDefined    = false;
        pNext       = NULL;
    }
};

//------------------------------------------------------------------------------

struct deja_owner
{
    int PreviousOwnerType;
    deja_type& DejaType;
    deja_owner( deja_type& DT )
    :   DejaType(DT)
    {
        PreviousOwnerType = DejaOwnerType;
        DejaOwnerType     = DT.TypeSeq;
    }
    ~deja_owner( void )
    {
        if( DejaPostPhase == 1 )
        {
            DejaTerminateType();
            DejaType.bDefined = true;
        }
        DejaOwnerType = PreviousOwnerType;
    }
private:
    deja_owner& operator = ( const deja_owner& ) { return(*this); };
};

//---------------------
#ifndef VARIADIC_MACROS
//------------------------------------------------------------------------------
//  Without variadic macros, we must use a template function to both accept an
//  arbitrary object and accept variable parameters.  This approach is 
//  functionally equivalent to the variadic macro form.  The only downside is
//  the need to employ templates which generate more code.  The following 
//  functions are used by the template functions.
//------------------------------------------------------------------------------

void DejaObjectLabel ( const void*    pAddr, 
                       const char*    pNLabelFormat, 
                       const wchar_t* pWLabelFormat,
                             va_list  args );

//-----------------------
#endif // VARIADIC_MACROS
//-----------------------

//------------------------------------------------------------------------------
//  We need to announce explicit versions of template functions DejaField and
//  DejaDescriptor for all atomic data types (char, int, float, etc).  

DEJA_F_DECL void DejaField( const          void**     ); 
DEJA_F_DECL void DejaField( const          bool&      ); 
DEJA_F_DECL void DejaField( const          float&     ); 
DEJA_F_DECL void DejaField( const          double&    ); 
DEJA_F_DECL void DejaField( const          char&      ); 
DEJA_F_DECL void DejaField( const   signed char&      ); 
DEJA_F_DECL void DejaField( const unsigned char&      ); 
DEJA_F_DECL void DejaField( const          wchar_t&   );
DEJA_F_DECL void DejaField( const   signed short&     ); 
DEJA_F_DECL void DejaField( const unsigned short&     ); 
DEJA_F_DECL void DejaField( const   signed int&       );
DEJA_F_DECL void DejaField( const unsigned int&       ); 
DEJA_F_DECL void DejaField( const   signed long&      );
DEJA_F_DECL void DejaField( const unsigned long&      ); 
DEJA_F_DECL void DejaField( const   signed long long& );
DEJA_F_DECL void DejaField( const unsigned long long& ); 

//------------------------------------------------------------------------------

DEJA_F_DECL void DejaDescriptor(          void* const&     ); 
DEJA_F_DECL void DejaDescriptor( const          void*      ); 
DEJA_F_DECL void DejaDescriptor( const          bool&      ); 
DEJA_F_DECL void DejaDescriptor( const          float&     ); 
DEJA_F_DECL void DejaDescriptor( const          double&    ); 
DEJA_F_DECL void DejaDescriptor( const          char&      ); 
DEJA_F_DECL void DejaDescriptor( const   signed char&      ); 
DEJA_F_DECL void DejaDescriptor( const unsigned char&      ); 
DEJA_F_DECL void DejaDescriptor( const          wchar_t&   );
DEJA_F_DECL void DejaDescriptor( const   signed short&     ); 
DEJA_F_DECL void DejaDescriptor( const unsigned short&     ); 
DEJA_F_DECL void DejaDescriptor( const   signed int&       );
DEJA_F_DECL void DejaDescriptor( const unsigned int&       ); 
DEJA_F_DECL void DejaDescriptor( const   signed long&      );
DEJA_F_DECL void DejaDescriptor( const unsigned long&      ); 
DEJA_F_DECL void DejaDescriptor( const   signed long long& );
DEJA_F_DECL void DejaDescriptor( const unsigned long long& ); 

//------------------------------------------------------------------------------
// This form catches the following:
//  - Arrays in DEJA_FIELD or DEJA_ENUM.

template< class T, int N >
void DejaField( const T(&V)[N] )
{
    DejaArraySize = N;
    DejaField( V[0] );
}

//------------------------------------------------------------------------------
// This form catches the following:
//  - Non-predefined types, including enums.
//  - Pointers to non-atomic types where T = "type*".

template< class T >
void DejaField( const T& V )
{
    DejaDescriptor( V );

    if( !DejaAtomicFlag )
    {
        if( DejaArraySize > 1 )
        {
            const T* pV = &V;
            for( int i = 1; i < DejaArraySize; i++ )
            {
                DejaFieldAddr = &pV[i];
                DejaDescriptor( pV[i] );
            }
        }

        DejaCompoundField();
    }

    // Safe to clear the Atomic flag on this execution path.
    DejaAtomicFlag = false;  
}

//------------------------------------------------------------------------------

template< class T >
void DejaVLAField( const T* pV )
{
    void DejaDescriptor( const T& );
    void (*pFn)( const T& ) = DejaDescriptor;
    DejaVLAField( (deja_d_fn*)pFn, (void*)pV );

    // Safe to clear the Atomic flag on this execution path.
    DejaAtomicFlag = false;
}

//------------------------------------------------------------------------------
//  Announce the explicit expansion for DejaVLAField<void>.
//------------------------------------------------------------------------------

DEJA_F_DECL void DejaVLAField( const void* pV );

//==============================================================================
//  MACROS
//==============================================================================

//==============================================================================
#ifndef DEJA_DISABLED
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Utility
//------------------------------------------------------------------------------

//  The compiler in Visual Studio 2003 does not properly compile constructs
//  using the token splice preprocessor directive (##) and the predefined line 
//  number macro (__LINE__).  This defect is present in SP1 as well.  The macro
//  __COUNTER__ can be used as a work-around as long as the generated symbol is
//  only needed once.  Also, the Edit and Continue features in Visual Studio
//  are "unhappy" when static variables are renamed, which can happen if the
//  __LINE__ macro is used to create unique names.  So, on Microsoft compilers
//  use the Microsoft specific macro __COUNTER__ and be certain not to need to
//  reference the generated unique name more than once.

#if defined( _MSC_VER )

#define DEJA_UNIQUE_NAME(a)     DEJA_SPLICE1( a, __COUNTER__ )
#define DEJA_SPLICE1(a,b)       DEJA_SPLICE2( a, b )
#define DEJA_SPLICE2(a,b)       a ## b

#else

#define DEJA_UNIQUE_NAME(a)     DEJA_SPLICE1( a, __LINE__ )
#define DEJA_SPLICE1(a,b)       DEJA_SPLICE2( a, b )
#define DEJA_SPLICE2(a,b)       a ## b

#endif

//  We stringize some of the macro parameters.  The preprocessor applies the
//  stringize operation before it performs other macro substitutions on macro
//  arguments.  Thus something like
//      DEJA_TRACE( MACRO("Channel"), MACRO("Format") );
//  would have problems even if MACRO(S) resolved into a literal string.  In
//  order to delay the stringize operation, we simply push it into its own
//  macro named DEJA_STRINGIZE.

#define DEJA_STRINGIZE( S )     #S

//------------------------------------------------------------------------------
//  General
//------------------------------------------------------------------------------

#define DEJA_FLUSH                  DejaFlush
#define DEJA_APP_LABEL              DejaAppLabel
#define DEJA_THREAD_LABEL           DejaThreadLabel
#define DEJA_GET_METRIC             DejaGetMetric
#define DEJA_SET_PARAMETER          DejaSetParameter
#define DEJA_SET_VALUE( N, V )      DejaSetValue( N, V, __FILE__, __LINE__ );
#define DEJA_NEXT_FRAME             DejaNextFrame
#define DEJA_SET_AUTO_FLUSH( F )    DejaSetAutoFlush( F, __FILE__, __LINE__ );
#define DEJA_LIB_CONNECTED          DejaLibConnected
#define DEJA_CONSOLE_WRITE          DejaConsoleWrite
#define DEJA_CONSOLE_ECHO           DejaConsoleEcho
#define DEJA_CONSOLE_READ           DejaConsoleRead
#define DEJA_LAST_ERROR             DejaGetLastError
#define DEJA_COMMAND                DejaCommand
#define DEJA_WII_READY              DejaWiiReady

#define DEJA_SET_MALLOC             DejaSetMallocFn
#define DEJA_SET_FREE               DejaSetFreeFn

#define DEJA_LOG_MALLOC             DejaLogMalloc
#define DEJA_LOG_REALLOC            DejaLogRealloc
#define DEJA_LOG_FREE               DejaLogFree
#define DEJA_LOG_NEW                DejaLogNew 
#define DEJA_LOG_DELETE             DejaLogDelete
#define DEJA_LOG_NEW_ARRAY          DejaLogNewArray
#define DEJA_LOG_DELETE_ARRAY       DejaLogDeleteArray

#define DEJA_ENUM_VALUE( T, V )     DejaEnumAlias( #T, V, #V, false )
#define DEJA_ENUM_ALIAS( T, V, A )  DejaEnumAlias( #T, V,  A, true  )

#define DEJA_SCOPE_OBJECT( O )                                                  \
    deja_object_scope DEJA_UNIQUE_NAME(ObjScope)( &O, __FILE__, __LINE__ );

#define DEJA_THREAD_INIT( )                                                     \
    if( !DejaSystemActive ) { } else                                            \
    {                                                                           \
        DejaLock( __FILE__, __LINE__ );                                         \
        DejaThreadInit();                                                       \
        DejaUnlock();                                                           \
    }

#define DEJA_THREAD_KILL( Code )                                                \
    if( !DejaSystemActive ) { } else                                            \
    {                                                                           \
        DejaLock( __FILE__, __LINE__ );                                         \
        DejaThreadKill( Code );                                                 \
        DejaUnlock();                                                           \
    }

#define DEJA_TERMINATE( )                                                       \
    if( !DejaSystemActive ) { } else                                            \
    {                                                                           \
        DejaLock( __FILE__, __LINE__ );                                         \
        DejaTerminate();                                                        \
        DejaUnlock();                                                           \
    }

//  The DEJA_SET_REALLOC function is being retired.  To prevent problems with
//  existing code, we will go ahead and provide a dead definition.
#define DEJA_SET_REALLOC( RFN )

//------------------------------------------------------------------------------
//  Logging - C++
//------------------------------------------------------------------------------

//================
#if !(defined( TARGET_WII ))
//----------------

#ifndef VARIADIC_MACROS
#error Support for variadic macros is required on this platform.
#endif

#define DEJA_TRACE( Channel, Format, ... )                                      \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 0, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceReg( 0, __FILE__, __LINE__,                            \
                              DEJA_STRINGIZE(Channel),                          \
                              DEJA_STRINGIZE(Format),                           \
                              Channel, Format );                                \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, Format, ## __VA_ARGS__ );                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_WARNING( Channel, Format, ... )                                    \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 0, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceReg( 2, __FILE__, __LINE__,                            \
                              DEJA_STRINGIZE(Channel),                          \
                              DEJA_STRINGIZE(Format),                           \
                              Channel, Format );                                \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, Format, ## __VA_ARGS__ );                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_ERROR( Channel, Format, ... )                                      \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 0, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceReg( 4, __FILE__, __LINE__,                            \
                              DEJA_STRINGIZE(Channel),                          \
                              DEJA_STRINGIZE(Format),                           \
                              Channel, Format );                                \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, Format, ## __VA_ARGS__ );                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_BOOKMARK( Channel, Format, ... )                                   \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 0, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceReg( 8, __FILE__, __LINE__,                            \
                              DEJA_STRINGIZE(Channel),                          \
                              DEJA_STRINGIZE(Format),                           \
                              Channel, Format );                                \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, Format, ## __VA_ARGS__ );                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_TRACE_OFF( Channel, Format, ... )                                  \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 1, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceReg( 0, __FILE__, __LINE__,                            \
                              DEJA_STRINGIZE(Channel),                          \
                              DEJA_STRINGIZE(Format),                           \
                              Channel, Format );                                \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, Format, ## __VA_ARGS__ );                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_WARNING_OFF( Channel, Format, ... )                                \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 1, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceReg( 2, __FILE__, __LINE__,                            \
                              DEJA_STRINGIZE(Channel),                          \
                              DEJA_STRINGIZE(Format),                           \
                              Channel, Format );                                \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, Format, ## __VA_ARGS__ );                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_ERROR_OFF( Channel, Format, ... )                                  \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 1, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceReg( 4, __FILE__, __LINE__,                            \
                              DEJA_STRINGIZE(Channel),                          \
                              DEJA_STRINGIZE(Format),                           \
                              Channel, Format );                                \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, Format, ## __VA_ARGS__ );                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_BOOKMARK_OFF( Channel, Format, ... )                               \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 1, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceReg( 8, __FILE__, __LINE__,                            \
                              DEJA_STRINGIZE(Channel),                          \
                              DEJA_STRINGIZE(Format),                           \
                              Channel, Format );                                \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, Format, ## __VA_ARGS__ );                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

//------------------
#endif // if not WII
//==================

//===============
#if defined( TARGET_WII )
//---------------

inline void DejaTraceRegFn      ( deja_trace_reg& DejaTraceReg,
                                        int       TraceCode,
                                  const char*     pFileName,
                                        int       Line,
                                  const char*     pStringizedChannel,
                                  const char*     pNChannel,
                                  const char*     pNFormat, ... )
{
    DejaTraceReg( TraceCode, pFileName, Line, pStringizedChannel, NULL, pNChannel, pNFormat );
}

inline void DejaTraceRegFn      ( deja_trace_reg& DejaTraceReg,
                                        int       TraceCode,
                                  const char*     pFileName,
                                        int       Line,
                                  const char*     pStringizedChannel,
                                  const wchar_t*  pWChannel,
                                  const char*     pNFormat, ... )
{
    DejaTraceReg( TraceCode, pFileName, Line, pStringizedChannel, NULL, pWChannel, pNFormat );
}

inline void DejaTraceRegFn      ( deja_trace_reg& DejaTraceReg,
                                        int       TraceCode,
                                  const char*     pFileName,
                                        int       Line,
                                  const char*     pStringizedChannel,
                                  const char*     pNChannel,
                                  const wchar_t*  pWFormat, ... )
{
    DejaTraceReg( TraceCode, pFileName, Line, pStringizedChannel, NULL, pNChannel, pWFormat );
}

inline void DejaTraceRegFn      ( deja_trace_reg& DejaTraceReg,
                                        int       TraceCode,
                                  const char*     pFileName,
                                        int       Line,
                                  const char*     pStringizedChannel,
                                  const wchar_t*  pWChannel,
                                  const wchar_t*  pWFormat, ... )
{
    DejaTraceReg( TraceCode, pFileName, Line, pStringizedChannel, NULL, pWChannel, pWFormat );
}

#define DEJA_TRACE( Channel, ... )                                              \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 0, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceRegFn( DejaTraceReg,                                   \
                                0, __FILE__, __LINE__,                          \
                                DEJA_STRINGIZE(Channel),                        \
                                Channel, __VA_ARGS__ );                         \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, __VA_ARGS__ );                           \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_WARNING( Channel, ... )                                            \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 0, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceRegFn( DejaTraceReg,                                   \
                                2, __FILE__, __LINE__,                          \
                                DEJA_STRINGIZE(Channel),                        \
                                Channel, __VA_ARGS__ );                         \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, __VA_ARGS__ );                           \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_ERROR( Channel, ... )                                              \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 0, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceRegFn( DejaTraceReg,                                   \
                                4, __FILE__, __LINE__,                          \
                                DEJA_STRINGIZE(Channel),                        \
                                Channel, __VA_ARGS__ );                         \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, __VA_ARGS__ );                           \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_BOOKMARK( Channel, ... )                                           \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 0, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceRegFn( DejaTraceReg,                                   \
                                8, __FILE__, __LINE__,                          \
                                DEJA_STRINGIZE(Channel),                        \
                                Channel, __VA_ARGS__ );                         \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, __VA_ARGS__ );                           \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_TRACE_OFF( Channel, ... )                                          \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 1, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceRegFn( DejaTraceReg,                                   \
                                0, __FILE__, __LINE__,                          \
                                DEJA_STRINGIZE(Channel),                        \
                                Channel, __VA_ARGS__ );                         \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, __VA_ARGS__ );                           \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_WARNING_OFF( Channel, ... )                                        \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 1, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceRegFn( DejaTraceReg,                                   \
                                2, __FILE__, __LINE__,                          \
                                DEJA_STRINGIZE(Channel),                        \
                                Channel, __VA_ARGS__ );                         \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, __VA_ARGS__ );                           \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_ERROR_OFF( Channel, ... )                                          \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 1, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceRegFn( DejaTraceReg,                                   \
                                4, __FILE__, __LINE__,                          \
                                DEJA_STRINGIZE(Channel),                        \
                                Channel, __VA_ARGS__ );                         \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, __VA_ARGS__ );                           \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

#define DEJA_BOOKMARK_OFF( Channel, ... )                                       \
                                                                                \
        if( DejaSystemActive )                                                  \
        {                                                                       \
            static deja_trace_reg DejaTraceReg = { 1, NULL };                   \
            if( !(DejaTraceReg.Data & 0x0100) )                                 \
            {                                                                   \
                DejaTraceRegFn( DejaTraceReg,                                   \
                                8, __FILE__, __LINE__,                          \
                                DEJA_STRINGIZE(Channel),                        \
                                Channel, __VA_ARGS__ );                         \
            }                                                                   \
            if( !(DejaTraceReg.Data & 0x0200) )                                 \
            {                                                                   \
                DejaTraceReg( Channel, __VA_ARGS__ );                           \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
        }

//--------------
#endif // if WII
//==============

//------------------------------------------------------------------------------
//  Logging - Script
//------------------------------------------------------------------------------

#define DEJA_SCRIPT_TRACE( pTraceReg, pMessage )                                \
    DejaTrace( pTraceReg, pMessage )

//------------------------------------------------------------------------------
//  Context
//------------------------------------------------------------------------------

#define DEJA_CONTEXT_REGISTRATION( C )                                          \
    static deja_context_reg DejaContextReg( DEJA_STRINGIZE( C ),                \
                                            __FILE__, __LINE__ );

#define DEJA_CONTEXT( ContextLabel )                                            \
    DEJA_CONTEXT_REGISTRATION( ContextLabel )                                   \
    deja_context DejaContext( &DejaContextReg )

#define DEJA_SCRIPT_CONTEXT DejaScriptContext

//------------------------------------------------------------------------------
//  Static 3D
//------------------------------------------------------------------------------
       
#define DEJA_3D_PREFIX          if( !DejaSystemActive ) { } else DejaLock( __FILE__, __LINE__ ) 

#define DEJA_3D_SETUP           DEJA_3D_PREFIX, Deja3DSetup 
#define DEJA_3D_LAYER           DEJA_3D_PREFIX, Deja3DLayer
#define DEJA_3D_MATRIX          DEJA_3D_PREFIX, Deja3DMatrix
#define DEJA_3D_COLOR           DEJA_3D_PREFIX, Deja3DColor
#define DEJA_3D_DATA_INDEX      DEJA_3D_PREFIX, Deja3DDataIndex
#define DEJA_3D_DATA_POSITION   DEJA_3D_PREFIX, Deja3DDataPosition
#define DEJA_3D_DATA_COLOR      DEJA_3D_PREFIX, Deja3DDataColor
#define DEJA_3D_EXEC_POINTS     DEJA_3D_PREFIX, Deja3DExecPoints
#define DEJA_3D_EXEC_MARKERS    DEJA_3D_PREFIX, Deja3DExecMarkers
#define DEJA_3D_EXEC_LINE_LIST  DEJA_3D_PREFIX, Deja3DExecLineList
#define DEJA_3D_EXEC_LINE_STRIP DEJA_3D_PREFIX, Deja3DExecLineStrip
#define DEJA_3D_EXEC_TRI_LIST   DEJA_3D_PREFIX, Deja3DExecTriList
#define DEJA_3D_EXEC_TRI_STRIP  DEJA_3D_PREFIX, Deja3DExecTriStrip
#define DEJA_3D_DRAW_MARKER     DEJA_3D_PREFIX, Deja3DDrawMarker
#define DEJA_3D_DRAW_SPHERE     DEJA_3D_PREFIX, Deja3DDrawSphere
#define DEJA_3D_DRAW_LABEL      DEJA_3D_PREFIX, Deja3DDrawLabel
#define DEJA_3D_DRAW_CAMERA     DEJA_3D_PREFIX, Deja3DDrawCamera

//------------------------------------------------------------------------------
//  Object Tracking
//------------------------------------------------------------------------------

#define DEJA_OBJECT_INIT( Object )                                             \
    if( !DejaSystemActive ) { } else                                           \
    {                                                                          \
        DejaLock( __FILE__, __LINE__ );                                        \
        DejaPostPhase = 2;                                                     \
        DejaObjectInfo( &Object, #Object );                                    \
        DejaDescriptor( Object );                                              \
        DejaObjectInit();                                                      \
        DejaUnlock();                                                          \
    }

//------------------------------------------------------------------------------

#define DEJA_OBJECT_KILL( Object )                                              \
    if( !DejaSystemActive ) { } else                                            \
    {                                                                           \
        DejaLock( __FILE__, __LINE__ );                                         \
        DejaObjectInfo( &Object, #Object );                                     \
        DejaObjectKill();                                                       \
        DejaUnlock();                                                           \
    }

//------------------------------------------------------------------------------
#ifdef VARIADIC_MACROS
//--------------------

#define DEJA_OBJECT_LABEL( Object, Label, ... )                                 \
    if( !DejaSystemActive ) { } else                                            \
    {                                                                           \
        DejaLock( __FILE__, __LINE__ );                                         \
        DejaObjectLabel( &Object, Label , ## __VA_ARGS__ );                     \
        DejaUnlock();                                                           \
    }

//----------------------
#else // VARIADIC_MACROS
//------------------------------------------------------------------------------
//  Without variadic macros, we must use a template function to both accept an
//  arbitrary object and accept variable parameters.  This approach is 
//  functionally equivalent to the variadic macro form.  The only downside is
//  the need to employ templates which generate more code.
//------------------------------------------------------------------------------

#define DEJA_OBJECT_LABEL                                                       \
    DejaLock( __FILE__, __LINE__ ), DejaObjectLabelT

template< class T >
void DejaObjectLabelT( const T& Object, const char* pNLabelFormat, ... )
{
    va_list args;
    va_start( args, pNLabelFormat );
    DejaObjectLabel( (void*)&Object, pNLabelFormat, (const wchar_t*)NULL, args );
    DejaUnlock();
}

template< class T >
void DejaObjectLabelT( const T& Object, const wchar_t* pWLabelFormat, ... )
{
    va_list args;
    va_start( args, pWLabelFormat );
    DejaObjectLabel( (void*)&Object, NULL, pWLabelFormat, args );
    DejaUnlock();
}

//-----------------------
#endif // VARIADIC_MACROS
//------------------------------------------------------------------------------

#define DEJA_POST( Object )                                                     \
    if( !DejaSystemActive ) { } else                                            \
    {                                                                           \
        DejaLock( __FILE__, __LINE__ );                                         \
        DejaPostPhase  = 3;                                                     \
        DejaObjectInfo( &Object, #Object );                                     \
        DejaDescriptor( Object );                                               \
        DejaAtomicFlag = false;  /* Atomic posts can't clear this. */           \
        DejaArraySize  = 1;      /* Atomic posts can't clear this. */           \
        DejaPostFlush();                                                        \
        DejaAutoFlush();                                                        \
        DejaUnlock();                                                           \
    }

//------------------------------------------------------------------------------

#define DEJA_TYPE( Object, Type )                                               \
    if( !DejaSystemActive )                                                     \
    {                                                                           \
       return;                                                                  \
    }                                                                           \
    const int DEJA_TYPE_must_be_first_Deja_macro_in_DejaDescriptor = 0;         \
    void (*pDescriptor)( const Type& ) = DejaDescriptor;                        \
    static deja_type DejaType( (deja_d_fn*)pDescriptor, sizeof(Type), #Type );  \
    if( !DejaPostLogic( DejaType, (void*)&Object ) )                            \
        return;                                                                 \
    DejaType.pNext = (void*)( ((Type*)(&Object)) + 1 );                         \
    deja_owner Owner( DejaType );                                               \
    DejaType.TypeSeq += DEJA_TYPE_must_be_first_Deja_macro_in_DejaDescriptor;

//------------------------------------------------------------------------------

#define DEJA_BASE( Object, BaseType )                                           \
    {                                                                           \
        const void* pOldFieldAddr    = DejaFieldAddr;                           \
        const char* pOldFieldName    = DejaFieldName;                           \
        const char* pOldEnumTypeName = DejaEnumTypeName;                        \
        int         OldArraySize     = DejaArraySize;                           \
        int         OldTypeSeq       = DejaTypeSeq;                             \
        bool        OldAtomicFlag    = DejaAtomicFlag;                          \
                                                                                \
        DejaFieldAddr    = (void*)( (BaseType*)(&(Object)) );                   \
        DejaFieldName    = #BaseType;                                           \
        DejaArraySize    = 1;                                                   \
        DejaEnumTypeName = NULL;                                                \
                                                                                \
        DejaField( *(BaseType*)(&(Object)) );                                   \
                                                                                \
        DejaFieldAddr    = pOldFieldAddr;                                       \
        DejaFieldName    = pOldFieldName;                                       \
        DejaEnumTypeName = pOldEnumTypeName;                                    \
        DejaArraySize    = OldArraySize;                                        \
        DejaTypeSeq      = OldTypeSeq;                                          \
        DejaAtomicFlag   = OldAtomicFlag;                                       \
                                                                                \
        DejaTypeSeq += DEJA_TYPE_must_be_first_Deja_macro_in_DejaDescriptor;    \
    }

//------------------------------------------------------------------------------

#define DEJA_FIELD( Field )                                                     \
    {                                                                           \
        const void* pOldFieldAddr    = DejaFieldAddr;                           \
        const char* pOldFieldName    = DejaFieldName;                           \
        const char* pOldEnumTypeName = DejaEnumTypeName;                        \
        int         OldArraySize     = DejaArraySize;                           \
        int         OldTypeSeq       = DejaTypeSeq;                             \
        bool        OldAtomicFlag    = DejaAtomicFlag;                          \
                                                                                \
        DejaFieldAddr    = &(Field);                                            \
        DejaFieldName    = #Field;                                              \
        DejaArraySize    = 1;                                                   \
        DejaEnumTypeName = NULL;                                                \
                                                                                \
        DejaField( Field );                                                     \
                                                                                \
        DejaFieldAddr    = pOldFieldAddr;                                       \
        DejaFieldName    = pOldFieldName;                                       \
        DejaEnumTypeName = pOldEnumTypeName;                                    \
        DejaArraySize    = OldArraySize;                                        \
        DejaTypeSeq      = OldTypeSeq;                                          \
        DejaAtomicFlag   = OldAtomicFlag;                                       \
                                                                                \
        DejaTypeSeq += DEJA_TYPE_must_be_first_Deja_macro_in_DejaDescriptor;    \
    }

//------------------------------------------------------------------------------

#define DEJA_ENUM( Field, EnumType )                                            \
    {                                                                           \
        const void* pOldFieldAddr    = DejaFieldAddr;                           \
        const char* pOldFieldName    = DejaFieldName;                           \
        const char* pOldEnumTypeName = DejaEnumTypeName;                        \
        int         OldArraySize     = DejaArraySize;                           \
        int         OldTypeSeq       = DejaTypeSeq;                             \
        bool        OldAtomicFlag    = DejaAtomicFlag;                          \
                                                                                \
        DejaFieldAddr    = &(Field);                                            \
        DejaFieldName    = #Field;                                              \
        DejaArraySize    = 1;                                                   \
        DejaEnumTypeName = #EnumType;                                           \
                                                                                \
        DejaField( Field );                                                     \
                                                                                \
        DejaFieldAddr    = pOldFieldAddr;                                       \
        DejaFieldName    = pOldFieldName;                                       \
        DejaEnumTypeName = pOldEnumTypeName;                                    \
        DejaArraySize    = OldArraySize;                                        \
        DejaTypeSeq      = OldTypeSeq;                                          \
        DejaAtomicFlag   = OldAtomicFlag;                                       \
                                                                                \
        DejaTypeSeq += DEJA_TYPE_must_be_first_Deja_macro_in_DejaDescriptor;    \
    }

//------------------------------------------------------------------------------

#define DEJA_VLARRAY( Pointer, Count )                                          \
    {                                                                           \
        const void* pOldFieldAddr    = DejaFieldAddr;                           \
        const char* pOldFieldName    = DejaFieldName;                           \
        const char* pOldEnumTypeName = DejaEnumTypeName;                        \
        int         OldArraySize     = DejaArraySize;                           \
        int         OldTypeSeq       = DejaTypeSeq;                             \
        bool        OldAtomicFlag    = DejaAtomicFlag;                          \
        bool        OldIfPtrFlag     = DejaIfPtrFlag;                           \
                                                                                \
        DejaFieldAddr    = Pointer;                                             \
        DejaFieldName    = #Pointer;                                            \
        DejaArraySize    = Count;                                               \
        DejaEnumTypeName = NULL;                                                \
        DejaIfPtrFlag    = false;                                               \
                                                                                \
        DejaVLAField( Pointer );                                                \
                                                                                \
        DejaFieldAddr    = pOldFieldAddr;                                       \
        DejaFieldName    = pOldFieldName;                                       \
        DejaEnumTypeName = pOldEnumTypeName;                                    \
        DejaArraySize    = OldArraySize;                                        \
        DejaTypeSeq      = OldTypeSeq;                                          \
        DejaAtomicFlag   = OldAtomicFlag;                                       \
        DejaIfPtrFlag    = OldIfPtrFlag;                                        \
                                                                                \
        DejaTypeSeq += DEJA_TYPE_must_be_first_Deja_macro_in_DejaDescriptor;    \
    }

//------------------------------------------------------------------------------

#define DEJA_IF_PTR( Pointer )                                                  \
    {                                                                           \
        const void* pOldFieldAddr    = DejaFieldAddr;                           \
        const char* pOldFieldName    = DejaFieldName;                           \
        const char* pOldEnumTypeName = DejaEnumTypeName;                        \
        int         OldArraySize     = DejaArraySize;                           \
        int         OldTypeSeq       = DejaTypeSeq;                             \
        bool        OldAtomicFlag    = DejaAtomicFlag;                          \
        bool        OldIfPtrFlag     = DejaIfPtrFlag;                           \
                                                                                \
        DejaFieldAddr    = Pointer;                                             \
        DejaFieldName    = #Pointer;                                            \
        DejaArraySize    = (Pointer ? 1 : 0);                                   \
        DejaEnumTypeName = NULL;                                                \
        DejaIfPtrFlag    = true;                                                \
                                                                                \
        DejaVLAField( Pointer );                                                \
                                                                                \
        DejaFieldAddr    = pOldFieldAddr;                                       \
        DejaFieldName    = pOldFieldName;                                       \
        DejaEnumTypeName = pOldEnumTypeName;                                    \
        DejaArraySize    = OldArraySize;                                        \
        DejaTypeSeq      = OldTypeSeq;                                          \
        DejaAtomicFlag   = OldAtomicFlag;                                       \
        DejaIfPtrFlag    = OldIfPtrFlag;                                        \
                                                                                \
        DejaTypeSeq += DEJA_TYPE_must_be_first_Deja_macro_in_DejaDescriptor;    \
    }

//------------------------------------------------------------------------------

#define DEJA_CSTRING( pString ) DEJA_VLARRAY( pString, DejaStrLen( pString ) );

//------------------------------------------------------------------------------
// The DEJA_ARRAY command is deprecated.  Use DEJA_FIELD going forward.  The 
// following definition of DEJA_ARRAY provides backwards compatibility.  It will
// be removed in a near future version.

#define DEJA_ARRAY( StaticArray ) DEJA_FIELD( StaticArray )

//------------------------------------------------------------------------------
#else // DEJA_DISABLED
//------------------------------------------------------------------------------

    //==========================================================================
    #ifdef VARIADIC_MACROS
    //==========================================================================

    #if defined( TARGET_PS3 ) || defined( TARGET_PS4 ) || defined( TARGET_WII ) || defined( TARGET_CAFE ) || defined( TARGET_OSX ) || defined( TARGET_LINUX ) || defined( TARGET_3DS ) || defined( TARGET_NX )
        #define DEJA_NULL_FN  ((void)0)
    #else
        #define DEJA_NULL_FN  __noop
    #endif

    //--------------------------------------------------------------------------
    //  General
    //--------------------------------------------------------------------------

    inline bool DejaLibFalse( void ) { return false; }

    #define DEJA_FLUSH( ... )               DEJA_NULL_FN
    #define DEJA_APP_LABEL( ... )           DEJA_NULL_FN
    #define DEJA_THREAD_LABEL( ... )        DEJA_NULL_FN
    #define DEJA_GET_METRIC( ... )          -1
    #define DEJA_SET_PARAMETER( ... )       DEJA_NULL_FN
    #define DEJA_SET_VALUE( ... )           DEJA_NULL_FN
    #define DEJA_NEXT_FRAME( ... )          DEJA_NULL_FN
    #define DEJA_SET_AUTO_FLUSH( ... )      DEJA_NULL_FN
    #define DEJA_LIB_CONNECTED( )           DejaLibFalse()
    #define DEJA_LAST_ERROR( )              ""
    #define DEJA_WII_READY( )               DEJA_NULL_FN
    #define DEJA_CONSOLE_WRITE( ... )       DEJA_NULL_FN
    #define DEJA_CONSOLE_ECHO( ... )        DEJA_NULL_FN
    #define DEJA_CONSOLE_READ( B, S )       (B[0] = '\0')
    #define DEJA_COMMAND( ... )             DEJA_NULL_FN

    #define DEJA_SET_MALLOC( ... )          DEJA_NULL_FN
    #define DEJA_SET_REALLOC( ... )         DEJA_NULL_FN
    #define DEJA_SET_FREE( ... )            DEJA_NULL_FN

    #define DEJA_LOG_MALLOC( ... )          DEJA_NULL_FN
    #define DEJA_LOG_REALLOC( ... )         DEJA_NULL_FN
    #define DEJA_LOG_FREE( ... )            DEJA_NULL_FN
    #define DEJA_LOG_NEW( ... )             DEJA_NULL_FN
    #define DEJA_LOG_DELETE( ... )          DEJA_NULL_FN
    #define DEJA_LOG_NEW_ARRAY( ... )       DEJA_NULL_FN
    #define DEJA_LOG_DELETE_ARRAY( ... )    DEJA_NULL_FN

    #define DEJA_ENUM_VALUE( ... )          DEJA_NULL_FN
    #define DEJA_ENUM_ALIAS( ... )          DEJA_NULL_FN

    #define DEJA_SCOPE_OBJECT( ... )        DEJA_NULL_FN

    #define DEJA_THREAD_INIT( ... )         DEJA_NULL_FN
    #define DEJA_THREAD_KILL( ... )         DEJA_NULL_FN
    #define DEJA_THREAD_KILL( ... )         DEJA_NULL_FN
    #define DEJA_TERMINATE( ... )           DEJA_NULL_FN

    //--------------------------------------------------------------------------
    //  Logging
    //--------------------------------------------------------------------------

    #define DEJA_TRACE(        Channel, Format, ... )  DEJA_NULL_FN
    #define DEJA_WARNING(      Channel, Format, ... )  DEJA_NULL_FN
    #define DEJA_ERROR(        Channel, Format, ... )  DEJA_NULL_FN
    #define DEJA_BOOKMARK(     Channel, Format, ... )  DEJA_NULL_FN
    #define DEJA_TRACE_OFF(    Channel, Format, ... )  DEJA_NULL_FN
    #define DEJA_WARNING_OFF(  Channel, Format, ... )  DEJA_NULL_FN
    #define DEJA_ERROR_OFF(    Channel, Format, ... )  DEJA_NULL_FN
    #define DEJA_BOOKMARK_OFF( Channel, Format, ... )  DEJA_NULL_FN

    //--------------------------------------------------------------------------
    //  Static 3D
    //--------------------------------------------------------------------------

    #define DEJA_3D_SETUP( ... )            DEJA_NULL_FN
    #define DEJA_3D_LAYER( ... )            DEJA_NULL_FN
    #define DEJA_3D_MATRIX( ... )           DEJA_NULL_FN
    #define DEJA_3D_COLOR( ... )            DEJA_NULL_FN
    #define DEJA_3D_DATA_INDEX( ... )       DEJA_NULL_FN
    #define DEJA_3D_DATA_POSITION( ... )    DEJA_NULL_FN
    #define DEJA_3D_DATA_COLOR( ... )       DEJA_NULL_FN
    #define DEJA_3D_EXEC_POINTS( ... )      DEJA_NULL_FN
    #define DEJA_3D_EXEC_MARKERS( ... )     DEJA_NULL_FN
    #define DEJA_3D_EXEC_LINE_LIST( ... )   DEJA_NULL_FN
    #define DEJA_3D_EXEC_LINE_STRIP( ... )  DEJA_NULL_FN 
    #define DEJA_3D_EXEC_TRI_LIST( ... )    DEJA_NULL_FN
    #define DEJA_3D_EXEC_TRI_STRIP( ... )   DEJA_NULL_FN
    #define DEJA_3D_DRAW_MARKER( ... )      DEJA_NULL_FN
    #define DEJA_3D_DRAW_SPHERE( ... )      DEJA_NULL_FN
    #define DEJA_3D_DRAW_LABEL( ... )       DEJA_NULL_FN
    #define DEJA_3D_DRAW_CAMERA( ... )      DEJA_NULL_FN

    //--------------------------------------------------------------------------
    //  Context
    //--------------------------------------------------------------------------

    #define DEJA_CONTEXT( ... )         DEJA_NULL_FN
    #define DEJA_SCRIPT_CONTEXT( ... )  DEJA_NULL_FN       

    //--------------------------------------------------------------------------
    //  Object Tracking
    //--------------------------------------------------------------------------

    #define DEJA_OBJECT_INIT( ... )     DEJA_NULL_FN
    #define DEJA_OBJECT_KILL( ... )     DEJA_NULL_FN
    #define DEJA_OBJECT_LABEL( ... )    DEJA_NULL_FN
    #define DEJA_POST( ... )            DEJA_NULL_FN
    #define DEJA_TYPE( I, T )           (void)I
    #define DEJA_BASE( ... )            DEJA_NULL_FN
    #define DEJA_FIELD( ... )           DEJA_NULL_FN
    #define DEJA_ENUM( ... )            DEJA_NULL_FN
    #define DEJA_ARRAY( ... )           DEJA_NULL_FN
    #define DEJA_VLARRAY( ... )         DEJA_NULL_FN
    #define DEJA_CSTRING( ... )         DEJA_NULL_FN
    #define DEJA_IF_PTR( ... )          DEJA_NULL_FN

    //==========================================================================
    #else // #ifdef VARIADIC_MACROS
    //==========================================================================

    #if defined( TARGET_PS3 ) || defined( TARGET_CAFE )
        #define DEJA_NULL_FN  ((void)0)
    #else
        #define DEJA_NULL_FN  __noop
    #endif

    //--------------------------------------------------------------------------
    //  General
    //--------------------------------------------------------------------------

    #define DEJA_FLUSH                  DEJA_NULL_FN
    #define DEJA_APP_LABEL              DEJA_NULL_FN
    #define DEJA_THREAD_LABEL           DEJA_NULL_FN
    #define DEJA_GET_METRIC( a )        -1
    #define DEJA_SET_PARAMETER( a, b )
    #define DEJA_SET_VALUE              DEJA_NULL_FN
    #define DEJA_NEXT_FRAME( )
    #define DEJA_SET_AUTO_FLUSH         DEJA_NULL_FN
    #define DEJA_LIB_CONNECTED( )       false
    #define DEJA_LAST_ERROR( )          ""
    #define DEJA_WII_READY              DEJA_NULL_FN
    #define DEJA_CONSOLE_WRITE          DEJA_NULL_FN
    #define DEJA_CONSOLE_ECHO           DEJA_NULL_FN
    #define DEJA_CONSOLE_READ( B, S )   (B[0] = '\0')    
    #define DEJA_COMMAND                DEJA_NULL_FN

    #define DEJA_SET_MALLOC             DEJA_NULL_FN
    #define DEJA_SET_REALLOC            DEJA_NULL_FN
    #define DEJA_SET_FREE               DEJA_NULL_FN

    #define DEJA_LOG_MALLOC             DEJA_NULL_FN
    #define DEJA_LOG_REALLOC            DEJA_NULL_FN
    #define DEJA_LOG_FREE               DEJA_NULL_FN
    #define DEJA_LOG_NEW                DEJA_NULL_FN
    #define DEJA_LOG_DELETE             DEJA_NULL_FN
    #define DEJA_LOG_NEW_ARRAY          DEJA_NULL_FN
    #define DEJA_LOG_DELETE_ARRAY       DEJA_NULL_FN

    #define DEJA_ENUM_VALUE( T, V )     
    #define DEJA_ENUM_ALIAS( T, V, A )  

    #define DEJA_SCOPE_OBJECT( O )    

    #define DEJA_THREAD_INIT( )
    #define DEJA_THREAD_KILL( Code )
    #define DEJA_TERMINATE( )

    //--------------------------------------------------------------------------
    //  Logging
    //--------------------------------------------------------------------------

    #define DEJA_TRACE                  DEJA_NULL_FN
    #define DEJA_WARNING                DEJA_NULL_FN
    #define DEJA_ERROR                  DEJA_NULL_FN
    #define DEJA_BOOKMARK               DEJA_NULL_FN
    #define DEJA_TRACE_OFF              DEJA_NULL_FN
    #define DEJA_WARNING_OFF            DEJA_NULL_FN
    #define DEJA_ERROR_OFF              DEJA_NULL_FN
    #define DEJA_BOOKMARK_OFF           DEJA_NULL_FN

    //--------------------------------------------------------------------------
    //  Static 3D
    //--------------------------------------------------------------------------
                                
    #define DEJA_3D_LAYER               DEJA_NULL_FN
    #define DEJA_3D_COLOR               DEJA_NULL_FN
    #define DEJA_3D_MATRIX              DEJA_NULL_FN
    #define DEJA_3D_POINT               DEJA_NULL_FN
    #define DEJA_3D_POINT_LIST          DEJA_NULL_FN
    #define DEJA_3D_LINE                DEJA_NULL_FN
    #define DEJA_3D_LINE_LIST           DEJA_NULL_FN
    #define DEJA_3D_LINE_STRIP          DEJA_NULL_FN
    #define DEJA_3D_TRI                 DEJA_NULL_FN
    #define DEJA_3D_TRI_LIST            DEJA_NULL_FN
    #define DEJA_3D_TRI_STRIP           DEJA_NULL_FN
    #define DEJA_3D_MARKER              DEJA_NULL_FN
    #define DEJA_3D_SPHERE              DEJA_NULL_FN
    #define DEJA_3D_BOX_LOCAL           DEJA_NULL_FN
    #define DEJA_3D_BOX                 DEJA_NULL_FN

    //--------------------------------------------------------------------------
    //  Context
    //--------------------------------------------------------------------------

    #define DEJA_CONTEXT( C )           
    #define DEJA_SCRIPT_CONTEXT         DEJA_NULL_FN    

    //--------------------------------------------------------------------------
    //  Object Tracking
    //--------------------------------------------------------------------------

    #define DEJA_OBJECT_INIT( E )
    #define DEJA_OBJECT_KILL( E )
    #define DEJA_OBJECT_LABEL           DEJA_NULL_FN
    #define DEJA_POST( V )
    #define DEJA_TYPE( I, T )           (void)I
    #define DEJA_BASE( I, T )
    #define DEJA_FIELD( F )  
    #define DEJA_ENUM( T, F )
    #define DEJA_ARRAY( A )  
    #define DEJA_VLARRAY( A, C )
    #define DEJA_CSTRING( S )
    #define DEJA_IF_PTR( P )

    //==========================================================================
    #endif // #ifdef VARIADIC_MACROS
    //==========================================================================

//------------------------------------------------------------------------------
#endif // #ifndef DEJA_DISABLED
//==============================================================================

//==============================================================================
#endif // #ifndef DEJA_PRIVATE_H
//==============================================================================

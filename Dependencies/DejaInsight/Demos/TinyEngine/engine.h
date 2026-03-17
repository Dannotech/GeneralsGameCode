//==============================================================================
//  engine.h
//==============================================================================
//  Copyright (C) DejaTools, LLC.  All rights reserved.
//==============================================================================

#ifndef engine_h
#define engine_h

//==============================================================================
//  INCLUDES
//==============================================================================

#include "app.h"
#include <assert.h>

//==============================================================================
//  DEFINES
//==============================================================================

#undef ASSERT
#define ASSERT( _expr_ )  assert( _expr_ )

//==============================================================================
//  TYPES
//==============================================================================

class engine
{
public:
                    engine          ( void );
virtual            ~engine          ( void );

virtual void        Exit            ( int       ExitCode );         // Exit application.

virtual int         sprintf         ( char*         buffer,
                                      size_t        sizeOfBuffer,
                                      const char*   format,
                                      ... );                        // Platform safe sprintf.

virtual float       GetAppTime      ( void );                       // Get the application time.
virtual float       GetRealTime     ( void );                       // Get real time.

virtual bool        IsKeyPressed    ( int       Key );              // Check if a key is pressed.

};

//==============================================================================
//  STORAGE
//==============================================================================

extern engine* Engine;

//==============================================================================
#ifdef WIN32
//------------------------------------------------------------------------------

//
//  WinMain entry point definition.
//

#define TINY_ENGINE_MAIN( App, Title )                          \
                                                                \
    app&     g_TheApp( App );                                   \
    wchar_t* g_pWindowTitle = L ## Title;                       \
                                                                \
    int APIENTRY _tWinMain( HINSTANCE hInstance,                \
                            HINSTANCE hPrevInstance,            \
                            LPTSTR    lpCmdLine,                \
                            int       nCmdShow )                \
    {                                                           \
        return TinyEngineMain( hInstance,                       \
                               hPrevInstance,                   \
                               lpCmdLine,                       \
                               nCmdShow );                      \
    }                                                           \

int TinyEngineMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPTSTR    lpCmdLine,
                    int       nCmdShow );

//------------------------------------------------------------------------------
#endif // WIN32
//==============================================================================

//==============================================================================
#ifdef _XBOX
//------------------------------------------------------------------------------

//
//  Xbox360 entry point definition.
//

#define TINY_ENGINE_MAIN( App, Title )                          \
                                                                \
    app&     g_TheApp( App );                                   \
                                                                \
    VOID __cdecl main()                                         \
    {                                                           \
        TinyEngineMain();                                       \
    }                                                           \

int TinyEngineMain( void );

//------------------------------------------------------------------------------
#endif // _XBOX
//==============================================================================

//==============================================================================
#ifdef __CELLOS_LV2__
//------------------------------------------------------------------------------

//
//  PS3 entry point definition.
//

int TinyEngineMain( void );

#define TINY_ENGINE_MAIN( App, Title )                          \
                                                                \
    app&     g_TheApp( App );                                   \
                                                                \
    int main( void )                                            \
    {                                                           \
        TinyEngineMain();                                       \
    }                                                           \

//------------------------------------------------------------------------------
#endif // __CELLOS_LV2__
//==============================================================================

//==============================================================================
#ifdef RVL
//------------------------------------------------------------------------------

//
//  Wii entry point definition.
//

int TinyEngineMain( void );

#define TINY_ENGINE_MAIN( App, Title )                          \
                                                                \
    app&     g_TheApp( App );                                   \
                                                                \
    int main( void )                                            \
    {                                                           \
        TinyEngineMain();                                       \
    }                                                           \

//------------------------------------------------------------------------------
#endif // RVL
//==============================================================================

//==============================================================================
#endif // engine_h
//==============================================================================

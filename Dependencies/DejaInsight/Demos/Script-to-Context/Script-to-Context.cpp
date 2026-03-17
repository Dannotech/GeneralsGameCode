//==============================================================================
//
//  Script to Context
//
//==============================================================================
//  Copyright (C) DejaTools, LLC.  All rights reserved.
//==============================================================================

//==============================================================================
//  
//  This sample project demonstrates how to connect Insight's context system to
//  an embedded scripting language.  The DejaLib interface includes minor
//  extensions to facilitate this process.
//  
//==============================================================================
//
//  Steps to add context support to your embedded script system:
//
//  ----------------------------------------------------------------------------
//  
//  (1) Devise a key which can uniquely identify specific code blocks within 
//      your scripting language.
//
//      If the scripts can be revised on-the-fly, then this unique key should 
//      also incorporate a revision number so that older keys will not bind to 
//      newer revisions of a section of script code.
//      
//      This key should _NOT_ incorporate information from the context label.
//      
//      ------------------------------------------------------------------------
//      
//      For this example, I'm using file name and line number.  More 
//      specifically, I'm using the _address_ of the file name and assuming
//      that there is only a single copy of the file name in memory.  Thus,
//      when I test the key, I do not need to perform a string compare on the
//      file name.
//  
//  ----------------------------------------------------------------------------
//  
//  (2) Implement a system which is able to map your key to instances of class
//      deja_script_context_reg.  When new keys are presented, the map must 
//      create (construct) a new instance of deja_script_context_reg to 
//      associate with the key.  When known keys are presented, the previously 
//      associated deja_script_context_reg instance must be retrieved.
//  
//      Once created, the deja_script_context_reg instances must _NOT_ be moved
//      in memory.  Insight uses the address of the deja_script_context_reg 
//      instances as its internal key to the script context.
//  
//      The deja_script_context_reg instances must not be destroyed until the
//      script environment has completely and permanently terminated for the
//      current underlying program execution session.
//  
//      ------------------------------------------------------------------------
//      
//      For this example, I've created a descendant of deja_script_context_reg 
//      which adds fields to hold the key data (file and line) as well as a
//      "next" pointer so that these objects can be stored in a singly linked
//      list.
//  
//      Given a file name (address) and a line number, I scan the linked list 
//      looking for a match.  If found, the node is used directly.  (Its address
//      is the same as the address of its ancestral instance of 
//      deja_script_context_reg.)  If no match is found, a new node is created 
//      which in turn creates a deja_script_context_reg.
//  
//  ----------------------------------------------------------------------------
//  
//  (3) Implement a mechanism which enters and exits contexts in your scripting
//      environment.  This can be done in a variety of ways ranging from simply
//      adding new functions for enter/exit context, to an automatic binding of 
//      designated function entry/exit silently to context entry/exit.  The
//      context enter/exit operations must route execution to associated C++
//      bridge functions which must then properly call DEJA_SCRIPT_CONTEXT.
//
//      The context enter operation must include the unique key (from step 1).
//      The C++ function which handles context entry must use the key to locate
//      or create the deja_script_context_reg instance which is then used as
//      the argument for DEJA_SCRIPT_CONTEXT.
//
//      If your particular key scheme does not utilize the script file name and
//      line number, you are encouraged to include this information anyway (if
//      applicable to your scripting environment).  If provided, the file name 
//      and line number will be displayed within Insight's Context Tree and
//      Context List views.  If you do not have file/line information, simply
//      use NULL and -1 when constructing the deja_script_context_reg.  Although
//      not currently supported, future versions of Insight plan to include a 
//      feature to allow you to jump from Insight to corresponding lines of
//      script code in the configured editor environment.
//      
//      ------------------------------------------------------------------------
//      
//      For this example, I do not include an actual scripting environment.  
//      Instead, I start at the point where the scripting environment would pass
//      control to the C++ support functions.
//      
//      Function Script_EnterContext accepts the context label and the file/line
//      values which also serve as the context unique key.  The function 
//      linearly searches a simple linked list looking for a previously created
//      script_context_node instance which matches the key.  If not found, a new
//      instance is created and added to the linked list.  Once the proper
//      script_context_node instance is located, the address of its embedded 
//      (ancestral) deja_script_context_reg is passed to DEJA_SCRIPT_CONTEXT.
//      
//      Function Script_ExitContext has no parameters.  It simply invokes
//      DEJA_SCRIPT_CONTEXT with no argument which causes Insight to exit the
//      most recently entered script context.
//      
//      Function Script_PurgeContextData releases all memory allocated in
//      support of script context.  It can only be called after the scripting
//      "system" is completely and permanently finished operating.
//      
//==============================================================================
//      
//  Comments:
//      
//    + THIS SAMPLE CODE IS NOT SUITABLE FOR REAL WORLD USE.
//      
//    + The code which maps the file/line key to deja_script_context_reg
//      instances is VERY INEFFICIENT.  It uses a linear search on a singly
//      linked list.  You are strongly encouraged to use some hashing/mapping
//      system to associate your key values to deja_script_context_reg 
//      instances.
//      
//    + This implementation is NOT THREAD SAFE.  If your scripting environment
//      executes in multiple threads, then the code which maps from your key
//      to the deja_script_context_reg instances must be made thread safe.
//      
//    + This example uses narrow strings (char*).  If your project uses wide
//      strings (wchar_t*) then simply change the parameters as needed.  The
//      deja_script_context_reg class includes overloaded constructors for all
//      combinations of narrow and wide string parameters.
//
//==============================================================================

#include "DejaLib.h"
#pragma comment( lib, "DejaLib.Win32.lib" )

//==============================================================================
//  TYPES
//==============================================================================

struct script_context_node : public deja_script_context_reg
{
    script_context_node* pNext;
    const char*          pFileName;
    int                  Line;

    script_context_node( const char* _pLabel, 
                         const char* _pFileName, 
                               int   _Line )
    :   deja_script_context_reg( _pLabel, _pFileName, _Line ),
        pFileName( _pFileName ),
        Line( _Line )
    {   
        pNext = NULL;
    }
};

//==============================================================================
//  STORAGE
//==============================================================================

static script_context_node*  g_pScriptContextNode = NULL;

//==============================================================================
//  Functions which connect the script context to the DejaLib.
//==============================================================================

void Script_EnterContext( const char* pLabel, const char* pFileName, int Line )
{
    (void)pLabel;
    (void)pFileName;
    (void)Line;

    //-------------------
    #ifndef DEJA_DISABLED
    //-------------------

    // Search list for a script_context_node with matching file/line.
    script_context_node* pSearch = g_pScriptContextNode;
    while( pSearch )
    {
        if( (pSearch->pFileName == pFileName) && (pSearch->Line == Line) )
        {
            break;
        }
        else
        {
            pSearch = pSearch->pNext;
        }
    }

    // If we didn't find one, then we need to create one.
    if( !pSearch )
    {
        pSearch = new script_context_node( pLabel, pFileName, Line );

        pSearch->pNext       = g_pScriptContextNode;
        g_pScriptContextNode = pSearch;
    }

    // And now, using the script context registration, enter the context.
    DEJA_SCRIPT_CONTEXT( pSearch );

    //-------------------------
    #endif // not DEJA_DISABLED
    //-------------------------
}

//==============================================================================

void Script_ExitContext( void )
{
    // Invoke with no parameter to exit last script context entered.
    DEJA_SCRIPT_CONTEXT();
}

//==============================================================================

void Script_PurgeContextData( void )
{
    script_context_node* pWalk = g_pScriptContextNode;    

    while( pWalk )
    {
        script_context_node* pNext = pWalk->pNext;
        delete pWalk;
        pWalk = pNext;
    }

    g_pScriptContextNode = NULL;
}

//==============================================================================
//  C++ functions available to the script environment.
//==============================================================================

void SimulatedIntrinsicFn_CallOut( void )
{
    DEJA_CONTEXT( "CallOut" );
    DEJA_TRACE( "CallOut", "C++ code called from script code." );
    // Do relevant stuff...
}

//==============================================================================
//  Simulated script functions.
//==============================================================================

void SimulatedScriptFn_Alpha( void )
{
    Script_EnterContext( "Alpha", __FILE__, __LINE__ );

    // Do relevant stuff...

    Script_ExitContext();
}

//==============================================================================

void SimulatedScriptFn_Beta( void )
{
    Script_EnterContext( "Beta", __FILE__, __LINE__ );
    
    // Do relevant stuff...
    SimulatedScriptFn_Alpha();
    SimulatedIntrinsicFn_CallOut();

    Script_ExitContext();
}

//==============================================================================

void SimulatedScriptFn_Gamma( void )
{
    Script_EnterContext( "Gamma", __FILE__, __LINE__ );

    // Do relevant stuff...
    SimulatedScriptFn_Alpha();
    SimulatedScriptFn_Beta ();
    SimulatedScriptFn_Alpha();

    Script_ExitContext();
}

//==============================================================================
//  Main function.
//==============================================================================

void main( void )
{
    DEJA_THREAD_LABEL( "main thread" );
    DEJA_CONTEXT( "main" );

    SimulatedScriptFn_Alpha();
    SimulatedScriptFn_Beta ();
    SimulatedScriptFn_Gamma();

    // No more script execution from this point on.
    // It is now safe to dump the memory we allocated.
    Script_PurgeContextData();

    // Context "main" will exit automatically when we leave this scope block.
}

//==============================================================================

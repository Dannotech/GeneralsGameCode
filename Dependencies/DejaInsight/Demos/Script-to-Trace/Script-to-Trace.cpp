//==============================================================================
//
//  Script to Trace
//
//==============================================================================
//  Copyright (C) DejaTools, LLC.  All rights reserved.
//==============================================================================

//==============================================================================
//  
//  This sample project demonstrates how to connect Insight's trace logging 
//  system to an embedded scripting language.  The DejaLib interface includes 
//  minor extensions to facilitate this process.
//  
//==============================================================================
//
//  Steps to add trace logging support to your embedded script system:
//
//  ----------------------------------------------------------------------------
//  
//  (1) Devise a key which can uniquely identify individual trace calls within 
//      your scripting language.
//
//      If the scripts can be revised on-the-fly, then this unique key should 
//      also incorporate a revision number so that older keys will not bind to 
//      newer revisions of a script.
//      
//      This key should _NOT_ incorporate information from the trace message.
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
//      deja_script_trace_reg.  When new keys are presented, the map must create
//      (construct) a new instance of deja_script_trace_reg to associate with 
//      the key.  When known keys are presented, the previously associated
//      deja_script_trace_reg instance must be retrieved.
//  
//      Once created, the deja_script_trace_reg instances must _NOT_ be moved
//      in memory.  Insight uses the address of the deja_script_trace_reg 
//      instances as its internal key to the script trace item.
//  
//      The deja_script_trace_reg instances must not be destroyed until the
//      script environment has completely and permanently terminated for the
//      current underlying program execution session.
//  
//      The parameters to the deja_script_trace_reg constructor include a 
//      "Severity" value where:
//          0 -- normal
//          1 -- warning
//          2 -- error
//          3 -- bookmark
//      The severity value used to construct the deja_script_trace_reg is 
//      applied to all trace messages which use that deja_script_trace_reg
//      instance.  Said another way, you can't change the severity value 
//      between calls which use the same unique key.
//  
//      ------------------------------------------------------------------------
//      
//      For this example, I've created a descendant of deja_script_trace_reg 
//      which adds fields to hold the key data (file and line) as well as a
//      "next" pointer so that these objects can be stored in a singly linked
//      list.
//  
//      Given a file name (address) and a line number, I scan the linked list 
//      looking for a match.  If found, the node is used directly.  (Its 
//      address is the same as the address of its ancestral instance of 
//      deja_script_trace_reg.)  If no match is found, a new node is created 
//      which in turn creates a deja_script_trace_reg.
//  
//  ----------------------------------------------------------------------------
//  
//  (3) Implement a trace logging mechanism in your scripting environment.  The
//      script trace logging operations must route execution to the associated
//      C++ bridge functions which must then properly call DEJA_SCRIPT_TRACE.
//
//      Each operation must include the unique key information (from step 1).
//      The C++ function which handles trace logging must use the key to locate
//      or create the deja_script_trace_reg instance which is then used as the
//      argument for DEJA_SCRIPT_TRACE.
//
//      If your particular key scheme does not utilize the script file name and
//      line number, you are encouraged to include this information anyway (if
//      applicable to your scripting environment).  If provided, the file name 
//      and line number will be displayed within Insight's Trace Log view.  If 
//      you do not have file/line information, simply use NULL and -1 when 
//      constructing the deja_script_trace_reg.  Although not currently 
//      supported, future versions of Insight plan to include a feature to allow
//      you to jump from Insight to corresponding lines of script code in the 
//      configured editor environment.
//      
//      ------------------------------------------------------------------------
//      
//      For this example, I do not include an actual scripting environment.  
//      Instead, I start at the point where the scripting environment would pass
//      control to the C++ support functions.
//      
//      The functions Script_Trace, Script_Warning, Script_Error, and 
//      Script_Bookmark are "public" functions.  The function Script_Log does 
//      some "behind the scenes" work (it adds the severity parameter) and 
//      is considered a "private" function.
//      
//      Users could call the Script_Log function directly, but the behavior 
//      could be unexpected if the Severity argument was a variable or an 
//      expression.  For example:
//      
//          Message = "Number of targets: " + TargetCount;
//
//          if( TargetCount > 4 )   Severity = 1;
//          else                    Severity = 0;
//
//          Script_Log( "Script::TargetLogic", Message, Severity, File, Line );
//      
//      In this case, the severity value used the first time this message was
//      issued would be used for all subsequent messages issued by the same 
//      line of code.
//      
//      The functions Script_Trace, Script_Warning, Script_Error, and 
//      Script_Bookmark each accept the channel label, the message, and the file
//      and line values which also serve as the trace unique key.  Each of these
//      public functions add the appropriate severity value and call the private
//      function Trace_Log.  Trace_Log linearly searches a simple linked list
//      looking for a previously created script_trace_node instance which 
//      matches the key.  If not found, a new instance is created and added to
//      the linked list.  Once the proper script_trace_node instance is located,
//      the address of its embedded (ancestral) deja_script_trace_reg is passed 
//      to DEJA_SCRIPT_TRACE along with the message payload.
//      
//      Function Script_PurgeContextData releases all memory allocated in
//      support of script trace.  It can only be called after the scripting
//      "system" is completely and permanently finished operating.
//      
//==============================================================================
//      
//  Comments:
//      
//    + THIS SAMPLE CODE IS NOT SUITABLE FOR REAL WORLD USE.
//      
//    + The code which maps the file/line key to deja_script_trace_reg
//      instances is VERY INEFFICIENT.  It uses a linear search on a singly
//      linked list.  You are strongly encouraged to use some hashing/mapping
//      system to associate your key values to deja_script_trace_reg instances.
//      
//    + This implementation is NOT THREAD SAFE.  If your scripting environment
//      executes in multiple threads, then the code which maps from your key
//      to the deja_script_trace_reg instances must be made thread safe.
//
//    + This example uses narrow strings (char*).  If your project uses wide
//      strings (wchar_t*) then simply change the parameters as needed.  The
//      deja_script_trace_reg class includes overloaded constructors for all
//      combinations of narrow and wide string parameters.
//
//==============================================================================

#include "DejaLib.h"
#pragma comment( lib, "DejaLib.Win32.lib" )

//==============================================================================
//  TYPES
//==============================================================================

struct script_trace_node : public deja_script_trace_reg
{
    script_trace_node* pNext;
    const char*        pFileName;
    int                Line;

    script_trace_node( const char* _pChannel, 
                       const char* _pFirstMsg,
                             int   _Severity, 
                       const char* _pFileName,
                             int   _Line )    
    :   deja_script_trace_reg( _pChannel,     
                               _pFirstMsg,    
                               _Severity,     
                               _pFileName,    
                               _Line ),       
        pFileName( _pFileName ),             
        Line( _Line )                         
    {                                         
        pNext = NULL;                         
    }                                         
};

//==============================================================================
//  STORAGE
//==============================================================================

static script_trace_node*  g_pScriptTraceNode = NULL;

//==============================================================================
//  Functions which connect the script trace logging to the DejaLib.
//==============================================================================

void Script_Log( const char* pChannel, 
                 const char* pMessage, 
                       int   Severity, 
                 const char* pFileName, 
                       int   Line )
{
    (void)pChannel;
    (void)pMessage;
    (void)Severity;
    (void)pFileName;
    (void)Line;

    //-------------------
    #ifndef DEJA_DISABLED
    //-------------------

    // Search list for a script_trace_node with matching file/line.
    script_trace_node* pSearch = g_pScriptTraceNode;
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
        pSearch = new script_trace_node( pChannel, 
                                         pMessage, 
                                         Severity, 
                                         pFileName, 
                                         Line );

        pSearch->pNext     = g_pScriptTraceNode;
        g_pScriptTraceNode = pSearch;
    }

    // And now, using the script trace registration, go ahead with the trace.
    DEJA_SCRIPT_TRACE( pSearch, pMessage );

    //-------------------------
    #endif // not DEJA_DISABLED
    //-------------------------
}

//==============================================================================

void Script_Trace( const char* pChannel, 
                   const char* pMessage, 
                   const char* pFileName, 
                         int   Line )
{
    Script_Log( pChannel, pMessage, 0, pFileName, Line );
}

//==============================================================================

void Script_Warning( const char* pChannel, 
                     const char* pMessage, 
                     const char* pFileName, 
                           int   Line )
{
    Script_Log( pChannel, pMessage, 1, pFileName, Line );
}

//==============================================================================

void Script_Error( const char* pChannel, 
                   const char* pMessage, 
                   const char* pFileName, 
                         int   Line )
{
    Script_Log( pChannel, pMessage, 2, pFileName, Line );
}

//==============================================================================

void Script_Bookmark( const char* pChannel, 
                      const char* pMessage, 
                      const char* pFileName, 
                            int   Line )
{
    Script_Log( pChannel, pMessage, 3, pFileName, Line );
}

//==============================================================================

void Script_PurgeTraceData( void )
{
    script_trace_node* pWalk = g_pScriptTraceNode;    

    while( pWalk )
    {
        script_trace_node* pNext = pWalk->pNext;
        delete pWalk;
        pWalk = pNext;
    }

    g_pScriptTraceNode = NULL;
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
    Script_Trace( "Script::Alpha", "Alpha Message", __FILE__, __LINE__ );

    // Do relevant stuff...
}

//==============================================================================

void SimulatedScriptFn_Beta( void )
{
    Script_Trace( "Script::Beta", "Beta Message", __FILE__, __LINE__ );
    
    // Do relevant stuff...
    SimulatedScriptFn_Alpha();
    SimulatedIntrinsicFn_CallOut();
}

//==============================================================================

void SimulatedScriptFn_Gamma( void )
{
    Script_Trace   ( "Script::Gamma", "Gamma Message",  __FILE__, __LINE__ );
    Script_Warning ( "Script::Gamma", "Gamma Warning",  __FILE__, __LINE__ );
    Script_Error   ( "Script::Gamma", "Gamma Error",    __FILE__, __LINE__ );
    Script_Bookmark( "Script::Gamma", "Gamma Bookmark", __FILE__, __LINE__ );

    // Do relevant stuff...
    SimulatedScriptFn_Alpha();
    SimulatedScriptFn_Beta ();
    SimulatedScriptFn_Alpha();
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
    Script_PurgeTraceData();
}

//==============================================================================

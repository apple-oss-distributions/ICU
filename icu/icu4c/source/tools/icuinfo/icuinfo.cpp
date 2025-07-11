// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/*
*******************************************************************************
*
*   Copyright (C) 1999-2016, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
*******************************************************************************
*   file name:  icuinfo.cpp
*   encoding:   UTF-8
*   tab size:   8 (not used)
*   indentation:4
*
*   created on: 2009-2010
*   created by: Steven R. Loomis
*
*   This program shows some basic info about the current ICU.
*/

#include <stdio.h>
#include <stdlib.h>
#include "unicode/utypes.h"
#include "unicode/putil.h"
#include "unicode/uclean.h"
#include "udbgutil.h"
#include "unewdata.h"
#include "cmemory.h"
#include "cstring.h"
#include "uoptions.h"
#include "toolutil.h"
#include "icuplugimp.h"
#include <unicode/uloc.h>
#include <unicode/ucnv.h>
#include "unicode/ucal.h"
#include <unicode/ulocdata.h>
#include "putilimp.h"
#include "unicode/uchar.h"

static UOption options[]={
  /*0*/ UOPTION_HELP_H,
  /*1*/ UOPTION_HELP_QUESTION_MARK,
  /*2*/ UOPTION_ICUDATADIR,
  /*3*/ UOPTION_VERBOSE,
  /*4*/ UOPTION_DEF("list-plugins", 'L', UOPT_NO_ARG), // may be a no-op if disabled
  /*5*/ UOPTION_DEF("milisecond-time", 'm', UOPT_NO_ARG),
  /*6*/ UOPTION_DEF("cleanup", 'K', UOPT_NO_ARG),
  /*7*/ UOPTION_DEF("xml", 'x', UOPT_REQUIRES_ARG),
#if APPLE_ICU_CHANGES
// rdar://
  /*8*/ UOPTION_DEF("perf", 'p', UOPT_NO_ARG), // Apple
#endif  // APPLE_ICU_CHANGES
};

static UErrorCode initStatus = U_ZERO_ERROR;
static UBool icuInitted = false;

static void do_init() {
    if(!icuInitted) {
      u_init(&initStatus);
      icuInitted = true;
    }
}

#if APPLE_ICU_CHANGES
// rdar://
static void cmd_perf(); // Apple
#endif  // APPLE_ICU_CHANGES

static void do_cleanup() {
  if (icuInitted) {
    u_cleanup();
    icuInitted = false;
  }
}

void cmd_millis()
{
  printf("Milliseconds since Epoch: %.0f\n", uprv_getUTCtime());
}

void cmd_version(UBool /* noLoad */, UErrorCode &errorCode)
{

    do_init();

    udbg_writeIcuInfo(stdout); /* print the XML format */
    
    union {
        uint8_t byte;
        uint16_t word;
    } u;
    u.word=0x0100;
    if(U_IS_BIG_ENDIAN==u.byte) {
      //printf("U_IS_BIG_ENDIAN: %d\n", U_IS_BIG_ENDIAN);
    } else {
        fprintf(stderr, "  error: U_IS_BIG_ENDIAN=%d != %d=actual 'is big endian'\n",
                U_IS_BIG_ENDIAN, u.byte);
        errorCode=U_INTERNAL_PROGRAM_ERROR;
    }

#if defined(_MSC_VER)
// Ignore warning 4127, conditional expression is constant. This is intentional below.
#pragma warning(push)
#pragma warning(disable: 4127)
#endif

    if(U_SIZEOF_WCHAR_T==sizeof(wchar_t)) {
      //printf("U_SIZEOF_WCHAR_T: %d\n", U_SIZEOF_WCHAR_T);
    } else {
        fprintf(stderr, "  error: U_SIZEOF_WCHAR_T=%d != %d=sizeof(wchar_t)\n",
                U_SIZEOF_WCHAR_T, static_cast<int>(sizeof(wchar_t)));
        errorCode=U_INTERNAL_PROGRAM_ERROR;
    }

    int charsetFamily;
    if('A'==0x41) {
        charsetFamily=U_ASCII_FAMILY;
    } else if('A'==0xc1) {
        charsetFamily=U_EBCDIC_FAMILY;
    } else {
        charsetFamily=-1;  // unknown
    }
    if(U_CHARSET_FAMILY==charsetFamily) {
      //printf("U_CHARSET_FAMILY: %d\n", U_CHARSET_FAMILY);
    } else {
        fprintf(stderr, "  error: U_CHARSET_FAMILY=%d != %d=actual charset family\n",
                U_CHARSET_FAMILY, charsetFamily);
        errorCode=U_INTERNAL_PROGRAM_ERROR;
    }

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

    printf("\n\nICU Initialization returned: %s\n", u_errorName(initStatus));
    

#if UCONFIG_ENABLE_PLUGINS    
#if U_ENABLE_DYLOAD
    const char *pluginFile = uplug_getPluginFile();
    printf("Plugin file is: %s\n", (pluginFile&&*pluginFile)?pluginFile:"(not set. try setting ICU_PLUGINS to a directory.)");
#else
    fprintf(stderr, "Dynamic Loading: is disabled. No plugins will be loaded at start-up.\n");
#endif
#else
    fprintf(stderr, "Plugins are disabled.\n");
#endif    
}

void cmd_cleanup()
{
    u_cleanup();
    fprintf(stdout, "ICU u_cleanup() called.\n");
}


void cmd_listplugins() {
#if UCONFIG_ENABLE_PLUGINS
    int32_t i;
    UPlugData *plug;

    do_init();
    printf("ICU Initialized: u_init() returned %s\n", u_errorName(initStatus));
    
    printf("Plugins: \n");
    printf(    "# %6s   %s \n",
                       "Level",
                       "Name" );
    printf(    "    %10s:%-10s\n",
                       "Library",
                       "Symbol"
            );

                       
    printf(    "       config| (configuration string)\n");
    printf(    " >>>   Error          | Explanation \n");
    printf(    "-----------------------------------\n");
        
    for(i=0;(plug=uplug_getPlugInternal(i))!=nullptr;i++) {
        UErrorCode libStatus = U_ZERO_ERROR;
        const char *name = uplug_getPlugName(plug);
        const char *sym = uplug_getSymbolName(plug);
        const char *lib = uplug_getLibraryName(plug, &libStatus);
        const char *config = uplug_getConfiguration(plug);
        UErrorCode loadStatus = uplug_getPlugLoadStatus(plug);
        const char *message = nullptr;
        
        printf("\n#%d  %-6s %s \n",
            i+1,
            udbg_enumName(UDBG_UPlugLevel,(int32_t)uplug_getPlugLevel(plug)),
            name!=nullptr?(*name?name:"this plugin did not call uplug_setPlugName()"):"(null)"
        );
        printf("    plugin| %10s:%-10s\n",
            (U_SUCCESS(libStatus)?(lib!=nullptr?lib:"(null)"):u_errorName(libStatus)),
            sym!=nullptr?sym:"(null)"
        );
        
        if(config!=nullptr&&*config) {
            printf("    config| %s\n", config);
        }
        
        switch(loadStatus) {
            case U_PLUGIN_CHANGED_LEVEL_WARNING:
                message = "Note: This plugin changed the system level (by allocating memory or calling something which does). Later plugins may not load.";
                break;
                
            case U_PLUGIN_DIDNT_SET_LEVEL:
                message = "Error: This plugin did not call uplug_setPlugLevel during QUERY.";
                break;
            
            case U_PLUGIN_TOO_HIGH:
                message = "Error: This plugin couldn't load because the system level was too high. Try loading this plugin earlier.";
                break;
                
            case U_ZERO_ERROR: 
                message = nullptr; /* no message */
                break;
            default:
                if(U_FAILURE(loadStatus)) {
                    message = "error loading:";
                } else {
                    message = "warning during load:";
                }            
        }
        
        if(message!=nullptr) {
            printf("\\\\\\ status| %s\n"
                   "/// %s\n", u_errorName(loadStatus), message);
        }
        
    }
	if(i==0) {
		printf("No plugins loaded.\n");
	}
#endif
}



extern int
main(int argc, char* argv[]) {
    UErrorCode errorCode = U_ZERO_ERROR;
    UBool didSomething = false;
    
    /* preset then read command line options */
    argc=u_parseArgs(argc, argv, UPRV_LENGTHOF(options), options);

    /* error handling, printing usage message */
    if(argc<0) {
        fprintf(stderr,
            "error in command line argument \"%s\"\n",
            argv[-argc]);
    }
    if( options[0].doesOccur || options[1].doesOccur) {
      fprintf(stderr, "%s: Output information about the current ICU\n", argv[0]);
      fprintf(stderr, "Options:\n"
              " -h     or  --help                 - Print this help message.\n"
              " -m     or  --millisecond-time     - Print the current UTC time in milliseconds.\n"
              " -d <dir>   or  --icudatadir <dir> - Set the ICU Data Directory\n"
              " -v                                - Print version and configuration information about ICU\n"
#if UCONFIG_ENABLE_PLUGINS
              " -L         or  --list-plugins     - List and diagnose issues with ICU Plugins\n"
#endif
              " -K         or  --cleanup          - Call u_cleanup() before exiting (will attempt to unload plugins)\n"
#if APPLE_ICU_CHANGES
// rdar://
              " -p         or  --perf             - Perf tests (Apple)\n"
#endif  // APPLE_ICU_CHANGES
              "\n"
              "If no arguments are given, the tool will print ICU version and configuration information.\n"
              );
      fprintf(stderr, "International Components for Unicode %s\n%s\n", U_ICU_VERSION, U_COPYRIGHT_STRING );
      return argc<0 ? U_ILLEGAL_ARGUMENT_ERROR : U_ZERO_ERROR;
    }
    
    if(options[2].doesOccur) {
      u_setDataDirectory(options[2].value);
    }

    if(options[5].doesOccur) {
      cmd_millis();
      didSomething=true;
    } 
    if(options[4].doesOccur) {
      cmd_listplugins();
      didSomething = true;
    }

    if(options[3].doesOccur) {
      cmd_version(false, errorCode);
      didSomething = true;
    }

    if(options[7].doesOccur) {  /* 2nd part of version: cleanup */
      FILE *out = fopen(options[7].value, "w");
      if(out==nullptr) {
        fprintf(stderr,"ERR: can't write to XML file %s\n", options[7].value);
        return 1;
      }
      /* todo: API for writing DTD? */
      fprintf(out, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      udbg_writeIcuInfo(out);
      fclose(out);
      didSomething = true;
    }

    if(options[6].doesOccur) {  /* 2nd part of version: cleanup */
      cmd_cleanup();
      didSomething = true;
    }

#if APPLE_ICU_CHANGES
// rdar://
    if(options[8].doesOccur) { // Apple
      cmd_perf();
      didSomething=TRUE;
    } 
#endif  // APPLE_ICU_CHANGES

    if(!didSomething) {
      cmd_version(false, errorCode);  /* at least print the version # */
    }

    do_cleanup();

    return U_FAILURE(errorCode);
}

#if APPLE_ICU_CHANGES
// rdar://
#if defined(__APPLE__) && defined(__MACH__)
// Apple addition
#include <unistd.h>
#include <mach/mach_time.h>
#include <unicode/ustring.h>
#include <unicode/udat.h>
enum { kUCharsOutMax = 128, kBytesOutMax = 256 };

static void cmd_perf() {
    static const char* locale = "en_US";
    static const UChar* tzName = (const UChar*)u"America/Los_Angeles";
    static const UDate udatTry1 = 1290714600000.0; // 2010 Nov. 25 (Thurs) 11:50:00 AM PT
    static const UDate udatTry2 = 1451736016000.0; // 2016 Jan. 02  ...
    int remaining = 2;
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);
    while (remaining-- > 0) {
        uint64_t start, durationOpen, durationUse1, durationUse2;
        UDateFormat *udatfmt;
        int32_t datlen1, datlen2;
        UChar outUChars[kUCharsOutMax];
        UErrorCode status = U_ZERO_ERROR;
    
        start = mach_absolute_time();
        udatfmt = udat_open(UDAT_MEDIUM, UDAT_FULL, locale, tzName, -1, NULL, 0, &status);
        durationOpen = ((mach_absolute_time() - start) * info.numer)/info.denom;
        if ( U_SUCCESS(status) ) {
            start = mach_absolute_time();
            datlen1 = udat_format(udatfmt, udatTry1, outUChars, kUCharsOutMax, NULL, &status);
            durationUse1 = ((mach_absolute_time() - start) * info.numer)/info.denom;

            start = mach_absolute_time();
            datlen2 = udat_format(udatfmt, udatTry2, outUChars, kUCharsOutMax, NULL, &status);
            durationUse2 = ((mach_absolute_time() - start) * info.numer)/info.denom;

            if ( U_SUCCESS(status) ) {
                printf("first time %d udat open, fmt1(len %d), fmt2(len %d) nsec:\t%llu\t%llu\t%llu\n", remaining, datlen1, datlen2, durationOpen, durationUse1, durationUse2);
            } else {
                printf("first time %d udat_format failed\n", remaining);
            }
            udat_close(udatfmt);
        } else {
            printf("first time %d udat_open failed\n", remaining);
        }
    }
}
#else
static void cmd_perf() {
    printf("This feature is unsupported on this platform\n");
}
#endif
#endif  // APPLE_ICU_CHANGES

// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 2008-2016, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include <stdio.h>
#include <stdlib.h>
#include "dtptngts.h"

#include "unicode/calendar.h"
#include "unicode/smpdtfmt.h"
#include "unicode/dtfmtsym.h"
#include "unicode/dtptngen.h"
#include "unicode/ustring.h"
#include "unicode/datefmt.h"
#include "cmemory.h"
#include "cstring.h"
#include "loctest.h"


// This is an API test, not a unit test.  It doesn't test very many cases, and doesn't
// try to test the full functionality.  It just calls each function in the class and
// verifies that it works on a basic level.

void IntlTestDateTimePatternGeneratorAPI::runIndexedTest( int32_t index, UBool exec, const char* &name, char* /*par*/ )
{
    if (exec) logln("TestSuite DateTimePatternGeneratorAPI");
    switch (index) {
        TESTCASE(0, testAPI);
        TESTCASE(1, testOptions);
        TESTCASE(2, testAllFieldPatterns);
        TESTCASE(3, testStaticGetSkeleton);
        TESTCASE(4, testC);
        TESTCASE(5, testSkeletonsWithDayPeriods);
        TESTCASE(6, testGetFieldDisplayNames);
        TESTCASE(7, testJjMapping);
        TESTCASE(8, test20640_HourCyclArsEnNH);
        TESTCASE(9, testFallbackWithDefaultRootLocale);
        TESTCASE(10, testGetDefaultHourCycle_OnEmptyInstance);
        TESTCASE(11, test_jConsistencyOddLocales);
        TESTCASE(12, testBestPattern);
        TESTCASE(13, testDateTimePatterns);
        TESTCASE(14, testRegionOverride);
        TESTCASE(15, testISO8601);
#if APPLE_ICU_CHANGES
// rdar://
        TESTCASE(16, testHorizontalInheritance);
        TESTCASE(17, testPunjabiPattern);
#endif  // APPLE_ICU_CHANGES
        default: name = ""; break;
    }
}

#if APPLE_ICU_CHANGES
// rdar://
#define MAX_LOCALE   15
#else
#define MAX_LOCALE   12
#endif  // APPLE_ICU_CHANGES

/**
 * Test various generic API methods of DateTimePatternGenerator for API coverage.
 */
void IntlTestDateTimePatternGeneratorAPI::testAPI(/*char *par*/)
{
    UnicodeString patternData[] = {
        UnicodeString("yM"),        // 00
        UnicodeString("yMMM"),      // 01
        UnicodeString("yMd"),       // 02
        UnicodeString("yMMMd"),     // 03
        UnicodeString("Md"),        // 04
        UnicodeString("MMMd"),      // 05
        UnicodeString("MMMMd"),     // 06
        UnicodeString("yQQQ"),      // 07
        UnicodeString("hhmm"),      // 08
        UnicodeString("HHmm"),      // 09
        UnicodeString("jjmm"),      // 10
        UnicodeString("mmss"),      // 11
        UnicodeString("yyyyMMMM"),  // 12
        UnicodeString("MMMEd"),     // 13
        UnicodeString("Ed"),        // 14
        UnicodeString("jmmssSSS"),  // 15
        UnicodeString("JJmm"),      // 16
        UnicodeString(),
     };

    const char* testLocale[MAX_LOCALE][4] = {
        {"en", "US", "", ""},                   // 0
        {"en", "US", "", "calendar=japanese"},  // 1
        {"de", "DE", "", ""},                   // 2
        {"fi", "", "", ""},                     // 3
        {"es", "", "", ""},                     // 4
        {"ja", "", "", ""},                     // 5
        {"ja", "", "", "calendar=japanese"},    // 6
        {"zh", "Hans", "CN", ""},               // 7
        {"zh", "TW", "", "calendar=roc"},       // 8
        {"ru", "", "", ""},                     // 9
        {"zh", "", "", "calendar=chinese"},     // 10
        {"ja", "JP", "TRADITIONAL", ""},        // 11
#if APPLE_ICU_CHANGES
// rdar://16482189&16567439 chinese calendar date formats: Use rU or r (not y), drop 日 in zh formats
// rdar://
        {"zh", "", "", "calendar=chinese;numbers=hanidays"},    // 12 Apple add
        {"ar", "", "", ""},                     // 13 Apple add
        {"en", "ID", "", "calendar=buddhist"},  // 14 Apple add <rdar://problem/37505940>
#endif  // APPLE_ICU_CHANGES
     };

    // For Weds, Jan 13, 1999, 23:58:59
    UnicodeString patternResults_en_US[] = {
        // en_US                                              // 0 en_US
        UnicodeString("1/1999"),                              // 00: yM
        UnicodeString("Jan 1999"),                            // 01: yMMM
        UnicodeString("1/13/1999"),                           // 02: yMd
        UnicodeString("Jan 13, 1999"),                        // 03: yMMMd
        UnicodeString("1/13"),                                // 04: Md
        UnicodeString("Jan 13"),                              // 05: MMMd
        UnicodeString("January 13"),                          // 06: MMMMd
        UnicodeString("Q1 1999"),                             // 07: yQQQ
        UnicodeString(u"11:58\u202FPM", -1),                 // 08: hhmm
        UnicodeString("23:58"),                               // 09: HHmm
        UnicodeString(u"11:58\u202FPM", -1),                 // 10: jjmm
        UnicodeString("58:59"),                               // 11: mmss
        UnicodeString("January 1999"),                        // 12: yyyyMMMM
        UnicodeString("Wed, Jan 13"),                         // 13: MMMEd -> EEE, MMM d
#if APPLE_ICU_CHANGES
// rdar://98584839 en_US: Compact date shown alongside inline widget shows “12 Fri” instead of “Fri 12”
        UnicodeString("Wed 13"),                              // 14: Ed    -> E d
#else
        UnicodeString("13 Wed"),                              // 14: Ed    -> d EEE
#endif  // APPLE_ICU_CHANGES
        UnicodeString(u"11:58:59.123\u202FPM", -1),          // 15: jmmssSSS -> "h:mm:ss.SSS a"
        UnicodeString("11:58"),                               // 16: JJmm
    };

    UnicodeString patternResults_en_US_japanese[] = {
        // en_US@calendar=japanese                            // 1 en_US@calendar=japanese
#if APPLE_ICU_CHANGES
// rdar://69457523 en/de/es/fr, japanese cal date formats should use G y or GGGGGy+
        UnicodeString("1/H11"),                               //  0: yM
        UnicodeString("Jan Heisei 11"),                       //  1: yMMM
        UnicodeString("1/13/H11"),                            //  2: yMd
        UnicodeString("Jan 13, Heisei 11"),                   //  3: yMMMd
#else
        UnicodeString("1/11 H"),                              //  0: yM
        UnicodeString("Jan 11 Heisei"),                       //  1: yMMM
        UnicodeString("1/13/11 H"),                           //  2: yMd
        UnicodeString("Jan 13, 11 Heisei"),                   //  3: yMMMd
#endif  // APPLE_ICU_CHANGES
        UnicodeString("1/13"),                                //  4: Md
        UnicodeString("Jan 13"),                              //  5: MMMd
        UnicodeString("January 13"),                          //  6: MMMMd
#if APPLE_ICU_CHANGES
// rdar://69457523 en/de/es/fr, japanese cal date formats should use G y or GGGGGy+
        UnicodeString("Q1 Heisei 11"),                        //  7: yQQQ
#else
        UnicodeString("Q1 11 Heisei"),                        //  7: yQQQ
#endif  // APPLE_ICU_CHANGES
        UnicodeString(u"11:58\u202FPM", -1),                 //  8: hhmm
        UnicodeString("23:58"),                               //  9: HHmm
        UnicodeString(u"11:58\u202FPM", -1),                 // 10: jjmm
        UnicodeString("58:59"),                               // 11: mmss
#if APPLE_ICU_CHANGES
// rdar://69457523 en/de/es/fr, japanese cal date formats should use G y or GGGGGy+
        UnicodeString("January Heisei 11"),                   // 12: yyyyMMMM
#else
        UnicodeString("January 11 Heisei"),                   // 12: yyyyMMMM
#endif  // APPLE_ICU_CHANGES
        UnicodeString("Wed, Jan 13"),                         // 13: MMMEd -> EEE, MMM d"
        UnicodeString("13 Wed"),                              // 14: Ed    -> d EEE
        UnicodeString(u"11:58:59.123\u202FPM", -1),          // 15: jmmssSSS -> "h:mm:ss.SSS a"
        UnicodeString("11:58"),                               // 16: JJmm
    };

    UnicodeString patternResults_de_DE[] = {
        // de_DE                                              // 2 de_DE
#if APPLE_ICU_CHANGES
// rdar://97937093 restore . not /
        UnicodeString("1.1999"),                              // 00: yM
#else
        UnicodeString("1/1999"),                              // 00: yM
#endif  // APPLE_ICU_CHANGES
        UnicodeString("Jan. 1999"),                           // 01: yMMM
        UnicodeString("13.1.1999"),                           // 02: yMd
        UnicodeString("13. Jan. 1999"),                       // 03: yMMMd
        UnicodeString("13.1."),                               // 04: Md
        UnicodeString("13. Jan."),                            // 05: MMMd
        UnicodeString("13. Januar"),                          // 06: MMMMd
        UnicodeString("Q1 1999"),                             // 07: yQQQ
        UnicodeString(u"11:58\u202FPM", -1),                 // 08: hhmm
        UnicodeString("23:58"),                               // 09: HHmm
        UnicodeString("23:58"),                               // 10: jjmm
        UnicodeString("58:59"),                               // 11: mmss
        UnicodeString("Januar 1999"),                         // 12: yyyyMMMM
#if APPLE_ICU_CHANGES
// rdar://
        UnicodeString("Mi. 13. Jan."),                        // 13: MMMEd -> E d. MMM
        UnicodeString("Mi. 13."),                             // 14: Ed   -> E d.
#else
        UnicodeString("Mi., 13. Jan."),                       // 13: MMMEd -> EEE, d. MMM
        UnicodeString("Mi., 13."),                            // 14: Ed   -> EEE d.
#endif  // APPLE_ICU_CHANGES
        UnicodeString("23:58:59,123"),                        // 15: jmmssSSS -> "HH:mm:ss,SSS"
        UnicodeString("23:58"),                               // 16: JJmm
    };

    UnicodeString patternResults_fi[] = {
        // fi                                                 // 3 fi
        UnicodeString("1.1999"),                              // 00: yM (fixed expected result per ticket:6626:)
        UnicodeString("tammi 1999"),                          // 01: yMMM
        UnicodeString("13.1.1999"),                           // 02: yMd
        UnicodeString("13.1.1999"),                           // 03: yMMMd
        UnicodeString("13.1."),                               // 04: Md
        UnicodeString("13.1."),                               // 05: MMMd
        UnicodeString("13. tammikuuta"),                      // 06: MMMMd
        UnicodeString("1. nelj. 1999"),                       // 07: yQQQ
        UnicodeString(u"11.58\u202Fip.", -1),                // 08: hhmm
        UnicodeString("23.58"),                               // 09: HHmm
        UnicodeString("23.58"),                               // 10: jjmm
        UnicodeString("58.59"),                               // 11: mmss
        UnicodeString("tammikuu 1999"),                       // 12: yyyyMMMM
        UnicodeString("ke 13.1."),                            // 13: MMMEd -> EEE d. MMM
        UnicodeString("ke 13."),                              // 14: Ed    -> ccc d.
        UnicodeString("23.58.59,123"),                        // 15: jmmssSSS -> "H.mm.ss,SSS"
        UnicodeString("23.58"),                               // 16: JJmm
    };

    UnicodeString patternResults_es[] = {
        // es                                                 // 4 es
        UnicodeString("1/1999"),                              // 00: yM    -> "M/y"
        UnicodeString("ene 1999"),                            // 01: yMMM  -> "MMM y"
        UnicodeString("13/1/1999"),                           // 02: yMd   -> "d/M/y"
        UnicodeString("13 ene 1999"),                         // 03: yMMMd -> "d MMM y"
        UnicodeString("13/1"),                                // 04: Md    -> "d/M"
        UnicodeString("13 ene"),                              // 05: MMMd  -> "d 'de' MMM"
        UnicodeString("13 de enero"),                         // 06: MMMMd -> "d 'de' MMMM"
        UnicodeString("T1 1999"),                             // 07: yQQQ  -> "QQQ y"
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("11:58\\u202Fp.\\u202Fm."),       // 08: hhmm  -> "hh:mm a"
#else
        UnicodeString(u"11:58\u202Fp.\u00A0m.", -1),          // 08: hhmm  -> "hh:mm a"
#endif  // APPLE_ICU_CHANGES
        UnicodeString("23:58"),                               // 09: HHmm  -> "HH:mm"
        UnicodeString("23:58"),                               // 10: jjmm  -> "HH:mm"
        UnicodeString("58:59"),                               // 11: mmss  -> "mm:ss"
        UnicodeString("enero de 1999"),                       // 12: yyyyMMMM -> "MMMM 'de' yyyy"
        CharsToUnicodeString("mi\\u00E9, 13 ene"),            // 13: MMMEd -> "E, d MMM"
        CharsToUnicodeString("mi\\u00E9 13"),                 // 14: Ed    -> "EEE d"
        UnicodeString("23:58:59,123"),                        // 15: jmmssSSS -> "H:mm:ss,SSS"
        UnicodeString("23:58"),                               // 16: JJmm
    };

    UnicodeString patternResults_ja[] = {
        // ja                                                             // 5 ja
        UnicodeString("1999/1"),                                          // 00: yM    -> y/M
        CharsToUnicodeString("1999\\u5E741\\u6708"),                      // 01: yMMM  -> y\u5E74M\u6708
        UnicodeString("1999/1/13"),                                       // 02: yMd   -> y/M/d
        CharsToUnicodeString("1999\\u5E741\\u670813\\u65E5"),             // 03: yMMMd -> y\u5E74M\u6708d\u65E5
        UnicodeString("1/13"),                                            // 04: Md    -> M/d
        CharsToUnicodeString("1\\u670813\\u65E5"),                        // 05: MMMd  -> M\u6708d\u65E5
        CharsToUnicodeString("1\\u670813\\u65E5"),                        // 06: MMMMd  -> M\u6708d\u65E5
        CharsToUnicodeString("1999/Q1"),                                  // 07: yQQQ  -> y/QQQ
        CharsToUnicodeString("\\u5348\\u5F8C11:58"),                      // 08: hhmm
        UnicodeString("23:58"),                                           // 09: HHmm  -> HH:mm
        UnicodeString("23:58"),                                           // 10: jjmm
        UnicodeString("58:59"),                                           // 11: mmss  -> mm:ss
        CharsToUnicodeString("1999\\u5E741\\u6708"),                      // 12: yyyyMMMM  -> y\u5E74M\u6708
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("1\\u670813\\u65E5(\\u6C34)"),               // 13: MMMEd -> M\u6708d\u65E5(E)
        CharsToUnicodeString("13\\u65E5(\\u6C34)"),                       // 14: Ed    -> d\u65E5(E)
#else
        CharsToUnicodeString("1\\u670813\\u65E5(\\u6C34)"),               // 13: MMMEd -> M\u6708d\u65E5(EEE)
        CharsToUnicodeString("13\\u65E5(\\u6C34)"),                       // 14: Ed    -> d\u65E5(EEE)
#endif  // APPLE_ICU_CHANGES
        UnicodeString("23:58:59.123"),                                    // 15: jmmssSSS -> "H:mm:ss.SSS"
        UnicodeString("23:58"),                                           // 16: JJmm
    };

    UnicodeString patternResults_ja_japanese[] = {
        // ja@calendar=japanese                                           // 6 ja@calendar=japanese
#if APPLE_ICU_CHANGES
// rdar://
        UnicodeString("H11/01"),                                          // 00: yM    -> GGGGGy/MM
#else
        UnicodeString("H11/1"),                                           // 00: yM    -> GGGGGy/m
#endif  // APPLE_ICU_CHANGES
        CharsToUnicodeString("\\u5E73\\u621011\\u5E741\\u6708"),          // 01: yMMM  -> Gy\u5E74M\u6708
#if APPLE_ICU_CHANGES
// rdar://
        UnicodeString("H11/01/13"),                                       // 02: yMd   -> GGGGGy/MM/dd
#else
        UnicodeString("H11/1/13"),                                        // 02: yMd   -> GGGGGy/m/d
#endif  // APPLE_ICU_CHANGES
        CharsToUnicodeString("\\u5E73\\u621011\\u5E741\\u670813\\u65E5"), // 03: yMMMd -> Gy\u5E74M\u6708d\u65E5
        UnicodeString("1/13"),                                            // 04: Md    -> M/d (M/dd for Apple, rdar:// )
        CharsToUnicodeString("1\\u670813\\u65E5"),                        // 05: MMMd  -> M\u6708d\u65E5
        CharsToUnicodeString("1\\u670813\\u65E5"),                        // 06: MMMMd  -> M\u6708d\u65E5
        CharsToUnicodeString("\\u5E73\\u621011/Q1"),                     // 07: yQQQ  -> Gy/QQQ
        CharsToUnicodeString("\\u5348\\u5F8C11:58"),                      // 08: hhmm  ->
        UnicodeString("23:58"),                                           // 09: HHmm  -> HH:mm          (as for ja)
        UnicodeString("23:58"),                                           // 10: jjmm
        UnicodeString("58:59"),                                           // 11: mmss  -> mm:ss          (as for ja)
        CharsToUnicodeString("\\u5E73\\u621011\\u5E741\\u6708"),          // 12: yyyyMMMM  -> Gyyyy\u5E74M\u6708
        CharsToUnicodeString("1\\u670813\\u65E5(\\u6C34)"),               // 13: MMMEd -> M\u6708d\u65E5(EEE)
        CharsToUnicodeString("13\\u65E5(\\u6C34)"),                       // 14: Ed    -> d\u65E5(EEE)
        UnicodeString("23:58:59.123"),                                    // 15: jmmssSSS -> "H:mm:ss.SSS"
        UnicodeString("23:58"),                                           // 16: JJmm
    };

    UnicodeString patternResults_zh_Hans_CN[] = {
        // zh_Hans_CN                                                     // 7 zh_Hans_CN
        CharsToUnicodeString("1999/1"),                                   // 00: yM
        CharsToUnicodeString("1999\\u5E741\\u6708"),                      // 01: yMMM  -> yyyy\u5E74MMM (fixed expected result per ticket:6626:)
        CharsToUnicodeString("1999/1/13"),                                // 02: yMd
        CharsToUnicodeString("1999\\u5E741\\u670813\\u65E5"),             // 03: yMMMd -> yyyy\u5E74MMMd\u65E5 (fixed expected result per ticket:6626:)
        UnicodeString("1/13"),                                            // 04: Md
        CharsToUnicodeString("1\\u670813\\u65E5"),                        // 05: MMMd  -> M\u6708d\u65E5 (fixed expected result per ticket:6626:)
        CharsToUnicodeString("1\\u670813\\u65E5"),                        // 06: MMMMd  -> M\u6708d\u65E5
        CharsToUnicodeString("1999\\u5E74\\u7B2C1\\u5B63\\u5EA6"),        // 07: yQQQ
        CharsToUnicodeString("\\u4E0B\\u534811:58"),                      // 08: hhmm
        UnicodeString("23:58"),                                           // 09: HHmm
        CharsToUnicodeString("23:58"),                                    // 10: jjmm
        UnicodeString("58:59"),                                           // 11: mmss
        CharsToUnicodeString("1999\\u5E741\\u6708"),                      // 12: yyyyMMMM  -> yyyy\u5E74MMM
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("1\\u670813\\u65E5 \\u5468\\u4E09"),         // 13: MMMEd -> MMMd\u65E5 EEE
        CharsToUnicodeString("13 \\u5468\\u4E09"),                        // 14: Ed    -> d\u65E5EEE
#else
        CharsToUnicodeString("1\\u670813\\u65E5\\u5468\\u4E09"),          // 13: MMMEd -> MMMd\u65E5EEE
        CharsToUnicodeString("13\\u65E5\\u5468\\u4E09"),                  // 14: Ed    -> d\u65E5EEE
#endif  // APPLE_ICU_CHANGES
        CharsToUnicodeString("23:58:59.123"),                             // 15: jmmssSSS -> "ah:mm:ss.SSS"
        UnicodeString("23:58"),                                           // 16: JJmm
    };

    UnicodeString patternResults_zh_TW_roc[] = {
        // zh_TW@calendar=roc                                             // 8 zh_TW@calendar=roc
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("\\u6C11\\u570B 88/1"),                      // 00: yM    -> G y/M
        CharsToUnicodeString("\\u6C11\\u570B 88\\u5E741\\u6708"),         // 01: yMMM  -> G y\u5E74M\u6708
        CharsToUnicodeString("\\u6C11\\u570B 88/1/13"),                   // 02: yMd   -> G y/M/d
        CharsToUnicodeString("\\u6C11\\u570B 88\\u5E741\\u670813\\u65E5"), // 03: yMMMd -> G y\u5E74M\u6708d\u65E5
#else
        CharsToUnicodeString("\\u6C11\\u570B88/1"),                       // 00: yM    -> Gy/M
        CharsToUnicodeString("\\u6C11\\u570B88\\u5E741\\u6708"),          // 01: yMMM  -> Gy\u5E74M\u6708
        CharsToUnicodeString("\\u6C11\\u570B88/1/13"),                    // 02: yMd   -> Gy/M/d
        CharsToUnicodeString("\\u6C11\\u570B88\\u5E741\\u670813\\u65E5"), // 03: yMMMd -> Gy\u5E74M\u6708d\u65E5
#endif  // APPLE_ICU_CHANGES
        UnicodeString("1/13"),                                            // 04: Md    -> M/d
        CharsToUnicodeString("1\\u670813\\u65E5"),                        // 05: MMMd  ->M\u6708d\u65E5
        CharsToUnicodeString("1\\u670813\\u65E5"),                        // 06: MMMMd  ->M\u6708d\u65E5
#if APPLE_ICU_CHANGES
// unknown Radar, later altered by rdar://17278425
        CharsToUnicodeString("\\u6C11\\u570B 88\\u5E741\\u5B63"),         // 07: yQQQ  -> G yQQQ
        CharsToUnicodeString("\\u665a\\u4e0a11:58"),                      // 08: hhmm  ->
        UnicodeString("23:58"),                                           // 09: HHmm  ->
        CharsToUnicodeString("\\u665a\\u4e0a11:58"),                      // 10: jjmm
#else
        CharsToUnicodeString("\\u6C11\\u570B88\\u5E74\\u7B2C1\\u5B63"),   // 07: yQQQ  -> Gy QQQ
        CharsToUnicodeString("\\u4E0B\\u534811:58"),                      // 08: hhmm  ->
        UnicodeString("23:58"),                                           // 09: HHmm  ->
        CharsToUnicodeString("\\u4E0B\\u534811:58"),                      // 10: jjmm
#endif  // APPLE_ICU_CHANGES
        UnicodeString("58:59"),                                           // 11: mmss  ->
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("\\u6C11\\u570B 88\\u5E741\\u6708"),         // 12: yyyyMMMM  -> G y\u5E74M\u670
        CharsToUnicodeString("1\\u670813\\u65E5 \\u9031\\u4E09"),         // 13: MMMEd -> M\u6708d\u65E5 E
        CharsToUnicodeString("13 \\u9031\\u4E09"),                        // 14: Ed    -> d E
        CharsToUnicodeString("\\u665a\\u4e0a11:58:59.123"),               // 15: jmmssSSS -> "ah:mm:ss.SSS"
#else
        CharsToUnicodeString("\\u6C11\\u570B88\\u5E741\\u6708"),          // 12: yyyyMMMM  -> Gy\u5E74M\u670
        CharsToUnicodeString("1\\u670813\\u65E5\\u9031\\u4E09"),          // 13: MMMEd -> M\u6708d\u65E5EEE
        CharsToUnicodeString("13 \\u9031\\u4E09"),                        // 14: Ed    -> d E
        CharsToUnicodeString("\\u4E0B\\u534811:58:59.123"),               // 15: jmmssSSS -> "ah:mm:ss.SSS"
#endif  // APPLE_ICU_CHANGES
        UnicodeString("11:58"),                                           // 16: JJmm
    };

    UnicodeString patternResults_ru[] = {
        // ru                                                             // 9 ru
        UnicodeString("01.1999"),                                         // 00: yM    -> MM.y
        UnicodeString(u"\u044F\u043D\u0432. 1999\u202F\u0433.", -1),      // 01: yMMM  -> LLL y
        UnicodeString("13.01.1999"),                                      // 02: yMd   -> dd.MM.y
        UnicodeString(u"13 \u044F\u043D\u0432. 1999\u202F\u0433.",-1),    // 03: yMMMd -> d MMM y
        UnicodeString("13.01"),                                           // 04: Md    -> dd.MM
        CharsToUnicodeString("13 \\u044F\\u043D\\u0432."),                // 05: MMMd  -> d MMM
        CharsToUnicodeString("13 \\u044F\\u043D\\u0432\\u0430\\u0440\\u044F"), // 06: MMMMd  -> d MMMM
        UnicodeString(u"1-\u0439 \u043A\u0432. 1999\u202F\u0433.",-1),    // 07: yQQQ  -> y QQQ
        UnicodeString(u"11:58\u202FPM", -1),                              // 08: hhmm  -> hh:mm a
        UnicodeString("23:58"),                                           // 09: HHmm  -> HH:mm
        UnicodeString("23:58"),                                           // 10: jjmm  -> HH:mm
        UnicodeString("58:59"),                                           // 11: mmss  -> mm:ss
        UnicodeString(u"\u044F\u043D\u0432\u0430\u0440\u044C 1999\u202F\u0433.",-1), // 12: yyyyMMMM -> LLLL y
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("\\u0421\\u0440, 13 \\u044F\\u043D\\u0432."), // 13: MMMEd -> ccc, d MMM
        CharsToUnicodeString("\\u0421\\u0440, 13"),                       // 14: Ed    -> EEE, d
#else
        CharsToUnicodeString("\\u0441\\u0440, 13 \\u044F\\u043D\\u0432."), // 13: MMMEd -> ccc, d MMM
        CharsToUnicodeString("\\u0441\\u0440, 13"),                       // 14: Ed    -> EEE, d
#endif  // APPLE_ICU_CHANGES
        UnicodeString("23:58:59,123"),                                    // 15: jmmssSSS -> "H:mm:ss,SSS"
        UnicodeString("23:58"),                                           // 16: JJmm
    };

    UnicodeString patternResults_zh_chinese[] = {
        // zh@calendar=chinese                                            // 10 zh@calendar=chinese
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u51AC\\u6708"),  // 00: yMMM
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u51AC\\u6708"),  // 01: yMMM
        CharsToUnicodeString("1998\\u5E74\\u51AC\\u670826"),              // 02: yMMMd
        CharsToUnicodeString("1998\\u5E74\\u51AC\\u670826"),              // 03: yMMMd
#else
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u5341\\u4E00\\u6708"), // 00: yMMM
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u5341\\u4E00\\u6708"), // 01: yMMM
        CharsToUnicodeString("1998\\u5E74\\u5341\\u4E00\\u670826"),             // 02: yMMMd
        CharsToUnicodeString("1998\\u5E74\\u5341\\u4E00\\u670826"),             // 03: yMMMd
#endif  // APPLE_ICU_CHANGES
        UnicodeString("11-26"),                                                 // 04: Md
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("\\u51AC\\u670826"),                         // 05: MMMd
        CharsToUnicodeString("\\u51AC\\u670826"),                         // 06: MMMMd
#else
        CharsToUnicodeString("\\u5341\\u4E00\\u670826\\u65E5"),                 // 05: MMMd
        CharsToUnicodeString("\\u5341\\u4E00\\u670826\\u65E5"),                 // 06: MMMMd
#endif  // APPLE_ICU_CHANGES
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u7b2c\\u56db\\u5B63\\u5EA6"),  // 07: yQQQ
        CharsToUnicodeString("\\u4E0B\\u534811:58"),                            // 08: hhmm
        UnicodeString("23:58"),                                                 // 09: HHmm
        CharsToUnicodeString("23:58"),                                          // 10: jjmm
        UnicodeString("58:59"),                                                 // 11: mmss
#if APPLE_ICU_CHANGES
// rdar://
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u51AC\\u6708"),        // 12: yyyyMMMM
        CharsToUnicodeString("\\u51AC\\u670826\\u5468\\u4E09"),                 // 13: MMMEd
        CharsToUnicodeString("26\\u5468\\u4E09"),                               // 14: Ed -> dEEE
#else
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u5341\\u4E00\\u6708"), // 12: yyyyMMMM
        CharsToUnicodeString("\\u5341\\u4E00\\u670826\\u65E5\\u5468\\u4E09"),   // 13: MMMEd
        CharsToUnicodeString("26\\u65E5\\u5468\\u4E09"),                        // 14: Ed    -> d\u65E5EEE
#endif  // APPLE_ICU_CHANGES
        CharsToUnicodeString("23:58:59.123"),                                   // 15: jmmssSS
        UnicodeString("23:58"),                                                 // 16: JJmm
    };

    UnicodeString patternResults_ja_jp_traditional[] = { // 11
        // ja_JP_TRADITIONAL                 // 11 ja_JP_TRADITIONAL
#if APPLE_ICU_CHANGES
// rdar://
        u"AD1999/01",                        // 00: yM
#else
        u"AD1999/1",                         // 00: yM
#endif  // APPLE_ICU_CHANGES
        u"西暦1999年1月",                     // 01: yMMM
        u"1999年1月13日",                     // 02: yMd
        u"西暦1999年1月13日",                  // 03: yMMMd
        u"1/13",                             // 04: Md
        u"1月13日",                           // 05: MMMd
        u"1月13日",                           // 06: MMMMd
        u"西暦1999/Q1",                       // 07: yQQQ
        u"午後11:58",                         // 08: hhmm
        u"23:58",                            // 09: HHmm
        u"23:58",                            // 10: jjmm
        u"58:59",                            // 11: mmss
        u"西暦1999年1月",                     // 12: yyyyMMMM
        u"1月13日(水)",                       // 13: MMMEd
        u"13日(水)",                          // 14: Ed
        u"23:58:59.123",                     // 15: jmmssSSS
        u"23:58",                            // 16: JJmm
    };

#if APPLE_ICU_CHANGES
// rdar://16482189&16567439 chinese calendar date formats: Use rU or r (not y), drop 日 in zh formats
    UnicodeString patternResults_zh_chinese_hanidays[] = { // 12 Apple add
        // zh@calendar=chinese,numbers=hanidays                           // 10 zh@calendar=chinese,numbers=hanidays
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u51AC\\u6708"),  // 00: yMMM
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u51AC\\u6708"),  // 01: yMMM
        CharsToUnicodeString("1998\\u5E74\\u51AC\\u6708\\u5EFF\\u516D"),  // 02: yMMMd
        CharsToUnicodeString("1998\\u5E74\\u51AC\\u6708\\u5EFF\\u516D"),  // 03: yMMMd
        CharsToUnicodeString("11-\\u5EFF\\u516D"),                        // 04: Md		// hmm
        CharsToUnicodeString("\\u51AC\\u6708\\u5EFF\\u516D"),             // 05: MMMd
        CharsToUnicodeString("\\u51AC\\u6708\\u5EFF\\u516D"),             // 06: MMMMd
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u7b2c\\u56db\\u5B63\\u5EA6"), // 07: yQQQ
        CharsToUnicodeString("\\u4E0B\\u534811:58"),                      // 08: hhmm
        UnicodeString("23:58"),                                           // 09: HHmm
        CharsToUnicodeString("23:58"),                      // 10: jjmm
        UnicodeString("58:59"),                                           // 11: mmss
        CharsToUnicodeString("1998\\u620A\\u5BC5\\u5E74\\u51AC\\u6708"),  // 12: yyyyMMMM
        CharsToUnicodeString("\\u51AC\\u6708\\u5EFF\\u516D\\u5468\\u4E09"), // 13: MMMEd
        CharsToUnicodeString("\\u5EFF\\u516D\\u5468\\u4E09"),             // 14: Ed -> dE
        CharsToUnicodeString("23:58:59.123"),               // 15: jmmssSS
        UnicodeString("23:58"),                                           // 16: JJmm
    };

    UnicodeString patternResults_ar[] = { // 13 Apple add (updated for rdar://116185298 to use Western digits)
        // ar                                                                                                             // 11 ar, for Weds, Jan 13, 1999, 23:58:59
        CharsToUnicodeString("1\\u200F/1999"),                                                                            // 00: yM
        CharsToUnicodeString("\\u064A\\u0646\\u0627\\u064A\\u0631 1999"),                                                 // 01: yMMM
        CharsToUnicodeString("13\\u200F/1\\u200F/1999"),                                                                  // 02: yMd
        CharsToUnicodeString("13 \\u064A\\u0646\\u0627\\u064A\\u0631\\u060C 1999"),                                       // 03: yMMMd
        CharsToUnicodeString("13\\u200F/1"),                                                                              // 04: Md
        CharsToUnicodeString("13 \\u064A\\u0646\\u0627\\u064A\\u0631"),                                                   // 05: MMMd
        CharsToUnicodeString("13 \\u064A\\u0646\\u0627\\u064A\\u0631"),                                                   // 06: MMMMd
        CharsToUnicodeString("\\u0627\\u0644\\u0631\\u0628\\u0639 \\u0627\\u0644\\u0623\\u0648\\u0644 1999"),             // 07: yQQQ
        CharsToUnicodeString("11:58\\u00A0\\u0645"),                                                                      // 08: hhmm
        CharsToUnicodeString("23:58"),                                                                                    // 09: HHmm
        CharsToUnicodeString("11:58\\u00A0\\u0645"),                                                                      // 10: jjmm
        CharsToUnicodeString("58:59"),                                                                                    // 11: mmss
        CharsToUnicodeString("\\u064A\\u0646\\u0627\\u064A\\u0631 1999"),                                                 // 12: yyyyMMMM
        CharsToUnicodeString("\\u0623\\u0631\\u0628\\u0639\\u0627\\u0621\\u060C 13 \\u064A\\u0646\\u0627\\u064A\\u0631"), // 13: MMMEd
        CharsToUnicodeString("\\u0623\\u0631\\u0628\\u0639\\u0627\\u0621\\u060C 13"),                                     // 14: Ed
        CharsToUnicodeString("11:58:59.123\\u00A0\\u0645"),                                                               // 15: jmmssSSS
        CharsToUnicodeString("11:58"),                                                                                    // 16: JJmm
    };

    UnicodeString patternResults_en_ID_buddhist[] = { // 14 Apple add <rdar://problem/37505940>
        // en_ID@calendar=buddhist           // 14 en_ID@calendar=buddhist
        u"01/2542 BE",                       // 00: yM
        u"Jan 2542 BE",                      // 01: yMMM
        u"13/01/2542 BE",                    // 02: yMd
        u"13 Jan 2542 BE",                   // 03: yMMMd
        u"13/01",                            // 04: Md
        u"13 Jan",                           // 05: MMMd
        u"13 January",                       // 06: MMMMd
        u"Q1 2542 BE",                       // 07: yQQQ
        u"11.58\u202FPM",                    // 08: hhmm
        u"23.58",                            // 09: HHmm
        u"23.58",                            // 10: jjmm
        u"58.59",                            // 11: mmss
        u"January 2542 BE",                  // 12: yyyyMMMM
        u"Wed, 13 Jan",                      // 13: MMMEd
        u"Wed, 13",                          // 14: Ed
        u"23.58.59,123",                     // 15: jmmssSSS
        u"23.58",                            // 16: JJmm
    };
#endif  // APPLE_ICU_CHANGES

    UnicodeString* patternResults[] = {
        patternResults_en_US, // 0
        patternResults_en_US_japanese, // 1
        patternResults_de_DE, // 2
        patternResults_fi, // 3
        patternResults_es, // 4
        patternResults_ja, // 5
        patternResults_ja_japanese, // 6
        patternResults_zh_Hans_CN, // 7
        patternResults_zh_TW_roc, // 8
        patternResults_ru, // 9
        patternResults_zh_chinese, // 10
        patternResults_ja_jp_traditional, // 11
#if APPLE_ICU_CHANGES
// rdar://
        patternResults_zh_chinese_hanidays, // 12 Apple add
        patternResults_ar, // 13 Apple add
        patternResults_en_ID_buddhist, // 14 Apple add
#endif  // APPLE_ICU_CHANGES
    };

    UnicodeString patternTests2[] = {
        UnicodeString("yyyyMMMdd"),
        UnicodeString("yyyyqqqq"),
        UnicodeString("yMMMdd"),
        UnicodeString("EyyyyMMMdd"),
        UnicodeString("yyyyMMdd"),
        UnicodeString("yyyyMMM"),
        UnicodeString("yyyyMM"),
        UnicodeString("yyMM"),
        UnicodeString("yMMMMMd"),
        UnicodeString("EEEEEMMMMMd"),
        UnicodeString("MMMd"),
        UnicodeString("MMMdhmm"),
        UnicodeString("EMMMdhmms"),
        UnicodeString("MMdhmm"),
        UnicodeString("EEEEMMMdhmms"),
        UnicodeString("yyyyMMMddhhmmss"),
        UnicodeString("EyyyyMMMddhhmmss"),
        UnicodeString("hmm"),
        UnicodeString("hhmm"),
        UnicodeString("hhmmVVVV"),
        UnicodeString(""),
    };
    UnicodeString patternResults2[] = {
        UnicodeString("Oct 14, 1999"),
        UnicodeString("4th quarter 1999"),
        UnicodeString("Oct 14, 1999"),
        UnicodeString("Thu, Oct 14, 1999"),
        UnicodeString("10/14/1999"),
        UnicodeString("Oct 1999"),
        UnicodeString("10/1999"),
        UnicodeString("10/99"),
        UnicodeString("O 14, 1999"),
        UnicodeString("T, O 14"),
        UnicodeString("Oct 14"),
#if APPLE_ICU_CHANGES
// rdar://
        UnicodeString("Oct 14 at 6:58\u202FAM"),
        UnicodeString("Thu, Oct 14 at 6:58:59\u202FAM"),
#else
        UnicodeString(u"Oct 14, 6:58\u202FAM", -1),
        UnicodeString(u"Thu, Oct 14, 6:58:59\u202FAM", -1),
#endif  // APPLE_ICU_CHANGES
        UnicodeString(u"10/14, 6:58\u202FAM", -1),
#if APPLE_ICU_CHANGES
// rdar://
        UnicodeString("Thursday, Oct 14 at 6:58:59\u202FAM"),
        UnicodeString("Oct 14, 1999 at 6:58:59\u202FAM"),
        UnicodeString("Thu, Oct 14, 1999 at 6:58:59\u202FAM"),
#else
        UnicodeString(u"Thursday, Oct 14, 6:58:59\u202FAM", -1),
        UnicodeString(u"Oct 14, 1999, 6:58:59\u202FAM", -1),
        UnicodeString(u"Thu, Oct 14, 1999, 6:58:59\u202FAM", -1),
#endif  // APPLE_ICU_CHANGES
        UnicodeString(u"6:58\u202FAM", -1),
        UnicodeString(u"6:58\u202FAM", -1),
        UnicodeString(u"6:58\u202FAM GMT", -1),
        UnicodeString(""),
    };

    // results for getSkeletons() and getPatternForSkeleton()
    const UnicodeString testSkeletonsResults[] = {
        UnicodeString("HH:mm"),
        UnicodeString("MMMMd"),
        UnicodeString("MMMMMdd"),
    };

    const UnicodeString testBaseSkeletonsResults[] = {
        UnicodeString("Hm"),
        UnicodeString("MMMMd"),
        UnicodeString("MMMMMd"),
    };

    const char* testGetSkeletonAndBase[][3] = {
        // pattern       skeleton    baseSkeleton
        { "dd-MMM",     "MMMdd",    "MMMd" },
        { "dd/MMMM/yy", "yyMMMMdd", "yMMMMd" },
        { "h",          "h",        "h" },
        { "ah",         "ah",       "ah" },
        { "aaaah",      "aaaah",    "aaaah" },
        { "Bh",         "Bh",       "Bh" }
    };

    UnicodeString newDecimal(" "); // space
    UnicodeString newAppendItemName("hrs.");
    UnicodeString newAppendItemFormat("{1} {0}");
    UnicodeString newDateTimeFormat("{1} {0}");
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString conflictingPattern;
    UDateTimePatternConflict conflictingStatus = UDATPG_NO_CONFLICT;
    (void)conflictingStatus;   // Suppress set but not used warning.

    // ======= Test CreateInstance with default locale
    logln("Testing DateTimePatternGenerator createInstance from default locale");

    DateTimePatternGenerator *instFromDefaultLocale=DateTimePatternGenerator::createInstance(status);
    if (U_FAILURE(status)) {
        dataerrln("ERROR: Could not create DateTimePatternGenerator (default) - exiting");
        return;
    }
    else {
        delete instFromDefaultLocale;
    }

    // ======= Test CreateInstance with given locale
    logln("Testing DateTimePatternGenerator createInstance from French locale");
    status = U_ZERO_ERROR;
    DateTimePatternGenerator *instFromLocale=DateTimePatternGenerator::createInstance(Locale::getFrench(), status);
    if (U_FAILURE(status)) {
        dataerrln("ERROR: Could not create DateTimePatternGenerator (Locale::getFrench()) - exiting");
        return;
    }

    // ======= Test clone DateTimePatternGenerator
    logln("Testing DateTimePatternGenerator::clone()");
    status = U_ZERO_ERROR;


    UnicodeString decimalSymbol = instFromLocale->getDecimal();
    UnicodeString newDecimalSymbol = UnicodeString("*");
    decimalSymbol = instFromLocale->getDecimal();
    instFromLocale->setDecimal(newDecimalSymbol);
    DateTimePatternGenerator *cloneDTPatternGen=instFromLocale->clone();
    decimalSymbol = cloneDTPatternGen->getDecimal();
    if (decimalSymbol != newDecimalSymbol) {
        errln("ERROR: inconsistency is found in cloned object.");
    }
    if ( !(*cloneDTPatternGen == *instFromLocale) ) {
        errln("ERROR: inconsistency is found in cloned object.");
    }

    if ( *cloneDTPatternGen != *instFromLocale ) {
        errln("ERROR: inconsistency is found in cloned object.");
    }

    delete instFromLocale;
    delete cloneDTPatternGen;

    // ======= Test simple use cases
    logln("Testing simple use cases");
    status = U_ZERO_ERROR;
    Locale deLocale=Locale::getGermany();
    UDate sampleDate=LocaleTest::date(99, 9, 13, 23, 58, 59);
    DateTimePatternGenerator *gen = DateTimePatternGenerator::createInstance(deLocale, status);
    if (U_FAILURE(status)) {
        dataerrln("ERROR: Could not create DateTimePatternGenerator (Locale::getGermany()) - exiting");
        return;
    }
    UnicodeString findPattern = gen->getBestPattern(UnicodeString("MMMddHmm"), status);
    SimpleDateFormat *format = new SimpleDateFormat(findPattern, deLocale, status);
    if (U_FAILURE(status)) {
        dataerrln("ERROR: Could not create SimpleDateFormat (Locale::getGermany())");
        delete gen;
        return;
    }
    TimeZone *zone = TimeZone::createTimeZone(UnicodeString("ECT"));
    if (zone==nullptr) {
        dataerrln("ERROR: Could not create TimeZone ECT");
        delete gen;
        delete format;
        return;
    }
    format->setTimeZone(*zone);
    UnicodeString dateReturned, expectedResult;
    dateReturned.remove();
    dateReturned = format->format(sampleDate, dateReturned, status);
    expectedResult=UnicodeString("14. Okt., 08:58", -1, US_INV);
    if ( dateReturned != expectedResult ) {
        errln("ERROR: Simple test in getBestPattern with Locale::getGermany()).");
    }
    // add new pattern
    status = U_ZERO_ERROR;
    conflictingStatus = gen->addPattern(UnicodeString("d'. von' MMMM", -1, US_INV), true, conflictingPattern, status);
    if (U_FAILURE(status)) {
        errln("ERROR: Could not addPattern - d\'. von\' MMMM");
    }
    status = U_ZERO_ERROR;
    UnicodeString testPattern=gen->getBestPattern(UnicodeString("MMMMdd"), status);
    testPattern=gen->getBestPattern(UnicodeString("MMMddHmm"), status);
    format->applyPattern(gen->getBestPattern(UnicodeString("MMMMdHmm"), status));
    dateReturned.remove();
    dateReturned = format->format(sampleDate, dateReturned, status);
    expectedResult=UnicodeString("14. von Oktober um 08:58", -1, US_INV);
    if ( dateReturned != expectedResult ) {
        errln(UnicodeString("ERROR: Simple test addPattern failed!: d\'. von\' MMMM   Got: ") + dateReturned + UnicodeString(" Expected: ") + expectedResult);
    }
    delete format;

    // get a pattern and modify it
    format = dynamic_cast<SimpleDateFormat*>(DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull,
                                                                  deLocale));
    format->setTimeZone(*zone);
    UnicodeString pattern;
    pattern = format->toPattern(pattern);
    dateReturned.remove();
    dateReturned = format->format(sampleDate, dateReturned, status);
    expectedResult=CharsToUnicodeString("Donnerstag, 14. Oktober 1999 um 08:58:59 Mitteleurop\\u00E4ische Sommerzeit");
    if ( dateReturned != expectedResult ) {
        errln("ERROR: Simple test uses full date format.");
        errln(UnicodeString(" Got: ") + dateReturned + UnicodeString(" Expected: ") + expectedResult);
    }

    // modify it to change the zone.
    UnicodeString newPattern = gen->replaceFieldTypes(pattern, UnicodeString("vvvv"), status);
    format->applyPattern(newPattern);
    dateReturned.remove();
    dateReturned = format->format(sampleDate, dateReturned, status);
    expectedResult=CharsToUnicodeString("Donnerstag, 14. Oktober 1999 um 08:58:59 Mitteleurop\\u00E4ische Zeit");
    if ( dateReturned != expectedResult ) {
        errln("ERROR: Simple test modify the timezone!");
        errln(UnicodeString(" Got: ")+ dateReturned + UnicodeString(" Expected: ") + expectedResult);
    }

    // setDeciaml(), getDeciaml()
    gen->setDecimal(newDecimal);
    if (newDecimal != gen->getDecimal()) {
        errln("ERROR: unexpected result from setDecimal() and getDecimal()!.\n");
    }

    // setAppenItemName() , getAppendItemName()
    gen->setAppendItemName(UDATPG_HOUR_FIELD, newAppendItemName);
    if (newAppendItemName != gen->getAppendItemName(UDATPG_HOUR_FIELD)) {
        errln("ERROR: unexpected result from setAppendItemName() and getAppendItemName()!.\n");
    }

    // setAppenItemFormat() , getAppendItemFormat()
    gen->setAppendItemFormat(UDATPG_HOUR_FIELD, newAppendItemFormat);
    if (newAppendItemFormat != gen->getAppendItemFormat(UDATPG_HOUR_FIELD)) {
        errln("ERROR: unexpected result from setAppendItemFormat() and getAppendItemFormat()!.\n");
    }

    // setDateTimeFormat() , getDateTimeFormat()
    gen->setDateTimeFormat(newDateTimeFormat);
    if (newDateTimeFormat != gen->getDateTimeFormat()) {
        errln("ERROR: unexpected result from setDateTimeFormat() and getDateTimeFormat()!.\n");
    }

    // ======== Test getSkeleton and getBaseSkeleton
    
    int32_t i, count = UPRV_LENGTHOF(testGetSkeletonAndBase);
    for (i = 0; i < count; i++) {
        status = U_ZERO_ERROR;
        pattern                            = UnicodeString(testGetSkeletonAndBase[i][0]);
        UnicodeString expectedSkeleton     = UnicodeString(testGetSkeletonAndBase[i][1]);
        UnicodeString expectedBaseSkeleton = UnicodeString(testGetSkeletonAndBase[i][2]);
        UnicodeString retSkeleton = gen->getSkeleton(pattern, status);
		if(U_FAILURE(status) || retSkeleton != expectedSkeleton ) {
			 errln("ERROR: Unexpected result from getSkeleton().\n");
			 errln(UnicodeString(" Got: ") + retSkeleton + UnicodeString(" Expected: ") + expectedSkeleton );
		}
		retSkeleton = gen->getBaseSkeleton(pattern, status);
		if(U_FAILURE(status) || retSkeleton !=  expectedBaseSkeleton) {
			 errln("ERROR: Unexpected result from getBaseSkeleton().\n");
			 errln(UnicodeString(" Got: ") + retSkeleton + UnicodeString(" Expected:")+ expectedBaseSkeleton);
		}
    }

    delete format;
    delete zone;
    delete gen;

    {
        // Trac# 6104
        status = U_ZERO_ERROR;
        pattern = UnicodeString("YYYYMMM");
        UnicodeString expR = CharsToUnicodeString("1999\\u5E741\\u6708"); // fixed expected result per ticket:6626:
        Locale loc("ja");
        UDate testDate1= LocaleTest::date(99, 0, 13, 23, 58, 59);
        DateTimePatternGenerator *patGen=DateTimePatternGenerator::createInstance(loc, status);
        if(U_FAILURE(status)) {
            dataerrln("ERROR: Could not create DateTimePatternGenerator");
            return;
        }
        UnicodeString bPattern = patGen->getBestPattern(pattern, status);
        UnicodeString rDate;
        SimpleDateFormat sdf(bPattern, loc, status);
        rDate.remove();
        rDate = sdf.format(testDate1, rDate);

        logln(UnicodeString(" ja locale with skeleton: YYYYMMM  Best Pattern:") + bPattern);
        logln(UnicodeString("  Formatted date:") + rDate);

        if ( expR!= rDate ) {
            errln(UnicodeString("\nERROR: Test Japanese month hack Got: ") + rDate +
                  UnicodeString(" Expected: ") + expR );
        }

        delete patGen;
    }
    {   // Trac# 6104
        Locale loc("zh");
        UnicodeString expR = CharsToUnicodeString("1999\\u5E741\\u6708"); // fixed expected result per ticket:6626:
        UDate testDate1= LocaleTest::date(99, 0, 13, 23, 58, 59);
        DateTimePatternGenerator *patGen=DateTimePatternGenerator::createInstance(loc, status);
        if(U_FAILURE(status)) {
            dataerrln("ERROR: Could not create DateTimePatternGenerator");
            return;
        }
        UnicodeString bPattern = patGen->getBestPattern(pattern, status);
        UnicodeString rDate;
        SimpleDateFormat sdf(bPattern, loc, status);
        rDate.remove();
        rDate = sdf.format(testDate1, rDate);

        logln(UnicodeString(" zh locale with skeleton: YYYYMMM  Best Pattern:") + bPattern);
        logln(UnicodeString("  Formatted date:") + rDate);
        if ( expR!= rDate ) {
            errln(UnicodeString("\nERROR: Test Chinese month hack Got: ") + rDate +
                  UnicodeString(" Expected: ") + expR );
        }
        delete patGen;
    }

    {
         // Trac# 6172 duplicate time pattern
         status = U_ZERO_ERROR;
         pattern = UnicodeString("hmv");
         UnicodeString expR = UnicodeString(u"h:mm\u202Fa v", -1);
         Locale loc("en");
         DateTimePatternGenerator *patGen=DateTimePatternGenerator::createInstance(loc, status);
         if(U_FAILURE(status)) {
             dataerrln("ERROR: Could not create DateTimePatternGenerator");
             return;
         }
         UnicodeString bPattern = patGen->getBestPattern(pattern, status);
         logln(UnicodeString(" en locale with skeleton: hmv  Best Pattern:") + bPattern);

         if ( expR!= bPattern ) {
             errln(UnicodeString("\nERROR: Test EN time format Got: ") + bPattern +
                   UnicodeString(" Expected: ") + expR );
         }

         delete patGen;
     }


    // ======= Test various skeletons.
    logln("Testing DateTimePatternGenerator with various skeleton");

    status = U_ZERO_ERROR;
    int32_t localeIndex=0;
    int32_t resultIndex=0;
    UnicodeString resultDate;
    UDate testDate= LocaleTest::date(99, 0, 13, 23, 58, 59) + 123.0;
    while (localeIndex < MAX_LOCALE )
    {
        resultIndex=0;
        int32_t dataIndex=0;
        UnicodeString bestPattern;

        Locale loc(testLocale[localeIndex][0], testLocale[localeIndex][1], testLocale[localeIndex][2], testLocale[localeIndex][3]);
        logln("\n\n Locale: %s_%s_%s@%s", testLocale[localeIndex][0], testLocale[localeIndex][1], testLocale[localeIndex][2], testLocale[localeIndex][3]);
        DateTimePatternGenerator *patGen=DateTimePatternGenerator::createInstance(loc, status);
        if(U_FAILURE(status)) {
            dataerrln("ERROR: Could not create DateTimePatternGenerator with locale index:%d . - exiting\n", localeIndex);
            return;
        }
        while (patternData[dataIndex].length() > 0) {
            log(patternData[dataIndex]);
            bestPattern = patGen->getBestPattern(patternData[dataIndex++], status);
            logln(UnicodeString(" -> ") + bestPattern);

            SimpleDateFormat sdf(bestPattern, loc, status);
            resultDate.remove();
            resultDate = sdf.format(testDate, resultDate);
            if ( resultDate != patternResults[localeIndex][resultIndex] ) {
                const auto* calendar = sdf.getCalendar();
                errln(UnicodeString("\nERROR: Test various skeletons[") + (dataIndex-1) + UnicodeString("], localeIndex ") + localeIndex +
                      u". Got: \"" + resultDate +
                      u"\" with calendar " + calendar->getType() +
                      u" Expected: \"" + patternResults[localeIndex][resultIndex] + u"\"");
            }

            resultIndex++;
        }
        delete patGen;
        localeIndex++;
    }

    // ======= More tests ticket#6110
    logln("Testing DateTimePatternGenerator with various skeleton");

    status = U_ZERO_ERROR;
    localeIndex=0;
    resultIndex=0;
    testDate= LocaleTest::date(99, 9, 13, 23, 58, 59);
    {
        int32_t dataIndex=0;
        UnicodeString bestPattern;
        logln("\n\n Test various skeletons for English locale...");
        DateTimePatternGenerator *patGen=DateTimePatternGenerator::createInstance(Locale::getEnglish(), status);
        if(U_FAILURE(status)) {
            dataerrln("ERROR: Could not create DateTimePatternGenerator with locale English . - exiting\n");
            return;
        }
        TimeZone *enZone = TimeZone::createTimeZone(UnicodeString("ECT/GMT"));
        if (enZone==nullptr) {
            dataerrln("ERROR: Could not create TimeZone ECT");
            delete patGen;
            return;
        }
        SimpleDateFormat *enFormat = dynamic_cast<SimpleDateFormat*>(DateFormat::createDateTimeInstance(DateFormat::kFull,
                         DateFormat::kFull, Locale::getEnglish()));
        enFormat->setTimeZone(*enZone);
        while (patternTests2[dataIndex].length() > 0) {
            logln(patternTests2[dataIndex]);
            bestPattern = patGen->getBestPattern(patternTests2[dataIndex], status);
            logln(UnicodeString(" -> ") + bestPattern);
            enFormat->applyPattern(bestPattern);
            resultDate.remove();
            resultDate = enFormat->format(testDate, resultDate);
            if ( resultDate != patternResults2[resultIndex] ) {
                errln(UnicodeString("\nERROR: Test various skeletons[") + dataIndex
                    + UnicodeString("]. Got: ") + resultDate + UnicodeString(" Expected: ") +
                    patternResults2[resultIndex] );
            }
            dataIndex++;
            resultIndex++;
        }
        delete patGen;
        delete enZone;
        delete enFormat;
    }



    // ======= Test random skeleton
    DateTimePatternGenerator *randDTGen= DateTimePatternGenerator::createInstance(status);
    if (U_FAILURE(status)) {
        dataerrln("ERROR: Could not create DateTimePatternGenerator (Locale::getFrench()) - exiting");
        return;
    }
    char16_t newChar;
    for (i=0; i<10; ++i) {
        UnicodeString randomSkeleton;
        int32_t len = rand() % 20;
        for (int32_t j=0; j<len; ++j ) {
            while ((newChar = static_cast<char16_t>(rand() % 0x7f)) >= static_cast<char16_t>(0x20)) {
                randomSkeleton += newChar;
            }
        }
        UnicodeString bestPattern = randDTGen->getBestPattern(randomSkeleton, status);
    }
    delete randDTGen;

    // UnicodeString randomString=Unicode
    // ======= Test getStaticClassID()

    logln("Testing getStaticClassID()");
    status = U_ZERO_ERROR;
    DateTimePatternGenerator *test= DateTimePatternGenerator::createInstance(status);

    if(test->getDynamicClassID() != DateTimePatternGenerator::getStaticClassID()) {
        errln("ERROR: getDynamicClassID() didn't return the expected value");
    }
    delete test;

    // ====== Test createEmptyInstance()

    logln("Testing createEmptyInstance()");
    status = U_ZERO_ERROR;

    test = DateTimePatternGenerator::createEmptyInstance(status);
    if(U_FAILURE(status)) {
         errln("ERROR: Fail to create an empty instance ! - exiting.\n");
         delete test;
         return;
    }

    conflictingStatus = test->addPattern(UnicodeString("MMMMd"), true, conflictingPattern, status);
    status = U_ZERO_ERROR;
    testPattern=test->getBestPattern(UnicodeString("MMMMdd"), status);
    conflictingStatus = test->addPattern(UnicodeString("HH:mm"), true, conflictingPattern, status);
    conflictingStatus = test->addPattern(UnicodeString("MMMMMdd"), true, conflictingPattern, status); //duplicate pattern
    StringEnumeration *output=nullptr;
    output = test->getRedundants(status);
    expectedResult=UnicodeString("MMMMd");
    if (output != nullptr) {
        output->reset(status);
        const UnicodeString *dupPattern=output->snext(status);
        if ( (dupPattern==nullptr) || (*dupPattern != expectedResult) ) {
            errln("ERROR: Fail in getRedundants !\n");
        }
    }

    // ======== Test getSkeletons and getBaseSkeletons
    StringEnumeration* ptrSkeletonEnum = test->getSkeletons(status);
    if(U_FAILURE(status)) {
        errln("ERROR: Fail to get skeletons !\n");
    }
    UnicodeString returnPattern, *ptrSkeleton;
    ptrSkeletonEnum->reset(status);
    count=ptrSkeletonEnum->count(status);
    for (i=0; i<count; ++i) {
        ptrSkeleton = const_cast<UnicodeString *>(ptrSkeletonEnum->snext(status));
        returnPattern = test->getPatternForSkeleton(*ptrSkeleton);
        if ( returnPattern != testSkeletonsResults[i] ) {
            errln(UnicodeString("ERROR: Unexpected result from getSkeletons and getPatternForSkeleton\nGot: ") + returnPattern
               + UnicodeString("\nExpected: ") + testSkeletonsResults[i]
               + UnicodeString("\n"));
        }
    }
    StringEnumeration* ptrBaseSkeletonEnum = test->getBaseSkeletons(status);
    if(U_FAILURE(status)) {
        errln("ERROR: Fail to get base skeletons !\n");
    }
    count=ptrBaseSkeletonEnum->count(status);
    for (i=0; i<count; ++i) {
        ptrSkeleton = const_cast<UnicodeString *>(ptrBaseSkeletonEnum->snext(status));
        if ( *ptrSkeleton != testBaseSkeletonsResults[i] ) {
            errln("ERROR: Unexpected result from getBaseSkeletons() !\n");
        }
    }

    // ========= DateTimePatternGenerator sample code in Userguide
    // set up the generator
    Locale locale = Locale::getFrench();
    status = U_ZERO_ERROR;
    DateTimePatternGenerator *generator = DateTimePatternGenerator::createInstance( locale, status);

    // get a pattern for an abbreviated month and day
    pattern = generator->getBestPattern(UnicodeString("MMMd"), status);
    SimpleDateFormat formatter(pattern, locale, status);

    zone = TimeZone::createTimeZone(UnicodeString("GMT"));
    formatter.setTimeZone(*zone);
    // use it to format (or parse)
    UnicodeString formatted;
    formatted = formatter.format(Calendar::getNow(), formatted, status);
    // for French, the result is "13 sept."
    formatted.remove();
    // cannot use the result from getNow() because the value change evreyday.
    testDate= LocaleTest::date(99, 0, 13, 23, 58, 59);
    formatted = formatter.format(testDate, formatted, status);
    expectedResult=UnicodeString("14 janv.");
    if ( formatted != expectedResult ) {
        errln("ERROR: Userguide sample code result!");
        errln(UnicodeString(" Got: ")+ formatted + UnicodeString(" Expected: ") + expectedResult);
    }

    delete zone;
    delete output;
    delete ptrSkeletonEnum;
    delete ptrBaseSkeletonEnum;
    delete test;
    delete generator;
}

/**
 * Test handling of options
 *
 * For reference, as of ICU 4.3.3,
 *  root/gregorian has
 *      Hm{"H:mm"}
 *      Hms{"H:mm:ss"}
 *      hm{"h:mm a"}
 *      hms{"h:mm:ss a"}
 *  en/gregorian has
 *      Hm{"H:mm"}
 *      Hms{"H:mm:ss"}
 *      hm{"h:mm a"}
 *  be/gregorian has
 *      HHmmss{"HH.mm.ss"}
 *      Hm{"HH.mm"}
 *      hm{"h.mm a"}
 *      hms{"h.mm.ss a"}
 */
typedef struct DTPtnGenOptionsData {
    const char *locale;
    const char *skel;
    const char16_t *expectedPattern;
    UDateTimePatternMatchOptions    options;
} DTPtnGenOptionsData;
void IntlTestDateTimePatternGeneratorAPI::testOptions(/*char *par*/)
{
    DTPtnGenOptionsData testData[] = {
    //   locale  skel   expectedPattern  options
        { "en", "Hmm",  u"HH:mm",        UDATPG_MATCH_NO_OPTIONS        },
        { "en", "HHmm", u"HH:mm",        UDATPG_MATCH_NO_OPTIONS        },
        { "en", "hhmm", u"h:mm\u202Fa",  UDATPG_MATCH_NO_OPTIONS        },
        { "en", "Hmm",  u"HH:mm",        UDATPG_MATCH_HOUR_FIELD_LENGTH },
        { "en", "HHmm", u"HH:mm",        UDATPG_MATCH_HOUR_FIELD_LENGTH },
        { "en", "hhmm", u"hh:mm\u202Fa", UDATPG_MATCH_HOUR_FIELD_LENGTH },
        { "da", "Hmm",  u"HH.mm",        UDATPG_MATCH_NO_OPTIONS        },
        { "da", "HHmm", u"HH.mm",        UDATPG_MATCH_NO_OPTIONS        },
        { "da", "hhmm", u"h.mm\u202Fa",  UDATPG_MATCH_NO_OPTIONS        },
        { "da", "Hmm",  u"H.mm",         UDATPG_MATCH_HOUR_FIELD_LENGTH },
        { "da", "HHmm", u"HH.mm",        UDATPG_MATCH_HOUR_FIELD_LENGTH },
        { "da", "hhmm", u"hh.mm\u202Fa", UDATPG_MATCH_HOUR_FIELD_LENGTH },
        //
        { "en",                   "yyyy",  u"yyyy",  UDATPG_MATCH_NO_OPTIONS },
        { "en",                   "YYYY",  u"YYYY",  UDATPG_MATCH_NO_OPTIONS },
        { "en",                   "U",     u"y",     UDATPG_MATCH_NO_OPTIONS },

// rdar://113679874 remove commas for EMMMMd format for en_GB, confirm it's different to en_001
#if APPLE_ICU_CHANGES
        { "en_001",                  "EMMMMd",  u"EEE, d MMMM",  UDATPG_MATCH_NO_OPTIONS },
        { "en_GB",                   "EMMMMd",  u"EEE d MMMM",   UDATPG_MATCH_NO_OPTIONS },
#else
        { "en_001",                  "EMMMMd",  u"EEE, d MMMM",  UDATPG_MATCH_NO_OPTIONS },
        { "en_GB",                   "EMMMMd",  u"EEE, d MMMM",  UDATPG_MATCH_NO_OPTIONS },
#endif  // APPLE_ICU_CHANGES

#if APPLE_ICU_CHANGES
// rdar://69457523 en/de/es/fr, japanese cal date formats should use G y or GGGGGy+
        { "en@calendar=japanese", "yyyy",  u"G y",   UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=japanese", "YYYY",  u"G Y",   UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=japanese", "U",     u"G y",   UDATPG_MATCH_NO_OPTIONS },
#else
        { "en@calendar=japanese", "yyyy",  u"y G",   UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=japanese", "YYYY",  u"Y G",   UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=japanese", "U",     u"y G",   UDATPG_MATCH_NO_OPTIONS },
#endif  // APPLE_ICU_CHANGES
        { "en@calendar=chinese",  "yyyy",  u"r(U)",  UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=chinese",  "YYYY",  u"Y(Y)",  UDATPG_MATCH_NO_OPTIONS }, // not a good result, want r(Y) or r(U)
        { "en@calendar=chinese",  "U",     u"r(U)",   UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=chinese",  "Gy",    u"r(U)",   UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=chinese",  "GU",    u"r(U)",   UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=chinese",  "ULLL",  u"MMM U",  UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=chinese",  "yMMM",  u"MMM r",  UDATPG_MATCH_NO_OPTIONS },
        { "en@calendar=chinese",  "GUMMM", u"MMM r",  UDATPG_MATCH_NO_OPTIONS },
#if APPLE_ICU_CHANGES
// rdar://
        { "zh@calendar=chinese",  "yyyy",  u"U年",    UDATPG_MATCH_NO_OPTIONS },
        { "zh@calendar=chinese",  "YYYY",  u"Y年",    UDATPG_MATCH_NO_OPTIONS }, // not a good result, may want r(Y) or r(U)
        { "zh@calendar=chinese",  "U",     u"U年",    UDATPG_MATCH_NO_OPTIONS },
#else
        { "zh@calendar=chinese",  "yyyy",  u"rU年",    UDATPG_MATCH_NO_OPTIONS },
        { "zh@calendar=chinese",  "YYYY",  u"YY年",    UDATPG_MATCH_NO_OPTIONS }, // not a good result, may want r(Y) or r(U)
        { "zh@calendar=chinese",  "U",     u"rU年",    UDATPG_MATCH_NO_OPTIONS },
#endif  // APPLE_ICU_CHANGES
        { "zh@calendar=chinese",  "Gy",    u"rU年",    UDATPG_MATCH_NO_OPTIONS },
        { "zh@calendar=chinese",  "GU",    u"rU年",    UDATPG_MATCH_NO_OPTIONS },
        { "zh@calendar=chinese",  "ULLL",  u"U年MMM",  UDATPG_MATCH_NO_OPTIONS },
        { "zh@calendar=chinese",  "yMMM",  u"rU年MMM", UDATPG_MATCH_NO_OPTIONS },
        { "zh@calendar=chinese",  "GUMMM", u"rU年MMM", UDATPG_MATCH_NO_OPTIONS },
    };

    int count = UPRV_LENGTHOF(testData);
    const DTPtnGenOptionsData * testDataPtr = testData;

    for (; count-- > 0; ++testDataPtr) {
        UErrorCode status = U_ZERO_ERROR;

        Locale locale(testDataPtr->locale);
        UnicodeString skel(testDataPtr->skel);
        UnicodeString expectedPattern(testDataPtr->expectedPattern, -1);
        UDateTimePatternMatchOptions options = testDataPtr->options;

        DateTimePatternGenerator * dtpgen = DateTimePatternGenerator::createInstance(locale, status);
        if (U_FAILURE(status)) {
            dataerrln("Unable to create DateTimePatternGenerator instance for locale(%s): %s", locale.getName(), u_errorName(status));
            delete dtpgen;
            continue;
        }
        UnicodeString pattern = dtpgen->getBestPattern(skel, options, status);
        if (pattern.compare(expectedPattern) != 0) {
            errln( UnicodeString("ERROR in getBestPattern, locale ") + UnicodeString(testDataPtr->locale) +
                   UnicodeString(", skeleton ") + skel +
                   ((options)?UnicodeString(", options!=0"):UnicodeString(", options==0")) +
                   UnicodeString(", expected pattern ") + expectedPattern +
                   UnicodeString(", got ") + pattern );
        }
        delete dtpgen;
    }
}

/**
 * Test that DTPG can handle all valid pattern character / length combinations
 *
 */
#define FIELD_LENGTHS_COUNT 6
#define FIELD_LENGTH_MAX 8
#define MUST_INCLUDE_COUNT 5

typedef struct AllFieldsTestItem {
    char patternChar;
    int8_t fieldLengths[FIELD_LENGTHS_COUNT+1]; // up to FIELD_LENGTHS_COUNT lengths to try
                                                // (length <=FIELD_LENGTH_MAX) plus 0 terminator
    char mustIncludeOneOf[MUST_INCLUDE_COUNT+1];// resulting pattern must include at least one of
                                                // these as a pattern char (0-terminated list)
} AllFieldsTestItem;

void IntlTestDateTimePatternGeneratorAPI::testAllFieldPatterns(/*char *par*/)
{
    const char * localeNames[] = {
        "root",
        "root@calendar=japanese",
        "root@calendar=chinese",
        "en",
        "en@calendar=japanese",
        "en@calendar=chinese",
        nullptr // terminator
    };
    AllFieldsTestItem testData[] = {
        //pat   fieldLengths    generated pattern must
        //chr   to test         include one of these
        { 'G',  {1,2,3,4,5,0},  "G"    }, // era
        // year
        { 'y',  {1,2,3,4,0},    "yU"   }, // year
        { 'Y',  {1,2,3,4,0},    "Y"    }, // year for week of year
        { 'u',  {1,2,3,4,5,0},  "yuU"  }, // extended year
        { 'U',  {1,2,3,4,5,0},  "yU"   }, // cyclic year name
        // quarter
        { 'Q',  {1,2,3,4,0},    "Qq"   }, // x
        { 'q',  {1,2,3,4,0},    "Qq"   }, // standalone
        // month
        { 'M',  {1,2,3,4,5,0},  "ML"   }, // x
        { 'L',  {1,2,3,4,5,0},  "ML"   }, // standalone
        // week
        { 'w',  {1,2,0},        "w"    }, // week of year
        { 'W',  {1,0},          "W"    }, // week of month
        // day
        { 'd',  {1,2,0},        "d"    }, // day of month
        { 'D',  {1,2,3,0},      "D"    }, // day of year
        { 'F',  {1,0},          "F"    }, // day of week in month
        { 'g',  {7,0},          "g"    }, // modified julian day
        // weekday
        { 'E',  {1,2,3,4,5,6},  "Eec"  }, // day of week
        { 'e',  {1,2,3,4,5,6},  "Eec"  }, // local day of week
        { 'c',  {1,2,3,4,5,6},  "Eec"  }, // standalone local day of week
        // day period
        { 'a',  {1,2,3,4,5,0},  "a"    }, // am or pm
        { 'b',  {1,2,3,4,5,0},  "b"    }, // dayPeriod AM/PM/noon
        { 'B',  {1,2,3,4,5,0},  "B"    }, // dayPeriod ranges
        // hour
        { 'h',  {1,2,0},        "hK"   }, // 12 (1-12)
        { 'H',  {1,2,0},        "Hk"   }, // 24 (0-23)
        { 'K',  {1,2,0},        "hK"   }, // 12 (0-11)
        { 'k',  {1,2,0},        "Hk"   }, // 24 (1-24)
        { 'j',  {1,2,0},        "hHKk" }, // locale default
        { 'J',  {1,2,0},        "hHKk" }, // locale default, without any dayPeriod
        { 'C',  {1,2,0},        "hHKk" }, // locale allowed first entry, possibly with b or B
        // minute
        { 'm',  {1,2,0},        "m"    }, // x
        // second & fractions
        { 's',  {1,2,0},        "s"    }, // x
        { 'S',  {1,2,3,4,0},    "S"    }, // fractional second
        { 'A',  {8,0},          "A"    }, // milliseconds in day
        // zone
        { 'z',  {1,2,3,4,0},    "z"    }, // x
        { 'Z',  {1,2,3,4,5,0},  "Z"    }, // x
        { 'O',  {1,4,0},        "O"    }, // x
        { 'v',  {1,4,0},        "v"    }, // x
        { 'V',  {1,2,3,4,0},    "V"    }, // x
        { 'X',  {1,2,3,4,5,0},  "X"    }, // x
        { 'x',  {1,2,3,4,5,0},  "x"    }, // x
    };

    const char ** localeNamesPtr = localeNames;
    const char * localeName;
    while ( (localeName = *localeNamesPtr++) != nullptr) {
        UErrorCode status = U_ZERO_ERROR;
        Locale locale = Locale::createFromName(localeName);
        DateTimePatternGenerator * dtpg = DateTimePatternGenerator::createInstance(locale, status);
        if (U_SUCCESS(status)) {
            const AllFieldsTestItem * testDataPtr = testData;
            int itemCount = UPRV_LENGTHOF(testData);
            for (; itemCount-- > 0; ++testDataPtr) {
                char skelBuf[FIELD_LENGTH_MAX];
                int32_t chrIndx, lenIndx;
                for (chrIndx = 0; chrIndx < FIELD_LENGTH_MAX; chrIndx++) {
                    skelBuf[chrIndx] = testDataPtr->patternChar;
                }
                for (lenIndx = 0; lenIndx < FIELD_LENGTHS_COUNT; lenIndx++) {
                    int32_t skelLen = testDataPtr->fieldLengths[lenIndx];
                    if (skelLen <= 0) {
                        break;
                    }
                    if (skelLen > FIELD_LENGTH_MAX) {
                        continue;
                    }
                    UnicodeString skeleton(skelBuf, skelLen, US_INV);
                    UnicodeString pattern = dtpg->getBestPattern(skeleton, status);
                    if (U_FAILURE(status)) {
                        errln("DateTimePatternGenerator getBestPattern for locale %s, skelChar %c skelLength %d fails: %s",
                              locale.getName(), testDataPtr->patternChar, skelLen, u_errorName(status));
                    } else if (pattern.length() <= 0) {
                        errln("DateTimePatternGenerator getBestPattern for locale %s, skelChar %c skelLength %d produces 0-length pattern",
                              locale.getName(), testDataPtr->patternChar, skelLen);
                    } else {
                        // test that resulting pattern has at least one char in mustIncludeOneOf
                        UnicodeString mustIncludeOneOf(testDataPtr->mustIncludeOneOf, -1, US_INV);
                        int32_t patIndx, patLen = pattern.length();
                        UBool inQuoted = false;
                        for (patIndx = 0; patIndx < patLen; patIndx++) {
                            char16_t c = pattern.charAt(patIndx);
                            if (c == 0x27) {
                                inQuoted = !inQuoted;
                            } else if (!inQuoted && c <= 0x007A && c >= 0x0041) {
                                if (mustIncludeOneOf.indexOf(c) >= 0) {
                                    break;
                                }
                            }
                        }
                        if (patIndx >= patLen) {
                            errln(UnicodeString("DateTimePatternGenerator getBestPattern for locale ") +
                                    UnicodeString(locale.getName(),-1,US_INV) +
                                    ", skeleton " + skeleton +
                                    ", produces pattern without required chars: " + pattern);
                        }

                    }
                }
            }
            delete dtpg;
        } else {
            dataerrln("Create DateTimePatternGenerator instance for locale(%s) fails: %s",
                      locale.getName(), u_errorName(status));
        }
    }
}

void IntlTestDateTimePatternGeneratorAPI::testStaticGetSkeleton(/*char *par*/)
{
    // Verify that staticGetSkeleton() doesn't mangle skeletons. (Ticket #11985)
    static const char* const testData[] = {
        "jmm",
        "jjmm",
        "Jmm",
        "JJmm"
    };

    for (size_t i = 0; i < UPRV_LENGTHOF(testData); i++) {
        UErrorCode status = U_ZERO_ERROR;
        UnicodeString skeleton = DateTimePatternGenerator::staticGetSkeleton(testData[i], status);
        if (!assertSuccess("staticGetSkeleton", status)) {
            return;
        }
        assertEquals("Skeleton", testData[i], skeleton);
    }
}

void IntlTestDateTimePatternGeneratorAPI::testC() {
    const char* tests[][3] = {
            // These may change with actual data for Bhmm/bhmm skeletons
            {"zh-TW",  "Cm",      "Bh:mm"},
            {"zh-TW",  "CCm",     "Bhh:mm"},
            {"zh-TW",  "CCCm",    "BBBBh:mm"},
            {"zh-TW",  "CCCCm",   "BBBBhh:mm"},
            {"zh-TW",  "CCCCCm",  "BBBBBh:mm"},
            {"zh-TW",  "CCCCCCm", "BBBBBhh:mm"},
            {"de",     "Cm",      "HH:mm"},
            {"de",     "CCm",     "HH:mm"},
            {"de",     "CCCm",    "HH:mm"},
            {"de",     "CCCCm",   "HH:mm"},
            {"en",     "Cm",      "h:mm\\u202Fa"},
            {"en",     "CCm",     "hh:mm\\u202Fa"},
            {"en",     "CCCm",    "h:mm\\u202Faaaa"},
            {"en",     "CCCCm",   "hh:mm\\u202Faaaa"},
            {"en",     "CCCCCm",  "h:mm\\u202Faaaaa"},
            {"en",     "CCCCCCm", "hh:mm\\u202Faaaaa"},
            {"en-BN",  "Cm",      "h:mm\\u202Fb"},
            {"gu-IN",  "Cm",      "h:mm B"},
            {"und-IN", "Cm",      "h:mm B"}
    };

    UErrorCode status = U_ZERO_ERROR;
    int32_t numTests = UPRV_LENGTHOF(tests);
    for (int32_t i = 0; i < numTests; ++i) {
        DateTimePatternGenerator *gen = DateTimePatternGenerator::createInstance(
                Locale::forLanguageTag(tests[i][0], status), status);
        if (gen == nullptr) {
            dataerrln("FAIL: DateTimePatternGenerator::createInstance failed for %s", tests[i][0]);
            return;
        }
        UDateTimePatternMatchOptions options = UDATPG_MATCH_HOUR_FIELD_LENGTH;
        UnicodeString pattern = gen->getBestPattern(tests[i][1], options, status);
        UnicodeString expectedPattern = UnicodeString(tests[i][2]).unescape();

        char message[100] = "\0";
        strcat(message, tests[i][0]);
        strcat(message, "/");
        strcat(message, tests[i][1]);
        assertEquals(message, expectedPattern, pattern);
        delete gen;
    }
}

enum { kCharBufMax = 31 };
void IntlTestDateTimePatternGeneratorAPI::testSkeletonsWithDayPeriods() {
    const char * patterns[] = {
        // since icu4c getEmptyInstance does not call addCanonicalItems (unlike J), set these here:
        "a",    // should get internal skeleton a
        "H",    // should get internalskeleton H
        "m",    // should get internalskeleton m
        "s",    // should get internalskeleton s
        // patterns from which to construct sample data for a locale
        //"H",  // should get internalskeleton H
        "h a",  // should get internalskeleton ah
        "B h",  // should get internalskeleton Bh
    };
    const char* testItems[][2] = {
        // sample requested skeletons and results
        // skel     pattern
        { "H",      "H"},
        { "HH",     "HH"},
        { "aH",     "H"},
        { "aHH",    "HH"},
        { "BH",     "H"},
        { "BHH",    "HH"},
        { "BBBBH",  "H"},
        { "h",      "h a"},
        { "hh",     "hh a"},
        { "ah",     "h a"},
        { "ahh",    "hh a"},
        { "aaaah",  "h aaaa"},
        { "aaaahh", "hh aaaa"},
        { "bh",     "h b"},
        { "bhh",    "hh b"},
        { "bbbbh",  "h bbbb"},
        { "Bh",     "B h"},
        { "Bhh",    "B hh"},
        { "BBBBh",  "BBBB h"},
        { "BBBBhh", "BBBB hh"},
        { "a",      "a"},
        { "aaaaa",  "aaaaa"},
        { "b",      "b"},
        { "bbbb",   "bbbb"},
        { "B",      "B"},
        { "BBBB",  "BBBB"},
    };
    UErrorCode status = U_ZERO_ERROR;
    DateTimePatternGenerator *gen = DateTimePatternGenerator::createEmptyInstance(status);
    if (U_FAILURE(status)) {
        errln("ERROR: createEmptyInstance fails, status: %s", u_errorName(status));
    } else {
        int32_t i, len = UPRV_LENGTHOF(patterns);
        for (i = 0; i < len; i++) {
            UnicodeString conflictingPattern;
            (void)gen->addPattern(UnicodeString(patterns[i]), true, conflictingPattern, status);
            if (U_FAILURE(status)) {
                errln("ERROR: addPattern %s fail, status: %s", patterns[i], u_errorName(status));
                break;
            }
        }
        if (U_SUCCESS(status)) {
            len = UPRV_LENGTHOF(testItems);
            for (i = 0; i < len; i++) {
                status = U_ZERO_ERROR;
                UDateTimePatternMatchOptions options = UDATPG_MATCH_HOUR_FIELD_LENGTH;
                UnicodeString result = gen->getBestPattern(UnicodeString(testItems[i][0]), options, status);
                if (U_FAILURE(status)) {
                    errln("ERROR: getBestPattern %s fail, status: %s", testItems[i][0], u_errorName(status));
                } else if (result != UnicodeString(testItems[i][1])) {
                    char charResult[kCharBufMax+1];
                    result.extract(0, result.length(), charResult, kCharBufMax);
                    charResult[kCharBufMax] = 0; // ensure termination
                    errln("ERROR: getBestPattern %s, expected %s, got %s", testItems[i][0], testItems[i][1], charResult);
                }
            }
        }
    }
    delete gen;
}

typedef struct FieldDisplayNameData {
    const char *            locale;
    UDateTimePatternField   field;
    UDateTimePGDisplayWidth width;
    const char *            expected; // can have escapes such as \\u00E0
} FieldDisplayNameData;
enum { kFieldDisplayNameMax = 32 };

void IntlTestDateTimePatternGeneratorAPI::testGetFieldDisplayNames() {
    const FieldDisplayNameData testData[] = {
        /*loc      field                              width               expectedName */
        { "de",    UDATPG_QUARTER_FIELD,              UDATPG_WIDE,        "Quartal" },
        { "de",    UDATPG_QUARTER_FIELD,              UDATPG_ABBREVIATED, "Quart." },
        { "de",    UDATPG_QUARTER_FIELD,              UDATPG_NARROW,      "Q" },
        { "en",    UDATPG_DAY_OF_WEEK_IN_MONTH_FIELD, UDATPG_WIDE,        "weekday of the month" },
        { "en",    UDATPG_DAY_OF_WEEK_IN_MONTH_FIELD, UDATPG_ABBREVIATED, "wkday. of mo." },
        { "en",    UDATPG_DAY_OF_WEEK_IN_MONTH_FIELD, UDATPG_NARROW,      "wkday. of mo." }, // fallback
        { "en_GB", UDATPG_DAY_OF_WEEK_IN_MONTH_FIELD, UDATPG_WIDE,        "weekday of the month" },
        { "en_GB", UDATPG_DAY_OF_WEEK_IN_MONTH_FIELD, UDATPG_ABBREVIATED, "wkday of mo" }, // override
        { "en_GB", UDATPG_DAY_OF_WEEK_IN_MONTH_FIELD, UDATPG_NARROW,      "wkday of mo" },
        { "it",    UDATPG_SECOND_FIELD,               UDATPG_WIDE,        "secondo" },
        { "it",    UDATPG_SECOND_FIELD,               UDATPG_ABBREVIATED, "s" },
        { "it",    UDATPG_SECOND_FIELD,               UDATPG_NARROW,      "s" },
    };

    int count = UPRV_LENGTHOF(testData);
    const FieldDisplayNameData * testDataPtr = testData;
    for (; count-- > 0; ++testDataPtr) {
        UErrorCode status = U_ZERO_ERROR;
        Locale locale(testDataPtr->locale);
        DateTimePatternGenerator * dtpg = DateTimePatternGenerator::createInstance(locale, status);
        if (U_FAILURE(status)) {
            dataerrln("FAIL: DateTimePatternGenerator::createInstance failed for locale %s", testDataPtr->locale);
        } else {
            char16_t expName[kFieldDisplayNameMax+1];
            u_unescape(testDataPtr->expected, expName, kFieldDisplayNameMax);
            expName[kFieldDisplayNameMax] = 0; // ensure 0 termination
            UnicodeString getName = dtpg->getFieldDisplayName(testDataPtr->field, testDataPtr->width);
            if (getName.compare(expName, u_strlen(expName)) != 0) {
                errln("ERROR: locale %s field %d width %d, expected %s\n",
                      testDataPtr->locale, testDataPtr->field, testDataPtr->width, testDataPtr->expected);
            }
            delete dtpg;
        }
    }
}

static const char16_t timeCycleChars[] = {
    static_cast<char16_t>(0x0048),
    static_cast<char16_t>(0x0068),
    static_cast<char16_t>(0x004B),
    static_cast<char16_t>(0x006B),
    static_cast<char16_t>(0)
};

void IntlTestDateTimePatternGeneratorAPI::testJjMapping() {
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString jSkeleton("j");
    // First test that j maps correctly by region in a locale for which we do not have data.
    {
        const char* testLocaleID = "de_US"; // short patterns from fallback locale "de" have "HH"
        Locale testLocale(testLocaleID);
        LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createInstance(testLocale, status));
        if (U_FAILURE(status)) {
            dataerrln("FAIL: DateTimePatternGenerator::createInstance failed for locale %s: %s", testLocaleID, u_errorName(status));
        } else {
            UnicodeString jPattern = dtpg->getBestPattern(jSkeleton, UDATPG_MATCH_ALL_FIELDS_LENGTH, status); // get pattern with h e.g. "h 'Uhr' a"
            if (U_FAILURE(status)) {
                errln("FAIL: DateTimePatternGenerator::getBestPattern locale %s, pattern j: %s", testLocaleID, u_errorName(status));
            } else {
                UnicodeString jPatSkeleton = DateTimePatternGenerator::staticGetSkeleton(jPattern, status); // strip literals, get e.g. "ah"
                if (U_FAILURE(status)) {
                    errln("FAIL: DateTimePatternGenerator::staticGetSkeleton locale %s: %s", testLocaleID, u_errorName(status));
                } else if (jPatSkeleton.indexOf(u'h') < 0) { // expect US preferred cycle 'h', not H or other cycle
                    errln("ERROR: DateTimePatternGenerator::getBestPattern locale %s, pattern j did not use 'h'", testLocaleID);
                }
            }
        }
    }

    // Next test that in all available Locales, the actual short time pattern uses the same cycle as produced by 'j'
    int32_t locCount;
    const Locale* localePtr = DateFormat::getAvailableLocales(locCount);
    for (; locCount-- > 0; localePtr++) {
        const char* localeID = localePtr->getName();
        status = U_ZERO_ERROR;
        LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createInstance(*localePtr, status));
        if (U_FAILURE(status)) {
            dataerrln("FAIL: DateTimePatternGenerator::createInstance failed for locale %s: %s", localeID, u_errorName(status));
            continue;
        }
        LocalPointer<DateFormat> dfmt(DateFormat::createTimeInstance(DateFormat::kShort, *localePtr));
        if (U_FAILURE(status)) {
            dataerrln("FAIL: DateFormat::createTimeInstance kShort failed for locale %s: %s", localeID, u_errorName(status));
            continue;
        }
        const SimpleDateFormat* sdfmt;
        if ((sdfmt = dynamic_cast<const SimpleDateFormat*>(reinterpret_cast<const DateFormat*>(dfmt.getAlias()))) == nullptr) {
            continue;
        }
        UnicodeString shortPattern;
        shortPattern = sdfmt->toPattern(shortPattern);
        UnicodeString jPattern = dtpg->getBestPattern(jSkeleton, status);
        if (U_FAILURE(status)) {
            errln("FAIL: DateTimePatternGenerator::getBestPattern locale %s, pattern j: %s", localeID, u_errorName(status));
            continue;
        }
        // Now check that shortPattern and jPattern use the same hour cycle
        if ((uprv_strncmp(localeID, "yue_Hant_CN", 11) == 0) 
        		&& logKnownIssue("CLDR-17979", "Need timeFormats with h for yue_Hant_CN")) {
            continue;
        }
#if APPLE_ICU_CHANGES
// rdar://123393073
        if ((uprv_strncmp(localeID, "lut", 3) == 0 || uprv_strncmp(localeID, "mus", 3) == 0) && logKnownIssue("N/A", "Minimal data available for lut and mus")) {
            continue;
        }
#endif // APPLE_ICU_CHANGES
        UnicodeString jPatSkeleton = DateTimePatternGenerator::staticGetSkeleton(jPattern, status);
        UnicodeString shortPatSkeleton = DateTimePatternGenerator::staticGetSkeleton(shortPattern, status);
        if (U_FAILURE(status)) {
            errln("FAIL: DateTimePatternGenerator::staticGetSkeleton locale %s: %s", localeID, u_errorName(status));
            continue;
        }
        const char16_t* charPtr = timeCycleChars;
        for (; *charPtr != static_cast<char16_t>(0); charPtr++) {
             if (jPatSkeleton.indexOf(*charPtr) >= 0) {
                 if (shortPatSkeleton.indexOf(*charPtr) < 0) {
                     char jcBuf[2], spBuf[32], jpBuf[32];
                     u_austrncpy(jcBuf, charPtr, 1);
                     jcBuf[1] = 0;
                     shortPattern.extract(0, shortPattern.length(), spBuf, 32);
                     jPattern.extract(0, jPattern.length(), jpBuf, 32);
                     const char* dfmtCalType = (dfmt->getCalendar())->getType();
                     const char* validLoc = dfmt->getLocaleID(ULOC_VALID_LOCALE, status);
                     errln("ERROR: locale %s (valid %s), expected j resolved char %s to occur in short time pattern '%s' for %s (best pattern: '%s')",
                             localeID, validLoc, jcBuf, spBuf, dfmtCalType, jpBuf);
                 }
                 break;
             }
        }
    }
}

void IntlTestDateTimePatternGeneratorAPI::test20640_HourCyclArsEnNH() {
    IcuTestErrorCode status(*this, "test20640_HourCyclArsEnNH");

    const struct TestCase {
        const char* localeName;
        const char16_t* expectedDtpgPattern;
        const char16_t* expectedTimePattern;
        UDateFormatHourCycle expectedDefaultHourCycle;
    } cases[] = {
        // ars is interesting because it does not have a region, but it aliases
        // to ar_SA, which has a region.
#if APPLE_ICU_CHANGES
// rdar://
        {"ars", u"h\u00A0a", u"h:mm\u00A0a", UDAT_HOUR_CYCLE_12},
#else
        {"ars", u"h\u202Fa", u"h:mm a", UDAT_HOUR_CYCLE_12},
#endif  // APPLE_ICU_CHANGES
        // en_NH is interesting because NH is a deprecated region code;
        // formerly New Hebrides, now Vanuatu => VU => h.
        {"en_NH", u"h\u202Fa", u"h:mm\u202Fa", UDAT_HOUR_CYCLE_12},
        // ch_ZH is a typo (should be zh_CN), but we should fail gracefully.
        {"cn_ZH", u"HH", u"HH:mm", UDAT_HOUR_CYCLE_23 }, // Desired & now actual behavior (does this fix ICU-20653?)
        // a non-BCP47 locale without a country code should not fail
        {"ja_TRADITIONAL", u"H時", u"H:mm", UDAT_HOUR_CYCLE_23},
    };

    for (const auto& cas : cases) {
        status.setScope(cas.localeName);

        Locale loc(cas.localeName);
        LocalPointer<DateFormat> dtf(DateFormat::createTimeInstance(DateFormat::kShort, loc), status);
        LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createInstance(loc, status));
        if (status.errIfFailureAndReset()) {
            return;
        }

        UnicodeString timePattern;
        dynamic_cast<SimpleDateFormat*>(dtf.getAlias())->toPattern(timePattern);
        UnicodeString dtpgPattern = dtpg->getBestPattern(u"j", status);
        if (status.errIfFailureAndReset()) {
            return;
        }
        UDateFormatHourCycle defaultHourCycle = dtpg->getDefaultHourCycle(status);
        if (status.errIfFailureAndReset()) {
            return;
        }

        assertEquals(UnicodeString("dtpgPattern ") + cas.localeName,
            cas.expectedDtpgPattern, dtpgPattern);
        assertEquals(UnicodeString("timePattern ") + cas.localeName,
            cas.expectedTimePattern, timePattern);
        assertEquals(UnicodeString("defaultHour ") + cas.localeName,
            cas.expectedDefaultHourCycle, defaultHourCycle);
    }

}

void IntlTestDateTimePatternGeneratorAPI::testFallbackWithDefaultRootLocale() {
    UErrorCode status = U_ZERO_ERROR;
    char original[ULOC_FULLNAME_CAPACITY];
    
    uprv_strcpy(original, uloc_getDefault());
    uloc_setDefault("root", &status);
    if (U_FAILURE(status)) {
        errln("ERROR: Failed to change the default locale to root! Default is: %s\n", uloc_getDefault());
    }
 
    DateTimePatternGenerator* dtpg = icu::DateTimePatternGenerator::createInstance("abcdedf", status);

    if (U_FAILURE(status)) {
        errln("ERROR: expected createInstance with invalid locale to succeed. Status: %s", u_errorName(status));
    }
    if (status != U_USING_DEFAULT_WARNING) {
        errln("ERROR: expected createInstance to return U_USING_DEFAULT_WARNING for invalid locale and default root locale. Status: %s", u_errorName(status));
    }

    delete dtpg;

    uloc_setDefault(original, &status);
    if (U_FAILURE(status)) {
        errln("ERROR: Failed to change the default locale back to %s\n", original);
    }
}

// ICU-21000 Ensure that calling getDefaultHourCycle on an empty instance doesn't call UPRV_UNREACHABLE_EXIT/abort.
void IntlTestDateTimePatternGeneratorAPI::testGetDefaultHourCycle_OnEmptyInstance() {
    UErrorCode status = U_ZERO_ERROR;

    LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createEmptyInstance(status), status);
    if (U_FAILURE(status)) {
        errln("ERROR: createEmptyInstance failed, status: %s", u_errorName(status));
        return;
    }
    (void)dtpg->getDefaultHourCycle(status);
    if (!U_FAILURE(status)) {
        errln("ERROR: expected getDefaultHourCycle on an empty instance to fail, status: %s", u_errorName(status));
        return;
    }

    status = U_USELESS_COLLATOR_ERROR;
    (void)dtpg->getDefaultHourCycle(status);
    if (status != U_USELESS_COLLATOR_ERROR) {
        errln("ERROR: getDefaultHourCycle shouldn't modify status if it is already failed, status: %s", u_errorName(status));
        return;
    }
}

void IntlTestDateTimePatternGeneratorAPI::test_jConsistencyOddLocales() { // ICU-20590
    static const char* localeIDs[] = {
        "en", "ro", // known languages 12h / 24h
        "en-RO", "ro-US",  // known languages with known regions, hour conflict language vs region
        "en-XZ", "ro-XZ", // known languages 12h / 24h, unknown region
        "xz-RO", "xz-US",  // unknown language with known regions
        "xz", // unknown language
        "xz-ZX",  // unknown language with unknown country
        "ars", "wuu" // aliased locales
    };
    static const char16_t* skeleton = u"jm";
    for (const char* localeID: localeIDs) {
        UErrorCode status = U_ZERO_ERROR;
        Locale locale(localeID);
        LocalPointer<DateFormat> dtfShort(DateFormat::createTimeInstance(DateFormat::kShort, locale), status);
        if (U_FAILURE(status)) {
            errln("DateFormat::createTimeInstance failed for locale %s: %s", localeID, u_errorName(status));
            continue;
        }
        LocalPointer<DateFormat> dtfSkel(DateFormat::createInstanceForSkeleton(skeleton, locale, status));
        if (U_FAILURE(status)) {
            errln("DateFormat::createInstanceForSkeleton failed for locale %s: %s", localeID, u_errorName(status));
            continue;
        }
        LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createInstance(locale, status));
        if (U_FAILURE(status)) {
            errln("DateTimePatternGenerator::createInstance failed for locale %s: %s", localeID, u_errorName(status));
            continue;
        }
        UnicodeString dtfShortPattern, dtfSkelPattern;
        dynamic_cast<SimpleDateFormat*>(dtfShort.getAlias())->toPattern(dtfShortPattern);
        dynamic_cast<SimpleDateFormat*>(dtfSkel.getAlias())->toPattern(dtfSkelPattern);
        UnicodeString dtpgPattern = (dtpg.getAlias())->getBestPattern(skeleton, status);
        if (U_FAILURE(status)) {
            errln("DateTimePatternGenerator::getBestPattern failed for locale %s: %s", localeID, u_errorName(status));
            continue;
        }
        if (dtfShortPattern != dtfSkelPattern || dtfSkelPattern != dtpgPattern) {
            const char* dtfShortValidLoc = dtfShort->getLocaleID(ULOC_VALID_LOCALE, status);
            const char* dtfShortActualLoc = dtfShort->getLocaleID(ULOC_ACTUAL_LOCALE, status);
            errln(UnicodeString("For locale ") + localeID +
                    " expected same pattern from DateTimePatGen: " + dtpgPattern +
                    ", DateFmt-forSkel: " + dtfSkelPattern + ", DateFmt-short: "  + dtfShortPattern +
                    "; latter has validLoc " + dtfShortValidLoc + ", actualLoc " + dtfShortActualLoc);
        }
    }
}

void IntlTestDateTimePatternGeneratorAPI::testBestPattern() {
    // generic test for DateTimePatternGenerator::getBestPattern() that can be used to test multiple
    // bugs in the resource data
    const struct TestCase {
        const char* localeID;
        const char* skeleton;
        const char16_t* expectedPattern;
    } testCases[] = {
        // ICU-21650: (See the "week day" section of https://www.unicode.org/reports/tr35/tr35-dates.html#Date_Field_Symbol_Table
        // for a full explanation of why this is the desired behavior)
        // if the user asks for E, the minimum field length is 3, but if he asks for c or e, it's 1
        { "en_US",      "E",           u"ccc"    },
        { "en_US",      "c",           u"c"      },
        { "en_US",      "e",           u"c"      },
        { "en_US",      "EE",          u"ccc"    },
        { "en_US",      "cc",          u"cc"     },
        { "en_US",      "ee",          u"cc"     },
        { "en_US",      "EEE",         u"ccc"    },
        { "en_US",      "ccc",         u"ccc"    },
        { "en_US",      "eee",         u"ccc"    },
        // and if the user asked for c or e and the field length is 1 or 2, the output pattern should contain
        // e instead of E (e supports numeric abbreviations; E doesn't)
        { "en_US",      "yMEd",        u"EEE, M/d/y"         },
        { "en_US",      "yMcd",        u"e, M/d/y"           },
        { "en_US",      "yMed",        u"e, M/d/y"           },
        { "en_US",      "yMMEEdd",     u"EEE, MM/dd/y"       },
        { "en_US",      "yMMccdd",     u"ee, MM/dd/y"        },
        { "en_US",      "yMMeedd",     u"ee, MM/dd/y"        },
        { "en_US",      "yMMMEd",      u"EEE, MMM d, y"      },
        { "en_US",      "yMMMcccd",    u"EEE, MMM d, y"      },
        { "en_US",      "yMMMeeed",    u"EEE, MMM d, y"      },
        { "en_US",      "yMMMMEEEEd",  u"EEEE, MMMM d, y"    },
        { "en_US",      "yMMMMccccd",  u"EEEE, MMMM d, y"    },
        { "en_US",      "yMMMMeeeed",  u"EEEE, MMMM d, y"    },
        // ICU-21428: Bad patterns for nonstandard calendars
        { "en_GB",                   "yMd", u"dd/MM/y"          },
        { "en_GB@calendar=coptic",   "yMd", u"dd/MM/y GGGGG"    },
#if APPLE_ICU_CHANGES
// rdar://
        { "en_GB@calendar=japanese", "yMd", u"dd/MM/GGGGGy"    },
#else
        { "en_GB@calendar=japanese", "yMd", u"dd/MM/y GGGGG"    },
#endif  // APPLE_ICU_CHANGES
        { "en_GB@calendar=buddhist", "yMd", u"dd/MM/y GGGGG"    },
        // ICU-22757: Not inheriting availableFormats patterns from root
        { "sv_SE",                   "yMd", u"y-MM-dd"          },
        // ICU-20992: Bad patterns for missing fields
        { "ckb_IR",     "mmSSS",       u"mm:ss\u066bSSS"     },
#if APPLE_ICU_CHANGES
// rdar://
        { "ckb_IR",     "BSSS",        u"SSS \u251C'\u067E.\u0646/\u062F.\u0646': B\u2524" },
        // rdar://71721198: Space before day of week in some zh_Hant patterns
        { "zh_Hant",    "MMMEd",       u"M\u6708d\u65E5 EEE"         },
        { "zh_Hant_HK", "MMMEd",       u"M\u6708d\u65E5 EEE"         },
        { "zh_Hant",    "GyMMMEd",     u"Gy\u5E74M\u6708d\u65E5 EEE" },
        { "zh_Hant_HK", "GyMMMEd",     u"Gy\u5E74M\u6708d\u65E5 EEE" },
        { "zh_Hant",    "yMMMEd",      u"y\u5E74M\u6708d\u65E5 EEE"  },
        { "zh_Hant_HK", "yMMMEd",      u"y\u5E74M\u6708d\u65E5 EEE"  },
        { "zh_Hant",    "full",        u"y\u5E74M\u6708d\u65E5 EEEE" },
        { "zh_Hant_HK", "full",        u"y\u5E74M\u6708d\u65E5 EEEE" },
        // rdar://90816171: Day format consistency for different calendars in vi
        { "vi",                   "Ed", u"EEE, d" },
        { "vi@calendar=buddhist", "Ed", u"EEE, d" },
        { "vi@calendar=islamic",  "Ed", u"EEE, d" },
        { "vi",                   "d",  u"d"      },
        { "vi@calendar=buddhist", "d",  u"d"      },
        { "vi@calendar=islamic",  "d",  u"d"      },
        // rdar://98584839: Weird en_US day+date pattern
        { "en",         "EEEd",        u"EEE d" }, // the case that was failing
        { "en_GB",      "EEEd",        u"EEE d" }, // the cases that were succeeding (and that we want to match)
        { "en_CA",      "EEEd",        u"EEE d" },
        { "en_CA",      "yMd",         u"y-MM-dd" }, //rdar: 116370369: Canadian date format no longer supported DD/MM/YYYY
#else
        { "ckb_IR",     "BSSS",        u"SSS \u251c'Dayperiod': B\u2524" },
#endif  // APPLE_ICU_CHANGES
    };
    
    for (int32_t i = 0; i < UPRV_LENGTHOF(testCases); i++) {
        UErrorCode err = U_ZERO_ERROR;
        UnicodeString actualPattern;
        
        if (uprv_strcmp(testCases[i].skeleton, "full") != 0) {
            LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createInstance(testCases[i].localeID, err), err);
            actualPattern = dtpg->getBestPattern(UnicodeString(testCases[i].skeleton), err);
        } else {
            LocalPointer<DateFormat> df(DateFormat::createDateInstance(DateFormat::kFull, testCases[i].localeID));
            SimpleDateFormat* sdf = dynamic_cast<SimpleDateFormat*>(df.getAlias());
            
            if (sdf != nullptr) {
                sdf->toPattern(actualPattern);
            }
        }
        
        if (U_FAILURE(err)) {
            errln("Failure for test case %s/%s: %s", testCases[i].localeID, testCases[i].skeleton, u_errorName(err));
        } else {
            char failureMessage[100];
            strcpy(failureMessage, "Wrong result for test case ");
            strcat(failureMessage, testCases[i].localeID);
            strcat(failureMessage, "/");
            strcat(failureMessage, testCases[i].skeleton);
            assertEquals(failureMessage, testCases[i].expectedPattern, actualPattern);
        }
    }
}

void IntlTestDateTimePatternGeneratorAPI::testDateTimePatterns() {
    UnicodeString skeletons[kNumDateTimePatterns] = {
        UnicodeString("yMMMMEEEEdjmm"), // full date, short time
        UnicodeString("yMMMMdjmm"),     // long date, short time
        UnicodeString("yMMMdjmm"),      // medium date, short time
        UnicodeString("yMdjmm")         // short date, short time
    };
    // The following tests some locales in which there are differences between the
    // DateTimePatterns of various length styles.
    DTPLocaleAndResults localeAndResults[] = {
        { "en", { UnicodeString(u"EEEE, MMMM d, y 'at' h:mm\u202Fa"), // long != medium
                  UnicodeString(u"MMMM d, y 'at' h:mm\u202Fa"),
#if APPLE_ICU_CHANGES
// rdar://
                  UnicodeString(u"MMM d, y 'at' h:mm\u202Fa"),
#else
                  UnicodeString(u"MMM d, y, h:mm\u202Fa"),
#endif  // APPLE_ICU_CHANGES
                  UnicodeString(u"M/d/y, h:mm\u202Fa") } },
        { "fr", { UnicodeString(u"EEEE d MMMM y 'à' HH:mm"), // medium != short
                  UnicodeString(u"d MMMM y 'à' HH:mm"),
 #if APPLE_ICU_CHANGES
// rdar://
                 UnicodeString(u"d MMM y 'à' HH:mm"),
#else
                 UnicodeString(u"d MMM y, HH:mm"),
#endif  // APPLE_ICU_CHANGES
                  UnicodeString(u"dd/MM/y HH:mm") } },
        { "ha", { UnicodeString(u"EEEE d MMMM, y 'da' HH:mm"),
                  UnicodeString(u"d MMMM, y 'da' HH:mm"),
                  UnicodeString(u"d MMM, y, HH:mm"),
                  UnicodeString(u"y-MM-dd, HH:mm") } },
        { nullptr, { UnicodeString(""), UnicodeString(""), // terminator
                    UnicodeString(""), UnicodeString("") } },
    };

    UnicodeString enDTPatterns[kNumDateTimePatterns] = {
        UnicodeString(u"{1} 'at' {0}"),
        UnicodeString(u"{1} 'at' {0}"),
#if APPLE_ICU_CHANGES
// rdar://
        UnicodeString(u"{1} 'at' {0}"),
#else
        UnicodeString(u"{1}, {0}"),
#endif  // APPLE_ICU_CHANGES
        UnicodeString(u"{1}, {0}")
    };
    UnicodeString modDTPatterns[kNumDateTimePatterns] = {
        UnicodeString(u"{1} _0_ {0}"),
        UnicodeString(u"{1} _1_ {0}"),
        UnicodeString(u"{1} _2_ {0}"),
        UnicodeString(u"{1} _3_ {0}")
    };
    DTPLocaleAndResults enModResults = { "en", { UnicodeString(u"EEEE, MMMM d, y _0_ h:mm\u202Fa"),
                                                 UnicodeString(u"MMMM d, y _1_ h:mm\u202Fa"),
                                                 UnicodeString(u"MMM d, y _2_ h:mm\u202Fa"),
                                                 UnicodeString(u"M/d/y _3_ h:mm\u202Fa") }
    };

    // Test various locales with standard data
    UErrorCode status;
    LocalPointer<DateTimePatternGenerator> dtpg;
    DTPLocaleAndResults* localeAndResultsPtr = localeAndResults;
    for (; localeAndResultsPtr->localeID != nullptr; localeAndResultsPtr++) {
        status = U_ZERO_ERROR;
        Locale locale(localeAndResultsPtr->localeID);
        dtpg.adoptInstead(DateTimePatternGenerator::createInstance(locale, status));
        if (U_FAILURE(status)) {
            dataerrln("FAIL: DateTimePatternGenerator::createInstance for locale %s: %s",
                        localeAndResultsPtr->localeID, u_errorName(status));
        } else {
            doDTPatternTest(dtpg.getAlias(), skeletons, localeAndResultsPtr);
        }
    }
    // Test getting and modifying date-time combining patterns
    status = U_ZERO_ERROR;
    dtpg.adoptInstead(DateTimePatternGenerator::createInstance(Locale::getEnglish(), status));
    if (U_FAILURE(status)) {
        dataerrln("FAIL: DateTimePatternGenerator::createInstance #2 for locale en: %s", u_errorName(status));
    } else {
        char bExpect[64];
        char bGet[64];
        // Test style out of range
        status = U_ZERO_ERROR;
        const UnicodeString& dtFormat0 = dtpg->getDateTimeFormat(UDAT_NONE, status);
        int32_t dtFormat0Len = dtFormat0.length();
        if (status != U_ILLEGAL_ARGUMENT_ERROR || dtFormat0Len != 0) {
            errln("ERROR: getDateTimeFormat with invalid style, expected U_ILLEGAL_ARGUMENT_ERROR and lero-length string, "
                  "got %s with length %d", u_errorName(status), dtFormat0Len);
        }
        // Test normal getting and setting
        for (int32_t patStyle = 0; patStyle < kNumDateTimePatterns; patStyle++) {
            status = U_ZERO_ERROR;
            const UnicodeString& dtFormat1 = dtpg->getDateTimeFormat(static_cast<UDateFormatStyle>(patStyle), status);
            if (U_FAILURE(status)) {
                errln("FAIL: getDateTimeFormat for en before mod, style %d, get %s", patStyle, u_errorName(status));
            } else  if (dtFormat1 != enDTPatterns[patStyle]) {
                enDTPatterns[patStyle].extract(0, enDTPatterns[patStyle].length(), bExpect, 64);
                dtFormat1.extract(0, dtFormat1.length(), bGet, 64);
                errln("ERROR: getDateTimeFormat for en before mod, style %d, expect \"%s\", get \"%s\"",
                        patStyle, bExpect, bGet);
            }
            status = U_ZERO_ERROR;
            dtpg->setDateTimeFormat(static_cast<UDateFormatStyle>(patStyle), modDTPatterns[patStyle], status);
            if (U_FAILURE(status)) {
                errln("FAIL: setDateTimeFormat for en, style %d, get %s", patStyle, u_errorName(status));
            } else {
                const UnicodeString& dtFormat2 = dtpg->getDateTimeFormat(static_cast<UDateFormatStyle>(patStyle), status);
                if (U_FAILURE(status)) {
                    errln("FAIL: getDateTimeFormat for en after  mod, style %d, get %s", patStyle, u_errorName(status));
                } else if (dtFormat2 != modDTPatterns[patStyle]) {
                    modDTPatterns[patStyle].extract(0, modDTPatterns[patStyle].length(), bExpect, 64);
                    dtFormat2.extract(0, dtFormat2.length(), bGet, 64);
                    errln("ERROR: getDateTimeFormat for en after mod, style %d, expect \"%s\", get \"%s\"",
                            patStyle, bExpect, bGet);
                }
            }
        }
        // Test result of setting
        doDTPatternTest(dtpg.getAlias(), skeletons, &enModResults);
        // Test old get/set functions
        const UnicodeString& dtFormat3 = dtpg->getDateTimeFormat();
        if (dtFormat3 != modDTPatterns[UDAT_MEDIUM]) {
            modDTPatterns[UDAT_MEDIUM].extract(0, modDTPatterns[UDAT_MEDIUM].length(), bExpect, 64);
            dtFormat3.extract(0, dtFormat3.length(), bGet, 64);
            errln("ERROR: old getDateTimeFormat for en after mod, expect \"%s\", get \"%s\"", bExpect, bGet);
        }
        dtpg->setDateTimeFormat(modDTPatterns[UDAT_SHORT]); // set all dateTimePatterns to the short format
        modDTPatterns[UDAT_SHORT].extract(0, modDTPatterns[UDAT_SHORT].length(), bExpect, 64);
        for (int32_t patStyle = 0; patStyle < kNumDateTimePatterns; patStyle++) {
            status = U_ZERO_ERROR;
            const UnicodeString& dtFormat4 = dtpg->getDateTimeFormat(static_cast<UDateFormatStyle>(patStyle), status);
            if (U_FAILURE(status)) {
                errln("FAIL: getDateTimeFormat for en after second mod, style %d, get %s", patStyle, u_errorName(status));
            } else if (dtFormat4 != modDTPatterns[UDAT_SHORT]) {
                dtFormat4.extract(0, dtFormat4.length(), bGet, 64);
                errln("ERROR: getDateTimeFormat for en after second mod, style %d, expect \"%s\", get \"%s\"",
                        patStyle, bExpect, bGet);
            }
        }
    }
}

void IntlTestDateTimePatternGeneratorAPI::testRegionOverride() {
    const struct TestCase {
        const char* locale;
        const char16_t* expectedPattern;
        UDateFormatHourCycle expectedHourCycle;
    } testCases[] = {
        { "en_US",           u"h:mm\u202fa", UDAT_HOUR_CYCLE_12 },
        { "en_GB",           u"HH:mm",       UDAT_HOUR_CYCLE_23 },
        { "en_US@rg=GBZZZZ", u"HH:mm",       UDAT_HOUR_CYCLE_23 },
        { "en_US@hours=h23", u"HH:mm",       UDAT_HOUR_CYCLE_23 },
    };

    for (int32_t i = 0; i < UPRV_LENGTHOF(testCases); i++) {
        UErrorCode err = U_ZERO_ERROR;
        LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createInstance(testCases[i].locale, err));
        
        if (assertSuccess("Error creating dtpg", err)) {
            UDateFormatHourCycle actualHourCycle = dtpg->getDefaultHourCycle(err);
            UnicodeString actualPattern = dtpg->getBestPattern(u"jmm", err);
            
            if (assertSuccess("Error using dtpg", err)) {
                assertEquals("Wrong hour cycle", testCases[i].expectedHourCycle, actualHourCycle);
                assertEquals("Wrong pattern", testCases[i].expectedPattern, actualPattern);
            }
        }
    }
}

// Test for ICU-21839: Make sure ISO8601 patterns/symbols are inherited from Gregorian
void IntlTestDateTimePatternGeneratorAPI::testISO8601() {
    const char* localeIDs[] = {
        "de-AT-u-ca-iso8601",
        "de-CH-u-ca-iso8601",
    };
    const UChar* skeleton = u"jms";

    for (int32_t i = 0; i < UPRV_LENGTHOF(localeIDs); i++) {
        UErrorCode err = U_ZERO_ERROR;
        LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createInstance(Locale::forLanguageTag(localeIDs[i], err), err));
        UnicodeString pattern = dtpg->getBestPattern(skeleton, err);
        if (pattern.indexOf(UnicodeString(u"├")) >= 0 || pattern.indexOf(UnicodeString(u"Minute")) >= 0) {
            errln(UnicodeString("ERROR: locale ") + localeIDs[i] + UnicodeString(", skeleton ") + skeleton + UnicodeString(", bad pattern: ") + pattern);
        }

        LocalPointer<DateFormat> df(DateFormat::createTimeInstance(DateFormat::MEDIUM, Locale::forLanguageTag(localeIDs[i], err)));
        UnicodeString format;
        df->format(ucal_getNow(), format, err);
        if (format.indexOf(UnicodeString(u"├")) >= 0 || format.indexOf(UnicodeString(u"Minute")) >= 0) {
            errln(UnicodeString("ERROR: locale ") + localeIDs[i] + UnicodeString(", MEDIUM, bad format: ") + format);
        }
    }
}

void IntlTestDateTimePatternGeneratorAPI::doDTPatternTest(DateTimePatternGenerator* dtpg, UnicodeString* skeletons, DTPLocaleAndResults* localeAndResultsPtr) {
    for (int32_t patStyle = 0; patStyle < kNumDateTimePatterns; patStyle++) {
        UErrorCode status = U_ZERO_ERROR;
        UnicodeString getPat = dtpg->getBestPattern(skeletons[patStyle], UDATPG_MATCH_NO_OPTIONS, status);
        if (U_FAILURE(status)) {
            errln("FAIL: DateTimePatternGenerator::getBestPattern locale %s, style %d: %s",
                        localeAndResultsPtr->localeID, patStyle, u_errorName(status));
        } else if (getPat != localeAndResultsPtr->expectPat[patStyle]) {
            char bExpect[64];
            char bGet[64];
            localeAndResultsPtr->expectPat[patStyle].extract(0, localeAndResultsPtr->expectPat[patStyle].length(), bExpect, 64);
            getPat.extract(0, getPat.length(), bGet, 64);
            errln("ERROR: DateTimePatternGenerator::getBestPattern locale %s, style %d, expect \"%s\", get \"%s\"",
                        localeAndResultsPtr->localeID, patStyle, bExpect, bGet);
        }
    }
}

#if APPLE_ICU_CHANGES
// rdar://
// rdar://78420184
void IntlTestDateTimePatternGeneratorAPI::testHorizontalInheritance() {
    static const char* testCases[] = {
        "en_DK@calendar=buddhist", "MMMd",    "d MMM",
        "en_DK@calendar=buddhist", "MMMEd",   "EEE, d MMM",
        "en_DK@calendar=buddhist", "GGGGGyM", "M y GGGGG",
        "en_DK@calendar=buddhist", "h",       "h\\u202Fa",
        "en_DK@calendar=islamic",  "MMMd",    "d MMM",
        "en_DK@calendar=islamic",  "MMMEd",   "EEE, d MMM",
        "en_DK@calendar=islamic",  "GGGGGyM", "M y GGGGG",
        "en_DK@calendar=islamic",  "h",       "h\\u202Fa",
        
        "en_IN@calendar=buddhist", "Ed",      "EEE d",
        "en_IN@calendar=islamic",  "Ed",      "EEE d",

        "es_MX@calendar=buddhist", "Ed",      "EEE d",
        "es_MX@calendar=hebrew",   "Ed",      "EEE d",
        "es_MX@calendar=islamic",  "Ed",      "EEE d",
        
        "es_AR@calendar=buddhist", "MMMd",    "d 'de' MMM",
        "es_AR@calendar=buddhist", "Ed",      "EEE d",
        "es_AR@calendar=buddhist", "yMd",     "d/M/y GGGGG",
        "es_AR@calendar=hebrew",   "MMMd",    "d 'de' MMM",
        "es_AR@calendar=hebrew",   "Ed",      "EEE d",
        "es_AR@calendar=hebrew",   "yMd",     "d/M/y GGGGG",
        "es_AR@calendar=islamic",  "MMMd",    "d 'de' MMM",
        "es_AR@calendar=islamic",  "Ed",      "EEE d",
        "es_AR@calendar=islamic",  "yMd",     "d/M/y GGGGG",
        
        "es_US@calendar=buddhist", "MMMEEE",  "LLL ccc",
        "es_US@calendar=buddhist", "Ed",      "EEE d",
        "es_US@calendar=hebrew",   "MMMEEE",  "LLL ccc",
        "es_US@calendar=hebrew",   "Ed",      "EEE d",
        "es_US@calendar=islamic",  "MMMEEE",  "LLL ccc",
        "es_US@calendar=islamic",  "Ed",      "EEE d",

        "fr_CA@calendar=hebrew",   "MMMEEE",  "LLL ccc",
        "fr_CA@calendar=hebrew",   "Ed",      "EEE d",
        "fr_CA@calendar=islamic",  "MMMEEE",  "LLL ccc",
        "fr_CA@calendar=islamic",  "Ed",      "EEE d",

        "fr_CH@calendar=hebrew",   "MMMEEE",  "LLL ccc",
        "fr_CH@calendar=hebrew",   "Ed",      "EEE d",
        "fr_CH@calendar=islamic",  "MMMEEE",  "LLL ccc",
        "fr_CH@calendar=islamic",  "Ed",      "EEE d",
    };
    
    for (int32_t i = 0; i < UPRV_LENGTHOF(testCases); i += 3) {
        const char* localeID(testCases[i]);
        const char* skeleton(testCases[i + 1]);
        const char* expectedPattern(testCases[i + 2]);
        
        UErrorCode err = U_ZERO_ERROR;
        LocalPointer<DateTimePatternGenerator> dtpg(DateTimePatternGenerator::createInstance(localeID, err), err);
        
        UnicodeString actualPattern = dtpg->getBestPattern(UnicodeString(skeleton), err);
        
        if (U_FAILURE(err)) {
            errln("Failure for test case %s/%s: %s", localeID, skeleton, u_errorName(err));
        } else {
            char failureMessage[100];
            UChar expected[32];
            int32_t expLen = u_unescape(expectedPattern, expected, sizeof(expected));
            strcpy(failureMessage, "Wrong result for test case ");
            strcat(failureMessage, localeID);
            strcat(failureMessage, "/");
            strcat(failureMessage, skeleton);
            assertEquals(failureMessage, UnicodeString(expected, expLen), actualPattern);
        }
    }
}

// rdar://139506314
void IntlTestDateTimePatternGeneratorAPI::testPunjabiPattern() {
  UErrorCode status = U_ZERO_ERROR;
  UnicodeString resultDate;
  UDate testDate= LocaleTest::date(99, 0, 13, 23, 58, 59) + 123.0;
  UnicodeString bestPattern;
  Locale loc("pa");
      
  DateTimePatternGenerator *patGen=DateTimePatternGenerator::createInstance(loc, status);
  if(U_FAILURE(status)) {
    dataerrln("ERROR: Could not create DateTimePatternGenerator with locale pa\n");
    return;
  }
          
  bestPattern = patGen->getBestPattern("MMMMd", status);
          
  SimpleDateFormat sdf(bestPattern, loc, status);
  resultDate.remove();
  resultDate = sdf.format(testDate, resultDate);
  UnicodeString expectedResult = CharsToUnicodeString("13 \\u0A1c\\u0A28\\u0A35\\u0A30\\u0A40");
  if ( resultDate != expectedResult ) {
    errln(UnicodeString("\nERROR: Testing MMMMd for pa locale; ") +
                    u". Got: \"" + resultDate +
                    u" Expected: \"" + expectedResult + u"\"");
  }

    delete patGen;
}


#endif  // APPLE_ICU_CHANGES

#endif /* #if !UCONFIG_NO_FORMATTING */

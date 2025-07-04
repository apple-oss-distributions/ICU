// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 1997-2016 International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/

#ifndef _INTLTESTDATETIMEPATTERNGENERATORAPI
#define _INTLTESTDATETIMEPATTERNGENERATORAPI

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "unicode/dtptngen.h"
#include "unicode/ustring.h"
#include "intltest.h"

/**
 * Test basic functionality of various API functions
 **/
class IntlTestDateTimePatternGeneratorAPI : public IntlTest {
    void runIndexedTest( int32_t index, UBool exec, const char* &name, char* par = nullptr ) override;

private:
    /**
     * Performs tests on many API functions, see detailed comments in source code
     **/
    void testAPI(/* char* par */);
    void testOptions(/* char* par */);
    void testAllFieldPatterns(/* char* par */);
    void testStaticGetSkeleton(/* char* par */);
    void testC();
    void testSkeletonsWithDayPeriods();
    void testGetFieldDisplayNames();
    void testJjMapping();
    void test20640_HourCyclArsEnNH();
    void testFallbackWithDefaultRootLocale();
    void testGetDefaultHourCycle_OnEmptyInstance();
    void test_jConsistencyOddLocales();
    void testBestPattern();
    void testDateTimePatterns();
    void testISO8601();
    void testRegionOverride();
#if APPLE_ICU_CHANGES
// rdar://
   void testHorizontalInheritance();   // rdar://78420184
    void testPunjabiPattern(); // rdar://139506314
#endif  // APPLE_ICU_CHANGES

    // items for testDateTimePatterns();
    enum { kNumDateTimePatterns = 4 };
    typedef struct {
        const char* localeID;
        const UnicodeString expectPat[kNumDateTimePatterns];
    } DTPLocaleAndResults;
    void doDTPatternTest(DateTimePatternGenerator* dtpg, UnicodeString* skeletons, DTPLocaleAndResults* localeAndResultsPtr);
};

#endif /* #if !UCONFIG_NO_FORMATTING */

#endif

// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 1997-2016, International Business Machines
 * Corporation and others. All Rights Reserved.
 ********************************************************************/

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "dtfmttst.h"
#include "unicode/localpointer.h"
#include "unicode/timezone.h"
#include "unicode/gregocal.h"
#include "unicode/smpdtfmt.h"
#include "unicode/datefmt.h"
#include "unicode/dtptngen.h"
#include "unicode/simpletz.h"
#include "unicode/strenum.h"
#include "unicode/dtfmtsym.h"
#include "unicode/ustring.h"
#include "cmemory.h"
#include "cstring.h"
#include "caltest.h"  // for fieldName
#include "charstr.h"

#if U_PLATFORM_USES_ONLY_WIN32_API
#include "windttst.h"
#endif

#define ASSERT_OK(status) UPRV_BLOCK_MACRO_BEGIN { \
    if(U_FAILURE(status)) { \
        errcheckln(status, #status " = %s @ %s:%d", u_errorName(status), __FILE__, __LINE__); \
        return; \
    } \
} UPRV_BLOCK_MACRO_END

// *****************************************************************************
// class DateFormatTest
// *****************************************************************************

void DateFormatTest::runIndexedTest( int32_t index, UBool exec, const char* &name, char* /*par*/ )
{
    if(exec) {
        logln("TestSuite DateFormatTest: ");
    }
    TESTCASE_AUTO_BEGIN;
    TESTCASE_AUTO(TestPatterns);
    TESTCASE_AUTO(TestEquals);
    TESTCASE_AUTO(TestTwoDigitYearDSTParse);
    TESTCASE_AUTO(TestFieldPosition);
    TESTCASE_AUTO(TestPartialParse994);
    TESTCASE_AUTO(TestRunTogetherPattern985);
    TESTCASE_AUTO(TestRunTogetherPattern917);
    TESTCASE_AUTO(TestCzechMonths459);
    TESTCASE_AUTO(TestLetterDPattern212);
    TESTCASE_AUTO(TestDayOfYearPattern195);
    TESTCASE_AUTO(TestQuotePattern161);
    TESTCASE_AUTO(TestBadInput135);
    TESTCASE_AUTO(TestBadInput135a);
    TESTCASE_AUTO(TestTwoDigitYear);
    TESTCASE_AUTO(TestDateFormatZone061);
    TESTCASE_AUTO(TestDateFormatZone146);
    TESTCASE_AUTO(TestLocaleDateFormat);
    TESTCASE_AUTO(TestFormattingLocaleTimeSeparator);
    TESTCASE_AUTO(TestWallyWedel);
    TESTCASE_AUTO(TestDateFormatCalendar);
    TESTCASE_AUTO(TestSpaceParsing);
    TESTCASE_AUTO(TestExactCountFormat);
    TESTCASE_AUTO(TestWhiteSpaceParsing);
    TESTCASE_AUTO(TestInvalidPattern);
    TESTCASE_AUTO(TestGeneral);
    TESTCASE_AUTO(TestGreekMay);
    TESTCASE_AUTO(TestGenericTime);
    TESTCASE_AUTO(TestGenericTimeZoneOrder);
    TESTCASE_AUTO(TestHost);
    TESTCASE_AUTO(TestEras);
    TESTCASE_AUTO(TestNarrowNames);
    TESTCASE_AUTO(TestShortDays);
    TESTCASE_AUTO(TestStandAloneDays);
    TESTCASE_AUTO(TestStandAloneMonths);
    TESTCASE_AUTO(TestQuarters);
    TESTCASE_AUTO(TestZTimeZoneParsing);
    TESTCASE_AUTO(TestRelative);
    TESTCASE_AUTO(TestRelativeClone);
    TESTCASE_AUTO(TestHostClone);
    TESTCASE_AUTO(TestHebrewClone);
    TESTCASE_AUTO(TestDateFormatSymbolsClone);
    TESTCASE_AUTO(TestTimeZoneDisplayName);
    TESTCASE_AUTO(TestTimeZoneInLocale);
    TESTCASE_AUTO(TestRoundtripWithCalendar);
    TESTCASE_AUTO(Test6338);
    TESTCASE_AUTO(Test6726);
    TESTCASE_AUTO(TestGMTParsing);
    TESTCASE_AUTO(Test6880);
    TESTCASE_AUTO(TestISOEra);
    TESTCASE_AUTO(TestFormalChineseDate);
    TESTCASE_AUTO(TestNumberAsStringParsing);
    TESTCASE_AUTO(TestStandAloneGMTParse);
    TESTCASE_AUTO(TestParsePosition);
    TESTCASE_AUTO(TestMonthPatterns);
    TESTCASE_AUTO(TestContext);
    TESTCASE_AUTO(TestNonGregoFmtParse);
    TESTCASE_AUTO(TestFormatsWithNumberSystems);
    /*
    TESTCASE_AUTO(TestRelativeError);
    TESTCASE_AUTO(TestRelativeOther);
    */
    TESTCASE_AUTO(TestDotAndAtLeniency);
    TESTCASE_AUTO(TestDateFormatLeniency);
    TESTCASE_AUTO(TestParseMultiPatternMatch);

    TESTCASE_AUTO(TestParseLeniencyAPIs);
    TESTCASE_AUTO(TestNumberFormatOverride);
    TESTCASE_AUTO(TestCreateInstanceForSkeleton);
    TESTCASE_AUTO(TestCreateInstanceForSkeletonDefault);
    TESTCASE_AUTO(TestCreateInstanceForSkeletonWithCalendar);
    TESTCASE_AUTO(TestDFSCreateForLocaleNonGregorianLocale);
    TESTCASE_AUTO(TestDFSCreateForLocaleWithCalendarInLocale);
    TESTCASE_AUTO(TestChangeCalendar);

    TESTCASE_AUTO(TestPatternFromSkeleton);

    TESTCASE_AUTO(TestAmPmMidnightNoon);
    TESTCASE_AUTO(TestFlexibleDayPeriod);
    TESTCASE_AUTO(TestDayPeriodWithLocales);
    TESTCASE_AUTO(TestMinuteSecondFieldsInOddPlaces);
    TESTCASE_AUTO(TestDayPeriodParsing);
    TESTCASE_AUTO(TestParseRegression13744);
    TESTCASE_AUTO(TestAdoptCalendarLeak);
    TESTCASE_AUTO(Test20741_ABFields);
    TESTCASE_AUTO(Test22023_UTCWithMinusZero);
    TESTCASE_AUTO(TestNumericFieldStrictParse);
    TESTCASE_AUTO(TestHourCycle);
    TESTCASE_AUTO(TestHCInLocale);
    TESTCASE_AUTO(TestBogusLocale);
    TESTCASE_AUTO(TestLongLocale);

    TESTCASE_AUTO_END;
}

void DateFormatTest::TestPatterns() {
    static const struct {
        const char *actualPattern;
        const char *expectedPattern;
        const char *localeID;
        const char16_t *expectedLocalPattern;
    } EXPECTED[] = {
        {UDAT_YEAR, "y", "en",u"y"},

        {UDAT_QUARTER, "QQQQ", "en", u"QQQQ"},
        {UDAT_ABBR_QUARTER, "QQQ", "en", u"QQQ"},
        {UDAT_YEAR_QUARTER, "yQQQQ", "en", u"QQQQ y"},
        {UDAT_YEAR_ABBR_QUARTER, "yQQQ", "en", u"QQQ y"},

        {UDAT_NUM_MONTH, "M", "en", u"L"},
        {UDAT_ABBR_MONTH, "MMM", "en", u"LLL"},
        {UDAT_MONTH, "MMMM", "en", u"LLLL"},
        {UDAT_YEAR_NUM_MONTH, "yM","en",u"M/y"},
        {UDAT_YEAR_ABBR_MONTH, "yMMM","en",u"MMM y"},
        {UDAT_YEAR_MONTH, "yMMMM","en",u"MMMM y"},

        {UDAT_DAY, "d","en",u"d"},
        {UDAT_YEAR_NUM_MONTH_DAY, "yMd", "en", u"M/d/y"},
        {UDAT_YEAR_ABBR_MONTH_DAY, "yMMMd", "en", u"MMM d, y"},
        {UDAT_YEAR_MONTH_DAY, "yMMMMd", "en", u"MMMM d, y"},
        {UDAT_YEAR_NUM_MONTH_WEEKDAY_DAY, "yMEd", "en", u"EEE, M/d/y"},
        {UDAT_YEAR_ABBR_MONTH_WEEKDAY_DAY, "yMMMEd", "en", u"EEE, MMM d, y"},
        {UDAT_YEAR_MONTH_WEEKDAY_DAY, "yMMMMEEEEd", "en", u"EEEE, MMMM d, y"},

        {UDAT_NUM_MONTH_DAY, "Md","en",u"M/d"},
        {UDAT_ABBR_MONTH_DAY, "MMMd","en",u"MMM d"},
        {UDAT_MONTH_DAY, "MMMMd","en",u"MMMM d"},
        {UDAT_NUM_MONTH_WEEKDAY_DAY, "MEd","en",u"EEE, M/d"},
        {UDAT_ABBR_MONTH_WEEKDAY_DAY, "MMMEd","en",u"EEE, MMM d"},
        {UDAT_MONTH_WEEKDAY_DAY, "MMMMEEEEd","en",u"EEEE, MMMM d"},

        {UDAT_HOUR, "j", "en", u"h\u202Fa"}, // (fixed expected result per ticket 6872<-6626)
        {UDAT_HOUR24, "H", "en", u"HH"}, // (fixed expected result per ticket 6872<-6626

        {UDAT_MINUTE, "m", "en", u"m"},
        {UDAT_HOUR_MINUTE, "jm","en",u"h:mm\u202Fa"}, // (fixed expected result per ticket 6872<-7180)
        {UDAT_HOUR24_MINUTE, "Hm", "en", u"HH:mm"}, // (fixed expected result per ticket 6872<-6626)

        {UDAT_SECOND, "s", "en", u"s"},
        {UDAT_HOUR_MINUTE_SECOND, "jms","en",u"h:mm:ss\u202Fa"}, // (fixed expected result per ticket 6872<-7180)
        {UDAT_HOUR24_MINUTE_SECOND, "Hms","en",u"HH:mm:ss"}, // (fixed expected result per ticket 6872<-6626)
        {UDAT_MINUTE_SECOND, "ms", "en", u"mm:ss"}, // (fixed expected result per ticket 6872<-6626)

        {UDAT_LOCATION_TZ, "VVVV", "en", u"VVVV"},
        {UDAT_GENERIC_TZ, "vvvv", "en", u"vvvv"},
        {UDAT_ABBR_GENERIC_TZ, "v", "en", u"v"},
        {UDAT_SPECIFIC_TZ, "zzzz", "en", u"zzzz"},
        {UDAT_ABBR_SPECIFIC_TZ, "z", "en", u"z"},
        {UDAT_ABBR_UTC_TZ, "ZZZZ", "en", u"ZZZZ"},

        {UDAT_YEAR_NUM_MONTH_DAY UDAT_ABBR_UTC_TZ, "yMdZZZZ", "en", u"M/d/y, ZZZZ"},
        {UDAT_MONTH_DAY UDAT_LOCATION_TZ, "MMMMdVVVV", "en", u"MMMM d 'at' VVVV"}
    };

    IcuTestErrorCode errorCode(*this, "TestPatterns()");
    for (int32_t i = 0; i < UPRV_LENGTHOF(EXPECTED); i++) {
        // Verify that patterns have the correct values
        UnicodeString actualPattern(EXPECTED[i].actualPattern, -1, US_INV);
        UnicodeString expectedPattern(EXPECTED[i].expectedPattern, -1, US_INV);
        Locale locale(EXPECTED[i].localeID);
        if (actualPattern != expectedPattern) {
            errln("FAILURE! Expected pattern: " + expectedPattern +
                    " but was: " + actualPattern);
        }

        // Verify that DataFormat instances produced contain the correct
        // localized patterns
        // TODO: use DateFormat::getInstanceForSkeleton(), ticket #9029
        // Java test code:
        // DateFormat date1 = DateFormat.getPatternInstance(actualPattern,
        //         locale);
        // DateFormat date2 = DateFormat.getPatternInstance(Calendar.getInstance(locale),
        //         actualPattern, locale);
        LocalPointer<DateTimePatternGenerator> generator(
                DateTimePatternGenerator::createInstance(locale, errorCode));
        if(errorCode.errDataIfFailureAndReset("DateTimePatternGenerator::createInstance() failed for locale ID \"%s\"", EXPECTED[i].localeID)) {
            continue;
        }
        UnicodeString pattern = generator->getBestPattern(actualPattern, errorCode);
        SimpleDateFormat date1(pattern, locale, errorCode);
        SimpleDateFormat date2(pattern, locale, errorCode);
        date2.adoptCalendar(Calendar::createInstance(locale, errorCode));
        if(errorCode.errIfFailureAndReset("DateFormat::getInstanceForSkeleton() failed")) {
            errln("  for actualPattern \"%s\" & locale ID \"%s\"",
                  EXPECTED[i].actualPattern, EXPECTED[i].localeID);
            continue;
        }

        UnicodeString expectedLocalPattern(EXPECTED[i].expectedLocalPattern, -1);
        UnicodeString actualLocalPattern1;
        UnicodeString actualLocalPattern2;
        date1.toLocalizedPattern(actualLocalPattern1, errorCode);
        date2.toLocalizedPattern(actualLocalPattern2, errorCode);
        if (actualLocalPattern1 != expectedLocalPattern) {
            errln("FAILURE! Expected local pattern: " + expectedLocalPattern
                    + " but was: " + actualLocalPattern1);
        }
        if (actualLocalPattern2 != expectedLocalPattern) {
            errln("FAILURE! Expected local pattern: " + expectedLocalPattern
                    + " but was: " + actualLocalPattern2);
        }
    }
}

// Test written by Wally Wedel and emailed to me.
void DateFormatTest::TestWallyWedel()
{
    UErrorCode status = U_ZERO_ERROR;
    /*
     * Instantiate a TimeZone so we can get the ids.
     */
    TimeZone *tz = new SimpleTimeZone(7,"");
    /*
     * Computational variables.
     */
    int32_t offset, hours, minutes, seconds;
    /*
     * Instantiate a SimpleDateFormat set up to produce a full time
     zone name.
     */
    SimpleDateFormat* sdf = new SimpleDateFormat(UnicodeString("zzzz"), status);
    /*
     * A String array for the time zone ids.
     */
    int32_t ids_length;
    StringEnumeration* ids = TimeZone::createEnumeration(status);
    if (U_FAILURE(status)) {
        dataerrln("Unable to create TimeZone enumeration.");
        delete sdf;
        return;
    }
    ids_length = ids->count(status);
    /*
     * How many ids do we have?
     */
    logln("Time Zone IDs size: %d", ids_length);
    /*
     * Column headings (sort of)
     */
    logln("Ordinal ID offset(h:m) name");
    /*
     * Loop through the tzs.
     */
    UDate today = Calendar::getNow();
    Calendar *cal = Calendar::createInstance(status);
    for (int32_t i = 0; i < ids_length; i++) {
        // logln(i + " " + ids[i]);
        const UnicodeString* id = ids->snext(status);
        TimeZone *ttz = TimeZone::createTimeZone(*id);
        // offset = ttz.getRawOffset();
        cal->setTimeZone(*ttz);
        cal->setTime(today, status);
        offset = cal->get(UCAL_ZONE_OFFSET, status) + cal->get(UCAL_DST_OFFSET, status);
        // logln(i + " " + ids[i] + " offset " + offset);
        const char* sign = "+";
        if (offset < 0) {
            sign = "-";
            offset = -offset;
        }
        hours = offset/3600000;
        minutes = (offset%3600000)/60000;
        seconds = (offset%60000)/1000;
        UnicodeString dstOffset = UnicodeString("") + sign + (hours < 10 ? "0" : "") +
            hours + ":" + (minutes < 10 ? "0" : "") + minutes;
        if (seconds != 0) {
            dstOffset = dstOffset + ":" + (seconds < 10 ? "0" : "") + seconds;
        }
        /*
         * Instantiate a date so we can display the time zone name.
         */
        sdf->setTimeZone(*ttz);
        /*
         * Format the output.
         */
        UnicodeString fmtOffset;
        FieldPosition pos(FieldPosition::DONT_CARE);
        sdf->format(today,fmtOffset, pos);
        // UnicodeString fmtOffset = tzS.toString();
        UnicodeString* fmtDstOffset = nullptr;
        if (fmtOffset.startsWith("GMT") && fmtOffset.length() != 3)
        {
            //fmtDstOffset = fmtOffset->substring(3);
            fmtDstOffset = new UnicodeString();
            fmtOffset.extract(3, fmtOffset.length(), *fmtDstOffset);
        }
        /*
         * Show our result.
         */
        UBool ok = fmtDstOffset == nullptr || *fmtDstOffset == dstOffset;
        if (ok)
        {
            logln(UnicodeString() + i + " " + *id + " " + dstOffset +
                  " " + fmtOffset +
                  (fmtDstOffset != nullptr ? " ok" : " ?"));
        }
        else
        {
            errln(UnicodeString() + i + " " + *id + " " + dstOffset +
                  " " + fmtOffset + " *** FAIL ***");
        }
        delete ttz;
        delete fmtDstOffset;
    }
    delete cal;
    //  delete ids;   // TODO:  BAD API
    delete ids;
    delete sdf;
    delete tz;
}

// -------------------------------------

/**
 * Test operator==
 */
void
DateFormatTest::TestEquals()
{
    DateFormat* fmtA = DateFormat::createDateTimeInstance(DateFormat::MEDIUM, DateFormat::FULL);
    DateFormat* fmtB = DateFormat::createDateTimeInstance(DateFormat::MEDIUM, DateFormat::FULL);
    if ( fmtA == nullptr || fmtB == nullptr){
        dataerrln("Error calling DateFormat::createDateTimeInstance");
        delete fmtA;
        delete fmtB;
        return;
    }

    if (!(*fmtA == *fmtB)) errln(UnicodeString("FAIL"));
    delete fmtA;
    delete fmtB;

    TimeZone* test = TimeZone::createTimeZone("PDT");
    delete test;
}

// -------------------------------------

/**
 * Test the parsing of 2-digit years.
 */
void
DateFormatTest::TestTwoDigitYearDSTParse()
{
    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat fullFmt(UnicodeString("EEE MMM dd HH:mm:ss.SSS zzz yyyy G"), status);
    SimpleDateFormat fmt(UnicodeString("dd-MMM-yy h:mm:ss 'o''clock' a z"), Locale::getEnglish(), status);
    //DateFormat* fmt = DateFormat::createDateTimeInstance(DateFormat::MEDIUM, DateFormat::FULL, Locale::ENGLISH);
    UnicodeString s(u"03-Apr-04 2:20:47 o'clock AM PST");
    LocalPointer<TimeZone> defaultTZ(TimeZone::createDefault());
    LocalPointer<TimeZone> PST(TimeZone::createTimeZone("PST"));
    int32_t defaultOffset = defaultTZ->getRawOffset();
    int32_t PSTOffset = PST->getRawOffset();
    int32_t hour = 2 + (defaultOffset - PSTOffset) / (60*60*1000);
    // hour is the expected hour of day, in units of seconds
    hour = ((hour < 0) ? hour + 24 : hour) * 60*60;

    UnicodeString str;

    if(U_FAILURE(status)) {
        dataerrln("Could not set up test. exiting - %s", u_errorName(status));
        return;
    }

    UDate d = fmt.parse(s, status);
    logln(s + " P> " + fullFmt.format(d, str));
    int32_t y, m, day, hr, min, sec;
    dateToFields(d, y, m, day, hr, min, sec);
    hour += defaultTZ->inDaylightTime(d, status) ? 1 : 0;
    hr = hr*60*60;
    if (hr != hour)
        errln(UnicodeString("FAIL: Should parse to hour ") + hour + " but got " + hr);

    if (U_FAILURE(status))
        errln(UnicodeString("FAIL: ") + static_cast<int32_t>(status));
}

// -------------------------------------

char16_t toHexString(int32_t i) { return static_cast<char16_t>(i + (i < 10 ? 0x30 : (0x41 - 10))); }

UnicodeString&
DateFormatTest::escape(UnicodeString& s)
{
    UnicodeString buf;
    for (int32_t i=0; i<s.length(); ++i)
    {
        char16_t c = s[i];
        if (c <= static_cast<char16_t>(0x7F)) buf += c;
        else {
            buf += static_cast<char16_t>(0x5c); buf += static_cast<char16_t>(0x55);
            buf += toHexString((c & 0xF000) >> 12);
            buf += toHexString((c & 0x0F00) >> 8);
            buf += toHexString((c & 0x00F0) >> 4);
            buf += toHexString(c & 0x000F);
        }
    }
    return (s = buf);
}

// -------------------------------------

/**
 * This MUST be kept in sync with DateFormatSymbols.gPatternChars.
 */
#if UDAT_HAS_PATTERN_CHAR_FOR_TIME_SEPARATOR
static const char* PATTERN_CHARS = "GyMdkHmsSEDFwWahKzYeugAZvcLQqVUOXxrbB:";
#else
static const char* PATTERN_CHARS = "GyMdkHmsSEDFwWahKzYeugAZvcLQqVUOXxrbB";
#endif

/**
 * A list of the names of all the fields in DateFormat.
 * This MUST be kept in sync with DateFormat.
 */
static const char* DATEFORMAT_FIELD_NAMES[] = {
    "ERA_FIELD",
    "YEAR_FIELD",
    "MONTH_FIELD",
    "DATE_FIELD",
    "HOUR_OF_DAY1_FIELD",
    "HOUR_OF_DAY0_FIELD",
    "MINUTE_FIELD",
    "SECOND_FIELD",
    "MILLISECOND_FIELD",
    "DAY_OF_WEEK_FIELD",
    "DAY_OF_YEAR_FIELD",
    "DAY_OF_WEEK_IN_MONTH_FIELD",
    "WEEK_OF_YEAR_FIELD",
    "WEEK_OF_MONTH_FIELD",
    "AM_PM_FIELD",
    "HOUR1_FIELD",
    "HOUR0_FIELD",
    "TIMEZONE_FIELD",
    "YEAR_WOY_FIELD",
    "DOW_LOCAL_FIELD",
    "EXTENDED_YEAR_FIELD",
    "JULIAN_DAY_FIELD",
    "MILLISECONDS_IN_DAY_FIELD",
    "TIMEZONE_RFC_FIELD",
    "GENERIC_TIMEZONE_FIELD",
    "STAND_ALONE_DAY_FIELD",
    "STAND_ALONE_MONTH_FIELD",
    "QUARTER_FIELD",
    "STAND_ALONE_QUARTER_FIELD",
    "TIMEZONE_SPECIAL_FIELD",
    "YEAR_NAME_FIELD",
    "TIMEZONE_LOCALIZED_GMT_OFFSET_FIELD",
    "TIMEZONE_ISO_FIELD",
    "TIMEZONE_ISO_LOCAL_FIELD",
    "RELATED_YEAR_FIELD",
    "AM_PM_MIDNIGHT_NOON_FIELD",
    "FLEXIBLE_DAY_PERIOD_FIELD",
    "UDAT_TIME_SEPARATOR_FIELD",
};

static const int32_t DATEFORMAT_FIELD_NAMES_LENGTH =
    UPRV_LENGTHOF(DATEFORMAT_FIELD_NAMES);

/**
 * Verify that returned field position indices are correct.
 */
void DateFormatTest::TestFieldPosition() {
    UErrorCode ec = U_ZERO_ERROR;
    int32_t i, j, exp;
    UnicodeString buf;

    // Verify data
    DateFormatSymbols rootSyms(Locale(""), ec);
    if (U_FAILURE(ec)) {
        dataerrln("Unable to create DateFormatSymbols - %s", u_errorName(ec));
        return;
    }

    // local pattern chars data is not longer loaded
    // from icu locale bundle
    assertEquals("patternChars", PATTERN_CHARS, rootSyms.getLocalPatternChars(buf));
    assertEquals("patternChars", PATTERN_CHARS, DateFormatSymbols::getPatternUChars());
    assertTrue("DATEFORMAT_FIELD_NAMES", DATEFORMAT_FIELD_NAMES_LENGTH == UDAT_FIELD_COUNT);
#if UDAT_HAS_PATTERN_CHAR_FOR_TIME_SEPARATOR
    assertTrue("Data", UDAT_FIELD_COUNT == uprv_strlen(PATTERN_CHARS));
#else
    assertTrue("Data", UDAT_FIELD_COUNT == uprv_strlen(PATTERN_CHARS) + 1); // +1 for missing TIME_SEPARATOR pattern char
#endif

    // Create test formatters
    const int32_t COUNT = 4;
    DateFormat* dateFormats[COUNT];
    dateFormats[0] = DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull, Locale::getUS());
    dateFormats[1] = DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull, Locale::getFrance());
    // Make the pattern "G y M d..."
    buf.remove().append(PATTERN_CHARS);
    for (j=buf.length()-1; j>=0; --j) buf.insert(j, static_cast<char16_t>(32) /*' '*/);
    dateFormats[2] = new SimpleDateFormat(buf, Locale::getUS(), ec);
    // Make the pattern "GGGG yyyy MMMM dddd..."
    for (j=buf.length()-1; j>=0; j-=2) {
        for (i=0; i<3; ++i) {
            buf.insert(j, buf.charAt(j));
        }
    }
    dateFormats[3] = new SimpleDateFormat(buf, Locale::getUS(), ec);
    if(U_FAILURE(ec)){
        errln(UnicodeString("Could not create SimpleDateFormat object for locale en_US. Error: " )+ UnicodeString(u_errorName(ec)));
        return;
    }
    UDate aug13 = 871508052513.0;

    // Expected output field values for above DateFormats on aug13
    // Fields are given in order of DateFormat field number
    const char* EXPECTED[] = {
        "", "1997", "August", "13", "", "", "34", "12", "", "Wednesday",
        "", "", "", "", "PM", "2", "", "Pacific Daylight Time", "", "",
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
#if UDAT_HAS_PATTERN_CHAR_FOR_TIME_SEPARATOR
        ":",
#else
        "",
#endif

        "", "1997", "ao\\u00FBt", "13", "", "14", "34", "12", "", "mercredi",
        "", "", "", "", "", "", "", "heure d\\u2019\\u00E9t\\u00E9 du Pacifique nord-am\\u00E9ricain", "", "",
        "", "", "", "", "",  "", "", "", "", "", "", "", "", "", "", "", "",
#if UDAT_HAS_PATTERN_CHAR_FOR_TIME_SEPARATOR
        ":",
#else
        "",
#endif

        "AD", "1997", "8", "13", "14", "14", "34", "12", "5", "Wed",
        "225", "2", "33", "3", "PM", "2", "2", "PDT", "1997", "4",
        "1997", "2450674", "52452513", "-0700", "PT",  "4", "8", "3", "3", "uslax",
        "1997", "GMT-7", "-07", "-07", "1997", "PM", "in the afternoon",
#if UDAT_HAS_PATTERN_CHAR_FOR_TIME_SEPARATOR
        ":",
#else
        "",
#endif

        "Anno Domini", "1997", "August", "0013", "0014", "0014", "0034", "0012", "5130", "Wednesday",
        "0225", "0002", "0033", "0003", "PM", "0002", "0002", "Pacific Daylight Time", "1997", "Wednesday",
        "1997", "2450674", "52452513", "GMT-07:00", "Pacific Time",  "Wednesday", "August", "3rd quarter", "3rd quarter", "Los Angeles Time",
        "1997", "GMT-07:00", "-0700", "-0700", "1997", "PM", "in the afternoon",
#if UDAT_HAS_PATTERN_CHAR_FOR_TIME_SEPARATOR
        ":",
#else
        "",
#endif
    };

    const int32_t EXPECTED_LENGTH = UPRV_LENGTHOF(EXPECTED);

    assertTrue("data size", EXPECTED_LENGTH == COUNT * UDAT_FIELD_COUNT);

    TimeZone* PT = TimeZone::createTimeZone("America/Los_Angeles");
    for (j = 0, exp = 0; j < COUNT; ++j) {
        //  String str;
        DateFormat* df = dateFormats[j];
        df->setTimeZone(*PT);
        SimpleDateFormat* sdtfmt = dynamic_cast<SimpleDateFormat*>(df);
        if (sdtfmt != nullptr) {
            logln(" Pattern = " + sdtfmt->toPattern(buf.remove()));
        } else {
            logln(" Pattern = ? (not a SimpleDateFormat)");
        }
        logln(UnicodeString("  Result = ") + df->format(aug13, buf.remove()));

        int32_t expBase = exp; // save for later
        for (i = 0; i < UDAT_FIELD_COUNT; ++i, ++exp) {
            FieldPosition pos(i);
            buf.remove();
            df->format(aug13, buf, pos);
            UnicodeString field;
            buf.extractBetween(pos.getBeginIndex(), pos.getEndIndex(), field);
            assertEquals(UnicodeString("localeidx #") + j + " field #" + i + " " +
                         DATEFORMAT_FIELD_NAMES[i], ctou(EXPECTED[exp]), field);
        }

        // test FieldPositionIterator API
        logln("FieldPositionIterator");
        {
          UErrorCode status = U_ZERO_ERROR;
          FieldPositionIterator posIter;
          FieldPosition fp;

          buf.remove();
          df->format(aug13, buf, &posIter, status);
          while (posIter.next(fp)) {
            int32_t i = fp.getField();
            UnicodeString field;
            buf.extractBetween(fp.getBeginIndex(), fp.getEndIndex(), field);
            assertEquals(UnicodeString("localeidx #") + j + " field #" + i + " " +
                         DATEFORMAT_FIELD_NAMES[i], ctou(EXPECTED[expBase + i]), field);
          }

        }
    }


    // test null posIter
    buf.remove();
    UErrorCode status = U_ZERO_ERROR;
    dateFormats[0]->format(aug13, buf, nullptr, status);
    // if we didn't crash, we succeeded.

    for (i=0; i<COUNT; ++i) {
        delete dateFormats[i];
    }
    delete PT;
}

// -------------------------------------

/**
 * General parse/format tests.  Add test cases as needed.
 */
void DateFormatTest::TestGeneral() {
    const char* DATA[] = {
        "yyyy MM dd HH:mm:ss.SSS",

        // Milliseconds are left-justified, since they format as fractions of a second
        "y/M/d H:mm:ss.S", "fp", "2004 03 10 16:36:31.567", "2004/3/10 16:36:31.5", "2004 03 10 16:36:31.500",
        "y/M/d H:mm:ss.SS", "fp", "2004 03 10 16:36:31.567", "2004/3/10 16:36:31.56", "2004 03 10 16:36:31.560",
        "y/M/d H:mm:ss.SSS", "F", "2004 03 10 16:36:31.567", "2004/3/10 16:36:31.567",
        "y/M/d H:mm:ss.SSSS", "pf", "2004/3/10 16:36:31.5679", "2004 03 10 16:36:31.567", "2004/3/10 16:36:31.5670",
    };
    expect(DATA, UPRV_LENGTHOF(DATA), Locale("en", "", ""));
}

// -------------------------------------

/**
 * Verify that strings which contain incomplete specifications are parsed
 * correctly.  In some instances, this means not being parsed at all, and
 * returning an appropriate error.
 */
void
DateFormatTest::TestPartialParse994()
{
    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat* f = new SimpleDateFormat(status);
    if (U_FAILURE(status)) {
        dataerrln("Fail new SimpleDateFormat: %s", u_errorName(status));
        delete f;
        return;
    }
    UDate null = 0;
    tryPat994(f, "yy/MM/dd HH:mm:ss", "97/01/17 10:11:42", date(97, 1 - 1, 17, 10, 11, 42));
    tryPat994(f, "yy/MM/dd HH:mm:ss", "97/01/17 10:", null);
    tryPat994(f, "yy/MM/dd HH:mm:ss", "97/01/17 10", null);
    tryPat994(f, "yy/MM/dd HH:mm:ss", "97/01/17 ", null);
    tryPat994(f, "yy/MM/dd HH:mm:ss", "97/01/17", null);
    if (U_FAILURE(status)) errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
    delete f;
}

// -------------------------------------

void
DateFormatTest::tryPat994(SimpleDateFormat* format, const char* pat, const char* str, UDate expected)
{
    UErrorCode status = U_ZERO_ERROR;
    UDate null = 0;
    logln(UnicodeString("Pattern \"") + pat + "\"   String \"" + str + "\"");
    //try {
        format->applyPattern(pat);
        UDate date = format->parse(str, status);
        if (U_FAILURE(status) || date == null)
        {
            logln(UnicodeString("ParseException: ") + static_cast<int32_t>(status));
            if (expected != null) errln(UnicodeString("FAIL: Expected ") + dateToString(expected));
        }
        else
        {
            UnicodeString f;
            (dynamic_cast<DateFormat*>(format))->format(date, f);
            logln(UnicodeString(" parse(") + str + ") -> " + dateToString(date));
            logln(UnicodeString(" format -> ") + f);
            if (expected == null ||
                !(date == expected)) errln(UnicodeString("FAIL: Expected null")); //" + expected);
            if (!(f == str)) errln(UnicodeString("FAIL: Expected ") + str);
        }
    //}
    //catch(ParseException e) {
    //    logln((UnicodeString)"ParseException: " + e.getMessage());
    //    if (expected != null) errln((UnicodeString)"FAIL: Expected " + dateToString(expected));
    //}
    //catch(Exception e) {
    //    errln((UnicodeString)"*** Exception:");
    //    e.printStackTrace();
    //}
}

// -------------------------------------

/**
 * Verify the behavior of patterns in which digits for different fields run together
 * without intervening separators.
 */
void
DateFormatTest::TestRunTogetherPattern985()
{
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString format("yyyyMMddHHmmssSSS");
    UnicodeString now, then;
    //UBool flag;
    SimpleDateFormat *formatter = new SimpleDateFormat(format, status);
    if (U_FAILURE(status)) {
        dataerrln("Fail new SimpleDateFormat: %s", u_errorName(status));
        delete formatter;
        return;
    }
    UDate date1 = Calendar::getNow();
    (dynamic_cast<DateFormat*>(formatter))->format(date1, now);
    logln(now);
    ParsePosition pos(0);
    UDate date2 = formatter->parse(now, pos);
    if (date2 == 0) then = UnicodeString("Parse stopped at ") + pos.getIndex();
    else (dynamic_cast<DateFormat*>(formatter))->format(date2, then);
    logln(then);
    if (!(date2 == date1)) errln(UnicodeString("FAIL"));
    delete formatter;
    if (U_FAILURE(status)) errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
}

// -------------------------------------

/**
 * Verify the behavior of patterns in which digits for different fields run together
 * without intervening separators.
 */
void
DateFormatTest::TestRunTogetherPattern917()
{
    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat* fmt;
    UnicodeString myDate;
    fmt = new SimpleDateFormat(UnicodeString("yyyy/MM/dd"), status);
    if (U_FAILURE(status)) {
        dataerrln("Fail new SimpleDateFormat: %s", u_errorName(status));
        delete fmt;
        return;
    }
    myDate = "1997/02/03";
    testIt917(fmt, myDate, date(97, 2 - 1, 3));
    delete fmt;
    fmt = new SimpleDateFormat(UnicodeString("yyyyMMdd"), status);
    myDate = "19970304";
    testIt917(fmt, myDate, date(97, 3 - 1, 4));
    delete fmt;
    if (U_FAILURE(status)) errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
}

// -------------------------------------

void
DateFormatTest::testIt917(SimpleDateFormat* fmt, UnicodeString& str, UDate expected)
{
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString pattern;
    logln(UnicodeString("pattern=") + fmt->toPattern(pattern) + "   string=" + str);
    Formattable o;
    //try {
        dynamic_cast<Format*>(fmt)->parseObject(str, o, status);
    //}
    if (U_FAILURE(status)) return;
    //catch(ParseException e) {
    //    e.printStackTrace();
    //    return;
    //}
    logln(UnicodeString("Parsed object: ") + dateToString(o.getDate()));
    if (!(o.getDate() == expected)) errln(UnicodeString("FAIL: Expected ") + dateToString(expected));
    UnicodeString formatted;
    fmt->format(o, formatted, status);
    logln(UnicodeString("Formatted string: ") + formatted);
    if (!(formatted == str)) errln(UnicodeString("FAIL: Expected ") + str);
    if (U_FAILURE(status)) errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
}

// -------------------------------------

/**
 * Verify the handling of Czech June and July, which have the unique attribute that
 * one is a proper prefix substring of the other.
 */
void
DateFormatTest::TestCzechMonths459()
{
    UErrorCode status = U_ZERO_ERROR;
    DateFormat* fmt = DateFormat::createDateInstance(DateFormat::FULL, Locale("cs", "", ""));
    if (fmt == nullptr){
        dataerrln("Error calling DateFormat::createDateInstance()");
        return;
    }

    UnicodeString pattern;
    logln(UnicodeString("Pattern ") + dynamic_cast<SimpleDateFormat*>(fmt)->toPattern(pattern));
    UDate june = date(97, UCAL_JUNE, 15);
    UDate july = date(97, UCAL_JULY, 15);
    UnicodeString juneStr; fmt->format(june, juneStr);
    UnicodeString julyStr; fmt->format(july, julyStr);
    //try {
        logln(UnicodeString("format(June 15 1997) = ") + juneStr);
        UDate d = fmt->parse(juneStr, status);
        UnicodeString s; fmt->format(d, s);
        int32_t month,yr,day,hr,min,sec; dateToFields(d,yr,month,day,hr,min,sec);
        logln(UnicodeString("  -> parse -> ") + s + " (month = " + month + ")");
        if (month != UCAL_JUNE) errln(UnicodeString("FAIL: Month should be June"));
        logln(UnicodeString("format(July 15 1997) = ") + julyStr);
        d = fmt->parse(julyStr, status);
        fmt->format(d, s);
        dateToFields(d,yr,month,day,hr,min,sec);
        logln(UnicodeString("  -> parse -> ") + s + " (month = " + month + ")");
        if (month != UCAL_JULY) errln(UnicodeString("FAIL: Month should be July"));
    //}
    //catch(ParseException e) {
    if (U_FAILURE(status))
        errln(UnicodeString("Exception: ") + static_cast<int32_t>(status));
    //}
    delete fmt;
}

// -------------------------------------

/**
 * Test the handling of 'D' in patterns.
 */
void
DateFormatTest::TestLetterDPattern212()
{
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString dateString("1995-040.05:01:29");
    UnicodeString bigD("yyyy-DDD.hh:mm:ss");
    UnicodeString littleD("yyyy-ddd.hh:mm:ss");
    UDate expLittleD = date(95, 0, 1, 5, 1, 29);
    UDate expBigD = expLittleD + 39 * 24 * 3600000.0;
    expLittleD = expBigD; // Expect the same, with default lenient parsing
    logln(UnicodeString("dateString= ") + dateString);
    SimpleDateFormat *formatter = new SimpleDateFormat(bigD, status);
    if (U_FAILURE(status)) {
        dataerrln("Fail new SimpleDateFormat: %s", u_errorName(status));
        delete formatter;
        return;
    }
    ParsePosition pos(0);
    UDate myDate = formatter->parse(dateString, pos);
    logln(UnicodeString("Using ") + bigD + " -> " + myDate);
    if (myDate != expBigD) errln(UnicodeString("FAIL: bigD - Expected ") + dateToString(expBigD));
    delete formatter;
    formatter = new SimpleDateFormat(littleD, status);
    ASSERT_OK(status);
    pos = ParsePosition(0);
    myDate = formatter->parse(dateString, pos);
    logln(UnicodeString("Using ") + littleD + " -> " + dateToString(myDate));
    if (myDate != expLittleD) errln(UnicodeString("FAIL: littleD - Expected ") + dateToString(expLittleD));
    delete formatter;
    if (U_FAILURE(status)) errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
}

// -------------------------------------

/**
 * Test the day of year pattern.
 */
void
DateFormatTest::TestDayOfYearPattern195()
{
    UErrorCode status = U_ZERO_ERROR;
    UDate today = Calendar::getNow();
    int32_t year,month,day,hour,min,sec; dateToFields(today,year,month,day,hour,min,sec);
    UDate expected = date(year, month, day);
    logln(UnicodeString("Test Date: ") + dateToString(today));
    SimpleDateFormat* sdf = dynamic_cast<SimpleDateFormat*>(DateFormat::createDateInstance());
    if (sdf == nullptr){
        dataerrln("Error calling DateFormat::createDateInstance()");
        return;
    }
    tryPattern(*sdf, today, nullptr, expected);
    tryPattern(*sdf, today, "G yyyy DDD", expected);
    delete sdf;
    if (U_FAILURE(status)) errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
}

// -------------------------------------

void
DateFormatTest::tryPattern(SimpleDateFormat& sdf, UDate d, const char* pattern, UDate expected)
{
    UErrorCode status = U_ZERO_ERROR;
    if (pattern != nullptr) sdf.applyPattern(pattern);
    UnicodeString thePat;
    logln(UnicodeString("pattern: ") + sdf.toPattern(thePat));
    UnicodeString formatResult; (*dynamic_cast<DateFormat*>(&sdf)).format(d, formatResult);
    logln(UnicodeString(" format -> ") + formatResult);
    // try {
        UDate d2 = sdf.parse(formatResult, status);
        logln(UnicodeString(" parse(") + formatResult + ") -> " + dateToString(d2));
        if (d2 != expected) errln(UnicodeString("FAIL: Expected ") + dateToString(expected));
        UnicodeString format2; (*dynamic_cast<DateFormat*>(&sdf)).format(d2, format2);
        logln(UnicodeString(" format -> ") + format2);
        if (!(formatResult == format2)) errln(UnicodeString("FAIL: Round trip drift"));
    //}
    //catch(Exception e) {
    if (U_FAILURE(status))
        errln(UnicodeString("Error: ") + static_cast<int32_t>(status));
    //}
}

// -------------------------------------

/**
 * Test the handling of single quotes in patterns.
 */
void
DateFormatTest::TestQuotePattern161()
{
    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat* formatter = new SimpleDateFormat(UnicodeString("MM/dd/yyyy 'at' hh:mm:ss a zzz"), status);
    if (U_FAILURE(status)) {
        dataerrln("Fail new SimpleDateFormat: %s", u_errorName(status));
        delete formatter;
        return;
    }
    UDate currentTime_1 = date(97, UCAL_AUGUST, 13, 10, 42, 28);
    UnicodeString dateString; (dynamic_cast<DateFormat*>(formatter))->format(currentTime_1, dateString);
    UnicodeString exp("08/13/1997 at 10:42:28 AM ");
    logln(UnicodeString("format(") + dateToString(currentTime_1) + ") = " + dateString);
    if (0 != dateString.compareBetween(0, exp.length(), exp, 0, exp.length())) errln(UnicodeString("FAIL: Expected ") + exp);
    delete formatter;
    if (U_FAILURE(status)) errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
}

// -------------------------------------

/**
 * Verify the correct behavior when handling invalid input strings.
 */
void
DateFormatTest::TestBadInput135()
{
    UErrorCode status = U_ZERO_ERROR;
    DateFormat::EStyle looks[] = {
        DateFormat::SHORT, DateFormat::MEDIUM, DateFormat::LONG, DateFormat::FULL
    };
    int32_t looks_length = UPRV_LENGTHOF(looks);
    const char* strings[] = {
        "Mar 15", "Mar 15 1997", "asdf", "3/1/97 1:23:", "3/1/00 1:23:45 AM"
    };
    int32_t strings_length = UPRV_LENGTHOF(strings);
    DateFormat *longFmt = DateFormat::createDateTimeInstance(DateFormat::LONG, DateFormat::MEDIUM, Locale::getEnglish());
    if(longFmt==nullptr) {
      dataerrln("could not create date time instance");
      return;
    }
    UnicodeString expected(u"March 1, 2000 at 1:23:45\u202FAM", -1);
    for (int32_t i = 0; i < strings_length;++i) {
        const char* text = strings[i];
        for (int32_t j = 0; j < looks_length;++j) {
            DateFormat::EStyle dateLook = looks[j];
            for (int32_t k = 0; k < looks_length;++k) {
                DateFormat::EStyle timeLook = looks[k];
                DateFormat *df = DateFormat::createDateTimeInstance(dateLook, timeLook);
                if (df == nullptr){
                    dataerrln("Error calling DateFormat::createDateTimeInstance()");
                    continue;
                }
                UnicodeString prefix = UnicodeString(text) + ", " + dateLook + "/" + timeLook + ": ";
                //try {
                    UDate when = df->parse(text, status);
                    if (when == 0 && U_SUCCESS(status)) {
                        errln(prefix + "SHOULD NOT HAPPEN: parse returned 0.");
                        continue;
                    }
                    if (U_SUCCESS(status))
                    {
                        UnicodeString format;
                        UnicodeString pattern;
                        SimpleDateFormat* sdtfmt = dynamic_cast<SimpleDateFormat*>(df);
                        if (sdtfmt != nullptr) {
                            sdtfmt->toPattern(pattern);
                        }
                        longFmt->format(when, format);
                        logln(prefix + "OK: " + format);
                        if (0!=format.compareBetween(0, expected.length(), expected, 0, expected.length()))
                            errln(UnicodeString("FAIL: Parse \"") + text + "\", pattern \"" + pattern + "\", expected " + expected + " got " + format);
                    }
                //}
                //catch(ParseException e) {
                    else
                        status = U_ZERO_ERROR;
                //}
                //catch(StringIndexOutOfBoundsException e) {
                //    errln(prefix + "SHOULD NOT HAPPEN: " + (int)status);
                //}
                delete df;
            }
        }
    }
    delete longFmt;
    if (U_FAILURE(status))
        errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
}

static const char* const parseFormats[] = {
    "MMMM d, yyyy",
    "MMMM d yyyy",
    "M/d/yy",
    "d MMMM, yyyy",
    "d MMMM yyyy",
    "d MMMM",
    "MMMM d",
    "yyyy",
    "h:mm a MMMM d, yyyy"
};

#if 0
// strict inputStrings
static const char* const inputStrings[] = {
    "bogus string", 0, 0, 0, 0, 0, 0, 0, 0, 0,
    "April 1, 1997", "April 1, 1997", 0, 0, 0, 0, 0, "April 1", 0, 0,
    "Jan 1, 1970", "January 1, 1970", 0, 0, 0, 0, 0, "January 1", 0, 0,
    "Jan 1 2037", 0, "January 1 2037", 0, 0, 0, 0, "January 1", 0, 0,
    "1/1/70", 0, 0, "1/1/70", 0, 0, 0, 0, "0001", 0,
    "5 May 1997", 0, 0, 0, 0, "5 May 1997", "5 May", 0, "0005", 0,
    "16 May", 0, 0, 0, 0, 0, "16 May", 0, "0016", 0,
    "April 30", 0, 0, 0, 0, 0, 0, "April 30", 0, 0,
    "1998", 0, 0, 0, 0, 0, 0, 0, "1998", 0,
    "1", 0, 0, 0, 0, 0, 0, 0, "0001", 0,
    "3:00 pm Jan 1, 1997", 0, 0, 0, 0, 0, 0, 0, "0003", "3:00 PM January 1, 1997",
};
#else
// lenient inputStrings
static const char* const inputStrings[] = {
    "bogus string", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    "April 1, 1997", "April 1, 1997", "April 1 1997", "4/1/97", nullptr, nullptr, nullptr, "April 1", nullptr, nullptr,
    "Jan 1, 1970", "January 1, 1970", "January 1 1970", "1/1/70", nullptr, nullptr, nullptr, "January 1", nullptr, nullptr,
    "Jan 1 2037", "January 1, 2037", "January 1 2037", "1/1/37", nullptr, nullptr, nullptr, "January 1", nullptr, nullptr,
    "1/1/70", "January 1, 0070", "January 1 0070", "1/1/70", "1 January, 0070", "1 January 0070", "1 January", "January 1", "0001", nullptr,
    "5 May 1997", nullptr, nullptr, nullptr, "5 May, 1997", "5 May 1997", "5 May", nullptr, "0005", nullptr,
    "16 May", nullptr, nullptr, nullptr, nullptr, nullptr, "16 May", nullptr, "0016", nullptr,
    "April 30", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "April 30", nullptr, nullptr,
    "1998", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "1998", nullptr,
    "1", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "0001", nullptr,
    "3:00 pm Jan 1, 1997", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "0003", "3:00 PM January 1, 1997",
};
#endif

// -------------------------------------

/**
 * Verify the correct behavior when parsing an array of inputs against an
 * array of patterns, with known results.  The results are encoded after
 * the input strings in each row.
 */
void
DateFormatTest::TestBadInput135a()
{
  UErrorCode status = U_ZERO_ERROR;
  SimpleDateFormat* dateParse = new SimpleDateFormat(status);
  if(U_FAILURE(status)) {
    dataerrln("Failed creating SimpleDateFormat with %s. Quitting test", u_errorName(status));
    delete dateParse;
    return;
  }
  const char* s;
  UDate date;
  const uint32_t PF_LENGTH = UPRV_LENGTHOF(parseFormats);
  const uint32_t INPUT_LENGTH = UPRV_LENGTHOF(inputStrings);

  dateParse->applyPattern("d MMMM, yyyy");
  dateParse->adoptTimeZone(TimeZone::createDefault());
  s = "not parseable";
  UnicodeString thePat;
  logln(UnicodeString("Trying to parse \"") + s + "\" with " + dateParse->toPattern(thePat));
  //try {
  date = dateParse->parse(s, status);
  if (U_SUCCESS(status))
    errln(UnicodeString("FAIL: Expected exception during parse"));
  //}
  //catch(Exception ex) {
  else
    logln(UnicodeString("Exception during parse: ") + static_cast<int32_t>(status));
  status = U_ZERO_ERROR;
  //}
  for (uint32_t i = 0; i < INPUT_LENGTH; i += (PF_LENGTH + 1)) {
    ParsePosition parsePosition(0);
    UnicodeString s( inputStrings[i]);
    for (uint32_t index = 0; index < PF_LENGTH;++index) {
      const char* expected = inputStrings[i + 1 + index];
      dateParse->applyPattern(parseFormats[index]);
      dateParse->adoptTimeZone(TimeZone::createDefault());
      //try {
      parsePosition.setIndex(0);
      date = dateParse->parse(s, parsePosition);
      if (parsePosition.getIndex() != 0) {
        UnicodeString s1, s2;
        s.extract(0, parsePosition.getIndex(), s1);
        s.extract(parsePosition.getIndex(), s.length(), s2);
        if (date == 0) {
          errln(UnicodeString("ERROR: null result fmt=\"") +
                     parseFormats[index] +
                     "\" pos=" + parsePosition.getIndex() + " " +
                     s1 + "|" + s2);
        }
        else {
          UnicodeString result;
          (dynamic_cast<DateFormat*>(dateParse))->format(date, result);
          logln(UnicodeString("Parsed \"") + s + "\" using \"" + dateParse->toPattern(thePat) + "\" to: " + result);
          if (expected == nullptr)
            errln(UnicodeString("FAIL: Expected parse failure, got ") + result);
          else if (!(result == expected))
            errln(UnicodeString("FAIL: Parse \"") + s + UnicodeString("\", expected ") + expected + UnicodeString(", got ") + result);
        }
      }
      else if (expected != nullptr) {
        errln(UnicodeString("FAIL: Expected ") + expected + " from \"" +
                     s + "\" with \"" + dateParse->toPattern(thePat) + "\"");
      }
      //}
      //catch(Exception ex) {
      if (U_FAILURE(status))
        errln(UnicodeString("An exception was thrown during parse: ") + static_cast<int32_t>(status));
      //}
    }
  }
  delete dateParse;
  if (U_FAILURE(status))
    errln(UnicodeString("FAIL: UErrorCode received during test: ") + static_cast<int32_t>(status));
}

// -------------------------------------

/**
 * Test the parsing of two-digit years.
 */
void
DateFormatTest::TestTwoDigitYear()
{
    UErrorCode ec = U_ZERO_ERROR;
    SimpleDateFormat fmt("dd/MM/yy", Locale::getUK(), ec);
    if (U_FAILURE(ec)) {
        dataerrln("FAIL: SimpleDateFormat constructor - %s", u_errorName(ec));
        return;
    }
    parse2DigitYear(fmt, "5/6/30", date(130, UCAL_JUNE, 5));
    parse2DigitYear(fmt, "4/6/50", date(50, UCAL_JUNE, 4));
}

// -------------------------------------

void
DateFormatTest::parse2DigitYear(DateFormat& fmt, const char* str, UDate expected)
{
    UErrorCode status = U_ZERO_ERROR;
    //try {
        UDate d = fmt.parse(str, status);
        UnicodeString thePat;
        logln(UnicodeString("Parsing \"") + str + "\" with " + (dynamic_cast<SimpleDateFormat*>(&fmt))->toPattern(thePat) +
            "  => " + dateToString(d));
        if (d != expected) errln(UnicodeString("FAIL: Expected ") + expected);
    //}
    //catch(ParseException e) {
        if (U_FAILURE(status))
        errln(UnicodeString("FAIL: Got exception"));
    //}
}

// -------------------------------------

/**
 * Test the formatting of time zones.
 */
void
DateFormatTest::TestDateFormatZone061()
{
    UErrorCode status = U_ZERO_ERROR;
    UDate date;
    DateFormat *formatter;
    date= 859248000000.0;
    logln(UnicodeString("Date 1997/3/25 00:00 GMT: ") + date);
    formatter = new SimpleDateFormat(UnicodeString("dd-MMM-yyyyy HH:mm"), Locale::getUK(), status);
    if(U_FAILURE(status)) {
      dataerrln("Failed creating SimpleDateFormat with %s. Quitting test", u_errorName(status));
      delete formatter;
      return;
    }
    formatter->adoptTimeZone(TimeZone::createTimeZone("GMT"));
    UnicodeString temp; formatter->format(date, temp);
    logln(UnicodeString("Formatted in GMT to: ") + temp);
    //try {
        UDate tempDate = formatter->parse(temp, status);
        logln(UnicodeString("Parsed to: ") + dateToString(tempDate));
        if (tempDate != date) errln(UnicodeString("FAIL: Expected ") + dateToString(date));
    //}
    //catch(Throwable t) {
    if (U_FAILURE(status))
        errln(UnicodeString("Date Formatter throws: ") + static_cast<int32_t>(status));
    //}
    delete formatter;
}

// -------------------------------------

/**
 * Test the formatting of time zones.
 */
void
DateFormatTest::TestDateFormatZone146()
{
    TimeZone *saveDefault = TimeZone::createDefault();

        //try {
    TimeZone *thedefault = TimeZone::createTimeZone("GMT");
    TimeZone::setDefault(*thedefault);
            // java.util.Locale.setDefault(new java.util.Locale("ar", "", ""));

            // check to be sure... its GMT all right
        TimeZone *testdefault = TimeZone::createDefault();
        UnicodeString testtimezone;
        testdefault->getID(testtimezone);
        if (testtimezone == "GMT")
            logln("Test timezone = " + testtimezone);
        else
            dataerrln("Test timezone should be GMT, not " + testtimezone);

        UErrorCode status = U_ZERO_ERROR;
        // now try to use the default GMT time zone
        GregorianCalendar *greenwichcalendar =
            new GregorianCalendar(1997, 3, 4, 23, 0, status);
        if (U_FAILURE(status)) {
            dataerrln("Fail new GregorianCalendar: %s", u_errorName(status));
        } else {
            //*****************************greenwichcalendar.setTimeZone(TimeZone.getDefault());
            //greenwichcalendar.set(1997, 3, 4, 23, 0);
            // try anything to set hour to 23:00 !!!
            greenwichcalendar->set(UCAL_HOUR_OF_DAY, 23);
            // get time
            UDate greenwichdate = greenwichcalendar->getTime(status);
            // format every way
            UnicodeString DATA [] = {
                UnicodeString("simple format:  "), UnicodeString("04/04/97 23:00 GMT"),
                    UnicodeString("MM/dd/yy HH:mm z"),
                UnicodeString("full format:    "), UnicodeString("Friday, April 4, 1997 11:00:00 o'clock PM GMT"),
                    UnicodeString("EEEE, MMMM d, yyyy h:mm:ss 'o''clock' a z"),
                UnicodeString("long format:    "), UnicodeString("April 4, 1997 11:00:00 PM GMT"),
                    UnicodeString("MMMM d, yyyy h:mm:ss a z"),
                UnicodeString("default format: "), UnicodeString("04-Apr-97 11:00:00 PM"),
                    UnicodeString("dd-MMM-yy h:mm:ss a"),
                UnicodeString("short format:   "), UnicodeString("4/4/97 11:00 PM"),
                    UnicodeString("M/d/yy h:mm a")
            };
            int32_t DATA_length = UPRV_LENGTHOF(DATA);

            for (int32_t i=0; i<DATA_length; i+=3) {
                SimpleDateFormat fmt(DATA[i+2], Locale::getEnglish(), status);
                if (U_FAILURE(status)) {
                    dataerrln("Unable to create SimpleDateFormat - %s", u_errorName(status));
                    break;
                }
                fmt.setCalendar(*greenwichcalendar);
                UnicodeString result;
                result = fmt.format(greenwichdate, result);
                logln(DATA[i] + result);
                if (result != DATA[i+1])
                    errln("FAIL: Expected " + DATA[i+1] + ", got " + result);
            }
        }
    //}
    //finally {
        TimeZone::adoptDefault(saveDefault);
    //}
        delete testdefault;
        delete greenwichcalendar;
        delete thedefault;


}

// -------------------------------------

/**
 * Test the formatting of dates in different locales.
 */
void
DateFormatTest::TestLocaleDateFormat() // Bug 495
{
    UDate testDate = date(97, UCAL_SEPTEMBER, 15);
    DateFormat *dfFrench = DateFormat::createDateTimeInstance(DateFormat::FULL,
        DateFormat::FULL, Locale::getFrench());
    DateFormat *dfUS = DateFormat::createDateTimeInstance(DateFormat::FULL,
        DateFormat::FULL, Locale::getUS());
    UnicodeString expectedFRENCH ( u"lundi 15 septembre 1997 à 00:00:00 heure d’été du Pacifique nord-américain", -1 );
    expectedFRENCH = expectedFRENCH.unescape();
    UnicodeString expectedUS ( u"Monday, September 15, 1997 at 12:00:00\u202FAM Pacific Daylight Time", -1 );
    logln(UnicodeString("Date set to : ") + dateToString(testDate));
    UnicodeString out;
    if (dfUS == nullptr || dfFrench == nullptr){
        dataerrln("Error calling DateFormat::createDateTimeInstance)");
        delete dfUS;
        delete dfFrench;
        return;
    }

    dfFrench->format(testDate, out);
    logln(UnicodeString("Date Formatted with French Locale ") + out);
    if (!(out == expectedFRENCH))
        errln(UnicodeString("FAIL: Expected ") + expectedFRENCH + ", got " + out);
    out.truncate(0);
    dfUS->format(testDate, out);
    logln(UnicodeString("Date Formatted with US Locale ") + out);
    if (!(out == expectedUS))
        errln(UnicodeString("FAIL: Expected ") + expectedUS + ", got " + out);
    delete dfUS;
    delete dfFrench;
}

void
DateFormatTest::TestFormattingLocaleTimeSeparator()
{
    // This test not as useful as it once was, since timeSeparator
    // in the Arabic locale is changed back to ":" in CLDR 28.
    const UDate testDate = 874266720000.;  // Sun Sep 14 21:52:00 CET 1997
    logln(UnicodeString("Date set to : ") + dateToString(testDate));

    const LocalPointer<const TimeZone> tz(TimeZone::createTimeZone("CET"));

    const LocalPointer<DateFormat> dfArab(DateFormat::createTimeInstance(
            DateFormat::SHORT, Locale("ar", "EG")));

    const LocalPointer<DateFormat> dfLatn(DateFormat::createTimeInstance(
            DateFormat::SHORT, Locale("ar", "EG", nullptr, "numbers=latn")));

    if (dfLatn.isNull() || dfArab.isNull()) {
        dataerrln("Error calling DateFormat::createTimeInstance()");
        return;
    }

    dfArab->setTimeZone(*tz);
    dfLatn->setTimeZone(*tz);

    const UnicodeString expectedArab = UnicodeString(
#if APPLE_ICU_CHANGES
// rdar://
            "\\u0669:\\u0665\\u0662\\u00A0\\u0645", -1, US_INV).unescape();
#else
            "\\u0669:\\u0665\\u0662 \\u0645", -1, US_INV).unescape();
#endif  // APPLE_ICU_CHANGES

    const UnicodeString expectedLatn = UnicodeString(
#if APPLE_ICU_CHANGES
// rdar://
            "9:52\\u00A0\\u0645", -1, US_INV).unescape();
#else
            "9:52 \\u0645", -1, US_INV).unescape();
#endif  // APPLE_ICU_CHANGES

    UnicodeString actualArab;
    UnicodeString actualLatn;

    dfArab->format(testDate, actualArab);
    dfLatn->format(testDate, actualLatn);

    assertEquals("Arab", expectedArab, actualArab);
    assertEquals("Latn", expectedLatn, actualLatn);
}

/**
 * Test DateFormat(Calendar) API
 */
void DateFormatTest::TestDateFormatCalendar() {
    DateFormat *date = nullptr, *time = nullptr, *full = nullptr;
    Calendar* cal = nullptr;
    UnicodeString str;
    ParsePosition pos;
    UDate when;
    UErrorCode ec = U_ZERO_ERROR;

    /* Create a formatter for date fields. */
    date = DateFormat::createDateInstance(DateFormat::kShort, Locale::getUS());
    if (date == nullptr) {
        dataerrln("FAIL: createDateInstance failed");
        goto FAIL;
    }

    /* Create a formatter for time fields. */
    time = DateFormat::createTimeInstance(DateFormat::kShort, Locale::getUS());
    if (time == nullptr) {
        errln("FAIL: createTimeInstance failed");
        goto FAIL;
    }

    /* Create a full format for output */
    full = DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull,
                                              Locale::getUS());
    if (full == nullptr) {
        errln("FAIL: createInstance failed");
        goto FAIL;
    }

    /* Create a calendar */
    cal = Calendar::createInstance(Locale::getUS(), ec);
    if (cal == nullptr || U_FAILURE(ec)) {
        errln(UnicodeString("FAIL: Calendar::createInstance failed with ") +
              u_errorName(ec));
        goto FAIL;
    }

    /* Parse the date */
    cal->clear();
    str = UnicodeString("4/5/2001", "");
    pos.setIndex(0);
    date->parse(str, *cal, pos);
    if (pos.getIndex() != str.length()) {
        errln(UnicodeString("FAIL: DateFormat::parse(4/5/2001) failed at ") +
              pos.getIndex());
        goto FAIL;
    }

    /* Parse the time */
    str = UnicodeString("5:45 PM", "");
    pos.setIndex(0);
    time->parse(str, *cal, pos);
    if (pos.getIndex() != str.length()) {
        errln(UnicodeString("FAIL: DateFormat::parse(17:45) failed at ") +
              pos.getIndex());
        goto FAIL;
    }

    /* Check result */
    when = cal->getTime(ec);
    if (U_FAILURE(ec)) {
        errln(UnicodeString("FAIL: cal->getTime() failed with ") + u_errorName(ec));
        goto FAIL;
    }
    str.truncate(0);
    full->format(when, str);
    // Thursday, April 5, 2001 5:45:00 PM PDT 986517900000
    if (when == 986517900000.0) {
        logln("Ok: Parsed result: " + str);
    } else {
        errln("FAIL: Parsed result: " + str + ", exp 4/5/2001 5:45 PM");
    }

 FAIL:
    delete date;
    delete time;
    delete full;
    delete cal;
}

/**
 * Test DateFormat's parsing of space characters.  See jitterbug 1916.
 */
void DateFormatTest::TestSpaceParsing() {
    const char* DATA[] = {
        "yyyy MM dd HH:mm:ss",

        // pattern, input, expected parse or nullptr if expect parse failure
        "MMMM d yy", " 04 05 06",  "2006 04 05 00:00:00",
        nullptr,        "04 05 06",   "2006 04 05 00:00:00",

        "MM d yy",   " 04 05 06",    "2006 04 05 00:00:00",
        nullptr,        "04 05 06",     "2006 04 05 00:00:00",
        nullptr,        "04/05/06",     "2006 04 05 00:00:00",
        nullptr,        "04-05-06",     "2006 04 05 00:00:00",
        nullptr,        "04.05.06",     "2006 04 05 00:00:00",
        nullptr,        "04 / 05 / 06", "2006 04 05 00:00:00",
        nullptr,        "Apr / 05/ 06", "2006 04 05 00:00:00",
        nullptr,        "Apr-05-06",    "2006 04 05 00:00:00",
        nullptr,        "Apr 05, 2006", "2006 04 05 00:00:00",

        "MMMM d yy", " Apr 05 06", "2006 04 05 00:00:00",
        nullptr,        "Apr 05 06",  "2006 04 05 00:00:00",
        nullptr,        "Apr05 06",   "2006 04 05 00:00:00",

        "hh:mm:ss a", "12:34:56 PM", "1970 01 01 12:34:56",
        nullptr,         "12:34:56PM",  "1970 01 01 12:34:56",
        nullptr,         "12.34.56PM",  "1970 01 01 12:34:56",
        nullptr,         "12 : 34 : 56  PM", "1970 01 01 12:34:56",

        "MM d yy 'at' hh:mm:ss a", "04/05/06 12:34:56 PM", "2006 04 05 12:34:56",

        "MMMM dd yyyy hh:mm a", "September 27, 1964 21:56 PM", "1964 09 28 09:56:00",
        nullptr,                   "November 4, 2008 0:13 AM",    "2008 11 04 00:13:00",

        "HH'h'mm'min'ss's'", "12h34min56s", "1970 01 01 12:34:56",
        nullptr,                "12h34mi56s",  "1970 01 01 12:34:56",
        nullptr,                "12h34m56s",   "1970 01 01 12:34:56",
        nullptr,                "12:34:56",    "1970 01 01 12:34:56"
    };
    const int32_t DATA_len = UPRV_LENGTHOF(DATA);

    expectParse(DATA, DATA_len, Locale("en"));
}

/**
 * Test handling of "HHmmss" pattern.
 */
void DateFormatTest::TestExactCountFormat() {
    const char* DATA[] = {
        "yyyy MM dd HH:mm:ss",

        // pattern, input, expected parse or nullptr if expect parse failure
        "HHmmss", "123456", "1970 01 01 12:34:56",
        nullptr,     "12345",  "1970 01 01 01:23:45",
        nullptr,     "1234",   nullptr,
        nullptr,     "00-05",  nullptr,
        nullptr,     "12-34",  nullptr,
        nullptr,     "00+05",  nullptr,
        "ahhmm",  "PM730",  "1970 01 01 19:30:00",
    };
    const int32_t DATA_len = UPRV_LENGTHOF(DATA);

    expectParse(DATA, DATA_len, Locale("en"));
}

/**
 * Test handling of white space.
 */
void DateFormatTest::TestWhiteSpaceParsing() {
    const char* DATA[] = {
        "yyyy MM dd",

        // pattern, input, expected parse or null if expect parse failure

        // Pattern space run should parse input text space run
        "MM   d yy",   " 04 01 03",    "2003 04 01",
        nullptr,          " 04  01   03 ", "2003 04 01",
    };
    const int32_t DATA_len = UPRV_LENGTHOF(DATA);

    expectParse(DATA, DATA_len, Locale("en"));
}


void DateFormatTest::TestInvalidPattern() {
    UErrorCode ec = U_ZERO_ERROR;
    SimpleDateFormat f(UnicodeString("Yesterday"), ec);
    if (U_FAILURE(ec)) {
        dataerrln("Fail construct SimpleDateFormat: %s", u_errorName(ec));
        return;
    }
    UnicodeString out;
    FieldPosition pos;
    f.format(static_cast<UDate>(0), out, pos);
    logln(out);
    // The bug is that the call to format() will crash.  By not
    // crashing, the test passes.
}

void DateFormatTest::TestGreekMay() {
    UErrorCode ec = U_ZERO_ERROR;
    UDate date = -9896080848000.0;
    SimpleDateFormat fmt("EEEE, dd MMMM yyyy h:mm:ss a", Locale("el", "", ""), ec);
    if (U_FAILURE(ec)) {
        dataerrln("Fail construct SimpleDateFormat: %s", u_errorName(ec));
        return;
    }
    UnicodeString str;
    fmt.format(date, str);
    ParsePosition pos(0);
    UDate d2 = fmt.parse(str, pos);
    if (date != d2) {
        errln("FAIL: unable to parse strings where case-folding changes length");
    }
}

void DateFormatTest::TestStandAloneMonths()
{
    const char *EN_DATA[] = {
        "yyyy MM dd HH:mm:ss",

        "yyyy LLLL dd H:mm:ss", "fp", "2004 03 10 16:36:31", "2004 March 10 16:36:31", "2004 03 10 16:36:31",
        "yyyy LLL dd H:mm:ss",  "fp", "2004 03 10 16:36:31", "2004 Mar 10 16:36:31",   "2004 03 10 16:36:31",
        "yyyy LLLL dd H:mm:ss", "F",  "2004 03 10 16:36:31", "2004 March 10 16:36:31",
        "yyyy LLL dd H:mm:ss",  "pf", "2004 Mar 10 16:36:31", "2004 03 10 16:36:31", "2004 Mar 10 16:36:31",

        "LLLL", "fp", "1970 01 01 0:00:00", "January",   "1970 01 01 0:00:00",
        "LLLL", "fp", "1970 02 01 0:00:00", "February",  "1970 02 01 0:00:00",
        "LLLL", "fp", "1970 03 01 0:00:00", "March",     "1970 03 01 0:00:00",
        "LLLL", "fp", "1970 04 01 0:00:00", "April",     "1970 04 01 0:00:00",
        "LLLL", "fp", "1970 05 01 0:00:00", "May",       "1970 05 01 0:00:00",
        "LLLL", "fp", "1970 06 01 0:00:00", "June",      "1970 06 01 0:00:00",
        "LLLL", "fp", "1970 07 01 0:00:00", "July",      "1970 07 01 0:00:00",
        "LLLL", "fp", "1970 08 01 0:00:00", "August",    "1970 08 01 0:00:00",
        "LLLL", "fp", "1970 09 01 0:00:00", "September", "1970 09 01 0:00:00",
        "LLLL", "fp", "1970 10 01 0:00:00", "October",   "1970 10 01 0:00:00",
        "LLLL", "fp", "1970 11 01 0:00:00", "November",  "1970 11 01 0:00:00",
        "LLLL", "fp", "1970 12 01 0:00:00", "December",  "1970 12 01 0:00:00",

        "LLL", "fp", "1970 01 01 0:00:00", "Jan", "1970 01 01 0:00:00",
        "LLL", "fp", "1970 02 01 0:00:00", "Feb", "1970 02 01 0:00:00",
        "LLL", "fp", "1970 03 01 0:00:00", "Mar", "1970 03 01 0:00:00",
        "LLL", "fp", "1970 04 01 0:00:00", "Apr", "1970 04 01 0:00:00",
        "LLL", "fp", "1970 05 01 0:00:00", "May", "1970 05 01 0:00:00",
        "LLL", "fp", "1970 06 01 0:00:00", "Jun", "1970 06 01 0:00:00",
        "LLL", "fp", "1970 07 01 0:00:00", "Jul", "1970 07 01 0:00:00",
        "LLL", "fp", "1970 08 01 0:00:00", "Aug", "1970 08 01 0:00:00",
        "LLL", "fp", "1970 09 01 0:00:00", "Sep", "1970 09 01 0:00:00",
        "LLL", "fp", "1970 10 01 0:00:00", "Oct", "1970 10 01 0:00:00",
        "LLL", "fp", "1970 11 01 0:00:00", "Nov", "1970 11 01 0:00:00",
        "LLL", "fp", "1970 12 01 0:00:00", "Dec", "1970 12 01 0:00:00",
    };

    const char *CS_DATA[] = {
        "yyyy MM dd HH:mm:ss",

        "yyyy LLLL dd H:mm:ss", "fp", "2004 04 10 16:36:31", "2004 duben 10 16:36:31", "2004 04 10 16:36:31",
        "yyyy MMMM dd H:mm:ss", "fp", "2004 04 10 16:36:31", "2004 dubna 10 16:36:31", "2004 04 10 16:36:31",
        "yyyy LLL dd H:mm:ss",  "fp", "2004 04 10 16:36:31", "2004 dub 10 16:36:31",   "2004 04 10 16:36:31",
        "yyyy LLLL dd H:mm:ss", "F",  "2004 04 10 16:36:31", "2004 duben 10 16:36:31",
        "yyyy MMMM dd H:mm:ss", "F",  "2004 04 10 16:36:31", "2004 dubna 10 16:36:31",
        "yyyy LLLL dd H:mm:ss", "pf", "2004 duben 10 16:36:31", "2004 04 10 16:36:31", "2004 duben 10 16:36:31",
        "yyyy MMMM dd H:mm:ss", "pf", "2004 dubna 10 16:36:31", "2004 04 10 16:36:31", "2004 dubna 10 16:36:31",

        "LLLL", "fp", "1970 01 01 0:00:00", "leden",               "1970 01 01 0:00:00",
        "LLLL", "fp", "1970 02 01 0:00:00", "\\u00FAnor",           "1970 02 01 0:00:00",
        "LLLL", "fp", "1970 03 01 0:00:00", "b\\u0159ezen",         "1970 03 01 0:00:00",
        "LLLL", "fp", "1970 04 01 0:00:00", "duben",               "1970 04 01 0:00:00",
        "LLLL", "fp", "1970 05 01 0:00:00", "kv\\u011Bten",         "1970 05 01 0:00:00",
        "LLLL", "fp", "1970 06 01 0:00:00", "\\u010Derven",         "1970 06 01 0:00:00",
        "LLLL", "fp", "1970 07 01 0:00:00", "\\u010Dervenec",       "1970 07 01 0:00:00",
        "LLLL", "fp", "1970 08 01 0:00:00", "srpen",               "1970 08 01 0:00:00",
        "LLLL", "fp", "1970 09 01 0:00:00", "z\\u00E1\\u0159\\u00ED", "1970 09 01 0:00:00",
        "LLLL", "fp", "1970 10 01 0:00:00", "\\u0159\\u00EDjen",     "1970 10 01 0:00:00",
        "LLLL", "fp", "1970 11 01 0:00:00", "listopad",            "1970 11 01 0:00:00",
        "LLLL", "fp", "1970 12 01 0:00:00", "prosinec",            "1970 12 01 0:00:00",

        "LLL", "fp", "1970 01 01 0:00:00", "led",  "1970 01 01 0:00:00",
        "LLL", "fp", "1970 02 01 0:00:00", "\\u00FAno",  "1970 02 01 0:00:00",
        "LLL", "fp", "1970 03 01 0:00:00", "b\\u0159e",  "1970 03 01 0:00:00",
        "LLL", "fp", "1970 04 01 0:00:00", "dub",  "1970 04 01 0:00:00",
        "LLL", "fp", "1970 05 01 0:00:00", "kv\\u011B",  "1970 05 01 0:00:00",
        "LLL", "fp", "1970 06 01 0:00:00", "\\u010Dvn",  "1970 06 01 0:00:00",
        "LLL", "fp", "1970 07 01 0:00:00", "\\u010Dvc",  "1970 07 01 0:00:00",
        "LLL", "fp", "1970 08 01 0:00:00", "srp",  "1970 08 01 0:00:00",
        "LLL", "fp", "1970 09 01 0:00:00", "z\\u00E1\\u0159",  "1970 09 01 0:00:00",
        "LLL", "fp", "1970 10 01 0:00:00", "\\u0159\\u00EDj", "1970 10 01 0:00:00",
        "LLL", "fp", "1970 11 01 0:00:00", "lis", "1970 11 01 0:00:00",
        "LLL", "fp", "1970 12 01 0:00:00", "pro", "1970 12 01 0:00:00",
    };

    expect(EN_DATA, UPRV_LENGTHOF(EN_DATA), Locale("en", "", ""));
    expect(CS_DATA, UPRV_LENGTHOF(CS_DATA), Locale("cs", "", ""));
}

void DateFormatTest::TestStandAloneDays()
{
    const char *EN_DATA[] = {
        "yyyy MM dd HH:mm:ss",

        "cccc", "fp", "1970 01 04 0:00:00", "Sunday",    "1970 01 04 0:00:00",
        "cccc", "fp", "1970 01 05 0:00:00", "Monday",    "1970 01 05 0:00:00",
        "cccc", "fp", "1970 01 06 0:00:00", "Tuesday",   "1970 01 06 0:00:00",
        "cccc", "fp", "1970 01 07 0:00:00", "Wednesday", "1970 01 07 0:00:00",
        "cccc", "fp", "1970 01 01 0:00:00", "Thursday",  "1970 01 01 0:00:00",
        "cccc", "fp", "1970 01 02 0:00:00", "Friday",    "1970 01 02 0:00:00",
        "cccc", "fp", "1970 01 03 0:00:00", "Saturday",  "1970 01 03 0:00:00",

        "ccc", "fp", "1970 01 04 0:00:00", "Sun", "1970 01 04 0:00:00",
        "ccc", "fp", "1970 01 05 0:00:00", "Mon", "1970 01 05 0:00:00",
        "ccc", "fp", "1970 01 06 0:00:00", "Tue", "1970 01 06 0:00:00",
        "ccc", "fp", "1970 01 07 0:00:00", "Wed", "1970 01 07 0:00:00",
        "ccc", "fp", "1970 01 01 0:00:00", "Thu", "1970 01 01 0:00:00",
        "ccc", "fp", "1970 01 02 0:00:00", "Fri", "1970 01 02 0:00:00",
        "ccc", "fp", "1970 01 03 0:00:00", "Sat", "1970 01 03 0:00:00",
    };

    const char *CS_DATA[] = {
        "yyyy MM dd HH:mm:ss",

        "cccc", "fp", "1970 01 04 0:00:00", "ned\\u011Ble",       "1970 01 04 0:00:00",
        "cccc", "fp", "1970 01 05 0:00:00", "pond\\u011Bl\\u00ED", "1970 01 05 0:00:00",
        "cccc", "fp", "1970 01 06 0:00:00", "\\u00FAter\\u00FD",   "1970 01 06 0:00:00",
        "cccc", "fp", "1970 01 07 0:00:00", "st\\u0159eda",       "1970 01 07 0:00:00",
        "cccc", "fp", "1970 01 01 0:00:00", "\\u010Dtvrtek",      "1970 01 01 0:00:00",
        "cccc", "fp", "1970 01 02 0:00:00", "p\\u00E1tek",        "1970 01 02 0:00:00",
        "cccc", "fp", "1970 01 03 0:00:00", "sobota",            "1970 01 03 0:00:00",

        "ccc", "fp", "1970 01 04 0:00:00", "ne",      "1970 01 04 0:00:00",
        "ccc", "fp", "1970 01 05 0:00:00", "po",      "1970 01 05 0:00:00",
        "ccc", "fp", "1970 01 06 0:00:00", "\\u00FAt", "1970 01 06 0:00:00",
        "ccc", "fp", "1970 01 07 0:00:00", "st",      "1970 01 07 0:00:00",
        "ccc", "fp", "1970 01 01 0:00:00", "\\u010Dt", "1970 01 01 0:00:00",
        "ccc", "fp", "1970 01 02 0:00:00", "p\\u00E1", "1970 01 02 0:00:00",
        "ccc", "fp", "1970 01 03 0:00:00", "so",      "1970 01 03 0:00:00",
    };

    expect(EN_DATA, UPRV_LENGTHOF(EN_DATA), Locale("en", "", ""));
    expect(CS_DATA, UPRV_LENGTHOF(CS_DATA), Locale("cs", "", ""));
}

void DateFormatTest::TestShortDays()
{
    const char *EN_DATA[] = {
        "yyyy MM dd HH:mm:ss",

        "EEEEEE, MMM d y", "fp", "2013 01 13 0:00:00", "Su, Jan 13 2013", "2013 01 13 0:00:00",
        "EEEEEE, MMM d y", "fp", "2013 01 16 0:00:00", "We, Jan 16 2013", "2013 01 16 0:00:00",
        "EEEEEE d",        "fp", "1970 01 17 0:00:00", "Sa 17",           "1970 01 17 0:00:00",
        "cccccc d",        "fp", "1970 01 17 0:00:00", "Sa 17",           "1970 01 17 0:00:00",
        "cccccc",          "fp", "1970 01 03 0:00:00", "Sa",              "1970 01 03 0:00:00",
    };
    const char *SV_DATA[] = {
        "yyyy MM dd HH:mm:ss",

        "EEEEEE d MMM y",  "fp", "2013 01 13 0:00:00", "s\\u00F6 13 jan. 2013", "2013 01 13 0:00:00",
        "EEEEEE d MMM y",  "fp", "2013 01 16 0:00:00", "on 16 jan. 2013",       "2013 01 16 0:00:00",
        "EEEEEE d",        "fp", "1970 01 17 0:00:00", "l\\u00F6 17",          "1970 01 17 0:00:00",
        "cccccc d",        "fp", "1970 01 17 0:00:00", "l\\u00F6 17",          "1970 01 17 0:00:00",
        "cccccc",          "fp", "1970 01 03 0:00:00", "l\\u00F6",             "1970 01 03 0:00:00",
    };
    expect(EN_DATA, UPRV_LENGTHOF(EN_DATA), Locale("en", "", ""));
    expect(SV_DATA, UPRV_LENGTHOF(SV_DATA), Locale("sv", "", ""));
}

void DateFormatTest::TestNarrowNames()
{
    const char *EN_DATA[] = {
            "yyyy MM dd HH:mm:ss",

            "yyyy MMMMM dd H:mm:ss", "2004 03 10 16:36:31", "2004 M 10 16:36:31",
            "yyyy LLLLL dd H:mm:ss",  "2004 03 10 16:36:31", "2004 M 10 16:36:31",

            "MMMMM", "1970 01 01 0:00:00", "J",
            "MMMMM", "1970 02 01 0:00:00", "F",
            "MMMMM", "1970 03 01 0:00:00", "M",
            "MMMMM", "1970 04 01 0:00:00", "A",
            "MMMMM", "1970 05 01 0:00:00", "M",
            "MMMMM", "1970 06 01 0:00:00", "J",
            "MMMMM", "1970 07 01 0:00:00", "J",
            "MMMMM", "1970 08 01 0:00:00", "A",
            "MMMMM", "1970 09 01 0:00:00", "S",
            "MMMMM", "1970 10 01 0:00:00", "O",
            "MMMMM", "1970 11 01 0:00:00", "N",
            "MMMMM", "1970 12 01 0:00:00", "D",

            "LLLLL", "1970 01 01 0:00:00", "J",
            "LLLLL", "1970 02 01 0:00:00", "F",
            "LLLLL", "1970 03 01 0:00:00", "M",
            "LLLLL", "1970 04 01 0:00:00", "A",
            "LLLLL", "1970 05 01 0:00:00", "M",
            "LLLLL", "1970 06 01 0:00:00", "J",
            "LLLLL", "1970 07 01 0:00:00", "J",
            "LLLLL", "1970 08 01 0:00:00", "A",
            "LLLLL", "1970 09 01 0:00:00", "S",
            "LLLLL", "1970 10 01 0:00:00", "O",
            "LLLLL", "1970 11 01 0:00:00", "N",
            "LLLLL", "1970 12 01 0:00:00", "D",

            "EEEEE", "1970 01 04 0:00:00", "S",
            "EEEEE", "1970 01 05 0:00:00", "M",
            "EEEEE", "1970 01 06 0:00:00", "T",
            "EEEEE", "1970 01 07 0:00:00", "W",
            "EEEEE", "1970 01 01 0:00:00", "T",
            "EEEEE", "1970 01 02 0:00:00", "F",
            "EEEEE", "1970 01 03 0:00:00", "S",

            "ccccc", "1970 01 04 0:00:00", "S",
            "ccccc", "1970 01 05 0:00:00", "M",
            "ccccc", "1970 01 06 0:00:00", "T",
            "ccccc", "1970 01 07 0:00:00", "W",
            "ccccc", "1970 01 01 0:00:00", "T",
            "ccccc", "1970 01 02 0:00:00", "F",
            "ccccc", "1970 01 03 0:00:00", "S",

            "h:mm a",     "2015 01 01 10:00:00", "10:00 AM",
            "h:mm a",     "2015 01 01 22:00:00", "10:00 PM",
            "h:mm aaaaa", "2015 01 01 10:00:00", "10:00 a",
            "h:mm aaaaa", "2015 01 01 22:00:00", "10:00 p",
        };

        const char *CS_DATA[] = {
            "yyyy MM dd HH:mm:ss",

            "yyyy LLLLL dd H:mm:ss", "2004 04 10 16:36:31", "2004 4 10 16:36:31",
            "yyyy MMMMM dd H:mm:ss", "2004 04 10 16:36:31", "2004 4 10 16:36:31",

            "MMMMM", "1970 01 01 0:00:00", "1",
            "MMMMM", "1970 02 01 0:00:00", "2",
            "MMMMM", "1970 03 01 0:00:00", "3",
            "MMMMM", "1970 04 01 0:00:00", "4",
            "MMMMM", "1970 05 01 0:00:00", "5",
            "MMMMM", "1970 06 01 0:00:00", "6",
            "MMMMM", "1970 07 01 0:00:00", "7",
            "MMMMM", "1970 08 01 0:00:00", "8",
            "MMMMM", "1970 09 01 0:00:00", "9",
            "MMMMM", "1970 10 01 0:00:00", "10",
            "MMMMM", "1970 11 01 0:00:00", "11",
            "MMMMM", "1970 12 01 0:00:00", "12",

            "LLLLL", "1970 01 01 0:00:00", "1",
            "LLLLL", "1970 02 01 0:00:00", "2",
            "LLLLL", "1970 03 01 0:00:00", "3",
            "LLLLL", "1970 04 01 0:00:00", "4",
            "LLLLL", "1970 05 01 0:00:00", "5",
            "LLLLL", "1970 06 01 0:00:00", "6",
            "LLLLL", "1970 07 01 0:00:00", "7",
            "LLLLL", "1970 08 01 0:00:00", "8",
            "LLLLL", "1970 09 01 0:00:00", "9",
            "LLLLL", "1970 10 01 0:00:00", "10",
            "LLLLL", "1970 11 01 0:00:00", "11",
            "LLLLL", "1970 12 01 0:00:00", "12",

            "EEEEE", "1970 01 04 0:00:00", "N",
            "EEEEE", "1970 01 05 0:00:00", "P",
            "EEEEE", "1970 01 06 0:00:00", "\\u00DA",
            "EEEEE", "1970 01 07 0:00:00", "S",
            "EEEEE", "1970 01 01 0:00:00", "\\u010C",
            "EEEEE", "1970 01 02 0:00:00", "P",
            "EEEEE", "1970 01 03 0:00:00", "S",

            "ccccc", "1970 01 04 0:00:00", "N",
            "ccccc", "1970 01 05 0:00:00", "P",
            "ccccc", "1970 01 06 0:00:00", "\\u00DA",
            "ccccc", "1970 01 07 0:00:00", "S",
            "ccccc", "1970 01 01 0:00:00", "\\u010C",
            "ccccc", "1970 01 02 0:00:00", "P",
            "ccccc", "1970 01 03 0:00:00", "S",

            "h:mm a",     "2015 01 01 10:00:00", "10:00 dop.",
            "h:mm a",     "2015 01 01 22:00:00", "10:00 odp.",
            "h:mm aaaaa", "2015 01 01 10:00:00", "10:00 dop.",
            "h:mm aaaaa", "2015 01 01 22:00:00", "10:00 odp.",
        };

        const char *CA_DATA[] = {
            "yyyy MM dd HH:mm:ss",

            "h:mm a",     "2015 01 01 10:00:00", "10:00 a.\\u00A0m.",
            "h:mm a",     "2015 01 01 22:00:00", "10:00 p.\\u00A0m.",
#if APPLE_ICU_CHANGES
// rdar://
            "h:mm aaaaa", "2015 01 01 10:00:00", "10:00 a.m.",
            "h:mm aaaaa", "2015 01 01 22:00:00", "10:00 p.m.",
#else
            "h:mm aaaaa", "2015 01 01 10:00:00", "10:00 a.\\u202Fm.",
            "h:mm aaaaa", "2015 01 01 22:00:00", "10:00 p.\\u202Fm.",
#endif  // APPLE_ICU_CHANGES
        };

      expectFormat(EN_DATA, UPRV_LENGTHOF(EN_DATA), Locale("en", "", ""));
      expectFormat(CS_DATA, UPRV_LENGTHOF(CS_DATA), Locale("cs", "", ""));
      expectFormat(CA_DATA, UPRV_LENGTHOF(CA_DATA), Locale("ca", "", ""));
}

void DateFormatTest::TestEras()
{
    const char *EN_DATA[] = {
        "yyyy MM dd",

        "MMMM dd yyyy G",    "fp", "1951 07 17", "July 17 1951 AD",          "1951 07 17",
        "MMMM dd yyyy GG",   "fp", "1951 07 17", "July 17 1951 AD",          "1951 07 17",
        "MMMM dd yyyy GGG",  "fp", "1951 07 17", "July 17 1951 AD",          "1951 07 17",
        "MMMM dd yyyy GGGG", "fp", "1951 07 17", "July 17 1951 Anno Domini", "1951 07 17",

        "MMMM dd yyyy G",    "fp", "-438 07 17", "July 17 0439 BC",            "-438 07 17",
        "MMMM dd yyyy GG",   "fp", "-438 07 17", "July 17 0439 BC",            "-438 07 17",
        "MMMM dd yyyy GGG",  "fp", "-438 07 17", "July 17 0439 BC",            "-438 07 17",
        "MMMM dd yyyy GGGG", "fp", "-438 07 17", "July 17 0439 Before Christ", "-438 07 17",
    };

    expect(EN_DATA, UPRV_LENGTHOF(EN_DATA), Locale("en", "", ""));
}

void DateFormatTest::TestQuarters()
{
    const char *EN_DATA[] = {
        "yyyy MM dd",

        "Q",     "fp", "1970 01 01", "1",           "1970 01 01",
        "QQ",    "fp", "1970 04 01", "02",          "1970 04 01",
        "QQQ",   "fp", "1970 07 01", "Q3",          "1970 07 01",
        "QQQQ",  "fp", "1970 10 01", "4th quarter", "1970 10 01",
        "QQQQQ", "fp", "1970 10 01", "4",           "1970 10 01",

        "q",     "fp", "1970 01 01", "1",           "1970 01 01",
        "qq",    "fp", "1970 04 01", "02",          "1970 04 01",
        "qqq",   "fp", "1970 07 01", "Q3",          "1970 07 01",
        "qqqq",  "fp", "1970 10 01", "4th quarter", "1970 10 01",
        "qqqqq", "fp", "1970 10 01", "4",           "1970 10 01",

        "Qyy",   "fp", "2015 04 01", "215",         "2015 04 01",
        "QQyy",  "fp", "2015 07 01", "0315",        "2015 07 01",
    };
    const char *ES_MX_DATA[] = {
        "yyyy MM dd",

#if APPLE_ICU_CHANGES
// rdar://
        "QQQQ y",  "fp", "1970 01 01", "1er. trimestre 1970", "1970 01 01",
        "QQQ y",   "fp", "1970 01 01", "1er. trim. 1970",     "1970 01 01",
        "QQQQQ y", "fp", "1970 01 01", "1T 1970",             "1970 01 01",
        "qqqq",    "fp", "1970 01 01", "1er. trimestre",      "1970 01 01",
#else
        "QQQQ y",  "fp", "1970 01 01", "1.\\u00BA trimestre 1970",  "1970 01 01",
        "QQQ y",   "fp", "1970 01 01", "T1 1970",             "1970 01 01",
        "QQQQQ y", "fp", "1970 01 01", "1 1970",              "1970 01 01",
        "qqqq",    "fp", "1970 01 01", "1.\\u00BA trimestre",      "1970 01 01",
#endif  // APPLE_ICU_CHANGES
        "qqq",     "fp", "1970 01 01", "1er. trim.",         "1970 01 01",
        "qqqqq",   "fp", "1970 01 01", "1T",                 "1970 01 01",
    };

    expect(EN_DATA, UPRV_LENGTHOF(EN_DATA), Locale("en", "", ""));
    expect(ES_MX_DATA, UPRV_LENGTHOF(ES_MX_DATA), Locale("es", "MX", ""));
}

/**
 * Test parsing.  Input is an array that starts with the following
 * header:
 *
 * [0]   = pattern string to parse [i+2] with
 *
 * followed by test cases, each of which is 3 array elements:
 *
 * [i]   = pattern, or nullptr to reuse prior pattern
 * [i+1] = input string
 * [i+2] = expected parse result (parsed with pattern [0])
 *
 * If expect parse failure, then [i+2] should be nullptr.
 */
void DateFormatTest::expectParse(const char** data, int32_t data_length,
                                 const Locale& loc) {
    const UDate FAIL = static_cast<UDate>(-1);
    const UnicodeString FAIL_STR("parse failure");
    int32_t i = 0;

    UErrorCode ec = U_ZERO_ERROR;
    SimpleDateFormat fmt("", loc, ec);
    SimpleDateFormat ref(data[i++], loc, ec);
    SimpleDateFormat gotfmt("G yyyy MM dd HH:mm:ss z", loc, ec);
    if (U_FAILURE(ec)) {
        dataerrln("FAIL: SimpleDateFormat constructor - %s", u_errorName(ec));
        return;
    }

    const char* currentPat = nullptr;
    while (i<data_length) {
        const char* pattern  = data[i++];
        const char* input    = data[i++];
        const char* expected = data[i++];

        ec = U_ZERO_ERROR;
        if (pattern != nullptr) {
            fmt.applyPattern(pattern);
            currentPat = pattern;
        }
        UDate got = fmt.parse(input, ec);
        UnicodeString gotstr(FAIL_STR);
        if (U_FAILURE(ec)) {
            got = FAIL;
        } else {
            gotstr.remove();
            gotfmt.format(got, gotstr);
        }

        UErrorCode ec2 = U_ZERO_ERROR;
        UDate exp = FAIL;
        UnicodeString expstr(FAIL_STR);
        if (expected != nullptr) {
            expstr = expected;
            exp = ref.parse(expstr, ec2);
            if (U_FAILURE(ec2)) {
                // This only happens if expected is in wrong format --
                // should never happen once test is debugged.
                errln("FAIL: Internal test error");
                return;
            }
        }

        if (got == exp) {
            logln(UnicodeString("Ok: ") + input + " x " +
                  currentPat + " => " + gotstr);
        } else {
            errln(UnicodeString("FAIL: ") + input + " x " +
                  currentPat + " => " + gotstr + ", expected " +
                  expstr);
        }
    }
}

/**
 * Test formatting and parsing.  Input is an array that starts
 * with the following header:
 *
 * [0]   = pattern string to parse [i+2] with
 *
 * followed by test cases, each of which is 3 array elements:
 *
 * [i]   = pattern, or null to reuse prior pattern
 * [i+1] = control string, either "fp", "pf", or "F".
 * [i+2..] = data strings
 *
 * The number of data strings depends on the control string.
 * Examples:
 * 1. "y/M/d H:mm:ss.SS", "fp", "2004 03 10 16:36:31.567", "2004/3/10 16:36:31.56", "2004 03 10 16:36:31.560",
 * 'f': Format date [i+2] (as parsed using pattern [0]) and expect string [i+3].
 * 'p': Parse string [i+3] and expect date [i+4].
 *
 * 2. "y/M/d H:mm:ss.SSS", "F", "2004 03 10 16:36:31.567", "2004/3/10 16:36:31.567"
 * 'F': Format date [i+2] and expect string [i+3],
 *      then parse string [i+3] and expect date [i+2].
 *
 * 3. "y/M/d H:mm:ss.SSSS", "pf", "2004/3/10 16:36:31.5679", "2004 03 10 16:36:31.567", "2004/3/10 16:36:31.5670",
 * 'p': Parse string [i+2] and expect date [i+3].
 * 'f': Format date [i+3] and expect string [i+4].
 */
void DateFormatTest::expect(const char** data, int32_t data_length,
                            const Locale& loc) {
    int32_t i = 0;
    UErrorCode ec = U_ZERO_ERROR;
    UnicodeString str, str2;
    SimpleDateFormat fmt("", loc, ec);
    SimpleDateFormat ref(data[i++], loc, ec);
    SimpleDateFormat univ("EE G yyyy MM dd HH:mm:ss.SSS z", loc, ec);
    if (U_FAILURE(ec)) {
        dataerrln("Fail construct SimpleDateFormat: %s", u_errorName(ec));
        return;
    }

    UnicodeString currentPat;
    while (i<data_length) {
        const char* pattern  = data[i++];
        if (pattern != nullptr) {
            fmt.applyPattern(pattern);
            currentPat = pattern;
        }

        const char* control = data[i++];

        if (uprv_strcmp(control, "fp") == 0) {
            // 'f'
            const char* datestr = data[i++];
            const char* string = data[i++];
            UDate date = ref.parse(ctou(datestr), ec);
            if (!assertSuccess("parse", ec)) return;
            assertEquals(UnicodeString("loc ") + ctou(loc.getName()) + " \"" + currentPat + "\".format(" + datestr + ")",
                         ctou(string),
                         fmt.format(date, str.remove()));
            // 'p'
            datestr = data[i++];
            date = ref.parse(ctou(datestr), ec);
            if (!assertSuccess("parse", ec)) return;
            UDate parsedate = fmt.parse(ctou(string), ec);
            if (assertSuccess(UnicodeString("\"") + currentPat + "\".parse(" + string + ")", ec)) {
                assertEquals(UnicodeString("loc ") + ctou(loc.getName()) + " \"" + currentPat + "\".parse(" + string + ")",
                             univ.format(date, str.remove()),
                             univ.format(parsedate, str2.remove()));
            }
        }

        else if (uprv_strcmp(control, "pf") == 0) {
            // 'p'
            const char* string = data[i++];
            const char* datestr = data[i++];
            UDate date = ref.parse(ctou(datestr), ec);
            if (!assertSuccess("parse", ec)) return;
            UDate parsedate = fmt.parse(ctou(string), ec);
            if (assertSuccess(UnicodeString("\"") + currentPat + "\".parse(" + string + ")", ec)) {
                assertEquals(UnicodeString("loc ") + ctou(loc.getName()) + " \"" + currentPat + "\".parse(" + string + ")",
                             univ.format(date, str.remove()),
                             univ.format(parsedate, str2.remove()));
            }
            // 'f'
            string = data[i++];
            assertEquals(UnicodeString("loc ") + ctou(loc.getName()) + " \"" + currentPat + "\".format(" + datestr + ")",
                         ctou(string),
                         fmt.format(date, str.remove()));
        }

        else if (uprv_strcmp(control, "F") == 0) {
            const char* datestr  = data[i++];
            const char* string   = data[i++];
            UDate date = ref.parse(ctou(datestr), ec);
            if (!assertSuccess("parse", ec)) return;
            assertEquals(UnicodeString("loc ") + ctou(loc.getName()) + " \"" + currentPat + "\".format(" + datestr + ")",
                         ctou(string),
                         fmt.format(date, str.remove()));

            UDate parsedate = fmt.parse(string, ec);
            if (assertSuccess(UnicodeString("\"") + currentPat + "\".parse(" + string + ")", ec)) {
                assertEquals(UnicodeString("loc ") + ctou(loc.getName()) + " \"" + currentPat + "\".parse(" + string + ")",
                             univ.format(date, str.remove()),
                             univ.format(parsedate, str2.remove()));
            }
        }

        else {
            errln(UnicodeString("FAIL: Invalid control string ") + control);
            return;
        }
    }
}

/**
 * Test formatting.  Input is an array that starts
 * with the following header:
 *
 * [0]   = pattern string to parse [i+2] with
 *
 * followed by test cases, each of which is 3 array elements:
 *
 * [i]   = pattern, or null to reuse prior pattern
 * [i+1] = data string a
 * [i+2] = data string b
 *
 * Examples:
 * Format date [i+1] and expect string [i+2].
 *
 * "y/M/d H:mm:ss.SSSS", "2004/3/10 16:36:31.5679", "2004 03 10 16:36:31.567"
 */
void DateFormatTest::expectFormat(const char** data, int32_t data_length,
                            const Locale& loc) {
    int32_t i = 0;
    UErrorCode ec = U_ZERO_ERROR;
    UnicodeString str, str2;
    SimpleDateFormat fmt("", loc, ec);
    SimpleDateFormat ref(data[i++], loc, ec);
    SimpleDateFormat univ("EE G yyyy MM dd HH:mm:ss.SSS z", loc, ec);
    if (U_FAILURE(ec)) {
        dataerrln("Fail construct SimpleDateFormat: %s", u_errorName(ec));
        return;
    }

    UnicodeString currentPat;

    while (i<data_length) {
        const char* pattern  = data[i++];
        if (pattern != nullptr) {
            fmt.applyPattern(pattern);
            currentPat = pattern;
        }

        const char* datestr = data[i++];
        const char* string = data[i++];
        UDate date = ref.parse(ctou(datestr), ec);
        if (!assertSuccess("parse", ec)) return;
        assertEquals(UnicodeString("loc ") + ctou(loc.getName()) + " \"" + currentPat + "\".format(" + datestr + ")",
                        ctou(string),
                        fmt.format(date, str.remove()));
    }
}

void DateFormatTest::TestGenericTime() {
  const Locale en("en");
  // Note: We no longer parse strings in different styles.
/*
  const char* ZDATA[] = {
        "yyyy MM dd HH:mm zzz",
        // round trip
        "y/M/d H:mm zzzz", "F", "2004 01 01 01:00 PST", "2004/1/1 1:00 Pacific Standard Time",
        "y/M/d H:mm zzz", "F", "2004 01 01 01:00 PST", "2004/1/1 1:00 PST",
        "y/M/d H:mm vvvv", "F", "2004 01 01 01:00 PST", "2004/1/1 1:00 Pacific Time",
        "y/M/d H:mm v", "F", "2004 01 01 01:00 PST", "2004/1/1 1:00 PT",
        // non-generic timezone string influences dst offset even if wrong for date/time
        "y/M/d H:mm zzz", "pf", "2004/1/1 1:00 PDT", "2004 01 01 01:00 PDT", "2004/1/1 0:00 PST",
        "y/M/d H:mm vvvv", "pf", "2004/1/1 1:00 PDT", "2004 01 01 01:00 PDT", "2004/1/1 0:00 Pacific Time",
        "y/M/d H:mm zzz", "pf", "2004/7/1 1:00 PST", "2004 07 01 02:00 PDT", "2004/7/1 2:00 PDT",
        "y/M/d H:mm vvvv", "pf", "2004/7/1 1:00 PST", "2004 07 01 02:00 PDT", "2004/7/1 2:00 Pacific Time",
        // generic timezone generates dst offset appropriate for local time
        "y/M/d H:mm zzz", "pf", "2004/1/1 1:00 PT", "2004 01 01 01:00 PST", "2004/1/1 1:00 PST",
        "y/M/d H:mm vvvv", "pf", "2004/1/1 1:00 PT", "2004 01 01 01:00 PST", "2004/1/1 1:00 Pacific Time",
        "y/M/d H:mm zzz", "pf", "2004/7/1 1:00 PT", "2004 07 01 01:00 PDT", "2004/7/1 1:00 PDT",
        "y/M/d H:mm vvvv", "pf", "2004/7/1 1:00 PT", "2004 07 01 01:00 PDT", "2004/7/1 1:00 Pacific Time",
        // daylight savings time transition edge cases.
        // time to parse does not really exist, PT interpreted as earlier time
        "y/M/d H:mm zzz", "pf", "2005/4/3 2:30 PT", "2005 04 03 03:30 PDT", "2005/4/3 3:30 PDT",
        "y/M/d H:mm zzz", "pf", "2005/4/3 2:30 PST", "2005 04 03 03:30 PDT", "2005/4/3 3:30 PDT",
        "y/M/d H:mm zzz", "pf", "2005/4/3 2:30 PDT", "2005 04 03 01:30 PST", "2005/4/3 1:30 PST",
        "y/M/d H:mm v", "pf", "2005/4/3 2:30 PT", "2005 04 03 03:30 PDT", "2005/4/3 3:30 PT",
        "y/M/d H:mm v", "pf", "2005/4/3 2:30 PST", "2005 04 03 03:30 PDT", "2005/4/3 3:30 PT",
        "y/M/d H:mm v", "pf", "2005/4/3 2:30 PDT", "2005 04 03 01:30 PST", "2005/4/3 1:30 PT",
        "y/M/d H:mm", "pf", "2005/4/3 2:30", "2005 04 03 03:30 PDT", "2005/4/3 3:30",
        // time to parse is ambiguous, PT interpreted as later time
        "y/M/d H:mm zzz", "pf", "2005/10/30 1:30 PT", "2005 10 30 01:30 PST", "2005/10/30 1:30 PST",
        "y/M/d H:mm v", "pf", "2005/10/30 1:30 PT", "2005 10 30  01:30 PST", "2005/10/30 1:30 PT",
        "y/M/d H:mm", "pf", "2005/10/30 1:30 PT", "2005 10 30 01:30 PST", "2005/10/30 1:30",

        "y/M/d H:mm zzz", "pf", "2004/10/31 1:30 PT", "2004 10 31 01:30 PST", "2004/10/31 1:30 PST",
        "y/M/d H:mm zzz", "pf", "2004/10/31 1:30 PST", "2004 10 31 01:30 PST", "2004/10/31 1:30 PST",
        "y/M/d H:mm zzz", "pf", "2004/10/31 1:30 PDT", "2004 10 31 01:30 PDT", "2004/10/31 1:30 PDT",
        "y/M/d H:mm v", "pf", "2004/10/31 1:30 PT", "2004 10 31 01:30 PST", "2004/10/31 1:30 PT",
        "y/M/d H:mm v", "pf", "2004/10/31 1:30 PST", "2004 10 31 01:30 PST", "2004/10/31 1:30 PT",
        "y/M/d H:mm v", "pf", "2004/10/31 1:30 PDT", "2004 10 31 01:30 PDT", "2004/10/31 1:30 PT",
        "y/M/d H:mm", "pf", "2004/10/31 1:30", "2004 10 31 01:30 PST", "2004/10/31 1:30",
  };
*/
  const char* ZDATA[] = {
        "yyyy MM dd HH:mm zzz",
        // round trip
        "y/M/d H:mm zzzz", "F", "2004 01 01 01:00 PST", "2004/1/1 1:00 Pacific Standard Time",
        "y/M/d H:mm zzz", "F", "2004 01 01 01:00 PST", "2004/1/1 1:00 PST",
        "y/M/d H:mm vvvv", "F", "2004 01 01 01:00 PST", "2004/1/1 1:00 Pacific Time",
        "y/M/d H:mm v", "F", "2004 01 01 01:00 PST", "2004/1/1 1:00 PT",
        // non-generic timezone string influences dst offset even if wrong for date/time
        "y/M/d H:mm zzz", "pf", "2004/1/1 1:00 PDT", "2004 01 01 01:00 PDT", "2004/1/1 0:00 PST",
        "y/M/d H:mm zzz", "pf", "2004/7/1 1:00 PST", "2004 07 01 02:00 PDT", "2004/7/1 2:00 PDT",
        // generic timezone generates dst offset appropriate for local time
        "y/M/d H:mm zzz", "pf", "2004/1/1 1:00 PST", "2004 01 01 01:00 PST", "2004/1/1 1:00 PST",
        "y/M/d H:mm vvvv", "pf", "2004/1/1 1:00 Pacific Time", "2004 01 01 01:00 PST", "2004/1/1 1:00 Pacific Time",
        "y/M/d H:mm zzz", "pf", "2004/7/1 1:00 PDT", "2004 07 01 01:00 PDT", "2004/7/1 1:00 PDT",
        "y/M/d H:mm vvvv", "pf", "2004/7/1 1:00 Pacific Time", "2004 07 01 01:00 PDT", "2004/7/1 1:00 Pacific Time",
        // daylight savings time transition edge cases.
        // time to parse does not really exist, PT interpreted as earlier time
        "y/M/d H:mm zzz", "pf", "2005/4/3 2:30 PST", "2005 04 03 03:30 PDT", "2005/4/3 3:30 PDT",
        "y/M/d H:mm zzz", "pf", "2005/4/3 2:30 PDT", "2005 04 03 01:30 PST", "2005/4/3 1:30 PST",
        "y/M/d H:mm v", "pf", "2005/4/3 2:30 PT", "2005 04 03 03:30 PDT", "2005/4/3 3:30 PT",
        "y/M/d H:mm", "pf", "2005/4/3 2:30", "2005 04 03 03:30 PDT", "2005/4/3 3:30",
        // time to parse is ambiguous, PT interpreted as later time
        "y/M/d H:mm v", "pf", "2005/10/30 1:30 PT", "2005 10 30  01:30 PST", "2005/10/30 1:30 PT",
        "y/M/d H:mm", "pf", "2005/10/30 1:30 PT", "2005 10 30 01:30 PST", "2005/10/30 1:30",

        "y/M/d H:mm zzz", "pf", "2004/10/31 1:30 PST", "2004 10 31 01:30 PST", "2004/10/31 1:30 PST",
        "y/M/d H:mm zzz", "pf", "2004/10/31 1:30 PDT", "2004 10 31 01:30 PDT", "2004/10/31 1:30 PDT",
        "y/M/d H:mm v", "pf", "2004/10/31 1:30 PT", "2004 10 31 01:30 PST", "2004/10/31 1:30 PT",
        "y/M/d H:mm", "pf", "2004/10/31 1:30", "2004 10 31 01:30 PST", "2004/10/31 1:30",
  };

  const int32_t ZDATA_length = UPRV_LENGTHOF(ZDATA);
  expect(ZDATA, ZDATA_length, en);

  UErrorCode status = U_ZERO_ERROR;

  logln("cross format/parse tests");    // Note: We no longer support cross format/parse
  UnicodeString basepat("yy/MM/dd H:mm ");
  SimpleDateFormat formats[] = {
    SimpleDateFormat(basepat + "vvv", en, status),
    SimpleDateFormat(basepat + "vvvv", en, status),
    SimpleDateFormat(basepat + "zzz", en, status),
    SimpleDateFormat(basepat + "zzzz", en, status)
  };
  if (U_FAILURE(status)) {
    dataerrln("Fail construct SimpleDateFormat: %s", u_errorName(status));
    return;
  }
  const int32_t formats_length = UPRV_LENGTHOF(formats);

  UnicodeString test;
  SimpleDateFormat univ("yyyy MM dd HH:mm zzz", en, status);
  ASSERT_OK(status);
  const UnicodeString times[] = {
    "2004 01 02 03:04 PST",
    "2004 07 08 09:10 PDT"
  };
  int32_t times_length = UPRV_LENGTHOF(times);
  for (int i = 0; i < times_length; ++i) {
    UDate d = univ.parse(times[i], status);
    logln(UnicodeString("\ntime: ") + d);
    for (int j = 0; j < formats_length; ++j) {
      test.remove();
      formats[j].format(d, test);
      logln("\ntest: '" + test + "'");
      for (int k = 0; k < formats_length; ++k) {
        UDate t = formats[k].parse(test, status);
        if (U_SUCCESS(status)) {
          if (d != t) {
            errln(UnicodeString("FAIL: format ") + k +
                  " incorrectly parsed output of format " + j +
                  " (" + test + "), returned " +
                  dateToString(t) + " instead of " + dateToString(d));
          } else {
            logln(UnicodeString("OK: format ") + k + " parsed ok");
          }
        } else if (status == U_PARSE_ERROR) {
          errln(UnicodeString("FAIL: format ") + k +
                " could not parse output of format " + j +
                " (" + test + ")");
        }
      }
    }
  }
}

void DateFormatTest::TestGenericTimeZoneOrder() {
  // generic times should parse the same no matter what the placement of the time zone string

  // Note: We no longer support cross style format/parse

  //const char* XDATA[] = {
  //  "yyyy MM dd HH:mm zzz",
  //  // standard time, explicit daylight/standard
  //  "y/M/d H:mm zzz", "pf", "2004/1/1 1:00 PT", "2004 01 01 01:00 PST", "2004/1/1 1:00 PST",
  //  "y/M/d zzz H:mm", "pf", "2004/1/1 PT 1:00", "2004 01 01 01:00 PST", "2004/1/1 PST 1:00",
  //  "zzz y/M/d H:mm", "pf", "PT 2004/1/1 1:00", "2004 01 01 01:00 PST", "PST 2004/1/1 1:00",

  //  // standard time, generic
  //  "y/M/d H:mm vvvv", "pf", "2004/1/1 1:00 PT", "2004 01 01 01:00 PST", "2004/1/1 1:00 Pacific Time",
  //  "y/M/d vvvv H:mm", "pf", "2004/1/1 PT 1:00", "2004 01 01 01:00 PST", "2004/1/1 Pacific Time 1:00",
  //  "vvvv y/M/d H:mm", "pf", "PT 2004/1/1 1:00", "2004 01 01 01:00 PST", "Pacific Time 2004/1/1 1:00",

  //  // dahylight time, explicit daylight/standard
  //  "y/M/d H:mm zzz", "pf", "2004/7/1 1:00 PT", "2004 07 01 01:00 PDT", "2004/7/1 1:00 PDT",
  //  "y/M/d zzz H:mm", "pf", "2004/7/1 PT 1:00", "2004 07 01 01:00 PDT", "2004/7/1 PDT 1:00",
  //  "zzz y/M/d H:mm", "pf", "PT 2004/7/1 1:00", "2004 07 01 01:00 PDT", "PDT 2004/7/1 1:00",

  //  // daylight time, generic
  //  "y/M/d H:mm vvvv", "pf", "2004/7/1 1:00 PT", "2004 07 01 01:00 PDT", "2004/7/1 1:00 Pacific Time",
  //  "y/M/d vvvv H:mm", "pf", "2004/7/1 PT 1:00", "2004 07 01 01:00 PDT", "2004/7/1 Pacific Time 1:00",
  //  "vvvv y/M/d H:mm", "pf", "PT 2004/7/1 1:00", "2004 07 01 01:00 PDT", "Pacific Time 2004/7/1 1:00",
  //};
  const char* XDATA[] = {
    "yyyy MM dd HH:mm zzz",
    // standard time, explicit daylight/standard
    "y/M/d H:mm zzz", "pf", "2004/1/1 1:00 PST", "2004 01 01 01:00 PST", "2004/1/1 1:00 PST",
    "y/M/d zzz H:mm", "pf", "2004/1/1 PST 1:00", "2004 01 01 01:00 PST", "2004/1/1 PST 1:00",
    "zzz y/M/d H:mm", "pf", "PST 2004/1/1 1:00", "2004 01 01 01:00 PST", "PST 2004/1/1 1:00",

    // standard time, generic
    "y/M/d H:mm vvvv", "pf", "2004/1/1 1:00 Pacific Time", "2004 01 01 01:00 PST", "2004/1/1 1:00 Pacific Time",
    "y/M/d vvvv H:mm", "pf", "2004/1/1 Pacific Time 1:00", "2004 01 01 01:00 PST", "2004/1/1 Pacific Time 1:00",
    "vvvv y/M/d H:mm", "pf", "Pacific Time 2004/1/1 1:00", "2004 01 01 01:00 PST", "Pacific Time 2004/1/1 1:00",

    // dahylight time, explicit daylight/standard
    "y/M/d H:mm zzz", "pf", "2004/7/1 1:00 PDT", "2004 07 01 01:00 PDT", "2004/7/1 1:00 PDT",
    "y/M/d zzz H:mm", "pf", "2004/7/1 PDT 1:00", "2004 07 01 01:00 PDT", "2004/7/1 PDT 1:00",
    "zzz y/M/d H:mm", "pf", "PDT 2004/7/1 1:00", "2004 07 01 01:00 PDT", "PDT 2004/7/1 1:00",

    // daylight time, generic
    "y/M/d H:mm v", "pf", "2004/7/1 1:00 PT", "2004 07 01 01:00 PDT", "2004/7/1 1:00 PT",
    "y/M/d v H:mm", "pf", "2004/7/1 PT 1:00", "2004 07 01 01:00 PDT", "2004/7/1 PT 1:00",
    "v y/M/d H:mm", "pf", "PT 2004/7/1 1:00", "2004 07 01 01:00 PDT", "PT 2004/7/1 1:00",
  };
  const int32_t XDATA_length = UPRV_LENGTHOF(XDATA);
  Locale en("en");
  expect(XDATA, XDATA_length, en);
}

void DateFormatTest::TestZTimeZoneParsing() {
    UErrorCode status = U_ZERO_ERROR;
    const Locale en("en");
    UnicodeString test;
    //SimpleDateFormat univ("yyyy-MM-dd'T'HH:mm Z", en, status);
    SimpleDateFormat univ("HH:mm Z", en, status);
    if (failure(status, "construct SimpleDateFormat", true)) return;
    const TimeZone *t = TimeZone::getGMT();
    univ.setTimeZone(*t);

    univ.setLenient(false);
    ParsePosition pp(0);
    struct {
        UnicodeString input;
        UnicodeString expected_result;
    } tests[] = {
        { "11:00 -0200", "13:00 +0000" },
        { "11:00 +0200", "09:00 +0000" },
        { "11:00 +0400", "07:00 +0000" },
        { "11:00 +0530", "05:30 +0000" }
    };

    UnicodeString result;
    int32_t tests_length = UPRV_LENGTHOF(tests);
    for (int i = 0; i < tests_length; ++i) {
        pp.setIndex(0);
        UDate d = univ.parse(tests[i].input, pp);
        if(pp.getIndex() != tests[i].input.length()){
            errln("Test %i: setZoneString() did not succeed. Consumed: %i instead of %i",
                  i, pp.getIndex(), tests[i].input.length());
            return;
        }
        result.remove();
        univ.format(d, result);
        if(result != tests[i].expected_result) {
            errln("Expected " + tests[i].expected_result
                  + " got " + result);
            return;
        }
        logln("SUCCESS: Parsed " + tests[i].input
              + " got " + result
              + " expected " + tests[i].expected_result);
    }
}

void DateFormatTest::TestHost()
{
#if U_PLATFORM_USES_ONLY_WIN32_API
    Win32DateTimeTest::testLocales(this);
#endif
}

// Relative Date Tests

void DateFormatTest::TestRelative(int daysdelta,
                                  const Locale& loc,
                                  const char *expectChars) {
    char banner[25];
    snprintf(banner, sizeof(banner), "%d", daysdelta);
    UnicodeString bannerStr(banner, "");

    UErrorCode status = U_ZERO_ERROR;

    FieldPosition pos(FieldPosition::DONT_CARE);
    UnicodeString test;
    Locale en("en");
    DateFormat *fullrelative = DateFormat::createDateInstance(DateFormat::kFullRelative, loc);

    if (fullrelative == nullptr) {
        dataerrln("DateFormat::createDateInstance(DateFormat::kFullRelative, %s) returned nullptr", loc.getName());
        return;
    }

    DateFormat *full         = DateFormat::createDateInstance(DateFormat::kFull        , loc);

    if (full == nullptr) {
        errln("DateFormat::createDateInstance(DateFormat::kFull, %s) returned nullptr", loc.getName());
        return;
    }

    DateFormat *en_full =         DateFormat::createDateInstance(DateFormat::kFull,         en);

    if (en_full == nullptr) {
        errln("DateFormat::createDateInstance(DateFormat::kFull, en) returned nullptr");
        return;
    }

    DateFormat *en_fulltime =         DateFormat::createDateTimeInstance(DateFormat::kFull,DateFormat::kFull,en);

    if (en_fulltime == nullptr) {
        errln("DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull, en) returned nullptr");
        return;
    }

    UnicodeString result;
    UnicodeString normalResult;
    UnicodeString expect;
    UnicodeString parseResult;

    Calendar *c = Calendar::createInstance(status);

    // Today = Today
    c->setTime(Calendar::getNow(), status);
    if(daysdelta != 0) {
        c->add(Calendar::DATE,daysdelta,status);
    }
    ASSERT_OK(status);

    // calculate the expected string
    if(expectChars != nullptr) {
        expect = expectChars;
    } else {
        full->format(*c, expect, pos); // expected = normal full
    }

    fullrelative   ->format(*c, result, pos);
    en_full        ->format(*c, normalResult, pos);

    if(result != expect) {
        errln("FAIL: Relative Format ["+bannerStr+"] of "+normalResult+" failed, expected "+expect+" but got " + result);
    } else {
        logln("PASS: Relative Format ["+bannerStr+"] of "+normalResult+" got " + result);
    }


    //verify
    UDate d = fullrelative->parse(result, status);
    ASSERT_OK(status);

    UnicodeString parseFormat; // parse rel->format full
    en_full->format(d, parseFormat, status);

    UnicodeString origFormat;
    en_full->format(*c, origFormat, pos);

    if(parseFormat!=origFormat) {
        errln("FAIL: Relative Parse ["+bannerStr+"] of "+result+" failed, expected "+parseFormat+" but got "+origFormat);
    } else {
        logln("PASS: Relative Parse ["+bannerStr+"] of "+result+" passed, got "+parseFormat);
    }

    delete full;
    delete fullrelative;
    delete en_fulltime;
    delete en_full;
    delete c;
}


void DateFormatTest::TestRelative()
{
    Locale en("en");
    TestRelative( 0, en, "today");
    TestRelative(-1, en, "yesterday");
    TestRelative( 1, en, "tomorrow");
    TestRelative( 2, en, nullptr);
    TestRelative( -2, en, nullptr);
    TestRelative( 3, en, nullptr);
    TestRelative( -3, en, nullptr);
    TestRelative( 300, en, nullptr);
    TestRelative( -300, en, nullptr);
#if APPLE_ICU_CHANGES
    // rdar://138228656
    Locale ur("ur"); // Urdu
    // "yesterday" and "day before yesterday" should be unmarked
    TestRelative(-1, ur, "کل"); // yesterday
    TestRelative(-2, ur, "پرسوں"); // day before yesterday
    // "tomorrow" and "day after tomorrow" should use consistent term for "future"
    TestRelative(+1, ur, "آئندہ کل"); // tomorrow
    TestRelative(+2, ur, "آئندہ پرسوں"); // day after tomorrow
#endif // APPLE_ICU_CHANGES
}

void DateFormatTest::TestRelativeClone()
{
    /*
    Verify that a cloned formatter gives the same results
    and is useable after the original has been deleted.
    */
    UErrorCode status = U_ZERO_ERROR;
    Locale loc("en");
    UDate now = Calendar::getNow();
    DateFormat *full = DateFormat::createDateInstance(DateFormat::kFullRelative, loc);
    if (full == nullptr) {
        dataerrln("FAIL: Can't create Relative date instance");
        return;
    }
    UnicodeString result1;
    full->format(now, result1, status);
    Format *fullClone = full->clone();
    delete full;
    full = nullptr;

    UnicodeString result2;
    fullClone->format(now, result2, status);
    ASSERT_OK(status);
    if (result1 != result2) {
        errln("FAIL: Clone returned different result from non-clone.");
    }
    delete fullClone;
}

void DateFormatTest::TestHostClone()
{
    /*
    Verify that a cloned formatter gives the same results
    and is useable after the original has been deleted.
    */
    // This is mainly important on Windows.
    UErrorCode status = U_ZERO_ERROR;
    Locale loc("en_US@compat=host");
    UDate now = Calendar::getNow();
    DateFormat *full = DateFormat::createDateInstance(DateFormat::kFull, loc);
    if (full == nullptr) {
        dataerrln("FAIL: Can't create host date instance");
        return;
    }
    UnicodeString result1;
    full->format(now, result1, status);
    Format *fullClone = full->clone();
    delete full;
    full = nullptr;

    UnicodeString result2;
    fullClone->format(now, result2, status);
    ASSERT_OK(status);
    if (result1 != result2) {
        errln("FAIL: Clone returned different result from non-clone.");
    }
    delete fullClone;
}

void DateFormatTest::TestHebrewClone()
{
    /*
    Verify that a cloned formatter gives the same results
    and is useable after the original has been deleted.
    */
    UErrorCode status = U_ZERO_ERROR;
    Locale loc("he@calendar=hebrew");
    UDate now = Calendar::getNow();
    LocalPointer<DateFormat> fmt(
            DateFormat::createDateInstance(DateFormat::kLong, loc));
    if (fmt.isNull()) {
        dataerrln("FAIL: Can't create Hebrew date instance");
        return;
    }
    UnicodeString result1;
    fmt->format(now, result1, status);
    LocalPointer<Format> fmtClone(fmt->clone());

    // free fmt to be sure that fmtClone is independent of fmt.
    fmt.adoptInstead(nullptr);

    UnicodeString result2;
    fmtClone->format(now, result2, status);
    ASSERT_OK(status);
    if (result1 != result2) {
        errln("FAIL: Clone returned different result from non-clone.");
    }
}

static UBool getActualAndValidLocales(
        const Format &fmt, Locale &valid, Locale &actual) {
    const SimpleDateFormat* dat = dynamic_cast<const SimpleDateFormat*>(&fmt);
    if (dat == nullptr) {
        return false;
    }
    const DateFormatSymbols *sym = dat->getDateFormatSymbols();
    if (sym == nullptr) {
        return false;
    }
    UErrorCode status = U_ZERO_ERROR;
    valid = sym->getLocale(ULOC_VALID_LOCALE, status);
    actual = sym->getLocale(ULOC_ACTUAL_LOCALE, status);
    return U_SUCCESS(status);
}

void DateFormatTest::TestDateFormatSymbolsClone()
{
    /*
    Verify that a cloned formatter gives the same results
    and is useable after the original has been deleted.
    */
    Locale loc("de_CH_LUCERNE");
    LocalPointer<DateFormat> fmt(
            DateFormat::createDateInstance(DateFormat::kDefault, loc));
    if (fmt.isNull()) {
        dataerrln("FAIL: DateFormat::createDateInstance failed for %s", loc.getName());
        return;
    }
    Locale valid1;
    Locale actual1;
    if (!getActualAndValidLocales(*fmt, valid1, actual1)) {
        dataerrln("FAIL: Could not fetch valid + actual locales");
        return;
    }
    LocalPointer<Format> fmtClone(fmt->clone());

    // Free fmt to be sure that fmtClone is really independent of fmt.
    fmt.adoptInstead(nullptr);
    Locale valid2;
    Locale actual2;
    if (!getActualAndValidLocales(*fmtClone, valid2, actual2)) {
        errln("FAIL: Could not fetch valid + actual locales");
        return;
    }
    if (valid1 != valid2 || actual1 != actual2) {
        errln("Date format symbol locales of clone don't match original");
    }
}

void DateFormatTest::TestTimeZoneDisplayName()
{
    // This test data was ported from ICU4J.  Don't know why the 6th column in there because it's not being
    // used currently.
    const char *fallbackTests[][6]  = {
        { "en", "America/Los_Angeles", "2004-01-15T00:00:00Z", "Z", "-0800", "-8:00" },
        { "en", "America/Los_Angeles", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-08:00", "-8:00" },
        { "en", "America/Los_Angeles", "2004-01-15T00:00:00Z", "ZZZZZ", "-08:00", "-8:00" },
        { "en", "America/Los_Angeles", "2004-01-15T00:00:00Z", "z", "PST", "America/Los_Angeles" },
        { "en", "America/Los_Angeles", "2004-01-15T00:00:00Z", "zzzz", "Pacific Standard Time", "America/Los_Angeles" },
        { "en", "America/Los_Angeles", "2004-07-15T00:00:00Z", "Z", "-0700", "-7:00" },
        { "en", "America/Los_Angeles", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-07:00", "-7:00" },
        { "en", "America/Los_Angeles", "2004-07-15T00:00:00Z", "z", "PDT", "America/Los_Angeles" },
        { "en", "America/Los_Angeles", "2004-07-15T00:00:00Z", "zzzz", "Pacific Daylight Time", "America/Los_Angeles" },
        { "en", "America/Los_Angeles", "2004-07-15T00:00:00Z", "v", "PT", "America/Los_Angeles" },
        { "en", "America/Los_Angeles", "2004-07-15T00:00:00Z", "vvvv", "Pacific Time", "America/Los_Angeles" },
        { "en", "America/Los_Angeles", "2004-07-15T00:00:00Z", "VVVV", "Los Angeles Time", "America/Los_Angeles" },
        { "en_GB", "America/Los_Angeles", "2004-01-15T12:00:00Z", "z", "GMT-8", "America/Los_Angeles" },
        { "en", "America/Phoenix", "2004-01-15T00:00:00Z", "Z", "-0700", "-7:00" },
        { "en", "America/Phoenix", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-07:00", "-7:00" },
        { "en", "America/Phoenix", "2004-01-15T00:00:00Z", "z", "MST", "America/Phoenix" },
        { "en", "America/Phoenix", "2004-01-15T00:00:00Z", "zzzz", "Mountain Standard Time", "America/Phoenix" },
        { "en", "America/Phoenix", "2004-07-15T00:00:00Z", "Z", "-0700", "-7:00" },
        { "en", "America/Phoenix", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-07:00", "-7:00" },
        { "en", "America/Phoenix", "2004-07-15T00:00:00Z", "z", "MST", "America/Phoenix" },
        { "en", "America/Phoenix", "2004-07-15T00:00:00Z", "zzzz", "Mountain Standard Time", "America/Phoenix" },
        { "en", "America/Phoenix", "2004-07-15T00:00:00Z", "v", "MST", "America/Phoenix" },
        { "en", "America/Phoenix", "2004-07-15T00:00:00Z", "vvvv", "Mountain Standard Time", "America/Phoenix" },
        { "en", "America/Phoenix", "2004-07-15T00:00:00Z", "VVVV", "Phoenix Time", "America/Phoenix" },

        { "en", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "en", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "en", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "en", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "Argentina Standard Time", "-3:00" },
        { "en", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "en", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "en", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "en", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "Argentina Standard Time", "-3:00" },
        { "en", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "Buenos Aires Time", "America/Buenos_Aires" },
        { "en", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "Argentina Standard Time", "America/Buenos_Aires" },
        { "en", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "VVVV", "Buenos Aires Time", "America/Buenos_Aires" },

        { "en", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "en", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "en", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "en", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "Argentina Standard Time", "-3:00" },
        { "en", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "en", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "en", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "en", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "Argentina Standard Time", "-3:00" },
        { "en", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "Buenos Aires Time", "America/Buenos_Aires" },
        { "en", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "Argentina Standard Time", "America/Buenos_Aires" },
        { "en", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "VVVV", "Buenos Aires Time", "America/Buenos_Aires" },

        { "en", "America/Havana", "2004-01-15T00:00:00Z", "Z", "-0500", "-5:00" },
        { "en", "America/Havana", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-05:00", "-5:00" },
        { "en", "America/Havana", "2004-01-15T00:00:00Z", "z", "GMT-5", "-5:00" },
        { "en", "America/Havana", "2004-01-15T00:00:00Z", "zzzz", "Cuba Standard Time", "-5:00" },
        { "en", "America/Havana", "2004-07-15T00:00:00Z", "Z", "-0400", "-4:00" },
        { "en", "America/Havana", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-04:00", "-4:00" },
        { "en", "America/Havana", "2004-07-15T00:00:00Z", "z", "GMT-4", "-4:00" },
        { "en", "America/Havana", "2004-07-15T00:00:00Z", "zzzz", "Cuba Daylight Time", "-4:00" },
        { "en", "America/Havana", "2004-07-15T00:00:00Z", "v", "Cuba Time", "America/Havana" },
        { "en", "America/Havana", "2004-07-15T00:00:00Z", "vvvv", "Cuba Time", "America/Havana" },
        { "en", "America/Havana", "2004-07-15T00:00:00Z", "VVVV", "Cuba Time", "America/Havana" },

        { "en", "Australia/ACT", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "en", "Australia/ACT", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "en", "Australia/ACT", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "en", "Australia/ACT", "2004-01-15T00:00:00Z", "zzzz", "Australian Eastern Daylight Time", "+11:00" },
        { "en", "Australia/ACT", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "en", "Australia/ACT", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "en", "Australia/ACT", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "en", "Australia/ACT", "2004-07-15T00:00:00Z", "zzzz", "Australian Eastern Standard Time", "+10:00" },
        { "en", "Australia/ACT", "2004-07-15T00:00:00Z", "v", "Sydney Time", "Australia/Sydney" },
        { "en", "Australia/ACT", "2004-07-15T00:00:00Z", "vvvv", "Eastern Australia Time", "Australia/Sydney" },
        { "en", "Australia/ACT", "2004-07-15T00:00:00Z", "VVVV", "Sydney Time", "Australia/Sydney" },

        { "en", "Australia/Sydney", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "en", "Australia/Sydney", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "en", "Australia/Sydney", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "en", "Australia/Sydney", "2004-01-15T00:00:00Z", "zzzz", "Australian Eastern Daylight Time", "+11:00" },
        { "en", "Australia/Sydney", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "en", "Australia/Sydney", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "en", "Australia/Sydney", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "en", "Australia/Sydney", "2004-07-15T00:00:00Z", "zzzz", "Australian Eastern Standard Time", "+10:00" },
        { "en", "Australia/Sydney", "2004-07-15T00:00:00Z", "v", "Sydney Time", "Australia/Sydney" },
        { "en", "Australia/Sydney", "2004-07-15T00:00:00Z", "vvvv", "Eastern Australia Time", "Australia/Sydney" },
        { "en", "Australia/Sydney", "2004-07-15T00:00:00Z", "VVVV", "Sydney Time", "Australia/Sydney" },

        { "en", "Europe/London", "2004-01-15T00:00:00Z", "Z", "+0000", "+0:00" },
        { "en", "Europe/London", "2004-01-15T00:00:00Z", "ZZZZ", "GMT", "+0:00" },
        { "en", "Europe/London", "2004-01-15T00:00:00Z", "z", "GMT", "+0:00" },
        { "en", "Europe/London", "2004-01-15T00:00:00Z", "zzzz", "Greenwich Mean Time", "+0:00" },
        { "en", "Europe/London", "2004-07-15T00:00:00Z", "Z", "+0100", "+1:00" },
        { "en", "Europe/London", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+01:00", "+1:00" },
        { "en", "Europe/London", "2004-07-15T00:00:00Z", "z", "GMT+1", "Europe/London" },
        { "en", "Europe/London", "2004-07-15T00:00:00Z", "zzzz", "British Summer Time", "Europe/London" },
    // icu en.txt has exemplar city for this time zone
        { "en", "Europe/London", "2004-07-15T00:00:00Z", "v", "United Kingdom Time", "Europe/London" },
        { "en", "Europe/London", "2004-07-15T00:00:00Z", "vvvv", "United Kingdom Time", "Europe/London" },
        { "en", "Europe/London", "2004-07-15T00:00:00Z", "VVVV", "United Kingdom Time", "Europe/London" },

        { "en", "Etc/GMT+3", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "en", "Etc/GMT+3", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "en", "Etc/GMT+3", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "en", "Etc/GMT+3", "2004-01-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "en", "Etc/GMT+3", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "en", "Etc/GMT+3", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "en", "Etc/GMT+3", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "en", "Etc/GMT+3", "2004-07-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "en", "Etc/GMT+3", "2004-07-15T00:00:00Z", "v", "GMT-3", "-3:00" },
        { "en", "Etc/GMT+3", "2004-07-15T00:00:00Z", "vvvv", "GMT-03:00", "-3:00" },

        // JB#5150
        { "en", "Asia/Calcutta", "2004-01-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "en", "Asia/Calcutta", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "en", "Asia/Calcutta", "2004-01-15T00:00:00Z", "z", "GMT+5:30", "+5:30" },
        { "en", "Asia/Calcutta", "2004-01-15T00:00:00Z", "zzzz", "India Standard Time", "+5:30" },
        { "en", "Asia/Calcutta", "2004-07-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "en", "Asia/Calcutta", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "en", "Asia/Calcutta", "2004-07-15T00:00:00Z", "z", "GMT+5:30", "+05:30" },
        { "en", "Asia/Calcutta", "2004-07-15T00:00:00Z", "zzzz", "India Standard Time", "+5:30" },
        { "en", "Asia/Calcutta", "2004-07-15T00:00:00Z", "v", "India Time", "Asia/Calcutta" },
        { "en", "Asia/Calcutta", "2004-07-15T00:00:00Z", "vvvv", "India Standard Time", "Asia/Calcutta" },

        // Proper CLDR primary zone support #9733
        { "en", "America/Santiago", "2013-01-01T00:00:00Z", "VVVV", "Chile Time", "America/Santiago" },
        { "en", "Pacific/Easter", "2013-01-01T00:00:00Z", "VVVV", "Easter Time", "Pacific/Easter" },

        // ==========

        { "de", "America/Los_Angeles", "2004-01-15T00:00:00Z", "Z", "-0800", "-8:00" },
        { "de", "America/Los_Angeles", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-08:00", "-8:00" },
        { "de", "America/Los_Angeles", "2004-01-15T00:00:00Z", "z", "GMT-8", "-8:00" },
        { "de", "America/Los_Angeles", "2004-01-15T00:00:00Z", "zzzz", "Nordamerikanische Westk\\u00fcsten-Normalzeit", "-8:00" },
        { "de", "America/Los_Angeles", "2004-07-15T00:00:00Z", "Z", "-0700", "-7:00" },
        { "de", "America/Los_Angeles", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-07:00", "-7:00" },
        { "de", "America/Los_Angeles", "2004-07-15T00:00:00Z", "z", "GMT-7", "-7:00" },
        { "de", "America/Los_Angeles", "2004-07-15T00:00:00Z", "zzzz", "Nordamerikanische Westk\\u00fcsten-Sommerzeit", "-7:00" },
        { "de", "America/Los_Angeles", "2004-07-15T00:00:00Z", "v", "Los Angeles (Ortszeit)", "America/Los_Angeles" },
        { "de", "America/Los_Angeles", "2004-07-15T00:00:00Z", "vvvv", "Nordamerikanische Westk\\u00fcstenzeit", "America/Los_Angeles" },

        { "de", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "de", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "de", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "de", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "Argentinische Normalzeit", "-3:00" },
        { "de", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "de", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "de", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "de", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "Argentinische Normalzeit", "-3:00" },
        { "de", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "Buenos Aires (Ortszeit)", "America/Buenos_Aires" },
        { "de", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "Argentinische Normalzeit", "America/Buenos_Aires" },

        { "de", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "de", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "de", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "de", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "Argentinische Normalzeit", "-3:00" },
        { "de", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "de", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "de", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "de", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "Argentinische Normalzeit", "-3:00" },
        { "de", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "Buenos Aires (Ortszeit)", "America/Buenos_Aires" },
        { "de", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "Argentinische Normalzeit", "America/Buenos_Aires" },

        { "de", "America/Havana", "2004-01-15T00:00:00Z", "Z", "-0500", "-5:00" },
        { "de", "America/Havana", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-05:00", "-5:00" },
        { "de", "America/Havana", "2004-01-15T00:00:00Z", "z", "GMT-5", "-5:00" },
        { "de", "America/Havana", "2004-01-15T00:00:00Z", "zzzz", "Kubanische Normalzeit", "-5:00" },
        { "de", "America/Havana", "2004-07-15T00:00:00Z", "Z", "-0400", "-4:00" },
        { "de", "America/Havana", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-04:00", "-4:00" },
        { "de", "America/Havana", "2004-07-15T00:00:00Z", "z", "GMT-4", "-4:00" },
        { "de", "America/Havana", "2004-07-15T00:00:00Z", "zzzz", "Kubanische Sommerzeit", "-4:00" },
        { "de", "America/Havana", "2004-07-15T00:00:00Z", "v", "Kuba (Ortszeit)", "America/Havana" },
        { "de", "America/Havana", "2004-07-15T00:00:00Z", "vvvv", "Kubanische Zeit", "America/Havana" },
        // added to test proper fallback of country name
        { "de_CH", "America/Havana", "2004-07-15T00:00:00Z", "v", "Kuba (Ortszeit)", "America/Havana" },
        { "de_CH", "America/Havana", "2004-07-15T00:00:00Z", "vvvv", "Kubanische Zeit", "America/Havana" },

        { "de", "Australia/ACT", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "de", "Australia/ACT", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "de", "Australia/ACT", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "de", "Australia/ACT", "2004-01-15T00:00:00Z", "zzzz", "Ostaustralische Sommerzeit", "+11:00" },
        { "de", "Australia/ACT", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "de", "Australia/ACT", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "de", "Australia/ACT", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "de", "Australia/ACT", "2004-07-15T00:00:00Z", "zzzz", "Ostaustralische Normalzeit", "+10:00" },
        { "de", "Australia/ACT", "2004-07-15T00:00:00Z", "v", "Sydney (Ortszeit)", "Australia/Sydney" },
        { "de", "Australia/ACT", "2004-07-15T00:00:00Z", "vvvv", "Ostaustralische Zeit", "Australia/Sydney" },

        { "de", "Australia/Sydney", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "de", "Australia/Sydney", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "de", "Australia/Sydney", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "de", "Australia/Sydney", "2004-01-15T00:00:00Z", "zzzz", "Ostaustralische Sommerzeit", "+11:00" },
        { "de", "Australia/Sydney", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "de", "Australia/Sydney", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "de", "Australia/Sydney", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "de", "Australia/Sydney", "2004-07-15T00:00:00Z", "zzzz", "Ostaustralische Normalzeit", "+10:00" },
        { "de", "Australia/Sydney", "2004-07-15T00:00:00Z", "v", "Sydney (Ortszeit)", "Australia/Sydney" },
        { "de", "Australia/Sydney", "2004-07-15T00:00:00Z", "vvvv", "Ostaustralische Zeit", "Australia/Sydney" },

        { "de", "Europe/London", "2004-01-15T00:00:00Z", "Z", "+0000", "+0:00" },
        { "de", "Europe/London", "2004-01-15T00:00:00Z", "ZZZZ", "GMT", "+0:00" },
        { "de", "Europe/London", "2004-01-15T00:00:00Z", "z", "GMT", "+0:00" },
        { "de", "Europe/London", "2004-01-15T00:00:00Z", "zzzz", "Mittlere Greenwich-Zeit", "+0:00" },
        { "de", "Europe/London", "2004-07-15T00:00:00Z", "Z", "+0100", "+1:00" },
        { "de", "Europe/London", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+01:00", "+1:00" },
        { "de", "Europe/London", "2004-07-15T00:00:00Z", "z", "GMT+1", "+1:00" },
        { "de", "Europe/London", "2004-07-15T00:00:00Z", "zzzz", "Britische Sommerzeit", "+1:00" },
        { "de", "Europe/London", "2004-07-15T00:00:00Z", "v", "Vereinigtes K\\u00f6nigreich (Ortszeit)", "Europe/London" },
        { "de", "Europe/London", "2004-07-15T00:00:00Z", "vvvv", "Vereinigtes K\\u00f6nigreich (Ortszeit)", "Europe/London" },

        { "de", "Etc/GMT+3", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "de", "Etc/GMT+3", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "de", "Etc/GMT+3", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "de", "Etc/GMT+3", "2004-01-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "de", "Etc/GMT+3", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "de", "Etc/GMT+3", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "de", "Etc/GMT+3", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "de", "Etc/GMT+3", "2004-07-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "de", "Etc/GMT+3", "2004-07-15T00:00:00Z", "v", "GMT-3", "-3:00" },
        { "de", "Etc/GMT+3", "2004-07-15T00:00:00Z", "vvvv", "GMT-03:00", "-3:00" },

        // JB#5150
        { "de", "Asia/Calcutta", "2004-01-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "de", "Asia/Calcutta", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "de", "Asia/Calcutta", "2004-01-15T00:00:00Z", "z", "GMT+5:30", "+5:30" },
        { "de", "Asia/Calcutta", "2004-01-15T00:00:00Z", "zzzz", "Indische Normalzeit", "+5:30" },
        { "de", "Asia/Calcutta", "2004-07-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "de", "Asia/Calcutta", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "de", "Asia/Calcutta", "2004-07-15T00:00:00Z", "z", "GMT+5:30", "+05:30" },
        { "de", "Asia/Calcutta", "2004-07-15T00:00:00Z", "zzzz", "Indische Normalzeit", "+5:30" },
        { "de", "Asia/Calcutta", "2004-07-15T00:00:00Z", "v", "Indien (Ortszeit)", "Asia/Calcutta" },
        { "de", "Asia/Calcutta", "2004-07-15T00:00:00Z", "vvvv", "Indische Normalzeit", "Asia/Calcutta" },

        // ==========

        { "zh", "America/Los_Angeles", "2004-01-15T00:00:00Z", "Z", "-0800", "-8:00" },
        { "zh", "America/Los_Angeles", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-08:00", "-8:00" },
        { "zh", "America/Los_Angeles", "2004-01-15T00:00:00Z", "z", "GMT-8", "America/Los_Angeles" },
        { "zh", "America/Los_Angeles", "2004-01-15T00:00:00Z", "zzzz", "\\u5317\\u7f8e\\u592a\\u5e73\\u6d0b\\u6807\\u51c6\\u65f6\\u95f4", "America/Los_Angeles" },
        { "zh", "America/Los_Angeles", "2004-07-15T00:00:00Z", "Z", "-0700", "-7:00" },
        { "zh", "America/Los_Angeles", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-07:00", "-7:00" },
        { "zh", "America/Los_Angeles", "2004-07-15T00:00:00Z", "z", "GMT-7", "America/Los_Angeles" },
        { "zh", "America/Los_Angeles", "2004-07-15T00:00:00Z", "zzzz", "\\u5317\\u7f8e\\u592a\\u5e73\\u6d0b\\u590f\\u4ee4\\u65f6\\u95f4", "America/Los_Angeles" },
    // icu zh.txt has exemplar city for this time zone
        { "zh", "America/Los_Angeles", "2004-07-15T00:00:00Z", "v", "\\u6D1B\\u6749\\u77F6\\u65F6\\u95F4", "America/Los_Angeles" },
        { "zh", "America/Los_Angeles", "2004-07-15T00:00:00Z", "vvvv", "\\u5317\\u7f8e\\u592a\\u5e73\\u6d0b\\u65f6\\u95f4", "America/Los_Angeles" },

        { "zh", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u963f\\u6839\\u5ef7\\u6807\\u51c6\\u65f6\\u95f4", "-3:00" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u963f\\u6839\\u5ef7\\u6807\\u51c6\\u65f6\\u95f4", "-3:00" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u5E03\\u5B9C\\u8BFA\\u65AF\\u827E\\u5229\\u65AF\\u65F6\\u95F4", "America/Buenos_Aires" },
        { "zh", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u963f\\u6839\\u5ef7\\u6807\\u51c6\\u65f6\\u95f4", "America/Buenos_Aires" },

        { "zh", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "zh", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "zh", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "zh", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u963f\\u6839\\u5ef7\\u6807\\u51c6\\u65f6\\u95f4", "-3:00" },
        { "zh", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "zh", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "zh", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "zh", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u963f\\u6839\\u5ef7\\u6807\\u51c6\\u65f6\\u95f4", "-3:00" },
        { "zh", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u5E03\\u5B9C\\u8BFA\\u65AF\\u827E\\u5229\\u65AF\\u65F6\\u95F4", "America/Buenos_Aires" },
        { "zh", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u963f\\u6839\\u5ef7\\u6807\\u51c6\\u65f6\\u95f4", "America/Buenos_Aires" },

        { "zh", "America/Havana", "2004-01-15T00:00:00Z", "Z", "-0500", "-5:00" },
        { "zh", "America/Havana", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-05:00", "-5:00" },
        { "zh", "America/Havana", "2004-01-15T00:00:00Z", "z", "GMT-5", "-5:00" },
        { "zh", "America/Havana", "2004-01-15T00:00:00Z", "zzzz", "\\u53e4\\u5df4\\u6807\\u51c6\\u65f6\\u95f4", "-5:00" },
        { "zh", "America/Havana", "2004-07-15T00:00:00Z", "Z", "-0400", "-4:00" },
        { "zh", "America/Havana", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-04:00", "-4:00" },
        { "zh", "America/Havana", "2004-07-15T00:00:00Z", "z", "GMT-4", "-4:00" },
        { "zh", "America/Havana", "2004-07-15T00:00:00Z", "zzzz", "\\u53e4\\u5df4\\u590f\\u4ee4\\u65f6\\u95f4", "-4:00" },
        { "zh", "America/Havana", "2004-07-15T00:00:00Z", "v", "\\u53e4\\u5df4\\u65f6\\u95f4", "America/Havana" },
        { "zh", "America/Havana", "2004-07-15T00:00:00Z", "vvvv", "\\u53e4\\u5df4\\u65f6\\u95f4", "America/Havana" },

        { "zh", "Australia/ACT", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "zh", "Australia/ACT", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "zh", "Australia/ACT", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "zh", "Australia/ACT", "2004-01-15T00:00:00Z", "zzzz", "\\u6fb3\\u5927\\u5229\\u4e9a\\u4e1c\\u90e8\\u590f\\u4ee4\\u65f6\\u95f4", "+11:00" },
        { "zh", "Australia/ACT", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "zh", "Australia/ACT", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "zh", "Australia/ACT", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "zh", "Australia/ACT", "2004-07-15T00:00:00Z", "zzzz", "\\u6fb3\\u5927\\u5229\\u4e9a\\u4e1c\\u90e8\\u6807\\u51c6\\u65f6\\u95f4", "+10:00" },
    // icu zh.txt does not have info for this time zone
        { "zh", "Australia/ACT", "2004-07-15T00:00:00Z", "v", "\\u6089\\u5C3C\\u65F6\\u95F4", "Australia/Sydney" },
        { "zh", "Australia/ACT", "2004-07-15T00:00:00Z", "vvvv", "\\u6fb3\\u5927\\u5229\\u4e9a\\u4e1c\\u90e8\\u65f6\\u95f4", "Australia/Sydney" },

        { "zh", "Australia/Sydney", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "zh", "Australia/Sydney", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "zh", "Australia/Sydney", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "zh", "Australia/Sydney", "2004-01-15T00:00:00Z", "zzzz", "\\u6fb3\\u5927\\u5229\\u4e9a\\u4e1c\\u90e8\\u590f\\u4ee4\\u65f6\\u95f4", "+11:00" },
        { "zh", "Australia/Sydney", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "zh", "Australia/Sydney", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "zh", "Australia/Sydney", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "zh", "Australia/Sydney", "2004-07-15T00:00:00Z", "zzzz", "\\u6fb3\\u5927\\u5229\\u4e9a\\u4e1c\\u90e8\\u6807\\u51c6\\u65f6\\u95f4", "+10:00" },
        { "zh", "Australia/Sydney", "2004-07-15T00:00:00Z", "v", "\\u6089\\u5C3C\\u65F6\\u95F4", "Australia/Sydney" },
        { "zh", "Australia/Sydney", "2004-07-15T00:00:00Z", "vvvv", "\\u6fb3\\u5927\\u5229\\u4e9a\\u4e1c\\u90e8\\u65f6\\u95f4", "Australia/Sydney" },

        { "zh", "Europe/London", "2004-01-15T00:00:00Z", "Z", "+0000", "+0:00" },
        { "zh", "Europe/London", "2004-01-15T00:00:00Z", "ZZZZ", "GMT", "+0:00" },
        { "zh", "Europe/London", "2004-01-15T00:00:00Z", "z", "GMT", "+0:00" },
        { "zh", "Europe/London", "2004-01-15T00:00:00Z", "ZZZZ", "GMT", "+0:00" },
        { "zh", "Europe/London", "2004-01-15T00:00:00Z", "z", "GMT", "+0:00" },
        { "zh", "Europe/London", "2004-01-15T00:00:00Z", "zzzz", "\\u683C\\u6797\\u5C3C\\u6CBB\\u6807\\u51C6\\u65F6\\u95F4", "+0:00" },
        { "zh", "Europe/London", "2004-07-15T00:00:00Z", "Z", "+0100", "+1:00" },
        { "zh", "Europe/London", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+01:00", "+1:00" },
        { "zh", "Europe/London", "2004-07-15T00:00:00Z", "z", "GMT+1", "+1:00" },
        { "zh", "Europe/London", "2004-07-15T00:00:00Z", "zzzz", "\\u82f1\\u56fd\\u590f\\u4ee4\\u65f6\\u95f4", "+1:00" },
        { "zh", "Europe/London", "2004-07-15T00:00:00Z", "v", "\\u82f1\\u56fd\\u65f6\\u95f4", "Europe/London" },
        { "zh", "Europe/London", "2004-07-15T00:00:00Z", "vvvv", "\\u82f1\\u56fd\\u65f6\\u95f4", "Europe/London" },
        { "zh", "Europe/London", "2004-07-15T00:00:00Z", "VVVV", "\\u82f1\\u56fd\\u65f6\\u95f4", "Europe/London" },

        { "zh", "Etc/GMT+3", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-01-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-07-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-07-15T00:00:00Z", "v", "GMT-3", "-3:00" },
        { "zh", "Etc/GMT+3", "2004-07-15T00:00:00Z", "vvvv", "GMT-03:00", "-3:00" },

        // JB#5150
        { "zh", "Asia/Calcutta", "2004-01-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "zh", "Asia/Calcutta", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "zh", "Asia/Calcutta", "2004-01-15T00:00:00Z", "z", "GMT+5:30", "+5:30" },
        { "zh", "Asia/Calcutta", "2004-01-15T00:00:00Z", "zzzz", "\\u5370\\u5ea6\\u65f6\\u95f4", "+5:30" },
        { "zh", "Asia/Calcutta", "2004-07-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "zh", "Asia/Calcutta", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "zh", "Asia/Calcutta", "2004-07-15T00:00:00Z", "z", "GMT+5:30", "+05:30" },
        { "zh", "Asia/Calcutta", "2004-07-15T00:00:00Z", "zzzz", "\\u5370\\u5ea6\\u65f6\\u95f4", "+5:30" },
        { "zh", "Asia/Calcutta", "2004-07-15T00:00:00Z", "v", "\\u5370\\u5ea6\\u65f6\\u95f4", "Asia/Calcutta" },
        { "zh", "Asia/Calcutta", "2004-07-15T00:00:00Z", "vvvv", "\\u5370\\u5ea6\\u65f6\\u95f4", "Asia/Calcutta" },

        // ==========

        { "hi", "America/Los_Angeles", "2004-01-15T00:00:00Z", "Z", "-0800", "-8:00" },
        { "hi", "America/Los_Angeles", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-08:00", "-8:00" },
        { "hi", "America/Los_Angeles", "2004-01-15T00:00:00Z", "z", "GMT-8", "-8:00" },
        { "hi", "America/Los_Angeles", "2004-01-15T00:00:00Z", "zzzz", "\\u0909\\u0924\\u094d\\u0924\\u0930\\u0940 \\u0905\\u092e\\u0947\\u0930\\u093f\\u0915\\u0940 \\u092a\\u094d\\u0930\\u0936\\u093e\\u0902\\u0924 \\u092e\\u093e\\u0928\\u0915 \\u0938\\u092e\\u092f", "-8:00" },
        { "hi", "America/Los_Angeles", "2004-07-15T00:00:00Z", "Z", "-0700", "-7:00" },
        { "hi", "America/Los_Angeles", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-07:00", "-7:00" },
        { "hi", "America/Los_Angeles", "2004-07-15T00:00:00Z", "z", "GMT-7", "-7:00" },
        { "hi", "America/Los_Angeles", "2004-07-15T00:00:00Z", "zzzz", "\\u0909\\u0924\\u094d\\u0924\\u0930\\u0940 \\u0905\\u092e\\u0947\\u0930\\u093f\\u0915\\u0940 \\u092a\\u094d\\u0930\\u0936\\u093e\\u0902\\u0924 \\u0921\\u0947\\u0932\\u093e\\u0907\\u091f \\u0938\\u092e\\u092f", "-7:00" },
        { "hi", "America/Los_Angeles", "2004-07-15T00:00:00Z", "v",  "\\u0932\\u0949\\u0938 \\u090f\\u0902\\u091c\\u093f\\u0932\\u094d\\u0938 \\u0938\\u092e\\u092f", "America/Los_Angeles" },
        { "hi", "America/Los_Angeles", "2004-07-15T00:00:00Z", "vvvv", "\\u0909\\u0924\\u094d\\u0924\\u0930\\u0940 \\u0905\\u092e\\u0947\\u0930\\u093f\\u0915\\u0940 \\u092a\\u094d\\u0930\\u0936\\u093e\\u0902\\u0924 \\u0938\\u092e\\u092f", "America/Los_Angeles" },

        { "hi", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u0905\\u0930\\u094D\\u091C\\u0947\\u0902\\u091F\\u0940\\u0928\\u093E \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "-3:00" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u0905\\u0930\\u094D\\u091C\\u0947\\u0902\\u091F\\u0940\\u0928\\u093E \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "-3:00" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u092C\\u094D\\u092F\\u0942\\u0928\\u0938 \\u0906\\u092F\\u0930\\u0938 \\u0938\\u092E\\u092F", "America/Buenos_Aires" },
        { "hi", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u0905\\u0930\\u094D\\u091C\\u0947\\u0902\\u091F\\u0940\\u0928\\u093E \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "America/Buenos_Aires" },

        { "hi", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "hi", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "hi", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "hi", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u0905\\u0930\\u094D\\u091C\\u0947\\u0902\\u091F\\u0940\\u0928\\u093E \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "-3:00" },
        { "hi", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "hi", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "hi", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "hi", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u0905\\u0930\\u094D\\u091C\\u0947\\u0902\\u091F\\u0940\\u0928\\u093E \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "-3:00" },
        { "hi", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u092C\\u094D\\u092F\\u0942\\u0928\\u0938 \\u0906\\u092F\\u0930\\u0938 \\u0938\\u092E\\u092F", "America/Buenos_Aires" },
        { "hi", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u0905\\u0930\\u094D\\u091C\\u0947\\u0902\\u091F\\u0940\\u0928\\u093E \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "America/Buenos_Aires" },

        { "hi", "America/Havana", "2004-01-15T00:00:00Z", "Z", "-0500", "-5:00" },
        { "hi", "America/Havana", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-05:00", "-5:00" },
        { "hi", "America/Havana", "2004-01-15T00:00:00Z", "z", "GMT-5", "-5:00" },
        { "hi", "America/Havana", "2004-01-15T00:00:00Z", "zzzz", "\\u0915\\u094d\\u092f\\u0942\\u092c\\u093e \\u092e\\u093e\\u0928\\u0915 \\u0938\\u092e\\u092f", "-5:00" },
        { "hi", "America/Havana", "2004-07-15T00:00:00Z", "Z", "-0400", "-4:00" },
        { "hi", "America/Havana", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-04:00", "-4:00" },
        { "hi", "America/Havana", "2004-07-15T00:00:00Z", "z", "GMT-4", "-4:00" },
        { "hi", "America/Havana", "2004-07-15T00:00:00Z", "zzzz", "\\u0915\\u094d\\u092f\\u0942\\u092c\\u093e \\u0921\\u0947\\u0932\\u093e\\u0907\\u091f \\u0938\\u092e\\u092f", "-4:00" },
        { "hi", "America/Havana", "2004-07-15T00:00:00Z", "v", "\\u0915\\u094d\\u092f\\u0942\\u092c\\u093e \\u0938\\u092E\\u092F", "America/Havana" },
        { "hi", "America/Havana", "2004-07-15T00:00:00Z", "vvvv", "\\u0915\\u094d\\u092f\\u0942\\u092c\\u093e \\u0938\\u092e\\u092f", "America/Havana" },

        { "hi", "Australia/ACT", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "hi", "Australia/ACT", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "hi", "Australia/ACT", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "hi", "Australia/ACT", "2004-01-15T00:00:00Z", "zzzz", "\\u0911\\u0938\\u094d\\u200d\\u091f\\u094d\\u0930\\u0947\\u0932\\u093f\\u092f\\u093e\\u0908 \\u092a\\u0942\\u0930\\u094d\\u0935\\u0940 \\u0921\\u0947\\u0932\\u093e\\u0907\\u091f \\u0938\\u092e\\u092f", "+11:00" },
        { "hi", "Australia/ACT", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "hi", "Australia/ACT", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "hi", "Australia/ACT", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "hi", "Australia/ACT", "2004-07-15T00:00:00Z", "zzzz", "\\u0911\\u0938\\u094d\\u200d\\u091f\\u094d\\u0930\\u0947\\u0932\\u093f\\u092f\\u093e\\u0908 \\u092a\\u0942\\u0930\\u094d\\u0935\\u0940 \\u092e\\u093e\\u0928\\u0915 \\u0938\\u092e\\u092f", "+10:00" },
        { "hi", "Australia/ACT", "2004-07-15T00:00:00Z", "v", "\\u0938\\u093F\\u0921\\u0928\\u0940 \\u0938\\u092E\\u092F", "Australia/Sydney" },
        { "hi", "Australia/ACT", "2004-07-15T00:00:00Z", "vvvv", "\\u092a\\u0942\\u0930\\u094d\\u0935\\u0940 \\u0911\\u0938\\u094d\\u091f\\u094d\\u0930\\u0947\\u0932\\u093f\\u092f\\u093e \\u0938\\u092e\\u092f", "Australia/Sydney" },

        { "hi", "Australia/Sydney", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "hi", "Australia/Sydney", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "hi", "Australia/Sydney", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "hi", "Australia/Sydney", "2004-01-15T00:00:00Z", "zzzz", "\\u0911\\u0938\\u094d\\u200d\\u091f\\u094d\\u0930\\u0947\\u0932\\u093f\\u092f\\u093e\\u0908 \\u092a\\u0942\\u0930\\u094d\\u0935\\u0940 \\u0921\\u0947\\u0932\\u093e\\u0907\\u091f \\u0938\\u092e\\u092f", "+11:00" },
        { "hi", "Australia/Sydney", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "hi", "Australia/Sydney", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "hi", "Australia/Sydney", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "hi", "Australia/Sydney", "2004-07-15T00:00:00Z", "zzzz", "\\u0911\\u0938\\u094d\\u200d\\u091f\\u094d\\u0930\\u0947\\u0932\\u093f\\u092f\\u093e\\u0908 \\u092a\\u0942\\u0930\\u094d\\u0935\\u0940 \\u092e\\u093e\\u0928\\u0915 \\u0938\\u092e\\u092f", "+10:00" },
        { "hi", "Australia/Sydney", "2004-07-15T00:00:00Z", "v", "\\u0938\\u093F\\u0921\\u0928\\u0940 \\u0938\\u092E\\u092F", "Australia/Sydney" },
        { "hi", "Australia/Sydney", "2004-07-15T00:00:00Z", "vvvv", "\\u092a\\u0942\\u0930\\u094d\\u0935\\u0940 \\u0911\\u0938\\u094d\\u091f\\u094d\\u0930\\u0947\\u0932\\u093f\\u092f\\u093e \\u0938\\u092e\\u092f", "Australia/Sydney" },

        { "hi", "Europe/London", "2004-01-15T00:00:00Z", "Z", "+0000", "+0:00" },
        { "hi", "Europe/London", "2004-01-15T00:00:00Z", "ZZZZ", "GMT", "+0:00" },
        { "hi", "Europe/London", "2004-01-15T00:00:00Z", "z", "GMT", "+0:00" },
        { "hi", "Europe/London", "2004-01-15T00:00:00Z", "zzzz", "\\u0917\\u094d\\u0930\\u0940\\u0928\\u0935\\u093f\\u091a \\u092e\\u0940\\u0928 \\u091f\\u093e\\u0907\\u092e", "+0:00" },
        { "hi", "Europe/London", "2004-07-15T00:00:00Z", "Z", "+0100", "+1:00" },
        { "hi", "Europe/London", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+01:00", "+1:00" },
        { "hi", "Europe/London", "2004-07-15T00:00:00Z", "z", "GMT+1", "+1:00" },
        { "hi", "Europe/London", "2004-07-15T00:00:00Z", "zzzz", "\\u092c\\u094d\\u0930\\u093f\\u091f\\u093f\\u0936 \\u0917\\u094d\\u0930\\u0940\\u0937\\u094d\\u092e\\u0915\\u093e\\u0932\\u0940\\u0928 \\u0938\\u092e\\u092f", "+1:00" },
        { "hi", "Europe/London", "2004-07-15T00:00:00Z", "v", "\\u092f\\u0942\\u0928\\u093e\\u0907\\u091f\\u0947\\u0921 \\u0915\\u093f\\u0902\\u0917\\u0921\\u092e \\u0938\\u092e\\u092f", "Europe/London" },
        { "hi", "Europe/London", "2004-07-15T00:00:00Z", "vvvv", "\\u092f\\u0942\\u0928\\u093e\\u0907\\u091f\\u0947\\u0921 \\u0915\\u093f\\u0902\\u0917\\u0921\\u092e \\u0938\\u092e\\u092f", "Europe/London" },

        { "hi", "Etc/GMT+3", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-01-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-07-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-07-15T00:00:00Z", "v", "GMT-3", "-3:00" },
        { "hi", "Etc/GMT+3", "2004-07-15T00:00:00Z", "vvvv", "GMT-03:00", "-3:00" },

        { "hi", "Asia/Calcutta", "2004-01-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "hi", "Asia/Calcutta", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "hi", "Asia/Calcutta", "2004-01-15T00:00:00Z", "z", "IST", "+5:30" },
        { "hi", "Asia/Calcutta", "2004-01-15T00:00:00Z", "zzzz", "\\u092D\\u093E\\u0930\\u0924\\u0940\\u092F \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "+5:30" },
        { "hi", "Asia/Calcutta", "2004-07-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "hi", "Asia/Calcutta", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "hi", "Asia/Calcutta", "2004-07-15T00:00:00Z", "z", "IST", "+05:30" },
        { "hi", "Asia/Calcutta", "2004-07-15T00:00:00Z", "zzzz", "\\u092D\\u093E\\u0930\\u0924\\u0940\\u092F \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "+5:30" },
        { "hi", "Asia/Calcutta", "2004-07-15T00:00:00Z", "v", "IST", "Asia/Calcutta" },
        { "hi", "Asia/Calcutta", "2004-07-15T00:00:00Z", "vvvv", "\\u092D\\u093E\\u0930\\u0924\\u0940\\u092F \\u092E\\u093E\\u0928\\u0915 \\u0938\\u092E\\u092F", "Asia/Calcutta" },

        // ==========

        { "bg", "America/Los_Angeles", "2004-01-15T00:00:00Z", "Z", "-0800", "-8:00" },
        { "bg", "America/Los_Angeles", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-08:00", "-8:00" },
        { "bg", "America/Los_Angeles", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-8", "America/Los_Angeles" },
        { "bg", "America/Los_Angeles", "2004-01-15T00:00:00Z", "zzzz", "\\u0421\\u0435\\u0432\\u0435\\u0440\\u043d\\u043e\\u0430\\u043c\\u0435\\u0440\\u0438\\u043a\\u0430\\u043d\\u0441\\u043a\\u043e \\u0442\\u0438\\u0445\\u043e\\u043e\\u043a\\u0435\\u0430\\u043d\\u0441\\u043a\\u043e \\u0441\\u0442\\u0430\\u043d\\u0434\\u0430\\u0440\\u0442\\u043d\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "America/Los_Angeles" },
        { "bg", "America/Los_Angeles", "2004-07-15T00:00:00Z", "Z", "-0700", "-7:00" },
        { "bg", "America/Los_Angeles", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-07:00", "-7:00" },
        { "bg", "America/Los_Angeles", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-7", "America/Los_Angeles" },
        { "bg", "America/Los_Angeles", "2004-07-15T00:00:00Z", "zzzz", "\\u0421\\u0435\\u0432\\u0435\\u0440\\u043d\\u043e\\u0430\\u043c\\u0435\\u0440\\u0438\\u043a\\u0430\\u043d\\u0441\\u043a\\u043e \\u0442\\u0438\\u0445\\u043e\\u043e\\u043a\\u0435\\u0430\\u043d\\u0441\\u043a\\u043e \\u043b\\u044f\\u0442\\u043d\\u043e \\u0447\\u0430\\u0441\\u043e\\u0432\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "America/Los_Angeles" },
    // icu bg.txt has exemplar city for this time zone
        { "bg", "America/Los_Angeles", "2004-07-15T00:00:00Z", "v", "\\u041B\\u043E\\u0441 \\u0410\\u043D\\u0434\\u0436\\u0435\\u043B\\u0438\\u0441", "America/Los_Angeles" },
        { "bg", "America/Los_Angeles", "2004-07-15T00:00:00Z", "vvvv", "\\u0421\\u0435\\u0432\\u0435\\u0440\\u043d\\u043e\\u0430\\u043c\\u0435\\u0440\\u0438\\u043a\\u0430\\u043d\\u0441\\u043a\\u043e \\u0442\\u0438\\u0445\\u043e\\u043e\\u043a\\u0435\\u0430\\u043d\\u0441\\u043a\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "America/Los_Angeles" },
        { "bg", "America/Los_Angeles", "2004-07-15T00:00:00Z", "VVVV", "\\u041B\\u043E\\u0441 \\u0410\\u043D\\u0434\\u0436\\u0435\\u043B\\u0438\\u0441", "America/Los_Angeles" },

        { "bg", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-3", "-3:00" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u0410\\u0440\\u0436\\u0435\\u043D\\u0442\\u0438\\u043D\\u0441\\u043a\\u043e \\u0441\\u0442\\u0430\\u043d\\u0434\\u0430\\u0440\\u0442\\u043d\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "-3:00" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-3", "-3:00" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u0410\\u0440\\u0436\\u0435\\u043D\\u0442\\u0438\\u043D\\u0441\\u043a\\u043e \\u0441\\u0442\\u0430\\u043d\\u0434\\u0430\\u0440\\u0442\\u043d\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "-3:00" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u0411\\u0443\\u0435\\u043D\\u043E\\u0441 \\u0410\\u0439\\u0440\\u0435\\u0441", "America/Buenos_Aires" },
        { "bg", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u0410\\u0440\\u0436\\u0435\\u043D\\u0442\\u0438\\u043D\\u0441\\u043a\\u043e \\u0441\\u0442\\u0430\\u043d\\u0434\\u0430\\u0440\\u0442\\u043d\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "America/Buenos_Aires" },

        { "bg", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "bg", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },
        { "bg", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-3", "-3:00" },
        { "bg", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u0410\\u0440\\u0436\\u0435\\u043D\\u0442\\u0438\\u043D\\u0441\\u043a\\u043e \\u0441\\u0442\\u0430\\u043d\\u0434\\u0430\\u0440\\u0442\\u043d\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "-3:00" },
        { "bg", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "bg", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },
        { "bg", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-3", "-3:00" },
        { "bg", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u0410\\u0440\\u0436\\u0435\\u043D\\u0442\\u0438\\u043D\\u0441\\u043a\\u043e \\u0441\\u0442\\u0430\\u043d\\u0434\\u0430\\u0440\\u0442\\u043d\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "-3:00" },
    // icu bg.txt does not have info for this time zone
        { "bg", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u0411\\u0443\\u0435\\u043D\\u043E\\u0441 \\u0410\\u0439\\u0440\\u0435\\u0441", "America/Buenos_Aires" },
        { "bg", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u0410\\u0440\\u0436\\u0435\\u043D\\u0442\\u0438\\u043D\\u0441\\u043a\\u043e \\u0441\\u0442\\u0430\\u043d\\u0434\\u0430\\u0440\\u0442\\u043d\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "America/Buenos_Aires" },

        { "bg", "America/Havana", "2004-01-15T00:00:00Z", "Z", "-0500", "-5:00" },
        { "bg", "America/Havana", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-05:00", "-5:00" },
        { "bg", "America/Havana", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-5", "-5:00" },
        { "bg", "America/Havana", "2004-01-15T00:00:00Z", "zzzz", "\\u041a\\u0443\\u0431\\u0438\\u043d\\u0441\\u043a\\u043e \\u0441\\u0442\\u0430\\u043d\\u0434\\u0430\\u0440\\u0442\\u043d\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "-5:00" },
        { "bg", "America/Havana", "2004-07-15T00:00:00Z", "Z", "-0400", "-4:00" },
        { "bg", "America/Havana", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-04:00", "-4:00" },
        { "bg", "America/Havana", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-4", "-4:00" },
        { "bg", "America/Havana", "2004-07-15T00:00:00Z", "zzzz", "\\u041a\\u0443\\u0431\\u0438\\u043d\\u0441\\u043a\\u043e \\u043b\\u044f\\u0442\\u043d\\u043e \\u0447\\u0430\\u0441\\u043e\\u0432\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "-4:00" },
        { "bg", "America/Havana", "2004-07-15T00:00:00Z", "v", "\\u041a\\u0443\\u0431\\u0430", "America/Havana" },
        { "bg", "America/Havana", "2004-07-15T00:00:00Z", "vvvv", "\\u041a\\u0443\\u0431\\u0438\\u043d\\u0441\\u043a\\u043e \\u0432\\u0440\\u0435\\u043C\\u0435", "America/Havana" },

        { "bg", "Australia/ACT", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "bg", "Australia/ACT", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+11:00", "+11:00" },
        { "bg", "Australia/ACT", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+11", "+11:00" },
        { "bg", "Australia/ACT", "2004-01-15T00:00:00Z", "zzzz", "\\u0418\\u0437\\u0442\\u043E\\u0447\\u043D\\u043E\\u0430\\u0432\\u0441\\u0442\\u0440\\u0430\\u043B\\u0438\\u0439\\u0441\\u043A\\u043E \\u043B\\u044F\\u0442\\u043D\\u043E \\u0447\\u0430\\u0441\\u043E\\u0432\\u043E \\u0432\\u0440\\u0435\\u043C\\u0435", "+11:00" },
        { "bg", "Australia/ACT", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "bg", "Australia/ACT", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+10:00", "+10:00" },
        { "bg", "Australia/ACT", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+10", "+10:00" },
        { "bg", "Australia/ACT", "2004-07-15T00:00:00Z", "zzzz", "\\u0418\\u0437\\u0442\\u043E\\u0447\\u043D\\u043E\\u0430\\u0432\\u0441\\u0442\\u0440\\u0430\\u043B\\u0438\\u0439\\u0441\\u043A\\u043E \\u0441\\u0442\\u0430\\u043D\\u0434\\u0430\\u0440\\u0442\\u043D\\u043E \\u0432\\u0440\\u0435\\u043C\\u0435", "+10:00" },
        { "bg", "Australia/ACT", "2004-07-15T00:00:00Z", "v", "\\u0421\\u0438\\u0434\\u043D\\u0438", "Australia/Sydney" },
        { "bg", "Australia/ACT", "2004-07-15T00:00:00Z", "vvvv", "\\u0418\\u0437\\u0442\\u043E\\u0447\\u043D\\u043E\\u0430\\u0432\\u0441\\u0442\\u0440\\u0430\\u043B\\u0438\\u0439\\u0441\\u043A\\u043E \\u0432\\u0440\\u0435\\u043C\\u0435", "Australia/Sydney" },

        { "bg", "Australia/Sydney", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "bg", "Australia/Sydney", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+11:00", "+11:00" },
        { "bg", "Australia/Sydney", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+11", "+11:00" },
        { "bg", "Australia/Sydney", "2004-01-15T00:00:00Z", "zzzz", "\\u0418\\u0437\\u0442\\u043E\\u0447\\u043D\\u043E\\u0430\\u0432\\u0441\\u0442\\u0440\\u0430\\u043B\\u0438\\u0439\\u0441\\u043A\\u043E \\u043B\\u044F\\u0442\\u043D\\u043E \\u0447\\u0430\\u0441\\u043E\\u0432\\u043E \\u0432\\u0440\\u0435\\u043C\\u0435", "+11:00" },
        { "bg", "Australia/Sydney", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "bg", "Australia/Sydney", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+10:00", "+10:00" },
        { "bg", "Australia/Sydney", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+10", "+10:00" },
        { "bg", "Australia/Sydney", "2004-07-15T00:00:00Z", "zzzz", "\\u0418\\u0437\\u0442\\u043E\\u0447\\u043D\\u043E\\u0430\\u0432\\u0441\\u0442\\u0440\\u0430\\u043B\\u0438\\u0439\\u0441\\u043A\\u043E \\u0441\\u0442\\u0430\\u043D\\u0434\\u0430\\u0440\\u0442\\u043D\\u043E \\u0432\\u0440\\u0435\\u043C\\u0435", "+10:00" },
        { "bg", "Australia/Sydney", "2004-07-15T00:00:00Z", "v", "\\u0421\\u0438\\u0434\\u043D\\u0438", "Australia/Sydney" },
        { "bg", "Australia/Sydney", "2004-07-15T00:00:00Z", "vvvv", "\\u0418\\u0437\\u0442\\u043E\\u0447\\u043D\\u043E\\u0430\\u0432\\u0441\\u0442\\u0440\\u0430\\u043B\\u0438\\u0439\\u0441\\u043A\\u043E \\u0432\\u0440\\u0435\\u043C\\u0435", "Australia/Sydney" },

        { "bg", "Europe/London", "2004-01-15T00:00:00Z", "Z", "+0000", "+0:00" },
        { "bg", "Europe/London", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447", "+0:00" },
        { "bg", "Europe/London", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447", "+0:00" },
        { "bg", "Europe/London", "2004-01-15T00:00:00Z", "zzzz", "\\u0421\\u0440\\u0435\\u0434\\u043d\\u043e \\u0433\\u0440\\u0438\\u043d\\u0443\\u0438\\u0447\\u043a\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "+0:00" },
        { "bg", "Europe/London", "2004-07-15T00:00:00Z", "Z", "+0100", "+1:00" },
        { "bg", "Europe/London", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+01:00", "+1:00" },
        { "bg", "Europe/London", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+1", "+1:00" },
        { "bg", "Europe/London", "2004-07-15T00:00:00Z", "zzzz", "\\u0411\\u0440\\u0438\\u0442\\u0430\\u043d\\u0441\\u043a\\u043e \\u043b\\u044f\\u0442\\u043d\\u043e \\u0447\\u0430\\u0441\\u043e\\u0432\\u043e \\u0432\\u0440\\u0435\\u043c\\u0435", "+1:00" },
        { "bg", "Europe/London", "2004-07-15T00:00:00Z", "v", "\\u041E\\u0431\\u0435\\u0434\\u0438\\u043D\\u0435\\u043D\\u043E\\u0442\\u043E \\u043A\\u0440\\u0430\\u043B\\u0441\\u0442\\u0432\\u043E", "Europe/London" },
        { "bg", "Europe/London", "2004-07-15T00:00:00Z", "vvvv", "\\u041E\\u0431\\u0435\\u0434\\u0438\\u043D\\u0435\\u043D\\u043E\\u0442\\u043E \\u043A\\u0440\\u0430\\u043B\\u0441\\u0442\\u0432\\u043E", "Europe/London" },

        { "bg", "Etc/GMT+3", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-3", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-01-15T00:00:00Z", "zzzz", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-3", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-07-15T00:00:00Z", "zzzz", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-07-15T00:00:00Z", "v", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-3", "-3:00" },
        { "bg", "Etc/GMT+3", "2004-07-15T00:00:00Z", "vvvv", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447-03:00", "-3:00" },

        // JB#5150
        { "bg", "Asia/Calcutta", "2004-01-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "bg", "Asia/Calcutta", "2004-01-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+05:30", "+5:30" },
        { "bg", "Asia/Calcutta", "2004-01-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+5:30", "+5:30" },
        { "bg", "Asia/Calcutta", "2004-01-15T00:00:00Z", "zzzz", "\\u0418\\u043D\\u0434\\u0438\\u0439\\u0441\\u043A\\u043E \\u0432\\u0440\\u0435\\u043c\\u0435", "+5:30" },
        { "bg", "Asia/Calcutta", "2004-07-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "bg", "Asia/Calcutta", "2004-07-15T00:00:00Z", "ZZZZ", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+05:30", "+5:30" },
        { "bg", "Asia/Calcutta", "2004-07-15T00:00:00Z", "z", "\\u0413\\u0440\\u0438\\u043D\\u0443\\u0438\\u0447+5:30", "+05:30" },
        { "bg", "Asia/Calcutta", "2004-07-15T00:00:00Z", "zzzz", "\\u0418\\u043D\\u0434\\u0438\\u0439\\u0441\\u043A\\u043E \\u0432\\u0440\\u0435\\u043c\\u0435", "+5:30" },
        { "bg", "Asia/Calcutta", "2004-07-15T00:00:00Z", "v", "\\u0418\\u043D\\u0434\\u0438\\u044F", "Asia/Calcutta" },
        { "bg", "Asia/Calcutta", "2004-07-15T00:00:00Z", "vvvv", "\\u0418\\u043D\\u0434\\u0438\\u0439\\u0441\\u043A\\u043E \\u0432\\u0440\\u0435\\u043c\\u0435", "Asia/Calcutta" },
    // ==========

        { "ja", "America/Los_Angeles", "2004-01-15T00:00:00Z", "Z", "-0800", "-8:00" },
        { "ja", "America/Los_Angeles", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-08:00", "-8:00" },
        { "ja", "America/Los_Angeles", "2004-01-15T00:00:00Z", "z", "GMT-8", "America/Los_Angeles" },
        { "ja", "America/Los_Angeles", "2004-01-15T00:00:00Z", "zzzz", "\\u30a2\\u30e1\\u30ea\\u30ab\\u592a\\u5e73\\u6d0b\\u6a19\\u6e96\\u6642", "America/Los_Angeles" },
        { "ja", "America/Los_Angeles", "2004-07-15T00:00:00Z", "Z", "-0700", "-700" },
        { "ja", "America/Los_Angeles", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-07:00", "-7:00" },
        { "ja", "America/Los_Angeles", "2004-07-15T00:00:00Z", "z", "GMT-7", "America/Los_Angeles" },
        { "ja", "America/Los_Angeles", "2004-07-15T00:00:00Z", "zzzz", "\\u30a2\\u30e1\\u30ea\\u30ab\\u592a\\u5e73\\u6d0b\\u590f\\u6642\\u9593", "America/Los_Angeles" },
    // icu ja.txt has exemplar city for this time zone
        { "ja", "America/Los_Angeles", "2004-07-15T00:00:00Z", "v", "\\u30ED\\u30B5\\u30F3\\u30BC\\u30EB\\u30B9\\u6642\\u9593", "America/Los_Angeles" },
        { "ja", "America/Los_Angeles", "2004-07-15T00:00:00Z", "vvvv", "\\u30A2\\u30E1\\u30EA\\u30AB\\u592A\\u5e73\\u6D0B\\u6642\\u9593", "America/Los_Angeles" },
        { "ja", "America/Los_Angeles", "2004-07-15T00:00:00Z", "VVVV", "\\u30ED\\u30B5\\u30F3\\u30BC\\u30EB\\u30B9\\u6642\\u9593", "America/Los_Angeles" },

        { "ja", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ja", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ja", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ja", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u30A2\\u30EB\\u30BC\\u30F3\\u30C1\\u30F3\\u6A19\\u6E96\\u6642", "-3:00" },
        { "ja", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ja", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ja", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ja", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u30A2\\u30EB\\u30BC\\u30F3\\u30C1\\u30F3\\u6A19\\u6E96\\u6642", "-3:00" },
    // icu ja.txt does not have info for this time zone
        { "ja", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u30D6\\u30A8\\u30CE\\u30B9\\u30A2\\u30A4\\u30EC\\u30B9\\u6642\\u9593", "America/Buenos_Aires" },
        { "ja", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u30A2\\u30EB\\u30BC\\u30F3\\u30C1\\u30F3\\u6A19\\u6E96\\u6642", "America/Buenos_Aires" },

        { "ja", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ja", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ja", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ja", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u30A2\\u30EB\\u30BC\\u30F3\\u30C1\\u30F3\\u6A19\\u6E96\\u6642", "-3:00" },
        { "ja", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ja", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ja", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ja", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u30A2\\u30EB\\u30BC\\u30F3\\u30C1\\u30F3\\u6A19\\u6E96\\u6642", "-3:00" },
        { "ja", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u30D6\\u30A8\\u30CE\\u30B9\\u30A2\\u30A4\\u30EC\\u30B9\\u6642\\u9593", "America/Buenos_Aires" },
        { "ja", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u30A2\\u30EB\\u30BC\\u30F3\\u30C1\\u30F3\\u6A19\\u6E96\\u6642", "America/Buenos_Aires" },

        { "ja", "America/Havana", "2004-01-15T00:00:00Z", "Z", "-0500", "-5:00" },
        { "ja", "America/Havana", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-05:00", "-5:00" },
        { "ja", "America/Havana", "2004-01-15T00:00:00Z", "z", "GMT-5", "-5:00" },
        { "ja", "America/Havana", "2004-01-15T00:00:00Z", "zzzz", "\\u30AD\\u30E5\\u30FC\\u30D0\\u6A19\\u6E96\\u6642", "-5:00" },
        { "ja", "America/Havana", "2004-07-15T00:00:00Z", "Z", "-0400", "-4:00" },
        { "ja", "America/Havana", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-04:00", "-4:00" },
        { "ja", "America/Havana", "2004-07-15T00:00:00Z", "z", "GMT-4", "-4:00" },
        { "ja", "America/Havana", "2004-07-15T00:00:00Z", "zzzz", "\\u30AD\\u30E5\\u30FC\\u30D0\\u590F\\u6642\\u9593", "-4:00" },
        { "ja", "America/Havana", "2004-07-15T00:00:00Z", "v", "\\u30ad\\u30e5\\u30fc\\u30d0\\u6642\\u9593", "America/Havana" },
        { "ja", "America/Havana", "2004-07-15T00:00:00Z", "vvvv", "\\u30ad\\u30e5\\u30fc\\u30d0\\u6642\\u9593", "America/Havana" },

        { "ja", "Australia/ACT", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "ja", "Australia/ACT", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "ja", "Australia/ACT", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "ja", "Australia/ACT", "2004-01-15T00:00:00Z", "zzzz", "\\u30AA\\u30FC\\u30B9\\u30C8\\u30E9\\u30EA\\u30A2\\u6771\\u90E8\\u590F\\u6642\\u9593", "+11:00" },
        { "ja", "Australia/ACT", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "ja", "Australia/ACT", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "ja", "Australia/ACT", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "ja", "Australia/ACT", "2004-07-15T00:00:00Z", "zzzz", "\\u30AA\\u30FC\\u30B9\\u30C8\\u30E9\\u30EA\\u30A2\\u6771\\u90E8\\u6A19\\u6E96\\u6642", "+10:00" },
    // icu ja.txt does not have info for this time zone
        { "ja", "Australia/ACT", "2004-07-15T00:00:00Z", "v", "\\u30B7\\u30C9\\u30CB\\u30FC\\u6642\\u9593", "Australia/Sydney" },
        { "ja", "Australia/ACT", "2004-07-15T00:00:00Z", "vvvv", "\\u30AA\\u30FC\\u30B9\\u30C8\\u30E9\\u30EA\\u30A2\\u6771\\u90E8\\u6642\\u9593", "Australia/Sydney" },

        { "ja", "Australia/Sydney", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "ja", "Australia/Sydney", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "ja", "Australia/Sydney", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "ja", "Australia/Sydney", "2004-01-15T00:00:00Z", "zzzz", "\\u30AA\\u30FC\\u30B9\\u30C8\\u30E9\\u30EA\\u30A2\\u6771\\u90E8\\u590F\\u6642\\u9593", "+11:00" },
        { "ja", "Australia/Sydney", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "ja", "Australia/Sydney", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "ja", "Australia/Sydney", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "ja", "Australia/Sydney", "2004-07-15T00:00:00Z", "zzzz", "\\u30AA\\u30FC\\u30B9\\u30C8\\u30E9\\u30EA\\u30A2\\u6771\\u90E8\\u6A19\\u6E96\\u6642", "+10:00" },
        { "ja", "Australia/Sydney", "2004-07-15T00:00:00Z", "v", "\\u30B7\\u30C9\\u30CB\\u30FC\\u6642\\u9593", "Australia/Sydney" },
        { "ja", "Australia/Sydney", "2004-07-15T00:00:00Z", "vvvv", "\\u30AA\\u30FC\\u30B9\\u30C8\\u30E9\\u30EA\\u30A2\\u6771\\u90E8\\u6642\\u9593", "Australia/Sydney" },

        { "ja", "Europe/London", "2004-01-15T00:00:00Z", "Z", "+0000", "+0:00" },
        { "ja", "Europe/London", "2004-01-15T00:00:00Z", "ZZZZ", "GMT", "+0:00" },
        { "ja", "Europe/London", "2004-01-15T00:00:00Z", "z", "GMT", "+0:00" },
        { "ja", "Europe/London", "2004-01-15T00:00:00Z", "zzzz", "\\u30B0\\u30EA\\u30CB\\u30C3\\u30B8\\u6A19\\u6E96\\u6642", "+0:00" },
        { "ja", "Europe/London", "2004-07-15T00:00:00Z", "Z", "+0100", "+1:00" },
        { "ja", "Europe/London", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+01:00", "+1:00" },
        { "ja", "Europe/London", "2004-07-15T00:00:00Z", "z", "GMT+1", "+1:00" },
        { "ja", "Europe/London", "2004-07-15T00:00:00Z", "zzzz", "\\u82f1\\u56fd\\u590f\\u6642\\u9593", "+1:00" },
        { "ja", "Europe/London", "2004-07-15T00:00:00Z", "v", "\\u30a4\\u30ae\\u30ea\\u30b9\\u6642\\u9593", "Europe/London" },
        { "ja", "Europe/London", "2004-07-15T00:00:00Z", "vvvv", "\\u30a4\\u30ae\\u30ea\\u30b9\\u6642\\u9593", "Europe/London" },
        { "ja", "Europe/London", "2004-07-15T00:00:00Z", "VVVV", "\\u30a4\\u30ae\\u30ea\\u30b9\\u6642\\u9593", "Europe/London" },

        { "ja", "Etc/GMT+3", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-01-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-07-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-07-15T00:00:00Z", "v", "GMT-3", "-3:00" },
        { "ja", "Etc/GMT+3", "2004-07-15T00:00:00Z", "vvvv", "GMT-03:00", "-3:00" },

        // JB#5150
        { "ja", "Asia/Calcutta", "2004-01-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "ja", "Asia/Calcutta", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "ja", "Asia/Calcutta", "2004-01-15T00:00:00Z", "z", "GMT+5:30", "+5:30" },
        { "ja", "Asia/Calcutta", "2004-01-15T00:00:00Z", "zzzz", "\\u30A4\\u30F3\\u30C9\\u6A19\\u6E96\\u6642", "+5:30" },
        { "ja", "Asia/Calcutta", "2004-07-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "ja", "Asia/Calcutta", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "ja", "Asia/Calcutta", "2004-07-15T00:00:00Z", "z", "GMT+5:30", "+05:30" },
        { "ja", "Asia/Calcutta", "2004-07-15T00:00:00Z", "zzzz", "\\u30A4\\u30F3\\u30C9\\u6A19\\u6E96\\u6642", "+5:30" },
        { "ja", "Asia/Calcutta", "2004-07-15T00:00:00Z", "v", "\\u30A4\\u30F3\\u30C9\\u6642\\u9593", "Asia/Calcutta" },
        { "ja", "Asia/Calcutta", "2004-07-15T00:00:00Z", "vvvv", "\\u30A4\\u30F3\\u30C9\\u6A19\\u6E96\\u6642", "Asia/Calcutta" },

    // ==========

        { "ti", "America/Los_Angeles", "2004-01-15T00:00:00Z", "Z", "-0800", "-8:00" },
        { "ti", "America/Los_Angeles", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-08:00", "-8:00" },
        { "ti", "America/Los_Angeles", "2004-01-15T00:00:00Z", "z", "GMT-8", "-8:00" },
        { "ti", "America/Los_Angeles", "2004-01-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u130d\\u12d8 \\u1353\\u1235\\u134a\\u12ad", "-8:00" },
        { "ti", "America/Los_Angeles", "2004-07-15T00:00:00Z", "Z", "-0700", "-7:00" },
        { "ti", "America/Los_Angeles", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-07:00", "-7:00" },
        { "ti", "America/Los_Angeles", "2004-07-15T00:00:00Z", "z", "GMT-7", "-7:00" },
        { "ti", "America/Los_Angeles", "2004-07-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12d3\\u120d\\u1272 \\u130d\\u12d8 \\u1353\\u1235\\u134a\\u12ad", "-7:00" },
        { "ti", "America/Los_Angeles", "2004-07-15T00:00:00Z", "v", "\\u130d\\u12d8 \\u120e\\u1235 \\u12a3\\u1295\\u1300\\u1208\\u1235", "America/Los_Angeles" },
        { "ti", "America/Los_Angeles", "2004-07-15T00:00:00Z", "vvvv", "\\u1293\\u12ed \\u1353\\u1235\\u134a\\u12ad \\u130d\\u12d8", "America/Los_Angeles" },

        { "ti", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u130d\\u12d8 \\u12a3\\u122d\\u1300\\u1295\\u1272\\u1293", "-3:00" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u130d\\u12d8 \\u12a3\\u122d\\u1300\\u1295\\u1272\\u1293", "-3:00" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u130d\\u12d8 \\u1265\\u12c8\\u1296\\u1235 \\u12a3\\u12ed\\u1228\\u1235", "America/Buenos_Aires" },
        { "ti", "America/Argentina/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u130d\\u12d8 \\u12a3\\u122d\\u1300\\u1295\\u1272\\u1293", "America/Buenos_Aires" },

        { "ti", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ti", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ti", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ti", "America/Buenos_Aires", "2004-01-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u130d\\u12d8 \\u12a3\\u122d\\u1300\\u1295\\u1272\\u1293", "-3:00" },
        { "ti", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ti", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ti", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ti", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u130d\\u12d8 \\u12a3\\u122d\\u1300\\u1295\\u1272\\u1293", "-3:00" },
        { "ti", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "v", "\\u130d\\u12d8 \\u1265\\u12c8\\u1296\\u1235 \\u12a3\\u12ed\\u1228\\u1235", "America/Buenos_Aires" },
        { "ti", "America/Buenos_Aires", "2004-07-15T00:00:00Z", "vvvv", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u130d\\u12d8 \\u12a3\\u122d\\u1300\\u1295\\u1272\\u1293", "America/Buenos_Aires" },

        { "ti", "America/Havana", "2004-01-15T00:00:00Z", "Z", "-0500", "-5:00" },
        { "ti", "America/Havana", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-05:00", "-5:00" },
        { "ti", "America/Havana", "2004-01-15T00:00:00Z", "z", "GMT-5", "-5:00" },
        { "ti", "America/Havana", "2004-01-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u130d\\u12d8 \\u12a9\\u1263", "-5:00" },
        { "ti", "America/Havana", "2004-07-15T00:00:00Z", "Z", "-0400", "-4:00" },
        { "ti", "America/Havana", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-04:00", "-4:00" },
        { "ti", "America/Havana", "2004-07-15T00:00:00Z", "z", "GMT-4", "-4:00" },
        { "ti", "America/Havana", "2004-07-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12d3\\u120d\\u1272 \\u130d\\u12d8 \\u12a9\\u1263", "-4:00" },
        { "ti", "America/Havana", "2004-07-15T00:00:00Z", "v", "\\u130d\\u12d8 \\u12a9\\u1263", "America/Havana" },
        { "ti", "America/Havana", "2004-07-15T00:00:00Z", "vvvv", "\\u1293\\u12ed \\u12a9\\u1263 \\u130d\\u12d8", "America/Havana" },

        { "ti", "Australia/ACT", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "ti", "Australia/ACT", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "ti", "Australia/ACT", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "ti", "Australia/ACT", "2004-01-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u121d\\u1265\\u122b\\u1253\\u12ca \\u1218\\u12d3\\u120d\\u1272 \\u12a3\\u12cd\\u1235\\u1275\\u122b\\u120d\\u12eb \\u130d\\u12d8", "+11:00" },
        { "ti", "Australia/ACT", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "ti", "Australia/ACT", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "ti", "Australia/ACT", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" }, 
        { "ti", "Australia/ACT", "2004-07-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u121d\\u1265\\u122b\\u1253\\u12ca \\u1218\\u12f0\\u1260\\u129b \\u12a3\\u12cd\\u1235\\u1275\\u122b\\u120d\\u12eb \\u130d\\u12d8", "+10:00" },
        { "ti", "Australia/ACT", "2004-07-15T00:00:00Z", "v", "\\u130d\\u12d8 \\u1232\\u12f5\\u1292", "Australia/Sydney" },
        { "ti", "Australia/ACT", "2004-07-15T00:00:00Z", "vvvv", "\\u1293\\u12ed \\u121d\\u1265\\u122b\\u1253\\u12ca \\u12a3\\u12cd\\u1235\\u1275\\u122b\\u120d\\u12eb \\u130d\\u12d8", "Australia/Sydney" },

        { "ti", "Australia/Sydney", "2004-01-15T00:00:00Z", "Z", "+1100", "+11:00" },
        { "ti", "Australia/Sydney", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+11:00", "+11:00" },
        { "ti", "Australia/Sydney", "2004-01-15T00:00:00Z", "z", "GMT+11", "+11:00" },
        { "ti", "Australia/Sydney", "2004-01-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u121d\\u1265\\u122b\\u1253\\u12ca \\u1218\\u12d3\\u120d\\u1272 \\u12a3\\u12cd\\u1235\\u1275\\u122b\\u120d\\u12eb \\u130d\\u12d8", "+11:00" },
        { "ti", "Australia/Sydney", "2004-07-15T00:00:00Z", "Z", "+1000", "+10:00" },
        { "ti", "Australia/Sydney", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+10:00", "+10:00" },
        { "ti", "Australia/Sydney", "2004-07-15T00:00:00Z", "z", "GMT+10", "+10:00" },
        { "ti", "Australia/Sydney", "2004-07-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u121d\\u1265\\u122b\\u1253\\u12ca \\u1218\\u12f0\\u1260\\u129b \\u12a3\\u12cd\\u1235\\u1275\\u122b\\u120d\\u12eb \\u130d\\u12d8", "+10:00" },
        { "ti", "Australia/Sydney", "2004-07-15T00:00:00Z", "v", "\\u130d\\u12d8 \\u1232\\u12f5\\u1292", "Australia/Sydney" },
        { "ti", "Australia/Sydney", "2004-07-15T00:00:00Z", "vvvv", "\\u1293\\u12ed \\u121d\\u1265\\u122b\\u1253\\u12ca \\u12a3\\u12cd\\u1235\\u1275\\u122b\\u120d\\u12eb \\u130d\\u12d8", "Australia/Sydney" },

        { "ti", "Europe/London", "2004-01-15T00:00:00Z", "Z", "+0000", "+0:00" },
        { "ti", "Europe/London", "2004-01-15T00:00:00Z", "ZZZZ", "GMT", "+0:00" },
        { "ti", "Europe/London", "2004-01-15T00:00:00Z", "z", "GMT", "+0:00" },
        { "ti", "Europe/London", "2004-01-15T00:00:00Z", "zzzz", "GMT", "+0:00" },
        { "ti", "Europe/London", "2004-07-15T00:00:00Z", "Z", "+0100", "+1:00" },
        { "ti", "Europe/London", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+01:00", "+1:00" },
        { "ti", "Europe/London", "2004-07-15T00:00:00Z", "z", "GMT+1", "+1:00" },
        { "ti", "Europe/London", "2004-07-15T00:00:00Z", "zzzz", "\\u130D\\u12DC \\u12AD\\u1228\\u121D\\u1272 \\u1265\\u122A\\u1323\\u1295\\u12EB", "+1:00" },
        { "ti", "Europe/London", "2004-07-15T00:00:00Z", "v", "\\u130d\\u12d8 \\u1265\\u122a\\u1323\\u1295\\u12eb", "Europe/London" },
        { "ti", "Europe/London", "2004-07-15T00:00:00Z", "vvvv", "\\u130d\\u12d8 \\u1265\\u122a\\u1323\\u1295\\u12eb", "Europe/London" },

        { "ti", "Etc/GMT+3", "2004-01-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-01-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-01-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-01-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-07-15T00:00:00Z", "Z", "-0300", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-07-15T00:00:00Z", "ZZZZ", "GMT-03:00", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-07-15T00:00:00Z", "z", "GMT-3", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-07-15T00:00:00Z", "zzzz", "GMT-03:00", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-07-15T00:00:00Z", "v", "GMT-3", "-3:00" },
        { "ti", "Etc/GMT+3", "2004-07-15T00:00:00Z", "vvvv", "GMT-03:00", "-3:00" },

        // JB#5150
        { "ti", "Asia/Calcutta", "2004-01-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "ti", "Asia/Calcutta", "2004-01-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "ti", "Asia/Calcutta", "2004-01-15T00:00:00Z", "z", "GMT+5:30", "+5:30" },
        { "ti", "Asia/Calcutta", "2004-01-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u1205\\u1295\\u12f5 \\u130d\\u12d8", "+5:30" },
        { "ti", "Asia/Calcutta", "2004-07-15T00:00:00Z", "Z", "+0530", "+5:30" },
        { "ti", "Asia/Calcutta", "2004-07-15T00:00:00Z", "ZZZZ", "GMT+05:30", "+5:30" },
        { "ti", "Asia/Calcutta", "2004-07-15T00:00:00Z", "z", "GMT+5:30", "+05:30" },
        { "ti", "Asia/Calcutta", "2004-07-15T00:00:00Z", "zzzz", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u1205\\u1295\\u12f5 \\u130d\\u12d8", "+5:30" },
        { "ti", "Asia/Calcutta", "2004-07-15T00:00:00Z", "v", "\\u130d\\u12d8 \\u1205\\u1295\\u12f2", "Alna/Calcutta" },
        { "ti", "Asia/Calcutta", "2004-07-15T00:00:00Z", "vvvv", "\\u1293\\u12ed \\u1218\\u12f0\\u1260\\u129b \\u1205\\u1295\\u12f5 \\u130d\\u12d8", "Asia/Calcutta" },

        // Ticket#8589 Partial location name to use country name if the zone is the golden
        // zone for the time zone's country.
        { "en_MX", "America/Chicago", "1995-07-15T00:00:00Z", "vvvv", "Central Time (United States)", "America/Chicago"},

        // Tests proper handling of time zones that should have empty sets when inherited from the parent.
        // For example, en_GB understands CET as Central European Time, but en_HK, which inherits from en_GB
        // does not
        { "en_GB", "Europe/Paris", "2004-01-15T00:00:00Z", "zzzz", "Central European Standard Time", "+1:00"},
        { "en_GB", "Europe/Paris", "2004-07-15T00:00:00Z", "zzzz", "Central European Summer Time", "+2:00"},
        { "en_GB", "Europe/Paris", "2004-01-15T00:00:00Z", "z", "CET", "+1:00"},
        { "en_GB", "Europe/Paris", "2004-07-15T00:00:00Z", "z", "CEST", "+2:00"},
        { "en_HK", "Europe/Paris", "2004-01-15T00:00:00Z", "zzzz", "Central European Standard Time", "+1:00"},
        { "en_HK", "Europe/Paris", "2004-07-15T00:00:00Z", "zzzz", "Central European Summer Time", "+2:00"},
        { "en_HK", "Europe/Paris", "2004-01-15T00:00:00Z", "z", "GMT+1", "+1:00"},
        { "en_HK", "Europe/Paris", "2004-07-15T00:00:00Z", "z", "GMT+2", "+2:00"},

        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
    };

    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<Calendar> cal(GregorianCalendar::createInstance(status));
    if (failure(status, "GregorianCalendar::createInstance", true)) return;
    SimpleDateFormat testfmt(UnicodeString("yyyy-MM-dd'T'HH:mm:ss'Z'"), status);
    if (failure(status, "SimpleDateFormat constructor", true)) return;
    testfmt.setTimeZone(*TimeZone::getGMT());

    for (int i = 0; fallbackTests[i][0]; i++) {
        const char **testLine = fallbackTests[i];
        UnicodeString info[5];
        for ( int j = 0 ; j < 5 ; j++ ) {
            info[j] = UnicodeString(testLine[j], -1, US_INV);
        }
        info[4] = info[4].unescape();
        logln("%s;%s;%s;%s", testLine[0], testLine[1], testLine[2], testLine[3]);

        TimeZone *tz = TimeZone::createTimeZone(info[1]);

        UDate d = testfmt.parse(testLine[2], status);
        cal->setTime(d, status);
        if (U_FAILURE(status)) {
            errln(UnicodeString("Failed to set date: ") + testLine[2]);
        }

        SimpleDateFormat fmt(info[3], Locale(testLine[0]),status);
        ASSERT_OK(status);
        cal->adoptTimeZone(tz);
        UnicodeString result;
        FieldPosition pos(FieldPosition::DONT_CARE);
        fmt.format(*cal.getAlias(), result,pos);
        if (result != info[4]) {
            errln(info[0] + ";" + info[1] + ";" + info[2] + ";" + info[3] + " expected: '" +
                  info[4] + "' but got: '" + result + "'");
        }
    }
}

void DateFormatTest::TestTimeZoneInLocale()
{
    const char *tests[][3]  = {
        { "en-u-tz-usden",                     "America/Denver",             "gregorian" },
        { "es-u-tz-usden",                     "America/Denver",             "gregorian" },
        { "ms-u-tz-mykul",                     "Asia/Kuala_Lumpur",          "gregorian" },
        { "zh-u-tz-mykul",                     "Asia/Kuala_Lumpur",          "gregorian" },
        { "fr-u-ca-buddhist-tz-phmnl",         "Asia/Manila",                "buddhist" },
        { "th-u-ca-chinese-tz-gblon",          "Europe/London",              "chinese" },
        { "de-u-ca-coptic-tz-ciabj",           "Africa/Abidjan",             "coptic" },
        { "ja-u-ca-dangi-tz-hkhkg",            "Asia/Hong_Kong",             "dangi" },
        { "da-u-ca-ethioaa-tz-ruunera",        "Asia/Ust-Nera",              "ethiopic-amete-alem" },
        { "ko-u-ca-ethiopic-tz-cvrai",         "Atlantic/Cape_Verde",        "ethiopic" },
        { "fil-u-ca-gregory-tz-aubne",         "Australia/Brisbane",         "gregorian" },
        { "fa-u-ca-hebrew-tz-brrbr",           "America/Rio_Branco",         "hebrew" },
        { "gr-u-ca-indian-tz-lccas",           "America/St_Lucia",           "indian" },
        { "or-u-ca-islamic-tz-cayyn",          "America/Swift_Current",      "islamic" },
        { "my-u-ca-islamic-umalqura-tz-kzala", "Asia/Almaty",                "islamic-umalqura" },
        { "lo-u-ca-islamic-tbla-tz-bmbda",     "Atlantic/Bermuda",           "islamic-tbla" },
        { "km-u-ca-islamic-civil-tz-aqplm",    "Antarctica/Palmer",          "islamic-civil" },
        { "kk-u-ca-islamic-rgsa-tz-usanc",     "America/Anchorage",          "islamic-rgsa" },
        { "ar-u-ca-iso8601-tz-bjptn",          "Africa/Porto-Novo",          "iso8601" },
        { "he-u-ca-japanese-tz-tzdar",         "Africa/Dar_es_Salaam",       "japanese" },
        { "bs-u-ca-persian-tz-etadd",          "Africa/Addis_Ababa",         "persian" },
        { "it-u-ca-roc-tz-aruaq",              "America/Argentina/San_Juan", "roc" },
    };

    for (int32_t i = 0; i < UPRV_LENGTHOF(tests); ++i) {
        UErrorCode status = U_ZERO_ERROR;
        const char **testLine = tests[i];
        Locale locale(testLine[0]);
        UnicodeString expectedTimezone(testLine[1], -1, US_INV);
        UnicodeString actual;

        SimpleDateFormat smptfmt("Z", locale, status);
        ASSERT_OK(status);
        assertEquals("TimeZone from SimpleDateFormat constructor",
                     expectedTimezone, smptfmt.getTimeZone().getID(actual));
        assertEquals("Calendar from SimpleDateFormat constructor",
                     testLine[2], smptfmt.getCalendar()->getType());

        LocalPointer<DateFormat> datefmt(
                DateFormat::createDateInstance(DateFormat::kDefault, locale));
        if (datefmt == nullptr) {
            dataerrln("Error calling DateFormat::createDateInstance()");
            return;
        }
        assertEquals("TimeZone from DateFormat::createDateInstance",
                     expectedTimezone, datefmt->getTimeZone().getID(actual));
        assertEquals("Calendar from DateFormat::createDateInstance",
                     testLine[2], datefmt->getCalendar()->getType());
        LocalPointer<DateFormat> timefmt(
                DateFormat::createTimeInstance(DateFormat::kDefault, locale));
        if (timefmt == nullptr) {
            dataerrln("Error calling DateFormat::createTimeInstance()");
            return;
        }
        assertEquals("TimeZone from TimeFormat::createTimeInstance",
                     expectedTimezone, timefmt->getTimeZone().getID(actual));
        assertEquals("Calendar from DateFormat::createTimeInstance",
                     testLine[2], timefmt->getCalendar()->getType());

        LocalPointer<DateFormat> datetimefmt(
                DateFormat::createDateTimeInstance(
                    DateFormat::kDefault, DateFormat::kDefault, locale));
        if (datetimefmt == nullptr) {
            dataerrln("Error calling DateFormat::createDateTimeInstance()");
            return;
        }
        assertEquals("TimeZone from DateTimeFormat::createDateTimeInstance",
                     expectedTimezone, datetimefmt->getTimeZone().getID(actual));
        assertEquals("Calendar from DateFormat::createDateTimeInstance",
                     testLine[2], datetimefmt->getCalendar()->getType());
    }
}

void DateFormatTest::TestRoundtripWithCalendar() {
    UErrorCode status = U_ZERO_ERROR;

    TimeZone *tz = TimeZone::createTimeZone("Europe/Paris");
    TimeZone *gmt = TimeZone::createTimeZone("Etc/GMT");

    Calendar *calendars[] = {
        Calendar::createInstance(*tz, Locale("und@calendar=gregorian"), status),
        Calendar::createInstance(*tz, Locale("und@calendar=buddhist"), status),
//        Calendar::createInstance(*tz, Locale("und@calendar=hebrew"), status),
        Calendar::createInstance(*tz, Locale("und@calendar=islamic"), status),
        Calendar::createInstance(*tz, Locale("und@calendar=japanese"), status),
        nullptr
    };
    if (U_FAILURE(status)) {
        dataerrln("Failed to initialize calendars: %s", u_errorName(status));
        for (int i = 0; calendars[i] != nullptr; i++) {
            delete calendars[i];
        }
        return;
    }

    //FIXME The formatters commented out below are currently failing because of
    // the calendar calculation problem reported by #6691

    // The order of test formatters must match the order of calendars above.
    DateFormat *formatters[] = {
        DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull, Locale("en_US")), //calendar=gregorian
        DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull, Locale("th_TH")), //calendar=buddhist
//        DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull, Locale("he_IL@calendar=hebrew")),
        DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull, Locale("ar_EG@calendar=islamic")),
//        DateFormat::createDateTimeInstance(DateFormat::kFull, DateFormat::kFull, Locale("ja_JP@calendar=japanese")),
        nullptr
    };

    UDate d = Calendar::getNow();
    UnicodeString buf;
    FieldPosition fpos;
    ParsePosition ppos;

    for (int i = 0; formatters[i] != nullptr; i++) {
        buf.remove();
        fpos.setBeginIndex(0);
        fpos.setEndIndex(0);
        calendars[i]->setTime(d, status);

        // Normal case output - the given calendar matches the calendar
        // used by the formatter
        formatters[i]->format(*calendars[i], buf, fpos);
        UnicodeString refStr(buf);

        for (int j = 0; calendars[j] != nullptr; j++) {
            if (j == i) {
                continue;
            }
            buf.remove();
            fpos.setBeginIndex(0);
            fpos.setEndIndex(0);
            calendars[j]->setTime(d, status);

            // Even the different calendar type is specified,
            // we should get the same result.
            formatters[i]->format(*calendars[j], buf, fpos);
            if (refStr != buf) {
                errln(UnicodeString("FAIL: Different format result with a different calendar for the same time -")
                        + "\n Reference calendar type=" + calendars[i]->getType()
                        + "\n Another calendar type=" + calendars[j]->getType()
                        + "\n Expected result=" + refStr
                        + "\n Actual result=" + buf);
            }
        }

        calendars[i]->setTimeZone(*gmt);
        calendars[i]->clear();
        ppos.setErrorIndex(-1);
        ppos.setIndex(0);

        // Normal case parse result - the given calendar matches the calendar
        // used by the formatter
        formatters[i]->parse(refStr, *calendars[i], ppos);

        for (int j = 0; calendars[j] != nullptr; j++) {
            if (j == i) {
                continue;
            }
            calendars[j]->setTimeZone(*gmt);
            calendars[j]->clear();
            ppos.setErrorIndex(-1);
            ppos.setIndex(0);

            // Even the different calendar type is specified,
            // we should get the same time and time zone.
            formatters[i]->parse(refStr, *calendars[j], ppos);
            if (calendars[i]->getTime(status) != calendars[j]->getTime(status)
                || calendars[i]->getTimeZone() != calendars[j]->getTimeZone()) {
                UnicodeString tzid;
                errln(UnicodeString("FAIL: Different parse result with a different calendar for the same string -")
                        + "\n Reference calendar type=" + calendars[i]->getType()
                        + "\n Another calendar type=" + calendars[j]->getType()
                        + "\n Date string=" + refStr
                        + "\n Expected time=" + calendars[i]->getTime(status)
                        + "\n Expected time zone=" + calendars[i]->getTimeZone().getID(tzid)
                        + "\n Actual time=" + calendars[j]->getTime(status)
                        + "\n Actual time zone=" + calendars[j]->getTimeZone().getID(tzid));
            }
        }
        if (U_FAILURE(status)) {
            errln(UnicodeString("FAIL: ") + u_errorName(status));
            break;
        }
    }

    delete tz;
    delete gmt;
    for (int i = 0; calendars[i] != nullptr; i++) {
        delete calendars[i];
    }
    for (int i = 0; formatters[i] != nullptr; i++) {
        delete formatters[i];
    }
}

/*
void DateFormatTest::TestRelativeError()
{
    UErrorCode status;
    Locale en("en");

    DateFormat *en_reltime_reldate =         DateFormat::createDateTimeInstance(DateFormat::kFullRelative,DateFormat::kFullRelative,en);
    if(en_reltime_reldate == nullptr) {
        logln("PASS: rel date/rel time failed");
    } else {
        errln("FAIL: rel date/rel time created, should have failed.");
        delete en_reltime_reldate;
    }
}

void DateFormatTest::TestRelativeOther()
{
    logln("Nothing in this test. When we get more data from CLDR, put in some tests of -2, +2, etc. ");
}
*/

void DateFormatTest::Test6338()
{
    UErrorCode status = U_ZERO_ERROR;

    SimpleDateFormat fmt1(UnicodeString(u"y-M-d"), Locale("ar"), status);
    if (failure(status, "new SimpleDateFormat", true)) return;

    UDate dt1 = date(2008-1900, UCAL_JUNE, 10, 12, 00);
    UnicodeString str1;
    str1 = fmt1.format(dt1, str1);
    logln(str1);

    UDate dt11 = fmt1.parse(str1, status);
    failure(status, "fmt->parse");

    UnicodeString str11;
    str11 = fmt1.format(dt11, str11);
    logln(str11);

    if (str1 != str11) {
        errln(UnicodeString("FAIL: Different dates str1:") + str1
            + " str2:" + str11);
    }

    /////////////////

    status = U_ZERO_ERROR;
    SimpleDateFormat fmt2(UnicodeString(u"y M d"), Locale("ar"), status);
    failure(status, "new SimpleDateFormat");

    UDate dt2 = date(2008-1900, UCAL_JUNE, 10, 12, 00);
    UnicodeString str2;
    str2 = fmt2.format(dt2, str2);
    logln(str2);

    UDate dt22 = fmt2.parse(str2, status);
    failure(status, "fmt->parse");

    UnicodeString str22;
    str22 = fmt2.format(dt22, str22);
    logln(str22);

    if (str2 != str22) {
        errln(UnicodeString("FAIL: Different dates str1:") + str2
            + " str2:" + str22);
    }

    /////////////////

    status = U_ZERO_ERROR;
    SimpleDateFormat fmt3(UnicodeString("y-M-d"), Locale("en-us"), status);
    failure(status, "new SimpleDateFormat");

    UDate dt3 = date(2008-1900, UCAL_JUNE, 10, 12, 00);
    UnicodeString str3;
    str3 = fmt3.format(dt3, str3);
    logln(str3);

    UDate dt33 = fmt3.parse(str3, status);
    failure(status, "fmt->parse");

    UnicodeString str33;
    str33 = fmt3.format(dt33, str33);
    logln(str33);

    if (str3 != str33) {
        errln(UnicodeString("FAIL: Different dates str1:") + str3
            + " str2:" + str33);
    }

    /////////////////

    status = U_ZERO_ERROR;
    SimpleDateFormat fmt4(UnicodeString("y M  d"), Locale("en-us"), status);
    failure(status, "new SimpleDateFormat");

    UDate dt4 = date(2008-1900, UCAL_JUNE, 10, 12, 00);
    UnicodeString str4;
    str4 = fmt4.format(dt4, str4);
    logln(str4);

    UDate dt44 = fmt4.parse(str4, status);
    failure(status, "fmt->parse");

    UnicodeString str44;
    str44 = fmt4.format(dt44, str44);
    logln(str44);

    if (str4 != str44) {
        errln(UnicodeString("FAIL: Different dates str1:") + str4
            + " str2:" + str44);
    }

}

void DateFormatTest::Test6726()
{
    // status
//    UErrorCode status = U_ZERO_ERROR;

    // fmtf, fmtl, fmtm, fmts;
    UnicodeString strf, strl, strm, strs;
    UDate dt = date(2008-1900, UCAL_JUNE, 10, 12, 00);

    Locale loc("ja");
    DateFormat* fmtf = DateFormat::createDateTimeInstance(DateFormat::FULL, DateFormat::FULL, loc);
    DateFormat* fmtl = DateFormat::createDateTimeInstance(DateFormat::LONG, DateFormat::FULL, loc);
    DateFormat* fmtm = DateFormat::createDateTimeInstance(DateFormat::MEDIUM, DateFormat::FULL, loc);
    DateFormat* fmts = DateFormat::createDateTimeInstance(DateFormat::SHORT, DateFormat::FULL, loc);
    if (fmtf == nullptr || fmtl == nullptr || fmtm == nullptr || fmts == nullptr) {
        dataerrln("Unable to create DateFormat. got nullptr.");
        /* It may not be true that if one is nullptr all is nullptr.  Just to be safe. */
        delete fmtf;
        delete fmtl;
        delete fmtm;
        delete fmts;

        return;
    }
    strf = fmtf->format(dt, strf);
    strl = fmtl->format(dt, strl);
    strm = fmtm->format(dt, strm);
    strs = fmts->format(dt, strs);


    logln("strm.charAt(10)=%04X wanted 0x20\n", strm.charAt(10));
    if (strm.charAt(10) != static_cast<char16_t>(0x0020)) {
      errln(UnicodeString("FAIL: Improper formatted date: ") + strm);
    }
    logln("strs.charAt(10)=%04X wanted 0x20\n", strs.charAt(8));
    if (strs.charAt(10) != static_cast<char16_t>(0x0020)) {
        errln(UnicodeString("FAIL: Improper formatted date: ") + strs);
    }

    delete fmtf;
    delete fmtl;
    delete fmtm;
    delete fmts;
}

/**
 * Test DateFormat's parsing of default GMT variants.  See ticket#6135
 */
void DateFormatTest::TestGMTParsing() {
    const char* DATA[] = {
        "HH:mm:ss Z",

        // pattern, input, expected output (in quotes)
        "HH:mm:ss Z",       "10:20:30 GMT+03:00",   "10:20:30 +0300",
        "HH:mm:ss Z",       "10:20:30 UT-02:00",    "10:20:30 -0200",
        "HH:mm:ss Z",       "10:20:30 GMT",         "10:20:30 +0000",
        "HH:mm:ss vvvv",    "10:20:30 UT+10:00",    "10:20:30 +1000",
        "HH:mm:ss zzzz",    "10:20:30 UTC",         "10:20:30 +0000",   // standalone "UTC"
        "ZZZZ HH:mm:ss",    "UT 10:20:30",          "10:20:30 +0000",
        "z HH:mm:ss",       "UT+0130 10:20:30",     "10:20:30 +0130",
        "z HH:mm:ss",       "UTC+0130 10:20:30",    "10:20:30 +0130",
        // Note: GMT-1100 no longer works because of the introduction of the short
        // localized GMT support. Previous implementation support this level of
        // leniency (no separator char in localized GMT format), but the new
        // implementation handles GMT-11 as the legitimate short localized GMT format
        // and stop at there. Otherwise, roundtrip would be broken.
        //"HH mm Z ss",       "10 20 GMT-1100 30",    "10:20:30 -1100",
        "HH mm Z ss",       "10 20 GMT-11 30",    "10:20:30 -1100",
        "HH:mm:ssZZZZZ",    "14:25:45Z",            "14:25:45 +0000",
        "HH:mm:ssZZZZZ",    "15:00:00-08:00",       "15:00:00 -0800",
    };
    const int32_t DATA_len = UPRV_LENGTHOF(DATA);
    expectParse(DATA, DATA_len, Locale("en"));
}

// Test case for localized GMT format parsing
// with no delimitters in offset format (Chinese locale)
void DateFormatTest::Test6880() {
    UErrorCode status = U_ZERO_ERROR;
    UDate d1, d2, dp1, dp2, dexp1, dexp2;
    UnicodeString s1, s2;

    TimeZone *tz = TimeZone::createTimeZone("Asia/Shanghai");
    GregorianCalendar gcal(*tz, status);
    if (failure(status, "construct GregorianCalendar", true)) return;

    gcal.clear();
    gcal.set(1900, UCAL_JULY, 1, 12, 00);   // offset 8:05:43
    d1 = gcal.getTime(status);

    gcal.clear();
    gcal.set(1950, UCAL_JULY, 1, 12, 00);   // offset 8:00
    d2 = gcal.getTime(status);

    gcal.clear();
    gcal.set(1970, UCAL_JANUARY, 1, 12, 00);
    dexp2 = gcal.getTime(status);
    dexp1 = dexp2 - (5*60 + 43)*1000;   // subtract 5m43s

    if (U_FAILURE(status)) {
        errln("FAIL: Gregorian calendar error");
    }

    DateFormat *fmt = DateFormat::createTimeInstance(DateFormat::kFull, Locale("zh"));
    if (fmt == nullptr) {
        dataerrln("Unable to create DateFormat. Got nullptr.");
        return;
    }
    fmt->adoptTimeZone(tz);

    fmt->format(d1, s1);
    fmt->format(d2, s2);

    dp1 = fmt->parse(s1, status);
    dp2 = fmt->parse(s2, status);

    if (U_FAILURE(status)) {
        errln("FAIL: Parse failure");
    }

    if (dp1 != dexp1) {
        errln("FAIL: Failed to parse " + s1 + " parsed: " + dp1 + " expected: " + dexp1);
    }
    if (dp2 != dexp2) {
        errln("FAIL: Failed to parse " + s2 + " parsed: " + dp2 + " expected: " + dexp2);
    }

    delete fmt;
}

typedef struct {
    const char * localeStr;
    UBool        lenient;
    UBool        expectFail;
    UnicodeString datePattern;
    UnicodeString dateString;
} NumAsStringItem;

void DateFormatTest::TestNumberAsStringParsing()
{
    const NumAsStringItem items[] = {
        // loc lenient fail?  datePattern                                         dateString
        { "",   false, true,  UnicodeString("y MMMM d HH:mm:ss"),                 UnicodeString("2009 7 14 08:43:57") },
        { "",   true,  false, UnicodeString("y MMMM d HH:mm:ss"),                 UnicodeString("2009 7 14 08:43:57") },
        { "en", false, false, UnicodeString("MMM d, y"),                          UnicodeString("Jul 14, 2009") },
        { "en", true,  false, UnicodeString("MMM d, y"),                          UnicodeString("Jul 14, 2009") },
        { "en", false, true,  UnicodeString("MMM d, y"),                          UnicodeString("7 14, 2009") },
        { "en", true,  false, UnicodeString("MMM d, y"),                          UnicodeString("7 14, 2009") },
        { "ja", false, false, UnicodeString("yyyy/MM/dd"),                        UnicodeString("2009/07/14")         },
        { "ja", true,  false, UnicodeString("yyyy/MM/dd"),                        UnicodeString("2009/07/14")         },
      //{ "ja", false, false, UnicodeString("yyyy/MMMMM/d"),                      UnicodeString("2009/7/14")          }, // #8860 covers test failure
        { "ja", true,  false, UnicodeString("yyyy/MMMMM/d"),                      UnicodeString("2009/7/14")          },
        { "ja", false, false, CharsToUnicodeString("y\\u5E74M\\u6708d\\u65E5"),   CharsToUnicodeString("2009\\u5E747\\u670814\\u65E5")   },
        { "ja", true,  false, CharsToUnicodeString("y\\u5E74M\\u6708d\\u65E5"),   CharsToUnicodeString("2009\\u5E747\\u670814\\u65E5")   },
        { "ja", false, false, CharsToUnicodeString("y\\u5E74MMMd\\u65E5"),        CharsToUnicodeString("2009\\u5E747\\u670814\\u65E5")   },
        { "ja", true,  false, CharsToUnicodeString("y\\u5E74MMMd\\u65E5"),        CharsToUnicodeString("2009\\u5E747\\u670814\\u65E5")   }, // #8820 fixes test failure
        { "ko", false, false, UnicodeString("yyyy. M. d."),                       UnicodeString("2009. 7. 14.")       },
        { "ko", true,  false, UnicodeString("yyyy. M. d."),                       UnicodeString("2009. 7. 14.")       },
        { "ko", false, false, UnicodeString("yyyy. MMMMM d."),                    CharsToUnicodeString("2009. 7\\uC6D4 14.")             },
        { "ko", true,  false, UnicodeString("yyyy. MMMMM d."),                    CharsToUnicodeString("2009. 7\\uC6D4 14.")             }, // #8820 fixes test failure
        { "ko", false, false, CharsToUnicodeString("y\\uB144 M\\uC6D4 d\\uC77C"), CharsToUnicodeString("2009\\uB144 7\\uC6D4 14\\uC77C") },
        { "ko", true,  false, CharsToUnicodeString("y\\uB144 M\\uC6D4 d\\uC77C"), CharsToUnicodeString("2009\\uB144 7\\uC6D4 14\\uC77C") },
        { "ko", false, false, CharsToUnicodeString("y\\uB144 MMM d\\uC77C"),      CharsToUnicodeString("2009\\uB144 7\\uC6D4 14\\uC77C") },
        { "ko", true,  false, CharsToUnicodeString("y\\uB144 MMM d\\uC77C"),      CharsToUnicodeString("2009\\uB144 7\\uC6D4 14\\uC77C") }, // #8820 fixes test failure
        { nullptr, false, false, UnicodeString(""),                                  UnicodeString("")                   }
    };
    const NumAsStringItem * itemPtr;
    for (itemPtr = items; itemPtr->localeStr != nullptr; itemPtr++ ) {
        Locale locale = Locale::createFromName(itemPtr->localeStr);
        UErrorCode status = U_ZERO_ERROR;
        SimpleDateFormat formatter(itemPtr->datePattern, locale, status);
        if (U_FAILURE(status)) {
            dataerrln("Unable to create SimpleDateFormat - %s", u_errorName(status));
            return;
        }

        formatter.setLenient(itemPtr->lenient);
        formatter.setBooleanAttribute(UDAT_PARSE_ALLOW_WHITESPACE, itemPtr->lenient, status).setBooleanAttribute(UDAT_PARSE_ALLOW_NUMERIC, itemPtr->lenient, status);
        UDate date1 = formatter.parse(itemPtr->dateString, status);
        if (U_FAILURE(status)) {
            if (!itemPtr->expectFail) {
                errln("FAIL, err when expected success: Locale \"" + UnicodeString(itemPtr->localeStr) + "\", lenient " + itemPtr->lenient +
                        ": using pattern \"" + itemPtr->datePattern + "\", could not parse \"" + itemPtr->dateString + "\"; err: " + u_errorName(status) );
            }
        } else if (itemPtr->expectFail) {
                errln("FAIL, expected err but got none: Locale \"" + UnicodeString(itemPtr->localeStr) + "\", lenient " + itemPtr->lenient +
                        ": using pattern \"" + itemPtr->datePattern + "\", did parse \"" + itemPtr->dateString + "\"." );
        } else if (!itemPtr->lenient) {
            UnicodeString formatted;
            formatter.format(date1, formatted);
            if (formatted != itemPtr->dateString) {
                errln("FAIL, mismatch formatting parsed date: Locale \"" + UnicodeString(itemPtr->localeStr) + "\", lenient " + itemPtr->lenient +
                        ": using pattern \"" + itemPtr->datePattern + "\", did parse \"" + itemPtr->dateString + "\", formatted result \"" + formatted + "\".");
            }
        }
    }
}

void DateFormatTest::TestISOEra() {

    const char* data[] = {
    // input, output
    "BC 4004-10-23T07:00:00Z", "BC 4004-10-23T07:00:00Z",
    "AD 4004-10-23T07:00:00Z", "AD 4004-10-23T07:00:00Z",
    "-4004-10-23T07:00:00Z"  , "BC 4005-10-23T07:00:00Z",
    "4004-10-23T07:00:00Z"   , "AD 4004-10-23T07:00:00Z",
    };

    int32_t numData = 8;

    UErrorCode status = U_ZERO_ERROR;

    // create formatter
    SimpleDateFormat *fmt1 = new SimpleDateFormat(UnicodeString("GGG yyyy-MM-dd'T'HH:mm:ss'Z"), status);
    failure(status, "new SimpleDateFormat", true);
    if (status == U_MISSING_RESOURCE_ERROR) {
        delete fmt1;
        return;
    }
    for(int i=0; i < numData; i+=2) {
        // create input string
        UnicodeString in = data[i];

        // parse string to date
        UDate dt1 = fmt1->parse(in, status);
        failure(status, "fmt->parse", true);

        // format date back to string
        UnicodeString out;
        out = fmt1->format(dt1, out);
        logln(out);

        // check that roundtrip worked as expected
        UnicodeString expected = data[i+1];
        if (out != expected) {
            dataerrln(UnicodeString("FAIL: ") + in + " -> " + out + " expected -> " + expected);
        }
    }

    delete fmt1;
}
void DateFormatTest::TestFormalChineseDate() {

    UErrorCode status = U_ZERO_ERROR;
    UnicodeString pattern(u"y\u5e74M\u6708d\u65e5");
    UnicodeString numsys_override(u"y=hanidec;M=hans;d=hans");

    // create formatter
    SimpleDateFormat sdf(pattern, numsys_override, Locale::getChina(),status);
    if (failure(status, "new SimpleDateFormat with override", true)) {
        return;
    }

    UDate thedate = date(2009-1900, UCAL_JULY, 28);
    FieldPosition pos(FieldPosition::DONT_CARE);
    UnicodeString result;
    sdf.format(thedate,result,pos);

    UnicodeString expected = "\\u4e8c\\u3007\\u3007\\u4e5d\\u5e74\\u4e03\\u6708\\u4e8c\\u5341\\u516b\\u65e5";
    expected = expected.unescape();
    if (result != expected) {
        dataerrln(UnicodeString("FAIL: -> ") + result + " expected -> " + expected);
    }

    UDate parsedate = sdf.parse(expected,status);
    if ( parsedate != thedate ) {
        UnicodeString pat1 ("yyyy-MM-dd'T'HH:mm:ss'Z'", -1, US_INV );
        SimpleDateFormat usf(pat1, Locale::getEnglish(), status);
        UnicodeString parsedres,expres;
        usf.format(parsedate,parsedres,pos);
        usf.format(thedate,expres,pos);
        dataerrln(UnicodeString("FAIL: parsed -> ") + parsedres + " expected -> " + expres);
    }
}

// Test case for #8675
// Incorrect parse offset with stand alone GMT string on 2nd or later iteration.
void DateFormatTest::TestStandAloneGMTParse() {
    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat sdf("ZZZZ", Locale(""), status);

    if (U_SUCCESS(status)) {

        UnicodeString inText(u"GMT$$$");
        for (int32_t i = 0; i < 10; i++) {
            ParsePosition pos(0);
            sdf.parse(inText, pos);
            if (pos.getIndex() != 3) {
                errln(UnicodeString("FAIL: Incorrect output parse position: actual=") + pos.getIndex() + " expected=3");
            }
        }

    } else {
        dataerrln("Unable to create SimpleDateFormat - %s", u_errorName(status));
    }
}

void DateFormatTest::TestParsePosition() {
    const char* TestData[][4] = {
        // {<pattern>, <lead>, <date string>, <trail>}
        {"yyyy-MM-dd HH:mm:ssZ", "", "2010-01-10 12:30:00+0500", ""},
        {"yyyy-MM-dd HH:mm:ss ZZZZ", "", "2010-01-10 12:30:00 GMT+05:00", ""},
        {"Z HH:mm:ss", "", "-0100 13:20:30", ""},
        {"y-M-d Z", "", "2011-8-25 -0400", " Foo"},
        {"y/M/d H:mm:ss z", "", "2011/7/1 12:34:00 PDT", ""},
        {"y/M/d H:mm:ss z", "+123", "2011/7/1 12:34:00 PDT", " PST"},
        {"vvvv a h:mm:ss", "", "Pacific Time AM 10:21:45", ""},
        {"HH:mm v M/d", "111", "14:15 PT 8/10", " 12345"},
        {"'time zone:' VVVV 'date:' yyyy-MM-dd", "xxxx", "time zone: Los Angeles Time date: 2010-02-25", "xxxx"},
        {"yG", "", "2012AD", ""},
        {"yG", "", "2012", "x"},
        {nullptr, nullptr, nullptr, nullptr},
    };

    for (int32_t i = 0; TestData[i][0]; i++) {
        UErrorCode status = U_ZERO_ERROR;
        SimpleDateFormat sdf(UnicodeString(TestData[i][0]), status);
        if (failure(status, "new SimpleDateFormat", true)) return;

        int32_t startPos, resPos;

        // lead text
        UnicodeString input(TestData[i][1]);
        startPos = input.length();

        // date string
        input += TestData[i][2];
        resPos = input.length();

        // trail text
        input += TestData[i][3];

        ParsePosition pos(startPos);
        //UDate d = sdf->parse(input, pos);
        (void)sdf.parse(input, pos);

        if (pos.getIndex() != resPos) {
            errln(UnicodeString("FAIL: Parsing [") + input + "] with pattern [" + TestData[i][0] + "] returns position - "
                + pos.getIndex() + ", expected - " + resPos);
        }
    }
}


typedef struct {
    int32_t era;
    int32_t year;
    int32_t month; // 1-based
    int32_t isLeapMonth;
    int32_t day;
} ChineseCalTestDate;

#define NUM_TEST_DATES 3

typedef struct {
    const char *   locale;
    int32_t        style; // <0 => custom
    UnicodeString  dateString[NUM_TEST_DATES];
} MonthPatternItem;

void DateFormatTest::TestMonthPatterns()
{
    const ChineseCalTestDate dates[NUM_TEST_DATES] = {
        // era yr mo lp da
        {  78, 29, 4, 0, 2 }, // (in chinese era 78) gregorian 2012-4-22
        {  78, 29, 4, 1, 2 }, // (in chinese era 78) gregorian 2012-5-22
        {  78, 29, 5, 0, 2 }, // (in chinese era 78) gregorian 2012-6-20
    };

    const MonthPatternItem items[] = {
        // locale                     date style;           expected formats for the 3 dates above
        { "root@calendar=chinese",    DateFormat::kLong,  { UnicodeString("2012(ren-chen) M04 2"),  UnicodeString("2012(ren-chen) M04bis 2"),  UnicodeString("2012(ren-chen) M05 2") } },
        { "root@calendar=chinese",    DateFormat::kShort, { UnicodeString("2012-04-02"),    UnicodeString("2012-04bis-02"),         UnicodeString("2012-05-02") } },
        { "root@calendar=chinese",    -1,                 { UnicodeString("29-4-2"),        UnicodeString("29-4bis-2"),             UnicodeString("29-5-2") } },
        { "root@calendar=chinese",    -2,                 { UnicodeString("78x29-4-2"),     UnicodeString("78x29-4bis-2"),          UnicodeString("78x29-5-2") } },
        { "root@calendar=chinese",    -3,                 { UnicodeString("ren-chen-4-2"),  UnicodeString("ren-chen-4bis-2"),       UnicodeString("ren-chen-5-2") } },
        { "root@calendar=chinese",    -4,                 { UnicodeString("ren-chen M04 2"),  UnicodeString("ren-chen M04bis 2"),   UnicodeString("ren-chen M05 2") } },
        { "en@calendar=gregorian",    -3,                 { UnicodeString("2012-4-22"),     UnicodeString("2012-5-22"),             UnicodeString("2012-6-20") } },
        { "en@calendar=chinese",      DateFormat::kLong,  { UnicodeString("Fourth Month 2, 2012(ren-chen)"), UnicodeString("Fourth Monthbis 2, 2012(ren-chen)"), UnicodeString("Fifth Month 2, 2012(ren-chen)") } },
        { "en@calendar=chinese",      DateFormat::kShort, { UnicodeString("4/2/2012"),      UnicodeString("4bis/2/2012"),           UnicodeString("5/2/2012") } },
        { "zh@calendar=chinese",      DateFormat::kLong,  { CharsToUnicodeString("2012\\u58EC\\u8FB0\\u5E74\\u56DB\\u6708\\u521D\\u4E8C"),
                                                            CharsToUnicodeString("2012\\u58EC\\u8FB0\\u5E74\\u95F0\\u56DB\\u6708\\u521D\\u4E8C"),
                                                            CharsToUnicodeString("2012\\u58EC\\u8FB0\\u5E74\\u4E94\\u6708\\u521D\\u4E8C") } },
#if APPLE_ICU_CHANGES
// rdar://
        { "zh@calendar=chinese",      DateFormat::kShort, { CharsToUnicodeString("2012-4-2"),
                                                            CharsToUnicodeString("2012-\\u95F04-2"),
                                                            CharsToUnicodeString("2012-5-2") } },
#else
        { "zh@calendar=chinese",      DateFormat::kShort, { CharsToUnicodeString("2012/4/2"),
                                                            CharsToUnicodeString("2012/\\u95F04/2"),
                                                            CharsToUnicodeString("2012/5/2") } },
#endif  // APPLE_ICU_CHANGES
        { "zh@calendar=chinese",      -3,                 { CharsToUnicodeString("\\u58EC\\u8FB0-4-2"),
                                                            CharsToUnicodeString("\\u58EC\\u8FB0-\\u95F04-2"),
                                                            CharsToUnicodeString("\\u58EC\\u8FB0-5-2") } },
        { "zh@calendar=chinese",      -4,                 { CharsToUnicodeString("\\u58EC\\u8FB0 \\u56DB\\u6708 2"),
                                                            CharsToUnicodeString("\\u58EC\\u8FB0 \\u95F0\\u56DB\\u6708 2"),
                                                            CharsToUnicodeString("\\u58EC\\u8FB0 \\u4E94\\u6708 2") } },
        { "zh_Hant@calendar=chinese", DateFormat::kLong,  { CharsToUnicodeString("2012\\u58EC\\u8FB0\\u5E74\\u56DB\\u6708\\u521D\\u4E8C"),
                                                            CharsToUnicodeString("2012\\u58EC\\u8FB0\\u5E74\\u958F\\u56DB\\u6708\\u521D\\u4E8C"),
                                                            CharsToUnicodeString("2012\\u58EC\\u8FB0\\u5E74\\u4E94\\u6708\\u521D\\u4E8C") } },
        { "zh_Hant@calendar=chinese", DateFormat::kShort, { CharsToUnicodeString("2012/4/2"),
                                                            CharsToUnicodeString("2012/\\u958F4/2"),
                                                            CharsToUnicodeString("2012/5/2") } },
        { "fr@calendar=chinese",      DateFormat::kLong,  { CharsToUnicodeString("2 s\\u00ECyu\\u00E8 ren-chen"),
                                                            CharsToUnicodeString("2 s\\u00ECyu\\u00E8bis ren-chen"),
                                                            CharsToUnicodeString("2 w\\u01D4yu\\u00E8 ren-chen") } },
        { "fr@calendar=chinese",      DateFormat::kShort, { UnicodeString("2/4/29"),        UnicodeString("2/4bis/29"),             UnicodeString("2/5/29") } },
        { "en@calendar=dangi",        DateFormat::kLong,  { UnicodeString("Third Monthbis 2, 2012(ren-chen)"),  UnicodeString("Fourth Month 2, 2012(ren-chen)"),       UnicodeString("Fifth Month 1, 2012(ren-chen)") } },
        { "en@calendar=dangi",        DateFormat::kShort, { UnicodeString("3bis/2/2012"),   UnicodeString("4/2/2012"),              UnicodeString("5/1/2012") } },
        { "en@calendar=dangi",        -2,                 { UnicodeString("78x29-3bis-2"),  UnicodeString("78x29-4-2"),             UnicodeString("78x29-5-1") } },
        { "ko@calendar=dangi",        DateFormat::kLong,  { CharsToUnicodeString("\\uC784\\uC9C4\\uB144 \\uC7243\\uC6D4 2\\uC77C"),
                                                            CharsToUnicodeString("\\uC784\\uC9C4\\uB144 4\\uC6D4 2\\uC77C"),
                                                            CharsToUnicodeString("\\uC784\\uC9C4\\uB144 5\\uC6D4 1\\uC77C") } },
        { "ko@calendar=dangi",        DateFormat::kShort, { CharsToUnicodeString("29. \\uC7243. 2."),
                                                            CharsToUnicodeString("29. 4. 2."),
                                                            CharsToUnicodeString("29. 5. 1.") } },
        // terminator
        { nullptr,                       0,                  { UnicodeString(""), UnicodeString(""), UnicodeString("") } }
    };

    //.                               style: -1        -2            -3       -4
    const UnicodeString customPatterns[] = { "y-Ml-d", "G'x'y-Ml-d", "U-M-d", "U MMM d" }; // like old root pattern, using 'l'

    UErrorCode status = U_ZERO_ERROR;
    Locale rootChineseCalLocale = Locale::createFromName("root@calendar=chinese");
    Calendar * rootChineseCalendar = Calendar::createInstance(rootChineseCalLocale, status);
    if (U_SUCCESS(status)) {
        const MonthPatternItem * itemPtr;
        for (itemPtr = items; itemPtr->locale != nullptr; itemPtr++ ) {
            Locale locale = Locale::createFromName(itemPtr->locale);
            DateFormat * dmft = (itemPtr->style >= 0)?
                    DateFormat::createDateInstance(static_cast<DateFormat::EStyle>(itemPtr->style), locale):
                    new SimpleDateFormat(customPatterns[-itemPtr->style - 1], locale, status);
            if ( dmft != nullptr ) {
                if (U_SUCCESS(status)) {
                    const ChineseCalTestDate * datePtr = dates;
                    int32_t idate;
                    for (idate = 0; idate < NUM_TEST_DATES; idate++, datePtr++) {
                        rootChineseCalendar->clear();
                        rootChineseCalendar->set(UCAL_ERA, datePtr->era);
                        rootChineseCalendar->set(datePtr->year, datePtr->month-1, datePtr->day);
                        rootChineseCalendar->set(UCAL_IS_LEAP_MONTH, datePtr->isLeapMonth);
                        UnicodeString result;
                        FieldPosition fpos(FieldPosition::DONT_CARE);
                        dmft->format(*rootChineseCalendar, result, fpos);
                        if ( result.compare(itemPtr->dateString[idate]) != 0 ) {
                            errln( UnicodeString("FAIL: Chinese calendar format for locale ") + UnicodeString(itemPtr->locale) + ", style " + itemPtr->style +
                                    ", expected \"" + itemPtr->dateString[idate] + "\", got \"" + result + "\"");
                        } else {
                            // formatted OK, try parse
                            ParsePosition ppos(0);
                            // ensure we are really parsing the fields we should be
                            rootChineseCalendar->set(UCAL_YEAR, 1);
                            rootChineseCalendar->set(UCAL_MONTH, 0);
                            rootChineseCalendar->set(UCAL_IS_LEAP_MONTH, 0);
                            rootChineseCalendar->set(UCAL_DATE, 1);
                            //
                            dmft->parse(result, *rootChineseCalendar, ppos);
                            int32_t year = rootChineseCalendar->get(UCAL_YEAR, status);
                            int32_t month = rootChineseCalendar->get(UCAL_MONTH, status) + 1;
                            int32_t isLeapMonth = rootChineseCalendar->get(UCAL_IS_LEAP_MONTH, status);
                            int32_t day = rootChineseCalendar->get(UCAL_DATE, status);
                            if ( ppos.getIndex() < result.length() || year != datePtr->year || month != datePtr->month || isLeapMonth != datePtr->isLeapMonth || day != datePtr->day ) {
                                errln( UnicodeString("FAIL: Chinese calendar parse for locale ") + UnicodeString(itemPtr->locale) + ", style " + itemPtr->style +
                                    ", string \"" + result + "\", expected " + datePtr->year +"-"+datePtr->month+"("+datePtr->isLeapMonth+")-"+datePtr->day + ", got pos " +
                                    ppos.getIndex() + " " + year +"-"+month+"("+isLeapMonth+")-"+day);
                            }
                        }
                    }
                } else {
                    dataerrln("Error creating SimpleDateFormat for Chinese calendar- %s", u_errorName(status));
                }
                delete dmft;
            } else {
                dataerrln("FAIL: Unable to create DateFormat for Chinese calendar- %s", u_errorName(status));
            }
        }
        delete rootChineseCalendar;
    } else {
        errln(UnicodeString("FAIL: Unable to create Calendar for root@calendar=chinese"));
    }
}

typedef struct {
    const char * locale;
    UnicodeString pattern;
    UDisplayContext capitalizationContext;
    UnicodeString expectedFormat;
} TestContextItem;

void DateFormatTest::TestContext()
{
    const UDate july022008 = 1215000001979.0;
    const TestContextItem items[] = {
        //locale              pattern    capitalizationContext                              expected formatted date
        { "fr", UnicodeString("MMMM y"), UDISPCTX_CAPITALIZATION_NONE,                      UnicodeString("juillet 2008") },
#if !UCONFIG_NO_BREAK_ITERATION
        { "fr", UnicodeString("MMMM y"), UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE,    UnicodeString("juillet 2008") },
        { "fr", UnicodeString("MMMM y"), UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE, UnicodeString("Juillet 2008") },
        { "fr", UnicodeString("MMMM y"), UDISPCTX_CAPITALIZATION_FOR_UI_LIST_OR_MENU,       UnicodeString("juillet 2008") },
        { "fr", UnicodeString("MMMM y"), UDISPCTX_CAPITALIZATION_FOR_STANDALONE,            UnicodeString("Juillet 2008") },
#endif
        { "cs", UnicodeString("LLLL y"), UDISPCTX_CAPITALIZATION_NONE,                      CharsToUnicodeString("\\u010Dervenec 2008") },
#if !UCONFIG_NO_BREAK_ITERATION
        { "cs", UnicodeString("LLLL y"), UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE,    CharsToUnicodeString("\\u010Dervenec 2008") },
        { "cs", UnicodeString("LLLL y"), UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE, CharsToUnicodeString("\\u010Cervenec 2008") },
        { "cs", UnicodeString("LLLL y"), UDISPCTX_CAPITALIZATION_FOR_UI_LIST_OR_MENU,       CharsToUnicodeString("\\u010Cervenec 2008") },
        { "cs", UnicodeString("LLLL y"), UDISPCTX_CAPITALIZATION_FOR_STANDALONE,            CharsToUnicodeString("\\u010Dervenec 2008") },
#endif
        // terminator
        { nullptr, UnicodeString(""), static_cast<UDisplayContext>(0), UnicodeString("") }
    };
    UErrorCode status = U_ZERO_ERROR;
    Calendar* cal = Calendar::createInstance(status);
    if (U_FAILURE(status)) {
        dataerrln(UnicodeString("FAIL: Unable to create Calendar for default timezone and locale."));
    } else {
        cal->setTime(july022008, status);
        const TestContextItem * itemPtr;
        for (itemPtr = items; itemPtr->locale != nullptr; itemPtr++ ) {
           Locale locale = Locale::createFromName(itemPtr->locale);
           status = U_ZERO_ERROR;
           SimpleDateFormat * sdmft = new SimpleDateFormat(itemPtr->pattern, locale, status);
           if (U_FAILURE(status)) {
                dataerrln(UnicodeString("FAIL: Unable to create SimpleDateFormat for specified pattern with locale ") + UnicodeString(itemPtr->locale));
           } else {
               sdmft->setContext(itemPtr->capitalizationContext, status);
               UnicodeString result;
               FieldPosition pos(FieldPosition::DONT_CARE);
               sdmft->format(*cal, result, pos);
               if (result.compare(itemPtr->expectedFormat) != 0) {
                   errln(UnicodeString("FAIL: format for locale ") + UnicodeString(itemPtr->locale) +
                           ", status " + static_cast<int>(status) +
                           ", capitalizationContext " + static_cast<int>(itemPtr->capitalizationContext) +
                           ", expected " + itemPtr->expectedFormat + ", got " + result);
               }
           }
           delete sdmft;
        }
    }
    delete cal;
}

// test item for a particular locale + calendar and date format
typedef struct {
    int32_t era;
    int32_t year;
    int32_t month;
    int32_t day;
    int32_t hour;
    int32_t minute;
    UnicodeString formattedDate;
} CalAndFmtTestItem;

// test item giving locale + calendar, date format, and CalAndFmtTestItems
typedef struct {
    const char * locale; // with calendar
    DateFormat::EStyle style;
    UnicodeString pattern; // ignored unless style == DateFormat::kNone
    const CalAndFmtTestItem *caftItems;
} TestNonGregoItem;

void DateFormatTest::TestNonGregoFmtParse()
{
    // test items for he@calendar=hebrew, long date format
    const CalAndFmtTestItem cafti_he_hebrew_long[] = {
        {  0, 4999, 12, 29, 12, 0, CharsToUnicodeString("\\u05DB\\u05F4\\u05D8 \\u05D1\\u05D0\\u05DC\\u05D5\\u05DC \\u05D3\\u05F3\\u05EA\\u05EA\\u05E7\\u05E6\\u05F4\\u05D8") },
        {  0, 5100,  0,  1, 12, 0, CharsToUnicodeString("\\u05D0\\u05F3 \\u05D1\\u05EA\\u05E9\\u05E8\\u05D9 \\u05E7\\u05F3") },
        {  0, 5774,  5,  1, 12, 0, CharsToUnicodeString("\\u05D0\\u05F3 \\u05D1\\u05D0\\u05D3\\u05E8 \\u05D0\\u05F3 \\u05EA\\u05E9\\u05E2\\u05F4\\u05D3") },
        {  0, 5999, 12, 29, 12, 0, CharsToUnicodeString("\\u05DB\\u05F4\\u05D8 \\u05D1\\u05D0\\u05DC\\u05D5\\u05DC \\u05EA\\u05EA\\u05E7\\u05E6\\u05F4\\u05D8") },
        {  0, 6100,  0,  1, 12, 0, CharsToUnicodeString("\\u05D0\\u05F3 \\u05D1\\u05EA\\u05E9\\u05E8\\u05D9 \\u05D5\\u05F3\\u05E7\\u05F3") },
        {  0,    0,  0,  0,  0, 0, UnicodeString("") } // terminator
    };
    const CalAndFmtTestItem cafti_zh_chinese_custU[] = {
        { 78,   31,  0,  1, 12, 0, CharsToUnicodeString("2014\\u7532\\u5348\\u5E74\\u6B63\\u67081") },
        { 77,   31,  0,  1, 12, 0, CharsToUnicodeString("1954\\u7532\\u5348\\u5E74\\u6B63\\u67081") },
        {  0,    0,  0,  0,  0, 0, UnicodeString("") } // terminator
    };
    const CalAndFmtTestItem cafti_zh_chinese_custNoU[] = {
        { 78,   31,  0,  1, 12, 0, CharsToUnicodeString("2014\\u5E74\\u6B63\\u67081") },
        { 77,   31,  0,  1, 12, 0, CharsToUnicodeString("1954\\u5E74\\u6B63\\u67081") },
        {  0,    0,  0,  0,  0, 0, UnicodeString("") } // terminator
    };
    const CalAndFmtTestItem cafti_ja_japanese_custGy[] = {
        {235,   26,  2,  5, 12, 0, CharsToUnicodeString("2014(\\u5E73\\u621026)\\u5E743\\u67085\\u65E5") },
        {234,   60,  2,  5, 12, 0, CharsToUnicodeString("1985(\\u662D\\u548C60)\\u5E743\\u67085\\u65E5") },
        {  0,    0,  0,  0,  0, 0, UnicodeString("") } // terminator
    };
    const CalAndFmtTestItem cafti_ja_japanese_custNoGy[] = {
        {235,   26,  2,  5, 12, 0, CharsToUnicodeString("2014\\u5E743\\u67085\\u65E5") },
        {234,   60,  2,  5, 12, 0, CharsToUnicodeString("1985\\u5E743\\u67085\\u65E5") },
        {  0,    0,  0,  0,  0, 0, UnicodeString("") } // terminator
    };
    const CalAndFmtTestItem cafti_en_islamic_cust[] = {
#if APPLE_ICU_CHANGES
// rdar://77151664 #233 better root abbrev months for Islamic calendar
        {  0, 1384,  0,  1, 12, 0, UnicodeString("1 Mhrm. 1384 AH, 1964") },
        {  0, 1436,  0,  1, 12, 0, UnicodeString("1 Mhrm. 1436 AH, 2014") },
        {  0, 1487,  0,  1, 12, 0, UnicodeString("1 Mhrm. 1487 AH, 2064") },
#else
        {  0, 1384,  0,  1, 12, 0, UnicodeString("1 Muh. 1384 AH, 1964") },
        {  0, 1436,  0,  1, 12, 0, UnicodeString("1 Muh. 1436 AH, 2014") },
        {  0, 1487,  0,  1, 12, 0, UnicodeString("1 Muh. 1487 AH, 2064") },
#endif  // APPLE_ICU_CHANGES
        {  0,    0,  0,  0,  0, 0, UnicodeString("") } // terminator
    };
    // overal test items
    const TestNonGregoItem items[] = {
        { "he@calendar=hebrew",   DateFormat::kLong, UnicodeString(""),                 cafti_he_hebrew_long },
        { "zh@calendar=chinese",  DateFormat::kNone, CharsToUnicodeString("rU\\u5E74MMMd"),                cafti_zh_chinese_custU },
        { "zh@calendar=chinese",  DateFormat::kNone, CharsToUnicodeString("r\\u5E74MMMd"),                 cafti_zh_chinese_custNoU },
        { "ja@calendar=japanese", DateFormat::kNone, CharsToUnicodeString("r(Gy)\\u5E74M\\u6708d\\u65E5"), cafti_ja_japanese_custGy },
        { "ja@calendar=japanese", DateFormat::kNone, CharsToUnicodeString("r\\u5E74M\\u6708d\\u65E5"),     cafti_ja_japanese_custNoGy },
        { "en@calendar=islamic",  DateFormat::kNone, UnicodeString("d MMM y G, r"),     cafti_en_islamic_cust },
        { nullptr, DateFormat::kNone, UnicodeString(""), nullptr } // terminator
    };
    const TestNonGregoItem * itemPtr;
    for (itemPtr = items; itemPtr->locale != nullptr; itemPtr++) {
        Locale locale = Locale::createFromName(itemPtr->locale);
        LocalPointer<DateFormat> dfmt;
        UErrorCode status = U_ZERO_ERROR;
        if (itemPtr->style != DateFormat::kNone) {
            dfmt.adoptInstead(DateFormat::createDateInstance(itemPtr->style, locale));
        } else {
            dfmt.adoptInstead(new SimpleDateFormat(itemPtr->pattern, locale, status));
        }
        if (U_FAILURE(status)) {
            dataerrln("new SimpleDateFormat fails for locale %s", itemPtr->locale);
        } else  if (!dfmt.isValid()) {
            dataerrln("DateFormat::createDateInstance fails for locale %s", itemPtr->locale);
        } else {
            LocalPointer<Calendar>cal((dfmt->getCalendar())->clone());
            if (!cal.isValid()) {
                dataerrln("(DateFormat::getCalendar)->clone() fails for locale %s", itemPtr->locale);
            } else {
                const CalAndFmtTestItem * caftItemPtr;
                for (caftItemPtr = itemPtr->caftItems; caftItemPtr->year != 0; caftItemPtr++) {
                    cal->clear();
                    cal->set(UCAL_ERA,    caftItemPtr->era);
                    cal->set(UCAL_YEAR,   caftItemPtr->year);
                    cal->set(UCAL_MONTH,  caftItemPtr->month);
                    cal->set(UCAL_DATE,   caftItemPtr->day);
                    cal->set(UCAL_HOUR_OF_DAY, caftItemPtr->hour);
                    cal->set(UCAL_MINUTE, caftItemPtr->minute);
                    UnicodeString result;
                    FieldPosition fpos(FieldPosition::DONT_CARE);
                    dfmt->format(*cal, result, fpos);
                    if ( result.compare(caftItemPtr->formattedDate) != 0 ) {
                        errln( UnicodeString("FAIL: date format for locale ") + UnicodeString(itemPtr->locale) + ", style " + itemPtr->style +
                                ", expected \"" + caftItemPtr->formattedDate + "\", got \"" + result + "\"");
                    } else {
                        // formatted OK, try parse
                        ParsePosition ppos(0);
                        dfmt->parse(result, *cal, ppos);
                        status = U_ZERO_ERROR;
                        int32_t era = cal->get(UCAL_ERA, status);
                        int32_t year = cal->get(UCAL_YEAR, status);
                        int32_t month = cal->get(UCAL_MONTH, status);
                        int32_t day = cal->get(UCAL_DATE, status);
                        if ( U_FAILURE(status) || ppos.getIndex() < result.length() || era != caftItemPtr->era ||
                                year != caftItemPtr->year || month != caftItemPtr->month || day != caftItemPtr->day ) {
                            errln( UnicodeString("FAIL: date parse for locale ") + UnicodeString(itemPtr->locale) +
                                ", style " + itemPtr->style + ", string \"" + result + "\", expected " +
                                caftItemPtr->era +":"+caftItemPtr->year +"-"+caftItemPtr->month+"-"+caftItemPtr->day + ", got pos " +
                                ppos.getIndex() + " " + year +"-"+month+"-"+day + " status " + UnicodeString(u_errorName(status)) );
                        }
                    }
                }
            }
        }
    }
}

typedef struct {
    const char*         localeID;
    DateFormat::EStyle  style;
    UnicodeString       expectPattern;
    UnicodeString       expectFormat;
} TestFmtWithNumSysItem;
enum { kBBufMax = 128 };
void DateFormatTest::TestFormatsWithNumberSystems()
{
    LocalPointer<TimeZone> zone(TimeZone::createTimeZone(UnicodeString("UTC")));
    const UDate date = 1451556000000.0; // for UTC: grego 31-Dec-2015 10 AM, hebrew 19 tevet 5776, chinese yi-wei 11mo 21day
    const TestFmtWithNumSysItem items[] = {
        { "haw@calendar=gregorian", DateFormat::kShort, UnicodeString("d/M/yy"),               UnicodeString("31/xii/15") },
        { "he@calendar=hebrew",     DateFormat::kLong, CharsToUnicodeString("d \\u05D1MMMM y"), CharsToUnicodeString("\\u05D9\\u05F4\\u05D8 \\u05D1\\u05D8\\u05D1\\u05EA \\u05EA\\u05E9\\u05E2\\u05F4\\u05D5") },
#if APPLE_ICU_CHANGES
// rdar://
        { "zh@calendar=chinese",      DateFormat::kLong, CharsToUnicodeString("rU\\u5E74MMMd"), CharsToUnicodeString("2015\\u4E59\\u672A\\u5E74\\u51AC\\u6708\\u5EFF\\u4E00") }, // "2015乙未年冬月廿一"
        { "zh_Hant@calendar=chinese", DateFormat::kLong, CharsToUnicodeString("rU\\u5E74MMMd"), CharsToUnicodeString("2015\\u4E59\\u672A\\u5E74\\u51AC\\u6708\\u5EFF\\u4E00") }, // "2015乙未年冬月廿一"
        { "ja@calendar=chinese", DateFormat::kLong, CharsToUnicodeString("U\\u5E74MMMd\\u65E5"), CharsToUnicodeString("\\u4E59\\u672A\\u5E74\\u5341\\u4E00\\u6708\\u4E8C\\u5341\\u4E00\\u65E5") }, // "乙未年十一月二十一日"
#else
        { "zh@calendar=chinese",      DateFormat::kLong, CharsToUnicodeString("rU\\u5E74MMMd"), CharsToUnicodeString("2015\\u4E59\\u672A\\u5E74\\u5341\\u4E00\\u6708\\u5EFF\\u4E00") },
        { "zh_Hant@calendar=chinese", DateFormat::kLong, CharsToUnicodeString("rU\\u5E74MMMd"), CharsToUnicodeString("2015\\u4E59\\u672A\\u5E74\\u51AC\\u6708\\u5EFF\\u4E00") },
        { "ja@calendar=chinese", DateFormat::kLong, CharsToUnicodeString("U\\u5E74MMMd\\u65E5"), CharsToUnicodeString("\\u4E59\\u672A\\u5E74\\u5341\\u4E00\\u6708\\u4E8C\\u4E00\\u65E5") },
#endif  // APPLE_ICU_CHANGES
        { nullptr, DateFormat::kNone, UnicodeString(""), UnicodeString("") },
    };
    const TestFmtWithNumSysItem * itemPtr;
    for (itemPtr = items; itemPtr->localeID != nullptr; itemPtr++) {
        char bExpected[kBBufMax];
        char bResult[kBBufMax];
        UErrorCode status = U_ZERO_ERROR;
        Locale locale = Locale::createFromName(itemPtr->localeID);
        LocalPointer<Calendar> cal(Calendar::createInstance(zone.orphan(), locale, status));
        if (U_FAILURE(status)) {
            dataerrln("Calendar::createInstance fails for locale %s, status %s", itemPtr->localeID, u_errorName(status));
            continue;
        }
        cal->setTime(date, status);
        if (U_FAILURE(status)) {
            dataerrln("Calendar::setTime fails for locale %s, date %.1f, status %s", itemPtr->localeID, date, u_errorName(status));
            continue;
        }
        LocalPointer<SimpleDateFormat> sdfmt(dynamic_cast<SimpleDateFormat *>(DateFormat::createDateInstance(itemPtr->style, locale)));
        if (sdfmt.isNull()) {
            dataerrln("DateFormat::createDateInstance fails for locale %s", itemPtr->localeID);
            continue;
        }
        UnicodeString getFormat;
        sdfmt->format(*(cal.getAlias()), getFormat, nullptr, status);
        if (U_FAILURE(status)) {
            errln("DateFormat::format fails for locale %s, status %s", itemPtr->localeID, u_errorName(status));
            continue;
        }
        if (getFormat.compare(itemPtr->expectFormat) != 0) {
            itemPtr->expectFormat.extract(0, itemPtr->expectFormat.length(), bExpected, kBBufMax);
            getFormat.extract(0, getFormat.length(), bResult, kBBufMax);
            errln("DateFormat::format for locale %s, expected \"%s\", got \"%s\"", itemPtr->localeID, bExpected, bResult);
        }
        UnicodeString getPattern;
        sdfmt->toPattern(getPattern);
        if (getPattern.compare(itemPtr->expectPattern) != 0) {
            itemPtr->expectPattern.extract(0, itemPtr->expectPattern.length(), bExpected, kBBufMax);
            getPattern.extract(0, getPattern.length(), bResult, kBBufMax);
            errln("DateFormat::toPattern() for locale %s, expected \"%s\", got \"%s\"", itemPtr->localeID, bExpected, bResult);
        }
    }
}

static const UDate TEST_DATE = 1326585600000.;  // 2012-jan-15

void DateFormatTest::TestDotAndAtLeniency() {
    // Test for date/time parsing regression with CLDR 22.1/ICU 50 pattern strings.
    // For details see https://unicode-org.atlassian.net/browse/ICU-9789
    static const char *locales[] = { "en", "fr" };
    for (int32_t i = 0; i < UPRV_LENGTHOF(locales); ++i) {
        Locale locale(locales[i]);

        for (DateFormat::EStyle dateStyle = DateFormat::FULL; dateStyle <= DateFormat::SHORT;
                  dateStyle = static_cast<DateFormat::EStyle>(dateStyle + 1)) {
            LocalPointer<DateFormat> dateFormat(DateFormat::createDateInstance(dateStyle, locale));

            for (DateFormat::EStyle timeStyle = DateFormat::FULL; timeStyle <= DateFormat::SHORT;
                      timeStyle = static_cast<DateFormat::EStyle>(timeStyle + 1)) {
                LocalPointer<DateFormat> format(DateFormat::createDateTimeInstance(dateStyle, timeStyle, locale));
                LocalPointer<DateFormat> timeFormat(DateFormat::createTimeInstance(timeStyle, locale));
                UnicodeString formattedString;
                if (format.isNull()) {
                    dataerrln("Unable to create DateFormat");
                    continue;
                }
                format->format(TEST_DATE, formattedString);

                if (!showParse(*format, formattedString)) {
                    errln(UnicodeString("    with date-time: dateStyle=") + dateStyle + " timeStyle=" + timeStyle);
                }

                UnicodeString ds, ts;
                formattedString = dateFormat->format(TEST_DATE, ds) + "  " + timeFormat->format(TEST_DATE, ts);
                if (!showParse(*format, formattedString)) {
                    errln(UnicodeString("    with date sp sp time: dateStyle=") + dateStyle + " timeStyle=" + timeStyle);
                }
                if (formattedString.indexOf("n ") >= 0) { // will add "." after the end of text ending in 'n', like Jan.
                    UnicodeString plusDot(formattedString);
                    plusDot.findAndReplace("n ", "n. ").append(".");
                    if (!showParse(*format, plusDot)) {
                        errln(UnicodeString("    with date plus-dot time: dateStyle=") + dateStyle + " timeStyle=" + timeStyle);
                    }
                }
                if (formattedString.indexOf(". ") >= 0) { // will subtract "." at the end of strings.
                    UnicodeString minusDot(formattedString);
                    minusDot.findAndReplace(". ", " ");
                    if (!showParse(*format, minusDot)) {
                        errln(UnicodeString("    with date minus-dot time: dateStyle=") + dateStyle + " timeStyle=" + timeStyle);
                    }
                }
            }
        }
    }
}

UBool DateFormatTest::showParse(DateFormat &format, const UnicodeString &formattedString) {
    ParsePosition parsePosition;
    UDate parsed = format.parse(formattedString, parsePosition);
    UBool ok = TEST_DATE == parsed && parsePosition.getIndex() == formattedString.length();
    UnicodeString pattern;
    dynamic_cast<SimpleDateFormat &>(format).toPattern(pattern);
    if (ok) {
        logln(pattern + "  parsed: " + formattedString);
    } else {
        errln(pattern + "  fails to parse: " + formattedString);
    }
    return ok;
}


typedef struct {
    const char * locale;
    UBool leniency;
    UnicodeString parseString;
    UnicodeString pattern;
    UnicodeString expectedResult;       // empty string indicates expected error
} TestDateFormatLeniencyItem;

void DateFormatTest::TestDateFormatLeniency() {
    // For details see https://unicode-org.atlassian.net/browse/ICU-10261

    const UDate july022008 = 1215000001979.0;
    const TestDateFormatLeniencyItem items[] = {
        //locale    leniency    parse String                    pattern                             expected result
        { "en",     true,       UnicodeString("2008-07 02"),    UnicodeString("yyyy-LLLL dd"),      UnicodeString("2008-July 02") },
        { "en",     false,      UnicodeString("2008-07 02"),    UnicodeString("yyyy-LLLL dd"),      UnicodeString("") },
        { "en",     true,       UnicodeString("2008-Jan 02"),   UnicodeString("yyyy-LLL. dd"),      UnicodeString("2008-Jan. 02") },
        { "en",     false,      UnicodeString("2008-Jan 02"),   UnicodeString("yyyy-LLL. dd"),      UnicodeString("") },
        { "en",     true,       UnicodeString("2008-Jan--02"),  UnicodeString("yyyy-MMM' -- 'dd"),  UnicodeString("2008-Jan -- 02") },
        { "en",     false,      UnicodeString("2008-Jan--02"),  UnicodeString("yyyy-MMM' -- 'dd"),  UnicodeString("") },
        // terminator
        { nullptr,     true,       UnicodeString(""),              UnicodeString(""),                  UnicodeString("") }
    };
    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<Calendar> cal(Calendar::createInstance(status));
    if (U_FAILURE(status)) {
        dataerrln(UnicodeString("FAIL: Unable to create Calendar for default timezone and locale."));
        return;
    }
    cal->setTime(july022008, status);
    const TestDateFormatLeniencyItem * itemPtr;
    LocalPointer<SimpleDateFormat> sdmft;
    for (itemPtr = items; itemPtr->locale != nullptr; itemPtr++ ) {

       Locale locale = Locale::createFromName(itemPtr->locale);
       status = U_ZERO_ERROR;
       ParsePosition pos(0);
       sdmft.adoptInsteadAndCheckErrorCode(new SimpleDateFormat(itemPtr->pattern, locale, status), status);
       if (U_FAILURE(status)) {
           dataerrln("Unable to create SimpleDateFormat - %s", u_errorName(status));
           continue;
       }
       sdmft->setBooleanAttribute(UDAT_PARSE_ALLOW_WHITESPACE, itemPtr->leniency, status).
              setBooleanAttribute(UDAT_PARSE_ALLOW_NUMERIC, itemPtr->leniency, status).
              setBooleanAttribute(UDAT_PARSE_PARTIAL_LITERAL_MATCH, itemPtr->leniency, status);
       UDate d = sdmft->parse(itemPtr->parseString, pos);

       if(itemPtr->expectedResult.length() == 0) {
           if(pos.getErrorIndex() != -1) {
               continue;
           } else {
                errln("error: unexpected parse success - " + itemPtr->parseString +
                    " - pattern " + itemPtr->pattern +
                    " - error index " + pos.getErrorIndex() +
                    " - leniency " + itemPtr->leniency);
                continue;
           }
       }
       if(pos.getErrorIndex() != -1) {
           errln("error: parse error for string - "  + itemPtr->parseString +
                 " - pattern " + itemPtr->pattern +
                 " - idx " + pos.getIndex() +
                 " - error index "+pos.getErrorIndex() +
                 " - leniency " + itemPtr->leniency);
            continue;
        }

       UnicodeString formatResult("");
       sdmft->format(d, formatResult);
       if(formatResult.compare(itemPtr->expectedResult) != 0) {
           errln("error: unexpected format result. pattern["+itemPtr->pattern+"] expected[" + itemPtr->expectedResult + "]  but result was[" + formatResult + "]");
           continue;
        } else {
            logln("formatted results match! - " + formatResult);
        }

    }
}


typedef struct {
    UBool leniency;
    UnicodeString parseString;
    UnicodeString pattern;
    UnicodeString expectedResult;       // empty string indicates expected error
} TestMultiPatternMatchItem;

void DateFormatTest::TestParseMultiPatternMatch() {
        // For details see https://unicode-org.atlassian.net/browse/ICU-10336
    const TestMultiPatternMatchItem items[] = {
          // leniency    parse String                                 pattern                               expected result
            {true,       UnicodeString("2013-Sep 13"),                UnicodeString("yyyy-MMM dd"),         UnicodeString("2013-Sep 13")},
            {true,       UnicodeString("2013-September 14"),          UnicodeString("yyyy-MMM dd"),         UnicodeString("2013-Sep 14")},
            {false,      UnicodeString("2013-September 15"),          UnicodeString("yyyy-MMM dd"),         UnicodeString("")},
            {false,      UnicodeString("2013-September 16"),          UnicodeString("yyyy-MMMM dd"),        UnicodeString("2013-September 16")},
            {true,       UnicodeString("2013-Sep 17"),                UnicodeString("yyyy-LLL dd"),         UnicodeString("2013-Sep 17")},
            {true,       UnicodeString("2013-September 18"),          UnicodeString("yyyy-LLL dd"),         UnicodeString("2013-Sep 18")},
            {false,      UnicodeString("2013-September 19"),          UnicodeString("yyyy-LLL dd"),         UnicodeString("")},
            {false,      UnicodeString("2013-September 20"),          UnicodeString("yyyy-LLLL dd"),        UnicodeString("2013-September 20")},
            {true,       UnicodeString("2013 Sat Sep 21"),            UnicodeString("yyyy EEE MMM dd"),     UnicodeString("2013 Sat Sep 21")},
            {true,       UnicodeString("2013 Sunday Sep 22"),         UnicodeString("yyyy EEE MMM dd"),     UnicodeString("2013 Sun Sep 22")},
            {false,      UnicodeString("2013 Monday Sep 23"),         UnicodeString("yyyy EEE MMM dd"),     UnicodeString("")},
            {false,      UnicodeString("2013 Tuesday Sep 24"),        UnicodeString("yyyy EEEE MMM dd"),    UnicodeString("2013 Tuesday Sep 24")},
            {true,       UnicodeString("2013 Wed Sep 25"),            UnicodeString("yyyy eee MMM dd"),     UnicodeString("2013 Wed Sep 25")},
            {true,       UnicodeString("2013 Thu Sep 26"),            UnicodeString("yyyy eee MMM dd"),     UnicodeString("2013 Thu Sep 26")},
            {false,      UnicodeString("2013 Friday Sep 27"),         UnicodeString("yyyy eee MMM dd"),     UnicodeString("")},
            {false,      UnicodeString("2013 Saturday Sep 28"),       UnicodeString("yyyy eeee MMM dd"),    UnicodeString("2013 Saturday Sep 28")},
            {true,       UnicodeString("2013 Sun Sep 29"),            UnicodeString("yyyy ccc MMM dd"),     UnicodeString("2013 Sun Sep 29")},
            {true,       UnicodeString("2013 Monday Sep 30"),         UnicodeString("yyyy ccc MMM dd"),     UnicodeString("2013 Mon Sep 30")},
            {false,      UnicodeString("2013 Sunday Oct 13"),         UnicodeString("yyyy ccc MMM dd"),     UnicodeString("")},
            {false,      UnicodeString("2013 Monday Oct 14"),         UnicodeString("yyyy cccc MMM dd"),    UnicodeString("2013 Monday Oct 14")},
            {true,       UnicodeString("2013 Oct 15 Q4"),             UnicodeString("yyyy MMM dd QQQ"),     UnicodeString("2013 Oct 15 Q4")},
            {true,       UnicodeString("2013 Oct 16 4th quarter"),    UnicodeString("yyyy MMM dd QQQ"),     UnicodeString("2013 Oct 16 Q4")},
            {false,      UnicodeString("2013 Oct 17 4th quarter"),    UnicodeString("yyyy MMM dd QQQ"),     UnicodeString("")},
            {false,      UnicodeString("2013 Oct 18 Q4"),             UnicodeString("yyyy MMM dd QQQ"),     UnicodeString("2013 Oct 18 Q4")},
            {true,       UnicodeString("2013 Oct 19 Q4"),             UnicodeString("yyyy MMM dd qqqq"),    UnicodeString("2013 Oct 19 4th quarter")},
            {true,       UnicodeString("2013 Oct 20 4th quarter"),    UnicodeString("yyyy MMM dd qqqq"),    UnicodeString("2013 Oct 20 4th quarter")},
            {false,      UnicodeString("2013 Oct 21 Q4"),             UnicodeString("yyyy MMM dd qqqq"),    UnicodeString("")},
            {false,      UnicodeString("2013 Oct 22 4th quarter"),    UnicodeString("yyyy MMM dd qqqq"),    UnicodeString("2013 Oct 22 4th quarter")},
            {false,      UnicodeString("--end--"),                    UnicodeString(""),                    UnicodeString("")},
    };

    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<Calendar> cal(Calendar::createInstance(status));
    if (U_FAILURE(status)) {
        dataerrln(UnicodeString("FAIL: Unable to create Calendar for default timezone and locale."));
        return;
    }
    const TestMultiPatternMatchItem * itemPtr;
    DateFormat* sdmft = DateFormat::createDateInstance();
    if (sdmft == nullptr) {
        dataerrln(UnicodeString("FAIL: Unable to create DateFormat"));
        return;
    }
    for (itemPtr = items; itemPtr->parseString != "--end--"; itemPtr++ ) {
       status = U_ZERO_ERROR;
       ParsePosition pos(0);
       (dynamic_cast<SimpleDateFormat*>(sdmft))->applyPattern(itemPtr->pattern);
       if (U_FAILURE(status)) {
           dataerrln("Unable to create SimpleDateFormat - %s", u_errorName(status));
           continue;
       }
       sdmft->setBooleanAttribute(UDAT_PARSE_MULTIPLE_PATTERNS_FOR_MATCH, itemPtr->leniency, status);
       UDate d = sdmft->parse(itemPtr->parseString, pos);

       if(itemPtr->expectedResult.length() == 0) {
           if(pos.getErrorIndex() != -1) {
               continue;
           } else {
                errln("error: unexpected parse success - " + itemPtr->parseString +
                    " - error index " + pos.getErrorIndex() +
                    " - leniency " + itemPtr->leniency);
                continue;
           }
        }
        if(pos.getErrorIndex() != -1) {
            errln("error: parse error for string - " +itemPtr->parseString + " -- idx["+pos.getIndex()+"] errIdx["+pos.getErrorIndex()+"]");
            continue;
        }

        UnicodeString formatResult("");
        sdmft->format(d, formatResult);
        if(formatResult.compare(itemPtr->expectedResult) != 0) {
            errln("error: unexpected format result. expected[" + itemPtr->expectedResult + "]  but result was[" + formatResult + "]");
        } else {
            logln("formatted results match! - " + formatResult);
        }
    }
    delete sdmft;
}

void DateFormatTest::TestParseLeniencyAPIs() {
    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<DateFormat> dateFormat(DateFormat::createDateInstance());
    DateFormat *fmt = dateFormat.getAlias();
    if (fmt == nullptr) {
        dataerrln("Failed calling dateFormat.getAlias()");
        return;
    }

    assertTrue("isLenient default", fmt->isLenient());
    assertTrue("isCalendarLenient default", fmt->isCalendarLenient());
    assertTrue("ALLOW_WHITESPACE default", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_WHITESPACE, status));
    assertTrue("ALLOW_NUMERIC default", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_NUMERIC, status));
    assertTrue("PARTIAL_MATCH default", fmt->getBooleanAttribute(UDAT_PARSE_PARTIAL_LITERAL_MATCH, status));
    assertTrue("MULTIPLE_PATTERNS default", fmt->getBooleanAttribute(UDAT_PARSE_MULTIPLE_PATTERNS_FOR_MATCH, status));

    // Set calendar to strict
    fmt->setCalendarLenient(false);

    assertFalse("isLenient after setCalendarLenient(false)", fmt->isLenient());
    assertFalse("isCalendarLenient after setCalendarLenient(false)", fmt->isCalendarLenient());
    assertTrue("ALLOW_WHITESPACE after setCalendarLenient(false)", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_WHITESPACE, status));
    assertTrue("ALLOW_NUMERIC  after setCalendarLenient(false)", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_NUMERIC, status));

    // Set to strict
    fmt->setLenient(false);

    assertFalse("isLenient after setLenient(false)", fmt->isLenient());
    assertFalse("isCalendarLenient after setLenient(false)", fmt->isCalendarLenient());
    assertFalse("ALLOW_WHITESPACE after setLenient(false)", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_WHITESPACE, status));
    assertFalse("ALLOW_NUMERIC  after setLenient(false)", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_NUMERIC, status));
    // These two boolean attributes are NOT affected according to the API specification
    assertTrue("PARTIAL_MATCH after setLenient(false)", fmt->getBooleanAttribute(UDAT_PARSE_PARTIAL_LITERAL_MATCH, status));
    assertTrue("MULTIPLE_PATTERNS after setLenient(false)", fmt->getBooleanAttribute(UDAT_PARSE_MULTIPLE_PATTERNS_FOR_MATCH, status));

    // Allow white space leniency
    fmt->setBooleanAttribute(UDAT_PARSE_ALLOW_WHITESPACE, true, status);

    assertFalse("isLenient after ALLOW_WHITESPACE/true", fmt->isLenient());
    assertFalse("isCalendarLenient after ALLOW_WHITESPACE/true", fmt->isCalendarLenient());
    assertTrue("ALLOW_WHITESPACE after ALLOW_WHITESPACE/true", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_WHITESPACE, status));
    assertFalse("ALLOW_NUMERIC  after ALLOW_WHITESPACE/true", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_NUMERIC, status));

    // Set to lenient
    fmt->setLenient(true);

    assertTrue("isLenient after setLenient(true)", fmt->isLenient());
    assertTrue("isCalendarLenient after setLenient(true)", fmt->isCalendarLenient());
    assertTrue("ALLOW_WHITESPACE after setLenient(true)", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_WHITESPACE, status));
    assertTrue("ALLOW_NUMERIC after setLenient(true)", fmt->getBooleanAttribute(UDAT_PARSE_ALLOW_NUMERIC, status));
}

void DateFormatTest::TestNumberFormatOverride() {
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString fields = UnicodeString("M");

    LocalPointer<SimpleDateFormat> fmt;
    fmt.adoptInsteadAndCheckErrorCode(new SimpleDateFormat(UnicodeString("MM d"), status), status);
    if (!assertSuccess("SimpleDateFormat with pattern MM d", status)) {
        return;
    }


    for(int i=0; i<3; i++){
        NumberFormat* check_nf = NumberFormat::createInstance(Locale("en_US"), status);
        assertSuccess("NumberFormat en_US", status);
        fmt->adoptNumberFormat(fields, check_nf, status);
        assertSuccess("adoptNumberFormat check_nf", status);

        const NumberFormat* get_nf = fmt->getNumberFormatForField(static_cast<char16_t>(0x004D) /*'M'*/);
        if (get_nf != check_nf) errln("FAIL: getter and setter do not work");
    }
    NumberFormat* check_nf = NumberFormat::createInstance(Locale("en_US"), status);
    assertSuccess("NumberFormat en_US", status);
    fmt->adoptNumberFormat(check_nf); // make sure using the same NF will not crash

    const char * DATA [][2] = {
        { "", "\\u521D\\u516D \\u5341\\u4E94"},
        { "M", "\\u521D\\u516D 15"},
        { "Mo", "\\u521D\\u516D 15"},
        { "Md", "\\u521D\\u516D \\u5341\\u4E94"},
        { "MdMMd", "\\u521D\\u516D \\u5341\\u4E94"},
        { "mixed", "\\u521D\\u516D \\u5341\\u4E94"}
    };

    UDate test_date = date(97, 6 - 1, 15);

    for(int i=0; i < UPRV_LENGTHOF(DATA); i++){
        fields = DATA[i][0];

        LocalPointer<SimpleDateFormat> fmt;
        fmt.adoptInsteadAndCheckErrorCode(new SimpleDateFormat(UnicodeString("MM d"), status), status);
        assertSuccess("SimpleDateFormat with pattern MM d", status);
        NumberFormat* overrideNF = NumberFormat::createInstance(Locale::createFromName("zh@numbers=hanidays"),status);
        assertSuccess("NumberFormat zh@numbers=hanidays", status);
        if (U_FAILURE(status)) {
            status = U_ZERO_ERROR;
            continue;
        }

        if (fields == UnicodeString("")) { // use the one w/o fields
            fmt->adoptNumberFormat(overrideNF);
        } else if (fields == UnicodeString("mixed")) { // set 1 field at first but then full override, both(M & d) should be override
            NumberFormat* singleOverrideNF = NumberFormat::createInstance(Locale::createFromName("en@numbers=hebr"),status);
            assertSuccess("NumberFormat en@numbers=hebr", status);

            fields = UnicodeString("M");
            fmt->adoptNumberFormat(fields, singleOverrideNF, status);
            assertSuccess("adoptNumberFormat singleOverrideNF", status);

            fmt->adoptNumberFormat(overrideNF);
        } else if (fields == UnicodeString("Mo")) { // o is invalid field
            fmt->adoptNumberFormat(fields, overrideNF, status);
            if(status == U_INVALID_FORMAT_ERROR) {
                status = U_ZERO_ERROR;
                continue;
            }
        } else {
            fmt->adoptNumberFormat(fields, overrideNF, status);
            assertSuccess("adoptNumberFormat overrideNF", status);
        }

        UnicodeString result;
        FieldPosition pos(FieldPosition::DONT_CARE);
        fmt->format(test_date,result, pos);

        UnicodeString expected = (UnicodeString(DATA[i][1])).unescape();

        if (result != expected)
            errln("FAIL: Expected " + expected + " get: " + result);

        // Ensure that adopted formats are handled correctly after copy constructing
        SimpleDateFormat fmtCopy = *fmt;
        result.remove();
        fmtCopy.format(test_date,result, pos);
        if (result != expected)
            errln("FAIL: Expected " + expected + " after copy constructing get: " + result);
    }
}

void DateFormatTest::TestCreateInstanceForSkeleton() {
    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<DateFormat> fmt(DateFormat::createInstanceForSkeleton(
            "yMMMMd", "en", status));
    if (!assertSuccess("Create with pattern yMMMMd", status)) {
        return;
    }
    UnicodeString result;
    FieldPosition pos(FieldPosition::DONT_CARE);
    fmt->format(date(98, 5-1, 25), result, pos);
    assertEquals("format yMMMMd", "May 25, 1998", result);
    fmt.adoptInstead(DateFormat::createInstanceForSkeleton(
            "yMd", "en", status));
    if (!assertSuccess("Create with pattern yMd", status)) {
        return;
    }
    result.remove();
    fmt->format(date(98, 5-1, 25), result, pos);
    assertEquals("format yMd", "5/25/1998", result);
}

void DateFormatTest::TestCreateInstanceForSkeletonDefault() {
    UErrorCode status = U_ZERO_ERROR;
    Locale savedLocale;
    Locale::setDefault(Locale::getUS(), status);
    LocalPointer<DateFormat> fmt(DateFormat::createInstanceForSkeleton(
            "yMMMd", status));
    Locale::setDefault(savedLocale, status);
    if (!assertSuccess("Create with pattern yMMMd", status)) {
        return;
    }
    UnicodeString result;
    FieldPosition pos(FieldPosition::DONT_CARE);
    fmt->format(date(98, 5-1, 25), result, pos);
    assertEquals("format yMMMd", "May 25, 1998", result);
}

void DateFormatTest::TestCreateInstanceForSkeletonWithCalendar() {
    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<DateFormat> fmt(
            DateFormat::createInstanceForSkeleton(
                    Calendar::createInstance(
                            TimeZone::createTimeZone("GMT-3:00"),
                            status),
                    "yMdHm", "en", status));
    if (!assertSuccess("Create with pattern yMMMMd", status)) {
        return;
    }
    UnicodeString result;
    FieldPosition pos(FieldPosition::DONT_CARE);

    LocalPointer<Calendar> cal(Calendar::createInstance(
        TimeZone::createTimeZone("GMT-7:00"),
        status));
    if (!assertSuccess("Creating GMT-7 time zone failed", status)) {
        return;
    }
    cal->clear();
    cal->set(1998, 5-1, 25, 0, 0, 0);

    // date format time zone should be 4 hours ahead.
    fmt->format(cal->getTime(status), result, pos);
    assertEquals("format yMdHm", "5/25/1998, 04:00", result);
    assertSuccess("", status);
}

void DateFormatTest::TestDFSCreateForLocaleNonGregorianLocale() {
    UErrorCode status = U_ZERO_ERROR;
    Locale fa("fa");
    LocalPointer<DateFormatSymbols> sym(
            DateFormatSymbols::createForLocale(fa, status));
    if (!assertSuccess("", status)) {
        return;
    }

    // Farsi should default to the persian calendar, not gregorian
    int32_t count;
    const UnicodeString *months = sym->getShortMonths(count);

    // First persian month.
    UnicodeString expected("\\u0641\\u0631\\u0648\\u0631\\u062f\\u06cc\\u0646");
    assertEquals("", expected.unescape(), months[0]);
}

void DateFormatTest::TestDFSCreateForLocaleWithCalendarInLocale() {
    UErrorCode status = U_ZERO_ERROR;
    Locale en_heb("en@calendar=hebrew");
    LocalPointer<DateFormatSymbols> sym(
            DateFormatSymbols::createForLocale(en_heb, status));
    if (!assertSuccess("", status)) {
        return;
    }

    // We should get the months of the hebrew calendar, not the gregorian
    // calendar.
    int32_t count;
    const UnicodeString *months = sym->getShortMonths(count);

    // First hebrew month.
    UnicodeString expected("Tishri");
    assertEquals("", expected, months[0]);
}

void DateFormatTest::TestChangeCalendar() {
    UErrorCode status = U_ZERO_ERROR;
    Locale en("en");
    Locale en_heb("en@calendar=hebrew");
    LocalPointer<DateFormat> fmt(
            DateFormat::createInstanceForSkeleton("yMMMd", en, status));
    if (!assertSuccess("", status)) {
        return;
    }
    fmt->adoptCalendar(Calendar::createInstance(en_heb, status));
    if (!assertSuccess("", status)) {
        return;
    }
    UnicodeString result;
    FieldPosition pos(FieldPosition::DONT_CARE);
    fmt->format(date(98, 5-1, 25), result, pos);
    assertEquals("format yMMMd", "Iyar 29, 5758", result);
}

void DateFormatTest::TestPatternFromSkeleton() {
    static const struct {
        const Locale& locale;
        const char* const skeleton;
        const char16_t* const pattern;
    } TESTDATA[] = {
        // Ticket #11985
        {Locale::getEnglish(), "jjmm", u"h:mm\u202Fa"},
        {Locale::getEnglish(), "JJmm", u"hh:mm"},
        {Locale::getGerman(), "jjmm", u"HH:mm"},
        {Locale::getGerman(), "JJmm", u"HH:mm"},
        // Ticket #20739
        // minutes+milliseconds, seconds missing, should be repaired
        {Locale::getEnglish(), "SSSSm", u"mm:ss.SSSS"},
        {Locale::getEnglish(), "mSSSS", u"mm:ss.SSSS"},
        {Locale::getEnglish(), "SSSm", u"mm:ss.SSS"},
        {Locale::getEnglish(), "mSSS", u"mm:ss.SSS"},
        {Locale::getEnglish(), "SSm", u"mm:ss.SS"},
        {Locale::getEnglish(), "mSS", u"mm:ss.SS"},
        {Locale::getEnglish(), "Sm", u"mm:ss.S"},
        {Locale::getEnglish(), "mS", u"mm:ss.S"},
        // only milliseconds, untouched, no repairs
        {Locale::getEnglish(), "S", u"S"},
        {Locale::getEnglish(), "SS", u"SS"},
        {Locale::getEnglish(), "SSS", u"SSS"},
        {Locale::getEnglish(), "SSSS", u"SSSS"},
        // hour:minute+seconds+milliseconds, correct, no repairs, proper pattern
        {Locale::getEnglish(), "jmsSSS", u"h:mm:ss.SSS\u202Fa"},
        {Locale::getEnglish(), "jmSSS", u"h:mm:ss.SSS\u202Fa"},
        // Ticket #20738
        // seconds+milliseconds, correct, no repairs, proper pattern
        {Locale::getEnglish(), "sS", u"s.S"},
        {Locale::getEnglish(), "sSS", u"s.SS"},
        {Locale::getEnglish(), "sSSS", u"s.SSS"},
        {Locale::getEnglish(), "sSSSS", u"s.SSSS"},
        {Locale::getEnglish(), "sS", u"s.S"},
        // minutes+seconds+milliseconds, correct, no repairs, proper pattern
        {Locale::getEnglish(), "msS", u"mm:ss.S"},
        {Locale::getEnglish(), "msSS", u"mm:ss.SS"},
        {Locale::getEnglish(), "msSSS", u"mm:ss.SSS"},
        {Locale::getEnglish(), "msSSSS", u"mm:ss.SSSS"}
    };

    for (size_t i = 0; i < UPRV_LENGTHOF(TESTDATA); i++) {
        UErrorCode status = U_ZERO_ERROR;
        LocalPointer<DateFormat> fmt(
                DateFormat::createInstanceForSkeleton(
                        TESTDATA[i].skeleton, TESTDATA[i].locale, status));
        if (!assertSuccess("createInstanceForSkeleton", status)) {
            return;
        }
        UnicodeString pattern;
        dynamic_cast<const SimpleDateFormat*>(fmt.getAlias())->toPattern(pattern);
        assertEquals("Format pattern", TESTDATA[i].pattern, pattern);
    }
}

void DateFormatTest::TestAmPmMidnightNoon() {
    // Some times on 2015-11-13 (UTC+0).
    UDate k000000 = 1447372800000.0;
    UDate k000030 = 1447372830000.0;
    UDate k003000 = 1447374600000.0;
    UDate k060000 = 1447394400000.0;
    UDate k120000 = 1447416000000.0;
    UDate k180000 = 1447437600000.0;

    UErrorCode errorCode = U_ZERO_ERROR;
    SimpleDateFormat sdf(UnicodeString(), errorCode);
    if (U_FAILURE(errorCode)) {
        dataerrln("Error creating SimpleDateFormat - %s", u_errorName(errorCode));
        return;
    }
    const TimeZone *tz = TimeZone::getGMT();
    sdf.setTimeZone(*tz);
    UnicodeString out;

    // Note: "midnight" can be ambiguous as to whether it refers to beginning of day or end of day.
    // For ICU 57 output of "midnight" is temporarily suppressed.

    // Short.
    sdf.applyPattern(UnicodeString("hh:mm:ss bbb"));

    // assertEquals("hh:mm:ss bbb | 00:00:00", "12:00:00 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss bbb | 00:00:00", "12:00:00 AM", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss bbb | 00:00:30", "12:00:30 AM", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss bbb | 00:30:00", "12:30:00 AM", sdf.format(k003000, out.remove()));
    assertEquals("hh:mm:ss bbb | 06:00:00", "06:00:00 AM", sdf.format(k060000, out.remove()));
    assertEquals("hh:mm:ss bbb | 12:00:00", "12:00:00 noon", sdf.format(k120000, out.remove()));
    assertEquals("hh:mm:ss bbb | 18:00:00", "06:00:00 PM", sdf.format(k180000, out.remove()));

    sdf.applyPattern(UnicodeString("hh:mm bbb"));

    // assertEquals("hh:mm bbb | 00:00:00", "12:00 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm bbb | 00:00:00", "12:00 AM", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm bbb | 00:00:30", "12:00 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm bbb | 00:00:30", "12:00 AM", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm bbb | 00:30:00", "12:30 AM", sdf.format(k003000, out.remove()));

    sdf.applyPattern(UnicodeString("hh bbb"));

    // assertEquals("hh bbb | 00:00:00", "12 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh bbb | 00:00:00", "12 AM", sdf.format(k000000, out.remove()));
    // assertEquals("hh bbb | 00:00:30", "12 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh bbb | 00:00:30", "12 AM", sdf.format(k000030, out.remove()));
    // assertEquals("hh bbb | 00:30:00", "12 midnight", sdf.format(k003000, out.remove()));
    assertEquals("hh bbb | 00:30:00", "12 AM", sdf.format(k003000, out.remove()));

    // Wide.
    sdf.applyPattern(UnicodeString("hh:mm:ss bbbb"));

    // assertEquals("hh:mm:ss bbbb | 00:00:00", "12:00:00 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss bbbb | 00:00:00", "12:00:00 AM", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss bbbb | 00:00:30", "12:00:30 AM", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss bbbb | 00:30:00", "12:30:00 AM", sdf.format(k003000, out.remove()));
    assertEquals("hh:mm:ss bbbb | 06:00:00", "06:00:00 AM", sdf.format(k060000, out.remove()));
    assertEquals("hh:mm:ss bbbb | 12:00:00", "12:00:00 noon", sdf.format(k120000, out.remove()));
    assertEquals("hh:mm:ss bbbb | 18:00:00", "06:00:00 PM", sdf.format(k180000, out.remove()));

    sdf.applyPattern(UnicodeString("hh:mm bbbb"));

    // assertEquals("hh:mm bbbb | 00:00:00", "12:00 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm bbbb | 00:00:00", "12:00 AM", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm bbbb | 00:00:30", "12:00 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm bbbb | 00:00:30", "12:00 AM", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm bbbb | 00:30:00", "12:30 AM", sdf.format(k003000, out.remove()));

    sdf.applyPattern(UnicodeString("hh bbbb"));

    // assertEquals("hh bbbb | 00:00:00", "12 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh bbbb | 00:00:00", "12 AM", sdf.format(k000000, out.remove()));
    // assertEquals("hh bbbb | 00:00:30", "12 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh bbbb | 00:00:30", "12 AM", sdf.format(k000030, out.remove()));
    // assertEquals("hh bbbb | 00:30:00", "12 midnight", sdf.format(k003000, out.remove()));
    assertEquals("hh bbbb | 00:30:00", "12 AM", sdf.format(k003000, out.remove()));

    // Narrow.
    sdf.applyPattern(UnicodeString("hh:mm:ss bbbbb"));

    // assertEquals("hh:mm:ss bbbbb | 00:00:00", "12:00:00 mi", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss bbbbb | 00:00:00", "12:00:00 a", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss bbbbb | 00:00:30", "12:00:30 a", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss bbbbb | 00:30:00", "12:30:00 a", sdf.format(k003000, out.remove()));
    assertEquals("hh:mm:ss bbbbb | 06:00:00", "06:00:00 a", sdf.format(k060000, out.remove()));
    assertEquals("hh:mm:ss bbbbb | 12:00:00", "12:00:00 n", sdf.format(k120000, out.remove()));
    assertEquals("hh:mm:ss bbbbb | 18:00:00", "06:00:00 p", sdf.format(k180000, out.remove()));

    sdf.applyPattern(UnicodeString("hh:mm bbbbb"));

    // assertEquals("hh:mm bbbbb | 00:00:00", "12:00 mi", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm bbbbb | 00:00:00", "12:00 a", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm bbbbb | 00:00:30", "12:00 mi", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm bbbbb | 00:00:30", "12:00 a", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm bbbbb | 00:30:00", "12:30 a", sdf.format(k003000, out.remove()));

    sdf.applyPattern(UnicodeString("hh bbbbb"));

    // assertEquals("hh bbbbb | 00:00:00", "12 mi", sdf.format(k000000, out.remove()));
    assertEquals("hh bbbbb | 00:00:00", "12 a", sdf.format(k000000, out.remove()));
    // assertEquals("hh bbbbb | 00:00:30", "12 mi", sdf.format(k000030, out.remove()));
    assertEquals("hh bbbbb | 00:00:30", "12 a", sdf.format(k000030, out.remove()));
    // assertEquals("hh bbbbb | 00:30:00", "12 mi", sdf.format(k003000, out.remove()));
    assertEquals("hh bbbbb | 00:30:00", "12 a", sdf.format(k003000, out.remove()));
}

void DateFormatTest::TestFlexibleDayPeriod() {
    // Some times on 2015-11-13 (UTC+0).
    UDate k000000 = 1447372800000.0;
    UDate k000030 = 1447372830000.0;
    UDate k003000 = 1447374600000.0;
    UDate k060000 = 1447394400000.0;
    UDate k120000 = 1447416000000.0;
    UDate k180000 = 1447437600000.0;

    UErrorCode errorCode = U_ZERO_ERROR;
    SimpleDateFormat sdf(UnicodeString(), errorCode);
    if (U_FAILURE(errorCode)) {
        dataerrln("Error creating SimpleDateFormat - %s", u_errorName(errorCode));
        return;
    }
    const TimeZone *tz = TimeZone::getGMT();
    sdf.setTimeZone(*tz);
    UnicodeString out;

    // Note: "midnight" can be ambiguous as to whether it refers to beginning of day or end of day.
    // For ICU 57 output of "midnight" is temporarily suppressed.

    // Short.
    sdf.applyPattern(UnicodeString("hh:mm:ss BBB"));

    // assertEquals("hh:mm:ss BBB | 00:00:00", "12:00:00 midnight", sdf.format(k000000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm:ss BBB | 00:00:00", "12:00:00 in the morning", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBB | 00:00:30", "12:00:30 in the morning", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss BBB | 00:30:00", "12:30:00 in the morning", sdf.format(k003000, out.remove()));
#else
    assertEquals("hh:mm:ss BBB | 00:00:00", "12:00:00 at night", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBB | 00:00:30", "12:00:30 at night", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss BBB | 00:30:00", "12:30:00 at night", sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES
    assertEquals("hh:mm:ss BBB | 06:00:00", "06:00:00 in the morning", sdf.format(k060000, out.remove()));
    assertEquals("hh:mm:ss BBB | 12:00:00", "12:00:00 noon", sdf.format(k120000, out.remove()));
    assertEquals("hh:mm:ss BBB | 18:00:00", "06:00:00 in the evening", sdf.format(k180000, out.remove()));

    sdf.applyPattern(UnicodeString("hh:mm BBB"));

    // assertEquals("hh:mm BBB | 00:00:00", "12:00 midnight", sdf.format(k000000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm BBB | 00:00:00", "12:00 in the morning", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm BBB | 00:00:30", "12:00 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBB | 00:00:00", "12:00 in the morning", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm BBB | 00:30:00", "12:30 in the morning", sdf.format(k003000, out.remove()));
#else
    assertEquals("hh:mm BBB | 00:00:00", "12:00 at night", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm BBB | 00:00:30", "12:00 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBB | 00:00:00", "12:00 at night", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm BBB | 00:30:00", "12:30 at night", sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES
    sdf.applyPattern(UnicodeString("hh BBB"));

#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    // assertEquals("hh BBB | 00:00:00", "12 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh BBB | 00:00:30", "12 in the morning", sdf.format(k000030, out.remove()));
    // assertEquals("hh BBB | 00:00:30", "12 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh BBB | 00:00:30", "12 in the morning", sdf.format(k000030, out.remove()));
    // assertEquals("hh BBB | 00:30:00", "12 midnight", sdf.format(k003000, out.remove()));
    assertEquals("hh BBB | 00:30:00", "12 in the morning", sdf.format(k003000, out.remove()));
#else
    // assertEquals("hh BBB | 00:00:00", "12 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh BBB | 00:00:30", "12 at night", sdf.format(k000030, out.remove()));
    // assertEquals("hh BBB | 00:00:30", "12 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh BBB | 00:00:30", "12 at night", sdf.format(k000030, out.remove()));
    // assertEquals("hh BBB | 00:30:00", "12 midnight", sdf.format(k003000, out.remove()));
    assertEquals("hh BBB | 00:30:00", "12 at night", sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES

    // Wide.
    sdf.applyPattern(UnicodeString("hh:mm:ss BBBB"));

    // assertEquals("hh:mm:ss BBBB | 00:00:00", "12:00:00 midnight", sdf.format(k000000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm:ss BBBB | 00:00:00", "12:00:00 in the morning", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 00:00:30", "12:00:30 in the morning", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss BBBB | 00:30:00", "12:30:00 in the morning", sdf.format(k003000, out.remove()));
#else
    assertEquals("hh:mm:ss BBBB | 00:00:00", "12:00:00 at night", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 00:00:30", "12:00:30 at night", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss BBBB | 00:30:00", "12:30:00 at night", sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES
    assertEquals("hh:mm:ss BBBB | 06:00:00", "06:00:00 in the morning", sdf.format(k060000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 12:00:00", "12:00:00 noon", sdf.format(k120000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 18:00:00", "06:00:00 in the evening", sdf.format(k180000, out.remove()));

    sdf.applyPattern(UnicodeString("hh:mm BBBB"));

    // assertEquals("hh:mm BBBB | 00:00:00", "12:00 midnight", sdf.format(k000000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm BBBB | 00:00:00", "12:00 in the morning", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm BBBB | 00:00:30", "12:00 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBBB | 00:00:30", "12:00 in the morning", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBBB | 00:30:00", "12:30 in the morning", sdf.format(k003000, out.remove()));
#else
    assertEquals("hh:mm BBBB | 00:00:00", "12:00 at night", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm BBBB | 00:00:30", "12:00 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBBB | 00:00:30", "12:00 at night", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBBB | 00:30:00", "12:30 at night", sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES

    sdf.applyPattern(UnicodeString("hh BBBB"));

#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    // assertEquals("hh BBBB | 00:00:00", "12 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh BBBB | 00:00:00", "12 in the morning", sdf.format(k000000, out.remove()));
    // assertEquals("hh BBBB | 00:00:30", "12 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh BBBB | 00:00:00", "12 in the morning", sdf.format(k000000, out.remove()));
    // assertEquals("hh BBBB | 00:80:00", "12 midnight", sdf.format(k003000, out.remove()));
    assertEquals("hh BBBB | 00:00:00", "12 in the morning", sdf.format(k000000, out.remove()));
#else
    // assertEquals("hh BBBB | 00:00:00", "12 midnight", sdf.format(k000000, out.remove()));
    assertEquals("hh BBBB | 00:00:00", "12 at night", sdf.format(k000000, out.remove()));
    // assertEquals("hh BBBB | 00:00:30", "12 midnight", sdf.format(k000030, out.remove()));
    assertEquals("hh BBBB | 00:00:00", "12 at night", sdf.format(k000000, out.remove()));
    // assertEquals("hh BBBB | 00:80:00", "12 midnight", sdf.format(k003000, out.remove()));
    assertEquals("hh BBBB | 00:00:00", "12 at night", sdf.format(k000000, out.remove()));
#endif // APPLE_ICU_CHANGES

    // Narrow.
    sdf.applyPattern(UnicodeString("hh:mm:ss BBBBB"));

    // assertEquals("hh:mm:ss BBBBB | 00:00:00", "12:00:00 mi", sdf.format(k000000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm:ss BBBBB | 00:00:00", "12:00:00 in the morning", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBBB | 00:00:30", "12:00:30 in the morning", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss BBBBB | 00:30:00", "12:30:00 in the morning", sdf.format(k003000, out.remove()));
#else
    assertEquals("hh:mm:ss BBBBB | 00:00:00", "12:00:00 at night", sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBBB | 00:00:30", "12:00:30 at night", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm:ss BBBBB | 00:30:00", "12:30:00 at night", sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES
    assertEquals("hh:mm:ss BBBBB | 06:00:00", "06:00:00 in the morning", sdf.format(k060000, out.remove()));
    assertEquals("hh:mm:ss BBBBB | 12:00:00", "12:00:00 n", sdf.format(k120000, out.remove()));
    assertEquals("hh:mm:ss BBBBB | 18:00:00", "06:00:00 in the evening", sdf.format(k180000, out.remove()));

    sdf.applyPattern(UnicodeString("hh:mm BBBBB"));

    // assertEquals("hh:mm BBBBB | 00:00:00", "12:00 mi", sdf.format(k000000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm BBBBB | 00:00:00", "12:00 in the morning", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm BBBBB | 00:00:30", "12:00 mi", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBBBB | 00:00:30", "12:00 in the morning", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBBBB | 00:30:00", "12:30 in the morning", sdf.format(k003000, out.remove()));
#else
    assertEquals("hh:mm BBBBB | 00:00:00", "12:00 at night", sdf.format(k000000, out.remove()));
    // assertEquals("hh:mm BBBBB | 00:00:30", "12:00 mi", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBBBB | 00:00:30", "12:00 at night", sdf.format(k000030, out.remove()));
    assertEquals("hh:mm BBBBB | 00:30:00", "12:30 at night", sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES

    sdf.applyPattern(UnicodeString("hh BBBBB"));

#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    // assertEquals("hh BBBBB | 00:00:00", "12 mi", sdf.format(k000000, out.remove()));
    assertEquals("hh BBBBB | 00:00:00", "12 in the morning", sdf.format(k000000, out.remove()));
    // assertEquals("hh BBBBB | 00:00:30", "12 mi", sdf.format(k000030, out.remove()));
    assertEquals("hh BBBBB | 00:00:30", "12 in the morning", sdf.format(k000030, out.remove()));
    // assertEquals("hh BBBBB | 00:30:00", "12 mi", sdf.format(k003000, out.remove()));
    assertEquals("hh BBBBB | 00:30:00", "12 in the morning", sdf.format(k003000, out.remove()));
#else
    // assertEquals("hh BBBBB | 00:00:00", "12 mi", sdf.format(k000000, out.remove()));
    assertEquals("hh BBBBB | 00:00:00", "12 at night", sdf.format(k000000, out.remove()));
    // assertEquals("hh BBBBB | 00:00:30", "12 mi", sdf.format(k000030, out.remove()));
    assertEquals("hh BBBBB | 00:00:30", "12 at night", sdf.format(k000030, out.remove()));
    // assertEquals("hh BBBBB | 00:30:00", "12 mi", sdf.format(k003000, out.remove()));
    assertEquals("hh BBBBB | 00:30:00", "12 at night", sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES
}

void DateFormatTest::TestDayPeriodWithLocales() {
    // Some times on 2015-11-13 (UTC+0).
    UDate k000000 = 1447372800000.0;
    UDate k010000 = 1447376400000.0;
    UDate k120000 = 1447416000000.0;
    UDate k220000 = 1447452000000.0;

    UErrorCode errorCode = U_ZERO_ERROR;
    const TimeZone *tz = TimeZone::getGMT();
    UnicodeString out;

    // Note: "midnight" can be ambiguous as to whether it refers to beginning of day or end of day.
    // For ICU 57 output of "midnight" and its localized equivalentns is temporarily suppressed.

    // Locale de has a word for midnight, but not noon.
    SimpleDateFormat sdf(UnicodeString(), Locale::getGermany(), errorCode);
    if (U_FAILURE(errorCode)) {
        dataerrln("Error creating SimpleDateFormat - %s", u_errorName(errorCode));
        return;
    }
    sdf.setTimeZone(*tz);

    sdf.applyPattern(UnicodeString("hh:mm:ss bbbb"));

    // assertEquals("hh:mm:ss bbbb | 00:00:00 | de", "12:00:00 Mitternacht",
    //     sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss bbbb | 00:00:00 | de", "12:00:00 AM",
        sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss bbbb | 12:00:00 | de", "12:00:00 PM",
        sdf.format(k120000, out.remove()));

    // Locale ee has a rule that wraps around midnight (21h - 4h).
    sdf = SimpleDateFormat(UnicodeString(), Locale("ee"), errorCode);
    sdf.setTimeZone(*tz);

    sdf.applyPattern(UnicodeString("hh:mm:ss BBBB"));

    assertEquals("hh:mm:ss BBBB | 22:00:00 | ee", UnicodeString("10:00:00 z\\u00E3").unescape(),
        sdf.format(k220000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 00:00:00 | ee", UnicodeString("12:00:00 z\\u00E3").unescape(),
        sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 01:00:00 | ee", UnicodeString("01:00:00 z\\u00E3").unescape(),
        sdf.format(k010000, out.remove()));

    // Locale root has rules for AM/PM only.
    sdf = SimpleDateFormat(UnicodeString(), Locale("root"), errorCode);
    sdf.setTimeZone(*tz);

    sdf.applyPattern(UnicodeString("hh:mm:ss BBBB"));

    assertEquals("hh:mm:ss BBBB | 00:00:00 | root", "12:00:00 AM",
        sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 12:00:00 | root", "12:00:00 PM",
        sdf.format(k120000, out.remove()));

    // Empty string should behave exactly as root.
    sdf = SimpleDateFormat(UnicodeString(), Locale(""), errorCode);
    sdf.setTimeZone(*tz);

    sdf.applyPattern(UnicodeString("hh:mm:ss BBBB"));

    assertEquals("hh:mm:ss BBBB | 00:00:00 | \"\" (root)", "12:00:00 AM",
        sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 12:00:00 | \"\" (root)", "12:00:00 PM",
        sdf.format(k120000, out.remove()));

    // Locale en_US should fall back to en.
    sdf = SimpleDateFormat(UnicodeString(), Locale("en_US"), errorCode);
    sdf.setTimeZone(*tz);

    sdf.applyPattern(UnicodeString("hh:mm:ss BBBB"));

    // assertEquals("hh:mm:ss BBBB | 00:00:00 | en_US", "12:00:00 midnight",
    //     sdf.format(k000000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm:ss BBBB | 00:00:00 | en_US", "12:00:00 in the morning",
         sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 01:00:00 | en_US", "01:00:00 in the morning",
        sdf.format(k010000, out.remove()));
#else
    assertEquals("hh:mm:ss BBBB | 00:00:00 | en_US", "12:00:00 at night",
         sdf.format(k000000, out.remove()));
    assertEquals("hh:mm:ss BBBB | 01:00:00 | en_US", "01:00:00 at night",
        sdf.format(k010000, out.remove()));
#endif // APPLE_ICU_CHANGES
    assertEquals("hh:mm:ss BBBB | 12:00:00 | en_US", "12:00:00 noon",
        sdf.format(k120000, out.remove()));

    // Locale es_CO should not fall back to es and should have a
    // different string for 1 in the morning.
    // (es_CO: "de la manana" (first n has a tilde) vs. es: "de la madrugada")
    sdf = SimpleDateFormat(UnicodeString(), Locale("es_CO"), errorCode);
    sdf.setTimeZone(*tz);

    sdf.applyPattern(UnicodeString("hh:mm:ss BBBB"));
    assertEquals("hh:mm:ss BBBB | 01:00:00 | es_CO", u"01:00:00 de la mañana",
        sdf.format(k010000, out.remove()));

    sdf = SimpleDateFormat(UnicodeString(), Locale("es"), errorCode);
    sdf.setTimeZone(*tz);

    sdf.applyPattern(UnicodeString("hh:mm:ss BBBB"));
    assertEquals("hh:mm:ss BBBB | 01:00:00 | es", "01:00:00 de la madrugada",
        sdf.format(k010000, out.remove()));

    // #13215: for locales with keywords, check hang in DayPeriodRules""getInstance(const Locale, ...),
    // which is called in SimpleDateFormat::format for patterns that include 'B'.
    sdf = SimpleDateFormat(UnicodeString(), Locale("en@calendar=buddhist"), errorCode);
    sdf.setTimeZone(*tz);

    sdf.applyPattern(UnicodeString("hh:mm:ss BBBB"));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm:ss BBBB | 01:00:00 | en@calendar=buddhist", "01:00:00 in the morning",
        sdf.format(k010000, out.remove()));
#else
    assertEquals("hh:mm:ss BBBB | 01:00:00 | en@calendar=buddhist", "01:00:00 at night",
#endif // APPLE_ICU_CHANGES
}

void DateFormatTest::TestMinuteSecondFieldsInOddPlaces() {
    // Some times on 2015-11-13 (UTC+0).
    UDate k000000 = 1447372800000.0;
    UDate k000030 = 1447372830000.0;
    UDate k003000 = 1447374600000.0;
    UDate k060030 = 1447394430000.0;
    UDate k063000 = 1447396200000.0;

    UErrorCode errorCode = U_ZERO_ERROR;
    const TimeZone *tz = TimeZone::getGMT();
    UnicodeString out;

    // Note: "midnight" can be ambiguous as to whether it refers to beginning of day or end of day.
    // For ICU 57 output of "midnight" is temporarily suppressed.

    // Seconds field is not present.

    // Apply pattern through constructor to make sure parsePattern() is called during initialization.
    SimpleDateFormat sdf(UnicodeString("hh:mm 'ss' bbbb"), errorCode);
    if (U_FAILURE(errorCode)) {
        dataerrln("Error creating SimpleDateFormat - %s", u_errorName(errorCode));
        return;
    }
    sdf.setTimeZone(*tz);

    // assertEquals("hh:mm 'ss' bbbb | 00:00:30", "12:00 ss midnight",
    //     sdf.format(k000030, out.remove()));
    assertEquals("hh:mm 'ss' bbbb | 00:00:30", "12:00 ss AM",
        sdf.format(k000030, out.remove()));
    assertEquals("hh:mm 'ss' bbbb | 06:00:30", "06:00 ss AM",
        sdf.format(k060030, out.remove()));

    sdf.applyPattern(UnicodeString("hh:mm 'ss' BBBB"));

    // assertEquals("hh:mm 'ss' BBBB | 00:00:30", "12:00 ss midnight",
    //     sdf.format(k000030, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh:mm 'ss' BBBB | 00:00:30", "12:00 ss in the morning",
        sdf.format(k000030, out.remove()));
#else
    assertEquals("hh:mm 'ss' BBBB | 00:00:30", "12:00 ss at night",
        sdf.format(k000030, out.remove()));
#endif // APPLE_ICU_CHANGES
    assertEquals("hh:mm 'ss' BBBB | 06:00:30", "06:00 ss in the morning",
        sdf.format(k060030, out.remove()));

    // Minutes field is not present.
    sdf.applyPattern(UnicodeString("hh 'mm ss' bbbb"));

    // assertEquals("hh 'mm ss' bbbb | 00:30:00", "12 mm ss midnight",
    //     sdf.format(k003000, out.remove()));
    assertEquals("hh 'mm ss' bbbb | 00:30:00", "12 mm ss AM",
        sdf.format(k003000, out.remove()));
    assertEquals("hh 'mm ss' bbbb | 06:30:00", "06 mm ss AM",
        sdf.format(k063000, out.remove()));

    sdf.applyPattern(UnicodeString("hh 'mm ss' BBBB"));

    // assertEquals("hh 'mm ss' BBBB | 00:30:00", "12 mm ss midnight",
    //     sdf.format(k003000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("hh 'mm ss' BBBB | 00:30:00", "12 mm ss in the morning",
        sdf.format(k003000, out.remove()));
#else
    assertEquals("hh 'mm ss' BBBB | 00:30:00", "12 mm ss at night",
        sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES
    assertEquals("hh 'mm ss' BBBB | 06:30:00", "06 mm ss in the morning",
        sdf.format(k063000, out.remove()));

    // Minutes and seconds fields appear after day periods.
    sdf.applyPattern(UnicodeString("bbbb hh:mm:ss"));

    // assertEquals("bbbb hh:mm:ss | 00:00:00", "midnight 12:00:00",
    //     sdf.format(k000000, out.remove()));
    assertEquals("bbbb hh:mm:ss | 00:00:00", "AM 12:00:00",
        sdf.format(k000000, out.remove()));
    assertEquals("bbbb hh:mm:ss | 00:00:30", "AM 12:00:30",
        sdf.format(k000030, out.remove()));
    assertEquals("bbbb hh:mm:ss | 00:30:00", "AM 12:30:00",
        sdf.format(k003000, out.remove()));

    sdf.applyPattern(UnicodeString("BBBB hh:mm:ss"));

    // assertEquals("BBBB hh:mm:ss | 00:00:00", "midnight 12:00:00",
    //     sdf.format(k000000, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("BBBB hh:mm:ss | 00:00:00", "in the morning 12:00:00",
        sdf.format(k000000, out.remove()));
    assertEquals("BBBB hh:mm:ss | 00:00:30", "in the morning 12:00:30",
        sdf.format(k000030, out.remove()));
    assertEquals("BBBB hh:mm:ss | 00:30:00", "in the morning 12:30:00",
        sdf.format(k003000, out.remove()));
#else
    assertEquals("BBBB hh:mm:ss | 00:00:00", "at night 12:00:00",
        sdf.format(k000000, out.remove()));
    assertEquals("BBBB hh:mm:ss | 00:00:30", "at night 12:00:30",
        sdf.format(k000030, out.remove()));
    assertEquals("BBBB hh:mm:ss | 00:30:00", "at night 12:30:00",
        sdf.format(k003000, out.remove()));
#endif // APPLE_ICU_CHANGES

    // Confirm applyPattern() reparses the pattern string.
    sdf.applyPattern(UnicodeString("BBBB hh"));
    // assertEquals("BBBB hh | 00:00:30", "midnight 12",
    //     sdf.format(k000030, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("BBBB hh | 00:00:30", "in the morning 12",
         sdf.format(k000030, out.remove()));
#else
    assertEquals("BBBB hh | 00:00:30", "at night 12",
         sdf.format(k000030, out.remove()));
#endif // APPLE_ICU_CHANGES
        
    sdf.applyPattern(UnicodeString("BBBB hh:mm:'ss'"));
    // assertEquals("BBBB hh:mm:'ss' | 00:00:30", "midnight 12:00:ss",
    //     sdf.format(k000030, out.remove()));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("BBBB hh | 00:00:30", "in the morning 12:00:ss",
        sdf.format(k000030, out.remove()));
#else
    assertEquals("BBBB hh | 00:00:30", "at night 12:00:ss",
        sdf.format(k000030, out.remove()));
#endif // APPLE_ICU_CHANGES

    sdf.applyPattern(UnicodeString("BBBB hh:mm:ss"));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("BBBB hh:mm:ss | 00:00:30", "in the morning 12:00:30",
        sdf.format(k000030, out.remove()));
#else
    assertEquals("BBBB hh:mm:ss | 00:00:30", "at night 12:00:30",
        sdf.format(k000030, out.remove()));
#endif // APPLE_ICU_CHANGES
}

void DateFormatTest::TestDayPeriodParsing() {
    // Some times on 2015-11-13 (UTC+0).
    UDate k000000 = 1447372800000.0;
    UDate k003700 = 1447375020000.0;
    UDate k010000 = 1447376400000.0;
    UDate k013000 = 1447378200000.0;
    UDate k030000 = 1447383600000.0;
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    UDate k060000 = 1447394400000.0;
#endif // APPLE_ICU_CHANGES
    UDate k090000 = 1447405200000.0;
    UDate k120000 = 1447416000000.0;
    UDate k130000 = 1447419600000.0;
    UDate k133700 = 1447421820000.0;
    UDate k150000 = 1447426800000.0;
    UDate k190000 = 1447441200000.0;
    UDate k193000 = 1447443000000.0;
    UDate k200000 = 1447444800000.0;
    UDate k210000 = 1447448400000.0;

    UErrorCode errorCode = U_ZERO_ERROR;
    SimpleDateFormat sdf(UnicodeString(), errorCode);
    if (U_FAILURE(errorCode)) {
        dataerrln("Error creating SimpleDateFormat - %s", u_errorName(errorCode));
        return;
    }
    const TimeZone *tz = TimeZone::getGMT();
    sdf.setTimeZone(*tz);
    UnicodeString out;

    // 'B' -- flexible day periods
    // A day period on its own parses to the center of that period.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd B"));
    assertEquals("yyyy-MM-dd B | 2015-11-13 midnight",
        k000000, sdf.parse(UnicodeString("2015-11-13 midnight"), errorCode));
    assertEquals("yyyy-MM-dd B | 2015-11-13 noon",
        k120000, sdf.parse(UnicodeString("2015-11-13 noon"), errorCode));
    assertEquals("yyyy-MM-dd B | 2015-11-13 in the afternoon",
        k150000, sdf.parse(UnicodeString("2015-11-13 in the afternoon"), errorCode));
    assertEquals("yyyy-MM-dd B | 2015-11-13 in the evening",
        k193000, sdf.parse(UnicodeString("2015-11-13 in the evening"), errorCode));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("yyyy-MM-dd B | 2015-11-13 in the morning",
        k060000, sdf.parse(UnicodeString("2015-11-13 in the morning"), errorCode));
#else
    assertEquals("yyyy-MM-dd B | 2015-11-13 at night",
        k013000, sdf.parse(UnicodeString("2015-11-13 at night"), errorCode));
#endif // APPLE_ICU_CHANGES

    // If time and day period are consistent with each other then time is parsed accordingly.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd hh:mm B"));
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 12:00 midnight",
        k000000, sdf.parse(UnicodeString("2015-11-13 12:00 midnight"), errorCode));
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 12:00 noon",
        k120000, sdf.parse(UnicodeString("2015-11-13 12:00 noon"), errorCode));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 01:00 in the morning",
        k010000, sdf.parse(UnicodeString("2015-11-13 01:00 in the morning"), errorCode));
#else
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 01:00 at night",
        k010000, sdf.parse(UnicodeString("2015-11-13 01:00 at night"), errorCode));
#endif // APPLE_ICU_CHANGES
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 01:00 in the afternoon",
        k130000, sdf.parse(UnicodeString("2015-11-13 01:00 in the afternoon"), errorCode));
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 09:00 in the morning",
        k090000, sdf.parse(UnicodeString("2015-11-13 09:00 in the morning"), errorCode));
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 09:00 at night",
        k210000, sdf.parse(UnicodeString("2015-11-13 09:00 at night"), errorCode));

    // If the hour is 13 thru 23 then day period has no effect on time (since time is assumed
    // to be in 24-hour format).
    sdf.applyPattern(UnicodeString("yyyy-MM-dd HH:mm B"));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 13:37 midnight",
        k133700, sdf.parse(UnicodeString("2015-11-13 13:37 midnight"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 13:37 noon",
        k133700, sdf.parse(UnicodeString("2015-11-13 13:37 noon"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 13:37 at night",
        k133700, sdf.parse(UnicodeString("2015-11-13 13:37 at night"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 13:37 in the afternoon",
        k133700, sdf.parse(UnicodeString("2015-11-13 13:37 in the afternoon"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 13:37 in the morning",
        k133700, sdf.parse(UnicodeString("2015-11-13 13:37 in the morning"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 13:37 at night",
        k133700, sdf.parse(UnicodeString("2015-11-13 13:37 at night"), errorCode));

    // Hour 0 is synonymous with hour 12 when parsed with 'h'.
    // This unfortunately means we have to tolerate "0 noon" as it's synonymous with "12 noon".
    sdf.applyPattern(UnicodeString("yyyy-MM-dd hh:mm B"));
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 00:00 midnight",
        k000000, sdf.parse(UnicodeString("2015-11-13 00:00 midnight"), errorCode));
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 00:00 noon",
        k120000, sdf.parse(UnicodeString("2015-11-13 00:00 noon"), errorCode));

    // But when parsed with 'H', 0 indicates a 24-hour time, therefore we disregard the day period.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd HH:mm B"));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 00:37 midnight",
        k003700, sdf.parse(UnicodeString("2015-11-13 00:37 midnight"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 00:37 noon",
        k003700, sdf.parse(UnicodeString("2015-11-13 00:37 noon"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 00:37 at night",
        k003700, sdf.parse(UnicodeString("2015-11-13 00:37 at night"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 00:37 in the afternoon",
        k003700, sdf.parse(UnicodeString("2015-11-13 00:37 in the afternoon"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 00:37 in the morning",
        k003700, sdf.parse(UnicodeString("2015-11-13 00:37 in the morning"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 00:37 at night",
        k003700, sdf.parse(UnicodeString("2015-11-13 00:37 at night"), errorCode));

    // Even when parsed with 'H', hours 1 thru 12 are considered 12-hour time and takes
    // day period into account in parsing.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd HH:mm B"));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 12:00 midnight",
        k000000, sdf.parse(UnicodeString("2015-11-13 12:00 midnight"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 12:00 noon",
        k120000, sdf.parse(UnicodeString("2015-11-13 12:00 noon"), errorCode));
#if APPLE_ICU_CHANGES
// rdar://124449739 (D74/22A223: Sleep Alarm shows Tomorrow Night where I would expect to see Tomorrow Morning)
// (also see https://unicode-org.atlassian.net/browse/CLDR-17484 English day periods are wrong)
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 01:00 in the morning",
        k010000, sdf.parse(UnicodeString("2015-11-13 01:00 in the morning"), errorCode));
#else
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 01:00 at night",
        k010000, sdf.parse(UnicodeString("2015-11-13 01:00 at night"), errorCode));
#endif // APPLE_ICU_CHANGES
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 01:00 in the afternoon",
        k130000, sdf.parse(UnicodeString("2015-11-13 01:00 in the afternoon"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 09:00 in the morning",
        k090000, sdf.parse(UnicodeString("2015-11-13 09:00 in the morning"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm B | 2015-11-13 09:00 at night",
        k210000, sdf.parse(UnicodeString("2015-11-13 09:00 at night"), errorCode));

    // If a 12-hour time and the day period don't agree with each other, time is parsed as close
    // to the given day period as possible.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd hh:mm B"));

    // AFTERNOON1 is [12, 18), but "7 in the afternoon" parses to 19:00.
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 07:00 in the afternoon",
        k190000, sdf.parse(UnicodeString("2015-11-13 07:00 in the afternoon"), errorCode));
    // NIGHT1 is [21, 6), but "8 at night" parses to 20:00.
    assertEquals("yyyy-MM-dd hh:mm B | 2015-11-13 08:00 at night",
        k200000, sdf.parse(UnicodeString("2015-11-13 08:00 at night"), errorCode));

    // 'b' -- fixed day periods (AM, PM, midnight, noon)
    // On their own, "midnight" parses to 00:00 and "noon" parses to 12:00.
    // AM and PM are handled by the 'a' parser (which doesn't handle this case well).
    sdf.applyPattern(UnicodeString("yyyy-MM-dd b"));
    assertEquals("yyyy-MM-dd b | 2015-11-13 midnight",
        k000000, sdf.parse(UnicodeString("2015-11-13 midnight"), errorCode));
    assertEquals("yyyy-MM-dd b | 2015-11-13 noon",
        k120000, sdf.parse(UnicodeString("2015-11-13 noon"), errorCode));

    // For 12-hour times, AM and PM should be parsed as if with pattern character 'a'.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd hh:mm b"));
    assertEquals("yyyy-MM-dd hh:mm b | 2015-11-13 01:00 AM",
        k010000, sdf.parse(UnicodeString("2015-11-13 01:00 AM"), errorCode));
    assertEquals("yyyy-MM-dd hh:mm b | 2015-11-13 01:00 PM",
        k130000, sdf.parse(UnicodeString("2015-11-13 01:00 PM"), errorCode));

    // 12 midnight parses to 00:00, and 12 noon parses to 12:00.
    assertEquals("yyyy-MM-dd hh:mm b | 2015-11-13 12:00 midnight",
        k000000, sdf.parse(UnicodeString("2015-11-13 12:00 midnight"), errorCode));
    assertEquals("yyyy-MM-dd hh:mm b | 2015-11-13 12:00 noon",
        k120000, sdf.parse(UnicodeString("2015-11-13 12:00 noon"), errorCode));

    // Hours 13-23 indicate 24-hour time so we disregard "midnight" or "noon".
    // Again, AM and PM are handled by the 'a' parser which doesn't handle this case well.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd HH:mm b"));
    assertEquals("yyyy-MM-dd HH:mm b | 2015-11-13 13:37 midnight",
        k133700, sdf.parse(UnicodeString("2015-11-13 13:37 midnight"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm b | 2015-11-13 13:37 noon",
        k133700, sdf.parse(UnicodeString("2015-11-13 13:37 noon"), errorCode));

    // Hour 0 is synonymous with hour 12 when parsed with 'h'.
    // Again, this means we have to tolerate "0 noon" as it's synonymous with "12 noon".
    sdf.applyPattern(UnicodeString("yyyy-MM-dd hh:mm b"));
    assertEquals("yyyy-MM-dd hh:mm b | 2015-11-13 00:00 midnight",
        k000000, sdf.parse(UnicodeString("2015-11-13 00:00 midnight"), errorCode));
    assertEquals("yyyy-MM-dd hh:mm b | 2015-11-13 00:00 noon",
        k120000, sdf.parse(UnicodeString("2015-11-13 00:00 noon"), errorCode));

    // With 'H' though 0 indicates a 24-hour time, therefore we disregard the day period.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd HH:mm b"));
    assertEquals("yyyy-MM-dd HH:mm b | 2015-11-13 00:37 midnight",
        k003700, sdf.parse(UnicodeString("2015-11-13 00:37 midnight"), errorCode));
    assertEquals("yyyy-MM-dd HH:mm b | 2015-11-13 00:37 noon",
        k003700, sdf.parse(UnicodeString("2015-11-13 00:37 noon"), errorCode));

    // If "midnight" or "noon" is parsed with a 12-hour time other than 12:00, choose
    // the version that's closer to the period given.
    sdf.applyPattern(UnicodeString("yyyy-MM-dd hh:mm b"));
    assertEquals("yyyy-MM-dd hh:mm b | 2015-11-13 03:00 midnight",
        k030000, sdf.parse(UnicodeString("2015-11-13 03:00 midnight"), errorCode));
    assertEquals("yyyy-MM-dd hh:mm b | 2015-11-13 03:00 noon",
        k150000, sdf.parse(UnicodeString("2015-11-13 03:00 noon"), errorCode));
}

void DateFormatTest::TestParseRegression13744() {
    LocalPointer<DateFormat> dfmt(DateFormat::createDateTimeInstance(
            DateFormat::SHORT, DateFormat::SHORT, Locale("en", "US")));
    if (dfmt.isNull()) {
        dataerrln("DateFormat::createDateTimeInstance() failed");
        return;
    }
    ParsePosition pos(0);
    UnicodeString inDate("4/27/18");
    dfmt->parse(inDate, pos);
    assertEquals("Error index", inDate.length(), pos.getErrorIndex());
}

void DateFormatTest::TestAdoptCalendarLeak() {
    UErrorCode status = U_ZERO_ERROR;
    // This test relies on the locale fullName exceeding ULOC_FULLNAME_CAPACITY
    // in order for setKeywordValue to fail.
    Calendar* cal = Calendar::createInstance(status);
    ASSERT_OK(status);
    SimpleDateFormat sdf(
        "d.M.y",
        Locale("de__POSIX@colstrength=primary;currency=eur;em=default;"
               "hours=h23;lb=strict;lw=normal;measure=metric;numbers=latn;"
               "rg=atzzzz;sd=atat1;ss=none;timezone=Europe/Vienna"),
        status);
    // ASSERT_OK(status); Please do NOT add ASSERT_OK here. The point of this
    // test is to ensure sdf.adoptCalendar won't leak AFTER the above FAILED.
    // If the following caused crash we should fix the implementation not change
    // this test.
    sdf.adoptCalendar(cal);
}

/**
 * Test that 'a' and 'B' fields are not duplicated in the field position iterator.
 */
void DateFormatTest::Test20741_ABFields() {
    IcuTestErrorCode status(*this, "Test20741_ABFields");

    const char16_t timeZone[] = u"PST8PDT";

    UnicodeString skeletons[] = {u"EEEEEBBBBB", u"EEEEEbbbbb"};

    for (int32_t j = 0; j < 2; j++) {
        UnicodeString skeleton = skeletons[j];

        int32_t count = 0;
        const Locale* locales = Locale::getAvailableLocales(count);
        for (int32_t i = 0; i < count; i++) {
            if (quick && (i % 17) != 0) { continue; }

            const Locale locale = locales[i];
            LocalPointer<DateTimePatternGenerator> gen(DateTimePatternGenerator::createInstance(locale, status));
            UnicodeString pattern = gen->getBestPattern(skeleton, status);

            SimpleDateFormat dateFormat(pattern, locale, status);
            FieldPositionIterator fpositer;
            UnicodeString result;
            LocalPointer<Calendar> calendar(Calendar::createInstance(TimeZone::createTimeZone(timeZone), status));
            calendar->setTime(static_cast<UDate>(0), status);
            dateFormat.format(*calendar, result, &fpositer, status);

            FieldPosition curFieldPosition;
            FieldPosition lastFieldPosition;
            lastFieldPosition.setBeginIndex(-1);
            lastFieldPosition.setEndIndex(-1);
            while(fpositer.next(curFieldPosition)) {
                assertFalse("Field missing on pattern", pattern.indexOf(PATTERN_CHARS[curFieldPosition.getField()]) == -1);
                if (curFieldPosition.getBeginIndex() == lastFieldPosition.getBeginIndex() && curFieldPosition.getEndIndex() == lastFieldPosition.getEndIndex()) {
                    assertEquals("Different fields at same position", PATTERN_CHARS[curFieldPosition.getField()], PATTERN_CHARS[lastFieldPosition.getField()]);
                }

                lastFieldPosition = curFieldPosition;
            }
        }
    }
}

void DateFormatTest::Test22023_UTCWithMinusZero() {
    IcuTestErrorCode status(*this, "Test22023_UTCWithMinusZero");
    Locale locale("en");
    SimpleDateFormat fmt("h a", locale, status);
    ASSERT_OK(status);
    fmt.adoptCalendar(Calendar::createInstance(
        TimeZone::createTimeZone("UTC"), locale, status));
    ASSERT_OK(status);
    FieldPositionIterator fp_iter;
    icu::UnicodeString formatted;
    // very small negative value in double cause it to be -0
    // internally and trigger the assertion and bug.
    fmt.format(-1e-9, formatted, &fp_iter, status);
    ASSERT_OK(status);
}

void DateFormatTest::TestNumericFieldStrictParse() {
    static const struct {
        const char*           localeID;
        const char16_t* const pattern;
        const char16_t* const text;
        int32_t               pos; // final parsed position
        UCalendarDateFields   field1;
        int32_t               value1;
        UCalendarDateFields   field2;
        int32_t               value2;
    } TESTDATA[] = {
        // Ticket #22337
        {"en_US", u"MM/dd/yyyy", u"1/1/2023", 8, UCAL_MONTH, UCAL_JANUARY, UCAL_DAY_OF_MONTH, 1},
        // Ticket #22259
        {"en_US", u"dd-MM-uuuu", u"1-01-2023", 9, UCAL_MONTH, UCAL_JANUARY, UCAL_DAY_OF_MONTH, 1},
        {"en_US", u"dd-MM-uuuu", u"01-01-223", 9, UCAL_DAY_OF_MONTH, 1, UCAL_EXTENDED_YEAR, 223},
    };
    for (size_t i = 0; i < UPRV_LENGTHOF(TESTDATA); i++) {
        UErrorCode status = U_ZERO_ERROR;
        char pbuf[64];
        char tbuf[64];

        Locale locale = Locale::createFromName(TESTDATA[i].localeID);
        LocalPointer<SimpleDateFormat> sdfmt(new SimpleDateFormat(UnicodeString(TESTDATA[i].pattern), locale, status));
        if (U_FAILURE(status)) {
            u_austrncpy(pbuf, TESTDATA[i].pattern, sizeof(pbuf));
            dataerrln("Fail in new SimpleDateFormat locale %s pattern %s: %s",
                        TESTDATA[i].localeID, pbuf, u_errorName(status));
            continue;
        }
        LocalPointer<Calendar> cal(Calendar::createInstance(*TimeZone::getGMT(), locale, status));
        if (U_FAILURE(status)) {
            dataerrln("Fail in Calendar::createInstance locale %s: %s",
                        TESTDATA[i].localeID, u_errorName(status));
            continue;
        }
        cal->clear();
        //cal->set(2023, 0, 1);
        ParsePosition ppos(0);
        sdfmt->setLenient(false);
        sdfmt->parse(UnicodeString(TESTDATA[i].text), *cal, ppos);

        u_austrncpy(pbuf, TESTDATA[i].pattern, sizeof(pbuf));
        u_austrncpy(tbuf, TESTDATA[i].text, sizeof(tbuf));
        if (ppos.getIndex() != TESTDATA[i].pos) {
            errln("SimpleDateFormat::parse locale %s pattern %s: expected pos %d, got %d, errIndex %d",
                        TESTDATA[i].localeID, pbuf, TESTDATA[i].pos, ppos.getIndex(), ppos.getErrorIndex());
            continue;
        }
        if (TESTDATA[i].field1 < UCAL_FIELD_COUNT) {
            int32_t value = cal->get(TESTDATA[i].field1, status);
            if (U_FAILURE(status)) {
                errln("Calendar::get locale %s pattern %s field %d: %s",
                        TESTDATA[i].localeID, pbuf, TESTDATA[i].field1, u_errorName(status));
            } else if (value != TESTDATA[i].value1) {
                errln("Calendar::get locale %s pattern %s field %d: expected value %d, got %d",
                        TESTDATA[i].localeID, pbuf, TESTDATA[i].field1, TESTDATA[i].value1, value);
           }
        }
        status = U_ZERO_ERROR;
        if (TESTDATA[i].field2 < UCAL_FIELD_COUNT) {
            int32_t value = cal->get(TESTDATA[i].field2, status);
            if (U_FAILURE(status)) {
                errln("Calendar::get locale %s pattern %s field %d: %s",
                        TESTDATA[i].localeID, pbuf, TESTDATA[i].field2, u_errorName(status));
            } else if (value != TESTDATA[i].value2) {
                errln("Calendar::get locale %s pattern %s field %d: expected value %d, got %d",
                        TESTDATA[i].localeID, pbuf, TESTDATA[i].field2, TESTDATA[i].value2, value);
           }
        }
    }
}

void DateFormatTest::TestHourCycle() {
    static const UDate date = -845601267742; // March 16, 1943 at 3:45 PM
    
    static const struct {
        const char* languageTag;
        UnicodeString expectedResult;
    } TEST_CASES[] = {
        // test some locales for which we have data
        { "en-us", u"Tuesday, March 16, 1943 at 3:45:32 PM" },
#if APPLE_ICU_CHANGES
// rdar://
        { "en-ca", u"Tuesday, March 16, 1943 at 3:45:32 PM" },
#else
        { "en-ca", u"Tuesday, March 16, 1943 at 3:45:32 p.m." },
#endif
        { "en-gb", u"Tuesday, 16 March 1943 at 15:45:32" },
        { "en-au", u"Tuesday, 16 March 1943 at 3:45:32 pm" },
        // test a couple locales for which we don't have specific locale files (we should still get the correct hour cycle)
        { "en-co", u"Tuesday, 16 March 1943 at 3:45:32 PM" },
        { "en-mx", u"Tuesday, 16 March 1943 at 3:45:32 p.m." },
        // test that the rg subtag does the right thing
#if APPLE_ICU_CHANGES
// rdar://112976115 (SEED: 21A5291h/iPhone13,3: Date format incorrect with en_US@rg=dkzzzz)
// IMPORTANT NOTE: Apple ICU *changes* the semantics of the @rg subtag from what open-source ICU does, and we did it
// in response to the above Radar.  Open-source ICU follows the locale's region code when determining the formatting
// pattern to use; Apple ICU follows the rg subtag.  Both use the rg subtag to determine the hour cycle for time formats
// and the locale's region code to determine the symbols to use (e.g., the AM/PM strings).  The LDML spec is actually
// rather vague as to which things should be controlled by the rg subtag.  We may be doing the wrong thing here,
// but probably can't without breaking rdar://112976115.  This needs more thought! --rtg 1/3/24
        { "en-us-u-rg-gbzzzz", u"Tuesday, 16 March 1943 at 15:45:32" },
        { "en-us-u-rg-cazzzz", u"Tuesday, March 16, 1943 at 3:45:32 PM" },
        { "en-ca-u-rg-uszzzz", u"Tuesday, March 16, 1943 at 3:45:32 PM" },
        { "en-gb-u-rg-uszzzz", u"Tuesday, March 16, 1943 at 3:45:32\u202Fpm" }, // rdar://123446235
        { "en-gb-u-rg-cazzzz", u"Tuesday, March 16, 1943 at 3:45:32\u202Fpm" }, // rdar://123446235
        { "en-gb-u-rg-auzzzz", u"Tuesday, 16 March 1943 at 3:45:32 pm" }, // rdar://123446235
#else
        { "en-us-u-rg-gbzzzz", u"Tuesday, March 16, 1943 at 15:45:32" },
        { "en-us-u-rg-cazzzz", u"Tuesday, March 16, 1943 at 3:45:32 PM" },
        { "en-ca-u-rg-uszzzz", u"Tuesday, March 16, 1943 at 3:45:32 p.m." },
        { "en-gb-u-rg-uszzzz", u"Tuesday, 16 March 1943 at 3:45:32 pm" },
        { "en-gb-u-rg-cazzzz", u"Tuesday, 16 March 1943 at 3:45:32 pm" },
        { "en-gb-u-rg-auzzzz", u"Tuesday, 16 March 1943 at 3:45:32 pm" },
#endif // APPLE_ICU_CHANGES
        // test that the hc ("hours") subtag does the right thing
        { "en-us-u-hc-h23", u"Tuesday, March 16, 1943 at 15:45:32" },
        { "en-gb-u-hc-h12", u"Tuesday, 16 March 1943 at 3:45:32 pm" },
        // test that the rg and hc subtags do the right thing when used together
#if APPLE_ICU_CHANGES
// rdar://112976115 (SEED: 21A5291h/iPhone13,3: Date format incorrect with en_US@rg=dkzzzz)
// (See block comment above)
        { "en-us-u-rg-gbzzzz-hc-h12", u"Tuesday, 16 March 1943 at 3:45:32\u202FPM" },
        { "en-gb-u-rg-uszzzz-hc-h23", u"Tuesday, March 16, 1943 at 15:45:32" },
#else
        { "en-us-u-rg-gbzzzz-hc-h12", u"Tuesday, March 16, 1943 at 3:45:32 PM" },
        { "en-gb-u-rg-uszzzz-hc-h23", u"Tuesday, 16 March 1943 at 15:45:32" },
#endif // APPLE_ICU_CHANGES
    };
    
    for (int32_t i = 0; i < UPRV_LENGTHOF(TEST_CASES); i++) {
        UErrorCode err = U_ZERO_ERROR;
        Locale locale = Locale::forLanguageTag(TEST_CASES[i].languageTag, err);
        LocalPointer<DateFormat> df(DateFormat::createDateTimeInstance(DateFormat::FULL, DateFormat::MEDIUM, locale));
        df->adoptTimeZone(TimeZone::createTimeZone(u"America/Los_Angeles"));
        
        UnicodeString actualResult;
        FieldPosition fp;
        df->format(date, actualResult, fp);
        
        err = U_ZERO_ERROR; // throw away result from Locale::forLangageTag()-- if that fails, it's a coding errir in this test
        CharString errorMessage;
        errorMessage.append("Wrong result for ", err);
        errorMessage.append(TEST_CASES[i].languageTag, err);
        assertEquals(errorMessage.data(), TEST_CASES[i].expectedResult, actualResult);
    }
}

void DateFormatTest::TestBogusLocale() {
    IcuTestErrorCode status(*this, "TestBogusLocale");
    LocalPointer<DateFormat> df;

    df.adoptInstead(DateFormat::createDateTimeInstance(DateFormat::kNone, DateFormat::kMedium,
                    Locale("notalanguage")));
}

void DateFormatTest::TestLongLocale() {
    IcuTestErrorCode status(*this, "TestLongLocale");
    LocalPointer<DateFormat> df;

    // This should not cause a crash
    std::string s(1023, ' ');
    s[1] = '-';
    df.adoptInstead(DateFormat::createDateTimeInstance(DateFormat::kDateTime, DateFormat::kMedium,
                    Locale(s.c_str())));
}

void DateFormatTest::TestHCInLocale() {
    IcuTestErrorCode status(*this, "TestHCInLocale");
    LocalPointer<Calendar> midnight(Calendar::createInstance(status));
    midnight->set(2020, 0, 1, 0, 0);

    LocalPointer<Calendar> noon(Calendar::createInstance(status));
    noon->set(2020, 0, 1, 12, 0);

    bool expected[][3] = {
        // midnightContains12  midnightContains24  noonContains12
        {  false,              false,              false},         // "en-u-hc-h11"
        {  true,               false,              true},          // "en-u-hc-h12"
        {  false,              false,              true},          // "en-u-hc-h23"
        {  false,              true,               true},          // "en-u-hc-h24"
    };
    Locale locales[] = {"en-u-hc-h11", "en-u-hc-h12", "en-u-hc-h23", "en-u-hc-h24"};
    int i = 0;
    for (Locale locale : locales) {
        for (DateFormat::EStyle style : {DateFormat::kFull, DateFormat::kLong, DateFormat::kMedium, DateFormat::kShort}) {
            LocalPointer<DateFormat> timeFormat(DateFormat::createTimeInstance(style, locale));
            LocalPointer<DateFormat> dateTimeFormat(DateFormat::createDateTimeInstance(style, style, locale));
            UnicodeString actualMidnightTime, actualMidnightDateTime, actualNoonTime, actualNoonDateTime;
            actualMidnightTime = timeFormat->format(*midnight, actualMidnightTime, nullptr, status);
            actualNoonTime = timeFormat->format(*noon, actualNoonTime, nullptr, status);
            actualMidnightDateTime = dateTimeFormat->format(*midnight, actualMidnightDateTime, nullptr, status);
            actualNoonDateTime = dateTimeFormat->format(*noon, actualNoonDateTime, nullptr, status);

            bool midnightContains12 = expected[i][0];
            bool midnightContains24 = expected[i][1];
            bool noonContains12 = expected[i][2];

            assertEquals("Midnight contains '12:'?", midnightContains12, actualMidnightTime.indexOf("12:") >= 0);
            assertEquals("Midnight contains '12:'?", midnightContains12, actualMidnightDateTime.indexOf("12:") >= 0);

            assertEquals("Midnight contains '24:'?", midnightContains24, actualMidnightTime.indexOf("24:") >= 0);
            assertEquals("Midnight contains '24:'?", midnightContains24, actualMidnightDateTime.indexOf("24:") >= 0);

            assertEquals("Noon contains '12:'?", noonContains12, actualNoonTime.indexOf("12:") >= 0);
            assertEquals("Noon contains '12:'?", noonContains12, actualNoonDateTime.indexOf("12:") >= 0);

        }
        i++;
    }
}

#endif /* #if !UCONFIG_NO_FORMATTING */

//eof

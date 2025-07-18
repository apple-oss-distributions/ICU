// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/********************************************************************
 * COPYRIGHT: 
 * Copyright (c) 2004-2015, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/
/********************************************************************************
*
* File reapits.c
*
*********************************************************************************/
/*C API TEST FOR Regular Expressions */
/**
*   This is an API test for ICU regular expressions in C.  It doesn't test very many cases, and doesn't
*   try to test the full functionality.  It just calls each function and verifies that it
*   works on a basic level.
*
*   More complete testing of regular expression functionality is done with the C++ tests.
**/

#include "unicode/utypes.h"

#if !UCONFIG_NO_REGULAR_EXPRESSIONS

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "unicode/uloc.h"
#include "unicode/uregex.h"
#include "unicode/ustring.h"
#include "unicode/utext.h"
#include "unicode/utf8.h"
#include "cintltst.h"
#include "cmemory.h"

#define TEST_ASSERT_SUCCESS(status) UPRV_BLOCK_MACRO_BEGIN { \
    if (U_FAILURE(status)) { \
        log_data_err("Failure at file %s:%d - error = %s (Are you missing data?)\n", __FILE__, __LINE__, u_errorName(status)); \
    } \
} UPRV_BLOCK_MACRO_END

#define TEST_ASSERT(expr) UPRV_BLOCK_MACRO_BEGIN { \
    if ((expr)==false) { \
        log_err("Test Failure at file %s:%d - ASSERT(%s) failed.\n", __FILE__, __LINE__, #expr); \
    } \
} UPRV_BLOCK_MACRO_END

/*
 *   TEST_SETUP and TEST_TEARDOWN
 *         macros to handle the boilerplate around setting up regex test cases.
 *         parameteres to setup:
 *              pattern:     The regex pattern, a (char *) null terminated C string.
 *              testString:  The string data, also a (char *) C string.
 *              flags:       Regex flags to set when compiling the pattern
 *
 *         Put arbitrary test code between SETUP and TEARDOWN.
 *         're" is the compiled, ready-to-go  regular expression.
 */
#define TEST_SETUP(pattern, testString, flags) UPRV_BLOCK_MACRO_BEGIN { \
    UChar   *srcString = NULL;  \
    status = U_ZERO_ERROR; \
    re = uregex_openC(pattern, flags, NULL, &status);  \
    TEST_ASSERT_SUCCESS(status);   \
    int32_t testStringLen = (int32_t)strlen(testString); \
    srcString = (UChar *)malloc( (testStringLen + 2) * sizeof(UChar) ); \
    u_uastrncpy(srcString, testString, testStringLen + 1); \
    uregex_setText(re, srcString, -1, &status); \
    TEST_ASSERT_SUCCESS(status);  \
    if (U_SUCCESS(status)) { \
        UPRV_BLOCK_MACRO_BEGIN {} UPRV_BLOCK_MACRO_END

#define TEST_TEARDOWN  \
    }  \
    TEST_ASSERT_SUCCESS(status);  \
    uregex_close(re);  \
    free(srcString);   \
} UPRV_BLOCK_MACRO_END


/**
 * @param expected utf-8 array of bytes to be expected
 */
static void test_assert_string(const char *expected, const UChar *actual, UBool nulTerm, const char *file, int line) {
     char     buf_inside_macro[120];
     int32_t  len = (int32_t)strlen(expected);
     UBool    success;
     if (nulTerm) {
         u_austrncpy(buf_inside_macro, (actual), len+1);
         buf_inside_macro[len+2] = 0;
         success = (strcmp((expected), buf_inside_macro) == 0);
     } else {
         u_austrncpy(buf_inside_macro, (actual), len);
         buf_inside_macro[len+1] = 0;
         success = (strncmp((expected), buf_inside_macro, len) == 0);
     }
     if (success == false) {
         log_err("Failure at file %s, line %d, expected \"%s\", got \"%s\"\n",
             file, line, (expected), buf_inside_macro);
     }
}

#define TEST_ASSERT_STRING(expected, actual, nulTerm) test_assert_string(expected, actual, nulTerm, __FILE__, __LINE__)
             

static UBool equals_utf8_utext(const char *utf8, UText *utext) {
    int32_t u8i = 0;
    UChar32 u8c = 0;
    UChar32 utc = 0;
    UBool   stringsEqual = true;
    utext_setNativeIndex(utext, 0);
    for (;;) {
        U8_NEXT_UNSAFE(utf8, u8i, u8c);
        utc = utext_next32(utext);
        if (u8c == 0 && utc == U_SENTINEL) {
            break;
        }
        if (u8c != utc || u8c == 0) {
            stringsEqual = false;
            break;
        }
    }
    return stringsEqual;
}


static void test_assert_utext(const char *expected, UText *actual, const char *file, int line) {
    utext_setNativeIndex(actual, 0);
    if (!equals_utf8_utext(expected, actual)) {
        UChar32 c;
        log_err("Failure at file %s, line %d, expected \"%s\", got \"", file, line, expected);
        c = utext_next32From(actual, 0);
        while (c != U_SENTINEL) {
            if (0x20<c && c <0x7e) {
                log_err("%c", c);
            } else {
                log_err("%#x", c);
            }
            c = UTEXT_NEXT32(actual);
        }
        log_err("\"\n");
    }
}

/*
 * TEST_ASSERT_UTEXT(const char *expected, const UText *actual)
 *     Note:  Expected is a UTF-8 encoded string, _not_ the system code page.
 */
#define TEST_ASSERT_UTEXT(expected, actual) test_assert_utext(expected, actual, __FILE__, __LINE__)

static UBool testUTextEqual(UText *uta, UText *utb) {
    UChar32 ca = 0;
    UChar32 cb = 0;
    utext_setNativeIndex(uta, 0);
    utext_setNativeIndex(utb, 0);
    do {
        ca = utext_next32(uta);
        cb = utext_next32(utb);
        if (ca != cb) {
            break;
        }
    } while (ca != U_SENTINEL);
    return ca == cb;
}

    


static void TestRegexCAPI(void);
static void TestBug4315(void);
static void TestUTextAPI(void);
static void TestRefreshInput(void);
static void TestBug8421(void);
static void TestBug10815(void);
#if APPLE_ICU_CHANGES
// rdar://
static void TestMatchStartLineWithEmptyText(void);
#endif  // APPLE_ICU_CHANGES

void addURegexTest(TestNode** root);

void addURegexTest(TestNode** root)
{
    addTest(root, &TestRegexCAPI, "regex/TestRegexCAPI");
    addTest(root, &TestBug4315,   "regex/TestBug4315");
    addTest(root, &TestUTextAPI,  "regex/TestUTextAPI");
    addTest(root, &TestRefreshInput, "regex/TestRefreshInput");
    addTest(root, &TestBug8421,   "regex/TestBug8421");
    addTest(root, &TestBug10815,   "regex/TestBug10815");
#if APPLE_ICU_CHANGES
// rdar://
    addTest(root, &TestMatchStartLineWithEmptyText,   "regex/TestMatchStartLineWithEmptyText");
#endif  // APPLE_ICU_CHANGES
}

/*
 * Call back function and context struct used for testing
 *    regular expression user callbacks.  This test is mostly the same as
 *   the corresponding C++ test in intltest. 
 */
typedef struct callBackContext {
    int32_t          maxCalls;
    int32_t          numCalls;
    int32_t          lastSteps;
} callBackContext;

static UBool U_EXPORT2 U_CALLCONV
TestCallbackFn(const void *context, int32_t steps) {
  callBackContext  *info = (callBackContext *)context;
  if (info->lastSteps+1 != steps) {
      log_err("incorrect steps in callback.  Expected %d, got %d\n", info->lastSteps+1, steps);
  }
  info->lastSteps = steps;
  info->numCalls++;
  return (info->numCalls < info->maxCalls);
}

/*
 *   Regular Expression C API Tests
 */
static void TestRegexCAPI(void) {
    UErrorCode           status = U_ZERO_ERROR;
    URegularExpression  *re;
    UChar                pat[200];
    UChar               *minus1;

    memset(&minus1, -1, sizeof(minus1));

    /* Mimimalist open/close */
    u_uastrncpy(pat, "abc*", UPRV_LENGTHOF(pat));
    re = uregex_open(pat, -1, 0, 0, &status);
    if (U_FAILURE(status)) {
         log_data_err("Failed to open regular expression, %s:%d, error is \"%s\" (Are you missing data?)\n", __FILE__, __LINE__, u_errorName(status));
         return;
    }
    uregex_close(re);

    /* Open with all flag values set */
    status = U_ZERO_ERROR;
    re = uregex_open(pat, -1, 
        UREGEX_CASE_INSENSITIVE | UREGEX_COMMENTS | UREGEX_DOTALL | UREGEX_MULTILINE | UREGEX_UWORD | UREGEX_LITERAL,
        0, &status);
    TEST_ASSERT_SUCCESS(status);
    uregex_close(re);

    /* Open with an invalid flag */
    status = U_ZERO_ERROR;
    re = uregex_open(pat, -1, 0x40000000, 0, &status);
    TEST_ASSERT(status == U_REGEX_INVALID_FLAG);
    uregex_close(re);

    /* Open with an unimplemented flag */
    status = U_ZERO_ERROR;
    re = uregex_open(pat, -1, UREGEX_CANON_EQ, 0, &status);
    TEST_ASSERT(status == U_REGEX_UNIMPLEMENTED);
    uregex_close(re);

    /* openC with an invalid parameter */
    status = U_ZERO_ERROR;
    re = uregex_openC(NULL,
        UREGEX_CASE_INSENSITIVE | UREGEX_COMMENTS | UREGEX_DOTALL | UREGEX_MULTILINE | UREGEX_UWORD, 0, &status);
    TEST_ASSERT(status == U_ILLEGAL_ARGUMENT_ERROR && re == NULL);

    /* openC with an invalid parameter */
    status = U_USELESS_COLLATOR_ERROR;
    re = uregex_openC(NULL,
        UREGEX_CASE_INSENSITIVE | UREGEX_COMMENTS | UREGEX_DOTALL | UREGEX_MULTILINE | UREGEX_UWORD, 0, &status);
    TEST_ASSERT(status == U_USELESS_COLLATOR_ERROR && re == NULL);

    /* openC   open from a C string */
    {
        const UChar   *p;
        int32_t  len;
        status = U_ZERO_ERROR;
        re = uregex_openC("abc*", 0, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        p = uregex_pattern(re, &len, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS above should change too... */
        if(U_SUCCESS(status)) {
            u_uastrncpy(pat, "abc*", UPRV_LENGTHOF(pat));
            TEST_ASSERT(u_strcmp(pat, p) == 0);
            TEST_ASSERT(len==(int32_t)strlen("abc*"));
        }

        uregex_close(re);

        /*  TODO:  Open with ParseError parameter */
    }

    /*
     *  clone
     */
    {
        URegularExpression *clone1;
        URegularExpression *clone2;
        URegularExpression *clone3;
        UChar  testString1[30];
        UChar  testString2[30];
        UBool  result;


        status = U_ZERO_ERROR;
        re = uregex_openC("abc*", 0, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        clone1 = uregex_clone(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(clone1 != NULL);

        status = U_ZERO_ERROR;
        clone2 = uregex_clone(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(clone2 != NULL);
        uregex_close(re);

        status = U_ZERO_ERROR;
        clone3 = uregex_clone(clone2, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(clone3 != NULL);

        u_uastrncpy(testString1, "abcccd", UPRV_LENGTHOF(pat));
        u_uastrncpy(testString2, "xxxabcccd", UPRV_LENGTHOF(pat));

        status = U_ZERO_ERROR;
        uregex_setText(clone1, testString1, -1, &status);
        TEST_ASSERT_SUCCESS(status);
        result = uregex_lookingAt(clone1, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(result==true);
        
        status = U_ZERO_ERROR;
        uregex_setText(clone2, testString2, -1, &status);
        TEST_ASSERT_SUCCESS(status);
        result = uregex_lookingAt(clone2, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(result==false);
        result = uregex_find(clone2, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(result==true);

        uregex_close(clone1);
        uregex_close(clone2);
        uregex_close(clone3);

    }

    /*
     *  pattern()
    */
    {
        const UChar  *resultPat;
        int32_t       resultLen;
        u_uastrncpy(pat, "hello", UPRV_LENGTHOF(pat));
        status = U_ZERO_ERROR;
        re = uregex_open(pat, -1, 0, NULL, &status);
        resultPat = uregex_pattern(re, &resultLen, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS above should change too... */
        if (U_SUCCESS(status)) {
            TEST_ASSERT(resultLen == -1);
            TEST_ASSERT(u_strcmp(resultPat, pat) == 0);
        }

        uregex_close(re);

        status = U_ZERO_ERROR;
        re = uregex_open(pat, 3, 0, NULL, &status);
        resultPat = uregex_pattern(re, &resultLen, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS above should change too... */
        if (U_SUCCESS(status)) {
            TEST_ASSERT(resultLen == 3);
            TEST_ASSERT(u_strncmp(resultPat, pat, 3) == 0);
            TEST_ASSERT(u_strlen(resultPat) == 3);
        }

        uregex_close(re);
    }

    /*
     *  flags()
     */
    {
        int32_t  t;

        status = U_ZERO_ERROR;
        re = uregex_open(pat, -1, 0, NULL, &status);
        t  = uregex_flags(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(t == 0);
        uregex_close(re);

        status = U_ZERO_ERROR;
        re = uregex_open(pat, -1, 0, NULL, &status);
        t  = uregex_flags(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(t == 0);
        uregex_close(re);

        status = U_ZERO_ERROR;
        re = uregex_open(pat, -1, UREGEX_CASE_INSENSITIVE | UREGEX_DOTALL, NULL, &status);
        t  = uregex_flags(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(t == (UREGEX_CASE_INSENSITIVE | UREGEX_DOTALL));
        uregex_close(re);
    }

    /*
     *  setText() and lookingAt()
     */
    {
        UChar  text1[50];
        UChar  text2[50];
        UBool  result;

        u_uastrncpy(text1, "abcccd",  UPRV_LENGTHOF(text1));
        u_uastrncpy(text2, "abcccxd", UPRV_LENGTHOF(text2));
        status = U_ZERO_ERROR;
        u_uastrncpy(pat, "abc*d", UPRV_LENGTHOF(pat));
        re = uregex_open(pat, -1, 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);

        /* Operation before doing a setText should fail... */
        status = U_ZERO_ERROR;
        uregex_lookingAt(re, 0, &status);
        TEST_ASSERT( status== U_REGEX_INVALID_STATE);

        status = U_ZERO_ERROR;
        uregex_setText(re, text1, -1, &status);
        result = uregex_lookingAt(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, text2, -1, &status);
        result = uregex_lookingAt(re, 0, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, text1, -1, &status);
        result = uregex_lookingAt(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, text1, 5, &status);
        result = uregex_lookingAt(re, 0, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, text1, 6, &status);
        result = uregex_lookingAt(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);

        uregex_close(re);
    }


    /*
     *  getText() 
     */
    {
        UChar    text1[50];
        UChar    text2[50];
        const UChar   *result;
        int32_t  textLength;

        u_uastrncpy(text1, "abcccd",  UPRV_LENGTHOF(text1));
        u_uastrncpy(text2, "abcccxd", UPRV_LENGTHOF(text2));
        status = U_ZERO_ERROR;
        u_uastrncpy(pat, "abc*d", UPRV_LENGTHOF(pat));
        re = uregex_open(pat, -1, 0, NULL, &status);

        uregex_setText(re, text1, -1, &status);
        result = uregex_getText(re, &textLength, &status);
        TEST_ASSERT(result == text1);
        TEST_ASSERT(textLength == -1);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, text2, 7, &status);
        result = uregex_getText(re, &textLength, &status);
        TEST_ASSERT(result == text2);
        TEST_ASSERT(textLength == 7);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, text2, 4, &status);
        result = uregex_getText(re, &textLength, &status);
        TEST_ASSERT(result == text2);
        TEST_ASSERT(textLength == 4);
        TEST_ASSERT_SUCCESS(status);
        uregex_close(re);
    }

    /*
     *  matches()
     */
    {
        UChar   text1[50];
        UBool   result;
        int     len;
        UChar   nullString[] = {0,0,0};

        u_uastrncpy(text1, "abcccde",  UPRV_LENGTHOF(text1));
        status = U_ZERO_ERROR;
        u_uastrncpy(pat, "abc*d", UPRV_LENGTHOF(pat));
        re = uregex_open(pat, -1, 0, NULL, &status);

        uregex_setText(re, text1, -1, &status);
        result = uregex_matches(re, 0, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, text1, 6, &status);
        result = uregex_matches(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, text1, 6, &status);
        result = uregex_matches(re, 1, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);
        uregex_close(re);

        status = U_ZERO_ERROR;
        re = uregex_openC(".?", 0, NULL, &status);
        uregex_setText(re, text1, -1, &status);
        len = u_strlen(text1);
        result = uregex_matches(re, len, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setText(re, nullString, -1, &status);
        TEST_ASSERT_SUCCESS(status);
        result = uregex_matches(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);
        uregex_close(re);
    }


    /*
     *  lookingAt()    Used in setText test.
     */


    /*
     *  find(), findNext, start, end, reset
     */
    {
        UChar    text1[50];
        UBool    result;
        u_uastrncpy(text1, "012rx5rx890rxrx...",  UPRV_LENGTHOF(text1));
        status = U_ZERO_ERROR;
        re = uregex_openC("rx", 0, NULL, &status);

        uregex_setText(re, text1, -1, &status);
        result = uregex_find(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 3);
        TEST_ASSERT(uregex_end(re, 0, &status) == 5);
        TEST_ASSERT_SUCCESS(status);

        result = uregex_find(re, 9, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 11);
        TEST_ASSERT(uregex_end(re, 0, &status) == 13);
        TEST_ASSERT_SUCCESS(status);

        result = uregex_find(re, 14, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_reset(re, 0, &status);

        result = uregex_findNext(re, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 3);
        TEST_ASSERT(uregex_end(re, 0, &status) == 5);
        TEST_ASSERT_SUCCESS(status);

        result = uregex_findNext(re, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 6);
        TEST_ASSERT(uregex_end(re, 0, &status) == 8);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_reset(re, 12, &status);

        result = uregex_findNext(re, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 13);
        TEST_ASSERT(uregex_end(re, 0, &status) == 15);
        TEST_ASSERT_SUCCESS(status);

        result = uregex_findNext(re, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);

        uregex_close(re);
    }

    /*
     *  groupCount
     */
    {
        int32_t result;

        status = U_ZERO_ERROR;
        re = uregex_openC("abc", 0, NULL, &status);
        result = uregex_groupCount(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(result == 0);
        uregex_close(re);

        status = U_ZERO_ERROR;
        re = uregex_openC("abc(def)(ghi(j))", 0, NULL, &status);
        result = uregex_groupCount(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(result == 3);
        uregex_close(re);

    }


    /*
     *  group()
     */
    {
        UChar    text1[80];
        UChar    buf[80];
        UBool    result;
        int32_t  resultSz;
        u_uastrncpy(text1, "noise abc interior def, and this is off the end",  UPRV_LENGTHOF(text1));

        status = U_ZERO_ERROR;
        re = uregex_openC("abc(.*?)def", 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);


        uregex_setText(re, text1, -1, &status);
        result = uregex_find(re, 0, &status);
        TEST_ASSERT(result==true);

        /*  Capture Group 0, the full match.  Should succeed.  */
        status = U_ZERO_ERROR;
        resultSz = uregex_group(re, 0, buf, UPRV_LENGTHOF(buf), &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING("abc interior def", buf, true);
        TEST_ASSERT(resultSz == (int32_t)strlen("abc interior def"));

        /*  Capture group #1.  Should succeed. */
        status = U_ZERO_ERROR;
        resultSz = uregex_group(re, 1, buf, UPRV_LENGTHOF(buf), &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING(" interior ", buf, true);
        TEST_ASSERT(resultSz == (int32_t)strlen(" interior "));

        /*  Capture group out of range.  Error. */
        status = U_ZERO_ERROR;
        uregex_group(re, 2, buf, UPRV_LENGTHOF(buf), &status);
        TEST_ASSERT(status == U_INDEX_OUTOFBOUNDS_ERROR);

        /* NULL buffer, pure pre-flight */
        status = U_ZERO_ERROR;
        resultSz = uregex_group(re, 0, NULL, 0, &status);
        TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);
        TEST_ASSERT(resultSz == (int32_t)strlen("abc interior def"));

        /* Too small buffer, truncated string */
        status = U_ZERO_ERROR;
        memset(buf, -1, sizeof(buf));
        resultSz = uregex_group(re, 0, buf, 5, &status);
        TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);
        TEST_ASSERT_STRING("abc i", buf, false);
        TEST_ASSERT(buf[5] == (UChar)0xffff);
        TEST_ASSERT(resultSz == (int32_t)strlen("abc interior def"));

        /* Output string just fits buffer, no NUL term. */
        status = U_ZERO_ERROR;
        resultSz = uregex_group(re, 0, buf, (int32_t)strlen("abc interior def"), &status);
        TEST_ASSERT(status == U_STRING_NOT_TERMINATED_WARNING);
        TEST_ASSERT_STRING("abc interior def", buf, false);
        TEST_ASSERT(resultSz == (int32_t)strlen("abc interior def"));
        TEST_ASSERT(buf[strlen("abc interior def")] == (UChar)0xffff);
        
        uregex_close(re);

    }
    
    /*
     *  Regions
     */
        
        
        /* SetRegion(), getRegion() do something  */
        TEST_SETUP(".*", "0123456789ABCDEF", 0);
        UChar resultString[40];
        TEST_ASSERT(uregex_regionStart(re, &status) == 0);
        TEST_ASSERT(uregex_regionEnd(re, &status) == 16);
        uregex_setRegion(re, 3, 6, &status);
        TEST_ASSERT(uregex_regionStart(re, &status) == 3);
        TEST_ASSERT(uregex_regionEnd(re, &status) == 6);
        TEST_ASSERT(uregex_findNext(re, &status));
        TEST_ASSERT(uregex_group(re, 0, resultString, UPRV_LENGTHOF(resultString), &status) == 3);
        TEST_ASSERT_STRING("345", resultString, true);
        TEST_TEARDOWN;
        
        /* find(start=-1) uses regions   */
        TEST_SETUP(".*", "0123456789ABCDEF", 0);
        uregex_setRegion(re, 4, 6, &status);
        TEST_ASSERT(uregex_find(re, -1, &status) == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 4);
        TEST_ASSERT(uregex_end(re, 0, &status) == 6);
        TEST_TEARDOWN;
        
        /* find (start >=0) does not use regions   */
        TEST_SETUP(".*", "0123456789ABCDEF", 0);
        uregex_setRegion(re, 4, 6, &status);
        TEST_ASSERT(uregex_find(re, 0, &status) == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 0);
        TEST_ASSERT(uregex_end(re, 0, &status) == 16);
        TEST_TEARDOWN;
         
        /* findNext() obeys regions    */
        TEST_SETUP(".", "0123456789ABCDEF", 0);
        uregex_setRegion(re, 4, 6, &status);
        TEST_ASSERT(uregex_findNext(re,&status) == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 4);
        TEST_ASSERT(uregex_findNext(re, &status) == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 5);
        TEST_ASSERT(uregex_findNext(re, &status) == false);
        TEST_TEARDOWN;

        /* matches(start=-1) uses regions                                           */
        /*    Also, verify that non-greedy *? succeeds in finding the full match.   */
        TEST_SETUP(".*?", "0123456789ABCDEF", 0);
        uregex_setRegion(re, 4, 6, &status);
        TEST_ASSERT(uregex_matches(re, -1, &status) == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 4);
        TEST_ASSERT(uregex_end(re, 0, &status) == 6);
        TEST_TEARDOWN;
        
        /* matches (start >=0) does not use regions       */
        TEST_SETUP(".*?", "0123456789ABCDEF", 0);
        uregex_setRegion(re, 4, 6, &status);
        TEST_ASSERT(uregex_matches(re, 0, &status) == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 0);
        TEST_ASSERT(uregex_end(re, 0, &status) == 16);
        TEST_TEARDOWN;
        
        /* lookingAt(start=-1) uses regions                                         */
        /*    Also, verify that non-greedy *? finds the first (shortest) match.     */
        TEST_SETUP(".*?", "0123456789ABCDEF", 0);
        uregex_setRegion(re, 4, 6, &status);
        TEST_ASSERT(uregex_lookingAt(re, -1, &status) == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 4);
        TEST_ASSERT(uregex_end(re, 0, &status) == 4);
        TEST_TEARDOWN;
        
        /* lookingAt (start >=0) does not use regions  */
        TEST_SETUP(".*?", "0123456789ABCDEF", 0);
        uregex_setRegion(re, 4, 6, &status);
        TEST_ASSERT(uregex_lookingAt(re, 0, &status) == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 0);
        TEST_ASSERT(uregex_end(re, 0, &status) == 0);
        TEST_TEARDOWN;

        /* hitEnd()       */
        TEST_SETUP("[a-f]*", "abcdefghij", 0);
        TEST_ASSERT(uregex_find(re, 0, &status) == true);
        TEST_ASSERT(uregex_hitEnd(re, &status) == false);
        TEST_TEARDOWN;

        TEST_SETUP("[a-f]*", "abcdef", 0);
        TEST_ASSERT(uregex_find(re, 0, &status) == true);
        TEST_ASSERT(uregex_hitEnd(re, &status) == true);
        TEST_TEARDOWN;

        /* requireEnd   */
        TEST_SETUP("abcd", "abcd", 0);
        TEST_ASSERT(uregex_find(re, 0, &status) == true);
        TEST_ASSERT(uregex_requireEnd(re, &status) == false);
        TEST_TEARDOWN;

        TEST_SETUP("abcd$", "abcd", 0);
        TEST_ASSERT(uregex_find(re, 0, &status) == true);
        TEST_ASSERT(uregex_requireEnd(re, &status) == true);
        TEST_TEARDOWN;
        
        /* anchoringBounds        */
        TEST_SETUP("abc$", "abcdef", 0);
        TEST_ASSERT(uregex_hasAnchoringBounds(re, &status) == true);
        uregex_useAnchoringBounds(re, false, &status);
        TEST_ASSERT(uregex_hasAnchoringBounds(re, &status) == false);
        
        TEST_ASSERT(uregex_find(re, -1, &status) == false);
        uregex_useAnchoringBounds(re, true, &status);
        uregex_setRegion(re, 0, 3, &status);
        TEST_ASSERT(uregex_find(re, -1, &status) == true);
        TEST_ASSERT(uregex_end(re, 0, &status) == 3);
        TEST_TEARDOWN;
        
        /* Transparent Bounds      */
        TEST_SETUP("abc(?=def)", "abcdef", 0);
        TEST_ASSERT(uregex_hasTransparentBounds(re, &status) == false);
        uregex_useTransparentBounds(re, true, &status);
        TEST_ASSERT(uregex_hasTransparentBounds(re, &status) == true);
        
        uregex_useTransparentBounds(re, false, &status);
        TEST_ASSERT(uregex_find(re, -1, &status) == true);    /* No Region */
        uregex_setRegion(re, 0, 3, &status);
        TEST_ASSERT(uregex_find(re, -1, &status) == false);   /* with region, opaque bounds */
        uregex_useTransparentBounds(re, true, &status);
        TEST_ASSERT(uregex_find(re, -1, &status) == true);    /* with region, transparent bounds */
        TEST_ASSERT(uregex_end(re, 0, &status) == 3);
        TEST_TEARDOWN;
        

    /*
     *  replaceFirst()
     */
    {
        UChar    text1[80];
        UChar    text2[80];
        UChar    replText[80];
        UChar    buf[80];
        int32_t  resultSz;
        u_uastrncpy(text1, "Replace xaax x1x x...x.",  UPRV_LENGTHOF(text1));
        u_uastrncpy(text2, "No match here.",  UPRV_LENGTHOF(text2));
        u_uastrncpy(replText, "<$1>", UPRV_LENGTHOF(replText));

        status = U_ZERO_ERROR;
        re = uregex_openC("x(.*?)x", 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);

        /*  Normal case, with match */
        uregex_setText(re, text1, -1, &status);
        resultSz = uregex_replaceFirst(re, replText, -1, buf, UPRV_LENGTHOF(buf), &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING("Replace <aa> x1x x...x.", buf, true);
        TEST_ASSERT(resultSz == (int32_t)strlen("Replace xaax x1x x...x."));

        /* No match.  Text should copy to output with no changes.  */
        status = U_ZERO_ERROR;
        uregex_setText(re, text2, -1, &status);
        resultSz = uregex_replaceFirst(re, replText, -1, buf, UPRV_LENGTHOF(buf), &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING("No match here.", buf, true);
        TEST_ASSERT(resultSz == (int32_t)strlen("No match here."));

        /*  Match, output just fills buffer, no termination warning. */
        status = U_ZERO_ERROR;
        uregex_setText(re, text1, -1, &status);
        memset(buf, -1, sizeof(buf));
        resultSz = uregex_replaceFirst(re, replText, -1, buf, (int32_t)strlen("Replace <aa> x1x x...x."), &status);
        TEST_ASSERT(status == U_STRING_NOT_TERMINATED_WARNING);
        TEST_ASSERT_STRING("Replace <aa> x1x x...x.", buf, false);
        TEST_ASSERT(resultSz == (int32_t)strlen("Replace xaax x1x x...x."));
        TEST_ASSERT(buf[resultSz] == (UChar)0xffff);

        /* Do the replaceFirst again, without first resetting anything.
         *  Should give the same results.
         */
        status = U_ZERO_ERROR;
        memset(buf, -1, sizeof(buf));
        resultSz = uregex_replaceFirst(re, replText, -1, buf, (int32_t)strlen("Replace <aa> x1x x...x."), &status);
        TEST_ASSERT(status == U_STRING_NOT_TERMINATED_WARNING);
        TEST_ASSERT_STRING("Replace <aa> x1x x...x.", buf, false);
        TEST_ASSERT(resultSz == (int32_t)strlen("Replace xaax x1x x...x."));
        TEST_ASSERT(buf[resultSz] == (UChar)0xffff);

        /* NULL buffer, zero buffer length */
        status = U_ZERO_ERROR;
        resultSz = uregex_replaceFirst(re, replText, -1, NULL, 0, &status);
        TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);
        TEST_ASSERT(resultSz == (int32_t)strlen("Replace xaax x1x x...x."));

        /* Buffer too small by one */
        status = U_ZERO_ERROR;
        memset(buf, -1, sizeof(buf));
        resultSz = uregex_replaceFirst(re, replText, -1, buf, (int32_t)strlen("Replace <aa> x1x x...x.")-1, &status);
        TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);
        TEST_ASSERT_STRING("Replace <aa> x1x x...x", buf, false);
        TEST_ASSERT(resultSz == (int32_t)strlen("Replace xaax x1x x...x."));
        TEST_ASSERT(buf[resultSz] == (UChar)0xffff);

        uregex_close(re);
    }


    /*
     *  replaceAll()
     */
    {
        UChar    text1[80];          /*  "Replace xaax x1x x...x." */
        UChar    text2[80];          /*  "No match Here"           */
        UChar    replText[80];       /*  "<$1>"                    */
        UChar    replText2[80];      /*  "<<$1>>"                  */
        const char * pattern = "x(.*?)x";
        const char * expectedResult = "Replace <aa> <1> <...>.";
        const char * expectedResult2 = "Replace <<aa>> <<1>> <<...>>.";
        UChar    buf[80];
        int32_t  resultSize;
        int32_t  expectedResultSize;
        int32_t  expectedResultSize2;
        int32_t  i;

        u_uastrncpy(text1, "Replace xaax x1x x...x.",  UPRV_LENGTHOF(text1));
        u_uastrncpy(text2, "No match here.",  UPRV_LENGTHOF(text2));
        u_uastrncpy(replText, "<$1>", UPRV_LENGTHOF(replText));
        u_uastrncpy(replText2, "<<$1>>", UPRV_LENGTHOF(replText2));
        expectedResultSize = (int32_t)strlen(expectedResult);
        expectedResultSize2 = (int32_t)strlen(expectedResult2);

        status = U_ZERO_ERROR;
        re = uregex_openC(pattern, 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);

        /*  Normal case, with match */
        uregex_setText(re, text1, -1, &status);
        resultSize = uregex_replaceAll(re, replText, -1, buf, UPRV_LENGTHOF(buf), &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING(expectedResult, buf, true);
        TEST_ASSERT(resultSize == expectedResultSize);

        /* No match.  Text should copy to output with no changes.  */
        status = U_ZERO_ERROR;
        uregex_setText(re, text2, -1, &status);
        resultSize = uregex_replaceAll(re, replText, -1, buf, UPRV_LENGTHOF(buf), &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING("No match here.", buf, true);
        TEST_ASSERT(resultSize == u_strlen(text2));

        /*  Match, output just fills buffer, no termination warning. */
        status = U_ZERO_ERROR;
        uregex_setText(re, text1, -1, &status);
        memset(buf, -1, sizeof(buf));
        resultSize = uregex_replaceAll(re, replText, -1, buf, expectedResultSize, &status);
        TEST_ASSERT(status == U_STRING_NOT_TERMINATED_WARNING);
        TEST_ASSERT_STRING(expectedResult, buf, false);
        TEST_ASSERT(resultSize == expectedResultSize);
        TEST_ASSERT(buf[resultSize] == (UChar)0xffff);

        /* Do the replaceFirst again, without first resetting anything.
         *  Should give the same results.
         */
        status = U_ZERO_ERROR;
        memset(buf, -1, sizeof(buf));
        resultSize = uregex_replaceAll(re, replText, -1, buf, (int32_t)strlen("Replace xaax x1x x...x."), &status);
        TEST_ASSERT(status == U_STRING_NOT_TERMINATED_WARNING);
        TEST_ASSERT_STRING("Replace <aa> <1> <...>.", buf, false);
        TEST_ASSERT(resultSize == (int32_t)strlen("Replace <aa> <1> <...>."));
        TEST_ASSERT(buf[resultSize] == (UChar)0xffff);

        /* NULL buffer, zero buffer length */
        status = U_ZERO_ERROR;
        resultSize = uregex_replaceAll(re, replText, -1, NULL, 0, &status);
        TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);
        TEST_ASSERT(resultSize == (int32_t)strlen("Replace <aa> <1> <...>."));

        /* Buffer too small.  Try every size, which will tickle edge cases
         * in uregex_appendReplacement (used by replaceAll)   */
        for (i=0; i<expectedResultSize; i++) {
            char  expected[80];
            status = U_ZERO_ERROR;
            memset(buf, -1, sizeof(buf));
            resultSize = uregex_replaceAll(re, replText, -1, buf, i, &status);
            TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);
            strcpy(expected, expectedResult);
            expected[i] = 0;
            TEST_ASSERT_STRING(expected, buf, false);
            TEST_ASSERT(resultSize == expectedResultSize);
            TEST_ASSERT(buf[i] == (UChar)0xffff);
        }

        /* Buffer too small.  Same as previous test, except this time the replacement
         * text is longer than the match capture group, making the length of the complete
         * replacement longer than the original string.
         */
        for (i=0; i<expectedResultSize2; i++) {
            char  expected[80];
            status = U_ZERO_ERROR;
            memset(buf, -1, sizeof(buf));
            resultSize = uregex_replaceAll(re, replText2, -1, buf, i, &status);
            TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);
            strcpy(expected, expectedResult2);
            expected[i] = 0;
            TEST_ASSERT_STRING(expected, buf, false);
            TEST_ASSERT(resultSize == expectedResultSize2);
            TEST_ASSERT(buf[i] == (UChar)0xffff);
        }


        uregex_close(re);
    }


    /*
     *  appendReplacement()
     */
    {
        UChar    text[100];
        UChar    repl[100];
        UChar    buf[100];
        UChar   *bufPtr;
        int32_t  bufCap;


        status = U_ZERO_ERROR;
        re = uregex_openC(".*", 0, 0, &status);
        TEST_ASSERT_SUCCESS(status);

        u_uastrncpy(text, "whatever",  UPRV_LENGTHOF(text));
        u_uastrncpy(repl, "some other", UPRV_LENGTHOF(repl));
        uregex_setText(re, text, -1, &status);

        /* match covers whole target string */
        uregex_find(re, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        bufPtr = buf;
        bufCap = UPRV_LENGTHOF(buf);
        uregex_appendReplacement(re, repl, -1, &bufPtr, &bufCap, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING("some other", buf, true);

        /* Match has \u \U escapes */
        uregex_find(re, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        bufPtr = buf;
        bufCap = UPRV_LENGTHOF(buf);
        u_uastrncpy(repl, "abc\\u0041\\U00000042 \\\\ \\$ \\abc", UPRV_LENGTHOF(repl));
        uregex_appendReplacement(re, repl, -1, &bufPtr, &bufCap, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING("abcAB \\ $ abc", buf, true); 

        /* Bug 6813, parameter check of NULL destCapacity; crashed before fix. */
        status = U_ZERO_ERROR;
        uregex_find(re, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        bufPtr = buf;
        status = U_BUFFER_OVERFLOW_ERROR;
        uregex_appendReplacement(re, repl, -1, &bufPtr, NULL, &status);
        TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);

        uregex_close(re);
    }


    /*
     *  appendTail().   Checked in ReplaceFirst(), replaceAll().
     */

    /*
     *  split()
     */
    {
        UChar    textToSplit[80];
        UChar    text2[80];
        UChar    buf[200];
        UChar    *fields[10];
        int32_t  numFields;
        int32_t  requiredCapacity;
        int32_t  spaceNeeded;
        int32_t  sz;

        u_uastrncpy(textToSplit, "first : second:  third",  UPRV_LENGTHOF(textToSplit));
        u_uastrncpy(text2, "No match here.",  UPRV_LENGTHOF(text2));

        status = U_ZERO_ERROR;
        re = uregex_openC(":", 0, NULL, &status);


        /*  Simple split */ 

        uregex_setText(re, textToSplit, -1, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if (U_SUCCESS(status)) {
            memset(fields, -1, sizeof(fields));
            numFields = 
                uregex_split(re, buf, UPRV_LENGTHOF(buf), &requiredCapacity, fields, 10, &status);
            TEST_ASSERT_SUCCESS(status);

            /* The TEST_ASSERT_SUCCESS call above should change too... */
            if(U_SUCCESS(status)) {
                TEST_ASSERT(numFields == 3);
                TEST_ASSERT_STRING("first ",  fields[0], true);
                TEST_ASSERT_STRING(" second", fields[1], true);
                TEST_ASSERT_STRING("  third", fields[2], true);
                TEST_ASSERT(fields[3] == NULL);

                spaceNeeded = u_strlen(textToSplit) -
                            (numFields - 1)  +  /* Field delimiters do not appear in output */
                            numFields;          /* Each field gets a NUL terminator */ 

                TEST_ASSERT(spaceNeeded == requiredCapacity);
            }
        }

        uregex_close(re);

    
        /*  Split with too few output strings available */
        status = U_ZERO_ERROR;
        re = uregex_openC(":", 0, NULL, &status);
        uregex_setText(re, textToSplit, -1, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            memset(fields, -1, sizeof(fields));
            numFields = 
                uregex_split(re, buf, UPRV_LENGTHOF(buf), &requiredCapacity, fields, 2, &status);
            TEST_ASSERT_SUCCESS(status);

            /* The TEST_ASSERT_SUCCESS call above should change too... */
            if(U_SUCCESS(status)) {
                TEST_ASSERT(numFields == 2);
                TEST_ASSERT_STRING("first ",  fields[0], true);
                TEST_ASSERT_STRING(" second:  third", fields[1], true);
                TEST_ASSERT(!memcmp(&fields[2],&minus1,sizeof(UChar*)));

                spaceNeeded = u_strlen(textToSplit) -
                            (numFields - 1)  +  /* Field delimiters do not appear in output */
                            numFields;          /* Each field gets a NUL terminator */ 

                TEST_ASSERT(spaceNeeded == requiredCapacity);

                /* Split with a range of output buffer sizes.  */
                spaceNeeded = u_strlen(textToSplit) -
                    (numFields - 1)  +  /* Field delimiters do not appear in output */
                    numFields;          /* Each field gets a NUL terminator */ 
                        
                for (sz=0; sz < spaceNeeded+1; sz++) {
                    memset(fields, -1, sizeof(fields));
                    status = U_ZERO_ERROR;
                    numFields = 
                        uregex_split(re, buf, sz, &requiredCapacity, fields, 10, &status);
                    if (sz >= spaceNeeded) {
                        TEST_ASSERT_SUCCESS(status);
                        TEST_ASSERT_STRING("first ",  fields[0], true);
                        TEST_ASSERT_STRING(" second", fields[1], true);
                        TEST_ASSERT_STRING("  third", fields[2], true);
                    } else {
                        TEST_ASSERT(status == U_BUFFER_OVERFLOW_ERROR);
                    }
                    TEST_ASSERT(numFields == 3);
                    TEST_ASSERT(fields[3] == NULL);
                    TEST_ASSERT(spaceNeeded == requiredCapacity);
                }
            }
        }

        uregex_close(re);
    }




    /* Split(), part 2.  Patterns with capture groups.  The capture group text
     *                   comes out as additional fields.  */
    {
        UChar    textToSplit[80];
        UChar    buf[200];
        UChar    *fields[10];
        int32_t  numFields;
        int32_t  requiredCapacity;
        int32_t  spaceNeeded;
        int32_t  sz;

        u_uastrncpy(textToSplit, "first <tag-a> second<tag-b>  third",  UPRV_LENGTHOF(textToSplit));

        status = U_ZERO_ERROR;
        re = uregex_openC("<(.*?)>", 0, NULL, &status);

        uregex_setText(re, textToSplit, -1, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            memset(fields, -1, sizeof(fields));
            numFields = 
                uregex_split(re, buf, UPRV_LENGTHOF(buf), &requiredCapacity, fields, 10, &status);
            TEST_ASSERT_SUCCESS(status);

            /* The TEST_ASSERT_SUCCESS call above should change too... */
            if(U_SUCCESS(status)) {
                TEST_ASSERT(numFields == 5);
                TEST_ASSERT_STRING("first ",  fields[0], true);
                TEST_ASSERT_STRING("tag-a",   fields[1], true);
                TEST_ASSERT_STRING(" second", fields[2], true);
                TEST_ASSERT_STRING("tag-b",   fields[3], true);
                TEST_ASSERT_STRING("  third", fields[4], true);
                TEST_ASSERT(fields[5] == NULL);
                spaceNeeded = (int32_t)strlen("first .tag-a. second.tag-b.  third.");  /* "." at NUL positions */
                TEST_ASSERT(spaceNeeded == requiredCapacity);
            }
        }
    
        /*  Split with too few output strings available (2) */
        status = U_ZERO_ERROR;
        memset(fields, -1, sizeof(fields));
        numFields = 
            uregex_split(re, buf, UPRV_LENGTHOF(buf), &requiredCapacity, fields, 2, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            TEST_ASSERT(numFields == 2);
            TEST_ASSERT_STRING("first ",  fields[0], true);
            TEST_ASSERT_STRING(" second<tag-b>  third", fields[1], true);
            TEST_ASSERT(!memcmp(&fields[2],&minus1,sizeof(UChar*)));

            spaceNeeded = (int32_t)strlen("first . second<tag-b>  third.");  /* "." at NUL positions */
            TEST_ASSERT(spaceNeeded == requiredCapacity);
        }

        /*  Split with too few output strings available (3) */
        status = U_ZERO_ERROR;
        memset(fields, -1, sizeof(fields));
        numFields = 
            uregex_split(re, buf, UPRV_LENGTHOF(buf), &requiredCapacity, fields, 3, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            TEST_ASSERT(numFields == 3);
            TEST_ASSERT_STRING("first ",  fields[0], true);
            TEST_ASSERT_STRING("tag-a",   fields[1], true);
            TEST_ASSERT_STRING(" second<tag-b>  third", fields[2], true);
            TEST_ASSERT(!memcmp(&fields[3],&minus1,sizeof(UChar*)));

            spaceNeeded = (int32_t)strlen("first .tag-a. second<tag-b>  third.");  /* "." at NUL positions */
            TEST_ASSERT(spaceNeeded == requiredCapacity);
        }

        /*  Split with just enough output strings available (5) */
        status = U_ZERO_ERROR;
        memset(fields, -1, sizeof(fields));
        numFields = 
            uregex_split(re, buf, UPRV_LENGTHOF(buf), &requiredCapacity, fields, 5, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            TEST_ASSERT(numFields == 5);
            TEST_ASSERT_STRING("first ",  fields[0], true);
            TEST_ASSERT_STRING("tag-a",   fields[1], true);
            TEST_ASSERT_STRING(" second", fields[2], true);
            TEST_ASSERT_STRING("tag-b",   fields[3], true);
            TEST_ASSERT_STRING("  third", fields[4], true);
            TEST_ASSERT(!memcmp(&fields[5],&minus1,sizeof(UChar*)));

            spaceNeeded = (int32_t)strlen("first .tag-a. second.tag-b.  third.");  /* "." at NUL positions */
            TEST_ASSERT(spaceNeeded == requiredCapacity);
        }

        /* Split, end of text is a field delimiter.   */
        status = U_ZERO_ERROR;
        sz = (int32_t)strlen("first <tag-a> second<tag-b>");
        uregex_setText(re, textToSplit, sz, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            memset(fields, -1, sizeof(fields));
            numFields = 
                uregex_split(re, buf, UPRV_LENGTHOF(buf), &requiredCapacity, fields, 9, &status);
            TEST_ASSERT_SUCCESS(status);

            /* The TEST_ASSERT_SUCCESS call above should change too... */
            if(U_SUCCESS(status)) {
                TEST_ASSERT(numFields == 5);
                TEST_ASSERT_STRING("first ",  fields[0], true);
                TEST_ASSERT_STRING("tag-a",   fields[1], true);
                TEST_ASSERT_STRING(" second", fields[2], true);
                TEST_ASSERT_STRING("tag-b",   fields[3], true);
                TEST_ASSERT_STRING("",        fields[4], true);
                TEST_ASSERT(fields[5] == NULL);
                TEST_ASSERT(fields[8] == NULL);
                TEST_ASSERT(!memcmp(&fields[9],&minus1,sizeof(UChar*)));
                spaceNeeded = (int32_t)strlen("first .tag-a. second.tag-b..");  /* "." at NUL positions */
                TEST_ASSERT(spaceNeeded == requiredCapacity);
            }
        }

        uregex_close(re);
    }

    /*
     * set/getTimeLimit
     */
     TEST_SETUP("abc$", "abcdef", 0);
     TEST_ASSERT(uregex_getTimeLimit(re, &status) == 0);
     uregex_setTimeLimit(re, 1000, &status);
     TEST_ASSERT(uregex_getTimeLimit(re, &status) == 1000);
     TEST_ASSERT_SUCCESS(status);
     uregex_setTimeLimit(re, -1, &status);
     TEST_ASSERT(status == U_ILLEGAL_ARGUMENT_ERROR);
     status = U_ZERO_ERROR;
     TEST_ASSERT(uregex_getTimeLimit(re, &status) == 1000);
     TEST_TEARDOWN;

     /*
      * set/get Stack Limit
      */
     TEST_SETUP("abc$", "abcdef", 0);
     TEST_ASSERT(uregex_getStackLimit(re, &status) == 8000000);
     uregex_setStackLimit(re, 40000, &status);
     TEST_ASSERT(uregex_getStackLimit(re, &status) == 40000);
     TEST_ASSERT_SUCCESS(status);
     uregex_setStackLimit(re, -1, &status);
     TEST_ASSERT(status == U_ILLEGAL_ARGUMENT_ERROR);
     status = U_ZERO_ERROR;
     TEST_ASSERT(uregex_getStackLimit(re, &status) == 40000);
     TEST_TEARDOWN;
     
     
     /*
      * Get/Set callback functions
      *     This test is copied from intltest regex/Callbacks
      *     The pattern and test data will run long enough to cause the callback
      *       to be invoked.  The nested '+' operators give exponential time
      *       behavior with increasing string length.
      */
     TEST_SETUP("((.)+\\2)+x", "aaaaaaaaaaaaaaaaaaab", 0);
     callBackContext cbInfo = {4, 0, 0};
     const void     *pContext   = &cbInfo;
     URegexMatchCallback    *returnedFn = &TestCallbackFn;
     
     /*  Getting the callback fn when it hasn't been set must return NULL  */
     uregex_getMatchCallback(re, &returnedFn, &pContext, &status);
     TEST_ASSERT_SUCCESS(status);
     TEST_ASSERT(returnedFn == NULL);
     TEST_ASSERT(pContext == NULL);
     
     /* Set thecallback and do a match.                                   */
     /* The callback function should record that it has been called.      */
     uregex_setMatchCallback(re, &TestCallbackFn, &cbInfo, &status);
     TEST_ASSERT_SUCCESS(status);
     TEST_ASSERT(cbInfo.numCalls == 0);
     TEST_ASSERT(uregex_matches(re, -1, &status) == false);
     TEST_ASSERT_SUCCESS(status);
     TEST_ASSERT(cbInfo.numCalls > 0);
     
     /* Getting the callback should return the values that were set above.  */
     uregex_getMatchCallback(re, &returnedFn, &pContext, &status);
     TEST_ASSERT(returnedFn == &TestCallbackFn);
     TEST_ASSERT(pContext == &cbInfo);

     TEST_TEARDOWN;
}



static void TestBug4315(void) {
    UErrorCode      theICUError = U_ZERO_ERROR;
    URegularExpression *theRegEx;
    UChar           *textBuff;
    const char      *thePattern;
    UChar            theString[100];
    UChar           *destFields[24];
    int32_t         neededLength1;
    int32_t         neededLength2;

    int32_t         wordCount = 0;
    int32_t         destFieldsSize = 24;

    thePattern  = "ck ";
    u_uastrcpy(theString, "The quick brown fox jumped over the slow black turtle.");

    /* open a regex */
    theRegEx = uregex_openC(thePattern, 0, NULL, &theICUError);
    TEST_ASSERT_SUCCESS(theICUError);

    /* set the input string */
    uregex_setText(theRegEx, theString, u_strlen(theString), &theICUError);
    TEST_ASSERT_SUCCESS(theICUError);

    /* split */
    /*explicitly pass NULL and 0 to force the overflow error -> this is where the
     *  error occurs! */
    wordCount = uregex_split(theRegEx, NULL, 0, &neededLength1, destFields,
        destFieldsSize, &theICUError);

    TEST_ASSERT(theICUError == U_BUFFER_OVERFLOW_ERROR);
    TEST_ASSERT(wordCount==3);

    if(theICUError == U_BUFFER_OVERFLOW_ERROR)
    {
        theICUError = U_ZERO_ERROR;
        textBuff = (UChar *) malloc(sizeof(UChar) * (neededLength1 + 1));
        wordCount = uregex_split(theRegEx, textBuff, neededLength1+1, &neededLength2,
            destFields, destFieldsSize, &theICUError);
        TEST_ASSERT(wordCount==3);
        TEST_ASSERT_SUCCESS(theICUError);
        TEST_ASSERT(neededLength1 == neededLength2);
        TEST_ASSERT_STRING("The qui", destFields[0], true);
        TEST_ASSERT_STRING("brown fox jumped over the slow bla", destFields[1], true);
        TEST_ASSERT_STRING("turtle.", destFields[2], true);
        TEST_ASSERT(destFields[3] == NULL);
        free(textBuff);
    }
    uregex_close(theRegEx);
}

/* Based on TestRegexCAPI() */
static void TestUTextAPI(void) {
    UErrorCode           status = U_ZERO_ERROR;
    URegularExpression  *re;
    UText                patternText = UTEXT_INITIALIZER;
    UChar                pat[200];
    const char           patternTextUTF8[5] = { 0x61, 0x62, 0x63, 0x2a, 0x00 };

    /* Mimimalist open/close */
    utext_openUTF8(&patternText, patternTextUTF8, -1, &status);
    re = uregex_openUText(&patternText, 0, 0, &status);
    if (U_FAILURE(status)) {
         log_data_err("Failed to open regular expression, %s:%d, error is \"%s\" (Are you missing data?)\n", __FILE__, __LINE__, u_errorName(status));
         utext_close(&patternText);
         return;
    }
    uregex_close(re);

    /* Open with all flag values set */
    status = U_ZERO_ERROR;
    re = uregex_openUText(&patternText, 
        UREGEX_CASE_INSENSITIVE | UREGEX_COMMENTS | UREGEX_DOTALL | UREGEX_MULTILINE | UREGEX_UWORD,
        0, &status);
    TEST_ASSERT_SUCCESS(status);
    uregex_close(re);

    /* Open with an invalid flag */
    status = U_ZERO_ERROR;
    re = uregex_openUText(&patternText, 0x40000000, 0, &status);
    TEST_ASSERT(status == U_REGEX_INVALID_FLAG);
    uregex_close(re);

    /* open with an invalid parameter */
    status = U_ZERO_ERROR;
    re = uregex_openUText(NULL,
        UREGEX_CASE_INSENSITIVE | UREGEX_COMMENTS | UREGEX_DOTALL | UREGEX_MULTILINE | UREGEX_UWORD, 0, &status);
    TEST_ASSERT(status == U_ILLEGAL_ARGUMENT_ERROR && re == NULL);

    /*
     *  clone
     */
    {
        URegularExpression *clone1;
        URegularExpression *clone2;
        URegularExpression *clone3;
        UChar  testString1[30];
        UChar  testString2[30];
        UBool  result;


        status = U_ZERO_ERROR;
        re = uregex_openUText(&patternText, 0, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        clone1 = uregex_clone(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(clone1 != NULL);

        status = U_ZERO_ERROR;
        clone2 = uregex_clone(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(clone2 != NULL);
        uregex_close(re);

        status = U_ZERO_ERROR;
        clone3 = uregex_clone(clone2, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(clone3 != NULL);

        u_uastrncpy(testString1, "abcccd", UPRV_LENGTHOF(pat));
        u_uastrncpy(testString2, "xxxabcccd", UPRV_LENGTHOF(pat));

        status = U_ZERO_ERROR;
        uregex_setText(clone1, testString1, -1, &status);
        TEST_ASSERT_SUCCESS(status);
        result = uregex_lookingAt(clone1, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(result==true);
        
        status = U_ZERO_ERROR;
        uregex_setText(clone2, testString2, -1, &status);
        TEST_ASSERT_SUCCESS(status);
        result = uregex_lookingAt(clone2, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(result==false);
        result = uregex_find(clone2, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(result==true);

        uregex_close(clone1);
        uregex_close(clone2);
        uregex_close(clone3);

    }

    /*
     *  pattern() and patternText()
     */
    {
        const UChar  *resultPat;
        int32_t       resultLen;
        UText        *resultText;
        const char str_hello[] = { 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x00 }; /* hello */
        const char str_hel[] = { 0x68, 0x65, 0x6c, 0x00 }; /* hel */
        u_uastrncpy(pat, "hello", UPRV_LENGTHOF(pat)); /* for comparison */
        status = U_ZERO_ERROR;
        
        utext_openUTF8(&patternText, str_hello, -1, &status);
        re = uregex_open(pat, -1, 0, NULL, &status);
        resultPat = uregex_pattern(re, &resultLen, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS above should change too... */
        if (U_SUCCESS(status)) {
            TEST_ASSERT(resultLen == -1);
            TEST_ASSERT(u_strcmp(resultPat, pat) == 0);
        }
        
        resultText = uregex_patternUText(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_UTEXT(str_hello, resultText);

        uregex_close(re);

        status = U_ZERO_ERROR;
        re = uregex_open(pat, 3, 0, NULL, &status);
        resultPat = uregex_pattern(re, &resultLen, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS above should change too... */
        if (U_SUCCESS(status)) {
            TEST_ASSERT(resultLen == 3);
            TEST_ASSERT(u_strncmp(resultPat, pat, 3) == 0);
            TEST_ASSERT(u_strlen(resultPat) == 3);
        }
        
        resultText = uregex_patternUText(re, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_UTEXT(str_hel, resultText);

        uregex_close(re);
    }

    /*
     *  setUText() and lookingAt()
     */
    {
        UText  text1 = UTEXT_INITIALIZER;
        UText  text2 = UTEXT_INITIALIZER;
        UBool  result;
        const char str_abcccd[] = { 0x62, 0x63, 0x64, 0x64, 0x64, 0x65, 0x00 }; /* abcccd */
        const char str_abcccxd[] = { 0x62, 0x63, 0x64, 0x64, 0x64, 0x79, 0x65, 0x00 }; /* abcccxd */
        const char str_abcd[] = { 0x62, 0x63, 0x64, 0x2b, 0x65, 0x00 }; /* abc*d */
        status = U_ZERO_ERROR;
        utext_openUTF8(&text1, str_abcccd, -1, &status);
        utext_openUTF8(&text2, str_abcccxd, -1, &status);
        
        utext_openUTF8(&patternText, str_abcd, -1, &status);
        re = uregex_openUText(&patternText, 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);

        /* Operation before doing a setText should fail... */
        status = U_ZERO_ERROR;
        uregex_lookingAt(re, 0, &status);
        TEST_ASSERT( status== U_REGEX_INVALID_STATE);

        status = U_ZERO_ERROR;
        uregex_setUText(re, &text1, &status);
        result = uregex_lookingAt(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setUText(re, &text2, &status);
        result = uregex_lookingAt(re, 0, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_setUText(re, &text1, &status);
        result = uregex_lookingAt(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);

        uregex_close(re);
        utext_close(&text1);
        utext_close(&text2);
    }


    /*
     *  getText() and getUText()
     */
    {
        UText  text1 = UTEXT_INITIALIZER;
        UText  text2 = UTEXT_INITIALIZER;
        UChar  text2Chars[20];
        UText  *resultText;
        const UChar   *result;
        int32_t  textLength;
        const char str_abcccd[] = { 0x62, 0x63, 0x64, 0x64, 0x64, 0x65, 0x00 }; /* abcccd */
        const char str_abcccxd[] = { 0x62, 0x63, 0x64, 0x64, 0x64, 0x79, 0x65, 0x00 }; /* abcccxd */
        const char str_abcd[] = { 0x62, 0x63, 0x64, 0x2b, 0x65, 0x00 }; /* abc*d */


        status = U_ZERO_ERROR;
        utext_openUTF8(&text1, str_abcccd, -1, &status);
        u_uastrncpy(text2Chars, str_abcccxd, UPRV_LENGTHOF(text2Chars));
        utext_openUChars(&text2, text2Chars, -1, &status);
        
        utext_openUTF8(&patternText, str_abcd, -1, &status);
        re = uregex_openUText(&patternText, 0, NULL, &status);

        /* First set a UText */
        uregex_setUText(re, &text1, &status);
        resultText = uregex_getUText(re, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(resultText != &text1);
        utext_setNativeIndex(resultText, 0);
        utext_setNativeIndex(&text1, 0);
        TEST_ASSERT(testUTextEqual(resultText, &text1));
        utext_close(resultText);
        
        result = uregex_getText(re, &textLength, &status); /* flattens UText into buffer */
        (void)result;    /* Suppress set but not used warning. */
        TEST_ASSERT(textLength == -1 || textLength == 6);
        resultText = uregex_getUText(re, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(resultText != &text1);
        utext_setNativeIndex(resultText, 0);
        utext_setNativeIndex(&text1, 0);
        TEST_ASSERT(testUTextEqual(resultText, &text1));
        utext_close(resultText);

        /* Then set a UChar * */
        uregex_setText(re, text2Chars, 7, &status);
        resultText = uregex_getUText(re, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        utext_setNativeIndex(resultText, 0);
        utext_setNativeIndex(&text2, 0);
        TEST_ASSERT(testUTextEqual(resultText, &text2));
        utext_close(resultText);
        result = uregex_getText(re, &textLength, &status);
        TEST_ASSERT(textLength == 7);
        
        uregex_close(re);
        utext_close(&text1);
        utext_close(&text2);
    }

    /*
     *  matches()
     */
    {
        UText   text1 = UTEXT_INITIALIZER;
        UBool   result;
        UText   nullText = UTEXT_INITIALIZER;
        const char str_abcccde[] = { 0x61, 0x62, 0x63, 0x63, 0x63, 0x64, 0x65, 0x00 }; /* abcccde */
        const char str_abcd[] = { 0x61, 0x62, 0x63, 0x2a, 0x64, 0x00 }; /* abc*d */

        status = U_ZERO_ERROR;
        utext_openUTF8(&text1, str_abcccde, -1, &status);
        utext_openUTF8(&patternText, str_abcd, -1, &status);
        re = uregex_openUText(&patternText, 0, NULL, &status);

        uregex_setUText(re, &text1, &status);
        result = uregex_matches(re, 0, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);
        uregex_close(re);

        status = U_ZERO_ERROR;
        re = uregex_openC(".?", 0, NULL, &status);
        uregex_setUText(re, &text1, &status);
        result = uregex_matches(re, 7, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        utext_openUTF8(&nullText, "", -1, &status);
        uregex_setUText(re, &nullText, &status);
        TEST_ASSERT_SUCCESS(status);
        result = uregex_matches(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT_SUCCESS(status);
        
        uregex_close(re);
        utext_close(&text1);
        utext_close(&nullText);
    }


    /*
     *  lookingAt()    Used in setText test.
     */


    /*
     *  find(), findNext, start, end, reset
     */
    {
        UChar    text1[50];
        UBool    result;
        u_uastrncpy(text1, "012rx5rx890rxrx...",  UPRV_LENGTHOF(text1));
        status = U_ZERO_ERROR;
        re = uregex_openC("rx", 0, NULL, &status);

        uregex_setText(re, text1, -1, &status);
        result = uregex_find(re, 0, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 3);
        TEST_ASSERT(uregex_end(re, 0, &status) == 5);
        TEST_ASSERT_SUCCESS(status);

        result = uregex_find(re, 9, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 11);
        TEST_ASSERT(uregex_end(re, 0, &status) == 13);
        TEST_ASSERT_SUCCESS(status);

        result = uregex_find(re, 14, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_reset(re, 0, &status);

        result = uregex_findNext(re, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 3);
        TEST_ASSERT(uregex_end(re, 0, &status) == 5);
        TEST_ASSERT_SUCCESS(status);

        result = uregex_findNext(re, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 6);
        TEST_ASSERT(uregex_end(re, 0, &status) == 8);
        TEST_ASSERT_SUCCESS(status);

        status = U_ZERO_ERROR;
        uregex_reset(re, 12, &status);

        result = uregex_findNext(re, &status);
        TEST_ASSERT(result == true);
        TEST_ASSERT(uregex_start(re, 0, &status) == 13);
        TEST_ASSERT(uregex_end(re, 0, &status) == 15);
        TEST_ASSERT_SUCCESS(status);

        result = uregex_findNext(re, &status);
        TEST_ASSERT(result == false);
        TEST_ASSERT_SUCCESS(status);

        uregex_close(re);
    }

    /*
     *  groupUText()
     */
    {
        UChar    text1[80];
        UText   *actual;
        UBool    result;
        int64_t  groupLen = 0;
        UChar    groupBuf[20];

        u_uastrncpy(text1, "noise abc interior def, and this is off the end",  UPRV_LENGTHOF(text1));

        status = U_ZERO_ERROR;
        re = uregex_openC("abc(.*?)def", 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);

        uregex_setText(re, text1, -1, &status);
        result = uregex_find(re, 0, &status);
        TEST_ASSERT(result==true);

        /*  Capture Group 0 with shallow clone API.  Should succeed.  */
        status = U_ZERO_ERROR;
        actual = uregex_groupUText(re, 0, NULL, &groupLen, &status);
        TEST_ASSERT_SUCCESS(status);

        TEST_ASSERT(utext_getNativeIndex(actual) == 6);  /* index of "abc " within "noise abc ..." */
        TEST_ASSERT(groupLen == 16);   /* length of "abc interior def"  */
        utext_extract(actual, 6 /*start index */, 6+16 /*limit index*/, groupBuf, sizeof(groupBuf), &status);

        TEST_ASSERT_STRING("abc interior def", groupBuf, true);
        utext_close(actual);

        /*  Capture group #1.  Should succeed. */
        status = U_ZERO_ERROR;

        actual = uregex_groupUText(re, 1, NULL, &groupLen, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT(9 == utext_getNativeIndex(actual));    /* index of " interior " within "noise abc interior def ... " */
                                                           /*    (within the string text1)           */
        TEST_ASSERT(10 == groupLen);                       /* length of " interior " */
        utext_extract(actual, 9 /*start index*/, 9+10 /*limit index*/, groupBuf, sizeof(groupBuf), &status);
        TEST_ASSERT_STRING(" interior ", groupBuf, true);

        utext_close(actual);

        /*  Capture group out of range.  Error. */
        status = U_ZERO_ERROR;
        actual = uregex_groupUText(re, 2, NULL, &groupLen, &status);
        TEST_ASSERT(status == U_INDEX_OUTOFBOUNDS_ERROR);
        utext_close(actual);

        uregex_close(re);
    }
    
    /*
     *  replaceFirst()
     */
    {
        UChar    text1[80];
        UChar    text2[80];
        UText    replText = UTEXT_INITIALIZER;
        UText   *result;
        const char str_Replxxx[] = { 0x52, 0x65, 0x70, 0x6c, 0x61, 0x63, 0x65, 0x20, 0x3c, 0x61, 0x61, 0x3e, 0x20, 0x78, 0x31, 0x78, 0x20, 0x78, 0x2e, 0x2e, 0x2e, 0x78, 0x2e, 0x00 }; /* Replace <aa> x1x x...x. */
        const char str_Nomatchhere[] = { 0x4e, 0x6f, 0x20, 0x6d, 0x61, 0x74, 0x63, 0x68, 0x20, 0x68, 0x65, 0x72, 0x65, 0x2e, 0x00 }; /* No match here. */
        const char str_u00411U00000042a[] =  { 0x5c, 0x5c, 0x5c, 0x75, 0x30, 0x30, 0x34, 0x31, 0x24, 0x31, 
               0x5c, 0x55, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x32, 0x5c, 0x24, 0x5c, 0x61, 0x00 }; /* \\\u0041$1\U00000042\$\a */
        const char str_1x[] = { 0x3c, 0x24, 0x31, 0x3e, 0x00 }; /* <$1> */
        const char str_ReplaceAaaBax1xxx[] = { 0x52, 0x65, 0x70, 0x6c, 0x61, 0x63, 0x65, 0x20, 0x5c, 0x41, 0x61, 0x61, 0x42, 0x24, 0x61, 0x20, 0x78, 0x31, 0x78, 0x20, 0x78, 0x2e, 0x2e, 0x2e, 0x78, 0x2e, 0x00 }; /* Replace \AaaB$a x1x x...x. */
        status = U_ZERO_ERROR;
        u_uastrncpy(text1, "Replace xaax x1x x...x.",  UPRV_LENGTHOF(text1));
        u_uastrncpy(text2, "No match here.",  UPRV_LENGTHOF(text2));
        utext_openUTF8(&replText, str_1x, -1, &status);

        re = uregex_openC("x(.*?)x", 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);

        /*  Normal case, with match */
        uregex_setText(re, text1, -1, &status);
        result = uregex_replaceFirstUText(re, &replText, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_UTEXT(str_Replxxx, result);
        utext_close(result);

        /* No match.  Text should copy to output with no changes.  */
        uregex_setText(re, text2, -1, &status);
        result = uregex_replaceFirstUText(re, &replText, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_UTEXT(str_Nomatchhere, result);
        utext_close(result);
        
        /* Unicode escapes */
        uregex_setText(re, text1, -1, &status);
        utext_openUTF8(&replText, str_u00411U00000042a, -1, &status);
        result = uregex_replaceFirstUText(re, &replText, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_UTEXT(str_ReplaceAaaBax1xxx, result);
        utext_close(result);

        uregex_close(re);
        utext_close(&replText);
    }


    /*
     *  replaceAll()
     */
    {
        UChar    text1[80];
        UChar    text2[80];
        UText    replText = UTEXT_INITIALIZER;
        UText   *result;
        const char str_1[] = { 0x3c, 0x24, 0x31, 0x3e, 0x00 }; /* <$1> */
        const char str_Replaceaa1[] = { 0x52, 0x65, 0x70, 0x6c, 0x61, 0x63, 0x65, 0x20, 0x3c, 0x61, 0x61, 0x3e, 0x20, 0x3c, 0x31, 0x3e, 0x20, 0x3c, 0x2e, 0x2e, 0x2e, 0x3e, 0x2e, 0x00 }; /* Replace <aa> <1> <...>. */
        const char str_Nomatchhere[] = { 0x4e, 0x6f, 0x20, 0x6d, 0x61, 0x74, 0x63, 0x68, 0x20, 0x68, 0x65, 0x72, 0x65, 0x2e, 0x00 }; /* No match here. */
        status = U_ZERO_ERROR;
        u_uastrncpy(text1, "Replace xaax x1x x...x.",  UPRV_LENGTHOF(text1));
        u_uastrncpy(text2, "No match here.",  UPRV_LENGTHOF(text2));
        utext_openUTF8(&replText, str_1, -1, &status);

        re = uregex_openC("x(.*?)x", 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);

        /*  Normal case, with match */
        uregex_setText(re, text1, -1, &status);
        result = uregex_replaceAllUText(re, &replText, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_UTEXT(str_Replaceaa1, result);
        utext_close(result);

        /* No match.  Text should copy to output with no changes.  */
        uregex_setText(re, text2, -1, &status);
        result = uregex_replaceAllUText(re, &replText, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_UTEXT(str_Nomatchhere, result);
        utext_close(result);

        uregex_close(re);
        utext_close(&replText);
    }


    /*
     *  appendReplacement()
     */
    {
        UChar    text[100];
        UChar    repl[100];
        UChar    buf[100];
        UChar   *bufPtr;
        int32_t  bufCap;

        status = U_ZERO_ERROR;
        re = uregex_openC(".*", 0, 0, &status);
        TEST_ASSERT_SUCCESS(status);

        u_uastrncpy(text, "whatever",  UPRV_LENGTHOF(text));
        u_uastrncpy(repl, "some other", UPRV_LENGTHOF(repl));
        uregex_setText(re, text, -1, &status);

        /* match covers whole target string */
        uregex_find(re, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        bufPtr = buf;
        bufCap = UPRV_LENGTHOF(buf);
        uregex_appendReplacement(re, repl, -1, &bufPtr, &bufCap, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING("some other", buf, true);

        /* Match has \u \U escapes */
        uregex_find(re, 0, &status);
        TEST_ASSERT_SUCCESS(status);
        bufPtr = buf;
        bufCap = UPRV_LENGTHOF(buf);
        u_uastrncpy(repl, "abc\\u0041\\U00000042 \\\\ \\$ \\abc", UPRV_LENGTHOF(repl));
        uregex_appendReplacement(re, repl, -1, &bufPtr, &bufCap, &status);
        TEST_ASSERT_SUCCESS(status);
        TEST_ASSERT_STRING("abcAB \\ $ abc", buf, true); 

        uregex_close(re);
    }


    /*
     *  appendReplacement(), appendTail() checked in replaceFirst(), replaceAll().
     */

    /*
     *  splitUText()
     */
    {
        UChar    textToSplit[80];
        UChar    text2[80];
        UText    *fields[10];
        int32_t  numFields;
        int32_t i;

        u_uastrncpy(textToSplit, "first : second:  third",  UPRV_LENGTHOF(textToSplit));
        u_uastrncpy(text2, "No match here.",  UPRV_LENGTHOF(text2));

        status = U_ZERO_ERROR;
        re = uregex_openC(":", 0, NULL, &status);


        /*  Simple split */ 

        uregex_setText(re, textToSplit, -1, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if (U_SUCCESS(status)) {
            memset(fields, 0, sizeof(fields));
            numFields = uregex_splitUText(re, fields, 10, &status);
            TEST_ASSERT_SUCCESS(status);

            /* The TEST_ASSERT_SUCCESS call above should change too... */
            if(U_SUCCESS(status)) {
              const char str_first[] = { 0x66, 0x69, 0x72, 0x73, 0x74, 0x20, 0x00 }; /* 'first ' */
              const char str_second[] = { 0x20, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x00 }; /* '  second' */
              const char str_third[] = { 0x20, 0x20, 0x74, 0x68, 0x69, 0x72, 0x64, 0x00 }; /* '  third' */
                TEST_ASSERT(numFields == 3);
                TEST_ASSERT_UTEXT(str_first,  fields[0]);
                TEST_ASSERT_UTEXT(str_second, fields[1]);
                TEST_ASSERT_UTEXT(str_third, fields[2]);
                TEST_ASSERT(fields[3] == NULL);
            }
            for(i = 0; i < numFields; i++) {
                utext_close(fields[i]);
            }
        }

        uregex_close(re);

    
        /*  Split with too few output strings available */
        status = U_ZERO_ERROR;
        re = uregex_openC(":", 0, NULL, &status);
        uregex_setText(re, textToSplit, -1, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            fields[0] = NULL;
            fields[1] = NULL;
            fields[2] = &patternText;
            numFields = uregex_splitUText(re, fields, 2, &status);
            TEST_ASSERT_SUCCESS(status);

            /* The TEST_ASSERT_SUCCESS call above should change too... */
            if(U_SUCCESS(status)) {
                const char str_first[] = { 0x66, 0x69, 0x72, 0x73, 0x74, 0x20, 0x00 }; /* first  */
                const char str_secondthird[] = { 0x20, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x3a, 0x20, 0x20, 0x74, 0x68, 0x69, 0x72, 0x64, 0x00 }; /*  second:  third */
                TEST_ASSERT(numFields == 2);
                TEST_ASSERT_UTEXT(str_first,  fields[0]);
                TEST_ASSERT_UTEXT(str_secondthird, fields[1]);
                TEST_ASSERT(fields[2] == &patternText);
            }
            for(i = 0; i < numFields; i++) {
                utext_close(fields[i]);
            }
        }

        uregex_close(re);
    }

    /* splitUText(), part 2.  Patterns with capture groups.  The capture group text
     *                   comes out as additional fields.  */
    {
        UChar    textToSplit[80];
        UText    *fields[10];
        int32_t  numFields;
        int32_t i;

        u_uastrncpy(textToSplit, "first <tag-a> second<tag-b>  third",  UPRV_LENGTHOF(textToSplit));

        status = U_ZERO_ERROR;
        re = uregex_openC("<(.*?)>", 0, NULL, &status);

        uregex_setText(re, textToSplit, -1, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            memset(fields, 0, sizeof(fields));
            numFields = uregex_splitUText(re, fields, 10, &status);
            TEST_ASSERT_SUCCESS(status);

            /* The TEST_ASSERT_SUCCESS call above should change too... */
            if(U_SUCCESS(status)) {
                const char str_first[] = { 0x66, 0x69, 0x72, 0x73, 0x74, 0x20, 0x00 }; /* first  */
                const char str_taga[] = { 0x74, 0x61, 0x67, 0x2d, 0x61, 0x00 }; /* tag-a */
                const char str_second[] = { 0x20, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x00 }; /*  second */
                const char str_tagb[] = { 0x74, 0x61, 0x67, 0x2d, 0x62, 0x00 }; /* tag-b */
                const char str_third[] = { 0x20, 0x20, 0x74, 0x68, 0x69, 0x72, 0x64, 0x00 }; /*   third */

                TEST_ASSERT(numFields == 5);
                TEST_ASSERT_UTEXT(str_first,  fields[0]);
                TEST_ASSERT_UTEXT(str_taga,   fields[1]);
                TEST_ASSERT_UTEXT(str_second, fields[2]);
                TEST_ASSERT_UTEXT(str_tagb,   fields[3]);
                TEST_ASSERT_UTEXT(str_third, fields[4]);
                TEST_ASSERT(fields[5] == NULL);
            }
            for(i = 0; i < numFields; i++) {
                utext_close(fields[i]);
            }
        }
    
        /*  Split with too few output strings available (2) */
        status = U_ZERO_ERROR;
        fields[0] = NULL;
        fields[1] = NULL;
        fields[2] = &patternText;
        numFields = uregex_splitUText(re, fields, 2, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            const char str_first[] = { 0x66, 0x69, 0x72, 0x73, 0x74, 0x20, 0x00 }; /* first  */
            const char str_secondtagbthird[] = { 0x20, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x3c, 0x74, 0x61, 0x67, 0x2d, 0x62, 0x3e, 0x20, 0x20, 0x74, 0x68, 0x69, 0x72, 0x64, 0x00 }; /*  second<tag-b>  third */
            TEST_ASSERT(numFields == 2);
            TEST_ASSERT_UTEXT(str_first,  fields[0]);
            TEST_ASSERT_UTEXT(str_secondtagbthird, fields[1]);
            TEST_ASSERT(fields[2] == &patternText);
        }
        for(i = 0; i < numFields; i++) {
            utext_close(fields[i]);
        }


        /*  Split with too few output strings available (3) */
        status = U_ZERO_ERROR;
        fields[0] = NULL;
        fields[1] = NULL;
        fields[2] = NULL;
        fields[3] = &patternText;
        numFields = uregex_splitUText(re, fields, 3, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            const char str_first[] = { 0x66, 0x69, 0x72, 0x73, 0x74, 0x20, 0x00 }; /* first  */
            const char str_taga[] = { 0x74, 0x61, 0x67, 0x2d, 0x61, 0x00 }; /* tag-a */
            const char str_secondtagbthird[] = { 0x20, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x3c, 0x74, 0x61, 0x67, 0x2d, 0x62, 0x3e, 0x20, 0x20, 0x74, 0x68, 0x69, 0x72, 0x64, 0x00 }; /*  second<tag-b>  third */
            TEST_ASSERT(numFields == 3);
            TEST_ASSERT_UTEXT(str_first,  fields[0]);
            TEST_ASSERT_UTEXT(str_taga,   fields[1]);
            TEST_ASSERT_UTEXT(str_secondtagbthird, fields[2]);
            TEST_ASSERT(fields[3] == &patternText);
        }
        for(i = 0; i < numFields; i++) {
            utext_close(fields[i]);
        }

        /*  Split with just enough output strings available (5) */
        status = U_ZERO_ERROR;
        fields[0] = NULL;
        fields[1] = NULL;
        fields[2] = NULL;
        fields[3] = NULL;
        fields[4] = NULL;
        fields[5] = &patternText;
        numFields = uregex_splitUText(re, fields, 5, &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            const char str_first[] = { 0x66, 0x69, 0x72, 0x73, 0x74, 0x20, 0x00 }; /* first  */
            const char str_taga[] = { 0x74, 0x61, 0x67, 0x2d, 0x61, 0x00 }; /* tag-a */
            const char str_second[] = { 0x20, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x00 }; /*  second */
            const char str_tagb[] = { 0x74, 0x61, 0x67, 0x2d, 0x62, 0x00 }; /* tag-b */
            const char str_third[] = { 0x20, 0x20, 0x74, 0x68, 0x69, 0x72, 0x64, 0x00 }; /*   third */

            TEST_ASSERT(numFields == 5);
            TEST_ASSERT_UTEXT(str_first,  fields[0]);
            TEST_ASSERT_UTEXT(str_taga,   fields[1]);
            TEST_ASSERT_UTEXT(str_second, fields[2]);
            TEST_ASSERT_UTEXT(str_tagb,   fields[3]);
            TEST_ASSERT_UTEXT(str_third, fields[4]);
            TEST_ASSERT(fields[5] == &patternText);
        }
        for(i = 0; i < numFields; i++) {
            utext_close(fields[i]);
        }

        /* Split, end of text is a field delimiter.   */
        status = U_ZERO_ERROR;
        uregex_setText(re, textToSplit, (int32_t)strlen("first <tag-a> second<tag-b>"), &status);
        TEST_ASSERT_SUCCESS(status);

        /* The TEST_ASSERT_SUCCESS call above should change too... */
        if(U_SUCCESS(status)) {
            memset(fields, 0, sizeof(fields));
            fields[9] = &patternText;
            numFields = uregex_splitUText(re, fields, 9, &status);
            TEST_ASSERT_SUCCESS(status);

            /* The TEST_ASSERT_SUCCESS call above should change too... */
            if(U_SUCCESS(status)) {
                const char str_first[] = { 0x66, 0x69, 0x72, 0x73, 0x74, 0x20, 0x00 }; /* first  */
                const char str_taga[] = { 0x74, 0x61, 0x67, 0x2d, 0x61, 0x00 }; /* tag-a */
                const char str_second[] = { 0x20, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x00 }; /*  second */
                const char str_tagb[] = { 0x74, 0x61, 0x67, 0x2d, 0x62, 0x00 }; /* tag-b */
                const char str_empty[] = { 0x00 };

                TEST_ASSERT(numFields == 5);
                TEST_ASSERT_UTEXT(str_first,  fields[0]);
                TEST_ASSERT_UTEXT(str_taga,   fields[1]);
                TEST_ASSERT_UTEXT(str_second, fields[2]);
                TEST_ASSERT_UTEXT(str_tagb,   fields[3]);
                TEST_ASSERT_UTEXT(str_empty,  fields[4]);
                TEST_ASSERT(fields[5] == NULL);
                TEST_ASSERT(fields[8] == NULL);
                TEST_ASSERT(fields[9] == &patternText);
            }
            for(i = 0; i < numFields; i++) {
                utext_close(fields[i]);
            }
        }

        uregex_close(re);
    }
    utext_close(&patternText);
}


static void TestRefreshInput(void) {
    /*
     *  RefreshInput changes out the input of a URegularExpression without
     *    changing anything else in the match state.  Used with Java JNI,
     *    when Java moves the underlying string storage.   This test
     *    runs a find() loop, moving the text after the first match.
     *    The right number of matches should still be found.
     */
    UChar testStr[]  = {0x41, 0x20, 0x42, 0x20, 0x43, 0x0};  /* = "A B C"  */
    UChar movedStr[] = {   0,    0,    0,    0,    0,   0};
    UErrorCode status = U_ZERO_ERROR;
    URegularExpression *re;
    UText ut1 = UTEXT_INITIALIZER;
    UText ut2 = UTEXT_INITIALIZER;
    
    re = uregex_openC("[ABC]", 0, 0, &status);
    TEST_ASSERT_SUCCESS(status);

    utext_openUChars(&ut1, testStr, -1, &status);
    TEST_ASSERT_SUCCESS(status);
    uregex_setUText(re, &ut1, &status);
    TEST_ASSERT_SUCCESS(status);

    /* Find the first match "A" in the original string */
    TEST_ASSERT(uregex_findNext(re, &status));
    TEST_ASSERT(uregex_start(re, 0, &status) == 0);
    
    /* Move the string, kill the original string.  */
    u_strcpy(movedStr, testStr);
    u_memset(testStr, 0, u_strlen(testStr));
    utext_openUChars(&ut2, movedStr, -1, &status);
    TEST_ASSERT_SUCCESS(status);
    uregex_refreshUText(re, &ut2, &status);
    TEST_ASSERT_SUCCESS(status);

    /* Find the following two matches, now working in the moved string. */
    TEST_ASSERT(uregex_findNext(re, &status));
    TEST_ASSERT(uregex_start(re, 0, &status) == 2);
    TEST_ASSERT(uregex_findNext(re, &status));
    TEST_ASSERT(uregex_start(re, 0, &status) == 4);
    TEST_ASSERT(false == uregex_findNext(re, &status));

    uregex_close(re);
}


static void TestBug8421(void) {
    /* Bug 8421:  setTimeLimit on a regular expression before setting text to be matched
     *             was failing. 
     */
    URegularExpression *re;
    UErrorCode status = U_ZERO_ERROR;
    int32_t  limit = -1;

    re = uregex_openC("abc", 0, 0, &status);
    TEST_ASSERT_SUCCESS(status);

    limit = uregex_getTimeLimit(re, &status);
    TEST_ASSERT_SUCCESS(status);
    TEST_ASSERT(limit == 0);

    uregex_setTimeLimit(re, 100, &status);
    TEST_ASSERT_SUCCESS(status);
    limit = uregex_getTimeLimit(re, &status);
    TEST_ASSERT_SUCCESS(status);
    TEST_ASSERT(limit == 100);

    uregex_close(re);
}

static UBool U_CALLCONV FindCallback(const void* context , int64_t matchIndex) {
    // suppress compiler warnings about unused variables
    (void)context;
    (void)matchIndex;
    return false;
}

static UBool U_CALLCONV MatchCallback(const void *context, int32_t steps) {
    // suppress compiler warnings about unused variables
    (void)context;
    (void)steps;
    return false;
}

static void TestBug10815(void) {
  /* Bug 10815:   uregex_findNext() does not set U_REGEX_STOPPED_BY_CALLER 
   *              when the callback function specified by uregex_setMatchCallback() returns false
   */
    URegularExpression *re;
    UErrorCode status = U_ZERO_ERROR;
    UChar    text[100];


    // findNext() with a find progress callback function.

    re = uregex_openC(".z", 0, 0, &status);
    TEST_ASSERT_SUCCESS(status);

    u_uastrncpy(text, "Hello, World.",  UPRV_LENGTHOF(text));
    uregex_setText(re, text, -1, &status);
    TEST_ASSERT_SUCCESS(status);

    uregex_setFindProgressCallback(re, FindCallback, NULL, &status);
    TEST_ASSERT_SUCCESS(status);

    uregex_findNext(re, &status);
    TEST_ASSERT(status == U_REGEX_STOPPED_BY_CALLER);

    uregex_close(re);

    // findNext() with a match progress callback function.

    status = U_ZERO_ERROR;
    re = uregex_openC("((xxx)*)*y", 0, 0, &status);
    TEST_ASSERT_SUCCESS(status);

    // Pattern + this text gives an exponential time match. Without the callback to stop the match,
    // it will appear to be stuck in a (near) infinite loop.
    u_uastrncpy(text, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",  UPRV_LENGTHOF(text));
    uregex_setText(re, text, -1, &status);
    TEST_ASSERT_SUCCESS(status);

    uregex_setMatchCallback(re, MatchCallback, NULL, &status);
    TEST_ASSERT_SUCCESS(status);

    uregex_findNext(re, &status);
    TEST_ASSERT(status == U_REGEX_STOPPED_BY_CALLER);

    uregex_close(re);
}

#if APPLE_ICU_CHANGES
// rdar://
static const UChar startLinePattern[] = { 0x5E, 0x78, 0 }; // "^x"

static void TestMatchStartLineWithEmptyText() {
    UErrorCode status = U_ZERO_ERROR;
    UText* ut = utext_openUChars(NULL, NULL, 0, &status);
    TEST_ASSERT_SUCCESS(status);
    if (U_SUCCESS(status)) {
        URegularExpression *re = uregex_open(startLinePattern, -1, 0, NULL, &status);
        TEST_ASSERT_SUCCESS(status);
        if (U_SUCCESS(status)) {
            uregex_setUText(re, ut, &status);
            TEST_ASSERT(U_SUCCESS(status));
            if (U_SUCCESS(status)) {
                UBool found = uregex_findNext(re, &status);
                TEST_ASSERT(U_SUCCESS(status) && !found);
            }
            uregex_close(re);
        }
        utext_close(ut);
    }
}
#endif  // APPLE_ICU_CHANGES
    
#endif   /*  !UCONFIG_NO_REGULAR_EXPRESSIONS */

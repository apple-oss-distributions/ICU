// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/*
**********************************************************************
*   Copyright (C) 1997-2014, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*
* File USCRIPT.C
*
* Modification History:
*
*   Date        Name        Description
*   07/06/2001    Ram         Creation.
******************************************************************************
*/

#include "unicode/uchar.h"
#include "unicode/uscript.h"
#include "unicode/uloc.h"
#include "bytesinkutil.h"
#include "charstr.h"
#include "cmemory.h"
#include "cstring.h"
#include "ulocimp.h"

static const UScriptCode JAPANESE[3] = { USCRIPT_KATAKANA, USCRIPT_HIRAGANA, USCRIPT_HAN };
static const UScriptCode KOREAN[2] = { USCRIPT_HANGUL, USCRIPT_HAN };
static const UScriptCode HAN_BOPO[2] = { USCRIPT_HAN, USCRIPT_BOPOMOFO };

static int32_t
setCodes(const UScriptCode *src, int32_t length,
         UScriptCode *dest, int32_t capacity, UErrorCode *err) {
    int32_t i;
    if(U_FAILURE(*err)) { return 0; }
    if(length > capacity) {
        *err = U_BUFFER_OVERFLOW_ERROR;
        return length;
    }
    for(i = 0; i < length; ++i) {
        dest[i] = src[i];
    }
    return length;
}

static int32_t
setOneCode(UScriptCode script, UScriptCode *scripts, int32_t capacity, UErrorCode *err) {
    if(U_FAILURE(*err)) { return 0; }
    if(1 > capacity) {
        *err = U_BUFFER_OVERFLOW_ERROR;
        return 1;
    }
    scripts[0] = script;
    return 1;
}

static int32_t
getCodesFromLocale(const char *locale,
                   UScriptCode *scripts, int32_t capacity, UErrorCode *err) {
    UErrorCode internalErrorCode = U_ZERO_ERROR;
    char lang[8] = {0};
    char script[8] = {0};
    int32_t scriptLength;
    if(U_FAILURE(*err)) { return 0; }
    // Multi-script languages, equivalent to the LocaleScript data
    // that we used to load from locale resource bundles.
    /*length = */ uloc_getLanguage(locale, lang, UPRV_LENGTHOF(lang), &internalErrorCode);
    if(U_FAILURE(internalErrorCode) || internalErrorCode == U_STRING_NOT_TERMINATED_WARNING) {
        return 0;
    }
    if(0 == uprv_strcmp(lang, "ja")) {
        return setCodes(JAPANESE, UPRV_LENGTHOF(JAPANESE), scripts, capacity, err);
    }
    if(0 == uprv_strcmp(lang, "ko")) {
        return setCodes(KOREAN, UPRV_LENGTHOF(KOREAN), scripts, capacity, err);
    }
    scriptLength = uloc_getScript(locale, script, UPRV_LENGTHOF(script), &internalErrorCode);
    if(U_FAILURE(internalErrorCode) || internalErrorCode == U_STRING_NOT_TERMINATED_WARNING) {
        return 0;
    }
    if(0 == uprv_strcmp(lang, "zh") && 0 == uprv_strcmp(script, "Hant")) {
        return setCodes(HAN_BOPO, UPRV_LENGTHOF(HAN_BOPO), scripts, capacity, err);
    }
    // Explicit script code.
    if(scriptLength != 0) {
#if APPLE_ICU_CHANGES
// rdar://47494884 de70bade5d.. For ur and ks, support both Arab and Aran (aliased). Add some localized names for Aran.
        if (0 == uprv_strcmp(script, "Aran")) {
            uprv_strcpy(script, "Arab");
        }
#endif // APPLE_ICU_CHANGES
        UScriptCode scriptCode = (UScriptCode)u_getPropertyValueEnum(UCHAR_SCRIPT, script);
        if(scriptCode != USCRIPT_INVALID_CODE) {
            if(scriptCode == USCRIPT_SIMPLIFIED_HAN || scriptCode == USCRIPT_TRADITIONAL_HAN) {
                scriptCode = USCRIPT_HAN;
            }
            return setOneCode(scriptCode, scripts, capacity, err);
        }
    }
    return 0;
}

/* TODO: this is a bad API and should be deprecated, ticket #11141 */
U_CAPI int32_t  U_EXPORT2
uscript_getCode(const char* nameOrAbbrOrLocale,
                UScriptCode* fillIn,
                int32_t capacity,
                UErrorCode* err){
    UBool triedCode;
    UErrorCode internalErrorCode;
    int32_t length;

    if(U_FAILURE(*err)) {
        return 0;
    }
    if(nameOrAbbrOrLocale==nullptr ||
            (fillIn == nullptr ? capacity != 0 : capacity < 0)) {
        *err = U_ILLEGAL_ARGUMENT_ERROR;
        return 0;
    }
#if APPLE_ICU_CHANGES
// rdar://47494884 de70bade5d.. For ur and ks, support both Arab and Aran (aliased). Add some localized names for Aran.
    if (0 == uprv_strcmp(nameOrAbbrOrLocale, "Aran")) {
        nameOrAbbrOrLocale = "Arab";
    }
#endif // APPLE_ICU_CHANGES

    triedCode = false;
    const char* lastSepPtr = uprv_strrchr(nameOrAbbrOrLocale, '-');
    if (lastSepPtr==nullptr) {
        lastSepPtr = uprv_strrchr(nameOrAbbrOrLocale, '_');
    }
    // Favor interpretation of nameOrAbbrOrLocale as a script alias if either
    // 1. nameOrAbbrOrLocale does not contain -/_. Handles Han, Mro, Nko, etc.
    // 2. The last instance of -/_ is at offset 3, and the portion after that is
    //    longer than 4 characters (i.e. not a script or region code). This handles
    //    Old_Hungarian, Old_Italic, etc. ("old" is a valid language code)
    // 3. The last instance of -/_ is at offset 7, and the portion after that is
    //    3 characters. This handles New_Tai_Lue ("new" is a valid language code).
    if (lastSepPtr==nullptr
            || (lastSepPtr-nameOrAbbrOrLocale == 3 && uprv_strlen(nameOrAbbrOrLocale) > 8)
            || (lastSepPtr-nameOrAbbrOrLocale == 7 && uprv_strlen(nameOrAbbrOrLocale) == 11) ) {
        /* try long and abbreviated script names first */
        UScriptCode code = (UScriptCode) u_getPropertyValueEnum(UCHAR_SCRIPT, nameOrAbbrOrLocale);
        if(code!=USCRIPT_INVALID_CODE) {
            return setOneCode(code, fillIn, capacity, err);
        }
        triedCode = true;
    }
    internalErrorCode = U_ZERO_ERROR;
    length = getCodesFromLocale(nameOrAbbrOrLocale, fillIn, capacity, err);
    if(U_FAILURE(*err) || length != 0) {
        return length;
    }
    icu::CharString likely;
    {
        icu::CharStringByteSink sink(&likely);
        ulocimp_addLikelySubtags(nameOrAbbrOrLocale, sink, &internalErrorCode);
    }
    if(U_SUCCESS(internalErrorCode) && internalErrorCode != U_STRING_NOT_TERMINATED_WARNING) {
        length = getCodesFromLocale(likely.data(), fillIn, capacity, err);
        if(U_FAILURE(*err) || length != 0) {
            return length;
        }
    }
    if(!triedCode) {
        /* still not found .. try long and abbreviated script names again */
        UScriptCode code = (UScriptCode) u_getPropertyValueEnum(UCHAR_SCRIPT, nameOrAbbrOrLocale);
        if(code!=USCRIPT_INVALID_CODE) {
            return setOneCode(code, fillIn, capacity, err);
        }
    }
    return 0;
}

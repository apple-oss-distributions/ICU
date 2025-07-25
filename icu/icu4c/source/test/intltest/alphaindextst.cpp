// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 2012-2016, International Business Machines Corporation
 * and others. All Rights Reserved.
 ********************************************************************/
//
//   file:  alphaindextst.cpp
//          Alphabetic Index Tests.
//
//   Note: please... no character literals cast to UChars.. use (char16_t)0xZZZZ

#include <stdio.h>  // for snprintf

#include "intltest.h"
#include "alphaindextst.h"
#include "cmemory.h"

#include "unicode/alphaindex.h"
#include "unicode/coll.h"
#include "unicode/localpointer.h"
#include "unicode/tblcoll.h"
#include "unicode/uniset.h"
#include "unicode/uscript.h"

#if !UCONFIG_NO_COLLATION && !UCONFIG_NO_NORMALIZATION

// #include <string>
// #include <iostream>

namespace {

UnicodeString joinLabelsAndAppend(AlphabeticIndex::ImmutableIndex &index, UnicodeString &dest) {
    int32_t oldLength = dest.length();
    const AlphabeticIndex::Bucket *bucket;
    for (int32_t i = 0; (bucket = index.getBucket(i)) != nullptr; ++i) {
        if (dest.length() > oldLength) {
            dest.append(static_cast<char16_t>(0x3A)); // ':'
        }
        dest.append(bucket->getLabel());
    }
    return dest;
}

}  // namespace

AlphabeticIndexTest::AlphabeticIndexTest() {
}

AlphabeticIndexTest::~AlphabeticIndexTest() {
}

void AlphabeticIndexTest::runIndexedTest( int32_t index, UBool exec, const char* &name, char* /*par*/ )
{
    if (exec) logln("TestSuite AlphabeticIndex: ");
    TESTCASE_AUTO_BEGIN;
    TESTCASE_AUTO(APITest);
    TESTCASE_AUTO(ManyLocalesTest);
    TESTCASE_AUTO(HackPinyinTest);
    TESTCASE_AUTO(TestBug9009);
    TESTCASE_AUTO(TestIndexCharactersList);
    TESTCASE_AUTO(TestHaniFirst);
    TESTCASE_AUTO(TestPinyinFirst);
    TESTCASE_AUTO(TestSchSt);
    TESTCASE_AUTO(TestNoLabels);
    TESTCASE_AUTO(TestChineseZhuyin);
    TESTCASE_AUTO(TestJapaneseKanji);
    TESTCASE_AUTO(TestChineseUnihan);
    TESTCASE_AUTO(testHasBuckets);
    TESTCASE_AUTO_END;
}

#define TEST_CHECK_STATUS UPRV_BLOCK_MACRO_BEGIN { \
    if (U_FAILURE(status)) { \
        dataerrln("%s:%d: Test failure.  status=%s", \
                  __FILE__, __LINE__, u_errorName(status)); \
        return; \
    } \
} UPRV_BLOCK_MACRO_END

#define TEST_ASSERT(expr) UPRV_BLOCK_MACRO_BEGIN { \
    if ((expr)==false) { \
        errln("%s:%d: Test failure \n", __FILE__, __LINE__); \
    } \
} UPRV_BLOCK_MACRO_END

//
//  APITest.   Invoke every function at least once, and check that it does something.
//             Does not attempt to check complete functionality.
//
void AlphabeticIndexTest::APITest() {
    //
    //  Simple constructor and destructor,  getBucketCount()
    //
    UErrorCode status = U_ZERO_ERROR;
    int32_t lc = 0;
    int32_t i  = 0;
    LocalPointer<AlphabeticIndex> index(new AlphabeticIndex(Locale::getEnglish(), status));
    TEST_CHECK_STATUS;
    lc = index->getBucketCount(status);
    TEST_CHECK_STATUS;
    TEST_ASSERT(28 == lc);    // 26 letters plus two under/overflow labels.
    //printf("getBucketCount() == %d\n", lc);
    index.adoptInstead(nullptr);

    // Constructor from a Collator
    //
    status = U_ZERO_ERROR;
    LocalPointer<RuleBasedCollator> coll(dynamic_cast<RuleBasedCollator *>(
        Collator::createInstance(Locale::getGerman(), status)), status);
    TEST_CHECK_STATUS;
    TEST_ASSERT(coll.isValid());
    RuleBasedCollator *originalColl = coll.getAlias();
    index.adoptInstead(new AlphabeticIndex(coll.orphan(), status));
    TEST_CHECK_STATUS;
    TEST_ASSERT(originalColl == &index->getCollator());
    assertEquals("only the underflow label in an index built from a collator",
                 1, index->getBucketCount(status));
    TEST_CHECK_STATUS;
    index.adoptInstead(nullptr);
    

    // addLabels()

    status = U_ZERO_ERROR;
    index.adoptInstead(new AlphabeticIndex(Locale::getEnglish(), status));
    TEST_CHECK_STATUS;
    UnicodeSet additions;
    additions.add(static_cast<UChar32>(0x410)).add(static_cast<UChar32>(0x415)); // A couple of Cyrillic letters
    index->addLabels(additions, status);
    TEST_CHECK_STATUS;
    lc = index->getBucketCount(status);
    TEST_CHECK_STATUS;
    assertEquals("underflow, A-Z, inflow, 2 Cyrillic, overflow",
                 31, index->getBucketCount(status));
    // std::cout << lc << std::endl;
    index.adoptInstead(nullptr);


    // addLabels(Locale)

    status = U_ZERO_ERROR;
    index.adoptInstead(new AlphabeticIndex(Locale::getEnglish(), status));
    TEST_CHECK_STATUS;
    AlphabeticIndex &aip = index->addLabels(Locale::getJapanese(), status);
    TEST_ASSERT(&aip == index.getAlias());
    TEST_CHECK_STATUS;
    lc = index->getBucketCount(status);
    TEST_CHECK_STATUS;
    TEST_ASSERT(35 < lc);  // Japanese should add a bunch.  Don't rely on the exact value.
    index.adoptInstead(nullptr);

    // GetCollator(),  Get under/in/over flow labels

    status = U_ZERO_ERROR;
    index.adoptInstead(new AlphabeticIndex(Locale::getGerman(), status));
    TEST_CHECK_STATUS;
    LocalPointer<Collator> germanCol(Collator::createInstance(Locale::getGerman(), status));
    TEST_CHECK_STATUS;
    const RuleBasedCollator &indexCol = index->getCollator();
    TEST_ASSERT(*germanCol == indexCol);

    UnicodeString ELLIPSIS;  ELLIPSIS.append(static_cast<UChar32>(0x2026));
    UnicodeString s = index->getUnderflowLabel();
    TEST_ASSERT(ELLIPSIS == s);
    s = index->getOverflowLabel();
    TEST_ASSERT(ELLIPSIS == s);
    s = index->getInflowLabel();
    TEST_ASSERT(ELLIPSIS == s);
    index->setOverflowLabel(UNICODE_STRING_SIMPLE("O"), status);
    index->setUnderflowLabel(UNICODE_STRING_SIMPLE("U"), status).setInflowLabel(UNICODE_STRING_SIMPLE("I"), status);
    s = index->getUnderflowLabel();
    TEST_ASSERT(UNICODE_STRING_SIMPLE("U") == s);
    s = index->getOverflowLabel();
    TEST_ASSERT(UNICODE_STRING_SIMPLE("O") == s);
    s = index->getInflowLabel();
    TEST_ASSERT(UNICODE_STRING_SIMPLE("I") == s);




    index.adoptInstead(nullptr);



    const UnicodeString adam = UNICODE_STRING_SIMPLE("Adam");
    const UnicodeString baker = UNICODE_STRING_SIMPLE("Baker");
    const UnicodeString charlie = UNICODE_STRING_SIMPLE("Charlie");
    const UnicodeString chad = UNICODE_STRING_SIMPLE("Chad");
    const UnicodeString zed  = UNICODE_STRING_SIMPLE("Zed");
    const UnicodeString Cyrillic = UNICODE_STRING_SIMPLE("\\u0410\\u0443\\u0435").unescape();

    // addRecord(), verify that it comes back out.
    //
    status = U_ZERO_ERROR;
    index.adoptInstead(new AlphabeticIndex(Locale::getEnglish(), status));
    TEST_CHECK_STATUS;
    index->addRecord(UnicodeString("Adam"), this, status);
    UBool   b;
    TEST_CHECK_STATUS;
    index->resetBucketIterator(status);
    TEST_CHECK_STATUS;
    index->nextBucket(status);  // Move to underflow label
    index->nextBucket(status);  // Move to "A"
    TEST_CHECK_STATUS;
    const UnicodeString &label2 = index->getBucketLabel();
    UnicodeString A_STR = UNICODE_STRING_SIMPLE("A");
    TEST_ASSERT(A_STR == label2);

    b = index->nextRecord(status);
    TEST_CHECK_STATUS;
    TEST_ASSERT(b);
    const UnicodeString &itemName = index->getRecordName();
    TEST_ASSERT(adam == itemName);

    const void *itemContext = index->getRecordData();
    TEST_ASSERT(itemContext == this);

    index.adoptInstead(nullptr);

    // clearRecords, addRecord(), Iteration

    status = U_ZERO_ERROR;
    index.adoptInstead(new AlphabeticIndex(Locale::getEnglish(), status));
    TEST_CHECK_STATUS;
    while (index->nextBucket(status)) {
        TEST_CHECK_STATUS;
        while (index->nextRecord(status)) {
            TEST_CHECK_STATUS;
            TEST_ASSERT(false);   // No items have been added.
        }
        TEST_CHECK_STATUS;
    }

    index->addRecord(adam, nullptr, status);
    index->addRecord(baker, nullptr, status);
    index->addRecord(charlie, nullptr, status);
    index->addRecord(chad, nullptr, status);
    TEST_CHECK_STATUS;
    int itemCount = 0;
    index->resetBucketIterator(status);
    while (index->nextBucket(status)) {
        TEST_CHECK_STATUS;
        while (index->nextRecord(status)) {
            TEST_CHECK_STATUS;
            ++itemCount;
        }
    }
    TEST_CHECK_STATUS;
    TEST_ASSERT(itemCount == 4);

    TEST_ASSERT(index->nextBucket(status) == false);
    index->resetBucketIterator(status);
    TEST_CHECK_STATUS;
    TEST_ASSERT(index->nextBucket(status) == true);

    index->clearRecords(status);
    TEST_CHECK_STATUS;
    index->resetBucketIterator(status);
    while (index->nextBucket(status)) {
        TEST_CHECK_STATUS;
        while (index->nextRecord(status)) {
            TEST_ASSERT(false);   // No items have been added.
        }
    }
    TEST_CHECK_STATUS;
    index.adoptInstead(nullptr);

    // getBucketLabel(), getBucketType()

    status = U_ZERO_ERROR;
    index.adoptInstead(new AlphabeticIndex(Locale::getEnglish(), status));
    TEST_CHECK_STATUS;
    index->setUnderflowLabel(adam, status).setOverflowLabel(charlie, status);
    TEST_CHECK_STATUS;
    for (i=0; index->nextBucket(status); i++) {
        TEST_CHECK_STATUS;
        UnicodeString label = index->getBucketLabel();
        UAlphabeticIndexLabelType type = index->getBucketLabelType();
        if (i == 0) {
            TEST_ASSERT(type == U_ALPHAINDEX_UNDERFLOW);
            TEST_ASSERT(label == adam);
        } else if (i <= 26) {
            // Labels A - Z for English locale
            TEST_ASSERT(type == U_ALPHAINDEX_NORMAL);
            UnicodeString expectedLabel(static_cast<char16_t>(0x40 + i));
            TEST_ASSERT(expectedLabel == label);
        } else if (i == 27) {
            TEST_ASSERT(type == U_ALPHAINDEX_OVERFLOW);
            TEST_ASSERT(label == charlie);
        } else {
            TEST_ASSERT(false);
        }
    }
    TEST_ASSERT(i==28);
    index.adoptInstead(nullptr);

    // getBucketIndex()

    status = U_ZERO_ERROR;
    index.adoptInstead(new AlphabeticIndex(Locale::getEnglish(), status));
    TEST_CHECK_STATUS;
    int32_t n = index->getBucketIndex(adam, status);
    TEST_CHECK_STATUS;
    TEST_ASSERT(n == 1);    /*  Label #0 is underflow, 1 is A, etc. */
    n = index->getBucketIndex(baker, status);
    TEST_ASSERT(n == 2);
    n = index->getBucketIndex(Cyrillic, status);
    TEST_ASSERT(n == 27);   // Overflow label
    n = index->getBucketIndex(zed, status);
    TEST_ASSERT(n == 26);

    for (i=0; index->nextBucket(status); i++) {
        n = index->getBucketIndex();
        TEST_ASSERT(n == i);
        UnicodeString label = index->getBucketLabel();
        TEST_ASSERT(n == i);
    }
    TEST_ASSERT(i == 28);

    index.adoptInstead(nullptr);
    index.adoptInstead(new AlphabeticIndex(Locale::createFromName("ru"), status));
    TEST_CHECK_STATUS;
    assertEquals("Russian index.getBucketCount()", 32, index->getBucketCount(status));
    // Latin-script names should go into the underflow label (0)
    // if the Russian collation does not use script reordering,
    // but into the overflow label (getBucketCount()-1)
    // if Russian sorts Cyrillic first.
    int32_t reorderCodes[20];
    int32_t expectedLatinIndex = 0;
    if (index->getCollator().getReorderCodes(reorderCodes, UPRV_LENGTHOF(reorderCodes), status) > 0) {
        expectedLatinIndex = index->getBucketCount(status) - 1;
    }
    n = index->getBucketIndex(adam, status);
    TEST_CHECK_STATUS;
    assertEquals("Russian index.getBucketIndex(adam)", expectedLatinIndex, n);
    n = index->getBucketIndex(baker, status);
    assertEquals("Russian index.getBucketIndex(baker)", expectedLatinIndex, n);
    n = index->getBucketIndex(Cyrillic, status);
    assertEquals("Russian index.getBucketIndex(Cyrillic)", 1, n);
    n = index->getBucketIndex(zed, status);
    assertEquals("Russian index.getBucketIndex(zed)", expectedLatinIndex, n);

}


static const char * KEY_LOCALES[] = {
            "en", "es", "de", "fr", "ja", "it", "tr", "pt", "zh", "nl",
            "pl", "ar", "ru", "zh_Hant", "ko", "th", "sv", "fi", "da",
            "he", "nb", "el", "hr", "bg", "sk", "lt", "vi", "lv", "sr",
            "pt_PT", "ro", "hu", "cs", "id", "sl", "fil", "fa", "uk",
            "ca", "hi", "et", "eu", "is", "sw", "ms", "bn", "am", "ta",
            "te", "mr", "ur", "ml", "kn", "gu", "or", ""};


void AlphabeticIndexTest::ManyLocalesTest() {
    UErrorCode status = U_ZERO_ERROR;
    int32_t  lc = 0;

    for (int i=0; ; ++i) {
        status = U_ZERO_ERROR;
        const char *localeName = KEY_LOCALES[i];
        if (localeName[0] == 0) {
            break;
        }
        // std::cout <<  localeName << "  ";
        Locale loc = Locale::createFromName(localeName);
        AlphabeticIndex index(loc, status);
        TEST_CHECK_STATUS;
        lc = index.getBucketCount(status);
        TEST_CHECK_STATUS;
        // std::cout << "getBucketCount() == " << lc << std::endl;

        LocalPointer<AlphabeticIndex::ImmutableIndex> immIndex(index.buildImmutableIndex(status));
        TEST_CHECK_STATUS;
        TEST_ASSERT(lc == immIndex->getBucketCount());

        assertEquals("initial bucket index", -1, index.getBucketIndex());
        int32_t bucketIndex = 0;
        while (index.nextBucket(status)) {
            TEST_CHECK_STATUS;
            assertEquals("bucket index", bucketIndex, index.getBucketIndex());
            const UnicodeString &label = index.getBucketLabel();
            TEST_ASSERT(label.length()>0);
            // std::string ss;
            // std::cout << ":" << label.toUTF8String(ss);
            const AlphabeticIndex::Bucket *bucket = immIndex->getBucket(bucketIndex);
            TEST_ASSERT(bucket != nullptr);
            assertEquals("bucket label vs. immutable: locale=" + UnicodeString(localeName) +
                         " index=" + bucketIndex,
                         label, bucket->getLabel());
            TEST_ASSERT(&label != &bucket->getLabel());  // not the same pointers
            UAlphabeticIndexLabelType labelType = index.getBucketLabelType();
            TEST_ASSERT(labelType == bucket->getLabelType());
            ++bucketIndex;
        }
        // std::cout << ":" << std::endl;

        TEST_ASSERT(immIndex->getBucketCount() == bucketIndex);
        TEST_ASSERT(immIndex->getBucket(-1) == nullptr);
        TEST_ASSERT(immIndex->getBucket(bucketIndex) == nullptr);
    }
}


// Test data for Pinyin based indexes.
//  The Chinese characters should be distributed under latin labels in
//  an index.

static const char *pinyinTestData[] = { 
        "\\u0101", "\\u5416", "\\u58ba", //
        "b", "\\u516b", "\\u62d4", "\\u8500", //
        "c", "\\u5693", "\\u7938", "\\u9e7e", //
        "d", "\\u5491", "\\u8fcf", "\\u964a", //
        "\\u0113","\\u59b8", "\\u92e8", "\\u834b", //
        "f", "\\u53d1", "\\u9197", "\\u99a5", //
        "g", "\\u7324", "\\u91d3", "\\u8142", //
        "h", "\\u598e", "\\u927f", "\\u593b", //
        "j", "\\u4e0c", "\\u6785", "\\u9d58", //
        "k", "\\u5494", "\\u958b", "\\u7a52", //
        "l", "\\u5783", "\\u62c9", "\\u9ba5", //
        "m", "\\u5638", "\\u9ebb", "\\u65c0", //
        "n", "\\u62ff", "\\u80ad", "\\u685b", //
        "\\u014D", "\\u5662", "\\u6bee", "\\u8bb4", //
        "p", "\\u5991", "\\u8019", "\\u8c31", //
        "q", "\\u4e03", "\\u6053", "\\u7f56", //
        "r", "\\u5465", "\\u72aa", "\\u6e03", //
        "s", "\\u4ee8", "\\u9491", "\\u93c1", //
        "t", "\\u4ed6", "\\u9248", "\\u67dd", //
        "w", "\\u5c72", "\\u5558", "\\u5a7a", //
        "x", "\\u5915", "\\u5438", "\\u6bbe", //
        "y", "\\u4e2b", "\\u82bd", "\\u8574", //
        "z", "\\u5e00", "\\u707d", "\\u5c0a",
        nullptr
    };

void AlphabeticIndexTest::HackPinyinTest() {
    UErrorCode status = U_ZERO_ERROR;
    AlphabeticIndex aindex(Locale::createFromName("zh"), status);
    TEST_CHECK_STATUS; 

    UnicodeString names[UPRV_LENGTHOF(pinyinTestData)];
    int32_t  nameCount;
    for (nameCount=0; pinyinTestData[nameCount] != nullptr; nameCount++) {
        names[nameCount] = UnicodeString(pinyinTestData[nameCount], -1, UnicodeString::kInvariant).unescape();
        aindex.addRecord(names[nameCount], &names[nameCount], status);
        TEST_CHECK_STATUS; 
        if (U_FAILURE(status)) {
            return;
        }
    }
    TEST_ASSERT(nameCount == aindex.getRecordCount(status));
    
    // Weak checking:  make sure that none of the Chinese names landed in the overflow bucket
    //   of the index, and that the names are distributed among several buckets.
    //   (Exact expected data would be subject to change with evolution of the collation rules.)

    int32_t bucketCount = 0;
    int32_t filledBucketCount = 0;
    while (aindex.nextBucket(status)) {
        bucketCount++;
        UnicodeString label = aindex.getBucketLabel();
        // std::string s;
        // std::cout << label.toUTF8String(s) << ":  ";

        UBool  bucketHasContents = false;
        while (aindex.nextRecord(status)) {
            bucketHasContents = true;
            UnicodeString name = aindex.getRecordName();
            if (aindex.getBucketLabelType() != U_ALPHAINDEX_NORMAL) {
                errln("File %s, Line %d, Name \"\\u%x\" is in an under or overflow bucket.",
                    __FILE__, __LINE__, name.char32At(0));
            }
            // s.clear();
            // std::cout << aindex.getRecordName().toUTF8String(s) << " ";
        }
        if (bucketHasContents) {
            filledBucketCount++;
        }
        // std::cout << std::endl;
    }
    TEST_ASSERT(bucketCount > 25);
    TEST_ASSERT(filledBucketCount > 15);
}


void AlphabeticIndexTest::TestBug9009() {
    UErrorCode status = U_ZERO_ERROR;
    Locale loc("root");
    AlphabeticIndex aindex(loc, status);
    TEST_CHECK_STATUS; 
    aindex.nextBucket(status);  // Crash here before bug was fixed.
    TEST_CHECK_STATUS; 
}

static const char *localeAndIndexCharactersLists[][2] = {
    /* Arabic*/ {"ar", "\\u0627:\\u0628:\\u062A:\\u062B:\\u062C:\\u062D:\\u062E:\\u062F:\\u0630:\\u0631:\\u0632:\\u0633:\\u0634:\\u0635:\\u0636:\\u0637:\\u0638:\\u0639:\\u063A:\\u0641:\\u0642:\\u0643:\\u0644:\\u0645:\\u0646:\\u0647:\\u0648:\\u064A"},
    /* Bulgarian*/  {"bg", "\\u0410:\\u0411:\\u0412:\\u0413:\\u0414:\\u0415:\\u0416:\\u0417:\\u0418:\\u0419:\\u041A:\\u041B:\\u041C:\\u041D:\\u041E:\\u041F:\\u0420:\\u0421:\\u0422:\\u0423:\\u0424:\\u0425:\\u0426:\\u0427:\\u0428:\\u0429:\\u042E:\\u042F"},
    /* Catalan*/    {"ca", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Czech*/  {"cs", "A:B:C:\\u010C:D:E:F:G:H:CH:I:J:K:L:M:N:O:P:Q:R:\\u0158:S:\\u0160:T:U:V:W:X:Y:Z:\\u017D"},
    /* Danish*/ {"da", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z:\\u00C6:\\u00D8:\\u00C5"},
    /* German*/ {"de", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Greek*/  {"el", "\\u0391:\\u0392:\\u0393:\\u0394:\\u0395:\\u0396:\\u0397:\\u0398:\\u0399:\\u039A:\\u039B:\\u039C:\\u039D:\\u039E:\\u039F:\\u03A0:\\u03A1:\\u03A3:\\u03A4:\\u03A5:\\u03A6:\\u03A7:\\u03A8:\\u03A9"},
    /* English*/    {"en", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Spanish*/    {"es", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:\\u00D1:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Estonian*/   {"et", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:\\u0160:Z:\\u017D:T:U:V:W:\\u00D5:\\u00C4:\\u00D6:\\u00DC:X:Y"},
    /* Basque*/ {"eu", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Finnish*/    {"fi", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z:\\u00C5:\\u00C4:\\u00D6"},
    /* Filipino*/   {"fil", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:\\u00D1:Ng:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* French*/ {"fr", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Hebrew*/ {"he", "\\u05D0:\\u05D1:\\u05D2:\\u05D3:\\u05D4:\\u05D5:\\u05D6:\\u05D7:\\u05D8:\\u05D9:\\u05DB:\\u05DC:\\u05DE:\\u05E0:\\u05E1:\\u05E2:\\u05E4:\\u05E6:\\u05E7:\\u05E8:\\u05E9:\\u05EA"},
    /* Icelandic*/  {"is", "A:\\u00C1:B:C:D:\\u00D0:E:\\u00C9:F:G:H:I:\\u00CD:J:K:L:M:N:O:\\u00D3:P:Q:R:S:T:U:\\u00DA:V:W:X:Y:\\u00DD:Z:\\u00DE:\\u00C6:\\u00D6"},
    /* Italian*/    {"it", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Japanese*/   {"ja", "\\u3042:\\u304B:\\u3055:\\u305F:\\u306A:\\u306F:\\u307E:\\u3084:\\u3089:\\u308F"},
    /* Korean*/ {"ko", "\\u3131:\\u3134:\\u3137:\\u3139:\\u3141:\\u3142:\\u3145:\\u3147:\\u3148:\\u314A:\\u314B:\\u314C:\\u314D:\\u314E"},
    /* Lithuanian*/ {"lt", "A:B:C:\\u010C:D:E:F:G:H:I:J:K:L:M:N:O:P:R:S:\\u0160:T:U:V:Z:\\u017D"},
    /* Latvian*/    {"lv", "A:\\u0100:B:C:\\u010C:D:E:\\u0112:F:G:\\u0122:H:I:Y:\\u012A:J:K:\\u0136:L:\\u013B:M:N:\\u0145:O:P:Q:R:S:\\u0160:T:U:\\u016A:V:W:X:Z:\\u017D"},
    /* Norwegian Bokm\\u00E5l*/  {"nb", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z:\\u00C6:\\u00D8:\\u00C5"},
    /* Dutch*/  {"nl", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Polish*/ {"pl", "A:B:C:\\u0106:D:E:F:G:H:I:J:K:L:\\u0141:M:N:O:\\u00D3:P:Q:R:S:\\u015A:T:U:V:W:X:Y:Z:\\u0179:\\u017B"},
    /* Portuguese*/ {"pt", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
    /* Romanian*/   {"ro", "A:\\u0102:\\u00C2:B:C:D:E:F:G:H:I:\\u00CE:J:K:L:M:N:O:P:Q:R:S:\\u0218:T:\\u021A:U:V:W:X:Y:Z"},
    /* Russian*/    {"ru", "\\u0410:\\u0411:\\u0412:\\u0413:\\u0414:\\u0415:\\u0416:\\u0417:\\u0418:\\u0419:\\u041A:\\u041B:\\u041C:\\u041D:\\u041E:\\u041F:\\u0420:\\u0421:\\u0422:\\u0423:\\u0424:\\u0425:\\u0426:\\u0427:\\u0428:\\u0429:\\u042B:\\u042D:\\u042E:\\u042F"},
    /* Slovak*/ {"sk", "A:\\u00C4:B:C:\\u010C:D:E:F:G:H:CH:I:J:K:L:M:N:O:\\u00D4:P:Q:R:S:\\u0160:T:U:V:W:X:Y:Z:\\u017D"},
    /* Slovenian*/  {"sl", "A:B:C:\\u010C:\\u0106:D:\\u0110:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:\\u0160:T:U:V:W:X:Y:Z:\\u017D"},
    /* Serbian*/    {"sr", "\\u0410:\\u0411:\\u0412:\\u0413:\\u0414:\\u0402:\\u0415:\\u0416:\\u0417:\\u0418:\\u0408:\\u041A:\\u041B:\\u0409:\\u041C:\\u041D:\\u040A:\\u041E:\\u041F:\\u0420:\\u0421:\\u0422:\\u040B:\\u0423:\\u0424:\\u0425:\\u0426:\\u0427:\\u040F:\\u0428"},
    /* Swedish*/    {"sv", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z:\\u00C5:\\u00C4:\\u00D6"},
    /* Turkish*/    {"tr", "A:B:C:\\u00C7:D:E:F:G:H:I:\\u0130:J:K:L:M:N:O:\\u00D6:P:Q:R:S:\\u015E:T:U:\\u00DC:V:W:X:Y:Z"},
    /* Ukrainian*/  {"uk", "\\u0410:\\u0411:\\u0412:\\u0413:\\u0490:\\u0414:\\u0415:\\u0404:\\u0416:\\u0417:\\u0418:\\u0406:\\u0407:\\u0419:\\u041A:\\u041B:\\u041C:\\u041D:\\u041E:\\u041F:\\u0420:\\u0421:\\u0422:\\u0423:\\u0424:\\u0425:\\u0426:\\u0427:\\u0428:\\u0429:\\u042E:\\u042F"},
    /* Vietnamese*/ {"vi", "A:\\u0102:\\u00C2:B:C:D:\\u0110:E:\\u00CA:F:G:H:I:J:K:L:M:N:O:\\u00D4:\\u01A0:P:Q:R:S:T:U:\\u01AF:V:W:X:Y:Z"},
    /* Chinese*/    {"zh", "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z"},
#if APPLE_ICU_CHANGES
// rdar://104369003 use full pinyin/stroke collations
    /* Chinese (Traditional Han)*/  {"zh_Hant", "1\\u5283:2\\u5283:3\\u5283:4\\u5283:5\\u5283:6\\u5283:7\\u5283:8\\u5283:9\\u5283:10\\u5283:11\\u5283:12\\u5283:13\\u5283:14\\u5283:15\\u5283:16\\u5283:17\\u5283:18\\u5283:19\\u5283:20\\u5283:21\\u5283:22\\u5283:23\\u5283:24\\u5283:25\\u5283:26\\u5283:27\\u5283:28\\u5283:29\\u5283:30\\u5283:31\\u5283:32\\u5283:33\\u5283:34\\u5283:35\\u5283:36\\u5283:37\\u5283:38\\u5283:39\\u5283:40\\u5283:41\\u5283:42\\u5283:43\\u5283:44\\u5283:46\\u5283:48\\u5283:52\\u5283:53\\u5283:58\\u5283:64\\u5283:76\\u5283:84\\u5283"},
#else
    /* Chinese (Traditional Han)*/  {"zh_Hant", "1\\u5283:2\\u5283:3\\u5283:4\\u5283:5\\u5283:6\\u5283:7\\u5283:8\\u5283:9\\u5283:10\\u5283:11\\u5283:12\\u5283:13\\u5283:14\\u5283:15\\u5283:16\\u5283:17\\u5283:18\\u5283:19\\u5283:20\\u5283:21\\u5283:22\\u5283:23\\u5283:24\\u5283:25\\u5283:26\\u5283:27\\u5283:28\\u5283:29\\u5283:30\\u5283:31\\u5283:32\\u5283:33\\u5283:35\\u5283:36\\u5283:39\\u5283:48\\u5283"},
#endif  // APPLE_ICU_CHANGES
};

void AlphabeticIndexTest::TestIndexCharactersList() {
    UErrorCode status = U_ZERO_ERROR;
    for (int32_t i = 0; i < UPRV_LENGTHOF(localeAndIndexCharactersLists); ++i) {
        const char *(&localeAndIndexCharacters)[2] = localeAndIndexCharactersLists[i];
        const char *locale = localeAndIndexCharacters[0];
        UnicodeString expectedIndexCharacters
            = (UnicodeString("\\u2026:") + localeAndIndexCharacters[1] + ":\\u2026").unescape();
        AlphabeticIndex index(locale, status);
        TEST_CHECK_STATUS;
        LocalPointer<AlphabeticIndex::ImmutableIndex> immIndex(index.buildImmutableIndex(status));
        TEST_CHECK_STATUS;

        // Join the elements of the list to a string with delimiter ":"
        UnicodeString actualIndexCharacters;
        assertEquals(locale,
                     expectedIndexCharacters,
                     joinLabelsAndAppend(*immIndex, actualIndexCharacters));
        logln(locale + UnicodeString(": ") + actualIndexCharacters);
    }
}

void AlphabeticIndexTest::TestHaniFirst() {
    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<RuleBasedCollator> coll(
        dynamic_cast<RuleBasedCollator *>(Collator::createInstance(Locale::getRoot(), status)));

    if (U_FAILURE(status)) {
        dataerrln("Failed Collator::createInstance call - %s", u_errorName(status));
        return;
    }
    int32_t reorderCodes[] = { USCRIPT_HAN };
    coll->setReorderCodes(reorderCodes, UPRV_LENGTHOF(reorderCodes), status);
    TEST_CHECK_STATUS; 
    AlphabeticIndex index(coll.orphan(), status);
    TEST_CHECK_STATUS; 
    assertEquals("getBucketCount()", 1, index.getBucketCount(status));   // ... (underflow only)
    index.addLabels(Locale::getEnglish(), status);
    assertEquals("getBucketCount()", 28, index.getBucketCount(status));  // ... A-Z ...
    int32_t bucketIndex = index.getBucketIndex(UnicodeString(static_cast<char16_t>(0x897f)), status);
    assertEquals("getBucketIndex(U+897F)", 0, bucketIndex);  // underflow bucket
    bucketIndex = index.getBucketIndex("i", status);
    assertEquals("getBucketIndex(i)", 9, bucketIndex);
    bucketIndex = index.getBucketIndex(UnicodeString(static_cast<char16_t>(0x03B1)), status);
    assertEquals("getBucketIndex(Greek alpha)", 27, bucketIndex);
    // U+50005 is an unassigned code point which sorts at the end, independent of the Hani group.
    bucketIndex = index.getBucketIndex(UnicodeString(static_cast<UChar32>(0x50005)), status);
    assertEquals("getBucketIndex(U+50005)", 27, bucketIndex);
    bucketIndex = index.getBucketIndex(UnicodeString(static_cast<char16_t>(0xFFFF)), status);
    assertEquals("getBucketIndex(U+FFFF)", 27, bucketIndex);
}

void AlphabeticIndexTest::TestPinyinFirst() {
    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<RuleBasedCollator> coll(
        dynamic_cast<RuleBasedCollator *>(Collator::createInstance(Locale::getChinese(), status)));
    if (U_FAILURE(status)) {
        dataerrln("Failed Collator::createInstance call - %s", u_errorName(status));
        return;
    }
    int32_t reorderCodes[] = { USCRIPT_HAN };
    coll->setReorderCodes(reorderCodes, UPRV_LENGTHOF(reorderCodes), status);
    TEST_CHECK_STATUS;
    AlphabeticIndex index(coll.orphan(), status);
    TEST_CHECK_STATUS;
    assertEquals("getBucketCount()", 28, index.getBucketCount(status));   // ... A-Z ...
    index.addLabels(Locale::getChinese(), status);
    assertEquals("getBucketCount()", 28, index.getBucketCount(status));  // ... A-Z ...
    int32_t bucketIndex = index.getBucketIndex(UnicodeString(static_cast<char16_t>(0x897f)), status);
    assertEquals("getBucketIndex(U+897F)", static_cast<int32_t>(static_cast<char16_t>(0x0058) /*X*/ - static_cast<char16_t>(0x0041) /*A*/ + 1), bucketIndex);
    bucketIndex = index.getBucketIndex("i", status);
    assertEquals("getBucketIndex(i)", 9, bucketIndex);
    bucketIndex = index.getBucketIndex(UnicodeString(static_cast<char16_t>(0x03B1)), status);
    assertEquals("getBucketIndex(Greek alpha)", static_cast<int32_t>(27), bucketIndex);
    // U+50005 is an unassigned code point which sorts at the end, independent of the Hani group.
    bucketIndex = index.getBucketIndex(UnicodeString(static_cast<UChar32>(0x50005)), status);
    assertEquals("getBucketIndex(U+50005)", 27, bucketIndex);
    bucketIndex = index.getBucketIndex(UnicodeString(static_cast<char16_t>(0xFFFF)), status);
    assertEquals("getBucketIndex(U+FFFF)", 27, bucketIndex);
}

void AlphabeticIndexTest::TestSchSt() {
    UErrorCode status = U_ZERO_ERROR;
    AlphabeticIndex index(Locale::getGerman(), status);
    index.addLabels(UnicodeSet("[\\u00C6{Sch*}{St*}]", status), status);
    TEST_CHECK_STATUS; 
    // ... A AE-ligature B-R S Sch St T-Z ...
    LocalPointer<AlphabeticIndex::ImmutableIndex> immIndex(index.buildImmutableIndex(status));
    TEST_CHECK_STATUS; 
    assertEquals("getBucketCount()", 31, index.getBucketCount(status));
    assertEquals("immutable getBucketCount()", 31, immIndex->getBucketCount());
    static const struct TestCase {
        const char *name;
        int32_t bucketIndex;
        const char *bucketLabel;
    } testCases[] = {
        // name, bucket index, bucket label
        { "Adelbert", 1, "A" },
        { "Afrika", 1, "A" },
        { "\\u00C6sculap", 2, "\\u00C6" },
        { "Aesthet", 2, "\\u00C6" },
        { "Berlin", 3, "B" },
        { "Rilke", 19, "R" },
        { "Sacher", 20, "S" },
        { "Seiler", 20, "S" },
        { "Sultan", 20, "S" },
        { "Schiller", 21, "Sch" },
        { "Steiff", 22, "St" },
        { "Thomas", 23, "T" }
    };
    for (int32_t i = 0; i < UPRV_LENGTHOF(testCases); ++i) {
        const TestCase &testCase = testCases[i];
        UnicodeString name = UnicodeString(testCase.name).unescape();
        UnicodeString label = UnicodeString(testCase.bucketLabel).unescape();
        char msg[100];
        snprintf(msg, sizeof(msg), "getBucketIndex(%s)", testCase.name);
        assertEquals(msg, testCase.bucketIndex, index.getBucketIndex(name, status));
        snprintf(msg, sizeof(msg), "immutable getBucketIndex(%s)", testCase.name);
        assertEquals(msg, testCase.bucketIndex, immIndex->getBucketIndex(name, status));
        snprintf(msg, sizeof(msg), "immutable bucket label (%s)", testCase.name);
        assertEquals(msg, label, immIndex->getBucket(testCase.bucketIndex)->getLabel());
    }
}

void AlphabeticIndexTest::TestNoLabels() {
    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<RuleBasedCollator> coll(
        dynamic_cast<RuleBasedCollator *>(Collator::createInstance(Locale::getRoot(), status)));
    TEST_CHECK_STATUS;
    AlphabeticIndex index(coll.orphan(), status);
    TEST_CHECK_STATUS;
    index.addRecord(UnicodeString(static_cast<char16_t>(0x897f)), nullptr, status);
    index.addRecord("i", nullptr, status);
    index.addRecord(UnicodeString(static_cast<char16_t>(0x03B1)), nullptr, status);
    assertEquals("getBucketCount()", 1, index.getBucketCount(status));  // ...
    TEST_ASSERT(index.nextBucket(status));
    assertEquals("underflow label type", static_cast<int32_t>(U_ALPHAINDEX_UNDERFLOW), index.getBucketLabelType());
    assertEquals("all records in the underflow bucket", static_cast<int32_t>(3), index.getBucketRecordCount());
}

void AlphabeticIndexTest::TestChineseZhuyin() {
    UErrorCode status = U_ZERO_ERROR;
    char loc[100];
    uloc_forLanguageTag("zh-u-co-zhuyin", loc, UPRV_LENGTHOF(loc), nullptr, &status);
    AlphabeticIndex index(loc, status);
    LocalPointer<AlphabeticIndex::ImmutableIndex> immIndex(index.buildImmutableIndex(status));
    TEST_CHECK_STATUS; 
    assertEquals("getBucketCount()", 38, immIndex->getBucketCount());
    assertEquals("label 1", UnicodeString(static_cast<char16_t>(0x3105)), immIndex->getBucket(1)->getLabel());
    assertEquals("label 2", UnicodeString(static_cast<char16_t>(0x3106)), immIndex->getBucket(2)->getLabel());
    assertEquals("label 3", UnicodeString(static_cast<char16_t>(0x3107)), immIndex->getBucket(3)->getLabel());
    assertEquals("label 4", UnicodeString(static_cast<char16_t>(0x3108)), immIndex->getBucket(4)->getLabel());
    assertEquals("label 5", UnicodeString(static_cast<char16_t>(0x3109)), immIndex->getBucket(5)->getLabel());
}

void AlphabeticIndexTest::TestJapaneseKanji() {
    UErrorCode status = U_ZERO_ERROR;
    AlphabeticIndex index(Locale::getJapanese(), status);
    LocalPointer<AlphabeticIndex::ImmutableIndex> immIndex(index.buildImmutableIndex(status));
    TEST_CHECK_STATUS;
    // There are no index characters for Kanji in the Japanese standard collator.
    // They should all go into the overflow bucket.
    static const UChar32 kanji[] = { 0x4E9C, 0x95C7, 0x4E00, 0x58F1 };
    int32_t overflowIndex = immIndex->getBucketCount() - 1;
    for(int32_t i = 0; i < UPRV_LENGTHOF(kanji); ++i) {
        char msg[40];
        snprintf(msg, sizeof(msg), "kanji[%d]=U+%04lX in overflow bucket", static_cast<int>(i), static_cast<long>(kanji[i]));
        assertEquals(msg, overflowIndex, immIndex->getBucketIndex(UnicodeString(kanji[i]), status));
        TEST_CHECK_STATUS;
    }
}

void AlphabeticIndexTest::TestChineseUnihan() {
    UErrorCode status = U_ZERO_ERROR;
    AlphabeticIndex index("zh-u-co-unihan", status);
    if(U_FAILURE(status)) {
        dataerrln("unable create an AlphabeticIndex for Chinese/unihan: %s", u_errorName(status));
        return;
    }
    index.setMaxLabelCount(500, status);  // ICU 54 default is 99.
    LocalPointer<AlphabeticIndex::ImmutableIndex> immIndex(index.buildImmutableIndex(status));
    TEST_CHECK_STATUS;
    int32_t bucketCount = immIndex->getBucketCount();
    if(bucketCount < 216) {
        // There should be at least an underflow and overflow label,
        // and one for each of 214 radicals,
        // and maybe additional labels for simplified radicals.
        dataerrln("too few buckets/labels for Chinese/unihan: %d (is zh/unihan data available?)",
                  bucketCount);
        return;
    } else {
        logln("Chinese/unihan has %d buckets/labels", bucketCount);
    }
    // bucketIndex = radical number, adjusted for simplified radicals in lower buckets.
    // CLDR 46+/ICU 76+ use a radical-stroke order matching UAX #38
    // which has buckets only for traditional radicals.
    int32_t bucketIndex = index.getBucketIndex(UnicodeString(static_cast<char16_t>(0x4e5d)), status);
    assertEquals("getBucketIndex(U+4E5D)", 5, bucketIndex);
    bucketIndex = index.getBucketIndex(UnicodeString(static_cast<char16_t>(0x7527)), status);
    assertEquals("getBucketIndex(U+7527)", 100, bucketIndex);
}

void AlphabeticIndexTest::testHasBuckets() {
    checkHasBuckets(Locale("am"), USCRIPT_ETHIOPIC);
    checkHasBuckets(Locale("haw"), USCRIPT_LATIN);
    checkHasBuckets(Locale("hy"), USCRIPT_ARMENIAN);
    checkHasBuckets(Locale("vai"), USCRIPT_VAI);
}

void AlphabeticIndexTest::checkHasBuckets(const Locale &locale, UScriptCode script) {
    IcuTestErrorCode errorCode(*this, "checkHasBuckets");
    AlphabeticIndex aindex(locale, errorCode);
    LocalPointer<AlphabeticIndex::ImmutableIndex> index(aindex.buildImmutableIndex(errorCode), errorCode);
    if (U_FAILURE(errorCode)) {
      dataerrln("%s %d  Error in index creation",  __FILE__, __LINE__);
      return;
    }
    UnicodeString loc = locale.getName();
    assertTrue(loc + u" at least 3 buckets", index->getBucketCount() >= 3);
    const AlphabeticIndex::Bucket *bucket = index->getBucket(1);
    assertEquals(loc + u" real bucket", U_ALPHAINDEX_NORMAL, bucket->getLabelType());
    assertEquals(loc + u" expected script", script,
            uscript_getScript(bucket->getLabel().char32At(0), errorCode));
}

#endif

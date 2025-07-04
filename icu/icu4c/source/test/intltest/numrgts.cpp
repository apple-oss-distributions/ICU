// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/***********************************************************************
 * Copyright (c) 1997-2016, International Business Machines Corporation
 * and others. All Rights Reserved.
 ***********************************************************************/
 
#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "numrgts.h"

#include <cmath>   // std::signbit
#include <float.h> // DBL_MIN, DBL_MAX
#include <stdio.h>

#include "unicode/dcfmtsym.h"
#include "unicode/decimfmt.h"
#include "unicode/locid.h"
#include "unicode/resbund.h"
#include "unicode/calendar.h"
#include "unicode/datefmt.h"
#include "unicode/ucurr.h"
#include "cmemory.h"
#include "cstr.h"
#include "putilimp.h"
#include "uassert.h"

class MyNumberFormatTest : public NumberFormat 
{
public:

    virtual UClassID getDynamicClassID() const override;
  
    virtual UnicodeString& format(    double            number, 
                    UnicodeString&        toAppendTo, 
                    FieldPositionIterator* posIter,
                    UErrorCode& status) const override
    {
        return NumberFormat::format(number, toAppendTo, posIter, status);
    }

    /* Just keep this here to make some of the compilers happy */
    virtual UnicodeString& format(const Formattable& obj,
                                  UnicodeString& toAppendTo,
                                  FieldPosition& pos,
                                  UErrorCode& status) const override
    {
        return NumberFormat::format(obj, toAppendTo, pos, status);
    }

    /* Just use one of the format functions */
    virtual UnicodeString& format(    double            /* number */, 
                    UnicodeString&        toAppendTo, 
                    FieldPosition&        /* pos */) const override
    {
        toAppendTo = "";
        return toAppendTo;
    }
  
    /*
    public Number parse(String text, ParsePosition parsePosition) 
    { return new Integer(0); }
    */
  
    /* Just use one of the parse functions */
    virtual void parse(    const UnicodeString&    /* text */, 
            Formattable&            result, 
            ParsePosition&          /* parsePosition */) const override
    {
        result.setLong(static_cast<int32_t>(0));
    }
  
    virtual void parse(    const UnicodeString&    text, 
            Formattable&            result, 
            UErrorCode&            status) const override
    {
        NumberFormat::parse(text, result, status);
    }
    virtual MyNumberFormatTest* clone() const override
    { return nullptr; }

    virtual UnicodeString& format(int32_t, 
                UnicodeString& foo, 
                FieldPosition&) const override
    { return foo.remove(); }

    virtual UnicodeString& format(int64_t, 
                UnicodeString& foo, 
                FieldPosition&) const override
    { return foo.remove(); }

    virtual void applyPattern(const UnicodeString&, UParseError&, UErrorCode&){
    }
};

int32_t gMyNumberFormatTestClassID;
UClassID MyNumberFormatTest::getDynamicClassID()  const
{
    return (UClassID)&gMyNumberFormatTestClassID;
}


// *****************************************************************************
// class NumberFormatRegressionTest
// *****************************************************************************

#define CASE(id,test) case id: name = #test; if (exec) { logln(#test "---"); logln((UnicodeString)""); test(); } break

void 
NumberFormatRegressionTest::runIndexedTest( int32_t index, UBool exec, const char* &name, char* /*par*/ )
{
    // if (exec) logln((UnicodeString)"TestSuite NumberFormatRegressionTest");
    switch (index) {
        CASE(0,Test4075713);
        CASE(1,Test4074620);
        CASE(2,Test4088161);
        CASE(3,Test4087245);
        CASE(4,Test4087535);
        CASE(5,Test4088503);
        CASE(6,Test4066646);
        CASE(7,Test4059870);
        CASE(8,Test4083018);
        CASE(9,Test4071492);
        CASE(10,Test4086575);
        CASE(11,Test4068693);
        CASE(12,Test4069754);
        CASE(13,Test4087251);
        CASE(14,Test4090489);
        CASE(15,Test4090504);
        CASE(16,Test4095713);
        CASE(17,Test4092561);
        CASE(18,Test4092480);
        CASE(19,Test4087244);
        CASE(20,Test4070798);
        CASE(21,Test4071005);
        CASE(22,Test4071014);
        CASE(23,Test4071859);
        CASE(24,Test4093610);
        CASE(25,Test4098741);
        CASE(26,Test4074454);
        CASE(27,Test4099404);
        CASE(28,Test4101481);
        CASE(29,Test4052223);
        CASE(30,Test4061302);
        CASE(31,Test4062486);
        CASE(32,Test4108738);
        CASE(33,Test4106658);
        CASE(34,Test4106662);
        CASE(35,Test4114639);
        CASE(36,Test4106664);
        CASE(37,Test4106667);
        CASE(38,Test4110936);
        CASE(39,Test4122840);
        CASE(40,Test4125885);
        CASE(41,Test4134034);
        CASE(42,Test4134300);
        CASE(43,Test4140009);
        CASE(44,Test4141750);
        CASE(45,Test4145457);
        CASE(46,Test4147295);
        CASE(47,Test4147706);
        CASE(48,Test4162198);
        CASE(49,Test4162852);
        CASE(50,Test4167494);
        CASE(51,Test4170798);
        CASE(52,Test4176114);
        CASE(53,Test4179818);
        CASE(54,Test4212072);
        CASE(55,Test4216742);
        CASE(56,Test4217661);
        CASE(57,Test4161100);
        CASE(58,Test4243011);
        CASE(59,Test4243108);
        CASE(60,TestJ691);
        CASE(61,Test8199);
        CASE(62,Test9109);
        CASE(63,Test9780);
        CASE(64,Test9677);
        CASE(65,Test10361);

        default: name = ""; break;
    }
}

UBool 
NumberFormatRegressionTest::failure(UErrorCode status, const UnicodeString& msg, const Locale& l, UBool possibleDataError)
{
    if(U_FAILURE(status)) {
        if (possibleDataError) {
            dataerrln(UnicodeString("FAIL: ", "") + msg
                  + UnicodeString(" failed, error ", "") + UnicodeString(u_errorName(status), "") + UnicodeString(l.getName(),""));
        } else {
            errcheckln(status, UnicodeString("FAIL: ", "") + msg
                  + UnicodeString(" failed, error ", "") + UnicodeString(u_errorName(status), "") + UnicodeString(l.getName(),""));
        }
        return true;
    }

    return false;
}

UBool 
NumberFormatRegressionTest::failure(UErrorCode status, const UnicodeString& msg, const char *l, UBool possibleDataError)
{
    if(U_FAILURE(status)) {
        if (possibleDataError) {
            dataerrln(UnicodeString("FAIL: ", "") + msg
                  + UnicodeString(" failed, error ", "") + UnicodeString(u_errorName(status), "") + UnicodeString(l, ""));
        } else {
            errcheckln(status, UnicodeString("FAIL: ", "") + msg
                  + UnicodeString(" failed, error ", "") + UnicodeString(u_errorName(status), "") + UnicodeString(l, ""));
        }
        return true;
    }

    return false;
}

UBool 
NumberFormatRegressionTest::failure(UErrorCode status, const UnicodeString& msg, UBool possibleDataError)
{
    if(U_FAILURE(status)) {
        if (possibleDataError) {
            dataerrln(UnicodeString("FAIL: ", "") + msg
                  + UnicodeString(" failed, error ", "") + UnicodeString(u_errorName(status), ""));
        } else {
            errcheckln(status, UnicodeString("FAIL: ", "") + msg
                  + UnicodeString(" failed, error ", "") + UnicodeString(u_errorName(status), ""));
        }
        return true;
    }

    return false;
}

/**
 * Convert Java-style strings with \u Unicode escapes into UnicodeString objects
 */
inline UnicodeString str(const char *input)
{
  return CharsToUnicodeString(input);
}

/* @bug 4075713
 * NumberFormat.equals comparing with null should always return false.
 */
// {sfb} kind of silly in C++, just checking for new success
void NumberFormatRegressionTest::Test4075713()
{
    //try {
        MyNumberFormatTest *tmp = new MyNumberFormatTest();
        if(tmp != nullptr)
            logln("NumberFormat.equals passed");
    /*} catch (NullPointerException e) {
        errln("(new MyNumberFormatTest()).equals(null) throws unexpected exception");
    }*/
    
    delete tmp;
}

/* @bug 4074620
 * NumberFormat.equals comparing two obj equal even the setGroupingUsed
 * flag is different.
 */
void NumberFormatRegressionTest::Test4074620() 
{

    MyNumberFormatTest *nf1 = new MyNumberFormatTest();
    MyNumberFormatTest *nf2 = new MyNumberFormatTest();

    nf1->setGroupingUsed(false);
    nf2->setGroupingUsed(true);

    if(nf1 == nf2) 
        errln("Test for bug 4074620 failed");
    else 
        logln("Test for bug 4074620 passed.");
    
    delete nf1;
    delete nf2;
}


/* @bug 4088161
 * DecimalFormat.format() incorrectly uses maxFractionDigits setting.
 */

void NumberFormatRegressionTest::Test4088161()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if (!failure(status, "new DecimalFormat", "")) {
        double d = 100;
        df->setMinimumFractionDigits(0);
        df->setMaximumFractionDigits(16);
        UnicodeString sBuf1;
        FieldPosition fp1(FieldPosition::DONT_CARE);
        logln(UnicodeString("d = ") + d);
        logln(UnicodeString("maxFractionDigits = ") + df->getMaximumFractionDigits());

        logln(" format(d) = '" + df->format(d, sBuf1, fp1) + "'");
        df->setMaximumFractionDigits(17);
        UnicodeString sBuf2;
        FieldPosition fp2(FieldPosition::DONT_CARE);
        logln(UnicodeString("maxFractionDigits = ") + df->getMaximumFractionDigits());
        sBuf2 = df->format(d, sBuf2, fp2);
        if(sBuf2 != "100")
            errln(" format(d) = '" + sBuf2 + "'");
    }

    delete df;
}

/* @bug 4087245
 * DecimalFormatSymbols should be cloned in the ctor DecimalFormat.
 * DecimalFormat(String, DecimalFormatSymbols).
 */
void NumberFormatRegressionTest::Test4087245()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols *symbols = new DecimalFormatSymbols(status);
    failure(status, "new DecimalFormatSymbols", "");
    // {sfb} One note about this test: if you pass in a pointer
    // to the symbols, they are adopted and this test will fail,
    // even though that is the correct behavior.  To test the cloning
    // of the symbols, it is necessary to pass in a reference to the symbols
    DecimalFormat *df = new DecimalFormat("#,##0.0", *symbols, status);
    failure(status, "new DecimalFormat with symbols", "");
    int32_t n = 123;
    UnicodeString buf1;
    UnicodeString buf2;
    FieldPosition pos(FieldPosition::DONT_CARE);
    logln(UnicodeString("format(") + n + ") = " + 
        df->format(n, buf1, pos));
    symbols->setSymbol(DecimalFormatSymbols::kDecimalSeparatorSymbol, UnicodeString(static_cast<char16_t>(0x70))); // change value of field
    logln(UnicodeString("format(") + n + ") = " +
        df->format(n, buf2, pos));
    if(buf1 != buf2)
        errln("Test for bug 4087245 failed");

    delete df;
    delete symbols;
}

/* @bug 4087535
 * DecimalFormat.format() incorrectly formats 0.0
 */
void NumberFormatRegressionTest::Test4087535()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if (U_FAILURE(status)) {
        dataerrln("Error creating DecimalFormat - %s", u_errorName(status));
        return;
    }
    df->setMinimumIntegerDigits(0);

    double n = 0;
    UnicodeString buffer;
    FieldPosition pos(FieldPosition::DONT_CARE);
    buffer = df->format(n, buffer, pos);
    if (buffer.length() == 0)
        errln(/*n + */": '" + buffer + "'");
    n = 0.1;
    buffer = df->format(n, buffer, pos);
    if (buffer.length() == 0)
        errln(/*n + */": '" + buffer + "'");

    delete df;
}

/* @bug 4088503
 * DecimalFormat.format fails when groupingSize is set to 0.
 */
// {sfb} how do I tell if this worked? --> FieldPosition doesn't change ??
void NumberFormatRegressionTest::Test4088503()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if (U_FAILURE(status)) {
        dataerrln("Error creating DecimalFormat - %s", u_errorName(status));
        return;
    }
    df->setGroupingSize(0);
    UnicodeString sBuf;
    FieldPosition fp(FieldPosition::DONT_CARE);
    //try {
        logln(df->format(static_cast<int32_t>(123), sBuf, fp));
        //if(fp == FieldPosition(FieldPosition::DONT_CARE))
        //    errln("Test for bug 4088503 failed.");
    /*} catch (Exception foo) {
        errln("Test for bug 4088503 failed.");
    }*/
    delete df;

}
/* @bug 4066646
 * NumberFormat.getCurrencyInstance is wrong.
 */
void NumberFormatRegressionTest::Test4066646() 
{
    assignFloatValue(2.04f);
    assignFloatValue(2.03f);
    assignFloatValue(2.02f);
    assignFloatValue(0.0f);
}

float 
NumberFormatRegressionTest::assignFloatValue(float returnfloat)
{
    logln(UnicodeString(" VALUE ") + returnfloat);
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *nfcommon =  NumberFormat::createCurrencyInstance(Locale::getUS(), status);
    if (failure(status, "NumberFormat::createCurrencyInstance", Locale::getUS(), true)){
        delete nfcommon;
        return returnfloat;
    }
    nfcommon->setGroupingUsed(false);

    UnicodeString stringValue;
    stringValue = nfcommon->format(returnfloat, stringValue);
    logln(" DISPLAYVALUE " + stringValue);
    Formattable result;
    nfcommon->parse(stringValue, result, status);
    failure(status, "nfcommon->parse", Locale::getUS());
    float floatResult = static_cast<float>(result.getType() == Formattable::kDouble
                                        ? result.getDouble() : result.getLong());
    if( uprv_fabs(floatResult - returnfloat) > 0.0001)
    //String stringValue = nfcommon.format(returnfloat).substring(1);
    //if (Float.valueOf(stringValue).floatValue() != returnfloat)
        errln(UnicodeString("FAIL: expected ") + returnfloat + ", got " + floatResult + " (" + stringValue+")");
    
    delete nfcommon;
    return returnfloat;
} // End Of assignFloatValue()

/* @bug 4059870
 * DecimalFormat throws exception when parsing "0"
 */
void NumberFormatRegressionTest::Test4059870() 
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *format = new DecimalFormat("00", status);
    failure(status, "new Decimalformat", Locale::getUS());
    //try {
        Formattable result;
        UnicodeString str;
        format->parse(UnicodeString("0"), result, status);
        failure(status, "format->parse", Locale::getUS());
        
    /*} 
    catch (Exception e) { 
        errln("Test for bug 4059870 failed : " + e); 
    }*/

    delete format;
}
/* @bug 4083018
 * DecimalFormatSymbol.equals should always return false when
 * comparing with null.
 */
// {sfb} this is silly in C++
void NumberFormatRegressionTest::Test4083018()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols *dfs = new DecimalFormatSymbols(status);
    failure(status, "new DecimalFormatSymbols", Locale::getUS());
    //try {
        if (dfs != nullptr)
            logln("Test Passed!");
        else
            errln("Test for bug 4083018 failed");
    /*} catch (Exception foo) {
        errln("Test for bug 4083018 failed => Message : " + foo.getMessage());
    }*/

    delete dfs;
}

/* @bug 4071492
 * DecimalFormat does not round up correctly.
 */
void NumberFormatRegressionTest::Test4071492()
{
    double x = 0.00159999;
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *nf = NumberFormat::createInstance(status);
    if (failure(status, "NumberFormat::createInstance", Locale::getUS(), true)) {
        delete nf;
        return;
    }
    nf->setMaximumFractionDigits(4);
    UnicodeString out;
    FieldPosition pos(FieldPosition::DONT_CARE);
    out = nf->format(x, out, pos);
    logln("0.00159999 formats with 4 fractional digits to " + out);
    UnicodeString expected("0.0016");
    if (out != expected)
        errln("FAIL: Expected " + expected);

    delete nf;
}

/* @bug 4086575
 * A space as a group separator for localized pattern causes
 * wrong format.  WorkAround : use non-breaking space.
 */
void NumberFormatRegressionTest::Test4086575() 
{
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *nf1 = NumberFormat::createInstance(Locale::getFrance(), status);

    // TODO: There is not a good way to find out that the creation of this number format has
    // failed. Major rewiring of format construction proposed.
    if(U_FAILURE(status)) {
      dataerrln("Something is wrong with French number format - it should not fallback. Exiting - %s", u_errorName(status));
      delete nf1;
      return;
    }
    failure(status, "NumberFormat::createInstance", Locale::getFrance());

    // C++ workaround to make sure cast works
    DecimalFormat *nf = dynamic_cast<DecimalFormat *>(nf1);
    if(nf == nullptr) {
        errln("NumberFormat::createInstance returned incorrect type.");
        return;
    }

    UnicodeString temp;
    logln("nf toPattern1: " + nf->toPattern(temp));
    logln("nf toLocPattern1: " + nf->toLocalizedPattern(temp));
    
    // No group separator
    logln("...applyLocalizedPattern ###,00;(###,00) ");
    nf->applyLocalizedPattern(UnicodeString("###,00;(###,00)"), status);
    failure(status, "nf->applyLocalizedPattern", Locale::getFrance());
    logln("nf toPattern2: " + nf->toPattern(temp));
    logln("nf toLocPattern2: " + nf->toLocalizedPattern(temp));

    FieldPosition pos(FieldPosition::DONT_CARE);
    logln("nf: " + nf->format(static_cast<int32_t>(1234), temp, pos));  // 1234,00
    logln("nf: " + nf->format(static_cast<int32_t>(-1234), temp, pos)); // (1234,00)

    // Space as group separator

    logln("...applyLocalizedPattern # ###,00;(# ###,00) ");
    // nbsp = \u00a0
    //nf->applyLocalizedPattern("#\u00a0###,00;(#\u00a0###,00)");
    char16_t patChars[] = {
             0x23, 0x202f, 0x23, 0x23, 0x23, 0x2c, 0x30, 0x30, 0x3b, 
        0x28, 0x23, 0x202f, 0x23, 0x23, 0x23, 0x2c, 0x30, 0x30, 0x29
    };
    UnicodeString pat(patChars, 19, 19);
    nf->applyLocalizedPattern(pat, status);
    failure(status, "nf->applyLocalizedPattern", Locale::getFrance());
    logln("nf toPattern2: " + nf->toPattern(temp));
    logln("nf toLocPattern2: " + nf->toLocalizedPattern(temp));
    UnicodeString buffer;
    buffer = nf->format(static_cast<int32_t>(1234), buffer, pos);
    //if (buffer != UnicodeString("1\u00a0234,00"))
    char16_t c[] = {
        0x31, 0x202f, 0x32, 0x33, 0x34, 0x2c, 0x30, 0x30
    };
    UnicodeString cc(c, 8, 8);
    if (buffer != cc)
        errln("nf : " + buffer); // Expect 1 234,00
    
    buffer.remove();
    buffer = nf->format(static_cast<int32_t>(-1234), buffer, pos);
    char16_t c1[] = {
        0x28, 0x31, 0x202f, 0x32, 0x33, 0x34, 0x2c, 0x30, 0x30, 0x29
    };
    UnicodeString cc1(c1, 10, 10);
    if (buffer != cc1)
        errln("nf : " + buffer); // Expect (1 234,00)

    // Erroneously prints:
    // 1234,00 ,
    // (1234,00 ,)

    delete nf1;
}
/* @bug 4068693
 * DecimalFormat.parse returns wrong value
 */
// {sfb} slightly converted into a round-trip test, since in C++
// there is no Double.toString()
void NumberFormatRegressionTest::Test4068693()
{
    logln("----- Test Application -----");
    ParsePosition pos(0);
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    Formattable d;
    //Double d = (Double)df.parse("123.55456", pos=new ParsePosition(0));
    df->parse(UnicodeString("123.55456"), d, pos);
    //if (!d.toString().equals("123.55456")) {
    UnicodeString dstr;
    df->setMaximumFractionDigits(999);
    df->setMaximumIntegerDigits(999);
    FieldPosition fp(FieldPosition::DONT_CARE);
    dstr = df->format(d.getDouble(), dstr, fp);
    if (dstr != UnicodeString("123.55456")) {
        errln(UnicodeString("Result -> ") + d.getDouble());
    }

    delete df;
}

/* @bug 4069754, 4067878
 * null pointer thrown when accessing a deserialized DecimalFormat
 * object.
 */
// {sfb} doesn't apply in C++
void NumberFormatRegressionTest::Test4069754()
{
/*    try {
        myformat it = new myformat();
        logln(it.Now());
        FileOutputStream ostream = new FileOutputStream("t.tmp");
        ObjectOutputStream p = new ObjectOutputStream(ostream);
        p.writeObject(it);
        ostream.close();
        logln("Saved ok.");

        FileInputStream istream = new FileInputStream("t.tmp");
        ObjectInputStream p2 = new ObjectInputStream(istream);
        myformat it2 = (myformat)p2.readObject();
        logln(it2.Now());
        istream.close();
        logln("Loaded ok.");
    } catch (Exception foo) {
        errln("Test for bug 4069754 or 4057878 failed => Exception: " + foo.getMessage());
    }
*/}

/* @bug 4087251
 * DecimalFormat.applyPattern(String) allows illegal patterns
 */
void NumberFormatRegressionTest::Test4087251()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    //try {
        df->applyPattern(UnicodeString("#.#.#"), status);
        if( ! U_FAILURE(status))
            errln("df->applyPattern with illegal pattern didn't fail");
        UnicodeString temp;
        logln("toPattern() returns \"" + df->toPattern(temp) + "\"");
        //errln("applyPattern(\"#.#.#\") doesn't throw IllegalArgumentException");
    /*} catch (IllegalArgumentException e) {
        logln("Caught Illegal Argument Error !");
    }*/
    // Second test; added 5/11/98 when reported to fail on 1.2b3
    //try {
        df->applyPattern("#0.0#0#0", status);
        if( ! U_FAILURE(status))
            errln("df->applyPattern with illegal pattern didn't fail");
        logln("toPattern() returns \"" + df->toPattern(temp) + "\"");
        //errln("applyPattern(\"#0.0#0#0\") doesn't throw IllegalArgumentException");
    /*} catch (IllegalArgumentException e) {
        logln("Ok - IllegalArgumentException for #0.0#0#0");
    }*/

    delete df;
}

/* @bug 4090489
 * DecimalFormat.format() loses precision
 */
void NumberFormatRegressionTest::Test4090489()
{
// {sfb} snprintf doesn't correctly handle the double, so there is nothing
// that NumberFormat can do.  For some reason, it does not format the last 1.

/*    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    failure(status, "new DecimalFormat");
    df->setMinimumFractionDigits(10);
    df->setMaximumFractionDigits(999);
    df->setGroupingUsed(false);
    double d = 1.000000000000001E7;
    //BigDecimal bd = new BigDecimal(d);
    UnicodeString sb;
    FieldPosition fp(FieldPosition::DONT_CARE);
    logln(UnicodeString("d = ") + d);
    //logln("BigDecimal.toString():  " + bd.toString());
    df->format(d, sb, fp);
    if (sb != "10000000.0000000100") {
        errln("DecimalFormat.format(): " + sb);
    }
*/
}

/* @bug 4090504
 * DecimalFormat.format() loses precision
 */
void NumberFormatRegressionTest::Test4090504()
{
    double d = 1;
    logln(UnicodeString("d = ") + d);
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    UnicodeString sb;
    FieldPosition fp(FieldPosition::DONT_CARE);
    //try {
        for (int i = 17; i <= 20; i++) {
            df->setMaximumFractionDigits(i);
            //sb = new StringBuffer("");
            fp.setField(0);
            logln(UnicodeString("  getMaximumFractionDigits() = ") + i);
            logln(UnicodeString("  formatted: ") + df->format(d, sb, fp));
        }
    /*} catch (Exception foo) {
        errln("Bug 4090504 regression test failed. Message : " + foo.getMessage());
    }*/

    delete df;
}
/* @bug 4095713
 * DecimalFormat.parse(String str, ParsePosition pp) loses precision
 */
void NumberFormatRegressionTest::Test4095713()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    UnicodeString str("0.1234");
    double d1 = 0.1234;
    //Double d1 = new Double(str);
    //Double d2 = (Double) df.parse(str, new ParsePosition(0));
    Formattable d2;
    ParsePosition pp(0);
    df->parse(str, d2, pp);
    logln(UnicodeString("") + d1);
    if (d2.getDouble() != d1)
        errln(UnicodeString("Bug 4095713 test failed, new double value : ") + d2.getDouble());
    delete df;
}

/* @bug 4092561
 * DecimalFormat.parse() fails when multiplier is not set to 1
 */
// {sfb} not sure what to do with this one
void NumberFormatRegressionTest::Test4092561()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");

    // {sfb} going to cheat here and use snprintf ??

    /*UnicodeString str = Long.toString(Long.MIN_VALUE);
    logln("Long.MIN_VALUE : " + df.parse(str, new ParsePosition(0)).toString());
    df.setMultiplier(100);
    Number num = df.parse(str, new ParsePosition(0));
    if (num.doubleValue() != -9.223372036854776E16)
        errln("Bug 4092561 test failed when multiplier is set to not 1.");
*/
    delete df;
}

/* @bug 4092480
 * DecimalFormat: Negative format ignored.
 */
void NumberFormatRegressionTest::Test4092480()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *dfFoo = new DecimalFormat(UnicodeString("000"), status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete dfFoo;
      return;
    }
    failure(status, "new DecimalFormat");

    //try {
        dfFoo->applyPattern("0000;-000", status);
        failure(status, "dfFoo->applyPattern");
        UnicodeString temp;
        if (dfFoo->toPattern(temp) != UnicodeString("0000"))
            errln("ERROR: dfFoo.toPattern : " + dfFoo->toPattern(temp));
        FieldPosition pos(FieldPosition::DONT_CARE);
        logln(dfFoo->format(static_cast<int32_t>(42), temp, pos));
        logln(dfFoo->format(static_cast<int32_t>(-42), temp, pos));
        dfFoo->applyPattern("000;-000", status);
        failure(status, "dfFoo->applyPattern");
        if (dfFoo->toPattern(temp) != UnicodeString("000"))
            errln("ERROR: dfFoo.toPattern : " + dfFoo->toPattern(temp));
        logln(dfFoo->format(static_cast<int32_t>(42), temp, pos));
        logln(dfFoo->format(static_cast<int32_t>(-42), temp, pos));

        dfFoo->applyPattern("000;-0000", status);
        failure(status, "dfFoo->applyPattern");
        if (dfFoo->toPattern(temp) != UnicodeString("000"))
            errln("ERROR: dfFoo.toPattern : " + dfFoo->toPattern(temp));
        logln(dfFoo->format(static_cast<int32_t>(42), temp, pos));
        logln(dfFoo->format(static_cast<int32_t>(-42), temp, pos));

        dfFoo->applyPattern("0000;-000", status);
        failure(status, "dfFoo->applyPattern");
        if (dfFoo->toPattern(temp) != UnicodeString("0000"))
            errln("ERROR: dfFoo.toPattern : " + dfFoo->toPattern(temp));
        logln(dfFoo->format(static_cast<int32_t>(42), temp, pos));
        logln(dfFoo->format(static_cast<int32_t>(-42), temp, pos));
    /*} catch (Exception foo) {
        errln("Message " + foo.getMessage());
    }*/

    delete dfFoo;
}
/* @bug 4087244
 * NumberFormat.getCurrencyInstance() produces format that uses
 * decimal separator instead of monetary decimal separator.
 *
 * Rewrote this test not to depend on the actual pattern.  Pattern should
 * never contain the monetary separator!  Decimal separator in pattern is
 * interpreted as monetary separator if currency symbol is seen!
 */
void NumberFormatRegressionTest::Test4087244() {
    UErrorCode status = U_ZERO_ERROR;
    char loc[256] = {0};

    uloc_canonicalize("pt_PT@currency=PTE", loc, 256, &status);
    Locale de(loc);
    LocalPointer<NumberFormat> nf(NumberFormat::createCurrencyInstance(de, status));
    if(U_FAILURE(status)) {
        dataerrln("Error creating DecimalFormat: %s", u_errorName(status));
        return;
    }
    DecimalFormat *df = dynamic_cast<DecimalFormat *>(nf.getAlias());
    if(df == nullptr) {
        errln("expected DecimalFormat!");
        return;
    }
    const DecimalFormatSymbols *sym = df->getDecimalFormatSymbols();
    UnicodeString decSep = sym->getSymbol(DecimalFormatSymbols::kDecimalSeparatorSymbol);
    UnicodeString monSep = sym->getSymbol(DecimalFormatSymbols::kMonetarySeparatorSymbol);
    if (decSep == monSep) {
        errln("ERROR in test: want decimal sep != monetary sep");
        return;
    }
    df->setMinimumIntegerDigits(1);
    df->setMinimumFractionDigits(2);
    UnicodeString str;
    FieldPosition pos;
    df->format(1.23, str, pos);
    UnicodeString monStr("1x23");
    monStr.replace(static_cast<int32_t>(1), 1, monSep);
    UnicodeString decStr("1x23");
    decStr.replace(static_cast<int32_t>(1), 1, decSep);
    if (str.indexOf(monStr) >= 0 && str.indexOf(decStr) < 0) {
        logln(UnicodeString("OK: 1.23 -> \"") + str + "\" contains \"" +
              monStr + "\" and not \"" + decStr + '"');
    } else {
        errln(UnicodeString("FAIL: 1.23 -> \"") + str + "\", should contain \"" +
              monStr +
              "\" and not \"" + decStr + '"');
    }
}
/* @bug 4070798
 * Number format data rounding errors for locale FR
 */
void NumberFormatRegressionTest::Test4070798() 
{
    NumberFormat *formatter;
    UnicodeString tempString;
    
    /* User error :
    String expectedDefault = "-5\u202f789,987";
    String expectedCurrency = "5\u202f789,98\u00a0F";
    String expectedPercent = "-578\u202f998%";
    */
    char16_t chars1 [] = {
        0x2d, 0x35, 0x202f, 0x37, 0x38, 0x39, 0x2c, 0x39, 0x38, 0x38
    };
    char16_t chars2 [] = {
        0x35, 0x202f, 0x37, 0x38, 0x39, 0x2c, 0x39, 0x39, 0x00a0, 0x46
    };
    char16_t chars3 [] = {
        0x2d, 0x35, 0x37, 0x38, 0x202f, 0x39, 0x39, 0x39, 0x00a0, 0x25
    };
    UnicodeString expectedDefault(chars1, 10, 10);
    UnicodeString expectedCurrency(chars2, 10, 10);
    UnicodeString expectedPercent(chars3, 10, 10);

    UErrorCode status = U_ZERO_ERROR;
    char loc[256]={0};
    int len = uloc_canonicalize("fr_FR@currency=FRF", loc, 256, &status);
    (void)len;  // Suppress set but not used warning.
    formatter = NumberFormat::createInstance(Locale(loc), status);
    if(U_FAILURE(status)) {
      dataerrln("Error creating DecimalFormat: %s", u_errorName(status));
      delete formatter;
      return;
    }
    failure(status, "NumberFormat::createInstance", loc);
    tempString = formatter->format (-5789.9876, tempString);

    if (tempString == expectedDefault) {
        logln ("Bug 4070798 default test passed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedDefault +
        " Received " + tempString );
    }
    delete formatter;
    len = uloc_canonicalize("fr_FR@currency=FRF", loc, 256, &status);
    formatter = NumberFormat::createCurrencyInstance(loc, status);
    failure(status, "NumberFormat::createCurrencyInstance", loc);
    tempString.remove();
    tempString = formatter->format( 5789.9876, tempString );

    if (tempString == expectedCurrency) {
        logln ("Bug 4070798 currency test passed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedCurrency +
        " Received " + tempString );
    }
    delete formatter;
   
    uloc_canonicalize("fr_FR@currency=FRF", loc, 256, &status);
    formatter = NumberFormat::createPercentInstance(Locale(loc), status);
    failure(status, "NumberFormat::createPercentInstance", loc);
    tempString.remove();
    tempString = formatter->format (-5789.9876, tempString);

    if (tempString == expectedPercent) {
        logln ("Bug 4070798 percentage test passed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedPercent +
        " Received " + tempString );
    }

    delete formatter;
}
/* @bug 4071005
 * Data rounding errors for French (Canada) locale
 */
void NumberFormatRegressionTest::Test4071005() 
{
    NumberFormat *formatter;
    UnicodeString tempString;
    /* User error :
    String expectedDefault = "-5\u00a0789,987";
    String expectedCurrency = "5\u00a0789,98\u00a0$";
    String expectedPercent = "-578\u00a0998%";
    */
    char16_t chars1 [] = {
        0x2d, 0x35, 0x00a0, 0x37, 0x38, 0x39, 0x2c, 0x39, 0x38, 0x38
    };
    char16_t chars2 [] = {
        0x35, 0x00a0, 0x37, 0x38, 0x39, 0x2c, 0x39, 0x39, 0x00a0, 0x24
    };
    char16_t chars3 [] = {
        0x2d, 0x35, 0x37, 0x38, 0x00a0, 0x39, 0x39, 0x39, 0x00a0, 0x25
    };
    UnicodeString expectedDefault(chars1, 10, 10);
    UnicodeString expectedCurrency(chars2, 10, 10);
    UnicodeString expectedPercent(chars3, 10, 10);

    UErrorCode status = U_ZERO_ERROR;
    formatter = NumberFormat::createInstance(Locale::getCanadaFrench(), status);
    if (failure(status, "NumberFormat::createInstance", Locale::getCanadaFrench(), true)){
        delete formatter;
        return;
    }
    tempString = formatter->format (-5789.9876, tempString);

    if (tempString == expectedDefault) {
        logln ("Bug 4071005 default test passed.");
    } else {
        errln(UnicodeString("Failed: fr_CA") +
        " Expected " + expectedDefault +
        " Received " + tempString );
    }
    delete formatter;

    formatter = NumberFormat::createCurrencyInstance(Locale::getCanadaFrench(), status);
    failure(status, "NumberFormat::createCurrencyInstance", Locale::getCanadaFrench());
    tempString.remove();
    tempString = formatter->format( 5789.9876, tempString );

    if (tempString == expectedCurrency) {
        logln ("Bug 4071005 currency test passed.");
    } else {
        errln(UnicodeString("Failed: fr_CA") +
        " Expected " + expectedCurrency +
        " Received " + tempString );
    }
    delete formatter;

    formatter = NumberFormat::createPercentInstance(Locale::getCanadaFrench(), status);
    failure(status, "NumberFormat::createPercentInstance", Locale::getCanadaFrench());
    tempString.remove();
    tempString = formatter->format (-5789.9876, tempString);

    if (tempString == expectedPercent) {
        logln ("Bug 4071005 percentage test passed.");
    } else {
        errln(UnicodeString("Failed: fr_CA") +
        " Expected " + expectedPercent +
        " Received " + tempString );
    }

    delete formatter;
}

/* @bug 4071014
 * Data rounding errors for German (Germany) locale
 */
void NumberFormatRegressionTest::Test4071014() 
{
    NumberFormat *formatter;
    UnicodeString tempString;
    /* user error :
    String expectedDefault = "-5.789,987";
    String expectedCurrency = "5.789,98 DM";
    String expectedPercent = "-578.998%";
    */
    UnicodeString expectedDefault("-5.789,988");
    UnicodeString expectedCurrency("5.789,99\\u00A0DM");
    UnicodeString expectedPercent("-578.999\\u00A0%");

    expectedCurrency = expectedCurrency.unescape();
    expectedPercent = expectedPercent.unescape();

    UErrorCode status = U_ZERO_ERROR;
    char loc[256]={0};
    uloc_canonicalize("de_DE@currency=DEM", loc, 256, &status);
    formatter = NumberFormat::createInstance(Locale(loc), status);
    if (failure(status, "NumberFormat::createInstance", loc, true)){
        delete formatter;
        return;
    }
    tempString.remove();
    tempString = formatter->format (-5789.9876, tempString);

    if (tempString == expectedDefault) {
        logln ("Bug 4071014 default test passed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedDefault +
        " Received " + tempString );
    }
    delete formatter;
    uloc_canonicalize("de_DE@currency=DEM", loc, 256, &status);
    formatter = NumberFormat::createCurrencyInstance(Locale(loc), status);
    failure(status, "NumberFormat::createCurrencyInstance", loc);
    tempString.remove();
    tempString = formatter->format( 5789.9876, tempString );

    if (tempString == expectedCurrency) {
        logln ("Bug 4071014 currency test assed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedCurrency +
        " Received " + tempString );
    }
    delete formatter;

    formatter = NumberFormat::createPercentInstance(Locale::getGermany(), status);
    failure(status, "NumberFormat::createPercentInstance", Locale::getGermany());
    tempString.remove();
    tempString = formatter->format (-5789.9876, tempString);

    if (tempString == expectedPercent) {
        logln ("Bug 4071014 percentage test passed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedPercent +
        " Received " + tempString );
    }

    delete formatter;
}
/* @bug 4071859
 * Data rounding errors for Italian locale number formats
 */
void NumberFormatRegressionTest::Test4071859() 
{
    NumberFormat *formatter;
    UnicodeString tempString;
    /* user error :
    String expectedDefault = "-5.789,987";
    String expectedCurrency = "-L.\\u00A05.789,98";
    String expectedPercent = "-578.998%";
    */
#if APPLE_ICU_CHANGES
// rdar://136543653 (Integrate ICU 76rc and CLDR 46rc into Apple ICU)
// OSCLDR in 45 or 46 changed minimumGroupingDigits for Italian to 2, but this test wasn't updated.
// I'm not sure why this isn't also failing in OSICU, since there are no Apple changes involved here,
// (maybe it was a data-generation error?  or minimumGroupingDigits wasn't working right?),
// but I'm changing the test because the minimumGroupingDigits change looks legitimate
    UnicodeString expectedDefault("-5789,988");
    UnicodeString expectedCurrency("-ITL\\u00A05790", -1, US_INV);
#else
    UnicodeString expectedDefault("-5.789,988");
    UnicodeString expectedCurrency("-ITL\\u00A05.790", -1, US_INV);
#endif // APPLE_ICU_CHANGES
    UnicodeString expectedPercent("-578.999%");
    expectedCurrency = expectedCurrency.unescape();

    UErrorCode status = U_ZERO_ERROR;
    char loc[256]={0};
    uloc_canonicalize("it_IT@currency=ITL", loc, 256, &status);
    formatter = NumberFormat::createInstance(Locale(loc), status);
    if (failure(status, "NumberFormat::createNumberInstance", true)){
        delete formatter;
        return;
    }
    tempString = formatter->format (-5789.9876, tempString);

    if (tempString == expectedDefault) {
        logln ("Bug 4071859 default test passed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedDefault +
        " Received " + tempString );
    }
    delete formatter;
    uloc_canonicalize("it_IT@currency=ITL", loc, 256, &status);
    formatter = NumberFormat::createCurrencyInstance(Locale(loc), status);
    failure(status, "NumberFormat::createCurrencyInstance");
    tempString.remove();
    tempString = formatter->format( -5789.9876, tempString );

    if (tempString == expectedCurrency) {
        logln ("Bug 4071859 currency test assed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedCurrency +
        " Received " + tempString );
    }
    delete formatter;
    uloc_canonicalize("it_IT@currency=ITL", loc, 256, &status);
    formatter = NumberFormat::createPercentInstance(Locale(loc), status);
    failure(status, "NumberFormat::createPercentInstance");
    tempString.remove();
    tempString = formatter->format (-5789.9876, tempString);

    if (tempString == expectedPercent) {
        logln ("Bug 4071859 percentage test passed.");
    } else {
        errln(UnicodeString("Failed:") +
        " Expected " + expectedPercent +
        " Received " + tempString );
    }

    delete formatter;
}
/* @bug 4071859
 * Test rounding for nearest even.
 */
void NumberFormatRegressionTest::Test4093610()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat df("#0.#", status);
    if (!failure(status, "new DecimalFormat")) {
        UnicodeString s("12.4");
        roundingTest(&df, 12.35, s);
        roundingTest(&df, 12.45, s);
        s = "12.5";
        roundingTest(&df, 12.452,s);
        s = "12.6";
        roundingTest(&df, 12.55, s);
        roundingTest(&df, 12.65, s);
        s = "12.7";
        roundingTest(&df, 12.652,s);
        s = "12.8";
        roundingTest(&df, 12.75, s);
        roundingTest(&df, 12.752,s);
        roundingTest(&df, 12.85, s);
        s = "12.9";
        roundingTest(&df, 12.852,s);
        s = "13";
        roundingTest(&df, 12.95, s);
        roundingTest(&df, 12.952,s);
    }
}

void NumberFormatRegressionTest::roundingTest(DecimalFormat *df, double x, UnicodeString& expected)
{
    UnicodeString out;
    FieldPosition pos(FieldPosition::DONT_CARE);
    out = df->format(x, out, pos);
    logln(UnicodeString("") + x + " formats with 1 fractional digits to " + out);
    if (out != expected) {
        dataerrln("FAIL: Expected '%s'; got '%s'", CStr(expected)(), CStr(out)());
    }
}
/* @bug 4098741
 * Tests the setMaximumFractionDigits limit.
 */
void NumberFormatRegressionTest::Test4098741()
{
    //try {
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *fmt = NumberFormat::createPercentInstance(status);
    if (U_FAILURE(status)) {
        dataerrln("Error calling NumberFormat::createPercentInstance");
        delete fmt;
        return;
    }

        fmt->setMaximumFractionDigits(20);
        UnicodeString temp;
        logln(fmt->format(.001, temp));
    /*} catch (Exception foo) {
        errln("Bug 4098471 failed with exception thrown : " + foo.getMessage());
    }*/
    delete fmt;
}
/* @bug 4074454
 * Tests illegal pattern exception.
 * Fix comment : HShih A31 Part1 will not be fixed and javadoc needs to be updated.
 * Part2 has been fixed.
 */
void NumberFormatRegressionTest::Test4074454()
{
    //try {
    UErrorCode status = U_ZERO_ERROR;  
    DecimalFormat *fmt = new DecimalFormat("#,#00.00;-#.#", status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete fmt;
      return;
    }
    failure(status, "new DecimalFormat");
      logln("Inconsistent negative pattern is fine.");
        DecimalFormat *newFmt = new DecimalFormat("#,#00.00 p''ieces;-#,#00.00 p''ieces", status);
        failure(status, "new DecimalFormat");
        UnicodeString tempString;
        FieldPosition pos(FieldPosition::DONT_CARE);
        tempString = newFmt->format(3456.78, tempString, pos);
        if (tempString != UnicodeString("3,456.78 p'ieces"))
            dataerrln("Failed!  3456.78 p'ieces expected, but got : " + tempString);
    /*} catch (Exception foo) {
        errln("An exception was thrown for any inconsistent negative pattern.");
    }*/

    delete fmt;
    delete newFmt;
}
/* @bug 4099404
 * Tests all different comments.
 * Response to some comments :
 * [1] DecimalFormat.parse API documentation is more than just one line.
 * This is not a reproducible doc error in 116 source code.
 * [2] See updated javadoc.
 * [3] Fixed.
 * [4] NumberFormat.parse(String, ParsePosition) : If parsing fails,
 * a null object will be returned.  The unchanged parse position also
 * reflects an error.
 * NumberFormat.parse(String) : If parsing fails, an ParseException
 * will be thrown.
 * See updated javadoc for more details.
 * [5] See updated javadoc.
 * [6] See updated javadoc.
 * [7] This is a correct behavior if the DateFormat object is linient.
 * Otherwise, an IllegalArgumentException will be thrown when formatting
 * "January 35".  See GregorianCalendar class javadoc for more details.
 */
void NumberFormatRegressionTest::Test4099404()
{
    //try {
        UErrorCode status = U_ZERO_ERROR;
        DecimalFormat *fmt = new DecimalFormat(UnicodeString("000.0#0"), status);
        if(! U_FAILURE(status))
            errln(UnicodeString("Bug 4099404 failed applying illegal pattern \"000.0#0\""));
    /*} catch (Exception foo) {
        logln("Bug 4099404 pattern \"000.0#0\" passed");
    }*/
    delete fmt;
    fmt = nullptr;
        //try {
        fmt = new DecimalFormat(UnicodeString("0#0.000"), status);
        if( !U_FAILURE(status))
           errln("Bug 4099404 failed applying illegal pattern \"0#0.000\"");
    /*} catch (Exception foo) {
        logln("Bug 4099404 pattern \"0#0.000\" passed");
    }*/

    delete fmt;
}
/* @bug 4101481
 * DecimalFormat.applyPattern doesn't set minimum integer digits
 */
void NumberFormatRegressionTest::Test4101481()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *sdf = new DecimalFormat(UnicodeString("#,##0"), status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete sdf;
      return;
    }
    failure(status, "new DecimalFormat");
    if (sdf->getMinimumIntegerDigits() != 1)
        errln(UnicodeString("Minimum integer digits : ") + sdf->getMinimumIntegerDigits());
    delete sdf;
}
/* @bug 4052223 (API addition request A27)
 * Tests ParsePosition.setErrorPosition() and ParsePosition.getErrorPosition().
 */
void NumberFormatRegressionTest::Test4052223()
{
    //try {
    UErrorCode status = U_ZERO_ERROR;
        DecimalFormat *fmt = new DecimalFormat(UnicodeString("#,#00.00"), status);
        if(U_FAILURE(status)) {
          errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
          delete fmt;
          return;
        }
        failure(status, "new DecimalFormat");
        Formattable num;
        fmt->parse(UnicodeString("abc3"), num, status);
        if(! U_FAILURE(status))
            errln(UnicodeString("Bug 4052223 failed : can't parse string \"a\".  Got ") /*+ num*/);
    /*} catch (ParseException foo) {
        logln("Caught expected ParseException : " + foo.getMessage() + " at index : " + foo.getErrorOffset());
    }*/
    delete fmt;
}
/* @bug 4061302
 * API tests for API addition request A9.
 */
void NumberFormatRegressionTest::Test4061302()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols *fmt = new DecimalFormatSymbols(status);
    if (U_FAILURE(status)) {
        dataerrln("Error creating DecimalFormatSymbols - %s", u_errorName(status));
        return;
    }
    UnicodeString currency(fmt->getSymbol(DecimalFormatSymbols::kCurrencySymbol));
    UnicodeString intlCurrency(fmt->getSymbol(DecimalFormatSymbols::kIntlCurrencySymbol));
    UnicodeString monDecSeparator(fmt->getSymbol(DecimalFormatSymbols::kMonetarySeparatorSymbol));
    if (currency == UnicodeString("") ||
        intlCurrency == UnicodeString("") ||
        monDecSeparator == UnicodeString(""))
    {
        errln("getCurrencySymbols failed, got empty string.");
    }
    UnicodeString monDecSeparatorStr;
    monDecSeparatorStr.append(monDecSeparator);
    logln(UnicodeString("Before set ==> Currency : ") + currency + UnicodeString(" Intl Currency : ") + intlCurrency + UnicodeString(" Monetary Decimal Separator : ") + monDecSeparatorStr);
    fmt->setSymbol(DecimalFormatSymbols::kCurrencySymbol, UnicodeString("XYZ"));
    fmt->setSymbol(DecimalFormatSymbols::kIntlCurrencySymbol, UnicodeString("ABC"));
    fmt->setSymbol(DecimalFormatSymbols::kMonetarySeparatorSymbol, UnicodeString(static_cast<char16_t>(0x002A)/*'*'*/));
    currency = fmt->getSymbol(DecimalFormatSymbols::kCurrencySymbol);
    intlCurrency = fmt->getSymbol(DecimalFormatSymbols::kIntlCurrencySymbol);
    monDecSeparator = fmt->getSymbol(DecimalFormatSymbols::kMonetarySeparatorSymbol);
    if (currency != UnicodeString("XYZ") ||
        intlCurrency != UnicodeString("ABC") ||
        monDecSeparator != UnicodeString(static_cast<char16_t>(0x002A)/*'*'*/)) {
        errln("setCurrencySymbols failed.");
    }
    monDecSeparatorStr.remove();
    monDecSeparatorStr.append(monDecSeparator);
    logln("After set ==> Currency : " + currency + " Intl Currency : " + intlCurrency + " Monetary Decimal Separator : " + monDecSeparatorStr);

    delete fmt;
}
/* @bug 4062486
 * API tests for API addition request A23. FieldPosition.getBeginIndex and
 * FieldPosition.getEndIndex.
 */
void NumberFormatRegressionTest::Test4062486()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *fmt = new DecimalFormat(UnicodeString("#,##0.00"), status);
    if (U_FAILURE(status)) {
        dataerrln("Error creating DecimalFormat - %s", u_errorName(status));
        return;
    }
    UnicodeString formatted;
    FieldPosition field(FieldPosition::DONT_CARE);
    double num = 1234.5;
    fmt->format(num, formatted, field);
    if (field.getBeginIndex() != 0 && field.getEndIndex() != 5)
        errln(UnicodeString("Format 1234.5 failed. Begin index: ") /*+ field.getBeginIndex() + " End index: " + field.getEndIndex()*/);
    field.setBeginIndex(7);
    field.setEndIndex(4);
    if (field.getBeginIndex() != 7 && field.getEndIndex() != 4)
        errln("Set begin/end field indexes failed. Begin index: " /*+ field.getBeginIndex() + " End index: " + field.getEndIndex()*/);

    delete fmt;
}

/* @bug 4108738
 * DecimalFormat.parse incorrectly works with a group separator.
 */
void NumberFormatRegressionTest::Test4108738()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols *syms = new DecimalFormatSymbols(Locale::getUS(), status);
    failure(status, "new DecimalFormatSymbols");
    DecimalFormat *df = new DecimalFormat("#,##0.###", syms, status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    UnicodeString text("1.222,111");
    Formattable num;
    ParsePosition pp(0);
    df->parse(text, num, pp);
    
    // {sfb} how to do this (again) ?
    // shouldn't just be another round-trip test, should it?
    UnicodeString temp;
    FieldPosition pos(FieldPosition::DONT_CARE);
    temp = df->format(num.getDouble(), temp, pos);
    //if (!num.toString().equals("1.222"))
    if (temp != UnicodeString("1.222"))
        //errln("\"" + text + "\"  is parsed as " + num);
        errln("\"" + text + "\"  is parsed as " + temp);
    text = UnicodeString("1.222x111");
    pp = ParsePosition(0);
    df->parse(text, num, pp);
    temp.remove();
    temp = df->format(num.getDouble(), temp, pos);
    //if (!num.toString().equals("1.222"))
    if (temp != UnicodeString("1.222"))
        errln("\"" + text + "\"  is parsed as " + temp);

    delete df;
}

/* @bug 4106658
 * DecimalFormat.format() incorrectly formats negative doubles.
 */
void NumberFormatRegressionTest::Test4106658()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status); // Corrected; see 4147706
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    volatile double d1 = 0.0;   // volatile to prevent code optimization
    double d2 = -0.0001;
    UnicodeString buffer;
    UnicodeString temp;
    FieldPosition pos(FieldPosition::DONT_CARE);

#if U_PLATFORM == U_PF_HPUX
    d1 = 0.0 * -1.0;    // old HPUX compiler ignores volatile keyword
#else
    d1 = d1 * -1.0; // Some compilers have a problem with defining -0.0
#endif
    logln("pattern: \"" + df->toPattern(temp) + "\"");
    df->format(d1, buffer, pos);
    if (buffer != UnicodeString("-0")) // Corrected; see 4147706
        errln(UnicodeString("") + d1 + "      is formatted as " + buffer);
    buffer.remove();
    df->format(d2, buffer, pos);
    if (buffer != UnicodeString("-0")) // Corrected; see 4147706
        errln(UnicodeString("") + d2 + "      is formatted as " + buffer);

    delete df;
}

/* @bug 4106662
 * DecimalFormat.parse returns 0 if string parameter is incorrect.
 */
void NumberFormatRegressionTest::Test4106662()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    UnicodeString text("x");
    ParsePosition pos1(0), pos2(0);

    UnicodeString temp;
    logln("pattern: \"" + df->toPattern(temp) + "\"");
    Formattable num;
    df->parse(text, num, pos1);
    if (pos1 == ParsePosition(0)/*num != null*/) {
        errln(UnicodeString("Test Failed: \"") + text + "\" is parsed as " /*+ num*/);
    }
    delete df;
    df = new DecimalFormat(UnicodeString("$###.00"), status);
    failure(status, "new DecimalFormat");
    df->parse(UnicodeString("$"), num, pos2);
    if (pos2 == ParsePosition(0) /*num != null*/){
        errln(UnicodeString("Test Failed: \"$\" is parsed as ") /*+ num*/);
    }

    delete df;
}

/* @bug 4114639 (duplicate of 4106662)
 * NumberFormat.parse doesn't return null
 */
void NumberFormatRegressionTest::Test4114639()
{
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *format = NumberFormat::createInstance(status);
    if(U_FAILURE(status)) {
      dataerrln("Error creating DecimalFormat: %s", u_errorName(status));
      delete format;
      return;
    }
    failure(status, "NumberFormat::createInstance");
    UnicodeString text("time 10:x");
    ParsePosition pos(8);
    Formattable result;
    format->parse(text, result, pos);
    if (/*result != null*/pos.getErrorIndex() != 8) 
        errln(UnicodeString("Should return null but got : ") /*+ result*/); // Should be null; it isn't

    delete format;
}

/* @bug 4106664
 * TODO: this test does not work because we need to use a 64 bit number and a
 * a double only MAY only have 52 bits of precision.
 * DecimalFormat.format(long n) fails if n * multiplier > MAX_LONG.
 */
void NumberFormatRegressionTest::Test4106664()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    // {sfb} long in java is 64 bits
    /*long*/double n = 1234567890123456.0;
    /*int*/int32_t m = 12345678;
    // {sfb} will this work?
    //BigInteger bigN = BigInteger.valueOf(n);
    //bigN = bigN.multiply(BigInteger.valueOf(m));
    double bigN = n * m;
    df->setMultiplier(m);
    df->setGroupingUsed(false);
    UnicodeString temp;
    FieldPosition pos(FieldPosition::DONT_CARE);
    logln("formatted: " +
        df->format(n, temp, pos));
    
    char buf [128];
    snprintf(buf, sizeof(buf), "%g", bigN);
    //logln("expected: " + bigN.toString());
    logln(UnicodeString("expected: ") + buf);

    delete df;
}
/* @bug 4106667 (duplicate of 4106658)
 * DecimalFormat.format incorrectly formats -0.0.
 */
void NumberFormatRegressionTest::Test4106667()
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat(status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete df;
      return;
    }
    failure(status, "new DecimalFormat");
    char16_t foo [] = { 0x002B };
    UnicodeString bar(foo, 1, 1);
    volatile double d = 0.0;   // volatile to prevent code optimization
    UnicodeString temp;
    UnicodeString buffer;
    FieldPosition pos(FieldPosition::DONT_CARE);

    logln("pattern: \"" + df->toPattern(temp) + "\"");
#if U_PLATFORM == U_PF_HPUX
    d = 0.0 * -1.0;    // old HPUX compiler ignores volatile keyword
#else
    d = d * -1.0; // Some compilers have a problem with defining -0.0
#endif
    df->setPositivePrefix(/*"+"*/bar);
    df->format(d, buffer, pos);
    if (buffer != UnicodeString("-0")) // Corrected; see 4147706
        errln(/*d + */UnicodeString("  is formatted as ") + buffer);

    delete df;
}

/* @bug 4110936
 * DecimalFormat.setMaximumIntegerDigits() works incorrectly.
 */
#if U_PLATFORM == U_PF_OS390
#   define MAX_INT_DIGITS 70
#else
#   define MAX_INT_DIGITS 128
#endif

void NumberFormatRegressionTest::Test4110936()
{
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *nf = NumberFormat::createInstance(status);
    if(U_FAILURE(status)) {
      dataerrln("Error creating DecimalFormat: %s", u_errorName(status));
      delete nf;
      return;
    }
    failure(status, "NumberFormat::createInstance");
    nf->setMaximumIntegerDigits(MAX_INT_DIGITS);
    logln("setMaximumIntegerDigits(MAX_INT_DIGITS)");
    if (nf->getMaximumIntegerDigits() != MAX_INT_DIGITS)
        errln(UnicodeString("getMaximumIntegerDigits() returns ") +
            nf->getMaximumIntegerDigits());

    delete nf;
}

/* @bug 4122840
 * Locale data should use generic currency symbol
 *
 * 1) Make sure that all currency formats use the generic currency symbol.
 * 2) Make sure we get the same results using the generic symbol or a
 *    hard-coded one.
 */
void NumberFormatRegressionTest::Test4122840()
{
    int32_t count = 0;
    const Locale *locales = Locale::getAvailableLocales(count);
    
    for (int i = 0; i < count; i++) {
        UErrorCode status = U_ZERO_ERROR;
        ResourceBundle *rb = new ResourceBundle(
            nullptr/*"java.text.resources.LocaleElements"*/, 
            locales[i], status);
        failure(status, "new ResourceBundle");
        ResourceBundle numPat = rb->getWithFallback("NumberElements", status);
        failure(status, "rb.get(NumberElements)");
        numPat = numPat.getWithFallback("latn",status);
        failure(status, "rb.get(latn)");
        numPat = numPat.getWithFallback("patterns",status);
        failure(status, "rb.get(patterns)");
        numPat = numPat.getWithFallback("currencyFormat",status);
        failure(status, "rb.get(currencyFormat)");
       //
        // Get the currency pattern for this locale.  We have to fish it
        // out of the ResourceBundle directly, since DecimalFormat.toPattern
        // will return the localized symbol, not \00a4
        //
        UnicodeString pattern = numPat.getString(status);
        failure(status, "rb->getString()");

        char16_t fo[] = { 0x00A4 };
        UnicodeString foo(fo, 1, 1);

        //if (pattern.indexOf("\u00A4") == -1 ) {
        if (pattern.indexOf(foo) == -1 ) {
            errln(UnicodeString("Currency format for ") + UnicodeString(locales[i].getName()) +
                    " does not contain generic currency symbol:" +
                    pattern );
        }

        // Create a DecimalFormat using the pattern we got and format a number
        DecimalFormatSymbols *symbols = new DecimalFormatSymbols(locales[i], status);
        failure(status, "new DecimalFormatSymbols");

        // Disable currency spacing for the purposes of this test.
        // To do this, set the spacing insert to the empty string both before and after the symbol.
        symbols->setPatternForCurrencySpacing(UNUM_CURRENCY_INSERT, false, u"");
        symbols->setPatternForCurrencySpacing(UNUM_CURRENCY_INSERT, true, u"");

        DecimalFormat *fmt1 = new DecimalFormat(pattern, *symbols, status);
        failure(status, "new DecimalFormat");
        
        UnicodeString result1;
        FieldPosition pos(FieldPosition::DONT_CARE);
        result1 = fmt1->format(1.111, result1, pos);
        
        //
        // Now substitute in the locale's currency symbol and create another
        // pattern.  We have to skip locales where the currency symbol
        // contains decimal separators, because that confuses things
        //
        char16_t ba[] = { 0x002E/*'.'*/ };
        UnicodeString bar(ba, 1, 1);

        if (symbols->getSymbol(DecimalFormatSymbols::kCurrencySymbol).indexOf(bar) == -1) {
            // {sfb} Also, switch the decimal separator to the monetary decimal
            // separator to mimic the behavior of a currency format
            symbols->setSymbol(DecimalFormatSymbols::kDecimalSeparatorSymbol,
                symbols->getSymbol(DecimalFormatSymbols::kMonetarySeparatorSymbol));
            
            UnicodeString buf(pattern);
            for (int j = 0; j < buf.length(); j++) {
                if (buf[j] == 0x00a4 ) {
                    if(buf[j + 1] == 0x00a4) {
                        // {sfb} added to support double currency marker (intl currency sign)
                        buf.replace(j, /*j+*/2, symbols->getSymbol(DecimalFormatSymbols::kIntlCurrencySymbol));
                        j += symbols->getSymbol(DecimalFormatSymbols::kIntlCurrencySymbol).length();
                    }
                    else {
                        buf.replace(j, /*j+*/1, symbols->getSymbol(DecimalFormatSymbols::kCurrencySymbol)); 
                        j += symbols->getSymbol(DecimalFormatSymbols::kCurrencySymbol).length() - 1;
                    }                    
                }
            }

            DecimalFormat *fmt2 = new DecimalFormat(buf, *symbols, status);
            failure(status, "new DecimalFormat");
            
            // Get the currency (if there is one) so we can set the rounding and fraction
            const char16_t *currency = fmt1->getCurrency();
            if (*currency != 0) {
                double rounding = ucurr_getRoundingIncrement(currency, &status);
                int32_t frac = ucurr_getDefaultFractionDigits(currency, &status);
                if (U_SUCCESS(status)) {
                    fmt2->setRoundingIncrement(rounding);
                    fmt2->setMinimumFractionDigits(frac);
                    fmt2->setMaximumFractionDigits(frac);
                }
                else {
                    failure(status, "Fetching currency rounding/fractions");
                }
            }
            
            UnicodeString result2;
            fmt2->format(1.111, result2, pos);
            
            if (result1 != result2) {
                errln("Results for " + UnicodeString(locales[i].getName()) + " differ: " +
                        result1 + " vs " + result2);
            }
        
            delete fmt2;
        }
    
        delete rb;
        delete fmt1;
        delete symbols;
    }
}
 
/* @bug 4125885
 * DecimalFormat.format() delivers wrong string.
 */
void NumberFormatRegressionTest::Test4125885()
{
    UErrorCode status = U_ZERO_ERROR;
    double rate = 12.34;
    DecimalFormat *formatDec = new DecimalFormat ("000.00", status);
    if(U_FAILURE(status)) {
      errcheckln(status, "Error creating DecimalFormat: %s", u_errorName(status));
      delete formatDec;
      return;
    }
    failure(status, "new DecimalFormat");
    UnicodeString temp;
    logln("toPattern: " + formatDec->toPattern(temp));
    UnicodeString rateString;
    FieldPosition pos(FieldPosition::DONT_CARE);
    rateString = formatDec->format(rate, rateString, pos);
    if (rateString != UnicodeString("012.34"))
        errln("result : " + rateString + " expected : 012.34");
    rate = 0.1234;
    delete formatDec;// = null;
    formatDec = new DecimalFormat ("+000.00%;-000.00%", status);
    failure(status, "new DecimalFormat");
    logln("toPattern: " + formatDec->toPattern(temp));
    rateString.remove();
    rateString = formatDec->format(rate, rateString, pos);
    if (rateString != UnicodeString("+012.34%"))
        errln("result : " + rateString + " expected : +012.34%");

    delete formatDec;
}

/**
 * @bug 4134034
 * DecimalFormat produces extra zeros when formatting numbers.
 */
void NumberFormatRegressionTest::Test4134034() 
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *nf = new DecimalFormat("##,###,###.00", status);
    if (!failure(status, "new DecimalFormat")) {
        UnicodeString f;
        FieldPosition pos(FieldPosition::DONT_CARE);
        f = nf->format(9.02, f, pos);
        if (f == UnicodeString("9.02")) 
            logln(f + " ok"); 
        else 
            errln("9.02 -> " + f + "; want 9.02");

        f.remove();
        f = nf->format(static_cast<int32_t>(0), f, pos);
        if (f == UnicodeString(".00")) 
            logln(f + " ok"); 
        else 
            errln("0 -> " + f + "; want .00");
    }

    delete nf;
}

/**
 * @bug 4134300
 * CANNOT REPRODUCE - This bug could not be reproduced.  It may be
 * a duplicate of 4134034.
 *
 * JDK 1.1.6 Bug, did NOT occur in 1.1.5
 * Possibly related to bug 4125885.
 * 
 * This class demonstrates a regression in version 1.1.6
 * of DecimalFormat class.
 * 
 * 1.1.6 Results
 * Value 1.2 Format #.00 Result '01.20' !!!wrong
 * Value 1.2 Format 0.00 Result '001.20' !!!wrong
 * Value 1.2 Format 00.00 Result '0001.20' !!!wrong
 * Value 1.2 Format #0.0# Result '1.2'
 * Value 1.2 Format #0.00 Result '001.20' !!!wrong
 * 
 * 1.1.5 Results
 * Value 1.2 Format #.00 Result '1.20'
 * Value 1.2 Format 0.00 Result '1.20'
 * Value 1.2 Format 00.00 Result '01.20'
 * Value 1.2 Format #0.0# Result '1.2'
 * Value 1.2 Format #0.00 Result '1.20'
 */
void NumberFormatRegressionTest::Test4134300() {
    UnicodeString DATA [] = {
     // Pattern      Expected string
        UnicodeString("#.00"),      UnicodeString("1.20"),
        UnicodeString("0.00"),      UnicodeString("1.20"),
        UnicodeString("00.00"),     UnicodeString("01.20"),
        UnicodeString("#0.0#"),     UnicodeString("1.2"),
        UnicodeString("#0.00"),     UnicodeString("1.20")
    };

    for (int i=0; i< 10; i+=2) {
        UnicodeString result;
        UErrorCode status = U_ZERO_ERROR;
        DecimalFormat *df = new DecimalFormat(DATA[i], status);
        if (!failure(status, "new DecimalFormat")) {
            FieldPosition pos(FieldPosition::DONT_CARE);
            result = df->format(1.2, result, pos);
            if (result != DATA[i+1]) {
                errln("Fail: 1.2 x " + DATA[i] + " = " + result +
                      "; want " + DATA[i+1]);
            }
            else {
                logln("Ok: 1.2 x " + DATA[i] + " = " + result);
            }
        }
    
        delete df;
    }
}

/**
 * @bug 4140009
 * Empty pattern produces double negative prefix.
 */
void NumberFormatRegressionTest::Test4140009() 
{
    UErrorCode status = U_ZERO_ERROR;
    LocalPointer<DecimalFormatSymbols> syms(new DecimalFormatSymbols(Locale::getEnglish(), status), status);
    if (failure(status, "new DecimalFormatSymbols")) {
        return;
    }
    DecimalFormat *f = new DecimalFormat(UnicodeString(u""), syms.orphan(), status);
    if (!failure(status, "new DecimalFormat")) {
        UnicodeString s;
        FieldPosition pos(FieldPosition::DONT_CARE);
        s = f->format(123.456, s, pos);
        if (s != UnicodeString("123.456"))
            errln("Fail: Format empty pattern x 123.456 => " + s);
        s.remove();
        s = f->format(-123.456, s, pos);
        if (s != UnicodeString("-123.456"))
            errln("Fail: Format empty pattern x -123.456 => " + s);
    }
    delete f;
}

/**
 * @bug 4141750
 * BigDecimal numbers get their fractions truncated by NumberFormat.
 */
// {sfb} not pertinent in C++ ??
void NumberFormatRegressionTest::Test4141750() {
    /*try {
        UnicodeString str("12345.67");
        BigDecimal bd = new BigDecimal(str);
        String sd = NumberFormat.getInstance(Locale.US).format(bd);
        if (!sd.endsWith("67")) errln("Fail: " + str + " x format -> " + sd);
    }
    catch (Exception e) {
        errln(e.toString());
        e.printStackTrace();
    }*/
}

/**
 * @bug 4145457
 * DecimalFormat toPattern() doesn't quote special characters or handle
 * single quotes.
 */
void NumberFormatRegressionTest::Test4145457() {
    //try {
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *nff = NumberFormat::createInstance(status);
    if (failure(status, "NumberFormat::createInstance", true)){
        delete nff;
        return;
    }
    DecimalFormat *nf = dynamic_cast<DecimalFormat *>(nff);
    if(nf == nullptr) {
        errln("DecimalFormat needed to continue");
        return;
    }

    DecimalFormatSymbols *sym = const_cast<DecimalFormatSymbols*>(nf->getDecimalFormatSymbols());
    sym->setSymbol(DecimalFormatSymbols::kDecimalSeparatorSymbol, static_cast<char16_t>(0x0027));
    nf->setDecimalFormatSymbols(*sym);
    double pi = 3.14159;
    
    UnicodeString PATS [] = { 
        UnicodeString("#.00 'num''ber'"), UnicodeString("''#.00''")
    };

    for (int32_t i=0; i<2; ++i) {
        nf->applyPattern(PATS[i], status);
        failure(status, "nf->applyPattern");
        UnicodeString out;
        FieldPosition pos(FieldPosition::DONT_CARE);
        out = nf->format(pi, out, pos);
        UnicodeString pat;
        pat = nf->toPattern(pat);
        Formattable num;
        ParsePosition pp(0);
        nf->parse(out, num, pp);
        double val = num.getDouble();
        
        nf->applyPattern(pat, status);
        failure(status, "nf->applyPattern");
        UnicodeString out2;
        out2 = nf->format(pi, out2, pos);
        UnicodeString pat2;
        pat2 = nf->toPattern(pat2);
        pp.setIndex(0);
        nf->parse(out2, num, pp);
        double val2 = num.getDouble();
        
        if (pat != pat2)
            errln("Fail with \"" + PATS[i] + "\": Patterns should concur, \"" +
                pat + "\" vs. \"" + pat2 + "\"");
        else
            logln("Ok \"" + PATS[i] + "\" toPattern() -> \"" + pat + '"');
        
        if (val == val2 && out == out2) {
            logln(UnicodeString("Ok ") + pi + " x \"" + PATS[i] + "\" -> \"" +
                out + "\" -> " + val + " -> \"" +
                out2 + "\" -> " + val2);
        }
        else {
            errln(UnicodeString("Fail ") + pi + " x \"" + PATS[i] + "\" -> \"" +
                out + "\" -> " + val + " -> \"" +
                out2 + "\" -> " + val2);
        }
    }
    /*}
    catch (ParseException e) {
        errln("Fail: " + e);
        e.printStackTrace();
    }*/

    delete nff;
}

/**
 * @bug 4147295
 * DecimalFormat.applyPattern() sets minimum integer digits incorrectly.
 * CANNOT REPRODUCE
 * This bug is a duplicate of 4139344, which is a duplicate of 4134300
 *
 * ICU 62: minInt is always at least one, and the getter should reflect that!
 */
void NumberFormatRegressionTest::Test4147295() 
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *sdf = new DecimalFormat(status);
    UnicodeString pattern("#,###");
    logln("Applying pattern \"" + pattern + "\"");
    sdf->applyPattern(pattern, status);
    if (!failure(status, "sdf->applyPattern")) {
        int minIntDig = sdf->getMinimumIntegerDigits();
        if (minIntDig != 1) {
            errln("Test failed");
            errln(UnicodeString(" Minimum integer digits : ") + minIntDig);
            UnicodeString temp;
            errln(UnicodeString(" new pattern: ") + sdf->toPattern(temp));
        } else {
            logln("Test passed");
            logln(UnicodeString(" Minimum integer digits : ") + minIntDig);
        }
    }
    delete sdf;
}

/**
 * @bug 4147706
 * DecimalFormat formats -0.0 as +0.0
 * See also older related bug 4106658, 4106667
 */
void NumberFormatRegressionTest::Test4147706() 
{
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *df = new DecimalFormat("#,##0.0##", status);
    failure(status, "new DecimalFormat");
    DecimalFormatSymbols *syms = new DecimalFormatSymbols(Locale::getEnglish(), status);
    if (!failure(status, "new DecimalFormatSymbols")) {
        UnicodeString f1;
        UnicodeString f2, temp;
        FieldPosition pos(FieldPosition::DONT_CARE);
        volatile double d1 = 0.0;   // volatile to prevent code optimization
        double d2 = -0.0001;

#if U_PLATFORM == U_PF_HPUX
        d1 = 0.0 * -1.0;    // old HPUX compiler ignores volatile keyword
#else
        d1 = d1 * -1.0; // Some compilers have a problem with defining -0.0
#endif
        df->adoptDecimalFormatSymbols(syms);
        f1 = df->format(d1, f1, pos);
        f2 = df->format(d2, f2, pos);
        if (f1 != UnicodeString("-0.0")) {
            errln(UnicodeString("") + d1 + UnicodeString(" x \"") + df->toPattern(temp) + "\" is formatted as \"" + f1 + '"');
        }
        if (f2 != UnicodeString("-0.0")) {
            errln(UnicodeString("") + d2 + UnicodeString(" x \"") + df->toPattern(temp) + "\" is formatted as \"" + f2 + '"');
        }
    }
    
    delete df;
}


// Not applicable, since no serialization in C++
/*class myformat implements Serializable
{
DateFormat _dateFormat = DateFormat.getDateInstance();

public String Now()
{
    GregorianCalendar calendar = new GregorianCalendar();
    Date t = calendar.getTime();
    String nowStr = _dateFormat.format(t);
    return nowStr;
}
}*/

/**
 * @bug 4162198
 * NumberFormat cannot format Double.MAX_VALUE
 */
// TODO: make this test actually test something
void 
NumberFormatRegressionTest::Test4162198() 
{
    // for some reason, DBL_MAX will not round trip. (bug in snprintf/atof)
    double dbl = INT32_MAX * 1000.0;
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *f = NumberFormat::createInstance(status);
    if(U_FAILURE(status)) {
        dataerrln("Couldn't create number format - %s", u_errorName(status));
        return;
    }
    f->setMaximumFractionDigits(INT32_MAX);
    f->setMaximumIntegerDigits(INT32_MAX);
    UnicodeString s;
    f->format(dbl,s);
    logln(UnicodeString("The number ") + dbl + " formatted to " + s);
    Formattable n;
    //try {
    f->parse(s, n, status);
    if(U_FAILURE(status))
        errln("Couldn't parse!");
    //} catch (java.text.ParseException e) {
    //    errln("Caught a ParseException:");
    //    e.printStackTrace();
    //}
    
    //logln("The string " + s + " parsed as " + n);

    // {dlf} The old code assumes n is a double, but it isn't any more...
    // Formattable apparently does not and never did interconvert... too bad.
    //if(n.getDouble() != dbl) {
    //    errln("Round trip failure");
    //}
    if (n.getInt64() != dbl) {
        errln("Round trip failure");
    }

    delete f;
}

/**
 * @bug 4162852
 * NumberFormat does not parse negative zero.
 */
void 
NumberFormatRegressionTest::Test4162852() 
{
    UErrorCode status = U_ZERO_ERROR;
    for(int32_t i=0; i < 2; ++i) {
        LocalPointer<NumberFormat> f(
            ((i == 0) ? NumberFormat::createInstance(status) : NumberFormat::createPercentInstance(status)),
            status);
        if(U_FAILURE(status)) {
            dataerrln("Couldn't create number format - %s", u_errorName(status));
            return;
        }
        double d = 0.0;
        d *= -1.0;
        UnicodeString s;
        f->format(d, s);
        Formattable n;
        f->parse(s, n, status);
        if(U_FAILURE(status)) {
            errln("Couldn't parse!");
            return;
        }
        double e = n.getDouble();
        logln("%f -> \"%s\" -> %f", d, CStr(s)(), e);
#if (U_PLATFORM == U_PF_OS390 && !defined(IEEE_754)) || U_PLATFORM == U_PF_OS400
        if (e != 0.0) {
#else
        if (e != 0.0 || (std::signbit(e) == false)) {
#endif
            errln("Failed to parse negative zero");
        }
    }
}

static double _u_abs(double a) { return a<0?-a:a; }

/**
 * May 17 1999 sync up - liu
 * @bug 4167494
 * NumberFormat truncates data
 */
void NumberFormatRegressionTest::Test4167494() {
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *fmt = NumberFormat::createInstance(Locale::getUS(), status);
    if (failure(status, "NumberFormat::createInstance", true)){
        delete fmt;
        return;
    }

    double a = DBL_MAX * 0.99; // DBL_MAX itself overflows to +Inf
    UnicodeString s;
    fmt->format(a, s);
    Formattable num;
    fmt->parse(s, num, status);
    failure(status, "Parse");
    if (num.getType() == Formattable::kDouble &&
        _u_abs(num.getDouble() - a) / a < 0.01) { // RT within 1%
        logln(UnicodeString("") + a + " -> \"" + s + "\" -> " +
              toString(num) + " ok");
    } else {
        errln(UnicodeString("") + a + " -> \"" + s + "\" -> " +
              toString(num) + " FAIL");
    }

    // We don't test Double.MIN_VALUE because the locale data for the US
    // currently doesn't specify enough digits to display Double.MIN_VALUE.
    // This is correct for now; however, we leave this here as a reminder
    // in case we want to address this later.

    delete fmt;
}

/**
 * May 17 1999 sync up - liu
 * @bug 4170798
 * DecimalFormat.parse() fails when ParseIntegerOnly set to true
 */
void NumberFormatRegressionTest::Test4170798() {
    IcuTestErrorCode status(*this, "Test4170798");
    LocalPointer<DecimalFormat> df(dynamic_cast<DecimalFormat*>(
            NumberFormat::createInstance(Locale::getUS(), status)), status);
    if (!assertSuccess("", status, true, __FILE__, __LINE__)) {
        return;
    }
    {
        Formattable n;
        ParsePosition pos(0);
        df->parse("-0.0", n, pos);
        if (n.getType() != Formattable::kDouble
            || n.getDouble() != -0.0) {
            errln(UnicodeString("FAIL: default parse(\"-0.0\") returns ") + toString(n));
        }
    }
    df->setParseIntegerOnly(true);
    {
        Formattable n;
        ParsePosition pos(0);
        df->parse("-0.0", n, pos);
        if (n.getType() != Formattable::kLong
            || n.getLong() != 0) {
            errln(UnicodeString("FAIL: integer parse(\"-0.0\") returns ") + toString(n));
        }
    }
}

/**
 * May 17 1999 sync up - liu
 * toPattern only puts the first grouping separator in.
 */
void NumberFormatRegressionTest::Test4176114() {
    const char* DATA[] = {
        "00", "00",
        "000", "000", // No grouping
        "#000", "000", // No grouping
        "#,##0", "#,##0",
        "#,000", "#,000",
        "0,000", "0,000",
        "00,000", "00,000",
        "000,000", "000,000",
        "0,000,000,000,000.0000", "0,000,000,000,000.0000", // Reported
    };
    int DATA_length = UPRV_LENGTHOF(DATA);
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString s;
    for (int i=0; i<DATA_length; i+=2) {
        DecimalFormat df(DATA[i], status);
        if (!failure(status, "DecimalFormat constructor")) {
            df.toPattern(s);
            UnicodeString exp(DATA[i+1]);
            if (s != exp) {
                errln(UnicodeString("FAIL: ") + DATA[i] + " -> " +
                      s + ", want " + exp);
            }
        }
    }
}

/**
 * May 17 1999 sync up - liu
 * @bug 4179818
 * DecimalFormat is incorrectly rounding numbers like 1.2501 to 1.2
 */
void NumberFormatRegressionTest::Test4179818() {
    const char* DATA[] = {
        // Input  Pattern  Expected output
        "1.2511", "#.#",   "1.3",
        "1.2501", "#.#",   "1.3",
        "0.9999", "#",     "1",
    };
    int DATA_length = UPRV_LENGTHOF(DATA); 
    double DOUBLE[] = {
        1.2511,
        1.2501,
        0.9999,
    };
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols sym(Locale::getUS(), status);
    failure(status, "Construct DecimalFormatSymbols");
    DecimalFormat fmt("#", sym, status);
    if (!failure(status, "Construct DecimalFormat")) {
        for (int i=0; i<DATA_length; i+=3) {
            double in = DOUBLE[i/3];
            UnicodeString pat(DATA[i+1]);
            UnicodeString exp(DATA[i+2]);
            fmt.applyPattern(pat, status);
            failure(status, "applyPattern");
            UnicodeString out;
            FieldPosition pos;
            fmt.format(in, out, pos);
            if (out == exp) {
                logln(UnicodeString("Ok: ") + in + " x " + pat + " = " + out);
            } else {
                errln(UnicodeString("FAIL: ") + in + " x  " + pat + " = " + out +
                      ", expected " + exp);
            }
        }
    }
}

/**
 * May 17 1999 sync up - liu
 * Some DecimalFormatSymbols changes are not picked up by DecimalFormat.
 * This includes the minus sign, currency symbol, international currency
 * symbol, percent, and permille.  This is filed as bugs 4212072 and
 * 4212073.
 */
void NumberFormatRegressionTest::Test4212072() {
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols sym(Locale::getUS(), status);

    failure(status, "DecimalFormatSymbols ct", Locale::getUS());
    DecimalFormat fmt(UnicodeString("#"), sym, status);
    if(failure(status, "DecimalFormat ct", Locale::getUS())) {
        return;
    }

    UnicodeString s;
    FieldPosition pos;

    sym.setSymbol(DecimalFormatSymbols::kMinusSignSymbol, static_cast<char16_t>(0x5e));
    fmt.setDecimalFormatSymbols(sym);
    s.remove();
    if (fmt.format(static_cast<int32_t>(-1), s, pos) != UNICODE_STRING("^1", 2)) {
        errln(UnicodeString("FAIL: -1 x (minus=^) -> ") + s +
              ", exp ^1");
    }
    s.remove();
    if (fmt.getNegativePrefix(s) != UnicodeString(static_cast<char16_t>(0x5e))) {
        errln(UnicodeString("FAIL: (minus=^).getNegativePrefix -> ") +
              s + ", exp ^");
    }
    sym.setSymbol(DecimalFormatSymbols::kMinusSignSymbol, static_cast<char16_t>(0x2d));

    fmt.applyPattern(UnicodeString("#%"), status);
    failure(status, "applyPattern percent");
    sym.setSymbol(DecimalFormatSymbols::kPercentSymbol, static_cast<char16_t>(0x5e));
    fmt.setDecimalFormatSymbols(sym);
    s.remove();
    if (fmt.format(0.25, s, pos) != UNICODE_STRING("25^", 3)) {
        errln(UnicodeString("FAIL: 0.25 x (percent=^) -> ") + s +
              ", exp 25^");
    }
    s.remove();
    if (fmt.getPositiveSuffix(s) != UnicodeString(static_cast<char16_t>(0x5e))) {
        errln(UnicodeString("FAIL: (percent=^).getPositiveSuffix -> ") +
              s + ", exp ^");
    }
    sym.setSymbol(DecimalFormatSymbols::kPercentSymbol, static_cast<char16_t>(0x25));

    fmt.applyPattern(str("#\\u2030"), status);
    failure(status, "applyPattern permill");
    sym.setSymbol(DecimalFormatSymbols::kPerMillSymbol, static_cast<char16_t>(0x5e));
    fmt.setDecimalFormatSymbols(sym);
    s.remove();
    if (fmt.format(0.25, s, pos) != UNICODE_STRING("250^", 4)) {
        errln(UnicodeString("FAIL: 0.25 x (permill=^) -> ") + s +
              ", exp 250^");
    }
    s.remove();
    if (fmt.getPositiveSuffix(s) != UnicodeString(static_cast<char16_t>(0x5e))) {
        errln(UnicodeString("FAIL: (permill=^).getPositiveSuffix -> ") +
              s + ", exp ^");
    }
    sym.setSymbol(DecimalFormatSymbols::kPerMillSymbol, static_cast<char16_t>(0x2030));

    fmt.applyPattern(str("\\u00A4#.00"), status);
    failure(status, "applyPattern currency");
    sym.setSymbol(DecimalFormatSymbols::kCurrencySymbol, "usd");
    fmt.setDecimalFormatSymbols(sym);
    s.remove();
    if (fmt.format(12.5, s, pos) != UnicodeString(u"usd\u00A012.50")) {
        errln(UnicodeString("FAIL: 12.5 x (currency=usd) -> ") + s +
              u", exp usd\u00A012.50");
    }
    s.remove();
    if (fmt.getPositivePrefix(s) != UnicodeString("usd")) {
        errln(UnicodeString("FAIL: (currency=usd).getPositivePrefix -> ") +
              s + ", exp usd");
    }
    sym.setSymbol(DecimalFormatSymbols::kCurrencySymbol, "$");

    fmt.applyPattern(str("\\u00A4\\u00A4#.00"), status);
    failure(status, "applyPattern intl currency");
    sym.setSymbol(DecimalFormatSymbols::kIntlCurrencySymbol, "DOL");
    fmt.setDecimalFormatSymbols(sym);
    s.remove();
    if (fmt.format(12.5, s, pos) != UnicodeString(u"DOL\u00A012.50")) {
        errln(UnicodeString("FAIL: 12.5 x (intlcurrency=DOL) -> ") + s +
              u", exp DOL\u00A012.50");
    }
    s.remove();
    if (fmt.getPositivePrefix(s) != UnicodeString("DOL")) {
        errln(UnicodeString("FAIL: (intlcurrency=DOL).getPositivePrefix -> ") +
              s + ", exp DOL");
    }
    sym.setSymbol(DecimalFormatSymbols::kIntlCurrencySymbol, "USD");

    // Since the pattern logic has changed, make sure that patterns round
    // trip properly.  Test stream in/out integrity too.
    int32_t n;
    const Locale* avail = NumberFormat::getAvailableLocales(n);
    static const char* type[] = {
        "",
        "$ ",
        "% ",
    };
    for (int i=0; i<n; ++i) {
        for (int j=0; j<3; ++j) {
            status = U_ZERO_ERROR;
            NumberFormat *nf;
            switch (j) {
            case 0:
                nf = NumberFormat::createInstance(avail[i], status);
                failure(status, "createInstance", avail[i]);
                break;
            case 1:
                nf = NumberFormat::createCurrencyInstance(avail[i], status);
                failure(status, "createCurrencyInstance", avail[i]);
                break;
            default:
                nf = NumberFormat::createPercentInstance(avail[i], status);
                failure(status, "createPercentInstance", avail[i]);
                break;
            }
            if (U_FAILURE(status)) {
                continue;
            }
            DecimalFormat *df = dynamic_cast<DecimalFormat*>(nf);

            // Test toPattern/applyPattern round trip
            UnicodeString pat;
            df->toPattern(pat);
            DecimalFormatSymbols symb(avail[i], status);
            failure(status, "Construct DecimalFormatSymbols", avail[i]);
            DecimalFormat f2(pat, symb, status);
            if (failure(status,
                        UnicodeString("Construct DecimalFormat(") + pat + ")")) {
                continue;
            }
            UnicodeString f2Pat;
            f2.toPattern(f2Pat);
            if (pat != f2Pat) {
                UnicodeString l;
                errln(UnicodeString("FAIL: ") + type[j] + avail[i].getDisplayName(l) +
                      " -> \"" + pat +
                      "\" -> \"" + f2Pat + "\"");
            } else {
                UnicodeString l, p;
                logln(UnicodeString("PASS: ") + type[j] + avail[i].getDisplayName(l) +
                      " -> \"" + pat +
                      "\"");
            }

            // Test toLocalizedPattern/applyLocalizedPattern round trip
// TODO(refactor): don't worry about localized patterns for now.
//            df->toLocalizedPattern(pat);
//           f2.applyLocalizedPattern(pat, status);
            failure(status,
                    UnicodeString("applyLocalizedPattern(") + pat + ")", avail[i]);
            if (U_FAILURE(status)) {
                continue;
            }
            
// TODO(refactor): don't worry about localized patterns for now.
            // Make sure we set the currency attributes appropriately
//            if (j == 1) {   // Currency format
//                f2.setCurrency(f2.getCurrency(), status);
//            }
            failure(status,
                    UnicodeString("setCurrency() for (") + pat + ")", avail[i]);
            if (U_FAILURE(status)) {
                continue;
            }

            if (pat != f2Pat) {
                UnicodeString l;
                errln(UnicodeString("FAIL: ") + type[j] + avail[i].getDisplayName(l) +
                      " -> localized \"" + pat +
                      "\" -> \"" + f2Pat + "\"");
            }

            delete nf;
 
            // Test writeObject/readObject round trip
            // NOT ON ICU -- Java only
        }
    }
}

/**
 * May 17 1999 sync up - liu
 * DecimalFormat.parse() fails for mulipliers 2^n.
 */
void NumberFormatRegressionTest::Test4216742() {
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormat *fmt = dynamic_cast<DecimalFormat*>(NumberFormat::createInstance(Locale::getUS(), status));
    if (failure(status, "createInstance", Locale::getUS(), true)){
        delete fmt;
        return;
    }
    int32_t DATA[] = { INT32_MIN, INT32_MAX, -100000000, 100000000 };
    int DATA_length = UPRV_LENGTHOF(DATA);
    for (int i=0; i<DATA_length; ++i) {
        UnicodeString str(UnicodeString("") + DATA[i]);
        for (int m = 1; m <= 100; m++) {
            fmt->setMultiplier(m);
            Formattable num;
            fmt->parse(str, num, status);
            failure(status, "parse", Locale::getUS());
            if (num.getType() != Formattable::kLong &&
                num.getType() != Formattable::kDouble) {
                errln(UnicodeString("FAIL: Wanted number, got ") +
                      toString(num));
            } else {
                double d = num.getType() == Formattable::kDouble ?
                    num.getDouble() : static_cast<double>(num.getLong());
                if ((d > 0) != (DATA[i] > 0)) {
                    errln(UnicodeString("\"") + str + "\" parse(x " +
                          fmt->getMultiplier() +
                          ") => " + toString(num));
                }
            }
        }
    }
    delete fmt;
}

/**
 * May 17 1999 sync up - liu
 * DecimalFormat formats 1.001 to "1.00" instead of "1" with 2 fraction
 * digits.
 */
void NumberFormatRegressionTest::Test4217661() {
    const double D[] = {  0.001, 1.001, 0.006,  1.006 };
    const char*  S[] = { "0",   "1",   "0.01", "1.01" };
    int D_length = UPRV_LENGTHOF(D);
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *fmt = NumberFormat::createInstance(Locale::getUS(), status);
    if (failure(status, "createInstance", Locale::getUS(), true)){
        delete fmt;
        return;
    }
    fmt->setMaximumFractionDigits(2);
    for (int i=0; i<D_length; i++) {
        UnicodeString s;
        fmt->format(D[i], s);
        if (s != UnicodeString(S[i])) {
            errln(UnicodeString("FAIL: Got ") + s + ", exp " + S[i]); 
        }
    }
    delete fmt;
}

/**
 * alphaWorks upgrade
 */
void NumberFormatRegressionTest::Test4161100() {
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *nf = NumberFormat::createInstance(Locale::getUS(), status);
    if (failure(status, "createInstance", Locale::getUS(), true)){
        delete nf;
        return;
    }
    nf->setMinimumFractionDigits(1);
    nf->setMaximumFractionDigits(1);
    double a = -0.09;
    UnicodeString s;
    nf->format(a, s);
    UnicodeString pat;
    logln(UnicodeString() + a + " x " +
          (dynamic_cast<DecimalFormat*>(nf))->toPattern(pat) + " = " + s);
    if (s != UnicodeString("-0.1")) {
        errln("FAIL");
    }
    delete nf;
}

/**
 * June 16 1999 sync up - liu
 * Formatting .5 rounds to "1" instead of "0". (Regression in 1.2.2 RC1)
 */
void NumberFormatRegressionTest::Test4243011() {
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols sym(Locale::getUS(), status);
    failure(status, "DecimalFormatSymbols ct", Locale::getUS());
    DecimalFormat fmt(UnicodeString("0."), sym, status);
    
    if (!failure(status, "DecimalFormat ct", Locale::getUS())) {
        const double NUM[] = {  -2.5,  -1.5,  -0.5,  0.5,  1.5,  2.5,  3.5,  4.5 };
        const char*  STR[] = { "-2.", "-2.", "-0.", "0.", "2.", "2.", "4.", "4." };
        int32_t N = UPRV_LENGTHOF(NUM);

        for (int32_t i=0; i<N; ++i) {
            UnicodeString str;
            UnicodeString exp(STR[i]);
            FieldPosition pos;
            fmt.format(NUM[i], str, pos);
            if (str == exp) {
                logln(UnicodeString("Ok   ") + NUM[i] + " x 0. = " + str);
            } else {
                errln(UnicodeString("FAIL ") + NUM[i] + " x 0. = " + str +
                      ", want " + exp);
            }
        }
    }
}

/**
 * June 16 1999 sync up - liu
 * format(0.0) gives "0.1" if preceded by parse("99.99").
 * (Regression in 1.2.2 RC1)
 */
void NumberFormatRegressionTest::Test4243108() {
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols sym(Locale::getUS(), status);
    failure(status, "DecimalFormatSymbols ct", Locale::getUS());
    DecimalFormat fmt(UnicodeString("#.#"), sym, status);
    if (failure(status, "DecimalFormat ct", Locale::getUS())) {
        return;
    }

    UnicodeString str;
    FieldPosition pos;

    fmt.format(0.0, str, pos);
    UnicodeString exp("0");
    if (str == exp) {
        logln(UnicodeString("Ok   0.0 x #.# = ") + str);
    } else {
        errln(UnicodeString("FAIL 0.0 x #.# = ") + str +
              ", want " + exp);
    }

    str = "99.99";
    Formattable val;
    fmt.parse(str, val, status);
    failure(status, "DecimalFormat.parse(99.99)", Locale::getUS());
    if (val.getType() == Formattable::kDouble &&
        val.getDouble() == 99.99) {
        logln(UnicodeString("Ok   99.99 / #.# = ") + toString(val));
    } else {
        errln(UnicodeString("FAIL 99.99 / #.# = ") + toString(val) +
              ", want " + 99.99);
    }

    str.remove();
    fmt.format(0.0, str, pos);
    if (str == exp) {
        logln(UnicodeString("Ok   0.0 x #.# = ") + str);
    } else {
        errln(UnicodeString("FAIL 0.0 x #.# = ") + str +
              ", want " + exp);
    }
}


/**
 * DateFormat should call setIntegerParseOnly(true) on adopted
 * NumberFormat objects.
 */
void NumberFormatRegressionTest::TestJ691() {
    UErrorCode status = U_ZERO_ERROR;
    Locale loc("fr", "CH");

    // set up the input date string & expected output
    UnicodeString udt(u"11.10.2000");
    UnicodeString exp(u"11.10.00");

    // create a Calendar for this locale
    LocalPointer<Calendar> cal(Calendar::createInstance(loc, status));
    if (U_FAILURE(status)) {
        dataerrln("FAIL: Calendar::createInstance() returned " + UnicodeString(u_errorName(status)));
        return;
    }

    // create a NumberFormat for this locale
    LocalPointer<NumberFormat> nf(NumberFormat::createInstance(loc, status));
    if (U_FAILURE(status)) {
        dataerrln("FAIL: NumberFormat::createInstance() returned " + UnicodeString(u_errorName(status)));
        return;
    }

    // *** Here's the key: We don't want to have to do THIS:
    // nf->setParseIntegerOnly(true);
    // or this (with changes to fr_CH per cldrbug:9370):
    // nf->setGroupingUsed(false);
    // so they are done in DateFormat::adoptNumberFormat

    // create the DateFormat
    LocalPointer<DateFormat> df(DateFormat::createDateInstance(DateFormat::kShort, loc));
    if (U_FAILURE(status)) {
        errln("FAIL: DateFormat::createInstance() returned " + UnicodeString(u_errorName(status)));
        return;
    }

    df->adoptCalendar(cal.orphan());
    df->adoptNumberFormat(nf.orphan());

    // set parsing to lenient & parse
    df->setLenient(true);
    UDate ulocdat = df->parse(udt, status);

    // format back to a string
    UnicodeString outString;
    df->format(ulocdat, outString);

    if (outString != exp) {
        errln("FAIL: " + udt + " => " + outString);
    }
}

//---------------------------------------------------------------------------
//
//   Error Checking / Reporting macros
//
//---------------------------------------------------------------------------
#define TEST_CHECK_STATUS(status) UPRV_BLOCK_MACRO_BEGIN { \
    if (U_FAILURE(status)) { \
        if (status == U_MISSING_RESOURCE_ERROR) { \
            dataerrln("File %s, Line %d: status=%s", __FILE__, __LINE__, u_errorName(status)); \
        } else { \
            errln("File %s, Line %d: status=%s", __FILE__, __LINE__, u_errorName(status)); \
        } return; \
    } \
} UPRV_BLOCK_MACRO_END

#define TEST_ASSERT(expr) UPRV_BLOCK_MACRO_BEGIN { \
    if ((expr)==false) {\
        errln("File %s, line %d: Assertion Failed: " #expr "\n", __FILE__, __LINE__);\
    } \
} UPRV_BLOCK_MACRO_END
#define TEST_ASSERT_EQUALS(x,y) UPRV_BLOCK_MACRO_BEGIN { \
      char _msg[1000]; \
      int32_t len = snprintf (_msg, sizeof(_msg), "File %s, line %d: " #x "==" #y, __FILE__, __LINE__); \
      (void)len;                                                         \
      U_ASSERT(len < (int32_t) sizeof(_msg));                            \
      assertEquals((const char*) _msg, x,y);                             \
} UPRV_BLOCK_MACRO_END


// Ticket 8199:  Parse failure for numbers in the range of 1E10 - 1E18

void NumberFormatRegressionTest::Test8199() {
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *nf = NumberFormat::createInstance(Locale::getEnglish(), status);
    if (nf == nullptr) {
        dataerrln("Fail: NumberFormat::createInstance(Locale::getEnglish(), status)");
        return;
    }
    TEST_CHECK_STATUS(status);

    // Note:  Retrieving parsed values from a Formattable as a reduced-precision type
    //        should always truncate, no other rounding scheme.

    UnicodeString numStr = "1000000000.6";   // 9 zeroes
    Formattable val;
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kDouble, val.getType());
    TEST_ASSERT_EQUALS(1000000000LL, val.getInt64(status));
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(1000000000.6, val.getDouble(status));
    TEST_CHECK_STATUS(status);

    numStr = "100000000000000001.1";   // approx 1E17, parses as a double rather
                                       //   than int64 because of the fraction
                                       //   even though int64 is more precise.
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kDouble, val.getType());
    TEST_ASSERT_EQUALS(100000000000000001LL, val.getInt64(status));
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(100000000000000000.0, val.getDouble(status));
    TEST_CHECK_STATUS(status);

    numStr = "1E17";  // Parses with the internal decimal number having non-zero exponent
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kInt64, val.getType());
    TEST_ASSERT_EQUALS(100000000000000000LL, val.getInt64());
    TEST_ASSERT_EQUALS(1.0E17, val.getDouble(status));
    TEST_CHECK_STATUS(status);

    numStr = "9223372036854775807";  // largest int64_t
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kInt64, val.getType());
    TEST_ASSERT_EQUALS(9223372036854775807LL, val.getInt64());
    // In the following check, note that a substantial range of integers will
    //    convert to the same double value.  There are also platform variations
    //    in the rounding at compile time of double constants.
    TEST_ASSERT(9223372036854775808.0 >= val.getDouble(status));
    TEST_ASSERT(9223372036854774700.0 <= val.getDouble(status));
    TEST_CHECK_STATUS(status);

    numStr = "-9223372036854775808";  // smallest int64_t
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kInt64, val.getType());
    // TEST_ASSERT_EQUALS(-9223372036854775808LL, val.getInt64()); // Compiler chokes on constant.
    TEST_ASSERT_EQUALS((int64_t)0x8000000000000000LL, val.getInt64());
    TEST_ASSERT_EQUALS(-9223372036854775808.0, val.getDouble(status));
    TEST_CHECK_STATUS(status);

    numStr = "9223372036854775808";  // largest int64_t + 1
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kDouble, val.getType());
    TEST_ASSERT_EQUALS(9223372036854775807LL, val.getInt64(status));
    TEST_ASSERT_EQUALS(status, U_INVALID_FORMAT_ERROR);
    status = U_ZERO_ERROR;
    TEST_ASSERT_EQUALS(9223372036854775810.0, val.getDouble(status));
    TEST_CHECK_STATUS(status);

    numStr = "-9223372036854775809";  // smallest int64_t - 1
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kDouble, val.getType());
    // TEST_ASSERT_EQUALS(-9223372036854775808LL, val.getInt64(status));  // spurious compiler warnings
    TEST_ASSERT_EQUALS((int64_t)0x8000000000000000LL, val.getInt64(status));
    TEST_ASSERT_EQUALS(status, U_INVALID_FORMAT_ERROR);
    status = U_ZERO_ERROR;
    TEST_ASSERT_EQUALS(-9223372036854775810.0, val.getDouble(status));
    TEST_CHECK_STATUS(status);

    // Test values near the limit of where doubles can represent all integers.
    // The implementation strategy of getInt64() changes at this boundary.
    // Strings to be parsed include a decimal fraction to force them to be
    //   parsed as doubles rather than ints.  The fraction is discarded
    //   from the parsed double value because it is beyond what can be represented.

    status = U_ZERO_ERROR;
    numStr = "9007199254740991.1";  // largest 53 bit int
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    // printf("getInt64() returns %lld\n", val.getInt64(status));
    TEST_ASSERT_EQUALS(Formattable::kDouble, val.getType());
    TEST_ASSERT_EQUALS(9007199254740991LL, val.getInt64(status));
    TEST_ASSERT_EQUALS(9007199254740991.0, val.getDouble(status));
    TEST_CHECK_STATUS(status);

    status = U_ZERO_ERROR;
    numStr = "9007199254740992.1";  // 54 bits for the int part.
    nf->parse(numStr, val, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kDouble, val.getType());
    TEST_ASSERT_EQUALS(9007199254740992LL, val.getInt64(status));
    TEST_ASSERT_EQUALS(9007199254740992.0, val.getDouble(status));
    TEST_CHECK_STATUS(status);

    status = U_ZERO_ERROR;
    numStr = "9007199254740993.1";  // 54 bits for the int part.  Double will round
    nf->parse(numStr, val, status); //    the ones digit, putting it up to ...994
    TEST_CHECK_STATUS(status);
    TEST_ASSERT_EQUALS(Formattable::kDouble, val.getType());
    TEST_ASSERT_EQUALS((int64_t)9007199254740993LL,val.getInt64(status));
    TEST_ASSERT_EQUALS((double)9007199254740994.0,(double)val.getDouble(status));
    TEST_CHECK_STATUS(status);

    delete nf;
}

void NumberFormatRegressionTest::Test9109() {
    UErrorCode status = U_ZERO_ERROR;
    Formattable val;
    ParsePosition pos;
    DecimalFormat fmt("+##", status);
    if (U_FAILURE(status)) {
        dataerrln("Failed to create DecimalFormat with pattern '+##' - %s", u_errorName(status));
        return;
    }

    fmt.setLenient(true);
    UnicodeString text("123");
    int32_t expected = 123;
    int32_t expos = 3;

    fmt.parse(text, val, pos);
    if (pos.getErrorIndex() >= 0) {
        errln(UnicodeString("Parse failure at ") + pos.getErrorIndex());
    } else if (val.getLong() != 123) {
        errln(UnicodeString("Incorrect parse result: ") + val.getLong() + " expected: " + expected);
    } else if (pos.getIndex() != 3) {
        errln(UnicodeString("Incorrect parse position: ") + pos.getIndex() + " expected: " + expos);
    }
}


void NumberFormatRegressionTest::Test9780() {
    UErrorCode status = U_ZERO_ERROR;
    NumberFormat *nf = NumberFormat::createInstance(Locale::getUS(), status);
    if (failure(status, "NumberFormat::createInstance", true)){
        delete nf;
        return;
    }
    DecimalFormat *df = dynamic_cast<DecimalFormat *>(nf);
    if(df == nullptr) {
        errln("DecimalFormat needed to continue");
        return;
    }
    df->setParseIntegerOnly(true);

    {
      Formattable n;
      ParsePosition pos(0);
      UnicodeString toParse("1,234","");
      df->parse(toParse, n, pos);
      if (n.getType() != Formattable::kLong
          || n.getLong() != 1234) {
        errln(UnicodeString("FAIL: parse(\"") + toParse + UnicodeString("\") returns ") + toString(n));
      }
    }
    // should still work in lenient mode, just won't get fastpath
    df->setLenient(true);
    {
      Formattable n;
      ParsePosition pos(0);
      UnicodeString toParse("1,234","");
      df->parse(toParse, n, pos);
      if (n.getType() != Formattable::kLong
          || n.getLong() != 1234) {
        errln(UnicodeString("FAIL: parse(\"") + toParse + UnicodeString("\") returns ") + toString(n));
      }
    }
    delete nf;
}


void NumberFormatRegressionTest::Test9677() {
  static const char16_t pattern[] = { 0x23,0x23,0x23,0x23,0x2E,0x23,0x23,0x23,0x23,0 }; // "####.####"
  static const char16_t positivePrefix[] = { 0x40,0 }; // "@"
  static const char16_t negativePrefix[] = { 0x6E,0 }; // "n"
  static const char16_t text[] = { 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0 }; // 123456789
  static const char16_t text2[] = { 0x6E, 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0 }; // n123456789
  
  UErrorCode status = U_ZERO_ERROR;
  LocalUNumberFormatPointer f(unum_open(UNUM_DEFAULT, nullptr, 0, "en_US", nullptr, &status));
  if (U_FAILURE(status)) {
      dataerrln("Failure opening unum_open");
      return;
  }

  if (U_SUCCESS(status)) {
    unum_applyPattern(f.getAlias(), false, pattern, -1, nullptr, &status);
    unum_setTextAttribute(f.getAlias(), UNUM_POSITIVE_PREFIX, positivePrefix, -1, &status);
    assertSuccess("setting attributes", status);
  }

  if(U_SUCCESS(status)) {
    int32_t n = unum_parse(f.getAlias(), text, -1, nullptr, &status);
    logln("unum_parse status %s, result %d\n", u_errorName(status), n);

    if(U_FAILURE(status)) {
        logln("Got expected parse error %s\n", u_errorName(status));
        status = U_ZERO_ERROR;
    } else {
        errln("FAIL: unum_parse status %s, result %d - expected failure\n", u_errorName(status), n);
    }
  }

  if (U_SUCCESS(status)) {
    unum_setTextAttribute(f.getAlias(), UNUM_POSITIVE_PREFIX, nullptr, 0, &status);
    assertSuccess("setting attributes", status);
    logln("removed positive prefix");
  }

  if(U_SUCCESS(status)) {
    int32_t n = unum_parse(f.getAlias(), text, -1, nullptr, &status);
    logln("unum_parse status %s, result %d\n", u_errorName(status), n);

    if(U_FAILURE(status)) {
        errln("FAIL: with pos prefix removed, parse error %s\n", u_errorName(status));
        status = U_ZERO_ERROR;
    } else {
        if(n!=123456789) {
          errln("FAIL: with pos prefix removed , unum_parse status %s, result %d expected 123456789\n", u_errorName(status), n);
        } else {
          logln("PASS: with pos prefix removed , unum_parse status %s, result %d expected 123456789\n", u_errorName(status),n);
        }
    }
  }

  if(U_SUCCESS(status)) {
    int32_t n = unum_parse(f.getAlias(), text2, -1, nullptr, &status);
    logln("unum_parse status %s, result %d\n", u_errorName(status), n);

    if(U_FAILURE(status)) {
        logln("text2: Got expected parse error %s\n", u_errorName(status));
        status = U_ZERO_ERROR;
    } else {
        errln("FAIL: text2: unum_parse status %s, result %d - expected failure\n", u_errorName(status), n);
    }
  }

  if (U_SUCCESS(status)) {
    unum_setTextAttribute(f.getAlias(), UNUM_NEGATIVE_PREFIX, negativePrefix, -1, &status);
    assertSuccess("setting attributes", status);
    logln("Set a different neg prefix prefix");
  }

  if(U_SUCCESS(status)) {
    int32_t n = unum_parse(f.getAlias(), text2, -1, nullptr, &status);
    logln("unum_parse status %s, result %d\n", u_errorName(status), n);

    if(U_FAILURE(status)) {
        errln("FAIL: with different neg prefix , parse error %s\n", u_errorName(status));
        status = U_ZERO_ERROR;
    } else {
        if(n!=-123456789) {
          errln("FAIL: with different neg prefix , unum_parse status %s, result %d expected -123456789\n", u_errorName(status), n);
        } else {
          logln("PASS: with different neg prefix , unum_parse status %s, result %d expected -123456789\n", u_errorName(status), n);
        }
    }
  }
}

void NumberFormatRegressionTest::Test10361() {
    // DecimalFormat/NumberFormat were artificially limiting the number of digits,
    //    preventing formatting of big decimals.
    UErrorCode status = U_ZERO_ERROR;
    DecimalFormatSymbols symbols(Locale::getEnglish(), status);
    LocalPointer<DecimalFormat> df(new DecimalFormat("###.##", symbols, status), status);
    TEST_CHECK_STATUS(status);

    // Create a decimal number with a million digits.
    const int32_t NUMSIZE=1000000;
    char *num = new char[NUMSIZE];
    for (int32_t i=0; i<NUMSIZE; i++) {
        num[i] = '0' + (i+1) % 10;
    }
    num[NUMSIZE-3] = '.';
    num[NUMSIZE-1] = 0;

    UnicodeString    s;
    Formattable      fmtable;
    fmtable.setDecimalNumber(num, status);
    TEST_CHECK_STATUS(status);

    FieldPosition pos(UNUM_DECIMAL_SEPARATOR_FIELD);
    df->format(fmtable, s, pos, status);
    TEST_CHECK_STATUS(status);
    TEST_ASSERT(999999 == s.length());
    TEST_ASSERT(999997 == pos.getBeginIndex());
    TEST_ASSERT(999998 == pos.getEndIndex());

    UnicodeString expected(num, -1, US_INV);
    TEST_ASSERT(expected == s);
    delete [] num;
}

#endif /* #if !UCONFIG_NO_FORMATTING */

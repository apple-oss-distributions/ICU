// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/********************************************************************
 * Copyright (c) 2016, International Business Machines Corporation
 * and others. All Rights Reserved.
 ********************************************************************/
/* C API TEST FOR DATE INTERVAL FORMAT */

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING && !UCONFIG_NO_BREAK_ITERATION

#include <stdbool.h>

#include "unicode/ureldatefmt.h"
#include "unicode/unum.h"
#include "unicode/udisplaycontext.h"
#include "unicode/ustring.h"
#include "cintltst.h"
#include "cmemory.h"
#include "cformtst.h"

static void TestRelDateFmt(void);
static void TestNumericField(void);
static void TestCombineDateTime(void);
static void TestFields(void);

void addRelativeDateFormatTest(TestNode** root);

#define TESTCASE(x) addTest(root, &x, "tsformat/crelativedateformattest/" #x)

void addRelativeDateFormatTest(TestNode** root)
{
    TESTCASE(TestRelDateFmt);
    TESTCASE(TestNumericField);
    TESTCASE(TestCombineDateTime);
    TESTCASE(TestFields);
}

static const double offsets[] = { -5.0, -2.2, -2.0, -1.0, -0.7, -0.0, 0.0, 0.7, 1.0, 2.0, 5.0 };
enum { kNumOffsets = UPRV_LENGTHOF(offsets) };

typedef struct {
    int32_t field;
    int32_t beginPos;
    int32_t endPos;
} FieldsDat;

static const char* en_decDef_long_midSent_sec[kNumOffsets*2] = {
/*  text                    numeric */
    "5 seconds ago",        "5 seconds ago",      /* -5   */
    "2.2 seconds ago",      "2.2 seconds ago",    /* -2.2 */
    "2 seconds ago",        "2 seconds ago",      /* -2   */
    "1 second ago",         "1 second ago",       /* -1   */
    "0.7 seconds ago",      "0.7 seconds ago",    /* -0.7 */
    "now",                  "0 seconds ago",      /*  -0  */
    "now",                  "in 0 seconds",       /*  0   */
    "in 0.7 seconds",       "in 0.7 seconds",     /*  0.7 */
    "in 1 second",          "in 1 second",        /*  1   */
    "in 2 seconds",         "in 2 seconds",       /*  2   */
    "in 5 seconds",         "in 5 seconds"        /*  5   */
};

static const FieldsDat en_attrDef_long_midSent_sec[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "5 seconds ago",        "5 seconds ago",       -5   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "2.2 seconds ago",      "2.2 seconds ago",     -2.2 */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 seconds ago",        "2 seconds ago",       -2   */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "1 second ago",         "1 second ago",        -1   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "0.7 seconds ago",      "0.7 seconds ago",     -0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "now",                  "0 seconds ago",        -0  */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "now",                  "in 0 seconds",         0   */
    {UDAT_REL_NUMERIC_FIELD,  3,  6}, {UDAT_REL_NUMERIC_FIELD,  3,  6}, /* "in 0.7 seconds",       "in 0.7 seconds",       0.7 */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 1 second",          "in 1 second",          1   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 2 seconds",         "in 2 seconds",         2   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 5 seconds",         "in 5 seconds"          5   */
};

static const char* en_decDef_long_midSent_week[kNumOffsets*2] = {
/*  text                    numeric */
    "5 weeks ago",          "5 weeks ago",        /* -5   */
    "2.2 weeks ago",        "2.2 weeks ago",      /* -2.2 */
    "2 weeks ago",          "2 weeks ago",        /* -2   */
    "last week",            "1 week ago",         /* -1   */
    "0.7 weeks ago",        "0.7 weeks ago",      /* -0.7 */
    "this week",            "0 weeks ago",        /*  -0  */
    "this week",            "in 0 weeks",         /*  0   */
    "in 0.7 weeks",         "in 0.7 weeks",       /*  0.7 */
    "next week",            "in 1 week",          /*  1   */
    "in 2 weeks",           "in 2 weeks",         /*  2   */
    "in 5 weeks",           "in 5 weeks"          /*  5   */
};

static const FieldsDat en_attrDef_long_midSent_week[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "5 weeks ago",          "5 weeks ago",         -5   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "2.2 weeks ago",        "2.2 weeks ago",       -2.2 */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 weeks ago",          "2 weeks ago",         -2   */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "last week",            "1 week ago",          -1   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "0.7 weeks ago",        "0.7 weeks ago",       -0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "this week",            "0 weeks ago",          -0  */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "this week",            "in 0 weeks",           0   */
    {UDAT_REL_NUMERIC_FIELD,  3,  6}, {UDAT_REL_NUMERIC_FIELD,  3,  6}, /* "in 0.7 weeks",         "in 0.7 weeks",         0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "next week",            "in 1 week",            1   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 2 weeks",           "in 2 weeks",           2   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 5 weeks",           "in 5 weeks"            5   */
};

static const char* en_dec0_long_midSent_week[kNumOffsets*2] = {
/*  text                    numeric */
    "5 weeks ago",          "5 weeks ago",        /* -5   */
    "2 weeks ago",          "2 weeks ago",        /* -2.2 */
    "2 weeks ago",          "2 weeks ago",        /* -2  */
    "last week",            "1 week ago",         /* -1   */
    "0 weeks ago",          "0 weeks ago",        /* -0.7 */
    "this week",            "0 weeks ago",        /* -0  */
    "this week",            "in 0 weeks",         /*  0   */
    "in 0 weeks",           "in 0 weeks",         /*  0.7 */
    "next week",            "in 1 week",          /*  1   */
    "in 2 weeks",           "in 2 weeks",         /*  2   */
    "in 5 weeks",           "in 5 weeks"          /*  5   */
};

static const FieldsDat en_attr0_long_midSent_week[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "5 weeks ago",          "5 weeks ago",         -5   */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 weeks ago",          "2 weeks ago",         -2.2 */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 weeks ago",          "2 weeks ago",         -2  */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "last week",            "1 week ago",          -1   */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "0 weeks ago",          "0 weeks ago",         -0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "this week",            "0 weeks ago",         -0  */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "this week",            "in 0 weeks",           0   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 0 weeks",           "in 0 weeks",           0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "next week",            "in 1 week",            1   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 2 weeks",           "in 2 weeks",           2   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 5 weeks",           "in 5 weeks"            5   */
};

static const char* en_decDef_short_midSent_week[kNumOffsets*2] = {
/*  text                    numeric */
    "5 wk. ago",            "5 wk. ago",          /* -5   */
    "2.2 wk. ago",          "2.2 wk. ago",        /* -2.2 */
    "2 wk. ago",            "2 wk. ago",          /* -2   */
    "last wk.",             "1 wk. ago",          /* -1   */
    "0.7 wk. ago",          "0.7 wk. ago",        /* -0.7 */
    "this wk.",             "0 wk. ago",          /* -0   */
    "this wk.",             "in 0 wk.",           /*  0   */
    "in 0.7 wk.",           "in 0.7 wk.",         /*  0.7 */
    "next wk.",             "in 1 wk.",           /*  1   */
    "in 2 wk.",             "in 2 wk.",           /*  2   */
    "in 5 wk.",             "in 5 wk."            /*  5   */
};

static const FieldsDat en_attrDef_short_midSent_week[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "5 wk. ago",            "5 wk. ago",           -5   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "2.2 wk. ago",          "2.2 wk. ago",         -2.2 */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 wk. ago",            "2 wk. ago",           -2   */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "last wk.",             "1 wk. ago",           -1   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "0.7 wk. ago",          "0.7 wk. ago",         -0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "this wk.",             "0 wk. ago",           -0   */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "this wk.",             "in 0 wk.",             0   */
    {UDAT_REL_NUMERIC_FIELD,  3,  6}, {UDAT_REL_NUMERIC_FIELD,  3,  6}, /* "in 0.7 wk.",           "in 0.7 wk.",           0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "next wk.",             "in 1 wk.",             1   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 2 wk.",             "in 2 wk.",             2   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 5 wk.",             "in 5 wk."              5   */
};

static const char* en_decDef_long_midSent_min[kNumOffsets*2] = {
/*  text                    numeric */
    "5 minutes ago",        "5 minutes ago",      /* -5   */
    "2.2 minutes ago",      "2.2 minutes ago",    /* -2.2 */
    "2 minutes ago",        "2 minutes ago",      /* -2   */
    "1 minute ago",         "1 minute ago",       /* -1   */
    "0.7 minutes ago",      "0.7 minutes ago",    /* -0.7 */
    "this minute",          "0 minutes ago",      /* -0   */
    "this minute",          "in 0 minutes",       /*  0   */
    "in 0.7 minutes",       "in 0.7 minutes",     /*  0.7 */
    "in 1 minute",          "in 1 minute",        /*  1   */
    "in 2 minutes",         "in 2 minutes",       /*  2   */
    "in 5 minutes",         "in 5 minutes"        /*  5   */
};

static const FieldsDat en_attrDef_long_midSent_min[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "5 minutes ago",        "5 minutes ago",       -5   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "2.2 minutes ago",      "2.2 minutes ago",     -2.2 */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 minutes ago",        "2 minutes ago",       -2   */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "1 minute ago",         "1 minute ago",        -1   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "0.7 minutes ago",      "0.7 minutes ago",     -0.7 */
    {-1,  -1,  -1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "this minute",        "0 minutes ago",       -0   */
    {-1,  -1,  -1}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "this minute",         "in 0 minutes",         0   */
    {UDAT_REL_NUMERIC_FIELD,  3,  6}, {UDAT_REL_NUMERIC_FIELD,  3,  6}, /* "in 0.7 minutes",       "in 0.7 minutes",       0.7 */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 1 minute",          "in 1 minute",          1   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 2 minutes",         "in 2 minutes",         2   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 5 minutes",         "in 5 minutes"          5   */
};

static const char* en_dec0_long_midSent_tues[kNumOffsets*2] = {
/*  text                    numeric */
    "5 Tuesdays ago",       "5 Tuesdays ago",     /* -5   */
    "2.2 Tuesdays ago",     "2.2 Tuesdays ago",   /* -2.2 */
    "2 Tuesdays ago",       "2 Tuesdays ago",     /* -2   */
    "last Tuesday",         "1 Tuesday ago",      /* -1   */
    "0.7 Tuesdays ago",     "0.7 Tuesdays ago",   /* -0.7 */
    "this Tuesday",         "0 Tuesdays ago",     /* -0   */
    "this Tuesday",         "in 0 Tuesdays",      /*  0   */
    "in 0.7 Tuesdays",      "in 0.7 Tuesdays",    /*  0.7 */
    "next Tuesday",         "in 1 Tuesday",       /*  1   */
    "in 2 Tuesdays",        "in 2 Tuesdays",      /*  2   */
    "in 5 Tuesdays",        "in 5 Tuesdays",      /*  5   */
};

static const FieldsDat en_attr0_long_midSent_tues[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "5 Tuesdays ago",       "5 Tuesdays ago",      -5   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "2.2 Tuesdays ago",     "2.2 Tuesdays ago",    -2.2 */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 Tuesdays ago",       "2 Tuesdays ago",      -2   */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "last Tuesday",         "1 Tuesday ago",       -1   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "0.7 Tuesdays ago",     "0.7 Tuesdays ago",    -0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "this Tuesday",         "0 Tuesdays ago",      -0   */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "this Tuesday",         "in 0 Tuesdays",        0   */
    {UDAT_REL_NUMERIC_FIELD,  3,  6}, {UDAT_REL_NUMERIC_FIELD,  3,  6}, /* "in 0.7 Tuesdays",      "in 0.7 Tuesdays",      0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "next Tuesday",         "in 1 Tuesday",         1   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 2 Tuesdays",        "in 2 Tuesdays",        2   */
    {UDAT_REL_NUMERIC_FIELD,  3,  4}, {UDAT_REL_NUMERIC_FIELD,  3,  4}, /* "in 5 Tuesdays",        "in 5 Tuesdays",        5   */
};

static const char* fr_decDef_long_midSent_day[kNumOffsets*2] = {
/*  text                    numeric */
    "il y a 5 jours",       "il y a 5 jours",     /* -5   */
    "il y a 2,2 jours",     "il y a 2,2 jours",   /* -2.2 */
    "avant-hier",           "il y a 2 jours",     /* -2   */
    "hier",                 "il y a 1 jour",      /* -1   */
    "il y a 0,7 jour",      "il y a 0,7 jour",    /* -0.7 */
    "aujourd\\u2019hui",    "il y a 0 jour",      /* -0   */
    "aujourd\\u2019hui",    "dans 0 jour",        /*  0   */
    "dans 0,7 jour",        "dans 0,7 jour",      /*  0.7 */
    "demain",               "dans 1 jour",        /*  1   */
    "apr\\u00E8s-demain",   "dans 2 jours",       /*  2   */
    "dans 5 jours",         "dans 5 jours"        /*  5   */
};

static const FieldsDat fr_attrDef_long_midSent_day[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  7,  8}, {UDAT_REL_NUMERIC_FIELD,  7,  8}, /* "il y a 5 jours",       "il y a 5 jours",      -5   */
    {UDAT_REL_NUMERIC_FIELD,  7, 10}, {UDAT_REL_NUMERIC_FIELD,  7, 10}, /* "il y a 2,2 jours",     "il y a 2,2 jours",    -2.2 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  7,  8}, /* "avant-hier",           "il y a 2 jours",      -2   */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  7,  8}, /* "hier",                 "il y a 1 jour",       -1   */
    {UDAT_REL_NUMERIC_FIELD,  7, 10}, {UDAT_REL_NUMERIC_FIELD,  7, 10}, /* "il y a 0,7 jour",      "il y a 0,7 jour",     -0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  7,  8}, /* "aujourd\\u2019hui",    "il y a 0 jour",       -0   */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  5,  6}, /* "aujourd\\u2019hui",    "dans 0 jour",          0   */
    {UDAT_REL_NUMERIC_FIELD,  5,  8}, {UDAT_REL_NUMERIC_FIELD,  5,  8}, /* "dans 0,7 jour",        "dans 0,7 jour",        0.7 */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  5,  6}, /* "demain",               "dans 1 jour",          1   */
    { -1, -1, -1},                    {UDAT_REL_NUMERIC_FIELD,  5,  6}, /* "apr\\u00E8s-demain",   "dans 2 jours",         2   */
    {UDAT_REL_NUMERIC_FIELD,  5,  6}, {UDAT_REL_NUMERIC_FIELD,  5,  6}, /* "dans 5 jours",         "dans 5 jours"          5   */
};

static const char* ak_decDef_long_stdAlon_sec[kNumOffsets*2] = { // falls back to root
/*  text                    numeric */
    "nnibuo 5 a atwam",     "nnibuo 5 a atwam",               /* -5   */
    "nnibuo 2.2 a atwam",   "nnibuo 2.2 a atwam",             /* -2.2 */
    "nnibuo 2 a atwam",     "nnibuo 2 a atwam",               /* -2   */
    "anibuo 1 a atwam",     "anibuo 1 a atwam",               /* -1   */
    "nnibuo 0.7 a atwam",   "nnibuo 0.7 a atwam",             /* -0.7 */
    "seesei",               "anibuo 0 a atwam",               /*  -0  */
    "seesei",               "anibuo 0 mu",               /*  0   */
    "nnibuo 0.7 mu",        "nnibuo 0.7 mu",             /*  0.7 */
    "anibuo 1 mu",          "anibuo 1 mu",               /*  1   */
    "nnibuo 2 mu",          "nnibuo 2 mu",               /*  2   */
    "nnibuo 5 mu",          "nnibuo 5 mu",               /*  5   */
};

static const FieldsDat ak_attrDef_long_stdAlon_sec[kNumOffsets*2] = {
    {UDAT_REL_NUMERIC_FIELD, 7, 8}, {UDAT_REL_NUMERIC_FIELD, 7, 8},
    {UDAT_REL_NUMERIC_FIELD, 7, 10}, {UDAT_REL_NUMERIC_FIELD, 7, 10},
    {UDAT_REL_NUMERIC_FIELD, 7, 8}, {UDAT_REL_NUMERIC_FIELD, 7, 8},
    {UDAT_REL_NUMERIC_FIELD, 7, 8}, {UDAT_REL_NUMERIC_FIELD, 7, 8},
    {UDAT_REL_NUMERIC_FIELD, 7, 10}, {UDAT_REL_NUMERIC_FIELD, 7, 10},
    { -1, -1, -1},                  {UDAT_REL_NUMERIC_FIELD, 7, 8},
    { -1, -1, -1},                  {UDAT_REL_NUMERIC_FIELD, 7, 8},
    {UDAT_REL_NUMERIC_FIELD, 7, 10}, {UDAT_REL_NUMERIC_FIELD, 7, 10},
    {UDAT_REL_NUMERIC_FIELD, 7, 8}, {UDAT_REL_NUMERIC_FIELD, 7, 8},
    {UDAT_REL_NUMERIC_FIELD, 7, 8}, {UDAT_REL_NUMERIC_FIELD, 7, 8},
    {UDAT_REL_NUMERIC_FIELD, 7, 8}, {UDAT_REL_NUMERIC_FIELD, 7, 8},
};

static const char* enIN_decDef_short_midSent_weds[kNumOffsets*2] = {
/*  text                    numeric */
    "5 Wed ago",            "5 Wed ago",          /* -5   */
    "2.2 Wed ago",          "2.2 Wed ago",        /* -2.2 */
    "2 Wed ago",            "2 Wed ago",          /* -2   */
    "last Wed",             "1 Wed ago",          /* -1   */
    "0.7 Wed ago",          "0.7 Wed ago",        /* -0.7 */
    "this Wed",             "0 Wed ago",          /*  -0  */
    "this Wed",             "in 0 Wed",           /*  0   */
    "in 0.7 Wed",           "in 0.7 Wed",         /*  0.7 */
    "next Wed",             "in 1 Wed",           /*  1   */ // in 1 Wed. missing in logical group
    "in 2 Wed",             "in 2 Wed",           /*  2   */
    "in 5 Wed",             "in 5 Wed"            /*  5   */
};

static const FieldsDat enIN_attrDef_short_midSent_weds[kNumOffsets*2] = {
    {UDAT_REL_NUMERIC_FIELD, 0, 1}, {UDAT_REL_NUMERIC_FIELD, 0, 1},
    {UDAT_REL_NUMERIC_FIELD, 0, 3}, {UDAT_REL_NUMERIC_FIELD, 0, 3},
    {UDAT_REL_NUMERIC_FIELD, 0, 1}, {UDAT_REL_NUMERIC_FIELD, 0, 1},
    { -1, -1, -1},                  {UDAT_REL_NUMERIC_FIELD, 0, 1},
    {UDAT_REL_NUMERIC_FIELD, 0, 3}, {UDAT_REL_NUMERIC_FIELD, 0, 3},
    { -1, -1, -1},                  {UDAT_REL_NUMERIC_FIELD, 0, 1},
    { -1, -1, -1},                  {UDAT_REL_NUMERIC_FIELD, 3, 4},
    {UDAT_REL_NUMERIC_FIELD, 3, 6}, {UDAT_REL_NUMERIC_FIELD, 3, 6},
    { -1, -1, -1},                  {UDAT_REL_NUMERIC_FIELD, 3, 4},
    {UDAT_REL_NUMERIC_FIELD, 3, 4}, {UDAT_REL_NUMERIC_FIELD, 3, 4},
    {UDAT_REL_NUMERIC_FIELD, 3, 4}, {UDAT_REL_NUMERIC_FIELD, 3, 4},
};

#if APPLE_ICU_CHANGES
// rdar://
static const char* nb_decDef_long_midSent_day[kNumOffsets*2] = {
/*  text                    numeric */
    "for 5 dager siden",    "for 5 dager siden",   /* -5   */
    "for 2,2 dager siden",  "for 2,2 dager siden", /* -2.2 */
    "i forg\\u00E5rs",      "for 2 dager siden",   /* -2   */
    "i g\\u00E5r",          "for 1 dag siden",     /* -1   */
    "for 0,7 dager siden",  "for 0,7 dager siden", /* -0.7 */
    "i dag",                "for 0 dager siden",   /* -0   */
    "i dag",                "om 0 dager",          /*  0   */
    "om 0,7 dager",         "om 0,7 dager",        /*  0.7 */
    "i morgen",             "om 1 dag",            /*  1   */
    "i overmorgen",         "om 2 dager",          /*  2   */
    "om 5 dager",           "om 5 dager"           /*  5   */
};

// Need attrDef for nb

// Apple addition (rdar://80742319)
static const char* hu_decDef_narrow_midSent_min[kNumOffsets*2] = {
/*  text                    numeric */
    "5 perce",              "5 perce",   /* -5   */
    "2,2 perce",            "2,2 perce", /* -2.2 */
    "2 perce",              "2 perce",   /* -2   */
    "1 perce",              "1 perce",   /* -1   */
    "0,7 perce",            "0,7 perce", /* -0.7 */
    "ebben a percben",      "0 perce",   /* -0   */
    "ebben a percben",      "0 perc m\\u00falva",        /*  0   */
    "0,7 perc m\\u00falva", "0,7 perc m\\u00falva",      /*  0.7 */
    "1 perc m\\u00falva",   "1 perc m\\u00falva",        /*  1   */
    "2 perc m\\u00falva",   "2 perc m\\u00falva",        /*  2   */
    "5 perc m\\u00falva",   "5 perc m\\u00falva"         /*  5   */
};

// Apple addition (rdar://80742319)
static const FieldsDat hu_attrDef_narrow_midSent_min[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "5 perce",          "5 perce",       -5   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "2,2 perce",        "2,2 perce",     -2.2 */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 perce",          "2 perce",       -2   */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "1 perce",          "1 perce",        -1   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "0,7 perce",        "0,7 perce",     -0.7 */
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "ebben a percben",  "0 perccel ezelőtt",       -0   */
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "ebben a percben",  "0 perc múlva",             0   */
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3}, /* "0,7 perc múlva",   "0,7 perc múlva",           0.7 */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "1 perc múlva",     "1 perc múlva",             1   */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "2 perc múlva",     "2 perc múlva",             2   */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1}, /* "5 perc múlva",     "5 perc múlva"              5   */
};

// BEGIN Apple additions for rdar://135027393
static const char* ta_decDef_long_midSent_hr[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2.2 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2.2   */
    "2 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd",              "1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd",   /* -1   */
    "0.7 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "0.7 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0.7   */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",   /* 0   */
    "0.7 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd", "0.7 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      /*  0.7 */
    "1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd", "1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      /*  1 */
    "2 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd", "2 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      /*  2 */
    "5 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd", "5 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      /*  5 */
};
static const FieldsDat ta_attrDef_long_midSent_hr[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
static const char* ta_decDef_long_midSent_min[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "2.2 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -2.2 */
    "2 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0bae\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -1   */
    "0.7 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "0.7 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -0.7 */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",        /*  0   */
    "0.7 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd", "0.7 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",      /*  0.7 */
    "1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",   "1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",        /*  1   */
    "2 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",   "2 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",        /*  2   */
    "5 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",   "5 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0b99\\u0bcd\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd"         /*  5   */
};
static const FieldsDat ta_attrDef_long_midSent_min[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
static const char* ta_decDef_long_midSent_sec[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "2.2 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -2.2 */
    "2 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "1 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -1   */
    "0.7 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "0.7 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -0.7 */
    "\\u0b87\\u0baa\\u0bcd\\u0baa\\u0bcb\\u0ba4\\u0bc1",      "0 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bcd \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0baa\\u0bcd\\u0baa\\u0bcb\\u0ba4\\u0bc1",      "0 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",        /*  0   */
    "0.7 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd", "0.7 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",      /*  0.7 */
    "1 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0baf\\u0bbf\\u0bb2\\u0bcd",   "1 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0baf\\u0bbf\\u0bb2\\u0bcd",        /*  1   */
    "2 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",   "2 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",        /*  2   */
    "5 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd",   "5 \\u0bb5\\u0bbf\\u0ba8\\u0bbe\\u0b9f\\u0bbf\\u0b95\\u0bb3\\u0bbf\\u0bb2\\u0bcd"         /*  5   */
};
static const FieldsDat ta_attrDef_long_midSent_sec[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
static const char* ta_decDef_short_midSent_hr[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "2.2 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -2.2 */
    "2 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "1 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -1   */
    "0.7 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "0.7 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -0.7 */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0bae.",        /*  0   */
    "0.7 \\u0bae.", "0.7 \\u0bae.",      /*  0.7 */
    "1 \\u0bae.",   "1 \\u0bae.",        /*  1   */
    "2 \\u0bae.",   "2 \\u0bae.",        /*  2   */
    "5 \\u0bae.",   "5 \\u0bae."         /*  5   */
};
static const FieldsDat ta_attrDef_short_midSent_hr[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
static const char* ta_decDef_short_midSent_min[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "2.2 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -2.2 */
    "2 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "1 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -1   */
    "0.7 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "0.7 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -0.7 */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0ba8\\u0bbf\\u0bae\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0ba8\\u0bbf\\u0bae\\u0bbf.",        /*  0   */
    "0.7 \\u0ba8\\u0bbf\\u0bae\\u0bbf.", "0.7 \\u0ba8\\u0bbf\\u0bae\\u0bbf.",      /*  0.7 */
    "1 \\u0ba8\\u0bbf\\u0bae\\u0bbf.",   "1 \\u0ba8\\u0bbf\\u0bae\\u0bbf.",        /*  1   */
    "2 \\u0ba8\\u0bbf\\u0bae\\u0bbf.",   "2 \\u0ba8\\u0bbf\\u0bae\\u0bbf.",        /*  2   */
    "5 \\u0ba8\\u0bbf\\u0bae\\u0bbf.",   "5 \\u0ba8\\u0bbf\\u0bae\\u0bbf."         /*  5   */
};
static const FieldsDat ta_attrDef_short_midSent_min[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
static const char* ta_decDef_short_midSent_sec[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "2.2 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -2.2 */
    "2 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "1 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -1   */
    "0.7 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "0.7 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -0.7 */
    "\\u0b87\\u0baa\\u0bcd\\u0baa\\u0bcb\\u0ba4\\u0bc1",      "0 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0baa\\u0bcd\\u0baa\\u0bcb\\u0ba4\\u0bc1",      "0 \\u0bb5\\u0bbf.",        /*  0   */
    "0.7 \\u0bb5\\u0bbf.", "0.7 \\u0bb5\\u0bbf.",      /*  0.7 */
    "1 \\u0bb5\\u0bbf.",   "1 \\u0bb5\\u0bbf.",        /*  1   */
    "2 \\u0bb5\\u0bbf.",   "2 \\u0bb5\\u0bbf.",        /*  2   */
    "5 \\u0bb5\\u0bbf.",   "5 \\u0bb5\\u0bbf."         /*  5   */
};
static const FieldsDat ta_attrDef_short_midSent_sec[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
static const char* ta_decDef_narrow_midSent_hr[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "2.2 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -2.2 */
    "2 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "1 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -1   */
    "0.7 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "0.7 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -0.7 */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0bae. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0bae\\u0ba3\\u0bbf\\u0ba8\\u0bc7\\u0bb0\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0bae.",        /*  0   */
    "0.7 \\u0bae.", "0.7 \\u0bae.",      /*  0.7 */
    "1 \\u0bae.",   "1 \\u0bae.",        /*  1   */
    "2 \\u0bae.",   "2 \\u0bae.",        /*  2   */
    "5 \\u0bae.",   "5 \\u0bae."         /*  5   */
};
static const FieldsDat ta_attrDef_narrow_midSent_hr[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
static const char* ta_decDef_narrow_midSent_min[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "2.2 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -2.2 */
    "2 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "1 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -1   */
    "0.7 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "0.7 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -0.7 */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0ba8\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0ba8\\u0bcd\\u0ba4 \\u0b92\\u0bb0\\u0bc1 \\u0ba8\\u0bbf\\u0bae\\u0bbf\\u0b9f\\u0ba4\\u0bcd\\u0ba4\\u0bbf\\u0bb2\\u0bcd",      "0 \\u0ba8\\u0bbf.",        /*  0   */
    "0.7 \\u0ba8\\u0bbf.", "0.7 \\u0ba8\\u0bbf.",      /*  0.7 */
    "1 \\u0ba8\\u0bbf.",   "1 \\u0ba8\\u0bbf.",        /*  1   */
    "2 \\u0ba8\\u0bbf.",   "2 \\u0ba8\\u0bbf.",        /*  2   */
    "5 \\u0ba8\\u0bbf.",   "5 \\u0ba8\\u0bbf."         /*  5   */
};
static const FieldsDat ta_attrDef_narrow_midSent_min[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
static const char* ta_decDef_narrow_midSent_sec[kNumOffsets*2] = {
/*  text                    numeric */
    "5 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "5 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -5   */
    "2.2 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "2.2 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -2.2 */
    "2 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "2 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -2   */
    "1 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",              "1 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -1   */
    "0.7 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",            "0.7 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd", /* -0.7 */
    "\\u0b87\\u0baa\\u0bcd\\u0baa\\u0bcb\\u0ba4\\u0bc1",      "0 \\u0bb5\\u0bbf. \\u0bae\\u0bc1\\u0ba9\\u0bcd",   /* -0   */
    "\\u0b87\\u0baa\\u0bcd\\u0baa\\u0bcb\\u0ba4\\u0bc1",      "0 \\u0bb5\\u0bbf.",        /*  0   */
    "0.7 \\u0bb5\\u0bbf.", "0.7 \\u0bb5\\u0bbf.",      /*  0.7 */
    "1 \\u0bb5\\u0bbf.",   "1 \\u0bb5\\u0bbf.",        /*  1   */
    "2 \\u0bb5\\u0bbf.",   "2 \\u0bb5\\u0bbf.",        /*  2   */
    "5 \\u0bb5\\u0bbf.",   "5 \\u0bb5\\u0bbf."         /*  5   */
};
static const FieldsDat ta_attrDef_narrow_midSent_sec[kNumOffsets*2] = {
/*  text           numeric           text                    numeric */
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {-1,  -1,  -1},                   {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  3}, {UDAT_REL_NUMERIC_FIELD,  0,  3},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
    {UDAT_REL_NUMERIC_FIELD,  0,  1}, {UDAT_REL_NUMERIC_FIELD,  0,  1},
};
// END Apple additions for rdar://135027393
#endif  // APPLE_ICU_CHANGES

typedef struct {
    const char*                         locale;
    int32_t                             decPlaces; /* fixed decimal places; -1 to use default num formatter */
    UDateRelativeDateTimeFormatterStyle width;
    UDisplayContext                     capContext;
    URelativeDateTimeUnit               unit;
    const char **                       expectedResults; /* for the various offsets */
    const FieldsDat*                    expectedAttributes;
} RelDateTimeFormatTestItem;

static const RelDateTimeFormatTestItem fmtTestItems[] = {
    { "en", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_SECOND,
      en_decDef_long_midSent_sec,   en_attrDef_long_midSent_sec },
    { "en", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_WEEK,
      en_decDef_long_midSent_week,  en_attrDef_long_midSent_week},
    { "en",  0, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_WEEK,
      en_dec0_long_midSent_week,    en_attr0_long_midSent_week},
    { "en", -1, UDAT_STYLE_SHORT, UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_WEEK,
      en_decDef_short_midSent_week, en_attrDef_short_midSent_week},
    { "en", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_MINUTE,
      en_decDef_long_midSent_min,   en_attrDef_long_midSent_min},
    { "en", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_TUESDAY,
      en_dec0_long_midSent_tues,    en_attr0_long_midSent_tues},
    { "fr", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_DAY,
      fr_decDef_long_midSent_day,   fr_attrDef_long_midSent_day},
    { "ak", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_STANDALONE,         UDAT_REL_UNIT_SECOND,
      ak_decDef_long_stdAlon_sec,   ak_attrDef_long_stdAlon_sec},
    { "en_IN", -1, UDAT_STYLE_SHORT, UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_WEDNESDAY,
      enIN_decDef_short_midSent_weds, enIN_attrDef_short_midSent_weds},
    { "en@calendar=iso8601", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_SECOND,
      en_decDef_long_midSent_sec,   en_attrDef_long_midSent_sec },
#if APPLE_ICU_CHANGES
// rdar://
    //add suitable entry for nb
    //{ "nb", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_DAY,     nb_decDef_long_midSent_day ...
    // Apple addition (rdar://80742319)
    { "hu", -1, UDAT_STYLE_NARROW,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_MINUTE,
      hu_decDef_narrow_midSent_min,   hu_attrDef_narrow_midSent_min},
    // lots of tests for rdar://135027393
    { "ta", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_HOUR,
      ta_decDef_long_midSent_hr,    ta_attrDef_long_midSent_hr },
    { "ta", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_MINUTE,
      ta_decDef_long_midSent_min,   ta_attrDef_long_midSent_min },
    { "ta", -1, UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_SECOND,
      ta_decDef_long_midSent_sec,   ta_attrDef_long_midSent_sec },
    { "ta", -1, UDAT_STYLE_SHORT,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_HOUR,
      ta_decDef_short_midSent_hr,   ta_attrDef_short_midSent_hr },
    { "ta", -1, UDAT_STYLE_SHORT,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_MINUTE,
      ta_decDef_short_midSent_min,   ta_attrDef_short_midSent_min },
    { "ta", -1, UDAT_STYLE_SHORT,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_SECOND,
      ta_decDef_short_midSent_sec,   ta_attrDef_short_midSent_sec },
    { "ta", -1, UDAT_STYLE_NARROW,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_HOUR,
      ta_decDef_narrow_midSent_hr,   ta_attrDef_narrow_midSent_hr },
    { "ta", -1, UDAT_STYLE_NARROW,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_MINUTE,
      ta_decDef_narrow_midSent_min,   ta_attrDef_narrow_midSent_min },
    { "ta", -1, UDAT_STYLE_NARROW,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, UDAT_REL_UNIT_SECOND,
      ta_decDef_narrow_midSent_sec,   ta_attrDef_narrow_midSent_sec },
#endif  // APPLE_ICU_CHANGES
    { NULL,  0, (UDateRelativeDateTimeFormatterStyle)0, (UDisplayContext)0, (URelativeDateTimeUnit)0, NULL, NULL } /* terminator */
};

enum { kUBufMax = 64, kBBufMax = 256 };

static void TestRelDateFmt(void)
{
    const RelDateTimeFormatTestItem *itemPtr;
    log_verbose("\nTesting ureldatefmt_open(), ureldatefmt_format(), ureldatefmt_formatNumeric() with various parameters\n");
    for (itemPtr = fmtTestItems; itemPtr->locale != NULL; itemPtr++) {
        URelativeDateTimeFormatter *reldatefmt = NULL;
        UNumberFormat* nfToAdopt = NULL;
        UErrorCode status = U_ZERO_ERROR;
        int32_t iOffset;

        if (itemPtr->decPlaces >= 0) {
            nfToAdopt = unum_open(UNUM_DECIMAL, NULL, 0, itemPtr->locale, NULL, &status);
            if ( U_FAILURE(status) ) {
                log_data_err("FAIL: unum_open(UNUM_DECIMAL, ...) for locale %s: %s\n", itemPtr->locale, myErrorName(status));
                continue;
            }
		    unum_setAttribute(nfToAdopt, UNUM_MIN_FRACTION_DIGITS, itemPtr->decPlaces);
		    unum_setAttribute(nfToAdopt, UNUM_MAX_FRACTION_DIGITS, itemPtr->decPlaces);
		    unum_setAttribute(nfToAdopt, UNUM_ROUNDING_MODE, UNUM_ROUND_DOWN);
        }
        reldatefmt = ureldatefmt_open(itemPtr->locale, nfToAdopt, itemPtr->width, itemPtr->capContext, &status);
        if ( U_FAILURE(status) ) {
            log_data_err("FAIL: ureldatefmt_open() for locale %s, decPlaces %d, width %d, capContext %d: %s\n",
                    itemPtr->locale, itemPtr->decPlaces, (int)itemPtr->width, (int)itemPtr->capContext,
                    myErrorName(status) );
            continue;
        }

        for (iOffset = 0; iOffset < kNumOffsets; iOffset++) {
            UChar ubufget[kUBufMax];
            int32_t ulenget;

            status = U_ZERO_ERROR;
            ulenget = ureldatefmt_format(reldatefmt, offsets[iOffset], itemPtr->unit, ubufget, kUBufMax, &status);
            if ( U_FAILURE(status) ) {
                log_err("FAIL: ureldatefmt_format() for locale %s, decPlaces %d, width %d, capContext %d, offset %.2f, unit %d: %s\n",
                    itemPtr->locale, itemPtr->decPlaces, (int)itemPtr->width, (int)itemPtr->capContext,
                    offsets[iOffset], (int)itemPtr->unit, myErrorName(status) );
            } else {
                UChar ubufexp[kUBufMax];
                int32_t ulenexp = u_unescape(itemPtr->expectedResults[iOffset*2], ubufexp, kUBufMax);
                if (ulenget != ulenexp || u_strncmp(ubufget, ubufexp, ulenexp) != 0) {
                    char  bbufget[kBBufMax];
                    u_austrncpy(bbufget, ubufget, kUBufMax);
                    log_err("ERROR: ureldatefmt_format() for locale %s, decPlaces %d, width %d, capContext %d, offset %.2f, unit %d;\n      expected %s\n      get      %s\n",
                        itemPtr->locale, itemPtr->decPlaces, (int)itemPtr->width, (int)itemPtr->capContext,
                        offsets[iOffset], (int)itemPtr->unit, itemPtr->expectedResults[iOffset*2], bbufget );
                }
            }

            status = U_ZERO_ERROR;
            ulenget = ureldatefmt_formatNumeric(reldatefmt, offsets[iOffset], itemPtr->unit, ubufget, kUBufMax, &status);
            if ( U_FAILURE(status) ) {
                log_err("FAIL: ureldatefmt_formatNumeric() for locale %s, decPlaces %d, width %d, capContext %d, offset %.2f, unit %d: %s\n",
                    itemPtr->locale, itemPtr->decPlaces, (int)itemPtr->width, (int)itemPtr->capContext,
                    offsets[iOffset], (int)itemPtr->unit, myErrorName(status) );
            } else {
                UChar ubufexp[kUBufMax];
                int32_t ulenexp = u_unescape(itemPtr->expectedResults[iOffset*2 + 1], ubufexp, kUBufMax);
                if (ulenget != ulenexp || u_strncmp(ubufget, ubufexp, ulenexp) != 0) {
                    char  bbufget[kBBufMax];
                    u_austrncpy(bbufget, ubufget, kUBufMax);
                    log_err("ERROR: ureldatefmt_formatNumeric() for locale %s, decPlaces %d, width %d, capContext %d, offset %.2f, unit %d;\n      expected %s\n      get      %s\n",
                        itemPtr->locale, itemPtr->decPlaces, (int)itemPtr->width, (int)itemPtr->capContext,
                        offsets[iOffset], (int)itemPtr->unit, itemPtr->expectedResults[iOffset*2 + 1], bbufget );
                }
            }
        }

        ureldatefmt_close(reldatefmt);
    }
}

static void TestNumericField(void)
{
    const RelDateTimeFormatTestItem *itemPtr;
    log_verbose("\nTesting ureldatefmt_open(), ureldatefmt_formatForFields(), ureldatefmt_formatNumericForFields() with various parameters\n");
    for (itemPtr = fmtTestItems; itemPtr->locale != NULL; itemPtr++) {
        URelativeDateTimeFormatter *reldatefmt = NULL;
        UNumberFormat* nfToAdopt = NULL;
        UErrorCode status = U_ZERO_ERROR;
        int32_t iOffset;

        if (itemPtr->decPlaces >= 0) {
            nfToAdopt = unum_open(UNUM_DECIMAL, NULL, 0, itemPtr->locale, NULL, &status);
            if ( U_FAILURE(status) ) {
                log_data_err("FAIL: unum_open(UNUM_DECIMAL, ...) for locale %s: %s\n", itemPtr->locale, myErrorName(status));
                continue;
            }
            unum_setAttribute(nfToAdopt, UNUM_MIN_FRACTION_DIGITS, itemPtr->decPlaces);
            unum_setAttribute(nfToAdopt, UNUM_MAX_FRACTION_DIGITS, itemPtr->decPlaces);
            unum_setAttribute(nfToAdopt, UNUM_ROUNDING_MODE, UNUM_ROUND_DOWN);
        }
        reldatefmt = ureldatefmt_open(itemPtr->locale, nfToAdopt, itemPtr->width, itemPtr->capContext, &status);
        if ( U_FAILURE(status) ) {
            log_data_err("FAIL: ureldatefmt_open() for locale %s, decPlaces %d, width %d, capContext %d: %s\n",
                    itemPtr->locale, itemPtr->decPlaces, (int)itemPtr->width, (int)itemPtr->capContext,
                    myErrorName(status) );
            continue;
        }

        for (iOffset = 0; iOffset < kNumOffsets; iOffset++) {
            /* Depend on the next one to verify the data */
            status = U_ZERO_ERROR;
            UFormattedRelativeDateTime* fv = ureldatefmt_openResult(&status);
            if ( U_FAILURE(status) ) {
                log_err("ureldatefmt_openResult fails, status %s\n", u_errorName(status));
                continue;
            }
            ureldatefmt_formatToResult(reldatefmt, offsets[iOffset], itemPtr->unit, fv, &status);
            if ( U_FAILURE(status) ) {
                log_err("FAIL: ureldatefmt_formatForFields() for locale %s, decPlaces %d, width %d, capContext %d, offset %.2f, unit %d: %s\n",
                    itemPtr->locale, itemPtr->decPlaces, (int)itemPtr->width, (int)itemPtr->capContext,
                    offsets[iOffset], (int)itemPtr->unit, myErrorName(status) );
            } else {
                UChar ubufexp[kUBufMax];
                int32_t ulenexp = u_unescape(itemPtr->expectedResults[iOffset*2], ubufexp, kUBufMax);
                int32_t ulenget;
                const UChar* ubufget = ufmtval_getString(ureldatefmt_resultAsValue(fv, &status), &ulenget, &status);
                assertUEquals("String content", ubufexp, ubufget);
                assertIntEquals("String length", ulenexp, ulenget);

                FieldsDat expectedAttr = itemPtr->expectedAttributes[iOffset*2];
                UConstrainedFieldPosition* cfpos = ucfpos_open(&status);
                UBool foundNumeric = false;
                while (true) {
                    foundNumeric = ufmtval_nextPosition(ureldatefmt_resultAsValue(fv, &status), cfpos, &status);
                    if (!foundNumeric) {
                        break;
                    }
                    if (ucfpos_getCategory(cfpos, &status) == UFIELD_CATEGORY_RELATIVE_DATETIME
                        && ucfpos_getField(cfpos, &status) == UDAT_REL_NUMERIC_FIELD) {
                        break;
                    }
                }
                assertSuccess("Looking for numeric", &status);
                int32_t beginPos, endPos;
                ucfpos_getIndexes(cfpos, &beginPos, &endPos, &status);
                if (expectedAttr.field == -1) {
                    if (foundNumeric) {
                        log_err("ureldatefmt_formatForFields as \"%s\"; expect no field, but got %d\n",
                                itemPtr->expectedResults[iOffset*2],
                                ucfpos_getField(cfpos, &status));
                    }
                } else {
                    if (!foundNumeric ||
                        beginPos != expectedAttr.beginPos ||
                        endPos != expectedAttr.endPos) {
                        log_err("ureldatefmt_formatForFields as \"%s\"; expect field %d range %d-%d, get range %d-%d\n",
                                itemPtr->expectedResults[iOffset*2],
                                expectedAttr.field, expectedAttr.beginPos, expectedAttr.endPos,
                                beginPos, endPos);
                    }
                }
                ucfpos_close(cfpos);
            }

            /* Depend on the next one to verify the data */
            status = U_ZERO_ERROR;
            ureldatefmt_formatNumericToResult(reldatefmt, offsets[iOffset], itemPtr->unit, fv, &status);
            if ( U_FAILURE(status) ) {
                log_err("FAIL: ureldatefmt_formatNumericForFields() for locale %s, decPlaces %d, width %d, capContext %d, offset %.2f, unit %d: %s\n",
                    itemPtr->locale, itemPtr->decPlaces, (int)itemPtr->width, (int)itemPtr->capContext,
                    offsets[iOffset], (int)itemPtr->unit, myErrorName(status) );
            } else {
                UChar ubufexp[kUBufMax];
                int32_t ulenexp = u_unescape(itemPtr->expectedResults[iOffset*2 + 1], ubufexp, kUBufMax);
                int32_t ulenget;
                const UChar* ubufget = ufmtval_getString(ureldatefmt_resultAsValue(fv, &status), &ulenget, &status);
                assertUEquals("String content", ubufexp, ubufget);
                assertIntEquals("String length", ulenexp, ulenget);

                FieldsDat expectedAttr = itemPtr->expectedAttributes[iOffset*2 + 1];
                UConstrainedFieldPosition* cfpos = ucfpos_open(&status);
                UBool foundNumeric = false;
                while (true) {
                    foundNumeric = ufmtval_nextPosition(ureldatefmt_resultAsValue(fv, &status), cfpos, &status);
                    if (!foundNumeric) {
                        break;
                    }
                    if (ucfpos_getCategory(cfpos, &status) == UFIELD_CATEGORY_RELATIVE_DATETIME
                        && ucfpos_getField(cfpos, &status) == UDAT_REL_NUMERIC_FIELD) {
                        break;
                    }
                }
                assertSuccess("Looking for numeric", &status);
                int32_t beginPos, endPos;
                ucfpos_getIndexes(cfpos, &beginPos, &endPos, &status);
                if (expectedAttr.field == -1) {
                    if (foundNumeric) {
                        log_err("ureldatefmt_formatForFields as \"%s\"; expect no field, but got %d rang %d-%d\n",
                                itemPtr->expectedResults[iOffset*2],
                                ucfpos_getField(cfpos, &status), beginPos, endPos);
                    }
                } else {
                    if (!foundNumeric ||
                        (beginPos != expectedAttr.beginPos || endPos != expectedAttr.endPos)) {
                        log_err("ureldatefmt_formatForFields as \"%s\"; expect field %d range %d-%d, get field %d range %d-%d\n",
                                itemPtr->expectedResults[iOffset*2 + 1],
                                expectedAttr.field, expectedAttr.beginPos, expectedAttr.endPos,
                                ucfpos_getField(cfpos, &status), beginPos, endPos);
                    }
                }
                ucfpos_close(cfpos);
            }
            ureldatefmt_closeResult(fv);
        }

        ureldatefmt_close(reldatefmt);
    }
}

typedef struct {
    const char*                         locale;
    UDateRelativeDateTimeFormatterStyle width;
    UDisplayContext                     capContext;
    const char *                        relativeDateString;
    const char *                        timeString;
    const char *                        expectedResult;
} CombineDateTimeTestItem;

static const CombineDateTimeTestItem combTestItems[] = {
#if APPLE_ICU_CHANGES
// rdar://
    { "en",  UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, "yesterday",  "3:45 PM",  "yesterday at 3:45 PM" },
#else
    { "en",  UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, "yesterday",  "3:45 PM",  "yesterday, 3:45 PM" },
#endif  // APPLE_ICU_CHANGES
#if APPLE_ICU_CHANGES
// rdar://
    // rdar://55667608 and https://unicode-org.atlassian.net/browse/CLDR-10321
    { "fi",  UDAT_STYLE_LONG,  UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, "eilen",  "3:45 PM",  "eilen klo 3:45 PM" },
#endif  // APPLE_ICU_CHANGES
    { NULL,  (UDateRelativeDateTimeFormatterStyle)0, (UDisplayContext)0, NULL, NULL, NULL } /* terminator */
};

static void TestCombineDateTime(void)
{
    const CombineDateTimeTestItem *itemPtr;
    log_verbose("\nTesting ureldatefmt_combineDateAndTime() with various parameters\n");
    for (itemPtr = combTestItems; itemPtr->locale != NULL; itemPtr++) {
        URelativeDateTimeFormatter *reldatefmt = NULL;
        UErrorCode status = U_ZERO_ERROR;
        UChar ubufreldate[kUBufMax];
        UChar ubuftime[kUBufMax];
        UChar ubufget[kUBufMax];
        int32_t ulenreldate, ulentime, ulenget;

        reldatefmt = ureldatefmt_open(itemPtr->locale, NULL, itemPtr->width, itemPtr->capContext, &status);
        if ( U_FAILURE(status) ) {
            log_data_err("FAIL: ureldatefmt_open() for locale %s, width %d, capContext %d: %s\n",
                    itemPtr->locale, (int)itemPtr->width, (int)itemPtr->capContext, myErrorName(status) );
            continue;
        }

        ulenreldate = u_unescape(itemPtr->relativeDateString, ubufreldate, kUBufMax);
        ulentime    = u_unescape(itemPtr->timeString,         ubuftime,    kUBufMax);
        ulenget     = ureldatefmt_combineDateAndTime(reldatefmt, ubufreldate, ulenreldate, ubuftime, ulentime, ubufget, kUBufMax, &status);
        if ( U_FAILURE(status) ) {
            log_err("FAIL: ureldatefmt_combineDateAndTime() for locale %s, width %d, capContext %d: %s\n",
                itemPtr->locale, (int)itemPtr->width, (int)itemPtr->capContext, myErrorName(status) );
        } else {
            UChar ubufexp[kUBufMax];
            int32_t ulenexp = u_unescape(itemPtr->expectedResult, ubufexp, kUBufMax);
            if (ulenget != ulenexp || u_strncmp(ubufget, ubufexp, ulenexp) != 0) {
                char  bbufget[kBBufMax];
                u_austrncpy(bbufget, ubufget, kUBufMax);
                log_err("ERROR: ureldatefmt_combineDateAndTime() for locale %s, width %d, capContext %d;\n      expected %s\n      get      %s\n",
                    itemPtr->locale, (int)itemPtr->width, (int)itemPtr->capContext, itemPtr->expectedResult, bbufget );
            }
        }
        // preflight test
        status = U_ZERO_ERROR;
        ulenget = ureldatefmt_combineDateAndTime(reldatefmt, ubufreldate, ulenreldate, ubuftime, ulentime, NULL, 0, &status);
        if ( status != U_BUFFER_OVERFLOW_ERROR) {
            log_err("FAIL: ureldatefmt_combineDateAndTime() preflight for locale %s, width %d, capContext %d: expected U_BUFFER_OVERFLOW_ERROR, got %s\n",
                itemPtr->locale, (int)itemPtr->width, (int)itemPtr->capContext, myErrorName(status) );
        } else {
            UChar ubufexp[kUBufMax];
            int32_t ulenexp = u_unescape(itemPtr->expectedResult, ubufexp, kUBufMax);
            if (ulenget != ulenexp) {
                log_err("ERROR: ureldatefmt_combineDateAndTime() preflight for locale %s, width %d, capContext %d;\n      expected len %d, get len %d\n",
                    itemPtr->locale, (int)itemPtr->width, (int)itemPtr->capContext, ulenexp, ulenget );
            }
        }

        ureldatefmt_close(reldatefmt);
    }
}

static void TestFields(void) {
    UErrorCode ec = U_ZERO_ERROR;
    URelativeDateTimeFormatter* fmt = ureldatefmt_open(
        "en-us",
        NULL,
        UDAT_STYLE_SHORT,
        UDISPCTX_CAPITALIZATION_NONE,
        &ec);
    assertSuccess("Creating RelDTFmt", &ec);
    UFormattedRelativeDateTime* frdt = ureldatefmt_openResult(&ec);
    assertSuccess("Creating FmtVal", &ec);

    ureldatefmt_formatNumericToResult(fmt, -50, UDAT_REL_UNIT_SATURDAY, frdt, &ec);
    assertSuccess("formatNumeric", &ec);
    {
        const UFormattedValue* fv = ureldatefmt_resultAsValue(frdt, &ec);
        assertSuccess("Should convert without error", &ec);
        static const UFieldPositionWithCategory expectedFieldPositions[] = {
            // category, field, begin index, end index
            {UFIELD_CATEGORY_NUMBER, UNUM_INTEGER_FIELD, 0, 2},
            {UFIELD_CATEGORY_RELATIVE_DATETIME, UDAT_REL_NUMERIC_FIELD, 0, 2},
            {UFIELD_CATEGORY_RELATIVE_DATETIME, UDAT_REL_LITERAL_FIELD, 3, 11}};
        checkMixedFormattedValue(
            "FormattedRelativeDateTime as FormattedValue (numeric)",
            fv,
            u"50 Sat. ago",
            expectedFieldPositions,
            UPRV_LENGTHOF(expectedFieldPositions));
    }

    ureldatefmt_formatToResult(fmt, -1, UDAT_REL_UNIT_WEEK, frdt, &ec);
    assertSuccess("format", &ec);
    {
        const UFormattedValue* fv = ureldatefmt_resultAsValue(frdt, &ec);
        assertSuccess("Should convert without error", &ec);
        static const UFieldPositionWithCategory expectedFieldPositions[] = {
            // category, field, begin index, end index
            {UFIELD_CATEGORY_RELATIVE_DATETIME, UDAT_REL_LITERAL_FIELD, 0, 8}};
        checkMixedFormattedValue(
            "FormattedRelativeDateTime as FormattedValue (relative)",
            fv,
            u"last wk.",
            expectedFieldPositions,
            UPRV_LENGTHOF(expectedFieldPositions));
    }

    ureldatefmt_closeResult(frdt);
    ureldatefmt_close(fmt);
}

#endif /* #if !UCONFIG_NO_FORMATTING && !UCONFIG_NO_BREAK_ITERATION */

#include "AudNR.h"
#include "stdlib.h"
#include "Type.h"
#include "UIAppMovie_AudNR.h"

UINT32 gAudNrFocusStopFreq     = 15000;
UINT32 gAudNrFocusReplaceFreq  = 15000;
UINT32 gAudNrFocusDecreaseLvl  = AUDNR_COEF(20.0);
UINT32 gAudNrFocusMixBackgrd   = AUDNR_COEF(0.7);

static INT32  gAudNrFocusBandNum      = 11;

static INT32  gpAudNrFocusNoiseFreqBand[11] = {
0,
6,
89,
147,
208,
275,
332,
460,
640,
800,
1023,
};

static INT64  gpAudNrFocusNoisePower[11] = {
0x0000000000000000LL,
0x000000000064eb73LL,
0x0000000006f28595LL,
0x0000000011fcd3dfLL,
0x000000001fc9cb0fLL,
0x000000002736c1f8LL,
0x000000001f526344LL,
0x0000000000a9791cLL,
0x000000000001b342LL,
0x000000000006b798LL,
0x000000007fe1fc4bLL,
};

static INT32  gpAudNrFocusNoiseSpec[1024] = {
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x353,
0x49e,
0xc2,
0x20d,
0x405,
0xb3,
0x2a1,
0x128,
0x6c,
0x1a6,
0xfc,
0x4b,
0x11b,
0xd3,
0x2c5,
0x78,
0x1b,
0x160,
0xd1,
0x20,
0x107,
0xda,
0xa5,
0x8e,
0xd4,
0xba,
0x91,
0x39,
0x8a,
0x167,
0x81,
0xb9,
0x1e,
0xb8,
0xca,
0x4f,
0x58,
0x99,
0x7c,
0xae,
0x65,
0xa9,
0x79,
0x85,
0x6d,
0x69,
0x89,
0x37,
0x84,
0xe9,
0x28,
0x79,
0x1b8,
0xed,
0x10f,
0x17,
0xa7,
0x31,
0x8d,
0x20,
0xa9,
0xa7,
0xa0,
0x12,
0x3b,
0x68,
0x59,
0x59,
0x6f,
0x3b,
0x4d,
0x92,
0x59,
0x30,
0x20,
0xbb,
0x71,
0x77,
0x49,
0x61,
0xd,
0x94,
0x5c,
0x14e,
0x78b,
0x571,
0x6b0,
0x516,
0x543,
0x3fe,
0x683,
0x70f,
0x2a2,
0x3f4,
0x42c,
0x6d4,
0x619,
0x2dd,
0x471,
0x529,
0x543,
0x7fb,
0x5f1,
0x43c,
0x5e1,
0x62c,
0x695,
0x686,
0x4e6,
0x569,
0xa32,
0x5c7,
0x7b0,
0x556,
0x350,
0x7dd,
0x2cf,
0x50f,
0x540,
0x3c0,
0x827,
0x508,
0x3be,
0x4a7,
0x420,
0x4a1,
0x31c,
0x826,
0x318,
0x4bb,
0x7dc,
0x478,
0x367,
0x399,
0x72f,
0x41d,
0x5a8,
0x3b0,
0x353,
0x564,
0x67b,
0x65a,
0x5e2,
0x7e5,
0x9c9,
0x969,
0x8eb,
0x5d5,
0x80a,
0x405,
0x8ac,
0x69f,
0x209,
0xa75,
0x2b7,
0x668,
0xac2,
0x27e,
0x5ee,
0xad9,
0x383,
0x3cb,
0x62a,
0x4eb,
0x5ce,
0x43c,
0x51d,
0x41c,
0x73e,
0xbbb,
0xe0b,
0x7fe,
0x6c6,
0xe81,
0xa45,
0x761,
0x8be,
0x6f5,
0x6c8,
0xaeb,
0x6f0,
0x2d2,
0xf64,
0xb33,
0x8e3,
0xa89,
0xa0d,
0x71e,
0x7ca,
0x59c,
0x543,
0x6c9,
0x58b,
0xb01,
0xd1e,
0x63c,
0xadd,
0x896,
0x1059,
0x9a5,
0xd37,
0xd4f,
0xff2,
0x1319,
0xb5b,
0x1529,
0x935,
0xae8,
0xf21,
0x114a,
0xcaa,
0x11ab,
0xe2e,
0x14b9,
0xdaf,
0x12a8,
0x1129,
0x1838,
0x1826,
0x1104,
0x12ba,
0xc3a,
0x13a6,
0x6c1,
0x668,
0x69f,
0x81d,
0x488,
0x88c,
0x88f,
0x293,
0x579,
0x755,
0x3b4,
0x442,
0x258,
0x42c,
0x25e,
0x20e,
0x4a3,
0x3b4,
0x1e3,
0x49c,
0x491,
0x58a,
0x643,
0x4de,
0x1a0,
0x3e5,
0x506,
0x5d9,
0x6f8,
0x6ba,
0x5a7,
0xca7,
0x39f,
0x758,
0x2ce,
0x855,
0x6f9,
0x67f,
0x3c0,
0x815,
0xa34,
0x43e,
0x6c4,
0xc51,
0x4fd,
0xec2,
0x1061,
0xfc8,
0xba5,
0x8e8,
0x1432,
0x841,
0xb58,
0xa9d,
0x6b9,
0xe98,
0x1418,
0x142d,
0xb09,
0x10c4,
0x75c,
0x1487,
0x1272,
0x89d,
0xc45,
0x167e,
0x8e2,
0x4c9,
0x11f6,
0xbea,
0x107a,
0xf2d,
0x88f,
0x15d4,
0xbab,
0x11d2,
0x1331,
0x449,
0x16dd,
0xebc,
0xb3f,
0xa13,
0x6fa,
0x116f,
0xf6e,
0x9a8,
0xbfe,
0xe51,
0xe38,
0x71e,
0xe73,
0xd0a,
0x922,
0x8e1,
0x968,
0x6ab,
0x329,
0x55f,
0x8c4,
0x53e,
0x36c,
0x42d,
0x3a1,
0x6ae,
0x55e,
0x42e,
0xd5b,
0x50d,
0x4cb,
0x655,
0xb25,
0xaf4,
0x950,
0x80e,
0x682,
0xea8,
0xa7e,
0xd9f,
0x5e5,
0x459,
0xaf0,
0x596,
0x461,
0xcd9,
0xcf4,
0xcdc,
0x1108,
0xf21,
0x1057,
0x4ec,
0x880,
0x988,
0x986,
0xbf1,
0x74a,
0x1bd2,
0xc06,
0xb75,
0xa3b,
0x990,
0xdb1,
0xa8d,
0xfd0,
0x485,
0x6e7,
0x67e,
0x995,
0xa0b,
0xc42,
0x689,
0xb2a,
0x851,
0x2ba,
0x9e4,
0x636,
0x387,
0x6e2,
0x61e,
0x828,
0x566,
0x4ec,
0x320,
0x276,
0x46d,
0x305,
0x1ce,
0x416,
0x2bd,
0x2df,
0x224,
0x233,
0x1a7,
0x43b,
0x306,
0x373,
0x2bd,
0x531,
0x2ca,
0x84a,
0x2c6,
0xa26,
0x631,
0x8d2,
0x94e,
0x830,
0x457,
0x668,
0x5f4,
0x5db,
0x661,
0x756,
0x724,
0x9aa,
0x87e,
0x921,
0x425,
0x8dc,
0x90b,
0x5aa,
0x513,
0xf6,
0xa3b,
0x2bb,
0x6bf,
0x712,
0x418,
0x4fa,
0x3e4,
0x359,
0x49a,
0x576,
0x21c,
0x477,
0x171,
0x281,
0x64a,
0x377,
0x168,
0x2c6,
0x235,
0x211,
0x159,
0x143,
0x251,
0x24f,
0x2b4,
0x37f,
0x1b1,
0x3db,
0x24b,
0x404,
0x6c,
0xf1,
0x1b1,
0x1a0,
0x12e,
0x1c2,
0x132,
0xe4,
0x11e,
0x5f,
0x1d9,
0x67,
0x1e6,
0x8d,
0x90,
0xdf,
0x115,
0x9b,
0x142,
0xc2,
0xa4,
0x117,
0x81,
0xd8,
0x97,
0x108,
0x92,
0x164,
0x2d,
0x162,
0x133,
0x1e,
0x14b,
0x116,
0x1a7,
0x102,
0x1a7,
0xba,
0x7f,
0x1b1,
0xf4,
0x83,
0x3b,
0x1ae,
0x88,
0xd9,
0x8f,
0xc3,
0xea,
0xde,
0x9c,
0x12d,
0xe1,
0x152,
0x169,
0x122,
0x5d,
0x8f,
0x64,
0xb5,
0xc3,
0x5e,
0x96,
0x140,
0x3b,
0x10a,
0x84,
0xf8,
0x6b,
0xa2,
0x158,
0x179,
0xa2,
0x175,
0xb2,
0x1dc,
0x96,
0x187,
0x108,
0x150,
0x14b,
0x109,
0x18c,
0xbf,
0xd7,
0x1a7,
0x76,
0x134,
0x6f,
0xa2,
0xcc,
0xc9,
0x37,
0xe9,
0x9e,
0x9e,
0x10d,
0x139,
0x17c,
0x130,
0xc0,
0xd6,
0x103,
0x7e,
0x9d,
0x9b,
0x137,
0x48,
0xe5,
0x78,
0xb5,
0x38,
0x46,
0x93,
0x21d,
0xbf,
0xfb,
0xac,
0x5e,
0xf7,
0xf5,
0x8f,
0x9a,
0x85,
0x8b,
0xba,
0xef,
0x7d,
0xe9,
0x19f,
0xf2,
0xb8,
0x18f,
0x116,
0x13e,
0x78,
0x27f,
0x95,
0x159,
0xa4,
0xca,
0xc6,
0x167,
0x78,
0x66,
0x7f,
0x75,
0x7d,
0x7c,
0x49,
0x9f,
0xdf,
0x4f,
0x6d,
0xd9,
0x64,
0xc7,
0xaa,
0xe9,
0xd0,
0x5c,
0x91,
0xbb,
0xab,
0x81,
0x1a1,
0x75,
0xc4,
0x38,
0xfa,
0x75,
0xd0,
0x21,
0x104,
0x1a2,
0xf4,
0x95,
0x79,
0x19a,
0xca,
0x5,
0x1,
0x1c,
0x3,
0x14,
0x5,
0x2c,
0x19,
0xa,
0x23,
0x2d,
0x23,
0x17,
0x18,
0x9,
0x1f,
0x38,
0x1f,
0x14,
0x20,
0xd,
0x24,
0x19,
0x3d,
0x6,
0x7,
0xe,
0x2b,
0xb,
0x1e,
0x2,
0x2f,
0x25,
0xb,
0x9,
0x1b,
0x3,
0x4,
0x4,
0x17,
0x25,
0x10,
0x3,
0x3a,
0x7,
0x9,
0x2,
0x5,
0xd,
0x4,
0x5,
0x1b,
0x8,
0xe,
0x12,
0xd,
0x10,
0x0,
0x2d,
0x6,
0x26,
0x2,
0xd,
0x18,
0x2a,
0x1f,
0xb,
0x6,
0x2d,
0x4,
0x2,
0x27,
0xc,
0xd,
0x9,
0x6,
0x24,
0xf,
0x2,
0xc,
0x4,
0x28,
0x19,
0x17,
0x4,
0x7,
0x6,
0xd,
0x5,
0x3,
0x3,
0x25,
0x4,
0x2,
0x5,
0xc,
0x4,
0x3e,
0x2e,
0x14,
0x7,
0x23,
0x16,
0x21,
0x18,
0x23,
0x1e,
0x3f,
0x23,
0x1,
0x3,
0x24,
0x4b,
0x6,
0xa,
0x19,
0x9,
0x12,
0x4,
0x1f,
0x1a,
0x2e,
0x43,
0x20,
0x5,
0x5,
0x9,
0x14,
0xe,
0x2,
0x17,
0xb,
0x7,
0x35,
0x17,
0x1,
0x4,
0x1a,
0x13,
0x5,
0x26,
0x4,
0xf,
0x16,
0xc,
0x5,
0x23,
0x1b,
0x24,
0x16,
0x22,
0x2c,
0x9,
0x18,
0x28,
0x36,
0x25,
0x4,
0x18,
0xc,
0xa,
0x18,
0x10,
0x4,
0x5,
0x1c,
0x1,
0x22,
0x23,
0x23,
0x26,
0x7,
0x26,
0x12,
0x6,
0x1e,
0x8,
0x2b,
0x4,
0x13,
0x13,
0x5,
0x2d,
0x5,
0xd,
0xa,
0xb,
0x23,
0x6,
0x6,
0x9,
0x9,
0x1f,
0x20,
0x8,
0x24,
0xa,
0x49,
0x13,
0x24,
0x1c,
0x5,
0x4,
0x2e,
0x26,
0x9,
0x29,
0x12,
0x25,
0x14,
0x1a,
0x17,
0x2,
0x23,
0xc,
0x1c,
0x24,
0x1a,
0x5,
0xf,
0x9,
0x9,
0x7,
0x1a,
0x6,
0x6,
0x2a,
0x17,
0x6,
0x30,
0x23,
0x32,
0x22,
0x2e,
0xe,
0x19,
0x1b,
0x31,
0x3f,
0x13,
0x4d,
0x4,
0x44,
0x51,
0x4e,
0x68,
0x40,
0x26,
0x47,
0x6e,
0x46,
0x1a,
0x3f,
0x26,
0x30,
0x2b,
0x45,
0x2d,
0x12,
0x23,
0x13,
0x16,
0x19,
0x57,
0x5a,
0x42,
0x1c,
0x35,
0x2b,
0x49,
0x39,
0x3b,
0x73,
0x50,
0x44,
0x3a,
0x5e,
0x27,
0x37,
0x48,
0x2f,
0x26,
0x26,
0x1b,
0x37,
0x21,
0x2d,
0x1d,
0x2b,
0x37,
0x3c,
0x41,
0x32,
0xf,
0x20,
0xb,
0x2f,
0x2b,
0x16,
0x1d,
0xb,
0x3a,
0x3d,
0x30,
0x21,
0x26,
0x1b,
0x19,
0x2b,
0x57,
0x37,
0x9,
0xe,
0x1a,
0x28,
0xa,
0x13,
0x2f,
0x1f,
0x4c,
0x1a,
0x2f,
0x28,
0x3b,
0x24,
0x2f,
0x35,
0xa,
0x46,
0x1a,
0x49,
0x9,
0x47,
0x44,
0x27,
0x41,
0x57,
0x23,
0x28,
0x20,
0x9,
0x25,
0x56,
0x22,
0x2c,
0x2c,
0xa,
0x2c,
0x4,
0x38,
0x23,
0x2a,
0x2e,
0x35,
0x29,
0x35,
0x53,
0x13,
0x23,
0x54,
0x24,
0x35,
0x26,
0x53,
0x2e,
0x1c,
0x3b,
0x41,
0x15,
0x22,
0x32,
0x13,
0x1f,
0x28,
0x16,
0x36,
0x2a,
0x28,
0x25,
0x11,
0x6,
0x2d,
0x24,
0x0,
};

AUDNR_NOISE_SPEC gpAudNrNoiseSpec_Focus =
{
    &gAudNrFocusBandNum,
    gpAudNrFocusNoiseFreqBand,
    gpAudNrFocusNoisePower,
    gpAudNrFocusNoiseSpec
};


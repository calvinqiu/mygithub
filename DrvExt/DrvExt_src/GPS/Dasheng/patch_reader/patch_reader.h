#include "../inc/types.h"
#include "../hal/hal.h"

bool Patch_Initialize(void);
bool ReadHex(GlIntU8* pAscii, GlIntU32* Value);
bool ReadDec(GlIntU8* pAscii, GlIntU32* Value);
GlIntS32 GetBlob(GlIntU8* aBuf, GlIntU32 iLen);
GlIntU32 GetAsicVersion(void);
GlIntU32 GetChangeList(void);
GlIntU32 GetPatchLevel(void);
GlIntU32 GetBlobLength(void);
GlIntU32 GetPatchAddress(void);
GlIntU32 GetType(void);
void reset_patch(void);
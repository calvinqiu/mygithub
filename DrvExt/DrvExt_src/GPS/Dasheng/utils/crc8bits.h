#include "../inc/types.h"

GlIntU8 Crc8Bits_Update(GlIntU16 context, const GlIntU8 *pucData, GlIntU16 usLen);
GlIntU8 Crc8Bits_GetState(GlIntU16 context);
void Crc8Bits_Reset(GlIntU16 context);
void reset_crc(void);


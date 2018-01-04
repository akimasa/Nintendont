// Nintendont (kernel): Broadband Adapter functions.
// Used by EXI.c.

#ifndef __BBA_H__
#define __BBA_H__

/**
 * Initialize the BBA context.
 */
void BBA_Init(void);

/**
 * BBA Immediate Write.
 * @param data Data.
 * @param size Size.
 */
void BBAImmWrite(u32 data, u32 size);

/**
 * BBA Immediate Read.
 * @param size Size.
 * @return Data.
 */
u32 BBAImmRead(u32 size);

#endif /* __BBA_H__ */

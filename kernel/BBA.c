// Nintendont (kernel): Broadband Adapter functions.
// Used by EXI.c.
#include <string.h>

#include "BBA_types.h"
#include "EXI.h"
static BBA_Ctx ctx;

void BBA_Init(void)
{
	// Initialize the BBA context.

	// Transfer status.
	ctx.transfer.direction = 0;
	ctx.transfer.region = 0;
	ctx.transfer.address = 0;
	ctx.transfer.valid = 0;

	// EXI status.
	ctx.exi.device_id = 0xD107;
	ctx.exi.revision_id = 0;
	ctx.exi.acstart = 0x4E;
	ctx.exi.interrupt_mask = 0;
	ctx.exi.interrupt = 0;
	ctx.exi.hash_challenge = 0;
	ctx.exi.hash_response = 0;
	ctx.exi.hash_status = 0;

	ctx.descriptor = 0;
	ctx.recv_buffer_len = 0;

	// MX hard reset.
	memset(ctx.bba_mem, 0, sizeof(ctx.bba_mem));
	ctx.bba_mem[BBA_NCRB] = NCRB_PR;
	ctx.bba_mem[BBA_NWAYC] = NWAYC_LTE | NWAYC_ANE;
	ctx.bba_mem[BBA_MISC] = MISC1_TPF | MISC1_TPH | MISC1_TXF | MISC1_TXH;

	// FIXME: Get the system's MAC address.
	// For now, using a generated MAC address.
	// Generated using: https://www.miniwebtool.com/mac-address-generator/
	static const u8 sys_mac_addr[6] = {0xCB,0x5C,0x21,0x26,0x8C,0x74};
	memcpy(&ctx.bba_mem[BBA_NAFR_PAR0], sys_mac_addr, sizeof(sys_mac_addr));

	// HACK: Assuming we have a fully-established 100BASE-T link.
	ctx.bba_mem[BBA_NWAYS] = NWAYS_LS100 | NWAYS_LPNWAY | NWAYS_100TXF | NWAYS_ANCLPT;
}

/**
 * Get the TX FIFO count.
 * @return TX FIFO count.
 */
static inline u16 GetTXFIFOCount(void)
{
	return ctx.bba_mem[BBA_TXFIFOCNT] | (ctx.bba_mem[BBA_TXFIFOCNT+1] << 8);
}

/**
 * Set the TX FIFO count.
 * @param count TX FIFO count.
 */
static inline void SetTXFIFOCount(u16 count)
{
	ctx.bba_mem[BBA_TXFIFOCNT] = (count & 0xFF);
	ctx.bba_mem[BBA_TXFIFOCNT+1] = (count >> 8);
}

/**
 * Send a packet using DMA.
 */
static void BBA_SendFromPacketBuffer(void)
{
	// TODO
}

/**
 * Direct FIFO Write.
 * @param data Data to write.
 * @param size Size.
 */
static void BBA_DirectFIFOWrite(const u8 *data, u32 size)
{
	// In direct mode, the hardware handles creating the state required by the
	// GMAC instead of finagling with packet descriptors and such.

	// FIFO count is 16-bit little-endian, so we have to byteswap it.
	u16 tx_fifo_count = GetTXFIFOCount();
	memcpy(&ctx.tx_fifo[tx_fifo_count], data, size);
	tx_fifo_count += size;

	// TODO: Not sure if this mask is correct.
	// However, BBA_TXFIFOCNT should never get even close to this amount,
	// so it shouldn't matter.
	tx_fifo_count &= ((1 << 12) - 1);

	// Swap tx_fifo_count back into memory.
	SetTXFIFOCount(tx_fifo_count);
}

/**
 * Send a packet using the direct FIFO.
 */
static void BBA_SendFromDirectFIFO(void)
{
	// TODO: SendFrame() function.
	//SendFrame(ctx.bba_mem, GetTXFIFOCount());
}

/**
 * Is this an MX command?
 * @return True if it is; false if it isn't.
 */
static inline bool IsMXCommand(u32 data)
{
	return !!(data & (1 << 31));
}

/**
 * Is this a write command?
 * @return True if it is; false if it isn't.
 */
static inline bool IsWriteCommand(u32 data)
{
	return (IsMXCommand(data) ? !!(data & (1 << 30)) : !!(data & (1 << 14)));
}

/**
 * BBA Immediate Write.
 * @param data Data.
 * @param size Size.
 */
void BBAImmWrite(u32 data, u32 size)
{
	// Adjust the data for the WRITE length.
	data >>= (4 - size) * 8;

	if (!ctx.transfer.valid) {
		// Start a new transfer.
		ctx.transfer.valid = true;
		ctx.transfer.region = IsMXCommand(data) ? BBA_MX : BBA_EXI;
		if (ctx.transfer.region == BBA_EXI) {
			ctx.transfer.address = ((data & ~0xC000) >> 8) & 0xFF;
		} else {
			ctx.transfer.address = (data >> 8) & 0xFFFF;
		}
		ctx.transfer.direction = IsWriteCommand(data) ? BBA_WRITE : BBA_READ;

		if (ctx.transfer.address == BBA_IOB && ctx.transfer.region == BBA_MX)
		{
			// FIXME: Dolphin dies here because this means the RX packet
			// descriptor has been corrupted.
		}

		// Transfer has been set up.
		return;
	}

	// Writing data to an EXI or MX region.
	if (ctx.transfer.region == BBA_EXI)
	{
		switch (ctx.transfer.address)
		{
			case BBA_INTERRUPT:
				ctx.exi.interrupt &= (data ^ 0xFF);
				break;
			case BBA_INTERRUPT_MASK:
				ctx.exi.interrupt_mask = data;
				break;
			default:
				break;
		}

		// TODO: Update interrupts.
		return;
	}

	// Handle an MX command.
	switch (ctx.transfer.address)
	{
		case BBA_NCRA:
			if (data & NCRA_RESET)
			{
				// Software reset.
				// TODO: Dolphin calls Activate() here.
			}

			// Check for start/stop state changes.
			if ((ctx.bba_mem[BBA_NCRA] & NCRA_SR) ^ (data & NCRA_SR))
			{
				// Changing start/stop state.
				// TODO: Need to start/stop actual network connectivity here.
#if 0
				if (data & NCRA_SR)
					BBA_RecvStart();
				else
					BBA_RecvStop();
#endif
			}

			// Only start a transfer if there isn't one currently running.
			if (!(ctx.bba_mem[BBA_NCRA] & (NCRA_ST0 | NCRA_ST1)))
			{
				// NOTE: Transfer DMA status should be kept in TXDMA,
				// but this isn't implemented in Dolphin.
				if (data & NCRA_ST0)
				{
					// Start transfer using local DMA.
					BBA_SendFromPacketBuffer();
				}
				else if (data & NCRA_ST1)
				{
					// Start transfer using FIFO.
					BBA_SendFromDirectFIFO();
					// HACK: Send completes instantly, so we don't
					// want to write the "send in status" bit to the register.
					data &= ~NCRA_ST1;
				}
			}

			// fallthrough to register write
			goto write_to_register;

		case BBA_WRTXFIFOD:
			// Write directly to the FIFO.
			BBA_DirectFIFOWrite((u8*)&data, size);
			// No address increment.
			break;

		case BBA_IR:
			data &= ((data & 0xFF) ^ 0xFF);
			goto write_to_register;

		case BBA_TXFIFOCNT:
		case BBA_TXFIFOCNT + 1:
			// Ignore all writes to BBA_TXFIFOCNT.
			ctx.transfer.address += size;
			break;

		write_to_register:
		default: {
			// Write to the BBA registers.
			int i;
			for (i = size-1; i >= 0; i--)
			{
				ctx.bba_mem[ctx.transfer.address++] = (data >> (i*8)) & 0xFF;
			}
			break;
		}
	}
}

/**
 * BBA Immediate Read.
 * @param size Size.
 * @return Data.
 */
u32 BBAImmRead(u32 size)
{
	u32 ret = 0;

	if (ctx.transfer.region == BBA_EXI) {
		// EXI region.
		switch (ctx.transfer.address) {
			case BBA_EXI_ID:
				ret = EXI_DEVTYPE_ETHER;
				break;
			case BBA_REVISION_ID:
				ret = ctx.exi.revision_id;
				break;
			case BBA_DEVICE_ID:
				ret = ctx.exi.device_id;
				break;
			case BBA_ACSTART:
				ret = ctx.exi.acstart;
				break;
			case BBA_INTERRUPT:
				ret = ctx.exi.interrupt;
				break;
			default:
				break;
		}

		ctx.transfer.address += size;
	} else {
		// MX region.
		int i;
		for (i = size-1; i >= 0; i--) {
			ret |= (ctx.bba_mem[ctx.transfer.address++] << (i*8));
		}
	}

	// Adjust the return value for the READ length.
	ret <<= (4 - size) * 8;
	return ret;
}

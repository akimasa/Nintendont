// Nintendont (kernel): Broadband Adapter types.
// Used by EXI.c.

// References:
// - https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/HW/EXI/EXI_DeviceEthernet.cpp
// - https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/HW/EXI/EXI_DeviceEthernet.h

#ifndef __BBA_TYPES_H__
#define __BBA_TYPES_H__

#include "global.h"

// Network Control Register A
typedef enum
{
	NCRA_RESET	= 0x01,	// RESET
	NCRA_ST0	= 0x02,	// Start transmit command/status [DMA]
	NCRA_ST1	= 0x04,	// Start transmit command/status [FIFO]
	NCRA_SR		= 0x08,	// Start receive
} BBA_NCRA_e;

// Network Control Register B
typedef enum
{
	NCRB_PR		= 0x01,	// Promiscuous Mode
	NCRB_CA		= 0x02,	// Capture Effect Mode
	NCRB_PM		= 0x04,	// Pass Multicast
	NCRB_PB		= 0x08,	// Pass Bad Frame
	NCRB_AB		= 0x10,	// Accept Broadcast
	NCRB_HBD	= 0x20,	// Reserved
	NCRB_RXINTC	= 0xC0,	// Receive Interrupt Counter (mask)
} BBA_NCRB_e;

// Interrupt Mask Register
// Interrupt Register
typedef enum
{
	INT_FRAG	= 0x01,	// Fragment Counter
	INT_R		= 0x02,	// Receive
	INT_T		= 0x04,	// Transmit
	INT_R_ERR	= 0x08,	// Receive Error
	INT_T_ERROR	= 0x10,	// Transmit Error
	INT_FIFO_ERR	= 0x20,	// FIFO Error
	INT_BUS_ERR	= 0x40,	// BUS Error
	INT_RBF		= 0x80,	// RX Buffer Full
} BBA_INT_e;

// NWAY Configuration Register
typedef enum
{
	NWAYC_FD	= 0x01,	// Full Duplex Mode
	NWAYC_PS100_10	= 0x02,	// 10/100 Selection
	NWAYC_ANE	= 0x04,	// Auto-Negotiate Enable

	// Auto-Negotiation status bits...

	NWAYC_NTTEST	= 0x40,	// Reserved
	NWAYC_LTE	= 0x80,	// Link Test Enable
} BBA_NWAYC_e;

typedef enum
{
	NWAYS_LS10	= 0x01,
	NWAYS_LS100	= 0x02,
	NWAYS_LPNWAY	= 0x04,
	NWAYS_ANCLPT	= 0x08,
	NWAYS_100TXF	= 0x10,
	NWAYS_100TXH	= 0x20,
	NWAYS_10TXF	= 0x40,
	NWAYS_10TXH	= 0x80,
} BBA_NWAYS_e;

typedef enum
{
	MISC1_BURSTDMA	= 0x01,
	MISC1_DISLDMA	= 0x02,
	MISC1_TPF	= 0x04,
	MISC1_TPH	= 0x08,
	MISC1_TXF	= 0x10,
	MISC1_TXH	= 0x20,
	MISC1_TXFIFORST	= 0x40,
	MISC1_RXFIFORST	= 0x80,
} BBA_MISC1_e;

typedef enum
{
	MISC2_HBRLEN0	= 0x01,
	MISC2_HBRLEN1	= 0x02,
	MISC2_RUNTSIZE	= 0x04,
	MISC2_DREQBCTRL	= 0x08,
	MISC2_RINTSEL	= 0x10,
	MISC2_ITPSEL	= 0x20,
	MISC2_A11A8EN	= 0x40,
	MISC2_AUTORCVR	= 0x80,
} BBA_MISC2_e;

// BBA registers.
typedef enum
{
	BBA_NCRA	= 0x00,
	BBA_NCRB	= 0x01,

	BBA_LTPS	= 0x04,
	BBA_LRPS	= 0x05,

	BBA_IMR		= 0x08,
	BBA_IR		= 0x09,

	BBA_BP		= 0x0A,
	BBA_TLBP	= 0x0C,
	BBA_TWP		= 0x0E,
	BBA_IOB		= 0x10,
	BBA_TRP		= 0x12,
	BBA_RWP		= 0x16,
	BBA_RRP		= 0x18,
	BBA_RHBP	= 0x1A,

	BBA_RXINTT	= 0x14,

	// BBA MAC address
	BBA_NAFR_PAR0	= 0x20,
	BBA_NAFR_PAR1	= 0x21,
	BBA_NAFR_PAR2	= 0x22,
	BBA_NAFR_PAR3	= 0x23,
	BBA_NAFR_PAR4	= 0x24,
	BBA_NAFR_PAR5	= 0x25,

	BBA_NAFR_MAR0	= 0x26,
	BBA_NAFR_MAR1	= 0x27,
	BBA_NAFR_MAR2	= 0x28,
	BBA_NAFR_MAR3	= 0x29,
	BBA_NAFR_MAR4	= 0x2A,
	BBA_NAFR_MAR5	= 0x2B,
	BBA_NAFR_MAR6	= 0x2C,
	BBA_NAFR_MAR7	= 0x2D,

	BBA_NWAYC	= 0x30,
	BBA_NWAYS	= 0x31,

	BBA_GCA		= 0x32,

	BBA_MISC	= 0x3D,

	BBA_TXFIFOCNT	= 0x3E,
	BBA_WRTXFIFOD	= 0x48,

	BBA_MISC2	= 0x50,

	BBA_SI_ACTRL	= 0x5C,
	BBA_SI_STATUS	= 0x5D,
	BBA_SI_ACTRL2	= 0x60
} BBA_REG_e;

// BBA configuration.
#define BBA_RECV_SIZE	0x800
#define BBA_NUM_PAGES	0x10
#define BBA_PAGE_SIZE	0x100
#define BBA_MEM_SIZE	(BBA_NUM_PAGES * BBA_PAGE_SIZE)
#define BBA_TXFIFO_SIZE	1518

typedef enum
{
	DESC_CC0	= 0x01,
	DESC_CC1	= 0x02,
	DESC_CC2	= 0x04,
	DESC_CC3	= 0x08,
	DESC_CRSLOST	= 0x10,
	DESC_UF		= 0x20,
	DESC_OWC	= 0x40,
	DESC_OWN	= 0x80,
} SendStatus;

typedef enum
{
	DESC_BF		= 0x01,
	DESC_CRC	= 0x02,
	DESC_FAE	= 0x04,
	DESC_FO		= 0x08,
	DESC_RW		= 0x10,
	DESC_MF		= 0x20,
	DESC_RF		= 0x40,
	DESC_RERR	= 0x80,
} RecvStatus;

/** EXI registers for BBA. */
typedef enum {
	BBA_EXI_ID		= 0x00,
	BBA_REVISION_ID		= 0x01,
	BBA_INTERRUPT_MASK	= 0x02,
	BBA_INTERRUPT		= 0x03,
	BBA_DEVICE_ID		= 0x04,
	BBA_ACSTART		= 0x05,

	BBA_HASH_READ		= 0x08,
	BBA_HASH_WRITE		= 0x09,

	BBA_HASH_STATUS		= 0x0B,
	BBA_RESET		= 0x0F,
} BBA_EXI_Reg;

/** BBA context. **/

typedef enum {
	BBA_READ = 0,
	BBA_WRITE = 1,
} BBA_Direction;

typedef enum {
	BBA_EXI = 0,
	BBA_MX = 1,
} BBA_Region;

typedef struct _BBA_Ctx
{
	// Transfer status.
	struct {
		u8 direction;	// 0 = READ; 1 = WRITE
		u8 region;	// 0 = EXI; 1 = MX
		u16 address;
		u8 valid;	// 1 if a transfer is in progress; 0 if not.
	} transfer;

	// EXI status.
	struct {
		u8 revision_id;
		u8 interrupt_mask;
		u8 interrupt;
		u16 device_id;
		u8 acstart;
		u32 hash_challenge;
		u32 hash_response;
		u8 hash_status;
	} exi;

	u32 descriptor;

	// MX region.
	u8 tx_fifo[BBA_TXFIFO_SIZE];
	u8 bba_mem[BBA_MEM_SIZE];
	u8 recv_buffer[BBA_RECV_SIZE];
	u32 recv_buffer_len;
} BBA_Ctx;

#endif /* __BBA_TYPES_H__ */

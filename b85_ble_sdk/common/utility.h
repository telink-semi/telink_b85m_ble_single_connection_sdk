/********************************************************************************************************
 * @file     utility.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/

#pragma once
#include "types.h"

#define abs(a)   (((a)>0)?((a)):(-(a)))


#ifndef min
#define min(a,b)	((a) < (b) ? (a) : (b))
#endif

#ifndef min2
#define min2(a,b)	((a) < (b) ? (a) : (b))
#endif

#ifndef min3
#define min3(a,b,c)	min2(min2(a, b), c)
#endif

#ifndef max2
#define max2(a,b)	((a) > (b) ? (a): (b))
#endif

#ifndef max3
#define max3(a,b,c)	max2(max2(a, b), c)
#endif

#define OFFSETOF(s, m) 			((unsigned int) &((s *)0)->m)
#define ROUND_INT(x, r)			(((x) + (r) - 1) / (r) * (r))
#define ROUND_TO_POW2(x, r)		(((x) + (r) - 1) & ~((r) - 1))

//  direct memory access 
#define U8_GET(addr)			(*(volatile unsigned char  *)(addr))
#define U16_GET(addr)			(*(volatile unsigned short *)(addr))
#define U32_GET(addr)			(*(volatile unsigned int  *)(addr))

#define U8_SET(addr, v)			(*(volatile unsigned char  *)(addr) = (unsigned char)(v))
#define U16_SET(addr, v)		(*(volatile unsigned short *)(addr) = (unsigned short)(v))
#define U32_SET(addr, v)		(*(volatile unsigned int  *)(addr) = (v))

#define U8_INC(addr)			U8_GET(addr) += 1
#define U16_INC(addr)			U16_GET(addr) += 1
#define U32_INC(addr)			U32_GET(addr) += 1

#define U8_DEC(addr)			U8_GET(addr) -= 1
#define U16_DEC(addr)			U16_GET(addr) -= 1
#define U32_DEC(addr)			U32_GET(addr) -= 1

#define U8_CPY(addr1,addr2)		U8_SET(addr1, U8_GET(addr2))
#define U16_CPY(addr1,addr2)	U16_SET(addr1, U16_GET(addr2))
#define U32_CPY(addr1,addr2)	U32_SET(addr1, U32_GET(addr2))

#define MAKE_U16(h,l) 			((unsigned short)(((h) << 8) | (l)))
#define MAKE_U32(a,b,c,d)		((unsigned int)(((a) << 24) | ((b) << 16) | ((c) << 8) | (d)))

#define BOUND(x, l, m)			((x) < (l) ? (l) : ((x) > (m) ? (m) : (x)))
#define SET_BOUND(x, l, m)		((x) = BOUND(x, l, m))
#define BOUND_INC(x, m)			do{++(x); (x) = (x) < (m) ? (x) :0;} while(0)
#define BOUND_INC_POW2(x, m)	do{								\
									STATIC_ASSERT_POW2(m);		\
									(x) = ((x)+1) & (m-1);		\
								}while(0)

#define IS_POWER_OF_2(x)		(!(x & (x-1)))
#define IS_LITTLE_ENDIAN 		(*(unsigned short*)"\0\xff" > 0x100) 

#define IMPLIES(x, y) 			(!(x) || (y))

// x > y ? 1 : (x ==y : 0 ? -1)
#define COMPARE(x, y) 			(((x) > (y)) - ((x) < (y)))
#define SIGN(x) 				COMPARE(x, 0)

// better than xor swap:  http://stackoverflow.com/questions/3912699/why-swap-with-xor-works-fine-in-c-but-in-java-doesnt-some-puzzle
#define SWAP(x, y, T) 			do { T tmp = (x); (x) = (y); (y) = tmp; } while(0)
#define SORT2(a, b, T) 			do { if ((a) > (b)) SWAP((a), (b), T); } while (0)

#define foreach(i, n) 			for(int i = 0; i < (n); ++i)
#define foreach_range(i, s, e) 	for(int i = (s); i < (e); ++i)
#define foreach_arr(i, arr) 	for(int i = 0; i < ARRAY_SIZE(arr); ++i)

#define ARRAY_SIZE(a) 			(sizeof(a) / sizeof(*a))

#define everyN(i, n) 			++(i); (i)=((i) < N ? (i) : 0); if(0 == (i))

#define U16_HI(a)    (((a) >> 8) & 0xFF)
#define U16_LO(a)    ((a) & 0xFF)

#define U32_BYTE0(a) ((a) & 0xFF)
#define U32_BYTE1(a) (((a) >> 8) & 0xFF)
#define U32_BYTE2(a) (((a) >> 16) & 0xFF)
#define U32_BYTE3(a) (((a) >> 24) & 0xFF)



void swapN (unsigned char *p, int n);
void swapX(const u8 *src, u8 *dst, int len);

void swap24(u8 dst[3], const u8 src[3]);
void swap32(u8 dst[4], const u8 src[4]);
void swap48(u8 dst[6], const u8 src[6]);
void swap56(u8 dst[7], const u8 src[7]);
void swap64(u8 dst[8], const u8 src[8]);
void swap128(u8 dst[16], const u8 src[16]);


typedef	struct {
	u32		size;
	u16		num;
	u8		wptr;
	u8		rptr;
	u8*		p;
}	my_fifo_t;

void my_fifo_init (my_fifo_t *f, int s, u8 n, u8 *p);
u8*  my_fifo_wptr (my_fifo_t *f);
void my_fifo_next (my_fifo_t *f);
int  my_fifo_push (my_fifo_t *f, u8 *p, int n);
void my_fifo_pop (my_fifo_t *f);
u8 * my_fifo_get (my_fifo_t *f);

static inline u64 mul64_32x32(u32 u, u32 v)
{
#if 1
    u32  u0,   v0,   w0;
    u32  u1,   v1,   w1,   w2,   t;
    u32  x, y;

    u0   =   u & 0xFFFF;
    u1   =   u >> 16;
    v0   =   v & 0xFFFF;
    v1   =   v >> 16;
    w0   =   u0 * v0;
    t    =   u1 * v0 + (w0 >> 16);
    w1   =   t & 0xFFFF;
    w2   =   t >> 16;
    w1   =   u0 * v1 + w1;

    //x is high 32 bits, y is low 32 bits

    x = u1 * v1 + w2 + (w1 >> 16);
    y = u * v;


    return(((u64)x << 32) | y);
#else //Eagle HW support this process
    return (u64)u*v;
#endif
}


#define		MYFIFO_INIT(name,size,n)		u8 name##_b[size * n]={0};my_fifo_t name = {size,n,0,0, name##_b}
#define		ATT_ALLIGN4_DMA_BUFF(n)			(((n + 10) + 3) / 4 * 4)


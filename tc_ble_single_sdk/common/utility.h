/********************************************************************************************************
 * @file    utility.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
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

#ifndef max
#define max(a,b)	((a) > (b) ? (a): (b))
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
#define IS_4BYTE_ALIGN(x) 		(!(x & 3))
#define IS_16BYTE_ALIGN(x) 		(!(x & 15))

#define	DATA_LENGTH_ALIGN4(n)	(((n) + 3) / 4 * 4)
#define	DATA_LENGTH_ALIGN16(n)	(((n) + 15) / 16 * 16)


#define IMPLIES(x, y) 			(!(x) || (y))

// x > y ? 1 : (x ==y : 0 ? -1)
#define COMPARE(x, y) 			(((x) > (y)) - ((x) < (y)))
#define SIGN(x) 				COMPARE(x, 0)

// better than xor swap:  http://stackoverflow.com/questions/3912699/why-swap-with-xor-works-fine-in-c-but-in-java-doesn't-some-puzzle
#define SWAP(x, y, T) 			do { T tmp = (x); (x) = (y); (y) = tmp; } while(0)
#define SORT2(a, b, T) 			do { if ((a) > (b)) SWAP((a), (b), T); } while (0)

#define foreach(i, n) 			for(int i = 0; i < (n); ++i)
#define foreach_range(i, s, e) 	for(int i = (s); i < (e); ++i)
#define foreach_arr(i, arr) 	for(unsigned int i = 0; i < ARRAY_SIZE(arr); ++i)

#define ARRAY_SIZE(a) 			(sizeof(a) / sizeof(*a))

#define everyN(i, n) 			++(i); (i)=((i) < N ? (i) : 0); if(0 == (i))

#define U16_HI(a)    (((a) >> 8) & 0xFF)
#define U16_LO(a)    ((a) & 0xFF)

#define U32_BYTE0(a) ((a) & 0xFF)
#define U32_BYTE1(a) (((a) >> 8) & 0xFF)
#define U32_BYTE2(a) (((a) >> 16) & 0xFF)
#define U32_BYTE3(a) (((a) >> 24) & 0xFF)


#define U16_TO_BYTES(n)			((u8) (n)), ((u8)((n) >> 8))
#define U24_TO_BYTES(n)			((u8) (n)),	((u8)((n) >> 8)), ((u8)((n) >> 16))
#define U32_TO_BYTES(n)			((u8) (n)),	((u8)((n) >> 8)), ((u8)((n) >> 16)), ((u8)((n) >> 24))

#define BYTE_TO_UINT16(n, p)	{n = ((u16)(p)[0] + ((u16)(p)[1]<<8));}
#define BYTE_TO_UINT24(n, p)	{n = ((u32)(p)[0] + ((u32)(p)[1]<<8) + \
									((u32)(p)[2]<<16));}
#define BYTE_TO_UINT32(n, p)	{n = ((u32)(p)[0] + ((u32)(p)[1]<<8) + \
									((u32)(p)[2]<<16) + ((u32)(p)[3]<<24));}

#define STREAM_TO_U8(n, p)		{n = *(p); p++;}
#define STREAM_TO_U16(n, p)		{BYTE_TO_UINT16(n,p); p+=2;}
#define STREAM_TO_U24(n, p)		{BYTE_TO_UINT24(n,p); p+=3;}
#define STREAM_TO_U32(n, p)		{BYTE_TO_UINT32(n,p); p+=4;}
#define STREAM_TO_STR(n, p, l)	{memcpy(n, p, l); p+=l;}

#define U8_TO_STREAM(p, n)		{*(p)++ = (u8)(n);}
#define U16_TO_STREAM(p, n)		{*(p)++ = (u8)(n); *(p)++ = (u8)((n)>>8);}
#define U24_TO_STREAM(p, n)		{*(p)++ = (u8)(n); *(p)++ = (u8)((n)>>8); \
								*(p)++ = (u8)((n)>>16);}
#define U32_TO_STREAM(p, n)		{*(p)++ = (u8)(n); *(p)++ = (u8)((n)>>8); \
								*(p)++ = (u8)((n)>>16); *(p)++ = (u8)((n)>>24);}
#define U40_TO_STREAM(p, n)		{*(p)++ = (u8)(n); *(p)++ = (u8)((n)>>8); \
								*(p)++ = (u8)((n)>>16); *(p)++ = (u8)((n)>>24); \
								*(p)++ = (u8)((n)>>32);}

#define STR_TO_STREAM(p, n, l)	{memcpy(p, n, l); p+=l;}


void swapN (unsigned char *p, int n);
void swapX(const u8 *src, u8 *dst, int len);

void swap24(u8 dst[3], const u8 src[3]);
void swap32(u8 dst[4], const u8 src[4]);
void swap48(u8 dst[6], const u8 src[6]);
void swap56(u8 dst[7], const u8 src[7]);
void swap64(u8 dst[8], const u8 src[8]);
void swap128(u8 dst[16], const u8 src[16]);


typedef	struct __attribute__((aligned(4))){
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


#define		MYFIFO_INIT(name,size,n)		u8 name##_b[(size) * (n)]={0};my_fifo_t name = {size,n,0,0, name##_b}
#define		ATT_ALIGN4_DMA_BUFF(n)			(((n + 10) + 3) / 4 * 4)

const char *hex_to_str(const void *buf, u8 len);


#ifndef __COMMON_H__
#define __COMMON_H__
typedef bit                   BIT;
typedef unsigned char         uint8_t;
typedef unsigned int          uint16_t;
typedef unsigned long         uint32_t;

typedef signed char         int8_t;
typedef signed int          int16_t;
typedef signed long         int32_t;


#define     CID_READ				0x0B
#define     DID_READ				0x0C

#define     ERASE_APROM				0x22
#define     READ_APROM				0x00
#define     PROGRAM_APROM			0x21
#define     ERASE_LDROM				
#define     READ_LDROM				
#define     PROGRAM_LDROM			
#define     READ_CFG					0xC0
#define     PROGRAM_CFG				0xE1
#define			READ_UID					0x04



extern bit BIT_TMP;
#endif
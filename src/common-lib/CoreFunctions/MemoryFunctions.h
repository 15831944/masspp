/**
 * @file MemoryFunctions.h
 * @brief interfaces of function which deals with memory
 *
 * @author S.Tanaka
 * @date 2006.07.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CORE_MEMORY_FUNCTIONS_H__
#define __KOME_CORE_MEMORY_FUNCTIONS_H__


/**
 * @fn void fillarray( void* p, void* value, size_t size, size_t n )
 * @brief This function assigns the specified value to each element.
 * @param[out] p array
 * @param[in] value specified value
 * @param[in] size size of element
 * @param[in] n size of array
 */
void fillarray( void* p, void* value, size_t size, size_t n );

/**
 * @fn void fillzero( void* p, size_t n )
 * @brief This function fills a block of memory zeros.
 * @param[out] p pointer to starting addres of the block of memory to fill with zeros.
 * @param[in] n size of the block of memory to fill with zeros.
 */
void fillzero( void* p, size_t n );

/**
 * @fn bool isbigendian()
 * @brief judges whether the system is big endian
 * @return If the system is big endian, this function returns true.
 */
bool isbigendian();

/**
 * @fn bool islittleendian()
 * @brief gets the system is little endian
 * @return If the system is little endian, this function returns true.
 */
bool islittleendian();

/**
 * @fn void memreverse( void* p, size_t n )
 * @brief reverse memory (ex. Big Endian <-> Little Endian)
 * @param[in,out] p the address of memory
 * @param[in] n memory size
 */
void memreverse( void* p, size_t n );

/**
 * @fn void memswap( void* p0, void* p1, size_t n )
 * @brief swaps memory ( p0 <-> p1 )
 * @param[in,out] p0 the address of memory to be swapped
 * @param[in,out] p1 the address of memory to swap
 * @param[in] n memory size
 */
void memswap( void* p0, void* p1, size_t n );


#endif		// __KOME_CORE_MEMORY_FUNCTIONS_H__

/*
 * eeprom.h
 *
 *  Created on: Mar 2, 2026
 *      Author: Navumchyk
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "stm32g030xx.h"
#include "stm32g0xx_hal.h"

#define PAGE_SIZE 		0x800UL
#define WORD_SIZE 		0x8UL
#define PAGE0 			15
#define PAGE0_START_ADR (FLASH_BASE + PAGE_SIZE * (PAGE0 - 1))
#define PAGE0_VERSION 	(PAGE0_START_ADR + WORD_SIZE)
#define PAGE1 			16
#define PAGE1_START_ADR (FLASH_BASE + PAGE_SIZE * (PAGE1 - 1))
#define PAGE1_VERSION 	(PAGE1_START_ADR + WORD_SIZE)

#define ERASED 	0xFFFFFFFFFFFFFFFFULL
#define VALID 	0x0000000000000000ULL
#define RECEIVE 0x5555555555555555ULL
//#define VALID 	0x5555555555555555ULL
//#define RECEIVE 0x0000000000000000ULL

#define EMPTY 	0xFFFFFFFFFFFFFFFFULL

typedef enum
{
	EE_ERROR = 0,
	EE_OK
}eeprom_state;

void initEEPROM();

eeprom_state findFreeCell(uint32_t *free_address);
void pageSwap();
void choose_true_valid_page();
void transfer(uint32_t from_start, uint32_t to_start);
void eeWrite(uint16_t data, uint8_t data_id);
void Flash_Erase_Page(uint32_t page);
void Flash_Write64(uint32_t address, uint64_t data);
uint16_t eeRead(uint8_t id);


//void writeData(uint64_t data, uint32_t address);


#endif /* INC_EEPROM_H_ */

/*
 * eeprom.c
 *
 *  Created on: Mar 2, 2026
 *      Author: Navumchyk
 */

#include "eeprom.h"
static uint32_t activePage;
static uint32_t p0_version;
static uint32_t p1_version;

void initEEPROM()
{
  uint64_t p0 = *(uint64_t*)PAGE0_START_ADR;
  uint64_t p1 = *(uint64_t*)PAGE1_START_ADR;
//  p0_version = *(uint32_t*)PAGE0_VERSION;
//  p1_version = *(uint32_t*)PAGE1_VERSION;

  if(p0 == ERASED &&  p1 == ERASED) // init
  {
    Flash_Write64(PAGE0_START_ADR, VALID);
    activePage = PAGE0;
    p0_version = 1;
    p1_version = 0;
    Flash_Write64(PAGE0_VERSION, p0_version);
    Flash_Write64(PAGE1_VERSION, p1_version);

  }
  else if(p0 == ERASED && p1 == VALID) // OK
  {
    activePage = PAGE1;
    return;
  }
  else if(p0 == VALID && p1 == ERASED)  // OK
  {
    activePage = PAGE0;
    return;
  }
  else if(p0 == VALID && p1 == RECEIVE) // transfer
  {
	  transfer(PAGE0_START_ADR + WORD_SIZE * 2, PAGE1_START_ADR + WORD_SIZE * 2);
  }
  else if(p0 == RECEIVE && p1 == VALID) // transfer
  {
	  transfer(PAGE1_START_ADR + WORD_SIZE * 2, PAGE0_START_ADR + WORD_SIZE * 2);
  }
  else if(p0 == VALID && p1 == VALID) // choose a true-valid page
  {
    choose_true_valid_page();
  }
  else
  {
    Flash_Erase_Page(PAGE0-1);
    Flash_Erase_Page(PAGE1-1);
    Flash_Write64(PAGE0_START_ADR, VALID);
    activePage = PAGE0;

    p0_version = 1;
    p1_version = 0;
    Flash_Write64(PAGE0_VERSION, (uint64_t)p0_version);
    Flash_Write64(PAGE1_VERSION, (uint64_t)p1_version);
  }
}

void choose_true_valid_page()
{

	  p0_version = *(uint32_t*)PAGE0_VERSION;
	  p1_version = *(uint32_t*)PAGE1_VERSION;

	  if(p0_version > p1_version)
	  {
		  activePage = PAGE0;
		  Flash_Erase_Page(PAGE1-1);
	  }
	  else
	  {
		  activePage = PAGE1;
		  Flash_Erase_Page(PAGE0-1);
	  }
}

uint32_t calc_CRC(uint32_t data)
{
    CRC->CR = CRC_CR_RESET;
    CRC->DR = data;   // подаём сразу 32 бита
    return CRC->DR;
}

void Flash_Erase_Page(uint32_t page)
{
    FLASH_EraseInitTypeDef erase;
    uint32_t PageError = 0;

    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.Page      = page;        // номер страницы
    erase.NbPages   = 1;           // стираем одну

    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&erase, &PageError);
    HAL_FLASH_Lock();
}


void Flash_Write64(uint32_t address, uint64_t data)
{
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data);
    HAL_FLASH_Lock();
}

eeprom_state findFreeCell(uint32_t *free_address)
{
  uint32_t start, end;
  if(activePage == PAGE0)
  {
    start = PAGE0_START_ADR + 0x10;
    end = PAGE0_START_ADR + PAGE_SIZE;
  }
  else
  {
    start = PAGE1_START_ADR + 0x10;
    end = PAGE1_START_ADR + PAGE_SIZE;
  }

	for(uint32_t address = start; address < end; address += WORD_SIZE)
	{
		if(*(uint64_t*)address == 0xFFFFFFFFFFFFFFFFULL)
		{
			*free_address = address;
			return EE_OK;
		}
	}
  return EE_ERROR;
}

void pageSwap()
{
  uint32_t receiving_address, start_page;
  uint32_t status_new_address;
  uint8_t nextPage;
  uint32_t page_version;
  if(activePage == PAGE0)
  {
    Flash_Write64(PAGE1_START_ADR, RECEIVE);

    start_page = PAGE0_START_ADR + (WORD_SIZE * 2);
    receiving_address = PAGE1_START_ADR + (WORD_SIZE * 2);
    status_new_address = PAGE1_START_ADR;
    nextPage = PAGE1;
    page_version = *(uint64_t*)(PAGE0_START_ADR + WORD_SIZE);
  }
  else
  {
    Flash_Write64(PAGE0_START_ADR, RECEIVE);
    start_page = PAGE1_START_ADR + (WORD_SIZE * 2);
    receiving_address = PAGE0_START_ADR + (WORD_SIZE * 2);
    status_new_address = PAGE0_START_ADR;
    nextPage = PAGE0;
    page_version =  *(uint64_t*)(PAGE1_START_ADR + WORD_SIZE);
  }

  transfer(start_page, receiving_address);
  Flash_Write64(status_new_address + WORD_SIZE, page_version + 1);
  uint8_t oldPage = activePage;
  activePage = nextPage;
  Flash_Write64(status_new_address, VALID);
  Flash_Erase_Page(oldPage-1);
}

void transfer(uint32_t from_start, uint32_t to_start)
{
  uint32_t from_end = from_start + PAGE_SIZE - (WORD_SIZE * 2);
  uint32_t where = to_start;
  uint32_t to_end = to_start + PAGE_SIZE - (WORD_SIZE * 2);
  uint64_t data_buffer;
  uint32_t stored_crc, data_and_id;
  uint8_t data_id;
  uint32_t calcCRC;
  uint32_t bitset[8] = {0};

  for(uint32_t i = from_end; i > from_start; )
  {
    i -= WORD_SIZE;
    data_buffer = *(volatile const uint64_t*)i;
    if( data_buffer != EMPTY)
    {
      stored_crc = (uint32_t)(data_buffer >> 32);
      data_and_id = (uint32_t)(data_buffer & 0xFFFFFFFFU);
      data_id = data_and_id >> 24;
      calcCRC = calc_CRC(data_and_id);

      const uint32_t idx  = (uint32_t)(data_id >> 5);       // /32
      const uint32_t mask = 1u << (data_id & 31);           // %32

      if(stored_crc == calcCRC)
      {
        if((bitset[idx] & mask) == 0U) //проверяет, установлен ли бит: если установлен - будет 1, а т.к. инверсия, то будет 0)
        {
            bitset[idx] |= mask;
                if (where > (to_end - WORD_SIZE)) // Контроль переполнения целевой страницы
                    break;
          Flash_Write64(where, data_buffer);
          where += WORD_SIZE;
        }
      }
      else
        continue;
    }
  }
}

void eeWrite(uint16_t data, uint8_t id)
{
    uint32_t data_id = ((uint32_t)id << 24 | data);
    uint32_t crc = calc_CRC(data_id);
    uint64_t packet = ((uint64_t)crc << 32 | data_id);

    uint32_t freeAddress;
    if(findFreeCell(&freeAddress) == EE_OK)
    {
    	Flash_Write64(freeAddress, packet);
    }
    else
    {
    	pageSwap();
    }
}

uint16_t eeRead(uint8_t id)
{
	uint64_t data_buffer;
	uint32_t stored_crc, data_and_id, calcCRC;
	uint8_t data_id;

	uint32_t start_page, end_page;

	if(activePage == PAGE0)
	{
		start_page = PAGE0_START_ADR + (2 * WORD_SIZE);
		end_page = PAGE0_START_ADR + PAGE_SIZE;
	}
	else
	{
		start_page = PAGE1_START_ADR + (2 * WORD_SIZE);
		end_page = PAGE1_START_ADR + PAGE_SIZE;
	}

	for(uint32_t id_address = end_page; id_address > start_page;)
	{
		id_address -= WORD_SIZE;
		data_buffer = *(uint64_t*)id_address;
		if( data_buffer == EMPTY)
			continue;
	    stored_crc = (uint32_t)(data_buffer >> 32);
	    data_and_id = (uint32_t)(data_buffer & 0xFFFFFFFFU);
	    data_id = data_and_id >> 24;
	    if(data_id == id)
	    {
		    calcCRC = calc_CRC(data_and_id);
		    if(calcCRC == stored_crc)
		    	return (uint16_t)(data_and_id &0xFFFF);
	    }


	}
	return 0;

}

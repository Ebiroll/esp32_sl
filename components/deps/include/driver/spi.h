// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef _DRIVER_SPI_H_
#define _DRIVER_SPI_H_
#include "spi.h"
#include "soc/spi_reg.h"
#include <esp_types.h>
#include "dma.h"

#define SPI_USR_ADDR_VALUE  0xFFFFFFFF
#define SPI_USR_ADDR_VALUE_M  ((SPI_USR_ADDR_VALUE_V)<<(SPI_USR_ADDR_VALUE_S))
#define SPI_USR_ADDR_VALUE_V  0xFFFFFFFF
#define SPI_USR_ADDR_VALUE_S  0

//*****************************************************************************
//
// Make sure all of the definitions in this header have a C binding.
//
//*****************************************************************************

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Support SPI1/SPI2/SPI3 module.
 *
 */
typedef enum {
    SpiNum_SPI1  = 0x1,
    SpiNum_SPI2  = 0x2,
    SpiNum_SPI3  = 0x3,
    SpiNum_Max   = SpiNum_SPI3,
} spi_num_e;

/**
 * @brief The SPI module can work in either master or slave mode.
 *
 */
typedef enum {
    SpiMode_Master = 0,
    SpiMode_Slave  = 1,
} spi_mode_e;

/**
 *  @brief SPI sub mode
 *
 * Support 4 sub modes based on SPI clock polarity and phase.
 * SPI_CPOL SPI_CPHA  SubMode
 *   0        0        0
 *   0        1        1
 *   1        0        2
 *   1        1        3
 */
typedef enum {
    SpiSubMode_0 = 0,
    SpiSubMode_1 = 1,
    SpiSubMode_2 = 2,
    SpiSubMode_3 = 3,
} spi_sub_mode_e;

/**
 * @brief The SPI module working speed.
 *
 * @attention Max speed 80MHz
 *
 */
typedef enum {
    SpiSpeed_2MHz  = 40,
    SpiSpeed_5MHz  = 16,
    SpiSpeed_8MHz  = 10,
    SpiSpeed_10MHz = 8,
    SpiSpeed_16MHz = 5,
    SpiSpeed_20MHz = 4,
} spi_speed_e;

/**
 * @brief The SPI mode working speed.
 *
 */
typedef enum {
    SpiBitOrder_MSBFirst = 0,
    SpiBitOrder_LSBFirst = 1,
} spi_bit_order_e;

// @brief SPI interrupt source defined.
typedef enum {
    SpiIntSrc_TransDone = SPI_TRANS_DONE,
    SpiIntSrc_WrStaDone = SPI_SLV_WR_STA_DONE,
    SpiIntSrc_RdStaDone = SPI_SLV_RD_STA_DONE,
    SpiIntSrc_WrBufDone = SPI_SLV_WR_BUF_DONE,
    SpiIntSrc_RdBufDone = SPI_SLV_RD_BUF_DONE,
    SpiIntSrc_OneBufRecvDone = SPI_IN_SUC_EOF_INT_ENA,
    SpiIntSrc_OneBufSendDone = SPI_OUT_EOF_INT_ENA,
} spi_int_src_e;

/**
 * @brief The SPI mode working speed.
 *
 */
typedef enum {
    SpiWorkMode_Half = 0,
    SpiWorkMode_Full = 1,
} spi_work_mode_e;

/**
 * @brief The SPI DMA channels.
 *
 */
typedef enum {
    SpiDMAchannel_0 = 1,
    SpiDMAchannel_l = 2,
} spi_dma_channel_e;

/**
 * @brief The SPI dma direction.
 *
 */
typedef enum {
 //   SpiDmaDir_Default   = 0,
    SpiDmaDir_In        = 1,
    SpiDmaDir_Out       = 2,
} spi_dma_dir_e;

#pragma pack(1)
/**
 * @brief SPI attribute
 */
typedef struct {
    spi_mode_e          mode;           ///< Master or slave mode
    spi_sub_mode_e      subMode;        ///< SPI SPI_CPOL SPI_CPHA mode
    spi_speed_e         speed;          ///< SPI Clock
    spi_bit_order_e     bitOrder;       ///< SPI bit order
    spi_work_mode_e     halfMode;       ///< Half or Full work mode
} spi_attr_t;

/**
 * @brief SPI attribute
 */
typedef struct {
    uint16_t    cmd;                ///< Command value
    uint8_t     cmdLen;             ///< Command byte length
    uint32_t    *addr;              ///< Point to address value
    uint8_t     addrLen;            ///< Address byte length
    uint32_t    *txData;            ///< Point to send data buffer
    uint8_t     txDataLen;          ///< Send data byte length.
    uint32_t    *rxData;            ///< Point to recevie data buffer
    uint8_t     rxDataLen;          ///< Recevie Data byte length.
} spi_data_t;

// SPI DMA attribite
typedef struct {
    spi_num_e          spiNum;     ///< Spi number
    uint32_t          mode;       ///< Spi work mode
    uint32_t          bufSize;    ///< Size of buffer
    spi_dma_dir_e       dir;        ///< Direction of data,in or out.
    spi_dma_channel_e   channel;    ///< Channel of DMA
    ping_pong_buf_t     *buf;       ///< Buffer address
} spi_dma_attr_t;
#pragma pack()

#if 1
// Waiting for test
/**
 * @brief Get SPI ping buffer address.
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return uint32_t*, NULL:indicates parameter error, others indicates ping buffer address.
 */
uint32_t* spi_dma_ping_buf_get(spi_dma_attr_t *obj);

/**
 * @brief Get SPI pong buffer address.
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return uint32_t*, NULL:indicates parameter error, others indicates pong buffer address.
 */
uint32_t* spi_dma_pong_buf_get(spi_dma_attr_t *obj);

/**
 * @brief Get without working ping-pong buffer address.
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return uint32_t*, NULL:indicates parameter error, others indicates not working buffer address.
 */
uint32_t* spi_dma_status_get(spi_dma_attr_t *obj);

/**
 * @brief Configrate the Ping-Pong buffer to the destination
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return None.
 */
int32_t spi_dma_memcpy(spi_dma_attr_t *obj, void *src, uint32_t len);

/**
 * @brief Configrate the Ping-Pong buffer to the destination
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return None.
 */
void spi_dma_dest_add_set(spi_dma_attr_t *obj);

/**
 * @brief Reset the SPI DMA instance
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 * @param [in] isr
 *             Indicates which function to be called, when interrupt hanpened.
 *
 * @return None.
 */
void spi_dma_rest(spi_dma_attr_t *obj);

/**
 * @brief Initialize DMA and create a SPI DMA instance
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 * @param [in] isr
 *             Indicates which function to be called, when interrupt hanpened.
 *
 * @return -1:indicates parameter error, -2: DMA buffer init failure, others indicates success.
 */
int spi_dma_init(spi_dma_attr_t *obj, void *isr);

/**
 * @brief Destroy the SPI DMA instance
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return int, -1:indicates parameter error, others indicates success.
 */
int spi_dma_uninit(spi_dma_attr_t *obj);

/**
 * @brief Configrate the Ping-Pong buffer to the destination
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return None.
 */
void spi_dma_int_enable(spi_num_e spiNum, spi_int_src_e intSrc);

/**
 * @brief Configrate the Ping-Pong buffer to the destination
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return None.
 */
void spi_dma_int_disable(spi_num_e spiNum, spi_int_src_e intSrc);

/**
 * @brief Configrate the Ping-Pong buffer to the destination
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return None.
 */
void spi_dma_int_clear(spi_num_e spiNum);

/**
 * @brief Configrate the Ping-Pong buffer to the destination
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return None.
 */
int32_t spi_dma_int_status_get(spi_num_e spiNum);

/**
 * @brief Start e SPI DMA instance
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 * @param [in] len
 *             Indicates how many data to be called.
 *
 * @return None.
 */
void spi_dma_start(spi_dma_attr_t *obj, uint32_t len);

/**
 * @brief Stop the SPI DMA transmit
 *
 * @param [in] obj
 *             Pointer to a struct spi_dma_attr_t that indicates SPI DMA working attribution.
 *
 * @return None.
 */
void spi_dma_stop(spi_dma_attr_t *obj);

#endif

/**
 * @brief Initialize SPI module.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 * @param [in] pAttr
 *              Pointer to a struct spi_attr_t that indicates SPI working attribution.
 *
 * @return void.
 */
void spi_init(spi_num_e spiNum, spi_attr_t* pAttr);

/**
 * @brief Set slave address value by master.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 * @param [in] addr
 *             Slave address to be set.
 *
 * @return void.
 */
void spi_master_cfg_addr(spi_num_e spiNum, uint32_t addr);

/**
 * @brief Set command value by master.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 * @param [in] cmd
 *             Command will be send to slave.
 *
 * @return void.
 */
void spi_master_cfg_cmd(spi_num_e spiNum, uint32_t cmd);

/**
 * @brief Send data to slave from master.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 * @param [in] pInData
 *             Pointer to a spi_data_t strcuture that indicates data, address and command will be send.
 *
 * @return int, -1:indicates failure,others indicates success.
 */
int spi_master_send_data(spi_num_e spiNum, spi_data_t* pInData);

/**
 * @brief Receive data from slave by master.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 * @param [in] pOutData
 *             Point to data buffer.
 *
 * @return int, -1:indicates failure,others indicates success.
 *
 */
int spi_master_recv_data(spi_num_e spiNum, spi_data_t* pData);

/**
 * @brief Load data to slave send buffer.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 * @param [in] pOutData
 *             Point to data buffer.
 * @param [in] outLen
 *             The number of bytes to be set.
 *
 * @return int, -1:indicates failure,others indicates success.
 */
int spi_slave_send_data(spi_num_e spiNum, uint32_t *pOutData, uint8_t outLen);

/**
 * @brief Receive data by slave.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 * @param [in] isrFunc
 *             isrFunc is a pointer to the function to be called when the SPI interrupt occurs.
 *
 * @return int, -1:indicates failure,others indicates success.
 */
int spi_slave_recv_data(spi_num_e spiNum, void(*isrFunc)(void*));

/**
 * @brief Set slave status by master.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 *
 * @param [in] data
 *             Data will be write to slave SPI_WR_STATUS.
 *
 * @return void.
 */
void spi_master_send_status(spi_num_e spiNum, uint8_t data);

/**
 * @brief Get salve status by master.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 *
 * @return int, -1: indicates failure; other value in slave status.
 */
int spi_master_recv_status(spi_num_e spiNum);

/**
 * @brief Enable SPI module interrupt source.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 *
 * @param [in] intSrc
 *              Indicates which interrupt source to enable.
 *
 * @return void.
 */
void spi_int_enable(spi_num_e spiNum, spi_int_src_e intSrc);

/**
 * @brief Disable SPI module interrupt source.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 *
 * @param [in] intSrc
 *              Indicates which interrupt source to disable.
 *
 * @return void.
 */
void spi_int_disable(spi_num_e spiNum, spi_int_src_e intSrc);

/**
 * @brief Clear all of spi interrupt.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 *
 * @return void.
 */
void spi_int_clear(spi_num_e spiNum);

/**
 * @brief Get the spi interrupt status.
 *
 * @param [in] spiNum
 *             Indicates which submode to be used, SPI or HSPI.
 *
 * @return void.
 */
int32_t spi_int_status_get(spi_num_e i2sNum);

#ifdef __cplusplus
}
#endif

#endif //  __SPI_H__

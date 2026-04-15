#ifndef __KEY_H
#define __KEY_H	 
#include "main.h"

/**
 * @file   key_config.h
 * @date   2025-03-20
 * @version 1.0
 * @brief  按键配置与状态管理
 * 
 * 该文件用于定义按键的硬件端口、引脚及其相关状态结构体，支持按键检测、
 * 消抖处理，并区分短按和长按事件。
 * 
 */

/************************** 按键引脚定义 **************************/

#define KEY1_PORT  KEY1_GPIO_Port   ///< 按键1 所在的 GPIO 端口
#define KEY1_PIN   KEY1_Pin  ///< 按键1 连接的引脚号

#define KEY2_PORT  KEY2_GPIO_Port   ///< 按键2 所在的 GPIO 端口
#define KEY2_PIN   KEY2_Pin  ///< 按键2 连接的引脚号

#define KEY3_PORT  KEY3_GPIO_Port   ///< 按键3 所在的 GPIO 端口
#define KEY3_PIN   KEY3_Pin  ///< 按键3 连接的引脚号

#define KEY4_PORT  KEY4_GPIO_Port   ///< 按键4 所在的 GPIO 端口
#define KEY4_PIN   KEY4_Pin  ///< 按键4 连接的引脚号

/************************** 按键 GPIO 结构体 **************************/

/**
 * @struct key_gpio_t
 * @brief  按键硬件引脚配置结构体
 * 
 * 该结构体用于存储每个按键的 GPIO 端口、引脚编号及按键按下的状态
 * 
 */
typedef struct
{
    GPIO_TypeDef* port;          ///< GPIO 端口 (如 GPIOA、GPIOB)
    uint16_t      pin;           ///< GPIO 引脚号
    uint16_t      pressed_state; ///< 按键按下时的状态 (0：低电平按下, 1：高电平按下)
} key_gpio_t;

/************************** 按键状态枚举 **************************/

/**
 * @enum key_status_t
 * @brief  按键状态枚举类型
 * 
 * 该枚举定义了按键的不同状态，包括释放、消抖、短按和长按。
 */
typedef enum
{
    KEY_RELEASE,         ///< 按键松开
    KEY_CONFIRM,         ///< 消抖确认状态
    KEY_SHORT_PRESSED,   ///< 短按（按下时间较短）
    KEY_LONG_PRESSED,    ///< 长按（按下时间较长）
} key_status_t;

/************************** 按键事件枚举 **************************/

/**
 * @enum key_event_t
 * @brief  按键事件类型
 * 
 * 该枚举定义了按键可能触发的事件，如短按、长按等。
 */
typedef enum
{
    EVENT_NULL,           ///< 无事件
    EVENT_SHORT_PRESSED,  ///< 触发短按事件
    EVENT_LONG_PRESSED,   ///< 触发长按事件
} key_event_t;

/************************** 按键参数结构体 **************************/

/**
 * @struct key_param_t
 * @brief  按键状态参数结构体
 * 
 * 该结构体存储了按键的当前状态、按下时间及触发的事件信息。
 */
typedef struct
{
    key_status_t current_state; ///< 按键当前状态
    uint32_t  pressed_time;     ///< 按键按下持续时间（用于区分短按和长按）
    key_event_t key_event;      ///< 按键事件类型（短按、长按等）
} key_param_t;

uint8_t key_scan(void);
void key_handle(void);

#endif

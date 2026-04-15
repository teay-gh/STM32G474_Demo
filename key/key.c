#include "key.h"
#include "hrtim.h"

/**
 * @file   key_handler.c
 * @date   2025-03-20
 * @version 1.0
 * @brief  按键读取与状态管理
 * 
 * 该文件用于定义按键的硬件端口、引脚及其状态存储，同时提供按键读取函数，
 * 以实现按键按下和释放状态的检测。
 * 
 */

/************************** 按键列表定义 **************************/

/**
 * @brief 按键硬件信息列表
 * 
 * 该列表存储了系统中所有按键的 GPIO 配置，包括端口、引脚号和按键按下的状态。
 */
key_gpio_t key_list[] =
{
    {KEY1_PORT, KEY1_PIN, 0},  ///< 按键1 (低电平按下)
    {KEY2_PORT, KEY2_PIN, 0},  ///< 按键2 (低电平按下)
    {KEY3_PORT, KEY3_PIN, 0},  ///< 按键3 (低电平按下)
    {KEY4_PORT, KEY4_PIN, 0},  ///< 按键4 (低电平按下)
};

/************************** 宏定义 **************************/
#define KEY_NUM_MAX      (sizeof(key_list)/sizeof(key_list[0])) ///< 按键总数计算
#define KEY_INTERRUPT_FREQ_HZ   1000  ///< 按键扫描/中断频率，单位 Hz
#define CONFIRM_TIME     KEY_INTERRUPT_FREQ_HZ*0.02     ///< 消抖确认时间20ms（单位：ms）
#define LONG_PRESS_TIME  KEY_INTERRUPT_FREQ_HZ*1.5    ///< 长按判定时间1500ms（单位：ms）
#define SHORT_RELEASE_VALID  1                     ///< 短按释放有效标志
#define LONG_RELEASE_VALID   1                    ///< 长按释放有效标志
/************************** 按键参数存储 **************************/

/**
 * @brief 按键状态参数数组
 * 
 * 该数组用于存储每个按键的状态信息，包括当前状态、按下时间和触发的事件类型。
 */
key_param_t key_param[KEY_NUM_MAX];

/************************** 按键读取函数 **************************/

/**
 * @brief  读取指定按键的当前状态
 * 
 * @param  index  按键索引（范围：0 ~ KEY_NUM_MAX-1）
 * @return uint8_t  按键状态 (1: 按键按下, 0: 按键未按下)
 * 
 * 该函数通过读取 GPIO 引脚的电平值，判断按键是否被按下。
 */
uint8_t read_key_state(uint8_t index)
{
    if(HAL_GPIO_ReadPin(key_list[index].port, key_list[index].pin) == key_list[index].pressed_state)
    {
        return 1; 
    }
    return 0;
}
/**
 * @brief  按键扫描函数
 * 
 * 该函数循环遍历所有按键，并根据当前状态机更新按键状态。支持短按、长按识别，并可根据配置触发相应的按键事件。
 * 
 * @return uint8_t 固定返回 0
 */
uint8_t key_scan(void)
{
    uint8_t key_press;
    uint8_t index;

    for(index = 0; index < KEY_NUM_MAX; index++)
    {
        key_press = read_key_state(index); 

        switch (key_param[index].current_state)                                    
        {
            case KEY_RELEASE:{
                if(key_press)                                                                                                 
                {
                    key_param[index].current_state = KEY_CONFIRM;
                }
                else
                {
                    key_param[index].pressed_time = 0; 
                }
                break;
            }
            case KEY_CONFIRM:{
                if(key_press)
                {
                    if(++key_param[index].pressed_time > CONFIRM_TIME)  
                    {
                        key_param[index].current_state = KEY_SHORT_PRESSED;
					#if (SHORT_RELEASE_VALID == 0)  
						key_param[index].key_event = EVENT_SHORT_PRESSED;  
					#endif
                    }
                }
                else
                {
                    key_param[index].current_state = KEY_RELEASE;
                }
                break;
            }
            case KEY_SHORT_PRESSED:{
                if(key_press)
                {
                    if(++key_param[index].pressed_time > LONG_PRESS_TIME) 
                    {
                        key_param[index].current_state = KEY_LONG_PRESSED;
					#if (LONG_RELEASE_VALID == 0)  
						key_param[index].key_event = EVENT_LONG_PRESSED;  
					#endif
                    }
                }
                else   
                {
                    key_param[index].current_state = KEY_RELEASE;
				#if (SHORT_RELEASE_VALID == 1)  
					key_param[index].key_event = EVENT_SHORT_PRESSED; 
				#endif
                }
                break;
            }
            case KEY_LONG_PRESSED:{
                if(!key_press)          
                {
                    key_param[index].current_state = KEY_RELEASE;
				#if (LONG_RELEASE_VALID == 1) 
					key_param[index].key_event = EVENT_LONG_PRESSED;  
				#endif
                } 
                break;
            }
            default:{
                key_param[index].current_state = KEY_RELEASE;
            }
        }
    }
    return 0;                            
}

/**
 * @file   key_handle.c
 * @brief  处理按键事件
 * @date   2025-03-20
 * 
 * 该函数遍历所有按键，并根据 `key_event` 变量执行相应的短按或长按处理。
 * 
 * 主要逻辑：
 * 1. 遍历 `key_param` 数组，检查是否有按键事件发生。
 * 2. 根据 `index` 选择不同的按键进行处理。
 * 3. 针对短按 (`EVENT_SHORT_PRESSED`) 和长按 (`EVENT_LONG_PRESSED`) 事件执行相应操作。
 * 
 * @note  具体功能逻辑可以在 `case` 语句中填充。
 */
void key_handle(void)
{  	   
    uint8_t index;

    // 遍历所有按键，检查是否有事件发生
    for (index = 0; index < KEY_NUM_MAX; index++)
    {
        if (key_param[index].key_event != EVENT_NULL)  // 若有按键事件发生
        {
            switch (index)
            {
                case 0:  // 按键1 处理
                {
                    if (key_param[index].key_event == EVENT_SHORT_PRESSED)
                    {
                        // 按键1短按处理逻辑

                    }
                    else if (key_param[index].key_event == EVENT_LONG_PRESSED)
                    {
                        // 按键1长按处理逻辑

                    }
                    break;
                }

                case 1:  // 按键2 处理
                {
                    if (key_param[index].key_event == EVENT_SHORT_PRESSED)
                    {
                        // 按键2短按处理逻辑
												
                    }
                    else if (key_param[index].key_event == EVENT_LONG_PRESSED)
                    {
                        // 按键2长按处理逻辑

                    }
                    break;
                }

                case 2:  // 按键3 处理
                {
                    if (key_param[index].key_event == EVENT_SHORT_PRESSED)
                    {
                        // 按键3短按处理逻辑
												
                    }
                    else if (key_param[index].key_event == EVENT_LONG_PRESSED)
                    {
                        // 按键3长按处理逻辑

                    }
                    break;
                }

                case 3:  // 按键4 处理
                {
                    if (key_param[index].key_event == EVENT_SHORT_PRESSED)
                    {
                        // 按键4短按处理逻辑
											
                    }
                    else if (key_param[index].key_event == EVENT_LONG_PRESSED)
                    {
                        // 按键4长按处理逻辑
                        
                    }
                    break;
                }

                default:  
                {
                    break;
                }
            }

            // 处理完当前按键事件后，清除 `key_event` 标志，防止重复触发
            key_param[index].key_event = EVENT_NULL;
        }
    }
}




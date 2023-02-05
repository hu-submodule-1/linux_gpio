/**
 * @file      : gpio.h
 * @brief     : ARM-Linux平台GPIO驱动头文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-01-18 11:27:54
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-01-18 huenrong        创建文件
 *
 */

#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

// gpio方向
typedef enum
{
    // GPIO输入
    E_GPIO_IN = 0,
    // GPIO输出
    E_GPIO_OUT = 1,
} gpio_direction_e;

// gpio电平值
typedef enum
{
    // GPIO低电平
    E_GPIO_LOW = 0,
    // GPIO高电平
    E_GPIO_HIGH = 1,
} gpio_value_e;

// gpio中断输入, 边沿值
typedef enum
{
    // 不是中断
    E_GPIO_NONE = 0,
    // 中断输入, 上升沿
    E_GPIO_RISING = 1,
    // 中断输入, 下降沿
    E_GPIO_FALLING = 2,
    // 中断输入, 上升沿和下降沿
    E_GPIO_BOTH = 3,
} gpio_edge_e;

/**
 * @brief  导出GPIO到用户空间
 * @param  gpio_num: 输入参数, 待导出的GPIO编号
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_export(const uint16_t gpio_num);

/**
 * @brief  取消导出到用户空间的GPIO
 * @param  gpio_num: 输入参数, 待取消导出的GPIO编号
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_unexport(const uint16_t gpio_num);

/**
 * @brief  设置GPIO方向
 * @param  gpio_num : 输入参数, 待设置的GPIO编号
 * @param  direction: 输入参数, 待设置的GPIO方向
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_set_direction(const uint16_t gpio_num, const gpio_direction_e direction);

/**
 * @brief  设置GPIO输出电平值
 * @param  gpio_num: 输入参数, 待设置的GPIO编号
 * @param  value   : 输入参数, 待设置的GPIO电平值
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_set_value(const uint16_t gpio_num, const gpio_value_e value);

/**
 * @brief  获取GPIO的电平值
 * @param  value   : 输出参数, GPIO电平值
 * @param  gpio_num: 输入参数, GPIO编号
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_get_value(gpio_value_e *value, const uint16_t gpio_num);

/**
 * @brief  设置GPIO触发边沿
 * @param  gpio_num: 输入参数, 待设置的GPIO编号
 * @param  edge    : 输入参数, 待设置的GPIO边沿
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_set_edge(const uint16_t gpio_num, const gpio_edge_e edge);

/**
 * @brief  打开GPIO设备
 * @param  gpio_num: 输入参数, 待打开的GPIO编号
 * @return 成功: GPIO设备文件描述符
 *         失败: -1
 */
int gpio_open(const uint16_t gpio_num);

/**
 * @brief  关闭GPIO设备
 * @param  fd: 输入参数, 待关闭的GPIO设备文件描述符
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_close(const int fd);

#ifdef __cplusplus
}
#endif

#endif // __GPIO_H

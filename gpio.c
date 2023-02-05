/**
 * @file      : gpio.c
 * @brief     : ARM-Linux平台GPIO驱动源文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-01-18 11:27:28
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-01-18 huenrong        创建文件
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "./gpio.h"

// gpio路径
#define SYS_GPIO_DIR "/sys/class/gpio"

// 命令buf最大长度
#define CMD_BUF_MAX_LEN 60

/**
 * @brief  导出GPIO到用户空间
 * @param  gpio_num: 输入参数, 待导出的GPIO编号
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_export(const uint16_t gpio_num)
{
    int ret = -1;
    int fd = -1;
    int len = 0;
    char cmd_buf[CMD_BUF_MAX_LEN] = {0};

    // GPIO已经导出, 直接返回成功
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d", SYS_GPIO_DIR, gpio_num);
    ret = access(cmd_buf, F_OK);
    if (0 == ret)
    {
        return true;
    }

    // 打开文件: /sys/class/gpio/export
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/export", SYS_GPIO_DIR);
    fd = open(cmd_buf, O_WRONLY);
    if (fd < 0)
    {
        return false;
    }

    // 写入待导出的GPIO编号到export文件
    memset(cmd_buf, 0, sizeof(cmd_buf));
    len = snprintf(cmd_buf, sizeof(cmd_buf), "%d", gpio_num);
    ret = write(fd, cmd_buf, len);
    if (len != ret)
    {
        close(fd);

        return false;
    }

    // 关闭文件
    ret = close(fd);
    if (0 != ret)
    {
        return false;
    }

    return true;
}

/**
 * @brief  取消导出到用户空间的GPIO
 * @param  gpio_num: 输入参数, 待取消导出的GPIO编号
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_unexport(const uint16_t gpio_num)
{
    int ret = -1;
    int fd = -1;
    int len = 0;
    char cmd_buf[CMD_BUF_MAX_LEN] = {0};

    // GPIO未导出, 直接返回成功
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d", SYS_GPIO_DIR, gpio_num);
    ret = access(cmd_buf, F_OK);
    if (-1 == ret)
    {
        return true;
    }

    // 打开文件: /sys/class/gpio/unexport
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/unexport", SYS_GPIO_DIR);
    fd = open(cmd_buf, O_WRONLY);
    if (fd < 0)
    {
        return false;
    }

    // 写入待取消导出的GPIO编号到unexport文件
    memset(cmd_buf, 0, sizeof(cmd_buf));
    len = snprintf(cmd_buf, sizeof(cmd_buf), "%d", gpio_num);
    ret = write(fd, cmd_buf, len);
    if (len != ret)
    {
        close(fd);

        return false;
    }

    // 关闭文件
    ret = close(fd);
    if (0 != ret)
    {
        return false;
    }

    return true;
}

/**
 * @brief  设置GPIO方向
 * @param  gpio_num : 输入参数, 待设置的GPIO编号
 * @param  direction: 输入参数, 待设置的GPIO方向
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_set_direction(const uint16_t gpio_num, const gpio_direction_e direction)
{
    int fd = -1;
    int ret = -1;
    char cmd_buf[CMD_BUF_MAX_LEN] = {0};

    // GPIO未导出, 直接返回错误
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d", SYS_GPIO_DIR, gpio_num);
    ret = access(cmd_buf, F_OK);
    if (-1 == ret)
    {
        return false;
    }

    // 打开文件: /sys/class/gpio/gpiox/direction
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d/direction", SYS_GPIO_DIR, gpio_num);
    fd = open(cmd_buf, O_WRONLY);
    if (fd < 0)
    {
        return false;
    }

    switch (direction)
    {
    // 设置为输入模式
    case E_GPIO_IN:
    {
        ret = write(fd, "in", strlen("in"));
        if (-1 == ret)
        {
            close(fd);

            return false;
        }

        break;
    }

    // 设置为输出模式
    case E_GPIO_OUT:
    {
        ret = write(fd, "out", strlen("out"));
        if (-1 == ret)
        {
            close(fd);

            return false;
        }

        break;
    }

    default:
    {
        return false;
    }
    }

    // 关闭文件
    ret = close(fd);
    if (-1 == ret)
    {
        return false;
    }

    return true;
}

/**
 * @brief  设置GPIO输出电平值
 * @param  gpio_num: 输入参数, 待设置的GPIO编号
 * @param  value   : 输入参数, 待设置的GPIO电平值
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_set_value(const uint16_t gpio_num, const gpio_value_e value)
{
    int ret = -1;
    int fd = -1;
    char cmd_buf[CMD_BUF_MAX_LEN] = {0};

    // GPIO未导出, 直接返回错误
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d", SYS_GPIO_DIR, gpio_num);
    ret = access(cmd_buf, F_OK);
    if (-1 == ret)
    {
        return false;
    }

    // 打开文件: /sys/class/gpio/gpiox/value
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d/value", SYS_GPIO_DIR, gpio_num);
    fd = open(cmd_buf, O_WRONLY);
    if (fd < 0)
    {
        return false;
    }

    switch (value)
    {
    // 设置输出低电平
    case E_GPIO_LOW:
    {
        ret = write(fd, "0", strlen("0"));
        if (-1 == ret)
        {
            close(fd);

            return false;
        }

        break;
    }

    // 设置输出高电平
    case E_GPIO_HIGH:
    {
        ret = write(fd, "1", strlen("1"));
        if (-1 == ret)
        {
            close(fd);

            return false;
        }

        break;
    }

    default:
    {
        return false;
    }
    }

    // 关闭文件
    ret = close(fd);
    if (-1 == ret)
    {
        return false;
    }

    return true;
}

/**
 * @brief  获取GPIO的电平值
 * @param  value   : 输出参数, GPIO电平值
 * @param  gpio_num: 输入参数, GPIO编号
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_get_value(gpio_value_e *value, const uint16_t gpio_num)
{
    int ret = -1;
    int fd = -1;
    // 获取到的GPIO电平值
    char ch = 0;
    char cmd_buf[CMD_BUF_MAX_LEN] = {0};

    if (!value)
    {
        return false;
    }

    // GPIO未导出, 直接返回错误
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d", SYS_GPIO_DIR, gpio_num);
    ret = access(cmd_buf, F_OK);
    if (-1 == ret)
    {
        return false;
    }

    // 打开文件: /sys/class/gpio/gpiox/value
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d/value", SYS_GPIO_DIR, gpio_num);
    fd = open(cmd_buf, O_RDONLY);
    if (fd < 0)
    {
        return false;
    }

    // 获取GPIO电平值
    ret = read(fd, &ch, 1);
    if (-1 == ret)
    {
        close(fd);

        return false;
    }

    // 根据获取到的电平值, 返回数据
    switch (ch)
    {
    case '0':
    {
        *value = E_GPIO_LOW;

        break;
    }

    case '1':
    {
        *value = E_GPIO_HIGH;

        break;
    }

    default:
    {
        close(fd);

        return false;
    }
    }

    // 关闭文件
    ret = close(fd);
    if (-1 == ret)
    {
        return false;
    }

    return true;
}

/**
 * @brief  设置GPIO触发边沿
 * @param  gpio_num: 输入参数, 待设置的GPIO编号
 * @param  edge    : 输入参数, 待设置的GPIO边沿
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_set_edge(const uint16_t gpio_num, const gpio_edge_e edge)
{
    int ret = -1;
    int fd = -1;
    char cmd_buf[CMD_BUF_MAX_LEN] = {0};

    // GPIO未导出, 直接返回错误
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d", SYS_GPIO_DIR, gpio_num);
    ret = access(cmd_buf, F_OK);
    if (-1 == ret)
    {
        return false;
    }

    // 打开文件: /sys/class/gpio/gpiox/edge
    memset(cmd_buf, 0, sizeof(cmd_buf));
    ret = snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d/edge", SYS_GPIO_DIR, gpio_num);
    fd = open(cmd_buf, O_WRONLY);
    if (fd < 0)
    {
        return false;
    }

    switch (edge)
    {
    // 不是中断
    case E_GPIO_NONE:
    {
        ret = write(fd, "none", strlen("none"));
        if (-1 == ret)
        {
            close(fd);

            return false;
        }

        break;
    }

    // 中断输入, 上升沿
    case E_GPIO_RISING:
    {
        ret = write(fd, "rising", strlen("rising"));
        if (-1 == ret)
        {
            close(fd);

            return false;
        }

        break;
    }

    // 中断输入, 下降沿
    case E_GPIO_FALLING:
    {
        ret = write(fd, "falling", strlen("falling"));
        if (-1 == ret)
        {
            close(fd);

            return false;
        }

        break;
    }

    // 中断输入, 上升沿和下降沿
    case E_GPIO_BOTH:
    {
        ret = write(fd, "both", strlen("both"));
        if (-1 == ret)
        {
            close(fd);

            return false;
        }

        break;
    }

    default:
    {
        return false;
    }
    }

    // 关闭文件
    ret = close(fd);
    if (-1 == ret)
    {
        return false;
    }

    return true;
}

/**
 * @brief  打开GPIO设备
 * @param  gpio_num: 输入参数, 待打开的GPIO编号
 * @return 成功: GPIO设备文件描述符
 *         失败: -1
 */
int gpio_open(const uint16_t gpio_num)
{
    int fd = -1;
    char cmd_buf[CMD_BUF_MAX_LEN] = {0};

    snprintf(cmd_buf, sizeof(cmd_buf), "%s/gpio%d/value", SYS_GPIO_DIR, gpio_num);
    fd = open(cmd_buf, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        return -1;
    }

    return fd;
}

/**
 * @brief  关闭GPIO设备
 * @param  fd: 输入参数, 待关闭的GPIO设备文件描述符
 * @return true : 成功
 * @return false: 失败
 */
bool gpio_close(const int fd)
{
    if (0 == close(fd))
    {
        return true;
    }

    return false;
}

/****************************************************************************
 * halo -- Open HAL for embedded systems                                    *
 *                                                                          *
 * MIT License:                                                             *
 * Copyright (c) 2017 SimonQian                                             *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 ****************************************************************************/

#ifndef __GD32F1X0_CORE_H_INCLUDED__
#define __GD32F1X0_CORE_H_INCLUDED__

#define GD32F1X0_CLK_HSI				(1UL << 0)
#define GD32F1X0_CLK_HSE				(1UL << 1)
#define GD32F1X0_CLK_PLL				(1UL << 2)

struct gd32f1x0_afio_pin_t
{
	uint8_t port;
	uint8_t pin;
	int8_t af;
};
vsf_err_t gd32f1x0_afio_config(const struct gd32f1x0_afio_pin_t *pin, uint32_t mode);

enum gd32f1x0_clksrc_t
{
	GD32F1X0_CLKSRC_HSI = 0,
	GD32F1X0_CLKSRC_HSE = 1,
	GD32F1X0_CLKSRC_PLL = 2
};
enum gd32f1x0_pllsrc_t
{
	GD32F1X0_PLLSRC_HSID2,
	GD32F1X0_PLLSRC_HSE,
	GD32F1X0_PLLSRC_HSED2,
};
struct gd32f1x0_info_t
{
	uint8_t priority_group;
	uint32_t vector_table;
	
	uint32_t clk_enable;
	
	enum gd32f1x0_clksrc_t clksrc;
	enum gd32f1x0_pllsrc_t pllsrc;
	
	uint32_t hse_freq_hz;
	uint32_t hsi_freq_hz;
	uint32_t pll_freq_hz;
	uint32_t ahb_freq_hz;
	uint32_t apb_freq_hz;
	
	uint32_t sys_freq_hz;
};

vsf_err_t gd32f1x0_interface_init(void *p);
vsf_err_t gd32f1x0_interface_fini(void *p);
vsf_err_t gd32f1x0_interface_reset(void *p);
uint32_t gd32f1x0_interface_get_stack(void);
vsf_err_t gd32f1x0_interface_set_stack(uint32_t sp);
void gd32f1x0_interface_sleep(uint32_t mode);
vsf_err_t gd32f1x0_interface_get_info(struct gd32f1x0_info_t **info);
vsf_err_t gd32f1x0_interface_pendsv_config(void (*on_pendsv)(void *), void *param);
vsf_err_t gd32f1x0_interface_pendsv_trigger(void);

uint32_t gd32f1x0_uid_get(uint8_t *buffer, uint32_t size);

vsf_err_t gd32f1x0_tickclk_init(void);
vsf_err_t gd32f1x0_tickclk_fini(void);
void gd32f1x0_tickclk_poll(void);
vsf_err_t gd32f1x0_tickclk_start(void);
vsf_err_t gd32f1x0_tickclk_stop(void);
uint32_t gd32f1x0_tickclk_get_count(void);
vsf_err_t gd32f1x0_tickclk_config_cb(void (*callback)(void*), void *param);

#endif	// __GD32F1X0_CORE_H_INCLUDED__

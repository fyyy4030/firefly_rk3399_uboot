/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _IRQ_GENERIC_H
#define _IRQ_GENERIC_H

#include <asm-generic/gpio.h>
#include <common.h>
#include <dt-bindings/pinctrl/rockchip.h>

/*
 * IRQ line status.
 *
 * IRQ_TYPE_NONE		- default, unspecified type
 * IRQ_TYPE_EDGE_RISING		- rising edge triggered
 * IRQ_TYPE_EDGE_FALLING	- falling edge triggered
 * IRQ_TYPE_EDGE_BOTH		- rising and falling edge triggered
 * IRQ_TYPE_LEVEL_HIGH		- high level triggered
 * IRQ_TYPE_LEVEL_LOW		- low level triggered
 * IRQ_TYPE_LEVEL_MASK		- mask to filter out the level bits
 * IRQ_TYPE_SENSE_MASK		- mask for all the above bits
 */
enum {
	IRQ_TYPE_NONE		= 0x00000000,
	IRQ_TYPE_EDGE_RISING	= 0x00000001,
	IRQ_TYPE_EDGE_FALLING	= 0x00000002,
	IRQ_TYPE_EDGE_BOTH	= (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING),
	IRQ_TYPE_LEVEL_HIGH	= 0x00000004,
	IRQ_TYPE_LEVEL_LOW	= 0x00000008,
	IRQ_TYPE_LEVEL_MASK	= (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH),
	IRQ_TYPE_SENSE_MASK	= 0x0000000f,
};

/*
 * struct irq_chip - hardware interrupt chip descriptor
 *
 * @name:		name for irq chip
 * @irq_enable:		enable the interrupt (defaults to chip->unmask if NULL)
 * @irq_disable:	disable the interrupt
 * @irq_ack:		start of a new interrupt
 * @irq_eoi:		end of interrupt
 * @irq_set_type:	set the flow type (IRQ_TYPE_LEVEL/etc.) of an IRQ
 */
struct irq_chip {
	const char	*name;
	int		(*irq_init)(void);
	int		(*irq_suspend)(void);
	int		(*irq_resume)(void);
	int		(*irq_get)(void);
	int		(*irq_enable)(int irq);
	int		(*irq_disable)(int irq);
	void		(*irq_ack)(int irq);
	void		(*irq_eoi)(int irq);
	int		(*irq_set_type)(int irq, unsigned int flow_type);
};

/* APIs for irqs */
void irq_install_handler(int irq, interrupt_handler_t *handler, void *data);
void irq_free_handler(int irq);
int irq_set_irq_type(int irq, unsigned int type);
int irq_handler_enable(int irq);
int irq_handler_disable(int irq);
int irqs_suspend(void);
int irqs_resume(void);
int gpio_to_irq(struct gpio_desc *gpio);

/*
 * Assign gpio to irq directly. Don't use it without special reasons.
 *
 * Usage example:
 *	int gpio0_a0, irq;
 *
 *	gpio = RK_IRQ_GPIO(RK_GPIO0, RK_PA0);
 *	irq = hard_gpio_to_irq(gpio0_a0);
 *	irq_install_handler(irq, ...);
 */
#define GPIO_BANK_SHIFT			8
#define RK_IRQ_GPIO(bank, pin) 		(((bank) << GPIO_BANK_SHIFT) | (pin))
int hard_gpio_to_irq(unsigned gpio);
int phandle_gpio_to_irq(u32 gpio_phandle, u32 pin);

/* only irq-gpio.c can use it */
void _generic_gpio_handle_irq(int irq);

#endif /* _IRQ_GENERIC_H */

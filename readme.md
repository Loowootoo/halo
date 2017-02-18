#ͨ��HAL(������)

ͨ��HAL�������Ƕ��ʽMCU���������У��߲����Եײ�Ӳ�����������⡣ͨ��ʵ��ͨ�õ�Ӳ������㣨HAL������ʵ�ֶԵײ�Ӳ�������Ρ�

1. **���ٿ���**�� Ӧ�ó��򿪷��û�������Ҫ��ϤĿ��MCU�ĸ�������Ĵ�����ֱ�ӵ���HAL�е�ͨ�ýӿڡ�
2. **������ֲ**��Ӳ��BSP�����˺�Ӳ����صĸ������ã�Ӧ�ó���ֱ�Ӹ���������ã��Ϳ���ʵ��Ӧ�õ���ֲ��

--------------------------------

[TOC]

##���

��������ARM��32λMCU�����У�ϵͳ��Դ�����ܵõ��˷ǳ���ķḻ����ߡ�ͨ��HAL����ƣ�����Ϊ��ʵ���ڲ�ͬ��MCU�ϣ��ṩͬһ��Ӳ�����ʽӿڣ�ʹ�ø߲�Ĵ������Ӳ���޹أ���߿�����ά����Ч�ʡ����ң����붼�ǹ����޹صģ��߲�Ӧ�ÿ���ʹ�ø��ֲ�ͬ�Ĺ����Լ�ʵʱ����ϵͳ��RTOS�������µ�ʾ�����룬������vsfhal���ʵ�֣�����Ҳֻ��ʾ���������������յ�ʵ�֡�

###�򵥵Ļ��ں����ķ��ʽӿ�

``` c
vsfhal_gpio_init(USBD_PULLUP_PORT);
vsfhal_gpio_clear(USBD_PULLUP_PORT, 1 << USBD_PULLUP_PIN);
vsfhal_gpio_config_pin(USBD_PULLUP_PORT, USBD_PULLUP_PIN, GPIO_OUTPP);
```

###�������ţ�����ͨ�����������

һ�㣬��������Ľӿڵĵ�һ��������Ϊ����������ţ���ź�ʵ��MCU�ϵ�������Դ�Ķ�Ӧ��ϵ���ɸ�MCU����������Աȷ���������ṩ��Ӧ���ĵ�������һ���ض���Ӳ�����弶֧�ְ���BSP���Ͱ�����ʹ�õ������������Ŷ��塣����ʵ�ַ�ʽֻ���Ƽ�������ǿ��Ҫ��

hw_cfg_XXXX_Board.h:
``` c
#define OSC_HZ							((uint32_t)8000000)
#define USB_PULLUP_PORT					0
#define USB_PULLUP_PIN					15
```

��Ӧ��.c�ļ��
``` c
struct bsp_hwcfg_t
{
	struct
	{
		struct interface_gpio_pin_t pullup;
	} usbd;
	// other hardware configurations
} static bsp_hwcfg =
{
	.usbd.pullup.port = USB_PULLUP_PORT,
	.usbd.pullup.pin = USB_PULLUP_PIN,
};
```

Ӧ�ô����У�ͨ��bspģ��ָ������źͲ�����������Ӧ��������ʡ�
``` c
vsfhal_gpio_init(bsp_hwcfg.usbd.pullup.port);
vsfhal_gpio_clear(bsp_hwcfg.usbd.pullup.port, 1 << bsp_hwcfg.usbd.pullup.pin);
vsfhal_gpio_config_pin(bsp_hwcfg.usbd.pullup.port, bsp_hwcfg.usbd.pullup.pin, GPIO_OUTPP);
```
����Ӳ�����ʽӿ���ͨ�õģ��������ض���Ӳ��ʵ�֣�ͨ��Ӳ�������ļ����������Ҫʹ�õ�����ӿںͲ�����������Ӧ�ô���Ͳ���Ҫ��Բ�ͬ��Ӳ�����޸ģ���Ҫ�޸ĵ�ֻ��Ӳ�������ļ����Ӷ�Ӧ�ô����������Ӳ��ʵ�֡�

���⣬�������Ҳ�����������GPIO����ӳ�䣬����Ҳ�ٸ�ʵ�����������ӡ�GD32��IIC���ſ����ڶ��GPIO���������ã���ͬ��������ϣ���Ӧһ��IIC��������š�
``` c
struct i2c_param_t
{
	I2C_TypeDef *reg;
	struct gd32f1x0_afio_pin_t sda[3];
	struct gd32f1x0_afio_pin_t scl[3];
} static const gd32f1x0_i2c_param[IIC_NUM] = 
{
	{
		I2C1,
		{
			{0, 10, 4},		// PA10, AF4
			{1, 7, 1},		// PB7, AF1
			{1, 9, 1},		// PB9, AF1
		},
		{
			{0, 9, 4},		// PA9, AF4
			{1, 6, 1},		// PB6, AF1
			{1, 8, 1},		// PB8, AF1
		},
	},
	{
		I2C2,
		{
			{0, 1, 4},		// PA1, AF4
			{1, 11, 1},		// PB11, AF1
			{6, 7, -1},		// PF7, default
		},
		{
			{0, 0, 4},		// PA0, AF4
			{1, 10, 1},		// PB10, AF1
			{6, 6, -1},		// PF6, default
		},
	},
};

// 2-bit iic_idx, 3-bit sda_idx, 3-bit scl_idx
vsf_err_t gd32f1x0_i2c_init(uint8_t index)
{
	uint8_t iic_idx = (index & 0x03) >> 0;
	uint8_t sda_idx = (index & 0x1C) >> 2;
	uint8_t scl_idx = (index & 0xE0) >> 5;
	I2C_TypeDef *reg = gd32f1x0_i2c_param[iic_idx].reg;

	gd32f1x0_afio_config(&gd32f1x0_i2c_param[iic_idx].sda[sda_idx], gd32f1x0_GPIO_AF | gd32f1x0_GPIO_OD);
	gd32f1x0_afio_config(&gd32f1x0_i2c_param[iic_idx].scl[scl_idx], gd32f1x0_GPIO_AF | gd32f1x0_GPIO_OD);
	
	RCC->APB1CCR |= RCC_APB1CCR_I2C1EN << iic_idx;
	reg->CTLR1 = 0;
	return VSFERR_NONE;
}
```
��GD32��IIC���������IIC��������Ű�����IIC������ӳ����Ϣ�����ԣ��û�������һ����ţ��Ϳ���ȷ��ʹ�ò�ͬ��IIC�����ˡ�

###�ӿڶ��Ƿ������ģ��ṩ��ѯ���жϻص�2�з�ʽ

����ϵͳ���ܵĲ�ͬ��ϵͳ������������ѯģʽ�£�Ҳ�������ж�ģʽ�£�ǰ��̨ϵͳ����������2��ϵͳ���ӿ��ϻ���һЩ���𡣱��磬����Ӧ������ѯģʽ�£���Ҫ��ѯ�շ���ɱ�־�������жϻص�ģʽ�£�ֻ��Ҫ���շ���ɺ󣬵�����Ӧ�Ļص��ӿڣ�����ģʽ������Ӳ�����жϣ�����2��ģʽ����ʵ��Ӧ���У������ܻ�����������USBD��ʵ����������

һ�㣬���MCU�Դ�USBD���裬��ô��ȫ����ʹ���ж�ģʽ����Ȼ��ʹ����ѯҲ���ԡ����ǣ����MCU����USBD���裬��Ҫʹ��GPIO��ģ��Ļ����������Ҫ�ϸ��ʱ��ȷ���ԣ��������µ�ARMоƬ��tail-chaining���ܣ�����ʹ��ʱ��ȷ�������ʱ�򣬾���Ҫʹ����ѯģʽ������ϵͳֻ��һ���ж�����ģ��USBD���������ֶ���������ѯģʽ�¡�

������о�һ��VSF�еĴ��롣
����ϵͳ1ms��ʱ���֣�һ������ʵ�֣�
``` c
static void app_tickclk_callback_int(void *param)
{
	vsftimer_callback_int();
}
......
vsfhal_tickclk_config_cb(app_tickclk_callback_int, NULL);
```
����tickclkģ�飬�������ûص������ĽӿںͲ����������ڻص���������ö�ʱ��ģ��Ļص��ӿ��Դ���ϵͳ�ĸ�����ʱ�������ϴ���һ�����������ж�ģʽ��callback���ж������У��������ѯģʽ�µĻ����������´��루����ͨ�����ú꣬�ر�VSF���жϲ��֣���
``` c
void usrapp_poll(struct usrapp_t *app)
{
	vsfhal_tickclk_poll();
}
```
usrapp_poll��VSF��������Ϊ��ѯģʽʱ�����ѯ���롣����vsfhal_tickclk_poll������ѯtickclkģ�飬���1ms������ٵ���֮ǰ���õ�callback��callback�������ж������С�
``` c
void vsfhal_tickclk_poll()
{
	if (SysTick->CTRL & CM3_SYSTICK_COUNTFLAG)
		SysTick_Handler();
}
```
���SysTick_Handler�����жϽӿڣ�����ѯģʽ�£�poll��Ĵ��룬���ڶ�ʱ�������ʱ�򣬵���SysTick_Handler��

###һЩ�����޹صĻ���ģ��

����Ӳ�����ƽӿ��⣬�������ṩһЩͨ�õĹ����޹صĻ���ģ�飬����fifoģ�顢streamģ��ȵȡ�

####vsfstream -- ����

vsfstream������һ�ײ������ڴ�ʵ�ֵ����صĽӿڣ�Ҳ����˵�������ڲ�ͬ��ʽ���ڴ�ṹ��ʵ���Զ����أ���ȥ�û������Լ�ʵ�����ء�Ŀǰʵ������3���������ң�����Ҳ������չ��������������������أ����ڸ߲�Ӧ����˵�����ýӿںͿ��Ʒ�ʽ��ȫһ����

1. vsf_fifostream_t: ����FIFO�ڴ�ṹ�����ء�
2. vsf_mbufstream_t: ���ڶ໺���ڴ�ṹ�����أ�һ���������豸�Ϳ��豸��ת����������Ƶ��������Ƶ֡��Ļ�ת��
3. vsf_bufstream_t: ������ͨ�ڴ��ṹ�����ء�

##�ӿڹ淶

TBD

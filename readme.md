#ͨ��HAL -- halo

halo�������Ƕ��ʽMCU���������У��߲����Եײ�Ӳ�����������⡣ͨ��ʵ��ͨ�õ�Ӳ������㣨HAL������ʵ�ֶԵײ�Ӳ�������Ρ����ң�haloʵ����һЩ������ϵͳ�޹صĻ���������halo������Ϊ����ʵʱ����ϵͳ��RTOS������״̬�����ܵ�Ӳ�������ͻ�����ܲ㡣

1. **���ٿ���**��Ӧ�ó��򿪷��û�������Ҫ��ϤĿ��MCU�ĸ�������Ĵ�����ֱ�ӵ���HAL�е�ͨ�ýӿڡ�
2. **������ֲ**��Ӳ��BSP�����˺�Ӳ����صĸ������ã�Ӧ�ó���ֱ�Ӹ���������ã��Ϳ���ʵ��Ӧ�õ���ֲ��
3. **GUI����**������ͨ��GUI������halo�����ң����Լ����û�����ģ������ù��ܡ�
4. ~~**����MISRA**���������µ�MISRA�����׼~~

--------------------------------

[TOC]

##���

Ƕ��ʽMCUӦ�õĿ��������ڵ�ǰӲ�����������£��������Ч�Ŀ�����ʽ����������ARM��32λMCU�����У�ϵͳ��Դ�����ܵõ��˷ǳ���ķḻ����ߡ�halo����ƣ�����Ϊ��ʵ���ڲ�ͬ��MCU�ϣ��ṩͬһ��Ӳ�����ʽӿڣ�ʹ�ø߲�Ĵ������Ӳ���޹أ���߿�����ά����Ч�ʡ����ң����붼�ǹ����޹صģ��߲�Ӧ�ÿ���ʹ�ø��ֲ�ͬ�Ĺ����Լ�ʵʱ����ϵͳ��RTOS����

���µ�ʾ�����룬��ʹ��vsfhal���ʵ�֣�����Ҳֻ��ʾ���������������յ�ʵ�֡�

###����������ʷ�ʽ

halo��ʵ����3��������ʵķ�ʽ������ѡ��ʲô��ʽ�����û������Լ���Ӧ�����󣬿���ά�����Ӷ�������ѡ�񡣲�ͬ�ķ��ʷ�ʽ�����ص㣬����û�ͬʱʹ�ö�̬�;�̬�Ľӿڣ��û���Ҫ������Դ����ͻ�����磬Ӧ���У�ģ��A��̬����õ���ʱ��0��ģ��Bָ��ʹ�ö�ʱ��0���������ڶ�ʱ��0��ʹ�ã��ͻ��г�ͻ����

####���ں����������裨���ڷ���оƬ�Դ���������Դ����
``` c
vsfhal_gpio_init(USBD_PULLUP_PORT);
vsfhal_gpio_clear(USBD_PULLUP_PORT, 1 << USBD_PULLUP_PIN);
vsfhal_gpio_config_pin(USBD_PULLUP_PORT, USBD_PULLUP_PIN, GPIO_OUTPP);
```
���ַ�ʽ������ֱ�ӵ��ú�����ʽ�������ӿڣ����ҵ�һ�������Ƕ�Ӧ���������š�����һЩ���裬��Ż��������������ŵĸ��ö��壬�ײ��������Զ�ִ�����Ÿ��õĳ�ʼ��������Ҫ�߲���롣�������ţ���ͨ��ͼ�λ����ý��棬�Զ����ɵġ�

####���ڽӿڷ������裨������оƬ�Դ��ģ�Ҳ������ͨ��������ʽʵ�ֵģ���
``` c
struct vsfusbd_device_t
{
	......
	struct interface_usbd_t *drv;
	......
};

static void vsfusbd_transact_out(struct vsfusbd_device_t *device,
								struct vsfusbd_transact_t *transact)
{
	uint8_t ep = transact->ep;
	struct interface_usbd_t *drv = device->drv;

	if (transact->idle)
	{
		uint16_t ep_size = drv->ep.get_OUT_epsize(ep);
		uint16_t pkg_size = min(ep_size, transact->data_size);
		uint32_t free_size = stream_get_free_size(transact->stream);

		transact->idle = free_size < pkg_size;
		if (!transact->idle)
		{
			drv->ep.enable_OUT(ep);
		}
	}
}
```
��������ΪusbdЭ��ջ�еĴ��룬usbd������оƬ�Դ������裬Ҳ������ʹ��GPIOģ��ģ�����������ʹ��һЩͨ�ýӿڣ�����SPI��������չоƬʵ�ֵġ�ͨ���ӿڷ������裬�������νӿڵĲ�ͬʵ�ַ�ʽ���Ը߲�����Ӱ�졣

���ڽӿڵ�������ʣ���BSP���У�������Ӧ����������֣�������������ɵ����ݽṹ�������磺IIC_EEPROM����ʾ����EEPROM��IIC�ӿڡ����õ�ʱ��ʹ��IIC_EEPROM.Init(...)�����ַ�ʽ���൱����Ϊ��һ�ַ�ʽ���������������ķ�װ��

####��̬�������
����;ټ����ʺϵ�Ӧ�������
1. ��ʱ����Դ�Զ����䡣�߲����ֻ��Ҫ����һ����ʱ����Դ�����Ҵ�����Ҫ�Ķ�ʱ�������������Զ�ƥ���ʺϵĶ�ʱ���������ʺ϶��ϵͳ���ƹ�����Դ�ķ��ʹ���
2. ����/Hash��Դ���䡣��ЩоƬ��CRC����Ӧ������CRC��ʱ�򣬵õ�Ӳ���Ľӿڡ����оƬ����CRC���õ��ľ������ʵ�ֵ�CRC�ӿڡ����ڸ߲�Ӧ�ã�����Ҫ����CRC��Ӳ�������������Ȼ��ʵ��ʹ�õ�ʱ�������ϻ��в��
3. �ɶ�̬�ع���Դ���䡣��ЩоƬ�����һ����Ӳ����̬�ع����ԣ��߲����������Ҫ�ع������裬�ײ��Զ��ع���

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

ͨ����ţ��Ϳ��Է��ʶ�Ӧ��оƬ���衣BSP���ŵ�ǰӲ��ʹ�õ����������Ӧ�ı�ţ��������Ϳ��Լ�ʵ��Ӳ�������ļ����ǵĶ��ڲ�ͬ�Ĺ��̣�Ӧ�ô���ͨ�á�оƬ��ֲҲֻ��Ҫ�½���Ӧ�Ĺ��̣������޸�Ӳ�������ļ����Է���ʵ��Ӳ����

###ͼ�λ������Լ������Զ�����
ͼ�λ����ÿ�������ʵ��оƬ���ⲿģ��Ĺ������ã��Զ�����BSPӲ�������ļ����Լ�ģ��������ļ���ͼ�λ������xml���壬�������ֲ�ͬMCU����������û�ģ�������

ͼ�λ����õ�ʱ���û������Լ�����������������MCU������������ض���MCUоƬ��Ҳ�����������MCU����Ȼ���ڸ������֮�佨����ϵ������������Ӧ�Ĳ��������磬��MCU�����һ·SPI���ӵ�Dataflash��������ֿ����Զ�������Ӧ��Ӳ��������Ϣ����Ȼ����������԰���������������һ��MP3����������߱�һ����������һ������������������ӵ���Ƶ�������裬������ӵ�USB��Audio�ӿڣ�Ȼ���Զ����ɶ�Ӧ�Ĵ��롣

###�ӿڶ��Ƿ������ģ����ҿ�������ѯ���жϷ�ʽ��ʹ��

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

###2��Ӧ�ÿ�����ʽ

�û������Լ�ѡ��ʹ�ö�̬��Ŀ�����ʽ��Ӧ�ÿ�������Ӽ򵥣�Ҳ���Ƽ��ķ�ʽ����Ȼ������û���Ҫ���õ��Ż����Լ�һЩ���ƻ��Ĺ��ܣ�����ѡ��ʹ��Դ������뿪����

####ֱ��ʹ��Դ���뿪��

halo�ĸ������ܶ��ṩԴ���룬�û������Ķ�����ģ���˵���ĵ����Լ�����ģ��Ĳ�����ֱ�Ӵ�Դ��������Ӧ�á�

####ʹ�ö�̬�⿪��
�����Ӧ�ÿ������˸��õĸ��룬Ӧ����3��������ɣ�Ӧ�ù��ܣ��Լ��м������BSP���ײ��оƬ������3�����̿��Զ������룬Ӧ�ô��벻�����ض�Ӳ����BSP�͵ײ�оƬ�������ײ�оƬ��������ֱ��ʹ�ñ���õ�lib�⡣

����CMSIS���ƹ㾭�飬һ��ʼCMSIS�ṩ�Ķ���Դ���룬���û��Լ����뵽���̱��룬�����80%�ķ������⣬���Ǻͱ����йصġ�����CMSISʹ�ÿ�ķ�ʽ���û��ͻ����ϲ�����������������ˡ�CMSIS���������ǳ�ֵ�ý����

###һЩ�����޹صĻ�������

����Ӳ�����ƽӿ��⣬�������ṩһЩͨ�õĹ����޹صĻ���ģ�飬����fifoģ�顢streamģ��ȵȡ�

####vsfstream -- ����

vsfstream������һ�ײ������ڴ�ʵ�ֵ����صĽӿڣ�Ҳ����˵�������ڲ�ͬ��ʽ���ڴ�ṹ��ʵ���Զ����أ���ȥ�û������Լ�ʵ�����ء�Ŀǰʵ������3��������ʵ��Ϊ�������3���ض�����λ���������͵������ͣ������ң�����Ҳ������չ��������������������أ����ڸ߲�Ӧ����˵�����ýӿںͿ��Ʒ�ʽ��ȫһ����

1. vsf_fifostream_t: ����FIFO�ڴ�ṹ�����ء�
2. vsf_mbufstream_t: ���ڶ໺���ڴ�ṹ�����أ�һ���������豸�Ϳ��豸��ת����������Ƶ��������Ƶ֡��Ļ�ת��
3. vsf_bufstream_t: ������ͨ�ڴ��ṹ�����ء�

TBD

##�ӿڹ淶

###core
``` c
struct interface_core_t
{
	vsf_err_t (*init)(void *p);
	vsf_err_t (*fini)(void *p);
	vsf_err_t (*reset)(void *p);
	uint32_t (*get_stack)(void);
	vsf_err_t (*set_stack)(uint32_t sp);
	void (*sleep)(uint32_t mode);
	vsf_err_t (*pendsv_config)(void (*on_pendsv)(void *), void *param);
	vsf_err_t (*pendsv_trigger)(void);
};
```

###Tick
``` c
struct interface_tickclk_t
{
	vsf_err_t (*init)(void);
	vsf_err_t (*fini)(void);
	void (*poll)(void);
	vsf_err_t (*start)(void);
	vsf_err_t (*stop)(void);
	uint32_t (*get_count)(void);
	vsf_err_t (*config_cb)(void (*)(void*), void*);
};
```

###GPIO
``` c
struct interface_gpio_t
{
#if IFS_CONST_EN
	struct
	{
		uint32_t INFLOAT;
		uint32_t INPU;
		uint32_t INPD;
		uint32_t OUTPP;
		uint32_t OUTOD;
	} constants;
#endif
	vsf_err_t (*init)(uint8_t index);
	vsf_err_t (*fini)(uint8_t index);
	vsf_err_t (*config_pin)(uint8_t index, uint8_t pin_idx, uint32_t mode);
	vsf_err_t (*config)(uint8_t index, uint32_t pin_mask, uint32_t io, uint32_t pull_en_mask, uint32_t input_pull_mask);
	vsf_err_t (*set)(uint8_t index, uint32_t pin_mask);
	vsf_err_t (*clear)(uint8_t index, uint32_t pin_mask);
	vsf_err_t (*out)(uint8_t index, uint32_t pin_mask, uint32_t value);
	vsf_err_t (*in)(uint8_t index, uint32_t pin_mask, uint32_t *value);
	uint32_t (*get)(uint8_t index, uint32_t pin_mask);
};
```

###SPI
``` c
struct interface_spi_t
{
#if IFS_CONST_EN
	struct
	{
		uint32_t MASTER;
		uint32_t SLAVE;
		uint32_t MODE0;
		uint32_t MODE1;
		uint32_t MODE2;
		uint32_t MODE3;
		uint32_t MSB_FIRST;
		uint32_t LSB_FIRST;
	} constants;
#endif
	vsf_err_t (*init)(uint8_t index);
	vsf_err_t (*fini)(uint8_t index);
	vsf_err_t (*get_ability)(uint8_t index, struct spi_ability_t *ability);
	vsf_err_t (*enable)(uint8_t index);
	vsf_err_t (*disable)(uint8_t index);
	vsf_err_t (*config)(uint8_t index, uint32_t kHz, uint32_t mode);
	vsf_err_t (*config_cb)(uint8_t index, uint32_t int_priority, void *p, void (*onready)(void *));

	vsf_err_t (*select)(uint8_t index, uint8_t cs);
	vsf_err_t (*deselect)(uint8_t index, uint8_t cs);

	vsf_err_t (*start)(uint8_t index, uint8_t *out, uint8_t *in, uint32_t len);
	uint32_t (*stop)(uint8_t index);
};
```

###IIC
``` c
struct interface_i2c_t
{
	vsf_err_t (*init)(uint8_t index);
	vsf_err_t (*fini)(uint8_t index);
	vsf_err_t (*config)(uint8_t index, uint16_t kHz);
	vsf_err_t (*config_cb)(uint8_t index, void *param, void (*cb)(void*, vsf_err_t));
	vsf_err_t (*xfer)(uint8_t index, uint16_t addr, struct interface_i2c_msg_t *msg, uint8_t msglen);
};
```

###Flash

###Timer

###Eint
``` c
struct interface_eint_t
{
#if IFS_CONST_EN
	struct
	{
		uint32_t ONFALL;
		uint32_t ONRISE;
		uint32_t ONLOW;
		uint32_t ONHIGH;
	} constants;
#endif
	vsf_err_t (*init)(uint32_t index);
	vsf_err_t (*fini)(uint32_t index);
	vsf_err_t (*config)(uint32_t index, uint32_t type, uint32_t int_priority, void *param, void (*callback)(void *param));
	vsf_err_t (*enable)(uint32_t index);
	vsf_err_t (*disable)(uint32_t index);
};
```

###USART
``` c
struct interface_usart_t
{
#if IFS_CONST_EN
	struct
	{
		uint32_t STOPBITS_1;
		uint32_t STOPBITS_1P5;
		uint32_t STOPBITS_2;
		uint32_t PARITY_NONE;
		uint32_t PARITY_ODD;
		uint32_t PARITY_EVEN;
	} constants;
#endif
	vsf_err_t (*init)(uint8_t index);
	vsf_err_t (*fini)(uint8_t index);
	vsf_err_t (*config)(uint8_t index, uint32_t baudrate, uint32_t mode);
	vsf_err_t (*config_cb)(uint8_t index, uint32_t int_priority, void *p, void (*ontx)(void *), void (*onrx)(void *, uint16_t));
	uint16_t (*tx_bytes)(uint8_t index, uint8_t *data, uint16_t size);
	uint16_t (*tx_get_free_size)(uint8_t index);
	uint16_t (*rx_bytes)(uint8_t index, uint8_t *data, uint16_t size);
	uint16_t (*rx_get_data_size)(uint8_t index);
};
```

###ADC
``` c
struct interface_adc_t
{
	vsf_err_t (*init)(uint8_t index);
	vsf_err_t (*fini)(uint8_t index);
	vsf_err_t (*config)(uint8_t index, uint32_t clock_hz, uint32_t mode);
	vsf_err_t (*config_channel)(uint8_t index, uint8_t channel, uint8_t cycles);
	uint32_t (*get_max_value)(uint8_t index);
	vsf_err_t (*calibrate)(uint8_t index, uint8_t channel);
	vsf_err_t (*start)(uint8_t index, uint8_t channel,
				 		void (callback)(void *, uint16_t), void *param);
};
```

###USBD

###USBDIO(GPIO emulated USBD)
``` c
struct interface_usbdio_t
{
	vsf_err_t (*init)(void (*onrx)(enum usbdio_evt_t evt, uint8_t *buf, uint16_t len));
	vsf_err_t (*fini)(void);
	vsf_err_t (*tx)(uint8_t *buf, uint16_t len);
};
```

###HCD(Host Controller driver, eg: OHCI, EHCI)
``` c
struct interface_hcd_t
{
	vsf_err_t (*init)(uint32_t index, vsf_err_t (*hcd_irq)(void *), void *param);
	vsf_err_t (*fini)(uint32_t index);
	void* (*regbase)(uint32_t index);
};
```

###PWM

###EBI

	/********************************************************
		
		1. ENABLE MSI and wait for the MSI to become Ready
		2. Enable the MSI range and Select the MSI range
		3. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
		4. Configure the FLASH PREFETCH and the LATENCY Related Settings
		5. Configure the PRESCALARS HCLK, PCLK1, PCLK2
		6. Select the Clock Source and wait for it to be set
		
	********************************************************/

	/* 1. ENABLE MSI and wait for the MSI to become Ready */

	//use  RCC clock control register for this
void RCCConfig(void)
{
	RCC->CR|=RCC_CR_MSION;
	while(!(RCC->CR & RCC_CR_MSIRDY));
	
	/* Bits 7:4 MSIRANGE[3:0]: MSI clock ranges
		These bits are configured by software to choose the frequency range of MSI when
		MSIRGSEL is set.12 frequency ranges are available:
		0000: range 0 around 100 kHz
		0001: range 1 around 200 kHz
		0010: range 2 around 400 kHz
		0011: range 3 around 800 kHz
		0100: range 4 around 1M Hz
		0101: range 5 around 2 MHz
		0110: range 6 around 4 MHz (reset value)
		0111: range 7 around 8 MHz
		1000: range 8 around 16 MHz
		1001: range 9 around 24 MHz
		1010: range 10 around 32 MHz
		1011: range 11 around 48 MHz
		others: not allowed (hardware write protection)
		Note: Warning: MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is
		ready (MSIRDY=1). MSIRANGE must NOT be modified when MSI is ON and NOT
		ready (MSION=1 and MSIRDY=0)
		*/
	
		/*
		Bit 3 MSIRGSEL: MSI clock range selection
		Set by software to select the MSI clock range with MSIRANGE[3:0]. Write 0 has no effect.
		After a standby or a reset MSIRGSEL is at 0 and the MSI range value is provided by
		MSISRANGE in CSR register.
		0: MSI Range is provided by MSISRANGE[3:0] in RCC_CSR register
		1: MSI Range is provided by MSIRANGE[3:0] in the RCC_CR register
	*/
	RCC->CR|=RCC_CR_MSIRANGE_7 | RCC_CR_MSIRANGE;
	

	/* 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR 
		use APB1 peripheral clock enable register 1 (RCC_APB1ENR1)
		
		This register enable clock for various APB1 peripherals such as PWR, TIM1...TIM7, LCD, RTC, WDG, SPI, USART, I2C, CAN etc
		
	*/

	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;


	/* Dynamic voltage scaling management
		
		The dynamic voltage scaling is a power management technique which consists in
		increasing or decreasing the voltage used for the digital peripherals (VCORE), according to
		the application performance and power consumption needs.
		Dynamic voltage scaling to increase VCORE is known as over-volting. It allows to improve
		the device performance.
		Dynamic voltage scaling to decrease VCORE is known as under-volting. It is performed to
		save power, particularly in laptop and other mobile devices where the energy comes from a
		battery and is thus limited.
		The main regulator have two possible programmable voltage range detailed below:
		
		• Range 1: High-performance range.
		The main regulator provides a typical output voltage at 1.2 V. The system clock frequency
		can be up to 80 MHz. The Flash access time for read access is minimum, write and erase
		operations are possible.
		
		• Range 2: Low-power range.
		The main regulator provides a typical output voltage at 1.0 V. The system clock frequency
		can be up to 26 MHz.The Flash access time for a read access is increased as compared to
		Range 1; write and erase operations are possible.
		Voltage scaling is selected through the VOS bit in the PWR_CR1 register.
		
		The sequence to go from Range 1 to Range 2 is:
		1.Reduce the system frequency to a value lower than 26 MHz
		2. Adjust number of wait states according new frequency target in Range 2
		(LATENCY bits in the FLASH_ACR).
		3. Program the VOS bits to “10” in the PWR_CR1 register.
		
		The sequence to go from Range 2 to Range 1 is:
		1. Program the VOS bits to “01” in the PWR_CR1 register.
		2. Wait until the VOSF flag is cleared in the PWR_SR2 register.
		3. Adjust number of wait states according new frequency target in Range 1
		(LATENCY bits in the FLASH_ACR).
		4. Increase the system frequency. */
		
	/* Bits 10:9 VOS: Voltage scaling range selection
	00: Cannot be written (forbidden by hardware)
	01: Range 1
	10: Range 2
	11: Cannot be written (forbidden by hardware) */

	PWR->CR1|=PWR_CR1_VOS_0;
	
/* 		
		3. Configure the FLASH PREFETCH and the LATENCY Related Settings
		use Flash access control register (FLASH_ACR)

		Bit 10 DCEN: Data cache enable
	0: Data cache is disabled
	1: Data cache is enabled

		Bit 9 ICEN: Instruction cache enable
	0: Instruction cache is disabled
	1: Instruction cache is enabled
	
	Bit 8 PRFTEN: Prefetch enable
	0: Prefetch disabled
	1: Prefetch enabled
		
		Bits 2:0 LATENCY[2:0]: Latency
	These bits represent the number of HCLK (AHB clock) period to the Flash
	access time.
	000: Zero wait state
	001: One wait state
	010: Two wait states
	011: Three wait states
	100: Four wait states
	others: Reserved
 */
 
 FLASH->ACR|=(FLASH_ACR_LATENCY_4WS) | (FLASH_ACR_PRFTEN) | (FLASH_ACR_ICEN) | (FLASH_ACR_DCEN);
 
 
 
 /* 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	use Clock configuration register (RCC_CFGR)
	
	Bits 13:11 PPRE2[2:0]: APB high-speed prescaler (APB2)
	Set and cleared by software to control the division factor of the APB2 clock (PCLK2).
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16

	Bits 10:8 PPRE1[2:0]:APB low-speed prescaler (APB1)
	Set and cleared by software to control the division factor of the APB1 clock (PCLK1).
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16

	Bits 7:4 HPRE[3:0]: AHB prescaler
	Set and cleared by software to control the division factor of the AHB clock.

	0xxx: SYSCLK not divided
	1000: SYSCLK divided by 2
	1001: SYSCLK divided by 4
	1010: SYSCLK divided by 8
	1011: SYSCLK divided by 16
	1100: SYSCLK divided by 64
	1101: SYSCLK divided by 128
	1110: SYSCLK divided by 256
	1111: SYSCLK divided by 512
 */
 
	RCC->CFGR|=(RCC_CFGR_HPRE_DIV1) | (RCC_CFGR_PPRE1_DIV4) | (RCC_CFGR_PPRE2_DIV2);
	
	/* 5. Select the Clock Source and wait for it to be set */
	
	/*
		Bits 3:2 SWS[1:0]: System clock switch status
	Set and cleared by hardware to indicate which clock source is used as system clock.
	00: MSI oscillator used as system clock
	01: HSI16 oscillator used as system clock
	10: HSE used as system clock
	11: PLL used as system clock

	Bits 1:0 SW[1:0]: System clock switch
	Set and cleared by software to select system clock source (SYSCLK).
	Configured by HW to force MSI oscillator selection when exiting Standby or Shutdown mode.
	Configured by HW to force MSI or HSI16 oscillator selection when exiting Stop mode or in
	case of failure of the HSE oscillator, depending on STOPWUCK value.
	00: MSI selected as system clock
	01: HSI16 selected as system clock
	10: HSE selected as system clock
	11: PLL selected as system clock */
	
	
	/* we are selecting MSI as clock source */
	RCC->CFGR |= (RCC_CFGR_SW_MSI);
	
	while(!(RCC->CFGR & RCC_CFGR_SWS_MSI));
}	
		
#define UBOOT_START_ADDR 0x88000000
__attribute__((noreturn)) void jump_uboot(void)
{
	((void(*)(void))UBOOT_START_ADDR)();

}
#define REG32(X) (*(volatile unsigned int *)(X))
#define MAGIC_KEY 0x12345678
#define MAGIC_KEY_ADDR 0x80000000
#define ROM_ADDR 0x0
void delay(volatile unsigned int x)
{
	volatile unsigned int i,j;
	volatile unsigned int reg_value;
	
	for (i = x; i >0; i--){
		for (j = 0xFFFF; j >0; j--){
			reg_value = REG32(ROM_ADDR);
		}
	}
}
__attribute__((noreturn)) void jump(void)
{
	while(REG32(MAGIC_KEY_ADDR) != MAGIC_KEY)
	{
		delay(10);	
	}
	REG32(MAGIC_KEY_ADDR) = 0;
	((void(*)(void))(REG32(MAGIC_KEY_ADDR + 4)))();

}

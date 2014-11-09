_estack = 0x20004000;

_Min_Heap_Size = 0x0;
_Min_Stack_Size = 0x200;

MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
	RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 16K
}

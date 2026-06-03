ENTRY(start)

PHDRS
{
	RE	PT_LOAD FLAGS(5);	/* Read + Executable */
	R	PT_LOAD FLAGS(4);	/* Read */
	RW	PT_LOAD FLAGS(6);	/* Read + Write */
}

SECTIONS
{
	. = 1M;

	.text :
	{
		*(.multiboot)
		*(.text*)
	} :RE

	.rodata :
	{
		*(.rodata*)
	} :R

	.data :
	{
		*(.data*)
	} :RW

	.bss :
	{
		*(.bss*)
	} :RW

	/DISCARD/ :
	{
		*(.eh_frame*)
	}
}

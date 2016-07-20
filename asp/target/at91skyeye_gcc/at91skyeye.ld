OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm") 
OUTPUT_ARCH(arm)
STARTUP(start.o)

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);

/*
 * メモリサイズの指定
 */
MEMORY
{
    rom        : ORIGIN = 0x00000000, LENGTH = 0x00100000 /* 1Mbytes */
    ram        : ORIGIN = 0x00100000, LENGTH = 0x00400000 /* 4Mbytes */
    shared_mem : ORIGIN = 0x90000000, LENGTH = 0x10000    /* 64kbyte */
}


SECTIONS
{
    .text :
    {
        __text = . ;
        *(.vector)
        *(.text)
        *(.glue_7t)
        *(.glue_7)
        *(.vfp11_veneer)
    } > rom
    _etext = .	;
    PROVIDE (etext = .)	;
    .rodata  : { 
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.rodata1)
    } > rom

    . = ALIGN(4);
    __idata_start = . ;
    .data   :  {
        __data_start = . ;
        *(.data)
    } > ram
    _edata  =  . ;
    PROVIDE (edata = .);
    . = ALIGN(4);
    __bss_start = . ;
    .bss       :
    {
      *(.bss)
      *(COMMON)
   . = ALIGN(32 / 8);
    } > ram

   . = ALIGN(32 / 8);
     _end = .		;
    __bss_end = .;
    PROVIDE (end = .)	;
    .comment 0 : { *(.comment) }

    .shared_region (NOLOAD) :
    {
        *(.shared_region)
    } > shared_mem


  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to 
     the beginning of the section so we begin them at 0.  */

  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }

  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }

  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }

  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }

  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}

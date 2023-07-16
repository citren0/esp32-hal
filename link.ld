

/* // Default entry point
ENTRY( call_start_cpu0 );

// Specify main memory areas
MEMORY
{
  // Use values from the ESP-IDF 'bootloader' component.
  // TODO: Use human-readable lengths
  // TODO: Use the full memory map - this is just a test
  iram_seg (RX) : ORIGIN = 0x40080400, len = 0xFC00
  dram_seg (RW) : ORIGIN = 0x3FFF0000, len = 0x1000
}

// Define output sections
SECTIONS {
  // The program code and other data goes into Instruction RAM
  .iram.text :
  {
    . = ALIGN(16);
    KEEP(*(.entry.text))
    *(.text)
    *(.text*)
    KEEP (*(.init))
    KEEP (*(.fini))
    *(.rodata)
    *(.rodata*)

    . = ALIGN(4);
    _etext = .;
  } >iram_seg

  // Initialized data goes into Data RAM
  _sidata = .;
  .data : AT(_sidata)
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)

    . = ALIGN(4);
    _edata = .;
  } >dram_seg

  // Uninitialized data also goes into Data RAM
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;
  } >dram_seg

  . = ALIGN(4);
  PROVIDE(end = .);
  PROVIDE(_end = .);
}  */


/* esp32_technical_reference_manual_en.pdf 1.3.2 */
MEMORY {
  drom   (rw)   : ORIGIN = 0X3ff90000, LENGTH = 64k
  irom   (rwx)  : ORIGIN = 0x40000000, LENGTH = 384k

  cache0 (rwx)  : ORIGIN = 0x40070000, LENGTH = 32k
  cache1 (rwx)  : ORIGIN = 0x40078000, LENGTH = 32k
  iram   (rwx)  : ORIGIN = 0x40080400, LENGTH = 127k  /* First 1k is vectors */
  dram   (rw)   : ORIGIN = 0x3ffb0000, LENGTH = 320k

  dflash (rw)   : ORIGIN = 0X3f400000, LENGTH = 1024k
  psram  (rw)   : ORIGIN = 0X3f800000, LENGTH = 1024k
  iflash (rwx)  : ORIGIN = 0X400c2000, LENGTH = 11512k
}

_eram = ORIGIN(dram) + LENGTH(dram);
ENTRY(call_start_cpu0)

SECTIONS {
  /* .vectors  : { KEEP(*(.vectors))   } > iram */
  .text     : { *(.text*) *(.text)  } > iram

  .data : {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    *(.rodata)
    *(.rodata*)
    *(.gnu.linkonce.r.*)
    *(.rodata1)
    . = ALIGN(4);
    _edata = .;
  } > dram

  .bss : {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > dram

  . = ALIGN(4);
  PROVIDE(end = .);
  PROVIDE(_end = .);

  /*
  /DISCARD/ : { *(.debug) *(.debug*) *(.xtensa.*) *(.comment) }
  */
}
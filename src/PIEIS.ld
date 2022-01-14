ENTRY(_pistart)

SECTIONS
{
   . = 0x100;
   .text : { *(.text) }
}

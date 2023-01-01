OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

EXTERN(rkp_app_manifest)
ENTRY(rkp_init)

ENTRY_BASE = 0x33880000;
ENTRY_SIZE = 0x100000;
SECTIONS
{
    . = ENTRY_BASE;

    .text : AT(ENTRY_BASE) {
        __boot_start = .;
        KEEP(*(.text.boot))
        __boot_end = .;
        . = ALIGN(4096);
        __text_start = .;
        *(.text)
        *(.text.*)
        __text_end = .;
    }

    .rodata : ALIGN(4096) {
        __rodata_start = .;
        *(.rodata)
        *(.rodata.*)
        __rodata_end = .;
    }

    .data : ALIGN(4096) {
        __data_start = .;
        *(.data)
        *(.data.cb)
        *(.data.*)
        __data_end = .;
    }

    .bss : ALIGN(4096) {
        __bss_start = .;
        *(.init.bss.start)
        *(.bss)
        *(.bss.*)
        *(.init.bss.end)
        __bss_end = .;
    }
}

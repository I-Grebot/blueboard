@echo off
"tools/stm32flash.exe" -b 115200 -w debug/BlueBoard.elf.hex -v -g 0x0 COM12

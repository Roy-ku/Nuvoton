SET PATH=D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\Microsoft SQL Server\Client SDK\ODBC\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\120\DTS\Binn\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;D:\Development\1.Development_Tool\MinGW\bin;C:\Users\Roy\AppData\Local\Microsoft\WindowsApps;;E:\Software\Development\Microsoft VS Code\bin
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=M058LDN
SET CPU_VENDOR=Nuvoton
SET UV2_TARGET=PCF8574
SET CPU_CLOCK=0x00B71B00
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\main._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\adc._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\adc.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\clk._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\clk.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\gpio._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\gpio.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\pwm._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\pwm.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\timer._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\timer.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\uart._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\uart.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\i2c._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\i2c.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\user_delay._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\user_delay.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\pcf8574_hardware_driver._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\pcf8574_hardware_driver.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\retarget._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\retarget.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_m051series._ia"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via "listings\system_m051series._ip"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\system_m051series.__i"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\M051.lnp"
"D:\Development\1.Development_Tool\Keil_5\Core\ARM\ARMCC\Bin\fromelf.exe" ".\Objects\M051.axf" --i32combined --output ".\Objects\M051.hex"

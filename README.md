#Honor Magicbook Art 14 fan control daemon

Honor Magicbook Art 14 fan controls are pretty well managed by EC by default with only one issue.
The fans start to early - in fact laptop can do all office stuff including youtube background video with ambient music without fans blowing easily.
So the main goal of this daemon is to monitor the CPU temp and keep the fans off until certain value reached.
 
My experiments with 255H based Honor Magicbook Art 14 2025 version shows that 
/sys/class/hwmon/hwmon5/temp1_input reaches about 56000 ( 56 C) , so setting the threshold to around 60  looks reasonable to keep laptop silent.

Fan speeds can be monitored here
/sys/class/hwmon/hwmon5/fan1_input
/sys/class/hwmon/hwmon5/fan2_input
Fans are usually in sync , so for the simplicity just watch one of them.

battery current
/sys/class/hwmon/hwmon2/curr1_input
if it is more than 2500 (2.5 apms) better to switch on fans to cool the charging circuits.

In order to work the daemon needs 2 modules not included into main kernel tree 

1. Huawei WMI extended driver developed  by Ayman Bagabas  - https://github.com/aymanbagabas/Huawei-WMI
2. ACPI call module by Michal Kottman (available in many distros ann included into Cachy OS kernel)  - https://github.com/mkottman/acpi_call

What this daemon do - read temp and fan1 speed using sysfs. If the fan is not 0 and temp is below 55 - switch off fans. 
If the fan is 0 and temp is 55-62 - switch fans to cruise at 15%. where the fans are almost silent.
if the fan is below 3000 rpm and cpu temp is 62+ or if the battery is being charged at high rate (2.5+ amps ) - switch fan control to auto.

Then sleep for 3 seconds.

#ACPI calls to control fans:

set fan control to manual: echo "\_SB.PC00.LPCB.H_EC.WTER 0x20020C20 0x0a" > /proc/acpi/call 
0x0a - both fans , 0x02 - Fan0. 0x08 - Fan1

set fan control to auto: echo "\_SB.PC00.LPCB.H_EC.WTER 0x20020C20 0x00" > /proc/acpi/call 


set rpm:  echo "\_SB.PC00.LPCB.H_EC.WTER 0x20020A07 0" > /proc/acpi/call
0 - stop fan0, 100 - full speed
set rpm:  echo "\_SB.PC00.LPCB.H_EC.WTER 0x20020A08 0" > /proc/acpi/call
0 - stop fan1, 100 - full speed


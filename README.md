# avr-doorbell
This is an old project of mine, that I use as thesis of my Bachelor Degree in electronic engineering. It's a quite simple project that use Atmel ATMega 328p. It was the time when Nokia was the king of the phone, with it's mohophonic ringtone. So I decide to create a project in which use a nokia style composer as an input to modify the song for a simple device (doorbell).

It use a very simple uart protocol to input the ringtone, and can be use with any serial client interface. Back then, it's quite popular to use windows xp's hyperterminal, before it's gone in the next version of windows. It was also tested using putty, another well-known terminal emulation program on windows based OS.

The hardware design was very minimalistic, with the two buttons, and one usb interface, and the final stage of amplifier was just using a simpel non transistor before feeding to a small 8 ohm speaker.

The firmware compiled with avr-gcc on windows os, with the makefile generated also by avr-gcc project. Everything was put on main.c on the execption for uart handling. I think the current trend was to devide the source into several file and logically separated the program into small pieces. I don't bother changing it right now, since I just upload it as an archive of my previous project.

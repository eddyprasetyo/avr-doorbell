# avr-doorbell
This is an old project of mine, that I use as thesis of my Bachelor Degree in electronic engineering. It's a quite simple project that use Atmel ATMega 328p. It was the time when Nokia was the king of the phone, with it's mohophonic ringtone. So I decide to create a project in which use a nokia style composer as an input to modify the song for a simple device (doorbell).

It use a very simple uart protocol to input the ringtone, and can be use with any serial client interface. Back then, it's quite popular to use windows xp's hyperterminal, before it's gone in the next version of windows. It was also tested using putty, another well-known terminal emulation program on windows based OS.

The hardware design was very minimalistic, with the two buttons, and one usb interface, and the final stage of amplifier was just using a simpel non transistor before feeding to a small 8 ohm speaker.

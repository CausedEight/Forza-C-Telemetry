# Forza-C-Telemetry
This is a simple C program to display in-game telemtry from Forza Horizon 5 over TCP.
Since there is no official documentation as to how data is mapped in the packets (Thanks a lot Microsoft), I was able to find a general outline as to what data should be where.
This is the documentation used:
https://support.forzamotorsport.net/hc/en-us/articles/21742934024211-Forza-Motorsport-Data-Out-Documentation

If you wish to use this, just follow the following steps:
1) Enable DATA OUT within the in game settings
2) Set the IP address to your local IPv4 address and select the port to where you want to recieve the data. Note: Use a higher port to not interfere with essentail port communications
3) In the main.c file, change the port variable to the one you selected within the game.
4) (Maybe I'll make this easier and just make a CMake file) Compile to an executable and run

While I haven't worked on this in a while as I thought it was pretty good, I might come back and make it cleaner as I learn more.

-Slav

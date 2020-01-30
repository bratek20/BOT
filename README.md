# BOT
Simple Api for writing bots. Written in C++ and using WinApi.
Written with Visual Studio 2017 and CUDA 10.2. Works only for Windows!

In main function three bot programs can be added:
1) IconFinder - choose two points on the screen and save a rect between them. Now you can search for this rect on your screen, wherever it is.
2) KeyLogger - writes all pressed letter and number keys in terminal.
3) Clicker - enable save clicking and perform some left mouse clicks. Now you can turn on repeating process to repeat all your previous clicks. 

IconFinder is enabled by default. 
Controls:
p - record point position
c - capture rectangle between two last recorded positions
s - search with CPU
d - search with CUDA
0 - close the program

Search operations automatically moves mouse to the found position.
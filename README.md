
# Here will be some code for an esp32 with a display to show the departures for sl. This is the stockholm comunity traffic

wget http://api.sl.se/api2/realtimedeparturesv4.json?key=<KEY>&siteid=9192&timewindow=5

http://api.sl.se/api2/realtimedeparturesV4.json?key=KEY&siteid=9702&timewindow=60

# Also check out this TFT library
https://github.com/loboris/ESP32_TFT_library


#

 use the setting tft_disp_type = DISP_TYPE_ILI9341 with the first available WROVER kit,
Output is mirrored. Looks great through a mirror though, is there an easy fix?

Problem solved by setting
#define TFT_INVERT_ROTATION1 0

import sensor, image, time, math, pyb


"""
initiate LED objects, and turn LED off
"""

green_led = pyb.LED(2)
red_led = pyb.LED(1)
pyb.LED.off(green_led)

"""
Set resolution, and initiate camera sensor
These settings can be tweaked to find get the
furthest detection distance
"""
resX = 250  #  this is near the max resolution that the find AprilTags function works for
resY = 150

uart = pyb.UART(3, 9600, timeout_char = 0)
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.VGA)
sensor.set_windowing((resX, resY))
sensor.skip_frames(time = 800)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()

"""
Main Loop
if AprilTag is located in frame, a number representing its
location is sent to the microcontroller.
"""

while(True):
    clock.tick()
    img = sensor.snapshot()
    location = []  #  all locations of AprilTags in frame are added to list
    for tag in img.find_apriltags(families=image.TAG16H5):
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))  #  draws a rectangle around tag in OpenMV IDE window
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))  #  draws a cross in the center
        location = [tag.cx() - (resX / 2), - tag.cy() + (resY / 2)]  #  sets locations to coordinates where center is [0,0]
    if location:
        pyb.LED.off(red_led)
        pyb.LED.on(green_led)  #  turns on green led when tag is in frame

        """
         The following code identifies where the tag is in the frame
         and assings it an integer (finder) representing the location in the frame.
         This integer is sent to the microcontroller to direct it where to move.
         __________________________________
        |           |          |           |
        |           |          |           |
        |     3     |    2     |     1     |
        |           |          |           |
        |___________|__________|___________|
        |           |          |           |
        |     4     |    0     |     8     |
        |___________|__________|___________|
        |           |          |           |
        |           |          |           |
        |     5     |    6     |     7     |
        |           |          |           |
        |___________|__________|___________|
        """
        locX = location[0]
        locY = location[1]
        a = - resX / 6  #  the middle 6th of the frame is the center
        b = resX / 6
        c = -resY / 6
        d = resY / 6
        highX = locX > b
        midX = a <= locX <= b
        lowX = locX < a
        highY = locY > d
        midY = c <= locY <= d
        lowY = locY < c
        if midX is True and midY is True:
            finder = 0
        if highX is True and highY is True:
            finder = 1
        if midX is True and highY is True:
            finder = 2
        if lowX is True and highY is True:
            finder = 3
        if lowX is True and midY is True:
            finder = 4
        if lowX is True and lowY is True:
            finder = 5
        if midX is True and lowY is True:
            finder = 6
        if highX is True and lowY is True:
            finder = 7
        if highX is True and midY is True:
            finder = 8
        uart.write(str(finder) + "\n")  #  sends finder to microcontroller via serial
        pyb.delay(35)
    else:
        pyb.LED.off(green_led)
        pyb.LED.on(red_led)  #  red LED is turned on when tag is not in frame

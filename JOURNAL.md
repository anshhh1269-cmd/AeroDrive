# AeroDrive

AeroDrive is a standalone, ESP32-powered smart dashboard that uses Wi-Fi to turn live traffic and weather data into physical animations (a spinning wheel and turbine) and screen updates across four custom modes

# 2026-08-26: final github repo

**Total time spent: 0.45 hours**

aight so i edited everything in github and added the source files for 3d, pcb and firmware..

also added some more pics of 3d model in the readme 

thats it for this proj..putting in review now :)

![image.png](https://cdn.hackclub.com/01a03ecf-4509-76c8-b545-b4698f781432/image.png)

# 2026-08-26: did the firmware

**Total time spent: 2.1 hours**

got the full code up and running for the aeroDrive today. the main goal was to hook up our custom PCB to the ILI9341 screen, pull real-time API data over Wi-Fi, and trigger physical stuff like our turbine fan and engine sound effects using both hardware switches and touch inputs.

How the Physical Controls Work

I set up the code so we have tactile switches for quick cockpit control, but also touch gestures directly on the screen for extra settings.

The first switch toggles Traffic Mode. When you flip it, the ESP32 sends a quick command to the DFPlayer Mini over serial pins (IO21/IO22) to play an engine rev sound on the speaker then show live traffic stuff..

The second switch puts it into Wind Mode. The code pulls live wind speed and gust data from the Open-Meteo API. If live gusts go over 15 km/h, the ESP32 sends pin IO13 HIGH, opening our 2N7000 MOSFET gate to spool up the physical 5V turbine fan; can also tap the screen to trigger a manual 5-second fan boost or swap speed units.

The third switch starts a 25-minute Pomodoro focus timer. It instantly kills the fan, stops the stepper motor, and mutes the audio module.

okay and also i wanted to use google map api like in my firmware there is a line of code for the google map API but because it just requires a card no money tho so i didnt out the card yet..will check it out later :)

![image.png](https://cdn.hackclub.com/01a03eb2-c79f-79df-bcc5-e5641b462d36/image.png)

# 2026-08-17: pcb in fusion

**Total time spent: 1.2 hours**

so i had to check the holes made in fusion if they align with the real pcb..so obvi i need a pro easyeda account to do it in step so i had to do the .obj which took the pcb into a mesh..used the convert mesh thing and it became a body..

next thing was that this was wayy too big..idk i searched it is supposed to import stuff with a scale of one but this was huge..so i had to figure out the scale which i did and then used the move feature..

the move feature took many tries because the screw holes on this pcb wasnt like perfect holes idk why the center of the hole wasnt seletcing so i had to do many trails and errors..more time was spent finding my 3d model when im zoomed in on the pcb cause they were very far apart and size diff before the scale..

i did use the move thingy successfully then i imported the m3 screws and aligned them on two holes diagonally because we dont need four screws ngl..

thats it for this session..next would be working on the repo like bom and 3d stuff

dont have lapse for this cause lapse wasnt allowing me to record in fusion..but honestly this took me around one hr cause it made it kinda difficult..

![image.png](https://cdn.hackclub.com/01a00e5d-10fd-78bc-bb40-5bcf58e187d5/image.png)

# 2026-08-15: screws and pcb

**Total time spent: 1.1 hours**

so basically this session was to add screw holes on my 3d enclosure..now the pcb was kinda quite big than the space my 3d enclosure had for the pcb so i had to remove the holes again and edit the whole thing in easyeda pcb editor...

after doing that i used a measure and measured the length and the width and create 3mm holes 5mm deep for the screws to go in..i am struggling to add the pcb just to showcase that the holes are fine so i might not be able to do it..

thats it for tdy

lapse link: https://lapse.hackclub.com/timelapse/Gh2aqhBUK-Ux

![image.png](https://cdn.hackclub.com/01a003ce-0087-76f9-90c9-1c7cff865e66/image.png)



# 2026-08-14: just made readme

**Total time spent: 0.567 hours**

so i wrote the readme for the github repo took some time cause i had to get the info for the switches again cause i kinda forgot :(

i have added all the pcb pics but 3d enclosure is still left because i have to make screw holes for the pcb in the enclosure which will take some time..

thats it 

![image.png](https://cdn.hackclub.com/01a000db-9748-7161-8b5d-8dc2fb2cf7e5/image.png)

# 2026-08-09: last part for the pcb :)

**Total time spent: 1 hour**

so basically i had fogotten to add the six pin header component for the three switches so its easy to connect. found many error going through that process but it came out perfectly i think with no DRC errors; however i have also started seeing the best website to order pcbs form which is cheap and can ship to my location :)

thats it for tdy

lapse link: https://lapse.hackclub.com/timelapse/abz-Fy-Vbi5F

![image.png](https://cdn.hackclub.com/019fe59f-c26f-7894-8462-7e6909e9365f/image.png)

# 2026-08-08: time for the pcbbb

**Total time spent: 1.1 hours**

so tdy was finishing up the schematics and also the pcb; so basically i got all the components together and routed all the connections for the pcb editor; DRC errors are zero :)

also did the copper area for gnd and also the 3d view of it looks amazing..i think next is pcb and finishing up the github before this is ready for shipping :)


lapse: https://lapse.hackclub.com/timelapse/hXL5Sw_q_Uqc

![image.png](https://cdn.hackclub.com/019fe1f4-e450-7e2b-b8d8-4058a6b2b2e2/image.png)

![image.png](https://cdn.hackclub.com/019fe1f5-274d-796c-a17d-61e3250010e6/image.png)



# 2026-08-04: schematic finished

**Total time spent: 0.35 hours**

yup, added all the crosses and also added header pins for the external components: fan, speaker and stepper motor

![image.png](https://cdn.hackclub.com/019fcda7-6c5b-765e-8c6b-6993db548bf7/image.png)

# 2026-08-04: schematic part 2

**Total time spent: 1.75 hours**

aight so im using easyeda for this instead of kicad idk why just doing it..

so i had put the wrong esp32 cause it didnt have 5V so that took a a lot of time to replace but im done rn for the schematic..next thing is for pcb..

however theres external connectors like fan, motor and speaker..ai told me to make header pins so maybe ill do tht or just leave it as it is and not cross them out but i might as well add the header pins lets see

thats it..i ahve added lapse links

https://lapse.hackclub.com/timelapse/jDW9MZFCGTUQ

https://lapse.hackclub.com/timelapse/JVzdDjOY-GFX

![image.png](https://cdn.hackclub.com/019fcd97-4597-7349-a3c0-8449b60afa7f/image.png)

# 2026-07-30: worked in easyeda

**Total time spent: 0.8 hours**

so basically i had to start the wiring tdy..used easyeda and imported the player, the speaker, esp32, tft screen, mosfet and resistors..

now i wanted to try a touchscreen tft component so i asked AI what could be the features if i used touch...it wouldn't be much, but it would be cool tho

so the touch screen would basically just control the locations for the traffic input the user wants, along with the study timer..didnt do much research yet but still doing it..

this session did very little wiring, only the esp32 one side so ill work on it later..

![image.png](https://cdn.hackclub.com/019fb15c-2b42-718c-a6e9-3ac5e2a6c2ed/image.png)

lapse: https://lapse.hackclub.com/timelapse/-4e8cCRhQGJV

# 2026-07-26: added switches

**Total time spent: 0.67 hours**

so the next thing was to work on the switches..it was quite simple to make the switches holes which was 3 on the top face of the main body, which took around 20 ish mins..

after that i had to figure out the purpose..the main goal was to hv something that connected with real-world live data and also doesn't become a headache if put near a study table..so the purposes of the switches are as follows:

Switch 1 (Traffic Mode): Revs an engine sound on startup, then continuously spins the car wheel fast for clear roads and slow for heavy traffic.

Switch 2 (Wind Mode): Displays live weather on the screen and briefly spools up the turbine fan whenever real outdoor wind gusts occur.

Switch 3 A 25-minute Pomodoro timer that mutes all motors for quiet studying, with double-tap display dimming and a long-press demo sweep.

![image.png](https://cdn.hackclub.com/019f9ede-f6c6-7478-8896-243c80f6dcd7/image.png)

# 2026-07-26: figured out wheel movement (part 2)

**Total time spent: 2.3 hours**

soo left the project for some while to focus on my other build proj...

checked the motor once again and i noticed a gap; the motor shaft wasnt fully inside the hole made, the hole was bigger than expected..

so i went back to the timeline, checked the radius of the curved part of the shaft which turned out to be 2.50mm, so i used filet for the rectangle hole and made it 1.25mm on both sides...now this was a grave mistake

the hole was no so big on the width side, rather than the length and now it looked even worse so 45 mins into the garbage..then i finally used my mind used project and made the hole exactly the one which was needed..

so this kinda took some time to do since i had to go back to the timeline and also find and delete any extrude made on the hole so that the project option remains valid. 

![Screenshot_2026-07-26_131915.png](https://cdn.hackclub.com/019f9ed9-b7a9-7c87-937b-d13746fe157d/Screenshot_2026-07-26_131915.png)

![Screenshot_2026-07-26_133437.png](https://cdn.hackclub.com/019f9ed9-d205-7fab-8b46-8f8767fc45a1/Screenshot_2026-07-26_133437.png)

![image.png](https://cdn.hackclub.com/019f9eda-e2dd-7f3c-b2bf-6ac0c2105359/image.png)

# 2026-07-16: figured out the wheel's movement

**Total time spent: 2.2 hours**

today was the day where I was checking how the wheel would move, because the wheel didn't have any open hole that would allow the motor shaft to turn the wheel. 

so basically the motor I was using was the 28BYG-48, and I asked Gemini for the exact dimensions of the motor shaft, which turned out to be not a simple rectangle but one with curved ends on two sides..

so i extruded a cylinder on the center of the wheel to 9 mm and then took the dimensions from Gemini to make the curved rectangle and extruded it to negative 8.5mm..

however, i still needed confirmation...so i went onto GrabCAD, found the exact model, and imported it right onto the hybrid assembly..after checking the exact size of the curved rectangle needed i was finally done for tdy..

tmr is figuring out how to screw the motor onto something so that when the wheel spins, it just doesn't come out of the main body while spinning. 

thats it for tdy :)

![Screenshot 2026-07-15 121427.png](https://cdn.hackclub.com/019f6bc6-3572-7c12-b29c-3df2d7d65437/Screenshot%202026-07-15%20121427.png)

![Screenshot 2026-07-15 121534.png](https://cdn.hackclub.com/019f6bc6-4714-78df-8b0e-60308fa9e2b3/Screenshot%202026-07-15%20121534.png)

![Screenshot 2026-07-15 210558.png](https://cdn.hackclub.com/019f6bc6-681a-713d-815d-2125cc90e852/Screenshot%202026-07-15%20210558.png)

# 2026-07-15: finished the back cover

**Total time spent: 1.89 hours**

so the back cover was such a headache because the main body is fully curved...not like my other projects...so I first made the outline of the back cover through sketching and taking in all of the dimensions.

Then I had to fillet it...obviously I had extruded the second inner rectangle more than everything else, so the cover fits on perfectly.

after trying to make it curve, idk why I didn't know what corner to choose for the fillet option, as the curve wasn't coming to the extent that I wanted it to. took a day off and thought about which line to take..and then it just snapped

the line should be the inside line of the extrude, and when I finally tried it, it worked!!

took a lot of time for the back cover ngl :(

thats it for tdy :)

![Screenshot 2026-07-15 112436.png](https://cdn.hackclub.com/019f640d-e05a-7136-accb-25f4a1604e1f/Screenshot%202026-07-15%20112436.png)

![Screenshot 2026-07-15 112851.png](https://cdn.hackclub.com/019f640d-f4c9-7798-a693-91dbe21ca1b1/Screenshot%202026-07-15%20112851.png)

![Screenshot 2026-07-15 112901.png](https://cdn.hackclub.com/019f640e-12f2-727a-ada1-1cc7146cdef8/Screenshot%202026-07-15%20112901.png)

# 2026-07-12: made the CAD better

**Total time spent: 2.067 hours**

so after researching for more features; my mind came across adding three switches; one of the switch would translate the screen for showing real time traffic stats and some local place journey time..second button would be for wind speed; and the traffic and wind speed would translate to the speed of the wheel turning and the fan behind the turbine.

now in fusion 360, i used shell and then made the holes bigger; added the imports and then checked yet again that the holes were too big and just kept on doing trial and error to perfect the scale factor of the imports..

reached on smthng then found the correct tft screen needed..and made borders so that the only thing visible outside would be the screen outline and nothing else..i think it looks a bit professional too now

second thing the third switch would be alarm for some kind so i would be also using a buzzer just maybe btw...if i have space remaining

the next thing to do is to draw the cut outs for the switches, then move onto the wiring diagram after making CAD better. 

thats it for tdy :)

![Screenshot 2026-07-13 001200.png](https://cdn.hackclub.com/019f5751-eeee-7afe-9da3-de68a408371e/Screenshot%202026-07-13%20001200.png)

![Screenshot 2026-07-13 001251.png](https://cdn.hackclub.com/019f5752-01d4-79a1-8f32-7c99bb61df9d/Screenshot%202026-07-13%20001251.png)

# 2026-07-12: started aerodrive 

**Total time spent: 1.56 hours**

so i first started aerodrive..looking at all features I can build by using AI; including the shape, the parts needed.

so first i went to grabcad...and i searched for aircraft turbines, which took some while to get the correct one along with the car wheels; then thought of having some action from these parts; so basically the car wheel spins and the turbines will have a fan behind it which would spin too according to the firmware which is a secret rn..

and then went to fusion 360, started a new project..and created a rectangle and use filet; along with importing these parts and creating the circles and rectangles for the input..

lastly, i used shell for the first time to remove the back cover and then hope to create a back cover

![Screenshot 2026-07-12 153239.png](https://cdn.hackclub.com/019f5718-895b-7a1d-ba42-b15688f264e1/Screenshot%202026-07-12%20153239.png)

![Screenshot 2026-07-12 230632.png](https://cdn.hackclub.com/019f5718-ae46-71e2-9021-b887f84da81e/Screenshot%202026-07-12%20230632.png)

![Screenshot 2026-07-12 230612.png](https://cdn.hackclub.com/019f5718-d255-7f66-a016-ebbd923ba8fa/Screenshot%202026-07-12%20230612.png)


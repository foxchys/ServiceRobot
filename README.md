# ServiceRobot

The project was designed for a service robot systerm。  
##UI
Here's the user interface.The main project at the file "Srobot1.0".From the interface we can see that the robot can talk with us , face recognition, autonomous movement and simple action performance.
![UI1](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/ui1.png)  
![UI2](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/ui2.png)  
## Face Recognition
You can upload faces on the spot or you can pre-upload faces.After the information is entered, the robot can recognize the faces in the camera. We use the [opencv_CascadeClassifier](https://docs.opencv.org/3.4.0/d1/de5/classcv_1_1CascadeClassifier.html#a6d01a748b103f0cd6bd2a20037ae8731) to detect the face in camera.After detecting the face in the camera,we use [facepp](https://www.faceplusplus.com.cn/) service for face recognition.Click [here](https://console.faceplusplus.com.cn/documents/4888381) to see the documentation for facepp.
![Facepp](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/facepp.png)
## Smart Dialog System
This system allows the robot to talk to us.Just like "Siri" the robots can talk to people.Use windowsapi for sound acquisition.[Xunfly Service](https://www.xfyun.cn/) help the robot take Voice to text.[Here](https://www.xfyun.cn/services/voicedictation) is Xunfly document.
![Xunfly](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/xunfly.png)  
After the robot recognizes our voice.We post the message to [TURING_RoBoT](http://www.tuling123.com/).![TURING_RoBoT](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/TURING_RoBoT.png)If you want to get more information about TURING_RoBoT please click [here](https://www.kancloud.cn/turing/www-tuling123-com/718218).To ensure the speed of speech synthesis.We use [Microsoft Speech SDK 5.1](http://www.microsoft.com/en-us/download/details.aspx?id=10121) to synthesize the information returned by the TURING_RoBoT.Now the robot can talk with us!  
## Wheel Robot Arm and Robot Face
DC motor universal wheels is used for the moving module of the robot.We use STC89C52![89C52](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/89c52.png) as the underlying driver.A 32*32 LED module is used for the face dispaly.The code of the modules at the file "Wheel" and "FaceDisplay".For robot arm,the robot arm is made of [digital servo MX-106](http://support.robotis.com/en/product/actuator/dynamixel/mx_series/mx-106.htm)
![MX-106](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/mx-106.png)The upper computer system is coordinated with the lower computer and the mechanical arm through the serial port.More information about serial port please visit [my project](https://github.com/NikofoxS/serial_assistant.git).
##  Other Library
We haved used many librarys in this project.We used [libcurl](https://curl.haxx.se/libcurl/) and [JsonCpp](https://github.com/open-source-parsers/jsoncpp.git) for network communications.  
## Review and summarize
This project was written when I first learned C++.There are a lot of inaccuracies at the code.After a long time I uploaded the project to github.This project can be a start for me to learn C++.My senior fellow apprentice Xujie Li gave me great help.I can't finsh this project without him.If you have questions about my code please contact me.  





<div style="text-align: right"> NikofoxS </div>  
<div style="text-align: right"> Email:chy_s@outlook.com </div>  
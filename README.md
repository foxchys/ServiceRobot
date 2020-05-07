# ServiceRobot

This project was designed for a service robot。  
## UI
Here's the user interface.The main project at the file "/Srobot1.0".From the interface we can see that the robot can talk to us, perform facial recognition and take simple actions.  
![UI1](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/ui1.png)  
![UI2](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/ui2.png)  
## Face Recognition  
For the input of face information.You can upload faces on the spot or you can pre-upload faces.When the information is uploaded, the robot can recognize the face captured by the camera. We use the [opencv_CascadeClassifier](https://docs.opencv.org/3.4.0/d1/de5/classcv_1_1CascadeClassifier.html#a6d01a748b103f0cd6bd2a20037ae8731) to detect the face in the pictures.After the faces are detected,we use [facepp](https://www.faceplusplus.com.cn/) service for face recognition.Click [here](https://console.faceplusplus.com.cn/documents/4888381) to see the documentation of the facepp.
![Facepp](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/facepp.png)
## Intelligent Conversation System
Just like "Siri" the robot can talk to people.Use windows_api for voice detection.[Xunfly Service](https://www.xfyun.cn/) can help us take Voice to text.[Here](https://www.xfyun.cn/services/voicedictation) is the documentation of Xunfly.
![Xunfly](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/xunfly.png)  
After our voice is recognized .We post the message to [TURING_RoBoT](http://www.tuling123.com/).![TURING_RoBoT](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/TURING_RoBoT.png)  
If you want to get more information about TURING_RoBoT please click [here](https://www.kancloud.cn/turing/www-tuling123-com/718218).To ensure the speed of speech synthesis.We use [Microsoft Speech SDK 5.1](http://www.microsoft.com/en-us/download/details.aspx?id=10121) to synthesize the information returned by the TURING_RoBoT.Now the robot can talk with us!  
## Wheel Robot Arm and Robot Face  
DC motor wheels is used for the moving module of the robot.We use STC89C52![89C52](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/89c52.png) as the lower computer.A 32*32 LED module is used for the pictures dispaly.The code of the modules at the file "/Wheel" and "/FaceDisplay".For the manipulator,the manipulator is made of [digital servo MX-106](http://support.robotis.com/en/product/actuator/dynamixel/mx_series/mx-106.htm)(or this [site](http://emanual.robotis.com/))   
![MX-106](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/mx-106.png)   
The connection between the upper computer and the lower computer through serial port.More information about serial port please visit [my project](https://github.com/foxchys/serial_assistant).
## Other Library  
Many librarys are used in this project.We used [libcurl](https://curl.haxx.se/libcurl/) and [JsonCpp](https://github.com/open-source-parsers/jsoncpp.git) for network connections.  
## Review and summarize  
This project was programmed when I first learned C++.There are a lot of problems at the code.After a long time I uploaded the project to github.This project can be a "Hellow World" for me to study C++. My senior fellow apprentice X. Lee had gaved me great help.I can't finsh this project without him(The program he wrote is at the folder "Service_robot_arm_L").If you have questions about my code please contact me.  





Foxchys   
Email:chy_s@outlook.com

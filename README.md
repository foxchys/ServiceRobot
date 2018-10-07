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
DC motor universal wheels is used for the moving module of the robot.We use STC89C52![STC89C52](https://raw.githubusercontent.com/NikofoxS/ServiceRobot/master/pictures/stc89c52.png) as the underlying driver

[MX-106](http://support.robotis.com/en/product/actuator/dynamixel/mx_series/mx-106.htm)
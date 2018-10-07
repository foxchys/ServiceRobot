# face_recognition
    face_recognition_pyhon_cpp
    This project is developmented by 32 bit Windows.
    It is convenient for dlib to install and compile on Ubuntu, but because the first half of my project is in the Windows environment, I want to configure it on Windows.
        I had a lot of problems halfway through.
    The python is packaged as a function module, and the python function module is called by c++ to obtain the face recognition results.
    Refer to this project https://github.com/ageitgey/face_recognition#face-recognition
    The development environment used in this project is python2.7 (Anaconda2).
    Use the face recognition module in dlib.Install this module (or Python 2 pip2) using pypi's pip3.Run pip3 install face_recognition to     complete the installation by manually installing dlib.
    When installing dlib,the following points need to be noted.
    Install the Cmake compiler. (https://cmake.org/)
    First download the boost library(https://www.boost.org/), based on the tips: 
    (1)Run bootstrap. Bat. Generate three files, including b2.
    -- the bootstrap. Bat 
    (2)Run the b2
    - b2 install
    (3) compile library files with b2.
    -- b2-a --with-python address-model=64 toolset= MSVC runtime-link=static.
    Here are some problems, the address-model=64 parameter setting has a problem, and then 32 is successful.
    (4) set variables
    - set BOOST_ROOT = C: \ local \ boost_1_57_0
    - set BOOST_LIBRARYDIR = C: \ local \ boost_1_57_0 \ stage \ lib
    Set these two variables to the Path variable: %BOOST_ROOT%;% % BOOST_LIBRARYDIR
    (5) then run the PIP install dlib.
    successful
    Dlib can be downloaded on the lot (https://github.com/davisking/dlib);Or go to dlib 'home page (http://dlib.NET/).
    C++ Runtime Error occurred, Error code R6034.  It's a strange question.Refer to stackoverflow.(http://stackoverflow.com/questions/14552348/runtime-error-r6034-in-embedded-python-application)
    Using the process explorer to view the dependencies, two msvcr90. DLLS were found, one in the VisualSVN Server directory and one under the system WinSxS directory.Finally, the test found that the uninstall VisualSVN or delete the msvcr90. DLL under the directory of anaconda2 (under the directory of anaconda2 or anaconda2\Library\bin), the program can run normally and no longer report the error of R6034.

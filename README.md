# tsunami
Remote Administration Tool | Qt-based Client Server Application

tsunami is a spy-ware consisting of a Windows-based Trojan that will act as a Remote Administration Tool to remotely connect and manage a single or multiple computers and perform unwanted actions ranging from basic data collection to file erasure and uploading, restart and shutdown etc.

#Compiling
The code can be compiled in  Qt 5+. Qt can be downloaded from:  http://qt-project.org/downloads.
After you’ve downloaded it, you’ve got to install it using the setup. Once installed, you will have to open the .pro file which is the project file and compile it by clicking the build button.


#Executing
Once you have build the project you can obtain an exe in either debug or release folder depending on weather your build was debug or release. For example, C:\Qt\Qt5.0.2\Tools\QtCreator\bin\debug\tsunami

This exe will only run in Qt Creator (since the path variables are set by default in Qt). To make this exe standalone, you have to include the dependencies in the folder too. You’ll need 3 dlls for Webkit support, 2 for OpenGL support, 4 or 5 of Qts own dlls and a few dlls from Microsoft. In addition you’ll need to include a plugins folder too with the required plugins. 

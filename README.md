# Classic_AUTOSAR_CPP
* This project is done for fun and practice of the C++11/14 new features. Industrial Classic AUTOSAR is implemented using Embedded C.
* The modules implements parts of the communication stack, such as Transmission/Reception, mode control(only going to full communication on Lin is currently supported) and Lin schedule tables.
* Starting point is LinDemo/Main.cpp by invoking the Execute function of the ECU object.
* Modules configurations are done inside the LinDemo/Config.cpp 
* The Demo starts and goes into Full Communication mode, then after timeout tries to sleep(Sleep is currently not supported so it wakeup again).
* To build, import the project into eclipse CDT and build the LinDemo app.
* The rest of the projects are the tests for each module and the source code - tested with Gtest/Gmock.

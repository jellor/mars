#SET(LIBRARY_OUTPUT_PATH ./)
SET(EXECUTABLE_OUTPUT_PATH ../bin)

#指定头文件的搜索路径, 相当于指定gcc的-I参数
INCLUDE_DIRECTORIES(../src)
#动态链接库或静态链接库的搜索路径, 相当于gcc的-L参数
LINK_DIRECTORIES(../lib)

#ADD_XXXX必须在TARGET_LINK_LIBRARIES前面, 否则会报错

#添加子目录
#ADD_SUBDIRECTORY()

#添加编译参数
ADD_DEFINITIONS(-std=c++11 -Wlogical-op-parentheses -rdynamic -g)

#编译生成程序库
#ADD_LIBRARY()

#指定生成目标
ADD_EXECUTABLE(client client.cpp)
#添加链接库, 相同于指定-l参数
TARGET_LINK_LIBRARIES(client mars)

ADD_EXECUTABLE(server server.cpp)
TARGET_LINK_LIBRARIES(server mars)

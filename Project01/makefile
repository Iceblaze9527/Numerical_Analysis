GPP = g++

WORKSPACE_FOLDER = /Users/xiangyutong/Documents/数值分析与算法/assignments/ass01/
INCLUDE_PATH = /usr/local/Cellar/opencv/4.1.2/include/opencv4
LIBRARY_PATH = /usr/local/Cellar/opencv/4.1.2/lib

INCLUDE = -I $(INCLUDE_PATH)
LIBS = -L $(LIBRARY_PATH)
LINK = -l opencv_highgui -l opencv_core -l opencv_imgcodecs
STD = -std=c++17

SRCS = interpolation.cpp
PROG = interpolation

$(PROG):$(SRCS)
	$(GPP) $(INCLUDE) $(LIBS) $(LINK) $(STD) $(SRCS) -o $(PROG)
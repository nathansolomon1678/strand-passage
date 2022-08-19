AM_CXXFLAGS = -g -fpermissive -std=c++11
AM_CFLAGS =
AM_LDFLAGS =

all: mmc zAnalyzer idknot homfly xinger

bin_PROGRAMS = mmc zAnalyzer idknot homfly xinger

obj_src = src/argprocessor.cpp src/autocorr.cpp src/CLK.cpp src/conformationAsList.cpp src/conformation.cpp src/legacy.cpp src/mmchain.cpp src/regularNgon.cpp src/zAnalyzer.cpp src/id.cpp src/vertex.cpp src/edge.cpp src/contiguous_list.cpp

mmc_SOURCES = $(obj_src) src/mmcMain.cpp

zAnalyzer_SOURCES = $(obj_src) src/zAnalyzerMain.cpp

idknot_SOURCES = $(obj_src) src/idknot.cpp

homfly_SOURCES = $(obj_src) src/cross.c src/polynomesAZ.c src/homfly1_21.c

xinger_SOURCES = src/xinger/main.cpp src/xinger/random.cpp src/xinger/util.cpp src/xinger/xinger.cpp

ACLOCAL_AMFLAGS = -I m4 --install
EXTRA_DIST = m4/NOTES

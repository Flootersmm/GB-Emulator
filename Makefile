.PHONY: clean doxygen

IDIR = include
IDIR_DEAR_IMGUI = include/dear_imgui
CC = clang
CXX = g++
USERFLAGS +=
override CFLAGS += -I$(IDIR) -I$(IDIR_DEAR_IMGUI) `sdl2-config --cflags` -g -Wall -Wpedantic $(USERFLAGS) -std=c11 -Wformat-extra-args
override CXXFLAGS += -I$(IDIR) -I$(IDIR_DEAR_IMGUI) `sdl2-config --cflags` -g -Wall -Wpedantic $(USERFLAGS) -std=c++11
PEDANTIC_CFLAGS = -std=c11 -Werror -Wpedantic -Wall -Wextra -Wformat=2 -O -Wuninitialized -Winit-self -Wswitch-enum -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Wno-long-long -Wglobal-constructors -Wshorten-64-to-32

ODIR = obj
SRCDIR = src

LIBS = -lm -ldl -lreadline
GUI_LIBS = -lglfw -lGL `sdl2-config --libs`

DEPS = $(wildcard $(IDIR)/*.h) $(wildcard $(IDIR_DEAR_IMGUI)/*.h)
CSRCS = $(wildcard $(SRCDIR)/*.c)
CPPSRCS = $(wildcard $(SRCDIR)/*.cpp) src/imgui_impl_glfw.cpp src/imgui_impl_opengl3.cpp
COBJS = $(patsubst $(SRCDIR)/%.c,$(ODIR)/%.o,$(CSRCS))
CPPOBJS = $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(CPPSRCS))

DEPS2 := $(COBJS:.o=.d) $(CPPOBJS:.o=.d)

all: gui

$(COBJS) $(CPPOBJS) $(ODIR)/main.o: $(DEPS)

-include $(DEPS2)

$(ODIR)/%.o: $(SRCDIR)/%.c
	+@[ -d $(ODIR) ] || mkdir -p $(ODIR)
	$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	+@[ -d $(ODIR) ] || mkdir -p $(ODIR)
	$(CXX) -MMD $(CXXFLAGS) -c -o $@ $<

gui: $(COBJS) $(CPPOBJS) $(ODIR)/imgui_wrapper.o $(ODIR)/main.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(GUI_LIBS)

doxygen:
	doxygen

clean:
	-rm -f $(ODIR)/*.o 
	-rm -f $(ODIR)/*.d
	-rm -f gui
	-rm -f cpu_log.txt
	-rm -rf obj/ *.dSYM
	-rm -f valgrind-*.txt

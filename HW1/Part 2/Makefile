all: compile link

INTELMAC_INCLUDEDIR=/usr/local/include			# Intel mac
APPLESILICON_INCLUDEDIR=/opt/homebrew/include	# Apple Silicon
UBUNTU_APPLESILICON_INCLUDEDIR=/usr/include		# Apple Silicon Ubuntu VM
UBUNTU_INTEL_INCLUDEDIR=/usr/include			# Intel Ubuntu VM

INTELMAC_LIBPATH=/usr/local/lib 							# Intel mac
APPLESILICON_LIBPATH=/opt/homebrew/lib						# Apple Silicon
UBUNTU_APPLESILICON_LIBPATH=/usr/lib/aarch64-linux-gnu		# Apple Silicon Ubuntu VM
UBUNTU_INTEL_LIBPATH=/usr/lib/x86_64-linux-gnu				# Intel Ubuntu VM

MACOS_INCLUDE=$(INTELMAC_INCLUDEDIR)
MACOS_LIB=$(INTELMAC_LIBPATH)
UBUNTU_INCLUDE=$(UBUNTU_INTEL_INCLUDEDIR)
UBUNTU_LIB=$(UBUNTU_INTEL_LIBPATH)

MACOS_COMPILER=/usr/bin/clang++
UBUNTU_COMPILER=/usr/bin/g++

uname_s := $(shell uname -s)
compile:
ifeq ($(uname_s),Darwin)
	$(MACOS_COMPILER) -c part2.cpp -I$(MACOS_INCLUDE)
else ifeq ($(uname_s),Linux)
	$(UBUNTU_COMPILER) -c part2.cpp -I$(UBUNTU_INCLUDE)
endif

uname_s := $(shell uname -s)
link:
ifeq ($(uname_s),Darwin)
	$(MACOS_COMPILER) part2.o -o sfml-test -lsfml-graphics -lsfml-window -lsfml-system -L$(MACOS_LIB)
else ifeq ($(uname_s),Linux)
	$(UBUNTU_COMPILER) part2.o -o sfml-test -lsfml-graphics -lsfml-window -lsfml-system -L$(UBUNTU_LIB)
endif

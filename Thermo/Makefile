#
# Copyright (c) 2018      University of Houston. All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#
include ../Makefile.defs

HEADER = ThermoDataFile.h    \
	 ThermoDynamicData.h \
	 ThermoGlobalParams.h\
	 ThermoStaticData.h

OBJ = 	ThermoDataFile.o     \
	ThermoDynamicData.o  \
	ThermoGlobalParams.o \
	ThermoStaticData.o

.SUFFIXES: .o.cpp

all:    
	make $(MZLIB_LIB) 


$(MZLIB_LIB): $(HEADER) $(OBJ) 
	$(AR) $(AR_FLAGS) $(MZLIB_LIB_DIR)/$(MZLIB_LIB) $(OBJ)

$(OBJ):	$(HEADER)
.cpp.o:	$(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

clean:  
	rm -rf *.o *~

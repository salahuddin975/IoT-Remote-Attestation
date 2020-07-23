CXX=clang++ -fpermissive
RM=rm -f
CPPFLAGS= -std=c++11 -stdlib=libstdc++
LDLIBS=-lwiringPi -lpthread -lssl -lcrypto
SC=-fno-stack-protector -z execstack 

SRCS=main.cpp server.cpp zone_controller.cpp  checksum.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: IrrigationServer

IrrigationServer: $(OBJS)
	    $(CXX) $(LDFLAGS) -o IrrigationServer $(OBJS) $(LDLIBS) $(SC)

depend: .depend

.depend: $(SRCS)
	    $(RM) ./.depend
	    $(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	    $(RM) $(OBJS)

distclean: clean
	    $(RM) *~ .depend

include .depend

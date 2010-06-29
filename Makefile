SOURCE=main.cpp serv.cpp mysock.cpp log.cpp dv_algo.cpp
HEADER=serv.h mysock.h log.h
all: DV_routing

DV_routing: $(SOURCE) $(HEADER)
	g++ $(SOURCE) -o DV_routing


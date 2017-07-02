CC=g++
FLAG=-c
OUTPUT=-o
SRCLIST=$(wildcard *.cpp)
OBJLIST=$(SRCLIST:%.cpp=%.o)
OBJ=online_disk_client

$(OBJ):$(OBJLIST)
	$(CC) $(OBJLIST) $(OUTPUT) $@

%.o:%.cpp
	$(CC) $(FLAG) $< $(OUTPUT) $@

.PHONY:clean
clean:
	rm -rf *.o $(OBJ)



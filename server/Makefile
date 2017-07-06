CC=g++ -lmysqlclient
FLAG=-c
OUTPUT=-o
SRCLIST=$(wildcard *.cpp)
OBJLIST=$(SRCLIST:%.CPP=%.O)
OBJ=online_disk_server

$(OBJ):$(OBJLIST)
	$(CC) $(OBJLIST) $(OUTPUT) $@
%.o:%.c
	$(CC) $(FLAG) $< $(OUTPUT) $@ 

.PHONY:clean
clean:
	rm -rf *.o online_disk_server

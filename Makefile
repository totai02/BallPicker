CC=g++
DEPS = armcontrol.h ballpicker.h config.h detectball.h motorcontrol.h timer.h
OBJ = main.o armcontrol.o ballpicker.o config.o detectball.o motorcontrol.o timer.o
OPENCV = `pkg-config opencv --cflags --libs`
LIBS    = -lwiringPi -lpigpio $(OPENCV)
PROG = FruitPicker

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(LIBS)

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

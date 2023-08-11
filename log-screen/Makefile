PP=g++ -std=c++17 -g
SRC_PATH=src
BUILD_PATH=build

log-screen: $(BUILD_PATH)/pathinfo $(SRC_PATH)/main.cpp $(BUILD_PATH)/log-listener.o $(BUILD_PATH)/port-socket.o
	$(PP) -o $(BUILD_PATH)/logscreen $(BUILD_PATH)/port-socket.o $(BUILD_PATH)/log-listener.o $(SRC_PATH)/main.cpp

$(BUILD_PATH)/log-listener.o: $(SRC_PATH)/log-listener.cpp
	$(PP) -c -o $(BUILD_PATH)/log-listener.o $(SRC_PATH)/log-listener.cpp

$(BUILD_PATH)/port-socket.o: $(SRC_PATH)/port-socket.cpp
	$(PP) -c -o $(BUILD_PATH)/port-socket.o $(SRC_PATH)/port-socket.cpp

$(BUILD_PATH)/pathinfo:
	mkdir -p $(BUILD_PATH)
	touch $(BUILD_PATH)/pathinfo

clean:
	rm -rf $(BUILD_PATH)
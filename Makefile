ARDUINO_LIBS = external_arduino
COMMON_CPP_FILES = $(ARDUINO_LIBS)/crypto/*.cpp $(ARDUINO_LIBS)/crypto_lw/*.cpp $(ARDUINO_LIBS)/lwc_finalists/*.c
COMMON_LIBRARIES = -I$(ARDUINO_LIBS)/crypto -I$(ARDUINO_LIBS)/crypto/utility -I$(ARDUINO_LIBS)/crypto_lw -I$(ARDUINO_LIBS)/lwc_finalists -pthread -I/usr/include/piduino/arduino -lpiduino -lcppdb -ldl -ludev

raspi_test_crypto = $(shell find raspi_tests/crypto -type f -iname "*.cpp" | sed "s/.*\///; s/\.cpp//")
raspi_test_crypto_lw = $(shell find raspi_tests/crypto_lw -type f -iname "*.cpp" | sed "s/.*\///; s/\.cpp//")
raspi_test_lwc_finalists = $(shell find raspi_tests/lwc_finalists -type f -iname "*.cpp" | sed "s/.*\///; s/\.cpp//")

$(raspi_test_crypto): %:
	mkdir -p bin/crypto
	g++ -o bin/crypto/$@ raspi_tests/crypto/$@.cpp $(COMMON_CPP_FILES) $(COMMON_LIBRARIES)

$(raspi_test_crypto_lw): %:
	mkdir -p bin/crypto_lw
	g++ -o bin/crypto_lw/$@ raspi_tests/crypto_lw/$@.cpp $(COMMON_CPP_FILES) $(COMMON_LIBRARIES)

$(raspi_test_lwc_finalists): %:
	mkdir -p bin/lwc_finalists
	g++ -o bin/lwc_finalists/$@ raspi_tests/lwc_finalists/$@.cpp $(COMMON_CPP_FILES) $(COMMON_LIBRARIES)

all: $(raspi_test_crypto) $(raspi_test_crypto_lw) $(raspi_test_lwc_finalists)

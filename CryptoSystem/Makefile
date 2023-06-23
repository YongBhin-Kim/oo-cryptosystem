CC			= g++
CPPFLAGS	= -W -O2 -std=c++11
# LDFLAGS	= -lm

SOURCES = main.cpp userinterface.cpp cryptographicmanager.cpp crypto.cpp keymanager.cpp aria.cpp aes.cpp galoisfield.cpp
HEADERS = userinterface.h cryptographicmanager.h crypto.h keymanager.h aria.h aes.h galoisfield.h

cryptosystem: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(CPPFLAGS)

clean:
	@rm -rf *.o cryptosystem
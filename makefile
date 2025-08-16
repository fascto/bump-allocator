
# Especifico que el compilador que voy a usar es gcc

# Las flags que se le van a pasar al compilador

# -Wall para que me tire todos los warnings
# -Wextra para que me tire los warnings extras?
# -Iinclude para que 
# -std=c99 para usar el estandar cd c99

# -O0 desactiva todas las optimizaciones del compilador, 
# lo que esta bien para desarrollo ya que lo va a hacer mas rapido y ademas es mas facil de debuggear mas tarde

# -g compila con la informacion de debug

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99 -O0 -g
SRC_DIR = src
TARGET_DIR = target

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(TARGET_DIR)/%.o,$(SOURCES))
TARGET = $(TARGET_DIR)/target

all: $(TARGET)

$(TARGET_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJECTS) core

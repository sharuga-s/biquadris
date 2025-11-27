CXX = g++
CXXFLAGS = -std=c++20 -Wall -g -MMD
EXEC = biquadris
OBJECTS = main.o \
          block-impl.o block.o \
          cell-impl.o cell.o \
          commandinterpreter-impl.o commandinterpreter.o \
          display.o \
          game-engine-impl.o game-engine.o \
          graphics-display-impl.o graphics-display.o \
          grid-impl.o grid.o \
          text-display-impl.o text-display.o \
          subject.o \
          player-impl.o player.o \
          level.o \
          level0-impl.o level0.o \
          level1-impl.o level1.o \
          level2-impl.o level2.o \
          level3-impl.o level3.o \
          level4-impl.o level4.o \
          level-factory-impl.o level-factory.o \
          iblock-impl.o iblock.o \
          jblock-impl.o jblock.o \
          lblock-impl.o lblock.o \
          oblock-impl.o oblock.o \
          sblock-impl.o sblock.o \
          zblock-impl.o zblock.o \
          tblock-impl.o tblock.o \
          starblock-impl.o starblock.o \
          special-action.o \
          blind-impl.o blind.o \
          heavy-impl.o heavy.o \
          window-impl.o window.o

DEPENDS = ${OBJECTS:.o=.d}

# X11 libraries for graphics
LIBS = -lX11

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${LIBS}

# ============================================================================
# MODULE INTERFACE UNITS - Must be compiled in dependency order
# ============================================================================

# Base modules (no dependencies)
cell.o: cell.cc
	${CXX} ${CXXFLAGS} -c $

level.o: level.cc
	${CXX} ${CXXFLAGS} -c $

special-action.o: special-action.cc
	${CXX} ${CXXFLAGS} -c $

commandinterpreter.o: commandinterpreter.cc
	${CXX} ${CXXFLAGS} -c $

window.o: window.cc
	${CXX} ${CXXFLAGS} -c $

# Modules depending on Cell
block.o: block.cc cell.o
	${CXX} ${CXXFLAGS} -c $

# Modules depending on Cell and Block
grid.o: grid.cc cell.o block.o
	${CXX} ${CXXFLAGS} -c $

# Display modules
display.o: display.cc grid.o block.o
	${CXX} ${CXXFLAGS} -c $

subject.o: subject.cc display.o
	${CXX} ${CXXFLAGS} -c $

text-display.o: text-display.cc display.o grid.o block.o
	${CXX} ${CXXFLAGS} -c $

graphics-display.o: graphics-display.cc display.o grid.o block.o window.o
	${CXX} ${CXXFLAGS} -c $

# Level modules
level0.o: level0.cc level.o
	${CXX} ${CXXFLAGS} -c $

level1.o: level1.cc level.o
	${CXX} ${CXXFLAGS} -c $

level2.o: level2.cc level.o
	${CXX} ${CXXFLAGS} -c $

level3.o: level3.cc level.o
	${CXX} ${CXXFLAGS} -c $

level4.o: level4.cc level.o
	${CXX} ${CXXFLAGS} -c $

level-factory.o: level-factory.cc level.o
	${CXX} ${CXXFLAGS} -c $

# SpecialAction modules
blind.o: blind.cc special-action.o
	${CXX} ${CXXFLAGS} -c $

heavy.o: heavy.cc special-action.o
	${CXX} ${CXXFLAGS} -c $

# Block type modules
iblock.o: iblock.cc block.o
	${CXX} ${CXXFLAGS} -c $

jblock.o: jblock.cc block.o
	${CXX} ${CXXFLAGS} -c $

lblock.o: lblock.cc block.o
	${CXX} ${CXXFLAGS} -c $

oblock.o: oblock.cc block.o
	${CXX} ${CXXFLAGS} -c $

sblock.o: sblock.cc block.o
	${CXX} ${CXXFLAGS} -c $

zblock.o: zblock.cc block.o
	${CXX} ${CXXFLAGS} -c $

tblock.o: tblock.cc block.o
	${CXX} ${CXXFLAGS} -c $

starblock.o: starblock.cc block.o
	${CXX} ${CXXFLAGS} -c $

# Player module
player.o: player.cc level.o grid.o special-action.o block.o
	${CXX} ${CXXFLAGS} -c $

# GameEngine module
game-engine.o: game-engine.cc player.o subject.o commandinterpreter.o
	${CXX} ${CXXFLAGS} -c $

# ============================================================================
# IMPLEMENTATION FILES - Compiled after their interface units
# ============================================================================

%-impl.o: %-impl.cc %.o
	${CXX} ${CXXFLAGS} -c $

# Special cases that need extra dependencies
level-factory-impl.o: level-factory-impl.cc level-factory.o level0.o level1.o level2.o level3.o level4.o
	${CXX} ${CXXFLAGS} -c $

player-impl.o: player-impl.cc player.o iblock.o jblock.o lblock.o oblock.o sblock.o zblock.o tblock.o starblock.o level-factory.o
	${CXX} ${CXXFLAGS} -c $

game-engine-impl.o: game-engine-impl.cc game-engine.o player.o subject.o commandinterpreter.o blind.o heavy.o level.o
	${CXX} ${CXXFLAGS} -c $

text-display-impl.o: text-display-impl.cc text-display.o grid.o block.o cell.o game-engine.o player.o
	${CXX} ${CXXFLAGS} -c $

graphics-display-impl.o: graphics-display-impl.cc graphics-display.o display.o grid.o block.o cell.o game-engine.o player.o window.o
	${CXX} ${CXXFLAGS} -c $

# ============================================================================
# MAIN
# ============================================================================

main.o: main.cc game-engine.o player.o text-display.o graphics-display.o
	${CXX} ${CXXFLAGS} -c $

# ============================================================================
# UTILITY TARGETS
# ============================================================================

-include ${DEPENDS}

.PHONY: clean

clean:
	rm -f ${OBJECTS} ${EXEC} ${DEPENDS}
	rm -rf gcm.cache
	
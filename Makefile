CXX = g++-14 -std=c++20 -fmodules-ts
CXXFLAGS = -Wall -g
LDFLAGS = -lX11
EXEC = biquadris

.PHONY: all clean cleanall headers

all: headers $(EXEC)

# Precompile standard library headers
headers:
	@echo "Precompiling standard library headers..."
	@mkdir -p gcm.cache
	$(CXX) -x c++-system-header iostream
	$(CXX) -x c++-system-header vector
	$(CXX) -x c++-system-header memory
	$(CXX) -x c++-system-header utility
	$(CXX) -x c++-system-header algorithm
	$(CXX) -x c++-system-header fstream
	$(CXX) -x c++-system-header sstream
	$(CXX) -x c++-system-header cstdlib
	$(CXX) -x c++-system-header ctime
	$(CXX) -x c++-system-header cctype
	$(CXX) -x c++-system-header string

# Build in correct dependency order
$(EXEC):
	@echo "Building base modules (no dependencies)..."
	$(CXX) $(CXXFLAGS) -c cell.cc
	$(CXX) $(CXXFLAGS) -c block.cc
	$(CXX) $(CXXFLAGS) -c level.cc
	
	@echo "Building grid (depends on Cell, Block)..."
	$(CXX) $(CXXFLAGS) -c grid.cc
	
	@echo "Building display (depends on Grid, Block)..."
	$(CXX) $(CXXFLAGS) -c display.cc
	
	@echo "Building subject (depends on Display)..."
	$(CXX) $(CXXFLAGS) -c subject.cc
	
	@echo "Building block types (depend on Block)..."
	$(CXX) $(CXXFLAGS) -c iblock.cc jblock.cc lblock.cc oblock.cc sblock.cc tblock.cc zblock.cc starblock.cc
	
	@echo "Building level types (depend on Level)..."
	$(CXX) $(CXXFLAGS) -c level0.cc level1.cc level2.cc level3.cc level4.cc
	
	@echo "Building level factory (depends on Level types)..."
	$(CXX) $(CXXFLAGS) -c level-factory.cc
	
	@echo "Building player (depends on Level, Grid, Block types)..."
	$(CXX) $(CXXFLAGS) -c player.cc
	
	@echo "Building special-action (depends on Player, Grid)..."
	$(CXX) $(CXXFLAGS) -c special-action.cc
	
	@echo "Building special actions (depend on SpecialAction, Player, Grid)..."
	$(CXX) $(CXXFLAGS) -c blind.cc heavy.cc
	
	@echo "Building command interpreter..."
	$(CXX) $(CXXFLAGS) -c commandinterpreter.cc
	
	@echo "Building game engine (depends on Player, Subject, CommandInterpreter)..."
	$(CXX) $(CXXFLAGS) -c game-engine.cc
	
	@echo "Building window..."
	$(CXX) $(CXXFLAGS) -c window.cc
	
	@echo "Building displays (depend on Display, GameEngine, Player, Grid, Block)..."
	$(CXX) $(CXXFLAGS) -c text-display.cc graphics-display.cc
	
	@echo "Building implementations..."
	$(CXX) $(CXXFLAGS) -c cell-impl.cc block-impl.cc grid-impl.cc
	$(CXX) $(CXXFLAGS) -c iblock-impl.cc jblock-impl.cc lblock-impl.cc oblock-impl.cc
	$(CXX) $(CXXFLAGS) -c sblock-impl.cc tblock-impl.cc zblock-impl.cc starblock-impl.cc
	$(CXX) $(CXXFLAGS) -c level0-impl.cc level1-impl.cc level2-impl.cc level3-impl.cc level4-impl.cc
	$(CXX) $(CXXFLAGS) -c level-factory-impl.cc
	$(CXX) $(CXXFLAGS) -c player-impl.cc
	$(CXX) $(CXXFLAGS) -c blind-impl.cc heavy-impl.cc
	$(CXX) $(CXXFLAGS) -c commandinterpreter-impl.cc
	$(CXX) $(CXXFLAGS) -c game-engine-impl.cc
	$(CXX) $(CXXFLAGS) -c window-impl.cc
	$(CXX) $(CXXFLAGS) -c text-display-impl.cc graphics-display-impl.cc
	
	@echo "Building main..."
	$(CXX) $(CXXFLAGS) -c main.cc
	
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) *.o $(LDFLAGS) -o $(EXEC)
	@echo "Build complete!"

clean:
	-rm -f $(EXEC) *.o

cleanall: clean
	-rm -rf gcm.cache
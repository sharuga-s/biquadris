CXX = g++-14 -std=c++20 -fmodules-ts
CXXFLAGS = -Wall -g
LDFLAGS = -lX11
EXEC = biquadris

.PHONY: all clean cleanall headers

all: headers $(EXEC)

headers:
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

$(EXEC):
	$(CXX) $(CXXFLAGS) -c cell.cc
	$(CXX) $(CXXFLAGS) -c block.cc
	$(CXX) $(CXXFLAGS) -c level.cc
	
	$(CXX) $(CXXFLAGS) -c grid.cc
	
	$(CXX) $(CXXFLAGS) -c display.cc
	
	$(CXX) $(CXXFLAGS) -c subject.cc
	
	$(CXX) $(CXXFLAGS) -c iblock.cc jblock.cc lblock.cc oblock.cc sblock.cc tblock.cc zblock.cc starblock.cc
	
	$(CXX) $(CXXFLAGS) -c level0.cc level1.cc level2.cc level3.cc level4.cc
	
	$(CXX) $(CXXFLAGS) -c level-factory.cc
	
	$(CXX) $(CXXFLAGS) -c player.cc
		
	$(CXX) $(CXXFLAGS) -c blind.cc heavy.cc
	
	$(CXX) $(CXXFLAGS) -c commandinterpreter.cc
	
	$(CXX) $(CXXFLAGS) -c game-engine.cc
	
	$(CXX) $(CXXFLAGS) -c window.cc
	
	$(CXX) $(CXXFLAGS) -c text-display.cc graphics-display.cc
	
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
	
	$(CXX) $(CXXFLAGS) -c main.cc
	
	$(CXX) $(CXXFLAGS) *.o $(LDFLAGS) -o $(EXEC)

clean:
	-rm -f $(EXEC) *.o

cleanall: clean
	-rm -rf gcm.cache
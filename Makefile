all: bin/bb.out bin/bb_flow.out bin/dp.out

bin/%.out: src/%.cpp
	mkdir -p bin
	g++ -std=c++14 -O2 $< -o $@

clean:
	rm -rf bin

all: bin/bb.out bin/bb_flow.out bin/dp.out

bin:
	mkdir bin

bin/%.out: src/%.cpp bin
	g++ -std=c++14 -O2 $< -o $@

clean:
	rm -rf bin

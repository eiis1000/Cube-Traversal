#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <bitset>
#include <array>

using std::cout, std::endl, std::bitset, std::array, std::string;


const int DIM = 3;
const int VERTICES = 1 << DIM;
const int EDGES = DIM * (1 << (DIM - 1));

// iteration order of cube edges is first by axis (left to right with x=[0]), then by lexicographic of remaining axes
const int NUM_ENCODING = EDGES - VERTICES + 1;
const bitset<EDGES> encoding_edges(string("1110000010100"));

struct path {
	path(bitset<DIM> pos, int prev_dir, array<short, EDGES> full_count) 
		: pos(pos), prev_dir(prev_dir), full_count(full_count) { };
	path(path p, int next_dir) : pos(p.pos), prev_dir(next_dir), full_count(p.full_count) {
		if (pos[next_dir]) {
			full_count[next_dir]--;
		} else {
			full_count[next_dir]++;
		}
		pos.flip(next_dir);	
	}
	~path() { };
	bitset<DIM> pos; // coords, x = pos[0]
	// vector<bitset<DIM>> ppos;
	int prev_dir; // the direction, so y=1
	array<short, EDGES> full_count;
	array<short, NUM_ENCODING> partial_count() const {
		array<short, NUM_ENCODING> pc{};
		int c = 0;
		for (int i = 0; i < EDGES; ++i)
			if (encoding_edges[i])
				pc[c++] = full_count[i];
		return pc;
	}
}

int main() {
	return 0;
}
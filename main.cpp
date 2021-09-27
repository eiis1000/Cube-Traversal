#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <bitset>
#include <array>
#include <map>

using std::cout, std::endl, std::bitset, std::vector, std::array, std::string, std::map;


const int DIM = 3;
const int VERTICES = 1 << DIM;
const int EDGES = DIM * (1 << (DIM - 1));

// iteration order of cube edges is first by axis (left to right with x=[0]), then by lexicographic of remaining axes
const bitset<EDGES> encoding_edges(string("1110000010100"));
const size_t NUM_ENCODING = VERTICES - EDGES + 1;

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

	std::optional<array<short, NUM_ENCODING>> partial_count_cached;
	array<short, NUM_ENCODING> partial_count() {
		if (partial_count_cached.has_value())
			return partial_count_cached.value();
		array<short, NUM_ENCODING> pc{};
		int c = 0;
		for (int i = 0; i < EDGES; ++i)
			if (encoding_edges[i])
				pc[c++] = full_count[i];
		partial_count_cached = {pc};
		return pc;
	}

	bool operator==(const path& other) const {
		return pos == other.pos && full_count == other.full_count;
	}
};

int main() {
	if (NUM_ENCODING != encoding_edges.count()) {
		throw std::runtime_error("NUM_ENCODING is not correct.");
		return -1;
	}
	map<array<short, EDGES>, array<short, NUM_ENCODING>> encodings_map;
	vector<path> cur_paths, prev_paths;
	return 0;
}
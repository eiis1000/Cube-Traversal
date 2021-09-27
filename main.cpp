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
const int EDGES = DIM << (DIM - 1);

// iteration order of cube edges is first by axis (left to right with x=[0]), then by lexicographic of remaining axes
const bitset<EDGES> encoding_edges(string("1110000010100"));
const size_t NUM_ENCODING = EDGES - VERTICES + 1;

struct path {
	path(bitset<DIM> pos, int prev_dir, array<short, EDGES> full_count) 
		: pos(pos), prev_dir(prev_dir), full_count(full_count), ppath(std::to_string(pos.to_ulong())) { };
	path(const path& p, int next_dir) 
		: pos(p.pos), prev_dir(next_dir), full_count(p.full_count) {
		unsigned long long edge = 0;
		for (int i = 0; i < DIM; ++i) {
			if (i != next_dir) {
				edge <<= 1;
				edge += pos[i];
			}
			edge += (next_dir << (DIM - 1));
		}

		if (pos[next_dir]) {
			full_count[edge]--;
		} else {
			full_count[edge]++;
		}
		// cout << next_dir << "  ;  " << full_count[next_dir] << endl;
		pos.flip(next_dir);	

		ppath = p.ppath + std::to_string(pos.to_ulong());
	}
	~path() { };

	bitset<DIM> pos; // coords, x = pos[0]
	// vector<bitset<DIM>> ppos;
	int prev_dir; // the direction, so y=1
	array<short, EDGES> full_count;
	string ppath;

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

	array<map<array<short, NUM_ENCODING>, array<short, EDGES>>, VERTICES> encodings_maps;
	vector<path> cur_paths, prev_paths;
	path zero_path(bitset<DIM>(0), -1, {});
	for (int i = 0; i < DIM; ++i)
		cur_paths.emplace_back(zero_path, i);
	for (path& p : cur_paths)
		encodings_maps[p.pos.to_ulong()].emplace(p.partial_count(), p.full_count);

	const int MAX_DEPTH = 50;
	for (int depth = 1; depth < MAX_DEPTH; ++depth) {
		cout << "Doing depth " << depth << endl;
		swap(cur_paths, prev_paths);
		cur_paths.clear();
		for (const path& p : prev_paths) {
			// cout << p.pos << endl;
			for (int d = 0; d < DIM; ++d) {
				if (d != p.prev_dir) {
					path n(p, d);
					auto search = encodings_maps[n.pos.to_ulong()].find(n.partial_count());
					if (search != encodings_maps[n.pos.to_ulong()].end()) {
						if (search->second != n.full_count) {
							throw std::runtime_error("Two pathways have different partial counts: " + n.ppath);
						} else {
							continue;
						}
					}
					cur_paths.push_back(n);
					encodings_maps[n.pos.to_ulong()].emplace(n.partial_count(), n.full_count);
				}
			}
		}
	}
	cout << "Done!" << endl;
}
#include <iostream>
#include <optional>
#include <vector>
#include <cmath>
#include <iterator>
using namespace std;
typedef optional<int> oint;
std::ostream& operator << (std::ostream& stream, oint& input) {
	if (!input) {
		stream << "#";
		return stream;
	}
	stream << input.value();
	return stream;
}
#include "TuringMachine.hpp"
#define NTAPES 2
typedef array<oint, NTAPES> ointa;

void processAndPrint(TuringMachine<oint, NTAPES> m, Tapes<oint, NTAPES> &input) {
	cout << "Start!" << endl;
	if (m.process(input)) {
		cout << "Success!" << endl;
	}
	else {
		cout << "Rejection!" << endl;
	}
	//cout << input;
}

int main()
{
	State start_state{};
	State state1{};
	State state2{};
	State state3{};
	State accept_state{};
	TransitionFunction<oint, NTAPES> f = {{
										   {{start_state, [](ointa i){return i[1] != nullopt;}},
											{start_state, {0, 1}, nullopt}}, //move right on output tape until null
										   {{start_state, [](ointa i){return !i[1];}},
											{state1, {0, 0}, nullopt}},
										   {{state1, [](ointa){return true;}},
											{state2, {0, 0}, [](ointa i){return ointa{i[0], i[0]};}}}, //copy from input to output
										   {{state2, [](ointa i){return i[0] != nullopt;}},
											{state2, {1, -1}, [](ointa i){return ointa{i[0], i[0]};}}}, //copy from input to output
										   {{state2, [](ointa i){return !i[0];}},
											{accept_state, {0, 0}, nullopt}} //copy from input to output
		}};
	TuringMachine m(f, start_state, accept_state);
	for (int n = 7;n < 8;n++) {
		int elements = pow(10, n);
		cout << pow(10, n) << endl;
		vector<oint> tape1(elements, 0);
		vector<oint> tape2(elements, 0);
		tape1.push_back(nullopt);
		tape2.push_back(nullopt);
		Tapes<oint, NTAPES> input = array<vector<oint>, NTAPES>{tape1, tape2};
		processAndPrint(m, input);
	}
};

#include <iostream>
#include <optional>
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

void processAndPrint(TuringMachine<oint, NTAPES> m, Tapes<oint, NTAPES> &input) {
	if (m.process(input)) {
		cout << "Success!" << endl;
	}
	else {
		cout << "Rejection!" << endl;
	}
	cout << input;
}

int main()
{
	State start_state{};
	State state1{};
	State accept_state{};
	TransitionFunction<oint, NTAPES> f = {{
										   {{start_state, [](std::array<oint, NTAPES> i){return true;}},
											{state1, [](std::array<oint, NTAPES> i){return std::array<oint, NTAPES>{1, 2};}, {0, 0}}},
										   {{state1, [](std::array<oint, NTAPES> i){return true;}},
											{accept_state, nullopt, {0, 0}}}
										   }};
	TuringMachine m(f, start_state, accept_state);
	Tapes<oint, NTAPES> input = (array<vector<oint>, NTAPES>) {vector<oint>{0},vector<oint>{0}};
	processAndPrint(m, input);
};

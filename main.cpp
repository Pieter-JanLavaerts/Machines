#include "TuringMachine.hpp"
#include <iostream>

using namespace std;
typedef optional<int> oint;

void processAndPrint(TuringMachine<oint> m, std::vector<oint> &input) {
	if (m.process(input)) {
		cout << "Success!" << endl;
	}
	else {
		cout << "Rejection!" << endl;
	}
	for (oint i : input) {
		if (i) {
			std::cout << i.value() << " ";
		}
	}
	std::cout << std::endl;
}

int main()
{
	State start_state;
	State state1;
	State accept_state;
	State reject_state;
	Transition<oint> t = {{
						   {{start_state, [](oint r, oint i){return true;}},
							{state1, [](oint r, oint i){return numeric_limits<int>::min();}, nullopt, None}},

						   {{state1, [](oint r, oint i){return !i;}},
							{accept_state, nullopt, nullopt, None}},

						   {{state1, [](oint r, oint i){return r && i && r <= i;}},
							{state1, [](oint r, oint i){return i;}, nullopt, Right}}
		}};
	TuringMachine<oint> m(t, start_state, accept_state, reject_state);
	std::vector<oint> good_input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, std::nullopt};
	std::vector<oint> bad_input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 12, 13, 14, 15, 16, std::nullopt};
	//                                                            ^
	processAndPrint(m, good_input);
	processAndPrint(m, bad_input);
};

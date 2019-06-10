#pragma once

#include <vector>
#include <map>
#include <optional>
#include <functional>

#include <iostream>

enum Direction {Left, None, Right};

class State{
	static int statecount;
	int state;
public:
	State() { state = ++statecount; }
	bool operator<(const State& o) const {
		return state < o.state;
	}
	friend bool operator==(const State& lhs, const State& rhs){
		return lhs.state == rhs.state;
	}
	friend bool operator!=(const State& lhs, const State& rhs){ return !(lhs == rhs); }
};
int State::statecount = 0;

template <typename T>
struct OriginFunction {
	State state;
	bool(*f)(T, T);
	bool operator< (const OriginFunction& o) const {
		return
			std::tie (state, f)
			< std::tie (o.state, o.f);
	}
};
template <typename T>
struct OriginArgument {
	State state;
	T r;
	T input;
};
template <typename T>
struct Destination {
	State state;
	std::optional<std::function<T(T, T)>> r;
	std::optional<std::function<T(T, T)>> output;
	Direction direction;
};
template <typename T>
class Transition {
	std::vector<std::pair<OriginFunction<T>, Destination<T>>> m;
public:
	Transition(std::vector<std::pair<OriginFunction<T>, Destination<T>>> m_) : m{m_}
	{}
	const std::optional<Destination<T>> at(const OriginArgument<T>& a){
		for (auto pair : m) {
			if (pair.first.state == a.state && pair.first.f(a.r, a.input)) {
				return pair.second;
			}
		}
		return std::nullopt;
	}
};
template <typename T>
class TuringMachine
{
	Transition<T> transition_function;
	T r;
	State start_state;
	State accept_state;
	State reject_state;
public:
	TuringMachine(Transition<T> transition_function_,
				  State start_state_,
				  State accept_state_,
				  State reject_state_) :
		transition_function{transition_function_},
		start_state{start_state_},
		accept_state{accept_state_},
		reject_state{reject_state_}
	{};
	bool process(std::vector<T>& input) {
		State current_state = start_state;
		int position = 0;
		while (current_state != accept_state && current_state != reject_state && position < input.size() && position >= 0) {
			std::optional<Destination<T>> maybe_d = transition_function.at(OriginArgument<T>{current_state, r, input[position]});
			if (maybe_d) {
				Destination<T> d = maybe_d.value();
				current_state = d.state;
				//write to register
				if (d.r) {
					r = d.r.value()(r, input[position]);
				}
				//write to tape
				if (d.output) {
					input[position] = d.output.value()(r, input[position]);
				}
				//move head
				if (d.direction == Left) {
					position--;
				}
				else if (d.direction == Right) {
					position++;
				}
			}
			else {
				current_state = reject_state;
			}
		}
		if (current_state == accept_state) {
			return true;
		}
		return false;

	}
};

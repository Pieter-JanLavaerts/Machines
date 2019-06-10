#pragma once

#include <vector>
#include <map>
#include <optional>
#include <functional>
#include <iostream>

class State{
	static unsigned int statecount;
	unsigned int state;
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
unsigned int State::statecount = 0;

template <typename T, unsigned int N>
struct OriginFunction {
	State state;
	bool(*f)(std::array<T, N>);
	bool operator< (const OriginFunction& o) const {
		return
			std::tie (state, f)
			< std::tie (o.state, o.f);
	}
};
template <typename T, unsigned int N>
struct OriginArgument {
	State state;
	std::array<T, N> input;
};
template <typename T, unsigned int N>
struct Destination {
	State state;
	std::array<int, N> directions;
	std::optional<std::function<std::array<T,N>(std::array<T, N>)>> output;
};
template <typename T, unsigned int N>
class TransitionFunction {
	std::vector<std::pair<OriginFunction<T, N>, Destination<T, N>>> m;
public:
	TransitionFunction(std::vector<std::pair<OriginFunction<T, N>, Destination<T, N>>> m_) : m{m_}
	{}
	std::optional<Destination<T, N>> at(const OriginArgument<T, N>& a) const {
		for (auto pair : m) {
			if (pair.first.state == a.state && pair.first.f(a.input)) {
				return pair.second;
			}
		}
		return std::nullopt;
	}
};
template <typename T, unsigned int N>
class Tapes
{
	std::array<std::vector<T>, N> tapes;
	std::array<int, N> positions;
public:
	Tapes(std::array<std::vector<T>, N> tapes_) : tapes{tapes_}, positions{}
	{};
	std::array<T, N> read() const {
		std::array<T, N> result;
		for (int i = 0; i < N; i++) {
			result[i] = tapes[i][positions[i]];
		}
		return result;
	}
	bool move(std::array<int, N> moves) {
		for (int i = 0; i < N; i++) {
			positions[i]+=moves[i];
			if (positions[i] < 0 || positions[i] >= tapes[i].size()) {
				return false;
			}
		}
		return true;
	}
	void write(std::array<T, N> inputs) {
		for (int i = 0; i < N; i++) {
			tapes[i][positions[i]] = inputs[i];
		}
	}
	friend std::ostream& operator << (std::ostream& stream, const Tapes& input) {
		for (std::vector tape : input.tapes) {
			for (T e : tape) {
				stream << e << " ";
			}
			stream << std::endl;
		}
		return stream;
	}
};
template <typename T, unsigned int N>
class TuringMachine
{
	TransitionFunction<T, N> transition_function;
	T r;
	State start_state;
	State accept_state;
public:
	TuringMachine(TransitionFunction<T, N> transition_function_,
				  State start_state_,
				  State accept_state_) :
		transition_function{transition_function_},
		start_state{start_state_},
		accept_state{accept_state_}
	{};
	bool process(Tapes<T, N> &input) {
		std::optional<Destination<T, N>> maybe_d = transition_function.at(OriginArgument<T, N>{start_state, input.read()});
		while (maybe_d && maybe_d.value().state != accept_state && input.move(maybe_d.value().directions)) {
			Destination<T, N> d = maybe_d.value();
			//write to tapes
			if (d.output) {
				std::array<T, N> outputs = d.output.value()(input.read());
				input.write(outputs);
			}
			maybe_d = transition_function.at(OriginArgument<T, N>{d.state, input.read()});
		}
		if (maybe_d && maybe_d.value().state == accept_state) {
			return true;
		}
		return false;
	}
};

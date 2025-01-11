# Aura
This is a simple interpreter and framework for finite automatas. It includes an interpreter which can construct and define different kinds of machines (currently only one kind!)
and run tests on them.

## Documentation
#### Supported machine types:
  - DFA (Deterministic Finite Automata)
#### Supported datatypes:
  - Strings
  - States
  - Machines
#### Syntax:
Example (test1.au):
```
# Simple test to define a DFA which takes inputs consisting of {'a', 'b'}
# but accepts the input only if the string ends with starts with at least
# one 'a' and ends with 'b'. eg. 'ab', 'aaaaabbb'

# DFA(set of states, set of inputs, set of final states, initial state) 

machine1 = DFA({q0, q1, q2, q3}, {'a', 'b'}, {q2}, q0) # Define a machine

machine1 := {
  q0: {'a'} -> q1, {'b'} -> q3
  q1: {'a'} -> q1, {'b'} -> q2
  q2: {'a'} -> q0, {'b'} -> q2
  q3: loop
}


run(machine1, 'ab') # should be accepeted.
run(machine1, 'baba') # should not be accepted.
run(machine1, 'aabba') # should not be accepted.
run(machine1, 'aabb') # should be accepted.
```

- `machine = DFA(...)` - declare a DFA machine. The DFA constructor takes in the set of states, set of inputs, set of final states and one initial state.
- `machine := {...}` - define transitions for each state within the machine.
- `run(machine, ...)` - run tests on the machine with the input defined for the machine.
- `state: {...} -> state` - define transitions for a given state within the machine.
- `# ...` - for comments.
- `state: loop` - define a self loop for a given state across all inputs using the `loop` keyword.

## Dependencies:
- C Compiler
- Cmake
- Cairo (for plotting)

## Build and run:
```
git clone https://github.com/wwsmiff/aura/
cd aura
cmake -G "Ninja" -S . -B build
cmake --build build
./build/aura run <file>
```
Additionally enabling `-DAURA_BUILD_TESTS=ON` when invoking cmake will also build the tests for the framework itself.

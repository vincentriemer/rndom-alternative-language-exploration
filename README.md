# React Native DOM Alternative Language Exploration

This is a small project for evaluating differently languages that can either be run on the web or simple compile to JS for adoption in react-native-dom's core. The module being rewritten for this evalutation is `LayoutAnimation`.

## Implementation Status

### KeyframeGenerator

- JS - Complete
- Reason - Partial (missing spring generator)
- Rust/WASM - Not Started

### LayoutAnimationManager

- JS - Not Started
- Reason - Not Started
- Rust/Wasm - Not Started

## Usage

```sh
# install dependencies
yarn

# test implementation correctness
yarn test

# run benchmarks
yarn bench
```

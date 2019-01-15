# Solver for TSP

## Features

- Solve TSP including those that do not satisfy triangle inequality
- Solve TSP faster than ![n^2 * 2^n](https://latex.codecogs.com/gif.latex?O(n^22^n)) DP

## Usage

1. build

```bash
make
```

2. run

```bash
bin/bb.out tests/n_20/tsp_n_20_p_1.0_1.tsp
```

## Performance

The performance (average running time) benchmarked by `tests/` files is as follows.

| Testcases | `src/bb.cpp` | `src/bb_flow.cpp` | `src/dp.cpp` |
|:-----------|------------:|------------:|------------:|
| `tests/n_16/*` | 0.01532 sec | 0.13547 sec | 0.03326 sec |
| `tests/n_18/*` | 0.03744 sec | 0.65817 sec | 0.11740 sec |
| `tests/n_20/*` | 0.15674 sec | 3.97176 sec | 0.48440 sec |
| `tests/n_22/*` | 0.36346 sec | 10.63145 sec | 2.19858 sec |
| `tests/n_24/*` | 0.96166 sec | 31.60966 sec | 9.79800 sec |
| `tests/n_26/*` | 1.20250 sec | 41.17456 sec | 53.70651 sec |
| `tests/n_28/*` | 5.00968 sec | 194.83778 sec | 445.56655 sec |
| `tests/n_30/*` | 44.71780 sec | 830.41925 sec | - |

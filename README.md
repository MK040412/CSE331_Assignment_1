# Sorting Algorithms Comparison Project

This project implements and compares various sorting algorithms, including both conventional and contemporary approaches. It measures performance metrics such as execution time, number of comparisons, assignments, memory usage, and stability across different data distributions.

## Directory Structure

```

├── main.cpp                # Main program entry point
├── data_generator.cpp      # Data generation utilities
├── data_generator.h        # Header for data generation
├── conventional/           # Conventional sorting algorithms
│   ├── bubble_sort.h       # Bubble Sort implementation
│   ├── insertion_sort.h    # Insertion Sort implementation
│   ├── selection_sort.h    # Selection Sort implementation
│   ├── quick_sort.h        # Quick Sort implementation
│   ├── merge_sort.h        # Merge Sort implementation
│   └── heap_sort.h         # Heap Sort implementation
├── contemporary/           # Contemporary sorting algorithms
│   ├── library_sort.h      # Library Sort implementation
│   ├── tim_sort.h          # Tim Sort implementation
│   ├── cocktail_sort.h     # Cocktail Sort implementation
│   ├── comb_sort.h         # Comb Sort implementation
│   ├── tournament_sort.h   # Tournament Sort implementation
│   └── intro_sort.h        # Intro Sort implementation
├── build/                  # Build directory (created during compilation)
├── bin/                    # Binary output directory
└── data/                   # Test data directory (created during execution)
```

## Implemented Sorting Algorithms

### Conventional Algorithms

1. **Bubble Sort**
   - Time Complexity: Best O(n), Average O(n²), Worst O(n²)
   - Stable: Yes

2. **Insertion Sort**
   - Time Complexity: Best O(n), Average O(n²), Worst O(n²)
   - Stable: Yes

3. **Selection Sort**
   - Time Complexity: Best O(n²), Average O(n²), Worst O(n²)
   - Stable: No

4. **Quick Sort**
   - Time Complexity: Best O(n log n), Average O(n log n), Worst O(n²)
   - Stable: No

5. **Merge Sort**
   - Time Complexity: Best O(n log n), Average O(n log n), Worst O(n log n)
   - Stable: Yes

6. **Heap Sort**
   - Time Complexity: Best O(n log n), Average O(n log n), Worst O(n log n)
   - Stable: No

### Contemporary Algorithms

1. **Library Sort**
   - Time Complexity: Best O(n), Average O(n log n), Worst O(n²)
   - Stable: Yes

2. **Tim Sort**
   - Time Complexity: Best O(n), Average O(n log n), Worst O(n log n)
   - Stable: Yes

3. **Cocktail Sort**
   - Time Complexity: Best O(n), Average O(n²), Worst O(n²)
   - Stable: Yes

4. **Comb Sort**
   - Time Complexity: Best O(n log n), Average O(n²), Worst O(n²)
   - Stable: No

5. **Tournament Sort**
   - Time Complexity: Best O(n log n), Average O(n log n), Worst O(n log n)
   - Stable: No

6. **Intro Sort**
   - Time Complexity: Best O(n log n), Average O(n log n), Worst O(n log n)
   - Stable: No

## Building the Project

The project uses a Makefile for compilation. To build the project:

```bash
make
```

This will create the executable in the `bin/` directory.

## Running the Program

After building, you can run the program with:

```bash
make run
```

or directly:

```bash
./bin/sorting_algorithms
```

## Test Data

The program generates and uses several types of test data:
- Sorted (ascending)
- Sorted (descending)
- Random
- Partially sorted

Each data type is generated in various sizes (1K, 10K, 100K, 1M elements).

## Program Options

When running the program, you can select from the following test options:

1. Small data size (1K)
2. Medium data sizes (1K, 10K)
3. Large data sizes (1K, 10K, 100K)
4. All data sizes (1K, 10K, 100K, 1M) - Warning: This may take a very long time
5. Stability tests only (1K)

## Performance Metrics

The program measures and reports the following metrics for each algorithm:
- Execution time (average over multiple runs)
- Number of comparisons
- Number of assignments
- Memory usage
- Theoretical stability
- Measured stability
- Sorting correctness

## Output

Results are displayed in tabular format in the console and also exported to a CSV file named `sorting_summary_<size>.csv` for further analysis.


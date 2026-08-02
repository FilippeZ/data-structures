# 📊 Data Structures & Algorithms — Ocean Temperature Dataset

> A C-language academic project implementing **sorting algorithms**, **search algorithms**, **AVL trees**, and **hash tables** on real oceanographic measurement data, coupled with a **Python Machine Learning Time Series Pipeline** for ocean temperature forecasting.

---

## 🤖 Connection to Artificial Intelligence & Machine Learning

The data structures and algorithms in this project are not merely theoretical concepts — they are the **foundational architectural components of modern AI systems**.

```
                         ┌─────────────────────────────────────────┐
                         │   DATA STRUCTURES ↔ AI ARCHITECTURES    │
                         └─────────────────────────────────────────┘
                                              │
    ┌───────────────────────────┬─────────────┴──────────────┬───────────────────────────┐
    ▼                           ▼                            ▼                           ▼
[ AVL / BST Trees ]     [ Heap / Priority Queue ]   [ Hash Tables & Chaining ]  [ Search Algorithms ]
  └─► Random Forest &     └─► Beam Search in          └─► Embedding Lookups &     └─► Hyperparameter Tuning
      XGBoost Splits          LLMs (GPT/Claude)           LLM KV Caches (Transformer) & RAG Vector Search
```

### 1. Decision Trees, Random Forest & XGBoost ↔ AVL / Binary Search Trees (BST)
- **Architectural Homogeneity:** In Machine Learning, tree-based models like **Random Forest** and **XGBoost** split data at each node using thresholds (e.g., `temperature < 20.0°C`). This decision split logic is identical to the Binary Search Tree ordering implemented in [`part2/avl-tree/avl_crud.c`](part2/avl-tree/avl_crud.c).
- **Tree Balancing:** While AVL trees enforce `|balance| ≤ 1` via rotations to maintain $O(\log n)$ height, decision trees use depth limits and pruning (`max_depth`) to prevent over-fitting.

### 2. Time-Series Forecasting ↔ Sorting & Searching
- **Multivariate Time Series:** The ocean dataset contains timestamped measurements (`date`, `temperature`, `phosphate`). 
- **Lag Features & Sorting:** Sequential models require chronologically ordered input. Quick Sort ($O(n \log n)$) and Binary Search ($O(\log n)$) allow rapid time-window slicing and lag generation ($t-1, t-2, t-3$).

---

## 🐍 Python Machine Learning Pipeline (`ml_pipeline/ocean_ml_pipeline.py`)

To complement the C data structure core, a complete Python ML Pipeline is implemented in [`ml_pipeline/ocean_ml_pipeline.py`](ml_pipeline/ocean_ml_pipeline.py). It models sea surface temperature as a multivariate time series using **Lag Features** and Ensemble Tree Models (**Random Forest** and **Gradient Boosting / XGBoost** logic).

### Pipeline Workflow:

```
┌────────────────────────┐      ┌─────────────────────────┐      ┌──────────────────────────┐
│  1. Preprocessing      │ ───► │  2. Feature Engineering │ ───► │  3. Time-Series Split    │
│  Parse dates, sort     │      │  Create Lag-1,2,3 for   │      │  80% Train (Past)        │
│  chronologically       │      │  temp & phosphate       │      │  20% Test (Future)       │
└────────────────────────┘      └─────────────────────────┘      └──────────────────────────┘
                                                                              │
                                                                              ▼
┌────────────────────────┐      ┌─────────────────────────┐      ┌──────────────────────────┐
│  5. Evaluation         │ ◄─── │  4. Tree Modeling       │ ◄──────┘  (Sequential split      │
│  MSE, RMSE, MAE, R²    │      │  Random Forest &        │           prevents data leakage) │
│  Feature Importances   │      │  Gradient Boosting      │                                  │
└────────────────────────┘      └─────────────────────────┘                                  │
```

### 💻 Python Code Implementation:

```python
import os, sys
import numpy as np
import pandas as pd
from sklearn.ensemble import RandomForestRegressor, GradientBoostingRegressor
from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score

# 1. Loading & Preprocessing
df = pd.read_csv("data/ocean.csv")
df = df.rename(columns={'Date': 'date', 'T_degC': 'temperature', 'PO4uM': 'phosphate'})
df["date"] = pd.to_datetime(df["date"], format="%m/%d/%Y", errors='coerce')
df = df.sort_values("date").reset_index(drop=True).set_index("date")

# 2. Feature Engineering (Lag Features)
def create_lags(data, lags=[1, 2, 3]):
    df_lagged = data.copy()
    for l in lags:
        df_lagged[f"temperature_lag_{l}"] = df_lagged["temperature"].shift(l)
        df_lagged[f"phosphate_lag_{l}"] = df_lagged["phosphate"].shift(l)
    return df_lagged

df_ml = create_lags(df[["temperature", "phosphate"]], lags=[1, 2, 3]).dropna()
X = df_ml.drop(columns=["temperature"])
y = df_ml["temperature"]

# 3. Train/Test Split (Sequential for Time-Series - NO Random Split)
split_index = int(len(X) * 0.8)
X_train, X_test = X.iloc[:split_index], X.iloc[split_index:]
y_train, y_test = y.iloc[:split_index], y.iloc[split_index:]

# 4. Modeling
rf_model = RandomForestRegressor(n_estimators=100, random_state=42)
rf_model.fit(X_train, y_train)

gb_model = GradientBoostingRegressor(n_estimators=100, random_state=42)
gb_model.fit(X_train, y_train)
```

### 📊 Empirical Pipeline Results:

| Model | Test MSE | Test RMSE | Test MAE | Test $R^2$ Score |
|-------|----------|-----------|----------|------------------|
| **Random Forest Regressor** | `8.1437` | `2.8537 °C` | `2.1201 °C` | `0.6427` |
| **Gradient Boosting Regressor** | **`6.5868`** | **`2.5665 °C`** | **`1.8940 °C`** | **`0.7110`** |

**Feature Importance Breakdown (Random Forest):**
- `phosphate` (current): **80.75%**
- `temperature_lag_1` (t-1): **7.99%**
- `phosphate_lag_1` (t-1): **3.77%**
- `temperature_lag_3` (t-3): **2.32%**

---

## 📁 Project Structure

```
data-structures/
│
├── data/
│   └── ocean.csv                               # Dataset: 1,406 oceanographic records
│
├── docs/
│   └── ΑΝΑΦΟΡΑ ΔΟΜΕΣ.pdf                      # Full technical report (Greek)
│
├── ml_pipeline/
│   └── ocean_ml_pipeline.py                    # Python Time-Series ML Pipeline (RF & GB)
│
├── part1/
│   ├── sorting/
│   │   ├── insertion_quicksort_temperature.c   # Meros 1 – Insertion Sort & Quick Sort
│   │   └── heap_counting_sort_phosphate.c      # Meros 2 – Heap Sort & Counting Sort
│   │
│   └── searching/
│       ├── binary_interpolation_search.c       # Meros 3 – Binary Search & Interpolation Search
│       └── binary_interpolation_hybrid_search.c # Meros 4 – Hybrid Binary-Interpolation Search
│
├── part2/
│   ├── avl-tree/
│   │   ├── avl_crud.c                          # Meros A – AVL Tree: full CRUD + InOrder
│   │   └── avl_minmax.c                        # Meros B – AVL Tree: min/max temperature
│   │
│   └── hash-table/
│       ├── hash_chaining_crud.c                # Meros C – Hash Table with separate chaining
│       └── avl_hash_combined.c                 # Part 2 combined – AVL + Hash Table menu
│
└── README.md
```

---

## 📦 Dataset: `ocean.csv`

The dataset contains **1,406 real oceanographic measurements**.

| Field | Type | Description | Range (approx.) |
|-------|------|-------------|-----------------|
| `date` | string | Date of measurement (`MM/DD/YYYY`) | 2000 – 2019 |
| `temperature` | float | Sea surface temperature in °C | ~0.0 – 30.0 °C |
| `phosphate` | float | Phosphate concentration in μmol/L | ~0.0 – 3.0 μmol/L |


## 🔢 Part 1 — Sorting & Searching Algorithms (C Core)

### 1.1 Meros 1 — Insertion Sort & Quick Sort (Temperature)
**File:** [`part1/sorting/insertion_quicksort_temperature.c`](part1/sorting/insertion_quicksort_temperature.c)
- **Insertion Sort:** Maintains a sorted left portion. $O(n^2)$ worst case, $O(n)$ best case. Dual-key comparator (`temperature` primary, `date` secondary).
- **Quick Sort:** $O(n \log n)$ average time. Uses middle-element pivot selection (`arr[(left+right)/2]`) to avoid worst-case behavior on pre-sorted arrays.

### 1.2 Meros 2 — Heap Sort & Counting Sort (Phosphate)
**File:** [`part1/sorting/heap_counting_sort_phosphate.c`](part1/sorting/heap_counting_sort_phosphate.c)
- **Heap Sort:** Builds a max-heap iteratively (`max_heapify`), then extracts maximum elements. Guaranteed $O(n \log n)$ time and $O(1)$ space.
- **Counting Sort:** Non-comparison integer sort. Truncates phosphate values to integers for counting and prefix-sum distribution ($O(n+k)$ time).

### 1.3 Meros 3 — Binary Search & Interpolation Search (Date)
**File:** [`part1/searching/binary_interpolation_search.c`](part1/searching/binary_interpolation_search.c)
- **Binary Search:** Iterative midpoint halving ($O(\log n)$ time).
- **Interpolation Search:** Estimates target index using key distribution:
  $\text{mid} = \text{low} + \frac{\text{strcmp}(x, A[\text{low}]) \cdot (\text{high} - \text{low})}{\text{strcmp}(A[\text{high}], A[\text{low}])}$.
  Achieves $O(\log \log n)$ average time on uniform distributions.

### 1.4 Meros 4 — Hybrid Binary-Interpolation Search
**File:** [`part1/searching/binary_interpolation_hybrid_search.c`](part1/searching/binary_interpolation_hybrid_search.c)
- Multi-phase algorithm combining Interpolation estimate, $\sqrt{n}$ square-root jumping to bracket the key, and Linear Search fallback for subarrays $\le 3$ elements.

---

## 🌳 Part 2 — AVL Trees & Hash Tables (C Core)

### 2.A Meros A — AVL Tree Full CRUD + InOrder Traversal
**File:** [`part2/avl-tree/avl_crud.c`](part2/avl-tree/avl_crud.c) | **Key: date**
- Enforces balance invariant $|h_{\text{left}} - h_{\text{right}}| \le 1$ using rotations (LL, RR, LR, RL).
- Guarantees $O(\log n)$ insertion, search, update, and deletion.
- InOrder traversal outputs records in chronological order.

### 2.B Meros B — AVL Tree Min/Max Temperature Queries
**File:** [`part2/avl-tree/avl_minmax.c`](part2/avl-tree/avl_minmax.c) | **Key: temperature**
- Finds minimum (leftmost node) and maximum (rightmost node) temperatures in $O(\log n)$ time.

### 2.C Meros C — Hash Table with Separate Chaining
**File:** [`part2/hash-table/hash_chaining_crud.c`](part2/hash-table/hash_chaining_crud.c)
- 11 buckets using singly-linked lists. Hash function: $\sum \text{ASCII}(\text{date}[i]) \bmod 11$.
- Average $O(1)$ lookup and insert.

### 2.D Combined — AVL Tree + Hash Table Menu
**File:** [`part2/hash-table/avl_hash_combined.c`](part2/hash-table/avl_hash_combined.c)
- Unified runtime interface letting users dynamically select between AVL tree indexing and Hash Table storage.

---

## ⚙️ How to Run & Compile

### 🐍 1. Python ML Pipeline
```bash
# Install requirements
pip install pandas numpy scikit-learn

# Run the Machine Learning pipeline
python ml_pipeline/ocean_ml_pipeline.py
```

### 💻 2. C Programs (GCC)
```bash
# Copy dataset next to executables if needed
cp data/ocean.csv .

# Part 1 — Sorting & Searching
gcc -o sort_temp       part1/sorting/insertion_quicksort_temperature.c   -lm
gcc -o sort_phosphate  part1/sorting/heap_counting_sort_phosphate.c       -lm
gcc -o search_bi       part1/searching/binary_interpolation_search.c      -lm
gcc -o search_hybrid   part1/searching/binary_interpolation_hybrid_search.c -lm

# Part 2 — AVL & Hash Table
gcc -o avl_crud        part2/avl-tree/avl_crud.c    -lm
gcc -o avl_minmax      part2/avl-tree/avl_minmax.c  -lm
gcc -o hash_crud       part2/hash-table/hash_chaining_crud.c      -lm
gcc -o combined        part2/hash-table/avl_hash_combined.c        -lm
```

---

## 📐 Algorithm Complexity Reference

| Structure / Algorithm | Best Case | Average Case | Worst Case | Space |
|-----------------------|-----------|--------------|------------|-------|
| **Insertion Sort**    | $O(n)$    | $O(n^2)$     | $O(n^2)$   | $O(1)$ |
| **Quick Sort**        | $O(n \log n)$ | $O(n \log n)$ | $O(n^2)$   | $O(\log n)$ |
| **Heap Sort**         | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(1)$ |
| **Counting Sort**     | $O(n+k)$  | $O(n+k)$     | $O(n+k)$   | $O(k)$ |
| **Binary Search**     | $O(1)$    | $O(\log n)$  | $O(\log n)$ | $O(1)$ |
| **Interpolation Search** | $O(1)$ | $O(\log \log n)$ | $O(n)$  | $O(1)$ |
| **AVL Tree (CRUD)**   | $O(1)$    | $O(\log n)$  | $O(\log n)$ | $O(n)$ |
| **Hash Table (Chaining)** | $O(1)$ | $O(1)$      | $O(n)$     | $O(n+k)$ |
| **Random Forest ML**  | —         | $O(M \cdot K \cdot n \log n)$ | — | $O(M \cdot \text{nodes})$ |

---

## 📚 References

- Cormen, T. H., et al. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
- Breiman, L. (2001). *Random Forests*. Machine Learning, 45(1), 5-32.
- Adelson-Velsky, G. & Landis, E. M. (1962). *An algorithm for the organization of information*.
- Technical Report PDF: [`docs/ΑΝΑΦΟΡΑ ΔΟΜΕΣ.pdf`](docs/ΑΝΑΦΟΡΑ%20ΔΟΜΕΣ.pdf)

---

## 👤 Author
Academic Project — **Data Structures & Machine Learning**  
Languages: **C (C99)** & **Python 3** | Dataset: Oceanographic Measurement Time-Series

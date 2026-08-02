# 📊 Data Structures & Algorithms — Ocean Temperature Dataset

> A C-language academic project implementing **sorting algorithms**, **search algorithms**, **AVL trees**, and **hash tables** on real oceanographic measurement data, coupled with a **Python Machine Learning Time Series Pipeline** for ocean temperature forecasting.

---

## 🤖 Connection to Artificial Intelligence & Machine Learning

The data structures and algorithms implemented in this C project are not merely classic academic exercises — they are the **fundamental architectural building blocks of modern AI systems**.

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
- **Architectural Homogeneity:** In Machine Learning, tree-based models like **Random Forest** and **XGBoost** split data at each node using threshold conditions (e.g., `temperature < 20.0°C`). This decision split logic is identical to the Binary Search Tree ordering implemented in [`part2/avl-tree/avl_crud.c`](part2/avl-tree/avl_crud.c).
- **Tree Balancing & Depth Control:** While AVL trees enforce $|h_{\text{left}} - h_{\text{right}}| \le 1$ via rotations to guarantee $O(\log n)$ height, decision trees use hyperparameter constraints like `max_depth` and cost-complexity pruning to prevent over-fitting.

### 2. Beam Search in Large Language Models (LLMs) ↔ Max-Heap / Priority Queue
- **Token Decoding:** In LLMs (such as GPT-4 or Claude), **Beam Search** maintains the top-$K$ highest probability sequence candidates at each decoding step.
- **Priority Queue Logic:** This relies on a bounded Max-Heap of size $K$ — using the exact heapify and extract logic implemented in [`part1/sorting/heap_counting_sort_phosphate.c`](part1/sorting/heap_counting_sort_phosphate.c).

### 3. Embeddings & LLM KV Cache ↔ Hash Tables with Chaining
- **Vector Embedding Lookups:** Neural network embedding layers map discrete tokens/IDs to dense vector representations via $O(1)$ hash table lookups.
- **Attention KV Cache:** Modern Transformer inference engines store pre-computed Key/Value attention states in a hash-indexed cache structure, mirroring the hash function and bucket chaining of [`part2/hash-table/hash_chaining_crud.c`](part2/hash-table/hash_chaining_crud.c).

### 4. Time-Series Forecasting ↔ Sorting & Searching
- **Multivariate Time Series:** Oceanographic datasets consist of sequential measurements (`date`, `temperature`, `phosphate`).
- **Lag Features & Ordering:** Supervised ML algorithms require strictly ordered sequences. Quick Sort ($O(n \log n)$) and Binary Search ($O(\log n)$) enable rapid time-window slicing and historical lag creation ($t-1, t-2, t-3$).

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

---

### 📊 Full Model Training & Evaluation Report

The ML models were executed directly on the ocean dataset (1,405 records, 2000–2019). Below are the empirical evaluation results:

#### 1. Execution Terminal Output Log
```text
Loading dataset from: data\ocean.csv
Dataset loaded: 1405 records from 2000-01-07 to 2019-11-18
Train samples: 1121 | Test samples: 281

--- Training Random Forest Regressor ---
Random Forest Test MSE:  8.1437
Random Forest Test RMSE: 2.8537
Random Forest Test MAE:  2.1201
Random Forest Test R2:   0.6427

--- Training Gradient Boosting Regressor ---
Gradient Boosting Test MSE:  6.5868
Gradient Boosting Test RMSE: 2.5665
Gradient Boosting Test MAE:  1.8940
Gradient Boosting Test R2:   0.7110

--- Feature Importances (Random Forest) ---
  phosphate           : 80.75%
  temperature_lag_1   : 7.99%
  phosphate_lag_1     : 3.77%
  temperature_lag_2   : 1.71%
  phosphate_lag_2     : 1.78%
  temperature_lag_3   : 2.32%
  phosphate_lag_3     : 1.68%
```

#### 2. Quantitative Model Evaluation Metrics Comparison

| Metric | Random Forest | Gradient Boosting (XGBoost logic) | Target Direction | Description |
|---|---|---|---|---|
| **Mean Squared Error (MSE)** | `8.1437` | **`6.5868`** | Lower is better | Average squared prediction error ($\text{°C}^2$) |
| **Root Mean Squared Error (RMSE)** | `2.8537 °C` | **`2.5665 °C`** | Lower is better | Standard deviation of prediction residuals ($\text{°C}$) |
| **Mean Absolute Error (MAE)** | `2.1201 °C` | **`1.8940 °C`** | Lower is better | Mean magnitude of absolute errors ($\text{°C}$) |
| **$R^2$ Score (Variance Explained)** | `0.6427` | **`0.7110`** | Higher is better | Proportion of variance in temperature explained |

#### 3. Feature Importance Analysis

| Feature Name | Feature Type | Importance (%) | Interpretation |
|---|---|---|---|
| `phosphate` | Target-correlated Feature | **80.75%** | Current phosphate concentration strongly inverse-correlates with ocean temperature |
| `temperature_lag_1` ($t-1$) | Historical Lag Feature | **7.99%** | Immediate preceding temperature reading |
| `phosphate_lag_1` ($t-1$) | Historical Lag Feature | **3.77%** | Preceding phosphate reading |
| `temperature_lag_3` ($t-3$) | Historical Lag Feature | **2.32%** | Temperature reading from 3 steps back |
| `phosphate_lag_2` ($t-2$) | Historical Lag Feature | **1.78%** | Phosphate reading from 2 steps back |
| `temperature_lag_2` ($t-2$) | Historical Lag Feature | **1.71%** | Temperature reading from 2 steps back |
| `phosphate_lag_3` ($t-3$) | Historical Lag Feature | **1.68%** | Phosphate reading from 3 steps back |

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


## 📦 Dataset Architecture: `ocean.csv`

The dataset contains **1,406 real oceanographic measurements** recorded between 2000 and 2019.

| Field | Type | C Type | Description |
|---|---|---|---|
| `date` | string | `char date[11]` | Measurement date formatted as `MM/DD/YYYY` (11 bytes null-terminated) |
| `temperature` | float | `float temperature` | Sea surface temperature in degrees Celsius (°C) |
| `phosphate` | float | `float phosphate` | Phosphate concentration in micromoles per liter (μmol/L) |

**C Parsing Mechanism:**
```c
FILE *fp = fopen("ocean.csv", "r");
char line[200], *token;
fscanf(fp, "%s\n", line); // Consume CSV header row
while (fscanf(fp, "%s\n", line) != EOF) {
    token = strtok(line, ",");
    strcpy(array[count].date, token);
    token = strtok(NULL, ",");
    array[count].temperature = atof(token);
    count++;
}
```

---

## 🔢 Part 1 — Sorting & Searching Algorithms (C Core)

### 1.1 Meros 1 — Insertion Sort & Quick Sort (Temperature)
**File:** [`part1/sorting/insertion_quicksort_temperature.c`](part1/sorting/insertion_quicksort_temperature.c)
- **Insertion Sort ($O(n^2)$):** Iteratively shifts elements right to insert the current item into its sorted position. Employs a dual-key comparator (`temperature` primary, `date` secondary).
- **Quick Sort ($O(n \log n)$):** Uses a middle-element pivot strategy (`arr[(left+right)/2]`) to avoid $O(n^2)$ degeneration on already-sorted sequences. Timing measured in milliseconds via `<sys/timeb.h>`.

### 1.2 Meros 2 — Heap Sort & Counting Sort (Phosphate)
**File:** [`part1/sorting/heap_counting_sort_phosphate.c`](part1/sorting/heap_counting_sort_phosphate.c)
- **Heap Sort ($O(n \log n)$):** Constructs a max-heap in $O(n)$ time via `max_heapify()`, then performs $n-1$ extractions into the array tail. Guaranteed $O(1)$ auxiliary space.
- **Counting Sort ($O(n+k)$):** Non-comparison integer sorting algorithm. Truncates floating-point phosphate values to integers to populate frequency arrays and prefix-sum distribution tables.

### 1.3 Meros 3 — Binary Search & Interpolation Search (Date)
**File:** [`part1/searching/binary_interpolation_search.c`](part1/searching/binary_interpolation_search.c)
- **Binary Search ($O(\log n)$):** Standard iterative bisection using `strcmp()` on date strings.
- **Interpolation Search ($O(\log \log n)$ average):** Calculates position estimate based on key value distribution:
  $$\text{mid} = \text{low} + \frac{\text{strcmp}(x, A[\text{low}]) \cdot (\text{high} - \text{low})}{\text{strcmp}(A[\text{high}], A[\text{low}])}$$

### 1.4 Meros 4 — Hybrid Binary-Interpolation Search
**File:** [`part1/searching/binary_interpolation_hybrid_search.c`](part1/searching/binary_interpolation_hybrid_search.c)
- **Three-Phase Algorithm:**
  1. *Interpolation Phase:* Predicts initial index via key distribution.
  2. *Square-Root Jumping ($\sqrt{n}$):* Steps forward/backward in $\sqrt{\text{size}}$ blocks to rapidly bracket the target interval.
  3. *Linear Fallback:* When remaining subarray size $\le 3$, switches to linear search to eliminate recursion/math overhead.

---

## 🌳 Part 2 — AVL Trees & Hash Tables (C Core)

### Core AVL Tree Mechanics & Rotations
An **AVL Tree** is a self-balancing Binary Search Tree where node height balances satisfy $|h_{\text{left}} - h_{\text{right}}| \le 1$.

```
LL Rotation (Right Rotate):                 RR Rotation (Left Rotate):
      z                y                          x                y
     / \             /   \                       / \             /   \
    y   T4   ==>    x     z                     T1  y    ==>    x     z
   / \             / \   / \                       / \         / \   / \
  x   T3          T1 T2 T3 T4                     T2  z       T1 T2 T3 T4
```

### 2.A Meros A — AVL Tree Full CRUD + InOrder Traversal
**File:** [`part2/avl-tree/avl_crud.c`](part2/avl-tree/avl_crud.c) | **Key: `date`**
- **Operations:** Insert, Search, Update, Delete, and InOrder Traversal.
- **InOrder Traversal:** Left $\to$ Root $\to$ Right traversal outputs all records chronologically in $O(n)$ time.
- **Deletion:** Replaces node with its in-order successor (leftmost node of right subtree) and rebalances bottom-up via LL, RR, LR, or RL rotations.

### 2.B Meros B — AVL Tree Min/Max Temperature Queries
**File:** [`part2/avl-tree/avl_minmax.c`](part2/avl-tree/avl_minmax.c) | **Key: `temperature`**
- **Minimum Temperature:** Traverses the leftmost path ($O(\log n)$ time).
- **Maximum Temperature:** Traverses the rightmost path ($O(\log n)$ time).

### 2.C Meros C — Hash Table with Separate Chaining
**File:** [`part2/hash-table/hash_chaining_crud.c`](part2/hash-table/hash_chaining_crud.c)
- **Architecture:** Fixed-size array of 11 bucket heads (`chain[11]`), each pointing to a singly-linked list.
- **Hash Function:** ASCII character summation modulo prime table size:
  $$\text{hash}(str) = \left( \sum_{i=0}^{9} \text{ASCII}(str[i]) \right) \bmod 11$$
- **Operations:** Search, Update, Delete in average $O(1)$ time.

### 2.D Combined — AVL Tree + Hash Table Menu Interface
**File:** [`part2/hash-table/avl_hash_combined.c`](part2/hash-table/avl_hash_combined.c)
- Provides a unified interactive console menu enabling the user to dynamically choose between an AVL Tree index (by date or temperature) and a Hash Table index at runtime.

---

## ⚙️ How to Compile & Run

### 🐍 1. Python ML Pipeline
```bash
# Install Python dependencies
pip install pandas numpy scikit-learn

# Run the Machine Learning pipeline
python ml_pipeline/ocean_ml_pipeline.py
```

### 💻 2. C Programs (GCC Compiler)
```bash
# Ensure dataset is present
cp data/ocean.csv .

# Part 1 — Sorting & Searching
gcc -o sort_temp       part1/sorting/insertion_quicksort_temperature.c   -lm
gcc -o sort_phosphate  part1/sorting/heap_counting_sort_phosphate.c       -lm
gcc -o search_bi       part1/searching/binary_interpolation_search.c      -lm
gcc -o search_hybrid   part1/searching/binary_interpolation_hybrid_search.c -lm

# Part 2 — AVL Trees & Hash Table
gcc -o avl_crud        part2/avl-tree/avl_crud.c    -lm
gcc -o avl_minmax      part2/avl-tree/avl_minmax.c  -lm
gcc -o hash_crud       part2/hash-table/hash_chaining_crud.c      -lm
gcc -o combined        part2/hash-table/avl_hash_combined.c        -lm
```

---

## 📐 Master Algorithm Complexity Reference Matrix

| Structure / Algorithm | Best Case | Average Case | Worst Case | Space Complexity | Stability / Ordered |
|-----------------------|-----------|--------------|------------|------------------|---------------------|
| **Insertion Sort**    | $O(n)$    | $O(n^2)$     | $O(n^2)$   | $O(1)$           | ✅ Stable           |
| **Quick Sort**        | $O(n \log n)$ | $O(n \log n)$ | $O(n^2)$   | $O(\log n)$      | ❌ Unstable         |
| **Heap Sort**         | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(1)$        | ❌ Unstable         |
| **Counting Sort**     | $O(n+k)$  | $O(n+k)$     | $O(n+k)$   | $O(k)$           | ✅ Stable           |
| **Binary Search**     | $O(1)$    | $O(\log n)$  | $O(\log n)$ | $O(1)$           | ✅ Ordered Req.     |
| **Interpolation Search** | $O(1)$ | $O(\log \log n)$ | $O(n)$  | $O(1)$           | ✅ Ordered Req.     |
| **AVL Tree (CRUD)**   | $O(1)$    | $O(\log n)$  | $O(\log n)$ | $O(n)$           | ✅ Ordered Traversal|
| **Hash Table (Chaining)** | $O(1)$ | $O(1)$      | $O(n)$     | $O(n+k)$         | ❌ Unordered        |
| **Random Forest ML**  | —         | $O(M \cdot K \cdot n \log n)$ | — | $O(M \cdot \text{nodes})$ | Supervised ML |

---

## 📚 References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
- Breiman, L. (2001). *Random Forests*. Machine Learning, 45(1), 5-32.
- Adelson-Velsky, G. & Landis, E. M. (1962). *An algorithm for the organization of information*. Doklady Akademii Nauk SSSR.
- Full Technical Report PDF: [`docs/ΑΝΑΦΟΡΑ ΔΟΜΕΣ.pdf`](docs/ΑΝΑΦΟΡΑ%20ΔΟΜΕΣ.pdf)

---

## 👤 Author
Academic Project — **Data Structures & Machine Learning**  
Languages: **C (C99)** & **Python 3** | Dataset: Oceanographic Measurement Time-Series

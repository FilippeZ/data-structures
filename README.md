# 📊 Data Structures & Algorithms — Ocean Temperature Dataset

> A C-language academic project implementing **sorting algorithms**, **search algorithms**, **AVL trees**, and **hash tables** on real oceanographic measurement data.
> All structures and algorithms are also analysed through the lens of **Artificial Intelligence & Machine Learning** — where the same data structures power modern AI systems.

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
├── part1/
│   ├── sorting/
│   │   ├── insertion_quicksort_temperature.c   # Meros 1 – Insertion Sort & Quick Sort (temperature)
│   │   └── heap_counting_sort_phosphate.c      # Meros 2 – Heap Sort & Counting Sort (phosphate)
│   │
│   └── searching/
│       ├── binary_interpolation_search.c       # Meros 3 – Binary Search & Interpolation Search (date)
│       └── binary_interpolation_hybrid_search.c # Meros 4 – Hybrid Binary-Interpolation Search
│
├── part2/
│   ├── avl-tree/
│   │   ├── avl_crud.c                          # Meros A – AVL Tree: full CRUD + InOrder traversal
│   │   └── avl_minmax.c                        # Meros B – AVL Tree: min/max temperature queries
│   │
│   └── hash-table/
│       ├── hash_chaining_crud.c                # Meros C – Hash Table with separate chaining
│       └── avl_hash_combined.c                 # Part 2 combined – AVL + Hash Table unified menu
│
└── README.md
```

---

## 📦 Dataset: `ocean.csv`

The dataset contains **1,406 real oceanographic measurements** collected over time.

| Field         | Type    | Description                                              | Range (approx.)    |
|---------------|---------|----------------------------------------------------------|--------------------|
| `date`        | string  | Date of measurement in `YYYY-MM-DD` format (11 chars)   | Multi-year span    |
| `temperature` | float   | Sea surface temperature in Celsius (°C)                  | ~0.0 – 30.0 °C     |
| `phosphate`   | float   | Phosphate concentration in μmol/L                        | ~0.0 – 3.0 μmol/L  |

**How data is parsed (shared across all programs):**

```c
fscanf(fp, "%s\n", line);              // skip header row
while (fscanf(fp, "%s\n", line) != EOF) {
    token = strtok(line, ",");         // token 1 = date
    strcpy(array[count].date, token);
    token = strtok(NULL, ",");         // token 2 = value
    array[count].temperature = atof(token);
    count++;
}
```

> All programs expect `ocean.csv` in the **same directory** as the compiled executable.

---

## 🔢 Part 1 — Sorting & Searching Algorithms

---

### 1.1 Meros 1 — Insertion Sort & Quick Sort (Temperature)

**File:** `part1/sorting/insertion_quicksort_temperature.c`

Loads all 1,406 records and benchmarks two comparison-based sorting algorithms on `temperature`. Ties are broken alphabetically by `date`.

#### Insertion Sort

Maintains a sorted left portion. For each new element, shifts right until the correct position is found.

```
[30.1 | 28.5 | 30.1 | 27.3]
→ [28.5 | 30.1 | 30.1 | 27.3]   (28.5 inserted)
→ [27.3 | 28.5 | 30.1 | 30.1]   (27.3 moves to front)
```

**Dual-key comparator:**
```c
while (j > 0 &&
       a[j].temperature < a[j-1].temperature ||
      (a[j].temperature == a[j-1].temperature &&
       strcmp(a[j].date, a[j-1].date) < 0)) {
    temp = a[j]; a[j] = a[j-1]; a[j-1] = temp;
    j--;
}
```

| Property | Value |
|----------|-------|
| Best case | O(n) — already sorted |
| Average / Worst | O(n²) |
| Space | O(1) in-place |
| Stable | Yes — equal temps keep original order |
| Adaptive | Yes — faster on nearly-sorted input |

#### Quick Sort

Middle-pivot partitioning. Custom `compare_data()` handles the dual-key (temperature + date):

```c
static void quicksort_(mydata *arr, int left, int right) {
    mydata p = arr[(left + right) / 2];
    int l = left, r = right;
    while (l <= r) {
        while (compare_data(arr+l, &p) < 0) ++l;
        while (compare_data(arr+r, &p) > 0) --r;
        if (l <= r) { swap_data(arr+l, arr+r); ++l; --r; }
    }
    if (left < r)  quicksort_(arr, left, r);
    if (l < right) quicksort_(arr, l, right);
}
```

| Property | Value |
|----------|-------|
| Best / Average | O(n log n) |
| Worst | O(n²) — when pivot always min/max |
| Space | O(log n) stack |
| Stable | No |

**Middle-pivot advantage:** Avoids worst-case on sorted input (unlike first/last-element pivots).

---

### 1.2 Meros 2 — Heap Sort & Counting Sort (Phosphate)

**File:** `part1/sorting/heap_counting_sort_phosphate.c`

#### Heap Sort

Builds a max-heap iteratively then extracts maximum elements one by one.

**max_heapify:** For each node, push it up to its correct heap position comparing both `phosphate` and `date`.
**heap_sort:** Calls `max_heapify` with shrinking heap size, placing each max at the tail.

| Property | Value |
|----------|-------|
| All cases | O(n log n) — guaranteed |
| Space | O(1) in-place |
| Stable | No |
| Cache | Poor — heap jumps across memory |

#### Counting Sort

Non-comparison sort. Counts occurrences of each integer value, builds prefix sums, then places elements.

```c
int max = (int)getMax(a, n);
int count[max+1];
for (i=0; i<=max; i++) count[i] = 0;
for (i=0; i<n; i++)   count[(int)a[i].phosphate]++;
for (i=1; i<=max; i++) count[i] += count[i-1];    // prefix sum
for (i=n-1; i>=0; i--) {
    output[count[(int)a[i].phosphate]-1] = a[i];
    count[(int)a[i].phosphate]--;
}
```

| Property | Value |
|----------|-------|
| Time | O(n + k) where k = max phosphate value |
| Space | O(n + k) |
| Stable | Yes |
| Prerequisite | Integer keys — phosphate is cast to int here |

> ⚠️ Casting phosphate to int loses fractional precision. Intentional educational simplification.

---

### 1.3 Meros 3 — Binary Search & Interpolation Search (Date)

**File:** `part1/searching/binary_interpolation_search.c`

The array **must be sorted by date** for both algorithms to work correctly.

#### Binary Search

At each step, compare the key with the midpoint; eliminate half the interval.

```c
int binarySearch(mydata a[], char key[11], int n) {
    int low = 0, high = n-1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (strcmp(key, a[mid].date) == 0) return mid;
        else if (strcmp(key, a[mid].date) > 0) low  = mid + 1;
        else                                    high = mid - 1;
    }
    return -1;
}
```

| Complexity (all cases) | O(log n) — guaranteed |
|---|---|
| Space | O(1) |
| Key type | Any orderable (string via strcmp here) |

#### Interpolation Search

Estimates the position of the key based on its value relative to the bounds — like opening a dictionary near the expected letter.

```c
mid = low + (strcmp(x, A[low].date) * (high - low)
             / strcmp(A[high].date, A[low].date));
```

| Average case | O(log log n) — much faster than binary on uniform data |
|---|---|
| Worst case | O(n) — clustered keys |
| Space | O(1) |

> Using `strcmp()` as arithmetic distance is a pedagogical approximation. Not a true linear interpolation, but converges faster in practice for date strings.

---

### 1.4 Meros 4 — Hybrid Binary-Interpolation Search

**File:** `part1/searching/binary_interpolation_hybrid_search.c`

Three-phase hybrid that combines the strengths of all approaches:

**Phase 1:** Compute initial interpolated position `next`.

**Phase 2 — Square-root jumping:**
```
IF remaining subarray size <= 3 → Phase 3 (linear)
IF key >= arr[next]:
    jump FORWARD in steps of √size until bracket found
    set new [left, right] bounds
ELSE:
    jump BACKWARD in steps of √size
    set new [left, right] bounds
Recalculate next via interpolation within new bounds
```

**Phase 3:** Linear scan on tiny subarray (size ≤ 3).

| Phase | Strategy | Complexity |
|-------|----------|------------|
| Initial estimate | Interpolation | O(1) |
| Jump phase | √n steps | O(√n) |
| Narrowing | Interpolation | O(log log n) avg |
| Fallback | Linear Search | O(1) practically |

---

## 🌳 Part 2 — AVL Trees & Hash Tables

---

### Shared Node Definitions

```c
typedef struct dedomena { char date[11]; float temperature; } mydata;

struct Node {          // AVL tree node
    char  date[11]; float temperature;
    struct Node *left, *right;
    int height;
};

struct node {          // Hash chain node
    float temperature; char date[11];
    struct node *next;
};
```

---

### Core AVL Tree Theory

AVL trees maintain: `|height(left) − height(right)| ≤ 1` at every node.

**Four rotation cases:**

```
LL (balance=+2, left child balance≥0):
         z                y
        / \             /   \
       y   T4   →      x     z
      / \              T1 T2 T3 T4
     x   T3
    Single Right Rotation

RR: Mirror of LL → Single Left Rotation
LR: Left rotate left child, then Right rotate root
RL: Right rotate right child, then Left rotate root
```

**Height update (after every rotation):**
```c
node->height = 1 + max(height(node->left), height(node->right));
```

---

### 2.A Meros A — AVL Tree Full CRUD + InOrder Traversal

**File:** `part2/avl-tree/avl_crud.c` | **Key: date (string)**

Loads all 1,406 records into an AVL tree keyed by date, then serves a persistent menu.

**Insert:** Recursive BST insertion by `strcmp`, then rebalance bottom-up with all 4 rotation cases.

**InOrder Traversal:** Left → Root → Right. Produces all 1,406 records in chronological order:
```c
void printInorder(struct Node* node) {
    if (node == NULL) return;
    printInorder(node->left);
    printf("--- Date: %s  Temp: %f ---\n", node->date, node->temperature);
    printInorder(node->right);
}
```

**Search:** Recursive descent via `strcmp`:
```c
struct Node* search(struct Node* root, char imerominia[11]) {
    if (root == NULL || strcmp(root->date, imerominia) == 0) return root;
    if (strcmp(root->date, imerominia) < 0) return search(root->right, imerominia);
    return search(root->left, imerominia);
}
```

**Update:** Find node via `search()`, overwrite temperature in-place — no structural change (key unchanged).

**Delete (3 cases):**
1. Leaf → free, return NULL
2. One child → replace with child, free old node
3. Two children → copy in-order successor data, delete successor from right subtree

Rebalance bottom-up after every deletion.

| Operation | Time |
|-----------|------|
| Insert / Search / Update / Delete | O(log n) |
| InOrder Traversal | O(n) |
| Space | O(n) |

---

### 2.B Meros B — AVL Tree Min/Max Temperature

**File:** `part2/avl-tree/avl_minmax.c` | **Key: temperature (float)**

In any BST keyed by temperature, the minimum is the **leftmost** node and maximum is the **rightmost** node.

```c
char* minValue(struct Node* node) {
    while (node->left != NULL) node = node->left;
    return node->date;  // O(log n) traversal
}
char* maxValue(struct Node* node) {
    while (node->right != NULL) node = node->right;
    return node->date;  // O(log n) traversal
}
```

> ⚠️ Duplicate temperature values are silently dropped (BST equality → `return node` without insert). A multi-set AVL (chain at each node) would be needed to retain all records with identical temperatures.

---

### 2.C Meros C — Hash Table with Separate Chaining

**File:** `part2/hash-table/hash_chaining_crud.c`

**Structure:** 11 buckets, each a singly-linked list.

```
chain[0] → [1990-01-15, 18.2] → [1990-12-01, 19.1] → NULL
chain[6] → [1992-03-10, 22.5] → NULL
...
Expected chain length: 1406/11 ≈ 128 nodes/bucket
```

**Hash Function:**
```c
int calculate_hash_value(char str[11]) {
    int sum = 0;
    for (int i = 0; i < 10; i++) sum += str[i];  // sum ASCII values
    return sum % 11;
}
```

**Example: date = "1990-01-15"**
```
'1'=49, '9'=57, '9'=57, '0'=48, '-'=45, '0'=48, '1'=49, '-'=45, '1'=49, '5'=53
Sum = 500 → 500 % 11 = 6 → stored in chain[6]
```

**Weaknesses:**
- Anagram dates (same character multiset) produce the same hash
- Narrow ASCII range for date digits causes uneven distribution
- Production alternative: `FNV-1a` polynomial rolling hash

**Why size 11?** Prime modulus reduces systematic collisions from input regularities.

| Operation | Average | Worst (all in one bucket) |
|-----------|---------|---------------------------|
| Insert / Search / Update / Delete | O(1) | O(n) |

**Delete logic (two sub-cases):**
```c
if (head matches) {
    chain[key] = head->next;
    free(head);
} else {
    walk chain until prev->next matches;
    prev->next = target->next;
    free(target);
}
```

---

### 2.D Combined — AVL Tree + Hash Table Unified Menu

**File:** `part2/hash-table/avl_hash_combined.c`

Chooses data structure at runtime. If AVL is selected, further chooses key type (date or temperature). Demonstrates that both structures expose the same data management interface despite different internal architectures.

---

## ⚙️ How to Compile & Run

All files are **C99** standard.

```bash
# Part 1 – Sorting
gcc -o sort_temp       part1/sorting/insertion_quicksort_temperature.c   -lm
gcc -o sort_phosphate  part1/sorting/heap_counting_sort_phosphate.c       -lm

# Part 1 – Searching
gcc -o search_bi       part1/searching/binary_interpolation_search.c      -lm
gcc -o search_hybrid   part1/searching/binary_interpolation_hybrid_search.c -lm

# Part 2 – AVL Trees
gcc -o avl_crud        part2/avl-tree/avl_crud.c    -lm
gcc -o avl_minmax      part2/avl-tree/avl_minmax.c  -lm

# Part 2 – Hash Table & Combined
gcc -o hash_crud       part2/hash-table/hash_chaining_crud.c      -lm
gcc -o combined        part2/hash-table/avl_hash_combined.c        -lm
```

```bash
cp data/ocean.csv .    # copy dataset beside executables
./sort_temp
./avl_crud
```

> **Windows:** Use `executable.exe`. The `<sys/timeb.h>` timing header is Windows-native.


---

## 📐 Algorithm Complexity Summary

### Sorting

| Algorithm       | Best       | Average    | Worst      | Space    | Stable | In-place |
|-----------------|------------|------------|------------|----------|--------|----------|
| Insertion Sort  | O(n)       | O(n²)      | O(n²)      | O(1)     | Yes    | Yes      |
| Quick Sort      | O(n log n) | O(n log n) | O(n²)      | O(log n) | No     | Yes      |
| Heap Sort       | O(n log n) | O(n log n) | O(n log n) | O(1)     | No     | Yes      |
| Counting Sort   | O(n+k)     | O(n+k)     | O(n+k)     | O(k)     | Yes    | No       |

### Searching

| Algorithm                   | Average      | Worst    | Prerequisite              |
|-----------------------------|--------------|----------|---------------------------|
| Binary Search               | O(log n)     | O(log n) | Sorted array              |
| Interpolation Search        | O(log log n) | O(n)     | Sorted, uniform keys      |
| Hybrid Binary-Interpolation | O(sqrt(n))   | O(n)     | Sorted array              |
| Linear Search (fallback)    | O(n)         | O(n)     | None                      |

### Data Structures

| Structure             | Insert   | Search   | Update   | Delete   | Space  | Ordered |
|-----------------------|----------|----------|----------|----------|--------|---------|
| AVL Tree              | O(log n) | O(log n) | O(log n) | O(log n) | O(n)   | Yes     |
| Hash Table (Chaining) | O(1) avg | O(1) avg | O(1) avg | O(1) avg | O(n+k) | No      |

---

## 📋 Design Decisions & Trade-offs

### AVL Tree vs Plain BST
A plain BST can degenerate to O(n) height on sorted/partially-sorted input — exactly the case for date sequences. AVL guarantees O(log n) height at all times via rotations.

### Separate Chaining vs Open Addressing

| Factor | Separate Chaining (used here) | Open Addressing |
|--------|-------------------------------|-----------------|
| High load factor | Handles gracefully (longer chains) | Degrades rapidly |
| Delete | Simple — unlink the node | Needs tombstones or re-hashing |
| Memory | Extra pointer per node | Contiguous array — better cache |

With load factor ~128, open addressing is completely impractical here.

### Why Prime Size (11) for Hash Table?
Prime modulus reduces systematic collisions from input regularities. Non-prime moduli (e.g., 10, 16) create aliasing when input patterns are multiples of the modulus.

### Hash Function Improvement Path
Current: `sum(ASCII) % 11` — weak, anagram-vulnerable.
Better:
```c
// FNV-1a hash — good distribution, avalanche effect
unsigned int fnv1a_hash(char *str, int size) {
    unsigned int hash = 2166136261u;
    for (int i = 0; str[i] && i < 10; i++) {
        hash ^= (unsigned char)str[i];
        hash *= 16777619u;
    }
    return hash % size;
}
```

---

## 🤖 Connection to Artificial Intelligence & Machine Learning

The data structures and algorithms in this project are **foundational building blocks of modern AI systems**. Every major ML framework, vector database, and language model relies on the same concepts implemented here.

---

### Sorting Algorithms in AI

| AI Task | Algorithm Used | Connection to This Project |
|---------|---------------|---------------------------|
| **Recommendation systems** (Netflix, Spotify) | Quick Sort, Merge Sort | Sort items by predicted score; serve top-K |
| **Beam Search** in GPT, BERT | Max-Heap (like Heap Sort) | Keep top-K probability sequences at each decoding step |
| **Ranking** (Google, Elasticsearch) | Tim Sort (Insertion + Merge) | Sort millions of documents by relevance score |
| **Curriculum Learning** in neural nets | Insertion Sort logic | Sort training samples from easy to hard; adaptive to order |
| **k-Nearest Neighbours (kNN)** | Partial sort | Find k closest vectors — need sorted distances, not full sort |

**Beam Search uses the same heap logic as Heap Sort:**
```
At each GPT decoding step:
  1. Score all ~50,000 vocabulary tokens
  2. Keep only top-K scored candidates (beam width K)
  3. This is a partial max-heap of size K
  → Same logic as max_heapify() in heap_counting_sort_phosphate.c
```

**Counting Sort and Histogram Equalization in CNNs:**
The prefix-sum pattern in Counting Sort is identical to the cumulative histogram used in image preprocessing before feeding data to convolutional neural networks. Normalising pixel distributions uses the same O(n+k) bucket-and-prefix-sum logic.

---

### Search Algorithms in AI

| AI Task | Search Concept | Connection |
|---------|---------------|------------|
| **Hyperparameter tuning** | Binary Search (bisection) | Find optimal learning rate by bisecting the performance curve |
| **Vector database lookup** (FAISS, Pinecone) | Approximate NN (ANN) | Locality-sensitive hashing — same principle as Interpolation Search |
| **Decision tree inference** | Binary Search (implicit) | Each tree node splits the data space — one binary search step |
| **Retrieval-Augmented Generation (RAG)** | Hybrid search | Semantic + keyword search combined — mirrors Hybrid Binary-Interpolation |

**Binary Search for Learning Rate Scheduling:**
```
Too high LR → loss diverges
Too low  LR → training is slow
Binary search over [LR_min, LR_max] finds the optimal value —
same logic as binarySearch() in binary_interpolation_search.c
```

**Interpolation Search and Bayesian Optimisation:**
Instead of evaluating at the midpoint (Binary Search), Bayesian Optimisation uses a surrogate model to predict where the optimum is — directly analogous to Interpolation Search estimating position from value distribution rather than always halving.

---

### AVL Trees in AI

| AI / ML System | AVL Tree Concept |
|----------------|-----------------|
| **Decision Trees** (CART, ID3) | BST structure — each node splits data by a feature threshold |
| **Random Forests** | Ensemble of decision trees — each is a BST variant |
| **Gradient Boosted Trees** (XGBoost, LightGBM) | Builds trees iteratively; each internal node = BST split point |
| **Feature stores** (Feast, Hopsworks) | B-trees (generalised AVL) index features for low-latency retrieval |
| **NLP Syntax Trees** | Tree traversal (InOrder, PreOrder) used in parsing and AST analysis |

**AVL Tree ↔ Decision Tree — structural analogy:**
```
AVL Tree Node (this project):       Decision Tree Node (sklearn):
  date < "1995-01-01"?                temperature < 20.0°C?
      /          \                         /            \
   go left     go right             predict "cold"  predict "warm"
```
Both use the same recursive descent pattern as `search()` in `avl_crud.c`.

**InOrder Traversal = Sorted Feature Statistics:**
InOrder traversal of the AVL tree keyed by temperature visits all records from coldest to warmest in O(n). ML systems use this for quantile computation (used in feature normalisation) with a single tree traversal — no separate sort needed.

---

### Hash Tables in AI

| AI Application | Hash Table Concept Used |
|----------------|------------------------|
| **Word embeddings** (Word2Vec, GloVe) | Word → hash lookup → 300-dim float vector |
| **Feature hashing** ("hashing trick") | Map high-cardinality categoricals to fixed-size vector via hash |
| **Gradient caching** in backpropagation | Store intermediate activations in hash map for reuse |
| **Model serving** (Redis, Memcached) | Hash tables cache model predictions for low-latency serving |
| **KV Cache in Transformers** (GPT, Claude) | Cache attention key/value pairs — a hash-indexed store |
| **Dataset deduplication** | Hash each sample; collision = duplicate |

**Feature Hashing — direct extension of calculate_hash_value():**
```python
# Python equivalent — used in scikit-learn HashingVectorizer
def feature_hash(feature_name: str, n_features: int) -> int:
    return hash(feature_name) % n_features
    # Maps any string feature name to a bucket index
    # Same principle as calculate_hash_value() in hash_chaining_crud.c
```

**KV Cache in Large Language Models — the same separate-chaining concept:**
```
LLM Inference KV Cache:
  Key   = (token_position, layer_id)   → hash to cache slot
  Value = pre-computed attention vectors
  Chaining: multiple tokens can map to the same attention head
  → Exact same structure as chain[] in hash_chaining_crud.c
```

---

### AVL + Hash Table = Hybrid Index Structures in AI

The `avl_hash_combined.c` design — choosing between AVL tree or hash table at runtime — mirrors production AI infrastructure:

| AI System | Tree-based Component | Hash-based Component |
|-----------|---------------------|---------------------|
| **Vector databases** (FAISS, Pinecone) | IVF tree index for approx. NN | Inverted hash index for exact ID lookup |
| **Feature stores** (Feast) | B-tree ordered index for time-range queries | Hash index for point lookups by entity ID |
| **ML model registries** (MLflow) | Versioned tree of experiment runs | Hash map from run ID → metadata |

**Rule:** Use ordered tree structures for range queries and sorted traversal; use hash tables for constant-time point lookups. Exactly the trade-off demonstrated by `avl_hash_combined.c`.

---

### The Ocean Dataset in Climate AI

Sea surface temperature and phosphate concentration are the type of data used in real environmental ML models:

| AI Application | Data Used | Relevance to This Dataset |
|----------------|-----------|--------------------------|
| **Sea surface temperature forecasting** | Time-series of temperature | Direct — this is the raw input |
| **Ocean acidification modelling** | Phosphate & CO2 measurements | Phosphate is a proxy for ocean chemistry |
| **Climate anomaly detection** | Historical temperature baselines | AVL min/max queries detect outliers in O(log n) |
| **El Nino / La Nina forecasting** | Multi-year temperature patterns | Date-keyed AVL enables efficient time-range statistics |

**Full ML pipeline using this project's tools:**
```
ocean.csv
    |
    | (parse with strtok — as in all programs here)
    v
Feature Engineering
    |- Sort by date (Quick Sort)       → time-series lag features
    |- Search for anomalies (Binary Search) → compare vs historical mean
    |- Build index (AVL Tree)          → efficient range stats for windows
    |
    v
ML Model (LSTM or Transformer for time-series forecasting)
    |
    v
Output: Predicted sea temperature for next 30 days
```

---

## 📚 References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
- Knuth, D. E. (1998). *The Art of Computer Programming, Vol. 3: Sorting and Searching*. Addison-Wesley.
- Adelson-Velsky, G. & Landis, E. M. (1962). *An algorithm for the organization of information*. Doklady Akademii Nauk SSSR, 146, 263-266.
- Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep Learning*. MIT Press. deeplearningbook.org
- Johnson, J. et al. (2019). *Billion-scale similarity search with GPUs* (FAISS). IEEE TPDS. — Hash + Tree hybrid indexing for AI vector databases.
- Full technical report: `docs/ΑΝΑΦΟΡΑ ΔΟΜΕΣ.pdf`

---

## 👤 Author

Academic project — **Data Structures Course**
Language: **C (C99)** | Platform: **Windows** (`<sys/timeb.h>` millisecond timing in Part 1)

# 🚀 C++ DSA / Competitive Programming Cheat Sheet

![C++14](https://img.shields.io/badge/C%2B%2B-14-blue?logo=cplusplus) ![C++17](https://img.shields.io/badge/C%2B%2B-17-purple) ![C++20](https://img.shields.io/badge/C%2B%2B-20-orange) ![DSA](https://img.shields.io/badge/topic-DSA-brightgreen) ![status](https://img.shields.io/badge/use-revision-yellow)

Quick-revision notes. Baseline is **C++14** (`g++ -std=c++14 -O2 -Wall -Wextra -fsanitize=address,undefined -g` while debugging). Anything marked 🟪 **[C++17]** needs `-std=c++17`; 🟧 **[C++20]** needs `-std=c++20`. Each has a C++14 fallback beside it.

> [!NOTE]
> Legend: 🟦 C++14 (baseline) · 🟪 **[C++17]** · 🟧 **[C++20]** · ⚠️ pitfall · 💡 trick · ✅ do · ❌ avoid.

> [!NOTE]
> **Done with this sheet?** Advanced topics (flows, SCC/2-SAT, HLD, lazy/persistent segment trees, suffix structures, NTT, geometry, game theory, theory and a practice roadmap) live in [notes_advanced.md](notes_advanced.md). Finish sections 1–20 first.

<a id="contents"></a>
## 📑 Contents
1. 🧰 [Template & setup](#s1)
2. 📚 [Standard headers (C++14, with C++17/20 additions marked)](#s2)
3. ⚠️ [Language essentials & pitfalls](#s3)
4. 🏗️ [Structs, classes & OOP essentials](#s4)
5. ⏱️ [Complexity cheat table](#s5)
6. 🧩 [Lambdas, comparators, hashing](#s6)
7. 📦 [STL containers](#s7)
8. 🔧 [STL algorithms & `<numeric>`](#s8)
9. 🔤 [Strings](#s9)
10. 🔢 [Bit manipulation](#s10)
11. ➗ [Math basics (modular arithmetic, GCD)](#s11)
12. 🧮 [Number theory & combinatorics](#s12)
13. 🌿 [Recursion, backtracking, bitmask](#s13)
14. 🎯 [Core techniques](#s14)
15. 🤑 [Greedy patterns](#s15)
16. 🧠 [Dynamic programming](#s16)
17. 🌳 [Trees & advanced data structures](#s17)
18. 🕸️ [Graphs](#s18)
19. 📝 [String algorithms](#s19)
20. 🧭 [Problem-solving flow & pattern map](#s20)
21. 🐞 [Debugging & contest checklist](#s21)

---

<a id="s1"></a>
## 🧰 1. Template & setup

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vll = vector<ll>;
#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()

int main() {
    ios::sync_with_stdio(false);  // don't mix with printf/scanf afterwards
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while (t--) { /* solve */
    }
}
```

- Use `'\n'` not `endl` (endl flushes -> slow).
- Interactive problems: do NOT untie; flush with `cout << flush`.
- Fast read of a whole line: `getline(cin >> ws, s);` (`ws` eats leftover newline after `>>`).
- `INF`: `const int INF = 1e9;` (sum of two INF fits in int), `const ll LINF = 4e18;` (careful with adding). `0x3f3f3f3f` works with `memset`.
- `memset(a, 0, sizeof a)` / `-1` only (byte-wise). Use `fill` / `assign` for other values.
- Random: `mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());` `rng() % n`, or `uniform_int_distribution<int>(l,r)(rng)`.

[⬆️ Back to contents](#contents)

---

<a id="s2"></a>
## 📚 2. Standard headers (C++14, with C++17/20 additions marked)

> [!NOTE]
> `<bits/stdc++.h>` is GCC-only; learning the individual headers below makes code portable.

`<bits/stdc++.h>` is a GCC-only convenience that includes everything below. It is non-standard (fails on MSVC/clang+libc++ on macOS) and slows compilation. Fine for contests; for portable code, or to learn what lives where, include the specific headers.

### Minimal include sets (portable)
```cpp
// typical solution
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <cmath>
#include <climits>
// add as needed: <unordered_map> <unordered_set> <stack> <deque> <bitset> <tuple> <functional> <iomanip> <sstream> <cstring> <random> <chrono> <cassert>
```

| Header | Provides | Use |
|---|---|---|
| `<iostream>` | `cin`, `cout`, `cerr`, `endl` | all input/output; pair with `ios::sync_with_stdio(false); cin.tie(nullptr);` for speed |
| `<vector>` | `vector` | dynamic array, adjacency lists, DP tables |
| `<algorithm>` | `sort`, `reverse`, `unique`, `lower_bound`, `next_permutation`, `min`, `max`, `swap`, `max_element` | sorting, binary search, permutations, min/max |
| `<numeric>` | `accumulate`, `iota`, `partial_sum`, `gcd`/`lcm` (C++17) | sums, prefix sums, fill 0..n-1 (e.g. DSU/index arrays) |
| `<string>` | `string`, `to_string`, `stoi`, `stoll`, `getline` | text processing, number/string conversion |
| `<map>` | `map`, `multimap` | ordered key to value, frequency counts in sorted order |
| `<set>` | `set`, `multiset` | ordered unique keys, `lower_bound`, floor/ceil queries |
| `<queue>` | `queue`, `priority_queue` | BFS; Dijkstra and top-k via heap |
| `<cmath>` | `sqrt`, `pow`, `abs(double)`, `floor`, `ceil`, `log2`, `fabs` | geometry, logs, floating-point math |
| `<climits>` | `INT_MAX`, `INT_MIN`, `LLONG_MAX` | "infinity" sentinels and overflow checks |
| `<unordered_map>` / `<unordered_set>` | hash map / hash set | O(1) average lookup; two-sum, visited sets |
| `<stack>` / `<deque>` | `stack`, `deque` | DFS, monotonic stack; monotonic deque, sliding window |
| `<bitset>` / `<tuple>` | `bitset<N>`, `tuple`, `tie` | bit-parallel DP/sieve; multi-value returns and comparisons |
| `<functional>` | `function`, `greater<>` | recursive lambdas, min-heap (`greater<>`) |
| `<iomanip>` / `<sstream>` | `setprecision`, `setw`; `stringstream` | formatted output; split/parse strings |
| `<cstring>` | `memset`, `memcpy`, `strlen` | fast array fill (0 / -1 only) |
| `<random>` / `<chrono>` | `mt19937`, `chrono::steady_clock` | random tests, anti-hash seeds; timing |
| `<cassert>` | `assert` | debug invariants |

### Containers
| Header | Provides | Use |
|---|---|---|
| `<vector>` | `vector` | dynamic array; default choice |
| `<array>` | `array<T,N>` | fixed-size, stack-allocated, has `.size()`/iterators |
| `<deque>` | `deque` | O(1) push/pop at both ends, random access; monotonic deque |
| `<list>` | `list` | doubly linked; O(1) insert/erase/`splice` at iterator |
| `<forward_list>` | `forward_list` | singly linked list |
| `<stack>` | `stack` | LIFO adapter (`push/pop/top`) |
| `<queue>` | `queue`, `priority_queue` | FIFO (BFS); heap (Dijkstra, top-K) |
| `<set>` | `set`, `multiset` | ordered unique/duplicate keys, `lower_bound`, predecessor/successor |
| `<map>` | `map`, `multimap` | ordered key->value, frequency counts, sorted iteration |
| `<unordered_set>` | `unordered_set`, `unordered_multiset` | O(1) avg membership |
| `<unordered_map>` | `unordered_map`, `unordered_multimap` | O(1) avg key->value (custom hash for pairs / anti-hack) |
| `<bitset>` | `bitset<N>` | compile-time-size bit array, `count()`, shifts, knapsack speedup |
| `<string>` | `string`, `stoi/stoll/stod`, `to_string`, `getline` | text; see section [9](#s9) |
| `<string_view>` | 🟪 **[C++17]** `string_view` | non-owning read-only view; zero-copy substrings |
| `<span>` | 🟧 **[C++20]** `span` | non-owning view over contiguous memory |

### Algorithms, numerics, functional
| Header | Provides | Use |
|---|---|---|
| `<algorithm>` | `sort`, `stable_sort`, `nth_element`, `partial_sort`, `lower_bound/upper_bound`, `binary_search`, `reverse`, `rotate`, `unique`, `remove/remove_if`, `next_permutation`, `min/max/minmax`, `min_element/max_element`, `count/count_if`, `find/find_if`, `any_of/all_of/none_of`, `fill`, `copy`, `transform`, `swap`, `merge`, `set_union/intersection/difference`, `includes`, `shuffle`, `equal`, `mismatch`, `is_sorted`; 🟪 **[C++17]** `clamp`, `sample` | nearly every problem |
| `<numeric>` | `accumulate`, `iota`, `partial_sum`, `adjacent_difference`, `inner_product`; 🟪 **[C++17]** `gcd`, `lcm`, `reduce`, `inclusive_scan`, `exclusive_scan`, `transform_reduce` | sums, prefix sums, ranges 0..n-1, gcd/lcm |
| `<functional>` | `function`, `greater<>`, `less<>`, `plus<>`, `multiplies<>`, `bind`, `ref`, `hash<T>` | recursive lambdas, min-heap comparator, custom ordering |
| `<iterator>` | `begin/end`, `next/prev`, `advance`, `distance`, `back_inserter`, `reverse_iterator` | iterator arithmetic on sets/lists (`next(s.begin(), k)`) |
| `<utility>` | `pair`, `make_pair`, `move`, `swap`, `forward`, `exchange` (C++14) | pairs, move semantics |
| `<tuple>` | `tuple`, `make_tuple`, `get`, `tie`, `tuple_cat` | multi-key compare via `tie`, multiple returns |
| `<optional>` | 🟪 **[C++17]** `optional<T>`, `nullopt` | "maybe value" returns |
| `<variant>` | 🟪 **[C++17]** `variant`, `visit` | tagged union; rare in DSA |
| `<any>` | 🟪 **[C++17]** `any` | type-erased value; rare |
| `<memory>` | `unique_ptr`, `shared_ptr`, `make_unique`, `make_shared`, `allocator` | tree/linked-list nodes without manual `delete` (usually raw pointers in practice) |
| `<initializer_list>` | `initializer_list` | enables `min({a,b,c})`, `vector<int>{1,2,3}` |
| `<type_traits>` | `is_same`, `enable_if`, `is_integral`, `make_unsigned` | templates / static_assert |
| `<execution>` | 🟪 **[C++17]** parallel policies for algorithms | rarely useful in contests |
| `<limits>` | `numeric_limits<T>::max()/min()/lowest()/epsilon()/infinity()` | sentinel INF values |

### Math & number
| Header | Provides | Use |
|---|---|---|
| `<cmath>` | `sqrt`, `pow`, `abs` (float), `floor`, `ceil`, `round`, `log`, `log2`, `log10`, `exp`, `sin/cos/tan`, `atan2`, `hypot`, `fabs`, `fmod`, `isnan`, `cbrt`, `sqrtl` | geometry, logs; floating point only |
| `<cstdlib>` | `abs(int/ll)`, `rand`, `srand`, `exit`, `atoi`, `qsort`, `malloc/free` | integer abs; prefer `<random>` over `rand` |
| `<cstdint>` | `int8_t...int64_t`, `uint32_t`, `uint64_t`, `INT32_MAX`, `INT64_MAX` | exact-width types |
| `<climits>` | `INT_MAX`, `INT_MIN`, `LLONG_MAX`, `LLONG_MIN`, `UINT_MAX`, `CHAR_BIT` | sentinels |
| `<cfloat>` | `DBL_MAX`, `DBL_EPSILON` | float limits |
| `<complex>` | `complex<double>` | FFT, 2D geometry |
| `<random>` | `mt19937`, `mt19937_64`, `uniform_int_distribution`, `uniform_real_distribution`, `random_device` | random tests, hashing seeds, randomized algorithms |
| `<ratio>` | `ratio<N,D>` | compile-time fractions (mostly for `chrono`) |
| `<numbers>` | (C++20) `std::numbers::pi` | 🟧 **[C++20]**; C++14: `acos(-1.0L)` |
| `<valarray>` | `valarray` | vector math; rare |

### I/O and formatting
| Header | Provides | Use |
|---|---|---|
| `<iostream>` | `cin`, `cout`, `cerr`, `clog` | main I/O; add `ios::sync_with_stdio(false)` |
| `<iomanip>` | `setprecision`, `fixed`, `setw`, `setfill`, `left/right`, `boolalpha`, `hex` (via `<ios>`), `ws` | output formatting, e.g. `cout << fixed << setprecision(6)` |
| `<sstream>` | `stringstream`, `istringstream`, `ostringstream` | split by whitespace/delimiter, int<->string building |
| `<charconv>` | 🟪 **[C++17]** `from_chars`, `to_chars` | very fast, allocation-free number parse/format |
| `<filesystem>` | 🟪 **[C++17]** paths, directory iteration | not for contests |
| `<fstream>` | `ifstream`, `ofstream` | file I/O; `freopen` alternative |
| `<cstdio>` | `scanf`, `printf`, `getchar`, `putchar`, `fread`, `fgets`, `freopen` | C-style fast I/O |
| `<ios>` / `<istream>` / `<ostream>` | base stream classes, `ios_base::sync_with_stdio` | pulled in by `<iostream>` |

### C-style / strings / chars
| Header | Provides | Use |
|---|---|---|
| `<cstring>` | `memset`, `memcpy`, `memcmp`, `strlen`, `strcmp`, `strcpy`, `strchr` | zero/`-1` fill arrays, C strings |
| `<cctype>` | `isdigit`, `isalpha`, `isalnum`, `isupper`, `islower`, `isspace`, `toupper`, `tolower` | char classification |
| `<cstdlib>` | see above | |
| `<cassert>` | `assert(cond)` | sanity checks in debug (disabled by `-DNDEBUG`) |
| `<cstddef>` | `size_t`, `ptrdiff_t`, `nullptr_t` | type definitions |
| `<ctime>` | `clock()`, `time()`, `CLOCKS_PER_SEC` | measure runtime: `clock() / (double)CLOCKS_PER_SEC` |
| `<chrono>` | `steady_clock`, `high_resolution_clock`, `duration`, `milliseconds` | precise timing; random seed from time |
| `<climits>` | see above | |
| `<cwchar>` / `<cuchar>` / `<locale>` / `<codecvt>` | wide chars, Unicode, locales | rarely needed |
| `<regex>` | `regex`, `regex_match`, `regex_search` | pattern matching; very slow, avoid in contests |
| `<bit>` | (C++20) `popcount`, `countl_zero`, `bit_width` | 🟧 **[C++20]**; C++14: `__builtin_popcount/clz/ctz` |

### Error handling / misc
| Header | Provides | Use |
|---|---|---|
| `<stdexcept>` | `runtime_error`, `invalid_argument`, `out_of_range`, `logic_error` | throwing/catching errors |
| `<exception>` | `exception`, `terminate` | base exception class |
| `<new>` | `bad_alloc`, placement new | memory failures |
| `<memory_resource>` | 🟪 **[C++17]** `pmr` allocators | custom allocators / performance tuning |
| `<typeinfo>` / `<typeindex>` | `typeid`, `type_index` | RTTI; rare |
| `<system_error>` | `error_code` | system-level errors |
| `<thread>`, `<mutex>`, `<atomic>`, `<condition_variable>`, `<future>` | concurrency | not used in DSA; contests are single-threaded |
| `<cinttypes>` | `PRId64`, `strtoimax` | printf macros for fixed-width ints |
| `<cstdarg>` | `va_list`, variadic C functions | rarely |
| `<csetjmp>`, `<csignal>`, `<cfenv>`, `<cerrno>`, `<clocale>` | low-level C facilities | rare |

### GCC-specific (not standard)
| Header | Provides | Use |
|---|---|---|
| `<bits/stdc++.h>` | all standard headers | contest shortcut |
| `<ext/pb_ds/assoc_container.hpp>` + `<ext/pb_ds/tree_policy.hpp>` | `tree<...>` ordered set with `find_by_order`/`order_of_key`; `gp_hash_table`/`cc_hash_table` | order statistics; faster hash tables (`using namespace __gnu_pbds;`) |
| `<ext/rope>` | `__gnu_cxx::rope` | O(log n) string split/concat; rare |
| `<ext/numeric>` | `power()` (SGI extension) | fast exponentiation; rare |
| Pragmas | `#pragma GCC optimize("O3,unroll-loops")`, `#pragma GCC target("avx2")` | micro-optimization; use sparingly |

### Which header? Quick lookup
- `sort`, `reverse`, `unique`, `lower_bound`, `next_permutation`, `max_element`, `min`, `max`, `swap`? `<algorithm>` (`swap`/`pair` also from `<utility>`)
- `accumulate`, `iota`, `partial_sum`? `<numeric>` (also `gcd`/`lcm` in C++17); `__gcd`? GCC
- `greater<>`, `function`? `<functional>`
- `INT_MAX`? `<climits>`; `numeric_limits`? `<limits>`
- `memset`? `<cstring>`; `isdigit`? `<cctype>`; `abs(int)`? `<cstdlib>`; `abs(double)`/`sqrt`? `<cmath>`
- `setprecision`, `setw`? `<iomanip>`; `stringstream`? `<sstream>`
- `mt19937`? `<random>`; timing? `<chrono>`
- `uint64_t`? `<cstdint>`; `pair`? `<utility>`; `tie`? `<tuple>`
- `ordered_set`? `<ext/pb_ds/...>` (GCC)

[⬆️ Back to contents](#contents)

---

<a id="s3"></a>
## ⚠️ 3. Language essentials & pitfalls

> [!WARNING]
> Top 5 causes of wrong answers: ① int overflow ② unsigned `size()-1` ③ off-by-one ④ globals not reset between tests ⑤ non-strict comparator.

| Topic | Note |
|---|---|
| `int` | 32-bit, ~±2.1e9 (2^31-1 = 2147483647). Overflow is UB for signed. |
| `long long` | ~±9.2e18. Cast **before** multiplying: `1LL * a * b`. |
| `size_t` / `.size()` | Unsigned! `v.size() - 1` on empty = huge. Use `int(v.size()) - 1`. `for (int i=0;i<n-1;i++)` with unsigned `n` breaks. |
| Integer division | Truncates toward 0. `-7/2 == -3`, `-7%2 == -1`. Positive mod: `((a%m)+m)%m`. |
| Float compare | Use `fabs(a-b) < 1e-9`. Never `==`. `long double` for extra precision. |
| `char` arithmetic | `c - '0'`, `c - 'a'`; `char` may be signed -> cast to `unsigned char` when indexing. |
| `a/b` as double | `(double)a / b` or `1.0 * a / b`. |
| Ceil div | `(a + b - 1) / b` for positives. |
| `abs` | `abs(long long)` ok in `<cstdlib>`; `abs(INT_MIN)` is UB. |
| `pow` | Returns double; loses precision. Write your own integer pow. |
| `sqrt` | Use `(ll)sqrtl(x)` then adjust `while(r*r>x) r--; while((r+1)*(r+1)<=x) r++;` |
| Uninitialized | Locals are garbage; globals are zero. Large arrays -> global/static (stack ~8MB, ~2e6 ints). |
| `vector<bool>` | Bit-packed proxy; can't take reference/address. Use `vector<char>` or `bitset` if needed. |
| Iterator invalidation | `push_back` may invalidate vector iterators/refs. `erase` invalidates from the point. For `map/set`, erase invalidates only the erased one: `it = m.erase(it)`. |
| Modifying while iterating | Don't `erase` inside range-for. |
| `auto` copies | `for (auto x : v)` copies! Use `const auto&` / `auto&` to avoid copy or to mutate. |
| `map[x]` | Inserts default if missing. Use `m.count(x)`, `m.find(x)`, `m.contains(x)` (C++20 only, not available). |
| Unpacking pairs/tuples | C++14: `p.first/p.second`, `tie(a, b) = p;`, `for (auto& kv : m) kv.first/kv.second`. 🟪 **[C++17]** structured bindings: `auto [a, b] = p;` `for (auto& [k, v] : m)`. |
| Pass big objects | By `const&`. Vectors by value in recursion = TLE. See *Argument passing & return values* below. |
| Recursion depth | ~1e5 deep may segfault (stack). Convert to iterative or increase stack. |
| Shadowing/`int i` reuse | Nested loops with same var; -Wshadow catches. |
| Comparator | Must be strict weak ordering (`<` not `<=`) else UB/crash in sort. |
| Ternary | `cond ? a : b` types must match (int vs ll promotes). |
| Short-circuit | `if (i < n && a[i] == x)` order matters. |
| `scanf` %lld | `%lld` ll, `%llu` ull, `%lf` double, `%Lf` long double. |
| printf double | `cout << fixed << setprecision(10) << x;` |


### Argument passing & return values

| Parameter | Copies? | Can change caller's object? | Use for |
|---|---|---|---|
| `T x` (by value) | yes | no | small types (`int`, `ll`, `double`, `char`, `pair<int,int>`, iterators, `string_view`), or when you need your own copy anyway |
| `T& x` | no | **yes** | output / in-out parameters: fill a vector, mark `vis`, modify a grid, backtracking state |
| `const T& x` | no | no | large read-only inputs: `vector`, `string`, `map`, structs. Also binds to temporaries |
| `T* x` | no (copies the pointer) | yes, through `*x` | "optional" argument (`nullptr` = none), C APIs, linked structures |
| `T*& x` | no | yes, **and can repoint the caller's pointer** | tree/list code that may replace the node itself (BST insert/delete) |
| `T&& x` | no | yes (it's a temporary you may steal from) | move constructors, sink overloads; rare in DSA code |

- Rule of thumb: built-in types and anything ≤ 16 bytes by value; containers and strings by `const&` (read) or `&` (write). Never pass a `vector`/`string` by value by accident.
- Reference vs pointer: a reference must be bound at creation, can't be null, can't be re-pointed, and needs no `*`/`->`. A pointer can be null, re-pointed and used for arithmetic.

#### Bug 1: forgetting `&` (the change happens to a copy)
```cpp
void incCopy(vector<int> v) { for (int& x : v) x++; }   // ❌ modifies a local copy; caller sees nothing
void incRef(vector<int>& v) { for (int& x : v) x++; }   // ✅
void markCopy(vector<bool> vis, int u) { vis[u] = true; }   // ❌ classic DFS bug: every call gets a fresh vis
```
- Symptom: the function "works" when you print inside it, but the caller's data is unchanged; or DFS revisits nodes forever / TLE.

#### Bug 2: pointer passed by value cannot replace the caller's pointer
```cpp
struct TNode { int val; TNode *l = nullptr, *r = nullptr; TNode(int v) : val(v) {} };
void insertLost(TNode* root, int x) {                 // ❌ root is a copy: new node is never linked
    if (!root) { root = new TNode(x); return; }
    insertLost(x < root->val ? root->l : root->r, x);
}
void insertRef(TNode*& root, int x) {                 // ✅ reference to the caller's pointer
    if (!root) { root = new TNode(x); return; }
    insertRef(x < root->val ? root->l : root->r, x);
}
TNode* insertRet(TNode* root, int x) {                // ✅ or return the (possibly new) root
    if (!root) return new TNode(x);
    if (x < root->val) root->l = insertRet(root->l, x);
    else root->r = insertRet(root->r, x);
    return root;
}
// usage: TNode* root = nullptr;  insertRef(root, 5);  root = insertRet(root, 3);
```
- `insertLost` also leaks the node it allocated: nothing points to it after the call.
- Same for linked lists: deleting the head or reversing needs `ListNode*&` or "return the new head" (LeetCode style).

#### Arrays: C arrays decay, vectors don't
```cpp
void sumC(int a[], int n);              // same as int* a: the size is lost, pass n (sizeof(a) is 8 here)
void grid2D(int g[][100], int rows);    // 2D C array: every dimension except the first is required
template <size_t N>
int sizeOf(int (&)[N]) { return N; }    // reference to array keeps N
void gridV(vector<vector<int>>& g);     // vectors know their sizes: g.size(), g[0].size()
```
- A C array parameter is **always** effectively by reference (it's a pointer): writes are visible to the caller. A `vector` parameter without `&` is a full copy.
- `vector<vector<int>> g(n, vector<int>(m))` passed by value copies `n·m` ints per call.

#### Recursion and backtracking: share state by reference, undo after the call
```cpp
void subsets(int i, const vector<int>& a, vector<int>& cur, vector<vector<int>>& out) {
    if (i == (int)a.size()) { out.push_back(cur); return; }   // copy only at the leaf
    subsets(i + 1, a, cur, out);       // skip a[i]
    cur.push_back(a[i]);
    subsets(i + 1, a, cur, out);       // take a[i]
    cur.pop_back();                    // undo: cur is shared by every call
}
```
- Passing `a` (input) by value here would copy the whole array at every call: O(n) extra per call, often TLE/MLE at depth 1e5.
- Passing `cur` by value is a valid alternative (no undo needed) but costs a copy per call; fine only for small depth/size.
- Globals (or lambda `[&]` captures) avoid long parameter lists in contests; remember to reset them per test case.

#### Returning several values
```cpp
pair<int, int> minMax(const vector<int>& a) {               // a must be non-empty
    auto p = minmax_element(a.begin(), a.end());
    return {*p.first, *p.second};
}
struct Stats { ll sum; int neg; };                          // named fields read better than .first/.second
Stats stats(const vector<int>& a) {
    Stats s{0, 0};
    for (int x : a) s.sum += x, s.neg += x < 0;
    return s;
}
void minMaxOut(const vector<int>& a, int& lo, int& hi) {   // output parameters (C style)
    lo = *min_element(a.begin(), a.end());
    hi = *max_element(a.begin(), a.end());
}
// C++14: int lo, hi; tie(lo, hi) = minMax(a);    C++17: auto [lo, hi] = minMax(a);
```
- Returning a `vector`/`string`/struct **by value is cheap**: the copy is elided or the result is moved. Don't switch to output parameters for speed.

#### Dangling references and pointers (compiles, then crashes or prints garbage)
```cpp
const string& name() { string s = "abc"; return s; }   // ❌ reference to a destroyed local
int* addr() { int x = 5; return &x; }                  // ❌ pointer to a destroyed local
auto counter() { int c = 0; return [&] { return ++c; }; }   // ❌ lambda outlives the c it captured by reference
// ✅ return by value: string name(); int value();   ✅ capture by value: [c]() mutable { return ++c; }
```
- `-Wall` warns for the first two ("returning reference/address of local variable"); the lambda case is **not** warned. `-fsanitize=address` catches all three at runtime (older GCC may need `ASAN_OPTIONS=detect_stack_use_after_return=1` for the lambda).
- Also dangling: a reference/pointer/iterator into a `vector` after `push_back` (reallocation), and a 🟪 **[C++17]** `string_view` of a temporary (`string_view sv = s + "x";`).
- Binding a temporary to a **local** `const T&` extends its lifetime (`const string& r = s + "x";` is fine); this does not apply to references returned from functions.

#### Hidden copies
- `for (auto row : grid)` copies every row; use `const auto& row` (read) or `auto& row` (write). `for (auto [k, v] : m)` copies each pair: `const auto& [k, v]`.
- `vector<int> nb = adj[u];` copies the adjacency list; use `const auto& nb = adj[u];` or loop directly.
- `sort`/`priority_queue` copy their comparator: a lambda that captures a big container **by value** (`[=]` or `[v]`) copies it each time. Capture by reference: `[&]`.
- `auto x = f();` is fine (no extra copy); `auto x = bigVector[i];` copies the element if it's a container.

#### Move semantics (what `std::move` actually does)
```cpp
vector<vector<int>> out;
vector<int> cur = {1, 2, 3};
out.push_back(cur);              // copies cur
out.push_back(move(cur));        // steals cur's buffer: O(1); cur is now valid but unspecified (empty in practice)
cur.clear();                     // reset before reusing a moved-from object
struct Person {
    string name;
    explicit Person(string n) : name(move(n)) {}   // "sink" parameter: take by value, then move in
};
```
- `std::move` doesn't move anything; it's a cast to `T&&` that lets a move constructor/assignment run. Moving a `const` object silently copies.
- `return local;` is already moved or elided; writing `return move(local);` can block elision (`-Wpessimizing-move` warns).
- Only move when the source is no longer needed: last use of a temporary result, `push_back` of a buffer you're about to reset, swapping large containers (`swap(a, b)` is O(1) for vectors).

#### Passing functions
```cpp
template <class F>
ll applyT(F f, int x) { return f(x); }                       // fastest: the call is inlined
ll applyFn(const function<ll(int)>& f, int x) { return f(x); }   // any callable, but type-erased: indirect call
ll sq(int x) { return 1LL * x * x; }
// applyT(sq, 3); applyT([&](int v) { return v + base; }, 3); applyFn(sq, 3);   ll (*fp)(int) = sq;
```
- `std::function` costs an indirect call (and maybe an allocation) per call: avoid it for hot recursion (memoized DFS over 1e6 states). Use a plain function with globals, a template parameter, or the C++14 `auto&& self` lambda trick from [section 6](#s6).

#### Other details
- **Default arguments** go at the end and only in one declaration: `void dfs(int u, int p = -1);`.
- **Overloads:** `f(const T&)` accepts everything (lvalues and temporaries); add `f(T&&)` only when you want a cheaper path for temporaries.
- **`auto&&` in range-for** binds to anything, including `vector<bool>` proxies: `for (auto&& b : vb) b = true;` works where `auto&` doesn't compile.
- **Evaluation order of arguments is unspecified:** `f(i++, i++)` or `f(read(), read())` may run in either order. Read into variables first.
- **`const` member functions** are required to call methods through a `const T&` (see [Structs, classes & OOP](#s4)).

#### Interview Q&A
| Question | Short answer |
|---|---|
| Pass by value vs by reference? | By value copies (caller unaffected); by reference aliases the caller's object (no copy, changes are visible). |
| Why `const T&` instead of `T`? | Avoids the copy for large objects and still accepts temporaries; `const` documents and enforces read-only. |
| Reference vs pointer? | Reference: non-null, bound once, no special syntax. Pointer: nullable, re-pointable, supports arithmetic. |
| What is `T*&`? | A reference to a pointer: the function can make the caller's pointer point elsewhere (e.g. create the root). |
| Is C++ "pass by reference" for arrays? | A C array argument decays to a pointer (so element writes are visible, size is lost). `std::vector` is copied unless you take it by `&`. |
| What does `std::move` do? | Casts to an rvalue reference so the move overload is chosen; the actual move happens in the move constructor/assignment. |
| Is returning a vector expensive? | No: copy elision / move makes it O(1) in practice. |

### C++14 vs C++17/20 quick diff
Use when the judge allows `-std=c++17`/`c++20`.

| Feature | C++14 | Newer |
|---|---|---|
| Unpack pair/tuple | `tie`, `.first/.second` | 🟪 **[C++17]** `auto [a,b] = p;` |
| If with init | declare var before `if` | 🟪 **[C++17]** `if (auto it = m.find(k); it != m.end())` |
| Compile-time branch | SFINAE / overloads | 🟪 **[C++17]** `if constexpr` |
| Class template deduction | `pair<int,int> p(1,2)` | 🟪 **[C++17]** `pair p(1,2); vector v{1,2,3};` |
| gcd / lcm | `__gcd`, hand-written | 🟪 **[C++17]** `std::gcd`, `std::lcm` |
| Nested namespaces, `inline` variables, fold expressions | – | 🟪 **[C++17]** `template<class... T> auto sum(T... x){ return (x + ...); }` |
| `[[nodiscard]]`, `[[maybe_unused]]` | – | 🟪 **[C++17]** attributes |
| `contains`, `erase_if`, `std::popcount`, `std::bit_width`, `ranges`, `span`, `<=>` | `count`, erase-remove, `__builtin_popcount`, `__lg` | 🟧 **[C++20]** |
| Digit separators, binary literals | `0b1010`, `1'000'000` | already C++14 |

[⬆️ Back to contents](#contents)

---

<a id="s4"></a>
## 🏗️ 4. Structs, classes & OOP essentials

> [!NOTE]
> In C++ `struct` and `class` are the **same thing** except for the default access level. Everything below applies to both.

### struct vs class
| | `struct` | `class` |
|---|---|---|
| Default member access | `public` | `private` |
| Default inheritance | `public` | `private` |
| Constructors, methods, templates, virtual, operators | yes | yes (identical) |
| Convention | plain data bundles (`Edge`, `Node`, `Point`, comparators) | types with invariants / encapsulation (`Stack`, `BankAccount`) |

- ✅ In contests and DSA code use `struct` (no `public:` boilerplate). In interviews, say the only language difference is default access, then state the convention.
- `struct S : Base` inherits publicly; `class C : Base` inherits **privately** unless you write `public Base`.
- `union` is different: all members share one memory slot (size = largest member); rare in DSA.

```cpp
struct P { int x, y; };          // x, y public
class  Q { int x, y; };          // x, y private: Q q; q.x = 1; // compile error
class  R { public: int x, y; };  // same as struct P
```

### Writing a struct: constructor, members, methods
```cpp
struct Edge {
    int to;
    ll w = 0;                          // default member initializer (C++11)
    Edge() = default;                  // keep it when you add other ctors, so `Edge e;` and vector<Edge>(n) work
    Edge(int to, ll w) : to(to), w(w) {}   // member-init list: runs before the body; required for const/reference members
    bool operator<(const Edge& o) const { return w < o.w; }
};
vector<Edge> g;
g.emplace_back(3, 10);                 // constructs in place
g.push_back({3, 10});                  // brace-init works if a matching ctor (or aggregate) exists
```
- **Aggregate** (no user ctor, no private members, no virtual): `Edge e{3, 10};` and `P p{1, 2};` work. 🟧 **[C++20]** designated init: `P p{.x = 1, .y = 2};`.
- Members are initialized in **declaration order**, not in the order of the init list (`-Wreorder` warns).
- Uninitialized `int` members hold garbage: `P p;` leaves `x, y` indeterminate; `P p{};` zeroes them.
- Mark methods that don't modify the object `const`: `int len() const { ... }`. A `const S&` can only call `const` methods.
- `this` is a pointer to the current object; `*this` is the object. Return `*this` (by reference) for chaining.
- `static` data member: one copy shared by all objects (define it outside the class, or `inline static` 🟪 **[C++17]**). `static` method: no `this`, called as `S::f()`.

### Access control and friend
| Specifier | Visible to |
|---|---|
| `public` | everyone |
| `protected` | the class and its derived classes |
| `private` | the class (and its `friend`s) only |

- `friend` grants a function/class access to privates; typically for `operator<<` / `operator>>`:
```cpp
struct V { int a, b; };
ostream& operator<<(ostream& os, const V& v) { return os << v.a << ' ' << v.b; }   // free function, public members: no friend needed
```

### Operators for STL (the part DSA actually needs)
| Used by | Needs | Notes |
|---|---|---|
| `sort`, `set`, `map`, `priority_queue` (max-heap) | `operator<` (strict weak ordering) | member `const`, or free function |
| `==`, `unique`, `find` | `operator==` | |
| `unordered_map` / `unordered_set` | `operator==` **and** a hash | specialize `std::hash<T>` (below) |
| `priority_queue` as min-heap | flip the comparison, or `greater<T>` + `operator>` | |

```cpp
struct State {
    int d, u;
    bool operator<(const State& o) const { return d > o.d; }   // reversed: priority_queue<State> becomes a min-heap on d
};
// equality + hash so State can be an unordered_set key
struct H { size_t operator()(const P& p) const { return (size_t)p.x * 1000003u ^ (size_t)p.y; } };
bool operator==(const P& a, const P& b) { return a.x == b.x && a.y == b.y; }
unordered_set<P, H> seen;
// or: namespace std { template<> struct hash<P> { size_t operator()(const P& p) const { return hash<ll>()((ll)p.x << 32 ^ p.y); } }; }
```
- 🟧 **[C++20]** `auto operator<=>(const P&) const = default;` generates all six comparisons (and `==` with `bool operator==(const P&) const = default;`).
- Comparison must be a **strict weak order**: never use `<=`; `set` treats `!(a<b) && !(b<a)` as equal and silently drops the second.
- Cheapest correct ordering for several keys: `return tie(a.x, a.y) < tie(o.x, o.y);` (see [Lambdas, comparators, hashing](#s6)).

### struct vs pair vs tuple
| | `pair`/`tuple` | custom `struct` |
|---|---|---|
| Boilerplate | none, has ordering + `==` for free | you write it |
| Readability | `.first`, `get<2>(t)` | named fields (`e.to`, `e.w`) |
| Hashing | not for `unordered_*` | custom hash |
| Pick | quick throwaway (heap of `{dist, node}`) | 3+ fields, reused across code, needs methods |

🟪 **[C++17]** structured bindings work on both: `auto [to, w] = e;` (struct needs all members public).

### Copy, move, destructor: rule of 0 / 3 / 5
- A class that owns a resource (`new[]`, file, lock) needs a **destructor**, **copy constructor** and **copy assignment** (rule of 3). Add **move constructor** and **move assignment** for the rule of 5.
- ✅ **Rule of 0**: hold members that already manage themselves (`vector`, `string`, `unique_ptr`) and write none of the five. This is what you want in almost all DSA code.
- The compiler-generated copy is **shallow**: for raw pointers it copies the pointer, so two objects free the same memory (double free).
```cpp
struct Buf {
    int* a; int n;
    Buf(int n) : a(new int[n]()), n(n) {}
    ~Buf() { delete[] a; }
    Buf(const Buf& o) : a(new int[o.n]), n(o.n) { copy(o.a, o.a + n, a); }          // deep copy
    Buf& operator=(Buf o) { swap(a, o.a); swap(n, o.n); return *this; }           // copy-and-swap: handles self-assignment
    Buf(Buf&& o) noexcept : a(o.a), n(o.n) { o.a = nullptr; o.n = 0; }             // steal
};
```
- Pass big objects as `const T&`; return by value (compilers elide the copy / move). Pass `T` by value only when you need your own copy anyway. Full rules: [argument passing](#s3).
- `= default` asks for the compiler version, `= delete` forbids it (`Buf(const Buf&) = delete;` makes the type non-copyable).

### Inheritance and polymorphism
```cpp
struct Shape {
    virtual double area() const = 0;      // pure virtual => Shape is abstract (cannot be instantiated)
    virtual ~Shape() = default;           // MUST be virtual if you delete through a base pointer
};
struct Circle : Shape {
    double r;
    explicit Circle(double r) : r(r) {}
    double area() const override { return 3.141592653589793 * r * r; }   // `override` catches signature typos
};
vector<unique_ptr<Shape>> v;  v.push_back(make_unique<Circle>(2));       // polymorphism needs pointers/references
for (auto& s : v) cout << s->area();
```
- **Virtual dispatch**: the call resolves at runtime via the object's vtable pointer; costs a pointer per object + an indirect call. Non-virtual calls are resolved at compile time.
- **Slicing**: `Shape s = circle;` copies only the base part. Hold `Shape*`, `Shape&` or smart pointers.
- Constructors run base → derived; destructors derived → base. Never call virtual functions from a constructor/destructor (derived part does not exist yet / is gone).
- `final` stops further overriding or inheriting. `explicit` on a one-argument constructor stops accidental implicit conversions.
- **Overload** = same name, different parameters (compile time). **Override** = same signature in a derived class, base marked `virtual` (runtime).
- Prefer composition ("has-a") over inheritance ("is-a") unless you truly need runtime polymorphism; STL containers are not designed to be inherited from (no virtual destructor).

### Templates with structs
```cpp
template <class T>
struct Seg {
    int n; vector<T> t;
    Seg(int n) : n(n), t(2 * n) {}
    T query(int l, int r);                  // methods can be defined inside or outside the struct
};
Seg<ll> st(100);
```
Keep `Seg<ll>` style generic structs for segment trees, BIT, DSU, matrices (`Mat<mod>`), modular ints (`struct Mint { int v; Mint operator+(Mint o) const {...} }`).

### Memory layout (struct padding)
```cpp
struct A { char c; int x; char d; };   // sizeof 12: padding after c and d
struct B { int x; char c; char d; };   // sizeof 8: order members largest first
```
- Each member is aligned to its own size; the struct is padded to a multiple of its largest alignment. Reorder members to save memory (matters for 1e6+ node arrays).
- `sizeof(empty struct)` is 1. `alignof(T)` gives the alignment.
- Arrays of structs (**AoS**) are fine for DSA; hot loops that touch one field of many objects are faster with separate arrays (**SoA**).

### Common DSA structs
```cpp
struct Node { int val; Node *l = nullptr, *r = nullptr; Node(int v) : val(v) {} };   // tree / list node
struct Edge { int u, v; ll w; bool operator<(const Edge& o) const { return w < o.w; } };   // Kruskal: sort(edges)
struct Cmp { bool operator()(const P& a, const P& b) const { return a.y < b.y; } };      // functor: set<P, Cmp>, priority_queue<P, vector<P>, Cmp>
```
- Pointers into `vector<Node>` are invalidated when the vector reallocates: use indices (`int l, r`) for arena-style trees, and `reserve` if you must keep pointers.
- For linked structures with `new`, free with `delete` (or just use an index arena; contest code rarely frees).

### Pitfalls
- ⚠️ `operator<` without `const` fails inside `set`/`sort` ("passing const as this discards qualifiers").
- ⚠️ Non-strict or inconsistent comparator (`<=`, or depending on a mutable field): `set` loses items, `sort` can crash.
- ⚠️ Forgetting the trailing `;` after `struct { ... };`.
- ⚠️ `Edge e();` declares a function (most vexing parse); write `Edge e;` or `Edge e{};`.
- ⚠️ Adding any constructor removes the implicit default constructor (and aggregate init): add `S() = default;`.
- ⚠️ Deleting a derived object through a base pointer without a `virtual` destructor is undefined behaviour.
- ⚠️ Modifying a key's fields inside a `set` element is not allowed (elements are `const`); erase and re-insert, or use a `map`.
- ⚠️ Large objects on the stack (`Seg` with `int t[1<<22]`) overflow it: make them global or use `vector`.

### Interview Q&A
| Question | Short answer |
|---|---|
| struct vs class? | Only default access (public vs private) and default inheritance. Convention: struct for data, class for encapsulation. |
| struct vs union? | struct members have separate storage; union members overlap (size = largest). |
| Class vs object? | Class = type/blueprint; object = an instance in memory. |
| Constructor vs destructor? | Constructor initializes on creation (can be overloaded); destructor cleans up on destruction (one per class, no params). |
| Shallow vs deep copy? | Shallow copies pointer values (shared target); deep copies the pointed-to data. |
| What is the rule of 3/5/0? | If you manage a resource, define destructor + copy (+ move); otherwise define none. |
| Virtual function? | Resolved at runtime through the vtable; enables polymorphism. Base needs a virtual destructor. |
| Abstract class? | Has at least one pure virtual function (`= 0`); cannot be instantiated. |
| Overload vs override? | Overload: same name, different params, compile time. Override: redefine a base virtual with the same signature, runtime. |
| `explicit`? | Blocks implicit conversion through a single-argument constructor. |
| Why pass `const T&`? | Avoids copying and promises not to modify. |
| Stack vs heap objects? | `S s;` lives on the stack (auto-destroyed at scope end); `new S` on the heap (needs `delete` or a smart pointer). |

[⬆️ Back to contents](#contents)

---

<a id="s5"></a>
## ⏱️ 5. Complexity cheat table

> [!TIP]
> 💡 ~1e8 simple ops/second. Read constraints first; they pick the algorithm.

Rough budget: **~1e8 simple ops/second** (1–2 s limit → ~2e8). Estimate from constraints `n`:

| n | Acceptable complexity |
|---|---|
| ≤ 10 | O(n!) , O(n·n!) |
| ≤ 20 | O(2^n · n) , meet-in-middle borderline |
| ≤ 25–30 | O(2^n) / 3^n (n ≤ 15) |
| ≤ 100 | O(n⁴) |
| ≤ 500 | O(n³) |
| ≤ 5,000 | O(n²) |
| ≤ 1e5 | O(n log n) / O(n √n) borderline |
| ≤ 1e6 | O(n) / O(n log n) light |
| ≤ 1e7–1e8 | O(n) tight |
| ≤ 1e18 | O(log n) / O(√n) factoring ≈ 1e9 → Pollard rho |

Memory: `int` = 4B, `ll` = 8B; 256MB ≈ 6e7 ints. `bool`/`char` arrays of 1e8 = 100MB.

Common recurrences: `T(n)=2T(n/2)+n → n log n`; `T(n)=T(n-1)+1 → n`; harmonic sum → n log n; sum of divisors count over 1..n ≈ n ln n; subsets of subsets = 3^n; permutations n! (10! = 3.6e6, 12! = 4.8e8, 13! > 6e9).

Powers: 2^10=1024≈1e3, 2^20≈1e6, 2^30≈1e9, 2^31−1=2147483647, 2^63−1≈9.22e18, 2^64≈1.8e19.

[⬆️ Back to contents](#contents)

---

<a id="s6"></a>
## 🧩 6. Lambdas, comparators, hashing

```cpp
auto cmp = [&](const A& x, const A& y) { return x.k < y.k; };  // strict!
sort(all(v), cmp);
set<int, decltype(cmp)> s(cmp);  // lambda comparator in set
function<void(int, int)> dfs = [&](int u, int p) {
    for (int w : g[u])
        if (w != p) dfs(w, u);
};
// std::function adds overhead; for hot recursion use a plain function with globals.
// C++14 generic lambda: auto f = [](auto&& self, int n) -> int { ... self(self, n-1) };
```
- Capture: `[&]` by ref, `[=]` copy, `[x, &y]` mixed.
- Sort by multiple keys: `return tie(a.x, a.y) < tie(b.x, b.y);` or `make_pair(-a.x, a.y)` for desc/asc mix.
- Operator overload in struct: `bool operator<(const S& o) const { ... }`.

[⬆️ Back to contents](#contents)

---

<a id="s7"></a>
## 📦 7. STL containers

> [!TIP]
> 💡 Default to `vector`; use `map/set` when you need order or `lower_bound`; `unordered_*` only with a safe hash; `priority_queue` for repeated min/max.

### Complexity table
| Container | Access | Insert | Erase | Find | Notes |
|---|---|---|---|---|---|
| `vector` | O(1) | O(1)* back, O(n) mid | O(1) back, O(n) mid | O(n) | contiguous, cache-friendly; `reserve(n)` |
| `deque` | O(1) | O(1) both ends | O(1) both ends | O(n) | random access, not contiguous |
| `list` | – | O(1) at iterator | O(1) | O(n) | rarely needed; `splice` is O(1) |
| `array<T,N>` | O(1) | – | – | – | fixed size, stack |
| `set/map/multiset/multimap` | – | O(log n) | O(log n) | O(log n) | ordered (red-black tree) |
| `unordered_set/map` | – | O(1) avg, O(n) worst | same | same | hackable -> custom hash |
| `priority_queue` | top O(1) | O(log n) | pop O(log n) | – | max-heap default |
| `stack/queue` | – | O(1) | O(1) | – | adapters on deque |
| `bitset<N>` | O(1) | – | – | – | `count()`, `any()`, `_Find_first()`, `<<`, `>>`, `&`, `\|`; N/64 speedup |

### vector
```cpp
vector<int> v(n, 0);  // n zeros
vector<vector<int>> g(n, vector<int>(m, -1));
v.assign(n, x);
v.resize(n);
v.clear();
v.shrink_to_fit();
v.push_back(x);
v.emplace_back(a, b);
v.pop_back();
v.back();
v.front();
v.insert(v.begin() + i, x);
v.erase(v.begin() + i);
v.erase(v.begin() + l, v.begin() + r);
vector<int> w(v.begin() + l, v.begin() + r);  // slice [l, r)
```
- Remove-erase idiom: `v.erase(remove(all(v), x), v.end());` / `remove_if`. C++20: `erase(v, x)`, `erase_if(v, pred)`.
- Unique: `sort(all(v)); v.erase(unique(all(v)), v.end());`
- Swap-with-last-then-pop for O(1) unordered delete.
- 2D vector init gotcha: `vector<vector<int>> g(n)` leaves rows empty.
- Lexicographic compare of vectors/pairs/tuples works with `<`, `==`.

### pair / tuple
```cpp
pair<int, int> p = {1, 2};
p.first;
p.second;  // compares first then second
auto t = make_tuple(1, 'a', 2.0);
get<0>(t);
int a;
char b;
double c;
tie(a, b, c) = t;             // [C++17] auto [a,b,c] = t;
tie(a, b) = make_pair(b, a);  // swap/unpack; also tie for multi-key compare
```

### set / map (ordered)
```cpp
set<int> s;
s.insert(x);
s.erase(x);
s.count(x);
s.find(x);
s.lower_bound(x);  // first >= x      s.upper_bound(x);  // first > x
*s.begin();
*s.rbegin();
s.size();
// predecessor (largest < x):
auto it = s.lower_bound(x);
if (it != s.begin()) --it;  // check validity
// k-th element needs pbds (see below), std::next(s.begin(), k) is O(k)
map<string, int> m;
m[k]++;
for (auto& kv : m) { /* kv.first, kv.second */
}                    // [C++17] for (auto& [k,v] : m)
m.lower_bound(k);
m.erase(it);  // [C++17] m.extract(k) moves a node out; C++14: find + erase
multiset : erase(value) removes ALL;
erase(s.find(value)) removes ONE.
```
- Use member `s.lower_bound(x)`, NOT `lower_bound(all(s), x)` (that is O(n) on sets).
- Custom order: `set<int, greater<int>>`, or struct with `operator<`.
- Frequency map -> `map<int,int>`; "multiset replacement" with counts.

### unordered_map / set
- `reserve(n)`, `max_load_factor(0.25)` speeds up.
- Anti-hash tests exist on Codeforces; use a custom hash:
```cpp
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t R = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + R);
    }
};
unordered_map<ll, int, custom_hash> mp;
// pair key: hash by combining, e.g. key = (ll)a * M + b, or encode into ll.
```
- No default hash for `pair`/`vector`/`tuple`: encode as `ll`, use `map`, or write a hash.

### priority_queue
```cpp
priority_queue<int> mx;                             // max-heap
priority_queue<int, vector<int>, greater<int>> mn;  // min-heap
priority_queue<pii, vector<pii>, greater<pii>> pq;  // Dijkstra: {dist, node}
// trick: push negatives for min-heap with default.
// no decrease-key: push duplicates, skip stale on pop (if d > dist[u]) continue;
// custom: struct cmp { bool operator()(A a, A b) { return a.x > b.x; } };  // '>' => min-heap (reversed!)
```
- `top()` on empty = UB. Build from vector in O(n): `priority_queue<int> pq(all(v));`.
- Lazy deletion with 2 heaps to support erase.

### stack / queue / deque
- Monotonic stack/deque tricks in section [14](#s14).
- `queue::front/back/pop/push`; `deque` supports `push_front/pop_front` and `[]`.

### bitset
```cpp
bitset<1000> b;
b[i] = 1;
b.set(i);
b.reset();
b.flip();
b.count();
b.any();
b.none();
b._Find_first();
b._Find_next(i);  // GCC only
b.to_string();
b.to_ullong();
bitset<8>("1010");
// subset-sum knapsack: b |= b << w;   // O(n*S/64)
```

### GCC pb_ds (order statistics) — GCC only
```cpp
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <class T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
ordered_set<int> os;
os.insert(x);
os.find_by_order(k);  // iterator to k-th (0-indexed) smallest
os.order_of_key(x);   // number of elements strictly < x
// for multiset behavior: insert pair<value, unique_id>.
```

[⬆️ Back to contents](#contents)

---

<a id="s8"></a>
## 🔧 8. STL algorithms & `<numeric>`

> [!TIP]
> 💡 Sort → `unique` → `lower_bound` is the coordinate-compression combo. `accumulate(..., 0LL)` avoids int overflow.

All on ranges `[first, last)`. Prefer these over hand loops.

```cpp
// ---- sorting & ordering ----
sort(all(v));                                      // ascending
sort(all(v), greater<>());                         // descending
sort(v.rbegin(), v.rend());                        // descending (reverse iterators)
stable_sort(all(v), cmp);                          // keeps the order of equal elements
partial_sort(v.begin(), v.begin() + k, v.end());   // smallest k elements, sorted
nth_element(v.begin(), v.begin() + k, v.end());    // O(n): k-th element in place
reverse(all(v));
rotate(v.begin(), v.begin() + k, v.end());         // left-rotate by k
shuffle(all(v), rng);                              // NOT random_shuffle (removed)
is_sorted(all(v));

// ---- permutations ----
sort(all(v));                                      // start from the smallest one
do {
    // use v
} while (next_permutation(all(v)));                // false after the last; prev_permutation goes back

// ---- searching (range must be sorted for the first four) ----
lower_bound(all(v), x);                            // first element >= x
upper_bound(all(v), x);                            // first element >  x
binary_search(all(v), x);                          // bool
equal_range(all(v), x);                            // pair [lb, ub)
// count of x = upper_bound - lower_bound
find(all(v), x);                                   // linear; end() if absent
find_if(all(v), pred);
count(all(v), x);
count_if(all(v), pred);
any_of(all(v), pred);                              // also all_of, none_of

// ---- min / max ----
min_element(all(v));                               // iterators: dereference with *
max_element(all(v));
minmax_element(all(v));
min({a, b, c});                                    // initializer-list form
max({a, b, c});
min(max(x, lo), hi);                               // [C++17] clamp(x, lo, hi)

// ---- numeric ----
accumulate(all(v), 0LL);                           // 0LL avoids int overflow!
accumulate(all(v), 1LL, multiplies<>());
iota(all(v), 0);                                   // 0, 1, 2, ...
partial_sum(all(v), p.begin());                    // prefix sums
adjacent_difference(all(v), d.begin());
__gcd(a, b);                                       // GCC, <algorithm>; lcm = a / __gcd(a, b) * b
// [C++17] gcd(a, b), lcm(a, b), inclusive_scan, exclusive_scan, reduce live in <numeric>

// ---- modifying & sets ----
unique(all(v));                                    // after sort; returns the new end
fill(all(v), x);
transform(all(v), out.begin(), f);
merge(all(a), all(b), out.begin());                // two sorted ranges into out
set_union(all(a), all(b), back_inserter(c));       // also set_intersection, set_difference
includes(all(a), all(b));                          // is sorted b a subset of sorted a?
swap(a, b);
fill(&a[0][0], &a[0][0] + N * M, x);               // fill a whole 2D C-array
```

- `lower_bound` on custom: `lower_bound(all(v), x, comp)` where comp(elem, x).
- Sort indices: `iota(all(id),0); sort(all(id), [&](int i,int j){return a[i]<a[j];});`
- Coordinate compression:
```cpp
vector<int> c = a;
sort(all(c));
c.erase(unique(all(c)), c.end());
for (auto& x : a)
    x = lower_bound(all(c), x) - c.begin();
```
- GCC builtins: `__builtin_popcount(x)` (`ll` version for 64-bit), `__builtin_clz`, `__builtin_ctz` (undefined for 0!), `__lg(x)` = floor(log2). C++20: `std::popcount`, `std::bit_width`, `std::countr_zero`.

[⬆️ Back to contents](#contents)

---

<a id="s9"></a>
## 🔤 9. Strings

```cpp
string s;
s.size();
s[i];
s.substr(pos, len);
s += c;
s.push_back(c);
s.pop_back();
s.find("ab");  // npos if not found (compare with string::npos)
s.find(c, from);
s.rfind(c);
s.find_first_of("aeiou");
s.find_first_not_of(" ");
s.insert(pos, t);
s.erase(pos, len);
s.replace(pos, len, t);
stoi(s), stoll(s), stod(s);
to_string(x);
reverse(all(s));
sort(all(s));
s == t;
s < t(lexicographic);
s.compare(t);
isdigit(c), isalpha(c), isalnum(c), isupper(c), islower(c), toupper(c), tolower(c);
```
- Split: `stringstream ss(line); string w; while (ss >> w) ...` ; custom delimiter: `while (getline(ss, w, ','))`.
- Frequency: `int cnt[26]{}; cnt[c-'a']++;`
- Anagram check: compare count arrays or sorted strings.
- Palindrome: `equal(s.begin(), s.begin()+n/2, s.rbegin())`, or `s == string(s.rbegin(), s.rend())`.
- C++14: pass `const string&` plus indices to avoid copies (see [argument passing](#s3)). 🟪 **[C++17]** `string_view` gives zero-copy substrings (`sv.substr()` is O(1)); don't keep a view of a temporary.
- Avoid `s = s + c` in loops (O(n) each); use `+=`.
- Char->string: `string(1, c)`. `to_string(int)`. Int digits sum: loop `% 10`.
- Base conversion: `bitset<32>(x).to_string()`, or loop. Parse binary: `stoi(s, nullptr, 2)`.

### String patterns (Striver step 5)
- **Parentheses depth**: one counter, `+1` on `(`, `-1` on `)`. Remove outermost: keep `(` when depth > 0 *before* incrementing, keep `)` when depth > 0 *after* decrementing. Max nesting depth = max counter.
- **Reverse words**: read with `stringstream`, push into a vector, join reversed (handles extra spaces). In-place: reverse whole string, then each word.
- **Largest odd number in a digit string**: cut after the last odd digit (`s.substr(0, i+1)`); strip leading zeros if needed.
- **Isomorphic**: two maps (`s→t` and `t→s`), or store the last index of each char in both strings and compare.
- **Rotation check**: `a.size()==b.size() && (a+a).find(b) != npos`.
- **Sort by frequency**: count, then sort chars by `(-cnt, c)`, or bucket by count.
- **Roman → int**: add each value; subtract it if the next value is larger. **Int → Roman**: greedy over `{1000,900,500,400,100,90,50,40,10,9,5,4,1}`.
- **atoi**: skip spaces → optional sign → digits; clamp before overflow (`if (r > (INT_MAX - d) / 10)`), stop at the first non-digit.
- **Longest palindromic substring**: expand around each of the `2n−1` centres, O(n²), O(1) space (Manacher for O(n)).
- **Sum of beauty of all substrings** (max freq − min freq): fix start, extend end with a count array, O(26·n²).
- **Substrings with exactly K distinct** = atMost(K) − atMost(K−1) (see [sliding window](#s14)).

[⬆️ Back to contents](#contents)

---

<a id="s10"></a>
## 🔢 10. Bit manipulation

| Op | Expression |
|---|---|
| Test bit i | `(x >> i) & 1` |
| Set / clear / toggle | `x \| (1<<i)` / `x & ~(1<<i)` / `x ^ (1<<i)` |
| Lowest set bit | `x & -x` |
| Clear lowest set bit | `x & (x-1)` |
| Power of 2 check | `x > 0 && (x & (x-1)) == 0` |
| All ones of k bits | `(1<<k) - 1` (use `1LL<<k` for k≥31) |
| Isolate lowest zero | `~x & (x+1)` |
| Swap | `a^=b; b^=a; a^=b;` |
| Negate | `-x == ~x + 1` |
| Divide/multiply by 2^k | `x >> k`, `x << k` |
| Count bits | `__builtin_popcount(x)` |
| Floor log2 | `31 - __builtin_clz(x)` or `__lg(x)` |
| Parity | `__builtin_parity(x)` |

Tricks:
- XOR: `a^a=0`, `a^0=a`; find the single non-duplicated element = XOR of all. Two uniques: XOR all -> split by lowest set bit of result.
- XOR prefix: `xor(l..r) = P[r] ^ P[l-1]`. XOR 0..n in O(1): `n%4==0: n; 1: 1; 2: n+1; 3: 0`.
- `a + b = (a ^ b) + 2*(a & b)`;  `a | b = (a ^ b) + (a & b)`.
- Max XOR pair: binary trie.
- Iterate submasks of `m`: `for (int s = m; s; s = (s-1) & m) { ... }` (include 0 separately). Total over all masks = 3^n.
- Iterate set bits: `for (int x = m; x; x &= x-1) { int b = __builtin_ctz(x); }`.
- Gray code: `i ^ (i >> 1)`.
- Odd check `x & 1`. Set the rightmost unset bit `x | (x+1)`; unset the rightmost set bit `x & (x-1)`.
- Bits to flip to turn A into B: `popcount(A ^ B)`.
- Power set with bits: mask `m` from 0 to `2^n − 1`, take `a[i]` when `(m >> i) & 1`.
- Divide without `*`, `/`: subtract the biggest `divisor << k` that fits, add `1 << k` to the quotient; work in `long long` with absolute values; `INT_MIN / -1` overflows → return `INT_MAX`.
- Single number where others appear 3 times: count each bit `mod 3` (or the `ones/twos` trick).
- Shift by ≥ width is UB; `1 << 31` on int is UB → `1u << 31` or `1LL << k`.
- Right shift of negative signed is implementation-defined (arithmetic on GCC).

[⬆️ Back to contents](#contents)

---

<a id="s11"></a>
## ➗ 11. Math basics (modular arithmetic, GCD)

### Modular arithmetic
```cpp
const int MOD = 1e9 + 7;  // or 998244353 (NTT-friendly)
ll add(ll a, ll b) { return (a + b) % MOD; }
ll mul(ll a, ll b) { return a * b % MOD; }  // a,b < MOD < 2^31 so fits in ll
ll sub(ll a, ll b) { return ((a - b) % MOD + MOD) % MOD; }

ll power(ll b, ll e, ll m = MOD) {  // binary exponentiation O(log e)
    ll r = 1;
    b %= m;
    for (; e; e >>= 1, b = b * b % m)
        if (e & 1) r = r * b % m;
    return r;
}
ll inv(ll a) { return power(a, MOD - 2); }  // Fermat, MOD prime only
// Division under mod: a / b = a * inv(b). For non-prime mod use extended gcd.
```
- For mod up to 1e18 multiplication use `__int128`: `(__int128)a * b % m`.

### GCD / extended Euclid
```cpp
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    ll g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}  // ax+by=g
// lcm = a / gcd(a,b) * b   (divide first to avoid overflow)
```
- `gcd(a,b)` properties: `gcd(a,b)=gcd(a,b-a)`; gcd of array = fold; gcd over range → sparse table / segment tree.
- Linear Diophantine `ax+by=c` solvable iff `gcd | c`.

### Misc math
- Integer sqrt/cbrt via binary search. Avoid float.
- Floor division with negatives: `(a - ((a % b) + b) % b) / b`.
- Digits count: `to_string(x).size()`; `floor(log10(x))+1` risky.
- Parity/invariants tricks, modular patterns (cycle in `a^k mod 10`).
- Sum of floor(n/i): O(√n) blocks, `for (l=1;l<=n;l=r+1){ q=n/l; r=n/q; ... }`.

[⬆️ Back to contents](#contents)

---

<a id="s12"></a>
## 🧮 12. Number theory & combinatorics

### Sieve (primes up to N, O(N log log N))
```cpp
vector<bool> comp(N + 1);
vector<int> primes;
for (int i = 2; i <= N; i++) {
    if (!comp[i]) {
        primes.push_back(i);
        for (ll j = (ll)i * i; j <= N; j += i)
            comp[j] = true;
    }
}
// smallest prime factor sieve -> factorize in O(log n):
vector<int> spf(N + 1);
iota(all(spf), 0);
for (int i = 2; (ll)i * i <= N; i++)
    if (spf[i] == i)
        for (int j = i * i; j <= N; j += i)
            if (spf[j] == j) spf[j] = i;
```
- Trial division prime check/factorization: loop `i*i <= n`, O(√n).
- Miller–Rabin for big (up to 1e18) with bases {2,3,5,7,11,13,17,19,23,29,31,37}; Pollard rho for factoring.
- Number of divisors / sum of divisors from factorization: `∏(e+1)`, `∏(p^(e+1)-1)/(p-1)`.
- Divisors enumeration O(√n). Harmonic: sum n/i ≈ n ln n (iterate multiples).
- Euler φ: `φ(n) = n ∏(1-1/p)`; sieve-able. `a^φ(m) ≡ 1 mod m` if gcd=1. Reduce huge exponent: `a^e mod m = a^(e mod φ(m)) ` (gcd=1) or Fermat `e mod (p-1)`.
- Sum formulas: `Σi = n(n+1)/2`, `Σi² = n(n+1)(2n+1)/6`, `Σi³ = (n(n+1)/2)²`, geometric `(r^n-1)/(r-1)`.

### Combinatorics
```cpp
const int N = 1e6 + 5;
ll fact[N], invfact[N];
void precompute() {                                  // factorials and inverse factorials up to N-1
    fact[0] = 1;
    for (int i = 1; i < N; i++) fact[i] = fact[i - 1] * i % MOD;
    invfact[N - 1] = power(fact[N - 1], MOD - 2);
    for (int i = N - 1; i >= 1; i--) invfact[i - 1] = invfact[i] * i % MOD;
}
ll C(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % MOD * invfact[n - r] % MOD;
}
```
- Pascal: `C[n][k] = C[n-1][k-1] + C[n-1][k]` (small n, any mod).
- Stars & bars: nonneg solutions of `x1+..+xk=n` → `C(n+k-1, k-1)`.
- Catalan: `C(2n,n)/(n+1)`: balanced parens, BSTs, Dyck paths. 1,1,2,5,14,42,132.
- Derangements: `D(n) = (n-1)(D(n-1)+D(n-2))`. Inclusion–exclusion for "at least/none" counts.
- Lucas theorem for `C(n,r) mod small prime p`.
- Pigeonhole, double counting, linearity of expectation (expected value = Σ prob of each event).
- Matrix exponentiation: linear recurrence in O(k³ log n); Fibonacci fast doubling.
- Modular: `(a*b)%m`, `(a-b+m)%m`; never apply `%` to a division.
- Chinese Remainder Theorem for coprime moduli (via exgcd).
- Fibonacci: `fib(n) < 2^63` for n ≤ 92. Pisano periods.

[⬆️ Back to contents](#contents)

---

<a id="s13"></a>
## 🌿 13. Recursion, backtracking, bitmask

### Backtracking template
```cpp
void bt(int idx, vector<int>& cur) {
    if (idx == n) {
        ans.push_back(cur);
        return;
    }  // or at every node for subsets
    for (int c : choices) {
        if (!valid(c)) continue;
        cur.push_back(c);
        mark(c);
        bt(idx + 1, cur);
        unmark(c);
        cur.pop_back();  // undo
    }
}
```
- Subsets: include/exclude, or iterate from `start`. Permutations: `used[]` or swap-based. Combinations: `start` index, prune with remaining count.
- Duplicates: sort, `if (i > start && a[i]==a[i-1]) continue;` (for permutations: `... && !used[i-1]`).
- Combination sum: reuse allowed → recurse with `i`, else `i+1`.
- N-Queens: columns, `r+c`, `r-c+n` diagonal arrays (or bitmasks). Sudoku: row/col/box masks.
- Word search: DFS with visited mark/unmark. Palindrome partitioning: DP table for isPal.
- Prune early: sort + break; bound by remaining sum.
- Generate parentheses: track `open`, `close` counts.
- Rat in a maze: 4-direction DFS with visited, try moves in `D, L, R, U` order for lexicographic output. M-coloring: try each colour for node `i`, check neighbours. Expression add operators: carry `(value, last operand)` so `*` can undo the last term: `val - last + last*cur`.
- Recursion on a stack (no extra container): **reverse** = pop all, then `insertAtBottom` each on the way back; **sort** = pop top, sort the rest, insert top into its sorted place.
- Pow(x, n): `x^n = (x^(n/2))²` (× x if odd); for negative `n` use `1/x` and `long long` for `-n` (`INT_MIN`). Count good numbers: `5^ceil(n/2) · 4^floor(n/2) mod M`.

### Subsequence patterns (pick / not pick)
Every subsequence problem is the same tree: at index `i` either take `a[i]` or skip it. Only the return type changes.
```cpp
// 1) print / collect all:  void f(i, cur) { if (i==n) { use(cur); return; } take: push, f(i+1), pop; skip: f(i+1) }
// 2) does one exist / print only one: return bool, stop as soon as a branch returns true
bool any(int i, int sum) {
    if (sum == k) return true;        // with positives only; else check at i == n
    if (i == n || sum > k) return false;
    return any(i + 1, sum + a[i]) || any(i + 1, sum);
}
// 3) count them: return int, add the branches
int cnt(int i, int sum) {
    if (i == n) return sum == k;
    return cnt(i + 1, sum + a[i]) + cnt(i + 1, sum);
}
```
- O(2ⁿ); with overlapping `(i, sum)` states memoize it → it becomes DP ([DP on subsequences](#s16)).
- Combination sum (reuse allowed): on take stay at `i`; Combination sum II / Subset sum II (no duplicate sets): sort, loop `j` from `i`, skip `j > i && a[j]==a[j-1]`. Combination sum III: pick from 1..9 with size `k`.
- Subset sums I: collect all `2ⁿ` sums, sort. Letter combinations: one digit per level, loop over its letters.
- Binary strings with no two consecutive 1s: place `1` only if the previous char is `0`.

### Bitmask enumeration
`for (int m = 0; m < (1<<n); m++)` subsets; n ≤ ~20 (2^20) or ~25 in simple loops.

[⬆️ Back to contents](#contents)

---

<a id="s14"></a>
## 🎯 14. Core techniques

### 13.1 Prefix sums / difference arrays
```cpp
vector<ll> P(n + 1);
for (int i = 0; i < n; i++) P[i + 1] = P[i] + a[i];   // sum[l,r] = P[r+1] - P[l]
// 2D: P[i][j] = a + P[i-1][j] + P[i][j-1] - P[i-1][j-1]; query by inclusion-exclusion
// difference array: add x on [l,r]: d[l] += x; d[r+1] -= x; then prefix-sum d.
```
- Subarray sum == k: prefix + hash map of seen prefix counts. Divisible by k: count of equal `prefix % k`.
- Replace 0 by -1 for "equal 0s and 1s" problems.
- **Longest** subarray with sum K: map stores the *first* index of each prefix (`if (!mp.count(p)) mp[p] = i`), `len = i - mp[p-K]`. Only positives → sliding window works, O(1) space. Largest 0-sum subarray = same with K = 0.
- Count subarrays with XOR K: prefix XOR `x`, add `cnt[x ^ K]`, start with `cnt[0] = 1`.

### 13.2 Two pointers
- Sorted array pair sum: `l=0, r=n-1`, move by comparison. 3Sum: fix i, then two-pointer; skip duplicates.
- Merge sorted arrays, remove duplicates in-place, partition (Dutch flag: low/mid/high).
- Container with most water, trapping rain water (two pointers or prefix max/suffix max).
- Fast/slow pointers: linked list cycle (Floyd), middle node, cycle start (reset one to head).

### 13.3 Sliding window
- Fixed size k: add right, remove left.
- Variable (longest/shortest satisfying condition): expand `r`, `while (invalid) shrink l`.
- "At most K distinct" ; exactly K = atMost(K) − atMost(K−1). Count of subarrays = `r - l + 1` per right end.
- Window min/max → monotonic deque.
- Keep the condition in counters: longest substring without repeats (last index of each char, `l = max(l, last[c]+1)`); max consecutive ones III (zeros in window ≤ k); fruit into baskets (≤ 2 distinct).
- Longest repeating character replacement: valid while `len − maxFreq ≤ k`; `maxFreq` never needs to decrease.
- Count subarrays with sum == goal (0/1 values, or odd count for "nice subarrays" after `a[i] %= 2`) = atMost(goal) − atMost(goal−1).
- Substrings containing all of `a,b,c`: keep last seen index of each; at `r` add `1 + min(last)`.
- Max points from cards (take k from the ends): start with the first k, then swap one left card for one right card k times (= total − min window of length n−k).
- Minimum window substring: `need[]` counts + `missing` counter, shrink while `missing == 0`. Minimum window **subsequence**: scan forward until all of `t` is matched, then scan back from there to find the best start; continue from start+1.

### 13.4 Monotonic stack / deque
```cpp
// Next greater element: iterate, pop while stack top < cur; popped elements' answer = cur.
for (int i = 0; i < n; i++) {
    while (!st.empty() && a[st.back()] < a[i]) {
        nge[st.back()] = i;
        st.pop_back();
    }
    st.push_back(i);
}
// Largest rectangle in histogram: for each bar find prev/next smaller → width*height.
// Sliding window max: deque of indices with decreasing values; pop front if out of window.
```
- Circular arrays: iterate `2n` indices with `i % n`.
- Sum of subarray minimums: contribution technique (left span × right span, handle ties with < vs <=).
- Four arrays cover most problems: next/previous greater/smaller (NGE, PGE, NSE, PSE). Pop condition decides which: `<` pop → greater, `>` pop → smaller.
- Sum of subarray ranges = Σ subarray maxima − Σ subarray minima (both by contribution).
- Stock span = `i − PGE(i)`. Asteroid collision: stack of survivors; a left-mover pops smaller right-movers, equal → both die.
- Maximal rectangle of 1s: build heights row by row, run largest-rectangle-in-histogram on each row, O(n·m).
- Remove K digits: pop while top > cur and k > 0; pop remaining k from the end; strip leading zeros; `"0"` if empty.
- Celebrity: two pointers `i=0, j=n−1`; if `i` knows `j` then `i` is not the celebrity, else `j` isn't; verify the survivor.

### 13.5 Binary search
```cpp
// first index in [lo,hi] where pred true (pred monotone F..F T..T)
int lo = 0, hi = n;  // hi = n means "none"
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (pred(mid))
        hi = mid;
    else
        lo = mid + 1;
}
// answer lo
```
- **Binary search on answer**: minimize max / maximize min (split array, aggregate cows, capacity to ship, Koko bananas). Define `feasible(x)`, check monotonicity, search range bounds.
- Real values: loop 100 iterations rather than epsilon.
- Rotated sorted array: compare mid with `a[hi]`/`a[lo]`; with duplicates shrink on equality.
- Peak element: compare `a[mid]` with `a[mid+1]`.
- lower_bound = first `≥ x` (= search insert position, ceil); upper_bound = first `> x`; floor = `ub − 1`; first/last occurrence = `lb`, `ub − 1`; count = `ub − lb`.
- Min in rotated sorted array: one half is sorted; take its smallest, search the other half. Times rotated = index of the minimum.
- Single element in a sorted array of pairs: before it, pairs start at even indices. Compare `mid` with `mid^1`: equal → go right.
- Kth missing positive: missing before index `i` = `a[i] − (i+1)`; answer = `lo + k` after the search.
- Median / kth element of two sorted arrays: binary search how many elements to take from the **smaller** array; valid when `l1 ≤ r2 && l2 ≤ r1`. O(log min(n, m)).
- Minimise max distance to gas station: binary search on the real answer `d`; stations needed for a gap `g` = `ceil(g/d) − 1`.
- 2D: sorted as one list → index `mid` is `(mid/m, mid%m)`. Rows and columns sorted → staircase from top-right, O(n+m). Row with most 1s → `lower_bound` per row, or staircase. Peak II → binary search on columns, take the max of the mid column. Matrix median → binary search on the value, count `≤ x` with `upper_bound` per row.
- Overflow: `mid = lo + (hi-lo)/2`.
- Ternary search for unimodal functions (or binary search on `f(m+1)-f(m)`).
- Binary lifting: `for (int k = LOG; k >= 0; k--) if (cond(pos + (1<<k))) pos += 1<<k;`.
- Parallel binary search, meet in the middle (split n≤40 into halves: 2^20 each + two pointers/binary search).

### 13.6 Sorting-based ideas
- Sort then greedy; intervals: sort by start (merge) or by end (max non-overlapping, activity selection).
- Sweep line: events (x, +1/-1), sort, process; meeting rooms = max concurrent.
- Counting sort when values small; radix sort.
- Custom sort on strings: `a+b > b+a` (largest number).
- Merge sort for counting inversions, `count smaller after self`.
- Quickselect for k-th element: avg O(n).

### 13.7 Hashing patterns
- Two-sum: map of value→index. Group anagrams: sorted string key. Longest consecutive sequence: set, start from sequence heads only.
- Frequency compare for window anagram/permutation.
- Count pairs with condition: iterate and query map of earlier elements.
- Rolling hash: `h = (h*B + c) % M` with two mods or 2^61−1 for safety; random base.

### 13.8 Intervals / misc patterns
- Merge intervals: sort by start, extend end.
- Insert interval, minimum arrows (sort by end), meeting rooms II (heap of end times).
- Kadane (max subarray): `cur = max(a[i], cur + a[i]); best = max(best, cur)`. Max product subarray: track min and max.
- Boyer–Moore majority vote: candidate + counter, O(1) space.
- Reservoir sampling; Fisher–Yates shuffle.
- Cyclic sort / index-as-hash for numbers in [1,n]: place value v at index v−1 (find missing/duplicates).
- Floyd cycle for duplicate number in array.
- Matrix traversal: spiral, rotate (transpose + reverse rows), directions `dx[]={1,-1,0,0}, dy[]={0,0,1,-1}`.
- Matrix in-place marker: use first row/col as flags.
- Linked list: dummy head, reverse (prev/cur/next), merge, find middle, remove nth from end (gap of n). More in [13.11](#s14).

### 13.9 Array patterns (Striver step 3)
- Second largest without sorting: track `first`, `second` in one pass (`second` updates only on a strictly smaller value).
- Rotate left by d: `d %= n`, reverse `[0,d)`, reverse `[d,n)`, reverse all. Move zeros: write pointer for non-zeros.
- Union of two sorted arrays: merge, skip a value equal to the last pushed. Missing number: `n(n+1)/2 − sum` or XOR of `1..n` and the array.
- Leaders (greater than everything to the right): scan from the right with a running max.
- Next permutation: from the right find the first `a[i] < a[i+1]`; swap `a[i]` with the rightmost element `> a[i]`; reverse `a[i+1..]`. No such `i` → reverse all. (`next_permutation` does this.)
- Rearrange by sign (equal counts): positives go to even indices, negatives to odd. Unequal counts: fill alternately, then append the rest.
- Stock buy/sell once: keep the min so far, best = `max(a[i] − mn)`. Print max-sum subarray: in Kadane, reset `start` when `cur` restarts.
- Majority > n/3: at most 2 answers; Boyer–Moore with two candidates and two counters, then verify counts.
- 4Sum: fix `i`, `j`, two-pointer the rest, skip duplicates at all four levels; sum in `long long`.
- Repeating and missing in 1..n: `S − Sn = x − y` and `S2 − S2n = x² − y²` → solve (use `long long`), or XOR split by the lowest set bit.
- Merge two sorted arrays without extra space: compare `a` from the end with `b` from the start, swap while `a[i] > b[j]`, then sort both. Or the gap method (shell sort, gap = `ceil(gap/2)`).
- Count inversions / reverse pairs (`a[i] > 2·a[j]`): merge sort; before merging, count with a pointer over the right half (`2LL * a[j]`).
- Pascal's triangle: element `(r, c)` = `C(r, c)`; a row in O(r) with `v = v·(r−c)/(c+1)`.

### 13.10 Stack & queue patterns (Striver step 9)
- Stack with an array: `top` index. Queue with an array: circular, `front`, `rear = (rear+1) % cap`, keep a `size` counter.
- Stack using one queue: push, then rotate the previous `size−1` elements behind it. Queue using two stacks: push into `in`; pop from `out`, refilling `out` from `in` only when it is empty (amortised O(1)).
- Balanced brackets: push openers; on a closer, the top must be its pair; stack empty at the end.
- Min stack in O(1) extra space: store `2x − min` when `x < min` (and update `min`); on popping a value `< min`, restore `min = 2·min − top`. Use `long long`.
- **Infix → postfix**: operand → output; `(` → push; `)` → pop until `(`; operator → pop while top has higher precedence (or equal and left-associative; `^` is right-associative), then push; pop the rest at the end.
- **Infix → prefix**: reverse, swap `(`/`)`, convert to postfix (for equal precedence pop only if right-associative), reverse.
- **Postfix → infix / prefix**: scan left to right, operand → push; operator → pop `b`, then `a`, push `"(a op b)"` (or `op a b`). **Prefix → infix / postfix**: same, scanning right to left and popping `a` first.
- Evaluate postfix: same stack, push numbers, apply operators.

### 13.11 Linked list patterns (Striver step 6)
- Use a **dummy head** whenever the head may change (insert/delete at head, merging, partitioning).
- DLL: insert/delete must fix both `prev` and `next` of the neighbours (check for null ends). Reverse DLL: swap `prev` and `next` in each node; new head is the last node.
- Tortoise-hare: middle (`fast = fast->next->next`); delete middle → start `fast` one step ahead (`head->next->next`) so `slow` stops before it. Loop: meet, then reset one to head for the start; walk the loop once for its length.
- Palindrome: find middle, reverse the second half, compare, reverse it back.
- Odd/even segregation, sort 0/1/2: build separate lists with dummy heads, then link them.
- Intersection of Y lists: `a = a ? a->next : headB` (and the mirror), they meet after ≤ m+n steps.
- Add two numbers: walk both with a carry, dummy result list. Add 1: reverse, add, reverse back (or recursion returning the carry).
- Rotate by k: link tail to head, `k %= n`, cut after node `n−k`.
- Reverse in groups of k: find the k-th node, cut, reverse the group, reconnect to the previous group's tail; a short last group stays as is.
- Sort list: merge sort (middle with slow/fast, split, merge). Flatten (sorted `bottom` lists): merge lists one by one from the right.
- Clone with random pointer: interleave copies (`A A' B B'`), set `copy->random = orig->random->next`, then separate. Or a map `old → new`.
- DLL pair sum (sorted): two pointers from head and tail. Remove duplicates from sorted DLL: skip equal next nodes, fix `prev`.

### 13.12 Pattern printing & basic sorting (Striver steps 1–2)
- Star patterns: outer loop = rows, inner loops = what one row prints (spaces, then stars/numbers/letters). Write the count of each as a formula in the row `i` (e.g. pyramid row `i`: `n−i−1` spaces, `2i+1` stars). Mirrored or diamond shapes = two halves, the second with `i` reversed.
- Selection: put the min of `[i..n)` at `i`. Bubble: adjacent swaps, stop if a pass makes no swap (O(n) on sorted input). Insertion: shift larger elements right, insert the key (fast on nearly sorted input). Recursive versions do one pass, then recurse on `n−1`.
- Merge sort: O(n log n) always, stable, O(n) extra. Quick sort: O(n log n) average, O(n²) worst (random pivot), in-place, not stable. Code in the [advanced notes](notes_advanced.md).

[⬆️ Back to contents](#contents)

---

<a id="s15"></a>
## 🤑 15. Greedy patterns

- **Prove it**: exchange argument / stays-ahead / counter-example hunt on small cases.
- Interval scheduling: sort by end time. Min platforms/rooms: sort starts & ends separately.
- Fractional knapsack (value/weight). Huffman coding (min-heap merge). Job sequencing with deadlines (sort by profit + DSU/heap).
- Jump game: track farthest reachable. Gas station: running total reset. Candy: two passes.
- Min number of coins (canonical coin systems only!). Assign cookies (sort both). Task scheduler (max-frequency formula).
- Merge stones/ropes: min-heap. Reorganize string: heap on frequencies.
- Remove K digits: monotonic stack. Largest number: custom comparator.
- Greedy + binary search on answer; greedy + heap ("regret" greedy: take, and undo worst if invalid).
- Lexicographically smallest string constraints: greedy position-by-position.
- Lemonade change: give back a 10 before two 5s. Valid parenthesis string with `*`: track the range `[lo, hi]` of possible open counts; clamp `lo ≥ 0`, fail if `hi < 0`.
- Jump game II: BFS by levels over ranges, `jumps++` when `i` passes the current range end.
- Non-overlapping intervals (min removals) = n − max non-overlapping (sort by end). Insert interval: copy those ending before, merge overlapping ones, copy the rest.
- SJF scheduling: sort burst times, average wait = Σ prefix sums / n. LRU page faults: simulate with a set + recency order.

[⬆️ Back to contents](#contents)

---

<a id="s16"></a>
## 🧠 16. Dynamic programming

> [!IMPORTANT]
> ✅ State → transition → base case → order → answer → space-optimize. If you cannot say what `dp[i][j]` *means* in one sentence, redefine it.

### Method
(1) define state precisely (what does `dp[i][j]` mean) → (2) transition → (3) base cases → (4) order of computation → (5) answer cell → (6) optimize space.
Top-down (memo, easy) vs bottom-up (fast, space-optimizable). Memo with `-1` sentinel (or a separate `vis`) when 0 is a valid value.

### Patterns
| Pattern | State / transition |
|---|---|
| Fibonacci/climb stairs/house robber | `dp[i] = f(dp[i-1], dp[i-2])`; robber: `max(dp[i-1], dp[i-2]+a[i])` |
| 0/1 knapsack | `for item: for w=W..wt: dp[w]=max(dp[w], dp[w-wt]+val)` (**reverse** loop) |
| Unbounded knapsack / coin change | same but `w` **forward**. Min coins: `dp[x]=min(dp[x-c]+1)`; count ways (combinations): coins outer, amount inner; permutations: amount outer |
| Subset sum / partition equal | boolean knapsack; bitset `b \|= b << x` |
| LIS | O(n²) `dp[i]=1+max dp[j] (a[j]<a[i])`; O(n log n): `tails` array with `lower_bound` (strict) / `upper_bound` (non-decreasing) |
| LCS | `dp[i][j] = a[i]==b[j] ? dp[i-1][j-1]+1 : max(dp[i-1][j], dp[i][j-1])` |
| Edit distance | `min(dp[i-1][j]+1, dp[i][j-1]+1, dp[i-1][j-1]+(a[i]!=b[j]))` |
| Longest palindromic subseq/substr | interval DP `dp[l][r]`; or expand around center O(n²) / Manacher O(n) |
| Grid paths | `dp[i][j] = dp[i-1][j] + dp[i][j-1]`; min path sum; obstacles → 0 |
| Matrix chain / interval DP | `dp[l][r] = min over k (dp[l][k]+dp[k+1][r]+cost)`; iterate by length |
| Burst balloons | interval DP choosing last element k |
| Stock problems | states: hold / not hold / cooldown, with k transactions `dp[k][0/1]` |
| Max subarray (Kadane) | 1D |
| Word break | `dp[i] = OR dp[j] && dict has s[j..i)` |
| Digit DP | `dp[pos][tight][started][state]` count numbers ≤ N with a property |
| Bitmask DP | `dp[mask][last]` — TSP O(2^n·n²); assignment; subset covering |
| DP on trees | `dfs(u, p)` combine children; rerooting for all-roots answers; independent set `dp[u][taken]` |
| DP on DAG/ graph | topological order; longest path |
| Probability/expectation DP | `E[state] = Σ p · (cost + E[next])` |
| SOS DP | `f[mask]` over subsets: for each bit, for each mask with bit: `f[mask] += f[mask^bit]` |
| Counting with mod | apply `%MOD` at every add/mul; subtract with `+MOD` |

### Striver DP patterns (step 16)
Write the recursion `f(i, ...)` first (try all choices at `i`), memoize, then tabulate in reverse order of the recursion, then keep only the previous row.

**1D / grid**
- Frog jump (K): `dp[i] = min over j=1..k (dp[i−j] + |h[i] − h[i−j]|)`. Non-adjacent sum / house robber II (circle): run twice, on `a[0..n−2]` and `a[1..n−1]`.
- Ninja's training: `dp[day][last task]`, pick a task ≠ last. Triangle: start at the bottom row, `dp[i][j] = a[i][j] + min(dp[i+1][j], dp[i+1][j+1])`. Falling path: from three cells above.
- Cherry pickup (two robots): move both together, `dp[row][c1][c2]`, 9 transitions, count a shared cell once.

**Subsequences / knapsack** (`f(i, target)` = take / not take)
- Min subset-sum difference: fill the subset-sum table for `total`, answer = `min |total − 2s|` over reachable `s`.
- Count partitions with difference D = count subsets with sum `(total − D)/2` (0 if negative or odd). Target sum (`+/−`) is the same problem.
- ⚠️ With zeros in the array, the count base case must be `i == 0`: `a[0]==0 && t==0` → 2 ways.
- Rod cutting = unbounded knapsack (take → stay at `i`). Min coins: base `t % a[0] == 0 ? t / a[0] : INF`.

**Strings** (`dp[i][j]` on prefixes, 1-indexed so row/col 0 = empty)
- Print LCS: walk back from `(n, m)`, take the char on match, else move to the larger neighbour.
- Longest common **substring**: on match `dp[i−1][j−1] + 1`, on mismatch 0; answer = max cell.
- Longest palindromic subsequence = LCS(s, reverse(s)). Min insertions to make a palindrome = `n − LPS`.
- Min insert + delete to turn A into B = `n + m − 2·LCS`. Shortest common supersequence length = `n + m − LCS`; print by walking back the LCS table and keeping both chars off the diagonal.
- Distinct subsequences (count `t` in `s`): match → `dp[i−1][j−1] + dp[i−1][j]`, else `dp[i−1][j]`; `dp[i][0] = 1`.
- Wildcard (`?`, `*`): `*` → `dp[i−1][j] || dp[i][j−1]` (match one more char / match empty). `dp[0][j]` true only if `p[0..j)` is all `*`.

**Stocks** (`f(i, canBuy, cap)`)
- Buy: `max(−p[i] + f(i+1, 0), f(i+1, 1))`; sell: `max(p[i] + f(i+1, 1), f(i+1, 0))`.
- I: one pass with the min price. II: unlimited. III / IV: add `cap` (2 / k), decrease on sell. Cooldown: after selling jump to `i+2`. Fee: subtract it on sell.

**LIS family**
- Print LIS: `dp[i]` + `parent[i]`, walk back from the best end. Count LIS: `cnt[i]` adds `cnt[j]` when `dp[j]+1 == dp[i]`, resets on a longer one.
- Largest divisible subset: sort, LIS with `a[i] % a[j] == 0`. Longest string chain: sort by length, "predecessor" = `b` with one char removed equals `a` (two-pointer check).
- Longest bitonic: LIS ending at `i` + LDS starting at `i` − 1.

**Partition (MCM) DP** (`f(i, j)` = best for the segment, try every split `k`)
- MCM: `f(i,j) = min (f(i,k) + f(k+1,j) + a[i−1]·a[k]·a[j])`. Tabulate `i` from right to left, `j` from `i+1` to the right.
- Cut sticks: sort cuts, add `0` and `n` at the ends, `f(i,j) = cuts[j+1] − cuts[i−1] + min (f(i,k−1) + f(k+1,j))`.
- Burst balloons: pick the **last** balloon `k` in `(i,j)`: `a[i−1]·a[k]·a[j+1]`.
- Boolean expression to true: `f(i, j, wantTrue)` combines true/false counts of both sides per operator.
- Palindrome partitioning II (front partition): `dp[i] = min over j (1 + dp[j+1])` where `s[i..j]` is a palindrome; answer `dp[0] − 1`. Partition array for max sum: last block of length ≤ k: `dp[i] = max (max(a[i..j])·len + dp[j+1])`.

**Squares**
- Count square submatrices of 1s: `dp[i][j] = 1 + min(top, left, top-left)`; answer = Σ dp. Largest square = max dp. Max rectangle of 1s → [histogram per row](#s14).

### Optimizations
- Rolling array: `dp[2][n]` using `i&1`, or 1D with correct iteration direction.
- Prefix sums over dp to speed transitions (range-sum transitions).
- Monotonic deque optimization for sliding-window-min transitions.
- Convex Hull Trick / Li Chao tree for `dp[i] = min(dp[j] + b[j]*a[i])`.
- Divide & conquer optimization, Knuth optimization (interval DP O(n²)), when opt monotone.
- Meet in the middle, bitset speedups.
- Reconstruct answer: store parent / choice, or walk backwards re-checking transitions.
- Always check dimensions: if states ≈ 1e7+ consider rolling arrays; memory = states × size (1e8 ints = 400MB).

[⬆️ Back to contents](#contents)

---

<a id="s17"></a>
## 🌳 17. Trees & advanced data structures

### Binary tree
- Traversals: pre/in/post (recursive and iterative with stack), level-order (BFS with level size), Morris for O(1) space.
- Inorder of BST is sorted. Validate BST with (lo, hi) bounds (use `long long`/pointers, not INT limits).
- Height: `1 + max(l, r)`; diameter: `max(l+r)` at each node; LCA: return node if p/q found in both sides.
- Build from preorder+inorder: root = pre[0], split by inorder index (use hashmap).
- Serialize/deserialize with null markers. Path sum (root-to-leaf, any-to-any with global max).
- Balanced check: return -1 as failure sentinel.
- Complete tree array: children `2i+1, 2i+2` (0-indexed) or `2i, 2i+1`.

#### Binary tree patterns (Striver step 13)
- **Views** (BFS with `(node, col)`, `col−1` left, `col+1` right): vertical order → `map<col, map<row, multiset>>`; top view → first node seen per col; bottom view → last node per col. Left/right view → DFS with `level`, record when `level == ans.size()` (visit right first for right view).
- Zigzag: level order, reverse every other level (or fill by index). Boundary: root, left edge (non-leaves), all leaves (DFS), right edge (non-leaves) reversed.
- Max width: index nodes `2i+1`, `2i+2`; subtract the level's first index each level to avoid overflow; width = `last − first + 1`.
- Root-to-node path: DFS, push, return true if found below, else pop. Identical / symmetric: compare `(l, r)` pairs (mirror: `l->left` with `r->right`).
- Nodes at distance K / time to burn the tree: build a parent map with BFS, then BFS from the target in 3 directions (left, right, parent) with a visited set; levels = distance / time.
- Children sum property: going down, if children sum < node, set children to node value; coming up, set node = sum of children.
- Count nodes of a complete tree: if leftmost height == rightmost height → `2^h − 1`, else `1 + count(left) + count(right)`. O(log² n).
- Unique tree needs **inorder** + (preorder or postorder). Inorder + postorder: root = last of postorder; build right subtree before left if consuming from the back.
- Iterative postorder: 2 stacks (root-right-left, then reverse) or 1 stack with a `last visited` pointer. All three in one pass: stack of `(node, state 1/2/3)` → pre at 1, in at 2, post at 3.
- Morris inorder: if `cur->left`, find its rightmost node `pre`; if `pre->right == null` link it to `cur` and go left, else unlink, visit `cur`, go right. Preorder: visit when creating the link.
- Flatten to a list (preorder order): reverse postorder (right, left, node) with a global `prev`: `node->right = prev; node->left = null; prev = node`. Or Morris-style in O(1) space.

#### BST patterns (Striver step 14)
- Search / insert / min / max / floor / ceil: walk one path, O(h). Floor: go right when `val ≤ key` (record it), else left.
- Delete: node with ≤1 child → return the child; else attach the left subtree's rightmost node to the right subtree (or copy the inorder successor and delete it).
- Kth smallest: inorder with a counter (Morris for O(1) space). Kth largest = (n−k+1)th smallest, or reverse inorder.
- LCA in BST: both smaller → left, both larger → right, else current node.
- Build from preorder in O(n): recurse with an upper bound; take `pre[i]` only if `< bound`.
- Inorder successor: walk from the root; if `key < node` record the node and go left, else go right.
- BST iterator: stack of the left chain; `next()` pops and pushes the left chain of the right child. Amortised O(1), O(h) space. Two sum in BST: one `next` and one `before` iterator, two pointers.
- Recover BST (two swapped nodes): inorder, find `first` (prev > cur first time), `middle`, `last` (second time); swap `first` with `last`, or with `middle` if adjacent.
- Largest BST in a binary tree: postorder returns `(min, max, size)`; a node is a BST if `left.max < val < right.min`; else return `(−INF, INF, max size)`.

### Heap
Array-based; `std::priority_queue`. Top-K: min-heap of size K. Merge K sorted lists. Median of stream: two heaps (max-heap lower half, min-heap upper half). Scheduling tasks, K closest points.
- Implementation: parent `(i−1)/2`; push → sift up; pop → move last to root, sift down. Build heap in O(n): sift down from `n/2 − 1` to 0 (also converts a min-heap to a max-heap). Array is a min-heap if `a[i] ≤ a[2i+1], a[2i+2]` for all `i < n/2`.
- Kth largest: min-heap of size k (also for a stream: top = answer). Sort a K-sorted array: min-heap of size k+1. Connect ropes: pop two smallest, push the sum.
- Replace by rank: sort a copy, unique, `lower_bound + 1` (coordinate compression).
- Hand of straights: `map<int,int>` counts; start each group at the smallest key, consume `key..key+W−1`.
- Max sum combinations (top K of `a[i]+b[j]`): sort both descending, heap from `(0,0)`, push `(i+1,j)` and `(i,j+1)` with a visited set.
- K most frequent: count, then min-heap of size k or bucket sort by frequency. Design Twitter: per-user tweet lists with timestamps, merge followees' latest with a heap.

### Trie (prefix tree)
O(L) per operation, L = word length.

Each node is one prefix; children by letter. Keep **two counters** per node: `pass` (words through it, for prefix counts) and `ends` (words ending there, for "is this word present?"). Without `ends`, inserting `"apple"` makes `"app"` look present.

```cpp
struct Trie {
    vector<array<int, 26>> ch;   // ch[u][c] = child of u by letter c; 0 = none (root 0 is never a child)
    vector<int> pass, ends;      // pass[u] = words through node u, ends[u] = words ending exactly at u
    Trie() { newNode(); }
    int newNode() {
        ch.push_back({});        // {} zero-fills the 26 children
        pass.push_back(0), ends.push_back(0);
        return (int)ch.size() - 1;
    }
    void insert(const string& s) {
        int u = 0;
        pass[0]++;
        for (char c : s) {
            int x = c - 'a';
            if (!ch[u][x]) { int v = newNode(); ch[u][x] = v; }   // not ch[u][x] = newNode(): see pitfalls
            u = ch[u][x];
            pass[u]++;
        }
        ends[u]++;
    }
    int walk(const string& s) const {   // node reached by s, or -1
        int u = 0;
        for (char c : s)
            if (!(u = ch[u][c - 'a'])) return -1;
        return u;
    }
    int countWords(const string& s) const { int u = walk(s); return u < 0 ? 0 : ends[u]; }    // copies of s
    int countPrefix(const string& p) const { int u = walk(p); return u < 0 ? 0 : pass[u]; }   // words starting with p
    bool search(const string& s) const { return countWords(s) > 0; }
    bool startsWith(const string& p) const { return countPrefix(p) > 0; }
    void erase(const string& s) {       // removes one copy; nodes stay, counts drop to 0
        if (!countWords(s)) return;
        int u = 0;
        pass[0]--;
        for (char c : s) {
            u = ch[u][c - 'a'];
            pass[u]--;
        }
        ends[u]--;
    }
};
```
- Size: one node per inserted character + 1, each ~112 bytes (26 child ints + 2 counters). 1e6 total characters ≈ 112 MB: check the memory limit; for big alphabets use `map<char,int>` children.
- This `vector` version works as a local variable and is fresh per test case. A fixed `int ch[MAXN][26]` array must be **global** (a local one overflows the stack and starts with garbage instead of zeros); reset it between test cases by clearing only the `tot` nodes you used.
- Other alphabets: map the character to an index (`c - 'A'`, `c - '0'`) and change 26 to the alphabet size.

#### Pointer version (what interviewers usually expect for "Implement Trie", LC 208)
```cpp
struct Node {
    Node* next[26] = {};
    bool end = false;
};
class PTrie {
    Node* root = new Node();
    Node* find(const string& s) const {
        Node* u = root;
        for (char c : s)
            if (!(u = u->next[c - 'a'])) return nullptr;
        return u;
    }
    static void freeAll(Node* u) {
        if (!u) return;
        for (Node* v : u->next) freeAll(v);
        delete u;
    }
public:
    PTrie() = default;
    PTrie(const PTrie&) = delete;              // owns raw pointers: forbid shallow copies (rule of 3)
    PTrie& operator=(const PTrie&) = delete;
    ~PTrie() { freeAll(root); }
    void insert(const string& w) {
        Node* u = root;
        for (char c : w) {
            if (!u->next[c - 'a']) u->next[c - 'a'] = new Node();
            u = u->next[c - 'a'];
        }
        u->end = true;
    }
    bool search(const string& w) const { Node* u = find(w); return u && u->end; }
    bool startsWith(const string& p) const { return find(p) != nullptr; }   // p non-empty: "" always finds the root
};
```

#### Binary trie (max XOR)
Insert numbers bit by bit from the top; to maximize `x ^ y`, take the opposite bit whenever it exists. O(B) per op.
```cpp
struct XorTrie {
    static const int B = 31;              // bits 30..0: any non-negative int
    vector<array<int, 2>> ch{{0, 0}};     // node 0 = root
    void insert(int x) {
        int u = 0;
        for (int b = B - 1; b >= 0; b--) {
            int bit = x >> b & 1;
            if (!ch[u][bit]) {
                ch[u][bit] = (int)ch.size();
                ch.push_back({0, 0});
            }
            u = ch[u][bit];
        }
    }
    int maxXor(int x) const {             // max of x ^ y over inserted y; trie must be non-empty
        int u = 0, r = 0;
        for (int b = B - 1; b >= 0; b--) {
            int want = (x >> b & 1) ^ 1;  // prefer the opposite bit
            if (ch[u][want]) r |= 1 << b, u = ch[u][want];
            else u = ch[u][want ^ 1];
        }
        return r;
    }
};

int maxXorPair(const vector<int>& a) {    // LC 421; a non-empty
    XorTrie t;
    int best = 0;
    for (int x : a) {
        t.insert(x);
        best = max(best, t.maxXor(x));    // x ^ x = 0 covers the single-element case
    }
    return best;
}

// LC 1707: for each query (x, m), max x ^ a[i] over a[i] <= m, or -1. Offline: sort both, insert as m grows.
vector<int> maxXorWithLimit(vector<int> a, const vector<pair<int, int>>& q) {
    sort(a.begin(), a.end());
    vector<int> ord(q.size()), ans(q.size());
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j) { return q[i].second < q[j].second; });
    XorTrie t;
    size_t k = 0;
    for (int i : ord) {
        while (k < a.size() && a[k] <= q[i].second) t.insert(a[k++]);
        ans[i] = k ? t.maxXor(q[i].first) : -1;
    }
    return ans;
}
```
- `B = 31` covers non-negative `int`; use `B = 30` if values < 2^30, and `long long` with `1LL << b` for 64-bit values. Values must be non-negative: for negatives store `unsigned` values with `B = 32` and an `unsigned` result.
- XOR of a **subarray**: insert prefix XORs (start with 0); the max subarray XOR is `max over i of maxXor(pre[i])`. Count pairs with XOR < k: store subtree sizes and walk the bits of `k`.
- XOR over a **range of the array** (`l..r`) online: persistent binary trie (see advanced notes).

#### Trie problems (Striver step 17, common OA/interview)
```cpp
// Distinct substrings: insert every suffix; each new node is a new distinct substring. O(n^2).
int distinctSubstrings(const string& s) {      // non-empty ones; +1 if the empty string counts
    Trie t;
    for (size_t i = 0; i < s.size(); i++) t.insert(s.substr(i));
    return (int)t.ch.size() - 1;
}

// Longest word whose every prefix is also a word (ties: lexicographically smallest), "" if none.
string longestAllPrefixes(const vector<string>& words) {
    Trie t;
    for (auto& w : words) t.insert(w);
    string best;
    for (auto& w : words) {
        int u = 0;
        bool ok = true;
        for (char c : w) {
            u = t.ch[u][c - 'a'];
            if (!t.ends[u]) { ok = false; break; }
        }
        if (ok && (w.size() > best.size() || (w.size() == best.size() && w < best))) best = w;
    }
    return best;
}

// Add and Search Word (LC 211): '.' matches any letter
bool matchDot(const Trie& t, const string& s, size_t i = 0, int u = 0) {
    if (i == s.size()) return t.ends[u] > 0;
    if (s[i] != '.') { int v = t.ch[u][s[i] - 'a']; return v && matchDot(t, s, i + 1, v); }
    for (int c = 0; c < 26; c++)
        if (t.ch[u][c] && matchDot(t, s, i + 1, t.ch[u][c])) return true;
    return false;
}
```
- Distinct substrings via a trie is O(n²) time and nodes: fine for n ≤ ~5000. For larger n use a suffix array (`n(n+1)/2 − Σ lcp`) or suffix automaton (see [String algorithms](#s19) and the advanced notes).
- "Complete string" (longest word with all prefixes present) uses the same check; Striver's version prints `"None"` when nothing qualifies.

#### Word Search II (LC 212)
Build a trie of the words, then DFS the grid **walking the trie at the same time**; a dead trie branch stops the DFS early.
```cpp
vector<string> findWords(vector<vector<char>>& g, const vector<string>& words) {
    vector<array<int, 26>> ch(1);         // trie; ch(1) = root with zero children
    vector<int> wid(1, -1);               // index of the word ending at a node, -1 if none
    for (int i = 0; i < (int)words.size(); i++) {
        int u = 0;
        for (char c : words[i]) {
            if (!ch[u][c - 'a']) {
                ch[u][c - 'a'] = (int)ch.size();
                ch.push_back({}), wid.push_back(-1);
            }
            u = ch[u][c - 'a'];
        }
        wid[u] = i;
    }
    int R = g.size(), C = g[0].size();
    vector<string> res;
    function<void(int, int, int)> dfs = [&](int r, int c, int u) {
        char k = g[r][c];
        if (k == '#' || !(u = ch[u][k - 'a'])) return;
        if (wid[u] >= 0) res.push_back(words[wid[u]]), wid[u] = -1;   // report once
        g[r][c] = '#';                                                 // mark visited on the path
        int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C) dfs(nr, nc, u);
        }
        g[r][c] = k;                                                   // backtrack
    };
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++) dfs(r, c, 0);
    return res;
}
```
- Each word is reported once (`wid[u] = -1` after finding it). Extra pruning for big inputs: keep `pass` counts and delete a word's path once found, so exhausted branches are skipped.

**Recognize a trie problem:** many words with shared prefixes (autocomplete, prefix counts, phone directory, replace words with shortest root), wildcard matching over a dictionary, or "maximize XOR" over a set of numbers.

#### Trie pitfalls
- ⚠️ No `ends` marker: `search("app")` is true after inserting only `"apple"`.
- ⚠️ `ch[u][x] = newNode();` with a `vector` trie: before C++17 the left side can be evaluated first, then `push_back` reallocates and the write goes to freed memory. Store the new id in a variable first (as above).
- ⚠️ `startsWith("")`: decide whether the empty prefix counts; the pointer version returns true even on an empty trie.
- ⚠️ `erase` on a word that is not present corrupts counts: check `countWords` first (done above).
- ⚠️ XOR trie with too few bits silently drops high bits; on an empty XOR trie `maxXor` is meaningless (answer -1 / skip).
- ⚠️ Pointer trie: `new` nodes are never freed unless you write a destructor; copying such a class double-frees (see [Structs, classes & OOP](#s4)).

### Fenwick tree (BIT)
Point update, prefix query, O(log n), 1-indexed.
```cpp
struct BIT {
    int n;
    vector<ll> t;
    BIT(int n) : n(n), t(n + 1) {}
    void add(int i, ll v) {
        for (; i <= n; i += i & -i)
            t[i] += v;
    }
    ll sum(int i) {
        ll s = 0;
        for (; i > 0; i -= i & -i)
            s += t[i];
        return s;
    }  // [1..i]
    ll range(int l, int r) { return sum(r) - sum(l - 1); }
};
```
- Uses: inversions count (compress values), order statistics (binary descent), range-update/point-query with difference array, 2D BIT.

### Segment tree
Range query + point/range update, O(log n).
```cpp
// iterative bottom-up (size n, for commutative-ish ops like sum/min/max)
int n;
vector<int> t(2 * n);
void upd(int p, int v) {
    for (t[p += n] = v; p > 1; p >>= 1)
        t[p >> 1] = t[p] + t[p ^ 1];
}
int query(int l, int r) {
    int res = 0;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {  // [l, r)
        if (l & 1) res += t[l++];
        if (r & 1) res += t[--r];
    }
    return res;
}
```
- Recursive with lazy propagation for range add/assign; define `push` down and `pull` up carefully; node stores (sum, min, max, count, lazy).
- Merge-sortable nodes, "max subarray sum in range" node (sum, pref, suf, best), segment tree descent for k-th/first index ≥ x, persistent seg tree, seg tree on compressed coordinates, Li Chao tree.

### Sparse table
Static range min/max (idempotent), O(n log n) build, O(1) query.
```cpp
// st[k][i] = min of a[i .. i + 2^k)
for (int i = 0; i < n; i++) st[0][i] = a[i];
for (int k = 1; (1 << k) <= n; k++)
    for (int i = 0; i + (1 << k) <= n; i++)
        st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
int q(int l, int r) {
    int k = __lg(r - l + 1);
    return min(st[k][l], st[k][r - (1 << k) + 1]);
}
```
- Also used for LCA (Euler tour), RMQ, suffix-array LCP.

### Sqrt decomposition / Mo's algorithm
Blocks of size √n; offline range queries sorted by (block of l, r), add/remove pointers → O((n+q)√n).

### Others
Treap/splay (implicit keys), Link-cut, HLD (heavy-light decomposition → path queries via segment tree), Euler tour technique (subtree → range), LRU cache (list + unordered_map), LFU, min stack (pair with min), queue using two stacks, 2D prefix, Merge Sort Tree.

[⬆️ Back to contents](#contents)

---

<a id="s18"></a>
## 🕸️ 18. Graphs

### Representation
```cpp
vector<vector<int>> g(n);  // adjacency list (0-indexed!)
vector<vector<pii>> g(n);  // {to, weight}
g[u].push_back(v);
g[v].push_back(u);  // undirected: both directions
// grid as implicit graph: neighbors via dx,dy; id = r*m + c
// edge list for Kruskal / Bellman–Ford
```
Complexity: E≤V² (dense: matrix OK for V≤~2000). Multi-edges & self-loops, disconnected graphs (loop over all nodes).

### BFS
Unweighted shortest path; multi-source: push all sources at once.
```cpp
queue<int> q;
vector<int> d(n, -1);
d[s] = 0;
q.push(s);
while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int v : g[u])
        if (d[v] < 0) {
            d[v] = d[u] + 1;
            q.push(v);
        }
}
```
- 0-1 BFS: deque; push front for weight 0, back for 1.
- State-space BFS: `(node, extra)` like keys/steps remaining; BFS on bitmask states.
- Word ladder, rotten oranges, shortest path in binary matrix, minimum knight moves.

### DFS
Recursive/iterative, visited array, entry/exit times `tin/tout` (subtree = interval), parent tracking for undirected cycle detection.
- Directed cycle: color 0/1/2 (white/gray/black) — back edge to gray node.
- Connected components, flood fill/islands, bipartite check (2-coloring), topological sort.

### Topological sort
Kahn (indegree queue) — cycle iff processed < n; or DFS post-order reversed. Lexicographically smallest → min-heap. Course schedule, alien dictionary.

### Dijkstra
Non-negative weights, O((V+E) log V).
```cpp
vector<ll> dist(n, LINF);
priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
dist[s] = 0;
pq.push({0, s});
while (!pq.empty()) {
    ll d = pq.top().first;
    int u = pq.top().second;
    pq.pop();                   // [C++17] auto [d,u] = pq.top(); pq.pop();
    if (d > dist[u]) continue;  // stale
    for (auto& e : g[u]) {
        int v = e.first;
        ll w = e.second;  // [C++17] for (auto [v,w] : g[u])
        if (dist[u] + w < dist[v]) {
            dist[v] = dist[u] + w;
            pq.push({dist[v], v});
        }
    }
}
```
- Track parent for path reconstruction; count shortest paths; state-augmented Dijkstra (k stops, coupons).
- Negative edges → Bellman–Ford O(VE) (also detects negative cycle: relax V-th time), SPFA (avg fast).
- All pairs: Floyd–Warshall O(V³): `for k for i for j d[i][j]=min(d[i][j], d[i][k]+d[k][j])` (k outermost!). Detect negative cycle via `d[i][i]<0`.
- Johnson's for sparse all-pairs with negatives.

### MST
- Kruskal: sort edges, DSU, add if different components. O(E log E).
- Prim: heap-based like Dijkstra with key = edge weight.
- Second-best MST, max edge on path (binary lifting). Boruvka rarely.

### DSU (Union-Find)
Near O(α) per operation (path compression + union by size).
```cpp
struct DSU {
    vector<int> p, sz;
    DSU(int n) : p(n), sz(n, 1) { iota(all(p), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};
```
- Uses: components count, cycle detection in undirected graph, Kruskal, offline connectivity, accounts merge, grid islands (dynamic adds), weighted DSU (parity/offset), DSU with rollback.

### Advanced graph
- Bridges & articulation points: Tarjan low-link: `low[u]=min(low[u], tin[v])` for back edge, `low[v]` for tree child; bridge if `low[v] > tin[u]`; articulation if `low[v] >= tin[u]` (root: ≥2 children).
- SCC: Kosaraju (DFS order, then reversed graph) or Tarjan; condense to DAG. 2-SAT via SCC.
- Euler path/circuit: degree conditions (undirected: 0 or 2 odd; directed: in=out), Hierholzer.
- Bipartite matching: Kuhn's O(VE); Hopcroft–Karp; König's theorem; max flow (Dinic) / min cut; bipartite → flow.
- Lowest common ancestor: binary lifting `up[k][v]`, Euler tour + sparse table.
- Tree diameter: BFS twice, or DP height. Centroid decomposition for path queries.
- Cycle detection in functional graphs: Floyd/visited states; binary lifting for k-th successor.
- Shortest path on grid with obstacles removal: 0-1 BFS / Dijkstra on `(r,c,k)`.
- Negative cycle, "arbitrage" → Bellman–Ford on `-log` rates.
- Graph with N ≤ 20 → bitmask DP (Hamiltonian).

### Graph patterns (Striver step 15)
**BFS / DFS**
- Undirected cycle: BFS/DFS that sees a visited node which is not the parent. Directed cycle: DFS with a `pathVis` array (on the current path), or Kahn's processed < n.
- Boundary trick (surrounded regions, number of enclaves): DFS/BFS from every boundary `O`/land first and mark it safe; everything unmarked is captured / an enclave.
- Distance to the nearest 0 / rotten oranges: multi-source BFS from all sources at once; answer = levels.
- Distinct islands: record each island's cells relative to its first cell (`r − r0, c − c0`) in a `set<vector<pair>>`.
- Word ladder I: BFS over words, try all 26 letters at each position, erase from the word set when pushed. II: BFS to record each word's level, then DFS back from the end word through words with level − 1.
- Eventual safe states: reverse the edges, Kahn from terminal nodes (outdegree 0); everything processed is safe. Or DFS: nodes not on any cycle.
- Alien dictionary: the first differing char of adjacent words gives an edge; if a word is longer than the next one and is its prefix, the order is invalid. Then topo sort.

**Shortest paths**
- DAG: topo order, relax edges in that order, O(V+E) (works with negative weights).
- Dijkstra with `set<pair>`: erase the old `{dist, v}` before inserting the new one. A plain queue also gives correct answers but may re-relax nodes many times; the heap processes each node once at its final distance.
- Print path: keep `parent[v]` when relaxing, walk back from the target, reverse.
- Path with minimum effort / swim in rising water (minimise the max edge on a path): Dijkstra with `max(d, w)` instead of `d + w` (or sort edges + DSU, or binary search + BFS).
- Cheapest flights within K stops: BFS by **stops** (queue of `{stops, node, cost}`), relax only if `stops ≤ K`; don't use a heap keyed by cost.
- Number of shortest paths: `ways[v] = ways[u]` on a strict improvement, `+= ways[u]` on a tie (mod).
- Minimum multiplications to reach end: BFS over values `0..99999` (`(x·a[i]) % 100000`), each edge costs 1.
- City with fewest neighbours within a threshold: Floyd–Warshall, then count `d[i][j] ≤ T` per city (ties → largest index).

**DSU**
- Make network connected: need `components − 1` extra edges; impossible if edges < n − 1.
- Most stones removed: union each stone's row with its column (`col + maxRow + 1`); answer = stones − components.
- Number of islands II (cells turn to land online): DSU on `r·m + c`, count +1 per new land, −1 per successful union with a land neighbour.
- Making a large island: DSU the existing islands; for each 0, sum the sizes of the **distinct** roots around it, +1.
- Accounts merge: union each account with the first account that owned each email (`map<email, id>`).

[⬆️ Back to contents](#contents)

---

<a id="s19"></a>
## 📝 19. String algorithms

### Prefix function / KMP
O(n+m).
```cpp
vector<int> pi(n);
for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while (j && s[i] != s[j])
        j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
}
// pattern search: run on pattern + '#' + text; pi == |pattern| marks a match.
// smallest period = n - pi[n-1]; border = pi[n-1].
```
### Z-function
`z[i]` = LCP of `s` and `s[i..]`. Pattern match via `P + '#' + T`.
```cpp
for (int i = 1, l = 0, r = 0; i < n; i++) {
    if (i < r) z[i] = min(r - i, z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]])
        z[i]++;
    if (i + z[i] > r) l = i, r = i + z[i];
}
```

### Rolling hash (Rabin–Karp)
Prefix hashes `H[i+1]=H[i]*B+s[i]`, substring `H[r]-H[l]*pw[r-l]`; compare substrings in O(1); double mod / mod 2^61−1 + random base. Palindromic check, longest duplicate substring (binary search + hash), count distinct substrings.

### Manacher
All palindromic substrings in O(n) (odd/even arrays `d1`, `d2`).

### Suffix array, suffix automaton, Aho–Corasick
**Suffix array** (O(n log n)) + LCP (Kasai): distinct substrings = `n(n+1)/2 − ΣLCP`; **Suffix automaton**, **Aho–Corasick** (multi-pattern).

### Other string problems
- Shortest palindrome (add chars in front): `t = s + '#' + reverse(s)`; `L = pi.back()` is the longest palindromic prefix; answer = `reverse(s.substr(L)) + s`.
- Longest happy prefix (longest prefix that is also a suffix) = `s.substr(0, pi[n−1])`.
- Min bracket reversals to balance: remove matched pairs with a stack; with `open` `{` and `close` `}` left, answer = `ceil(open/2) + ceil(close/2)`; odd length → −1.
- Count and say: start with `"1"`, run-length encode `n−1` times (`count` then `digit`).

Longest common prefix of strings (sort, compare first & last), minimum window substring (sliding window with need/have counts), minimum rotation (Booth/doubling trick: search in `s+s`), run-length encoding, anagram/palindrome partitions, `s+s` contains rotations.

[⬆️ Back to contents](#contents)

---

<a id="s20"></a>
## 🧭 20. Problem-solving flow & pattern map

### Problem-solving flow
Restate → constraints → brute force → spot pattern (sorting? hashing? two pointers? BS on answer? DP? graph? greedy? bitmask?) → verify on examples → code → test edge cases → analyze complexity.

### Pattern recognition quick map
| Clue in problem | Think |
|---|---|
| "subarray / contiguous" + sums | prefix sums, sliding window, Kadane, hashmap of prefix |
| "sorted array" | binary search, two pointers |
| "minimize the maximum / maximize the minimum" | binary search on answer |
| "k-th largest/smallest, top k" | heap, nth_element, quickselect, BS |
| "next greater/smaller", histogram | monotonic stack |
| "shortest path" unweighted / weighted / negative | BFS / Dijkstra / Bellman–Ford |
| "connected / groups / merge" | DSU, DFS |
| "dependencies / ordering" | topological sort |
| "count ways / min cost / choices with overlapping subproblems" | DP |
| "all combinations/permutations", n ≤ 15 | backtracking / bitmask |
| "range query + updates" | BIT / segment tree; static → prefix / sparse table |
| "prefix / dictionary of words" | trie |
| "pattern matching in string" | KMP / Z / hashing |
| "intervals" | sort + sweep / heap |
| "xor, parity, pairs" | bit tricks, trie |
| "modulo 1e9+7 counting" | combinatorics, mod inverse, DP |
| "tree queries (path/subtree)" | Euler tour, LCA, HLD, rerooting |
| "n ≤ 40" | meet in the middle |
| "minimum operations to reach target state" | BFS on states |

[⬆️ Back to contents](#contents)

---

<a id="s21"></a>
## 🐞 21. Debugging & contest checklist

> [!CAUTION]
> ❌ Never submit with `endl` spam, debug prints to stdout, or `-fsanitize` leftovers in the timed build.

### Before coding
- Re-read constraints (n, values, T test cases, sum of n). Pick complexity first.
- Write 2–3 small test cases incl. edge cases: `n=0/1`, all equal, sorted/reverse sorted, max values, negatives, duplicates, empty string, disconnected graph, single node, cycles, overflow.
- Think about brute force → optimize. Write brute force to stress test.

### Common bugs
- Overflow (`int` in products/sums; use `ll`, `accumulate(..., 0LL)`).
- Off-by-one in loops/ranges; inclusive vs half-open; 0- vs 1-indexing (BIT is 1-indexed).
- Unsigned underflow with `.size()`.
- Not resetting globals between test cases (clear vectors, `fill`, DSU re-init); `memset` over huge arrays per test = TLE.
- Out-of-bounds (`a[-1]`, `a[n]`); use `.at()` or `-fsanitize=address,undefined -D_GLIBCXX_DEBUG`.
- Uninitialized variables; `INF` that overflows when added; comparing `INF` states.
- Modifying container during iteration; reading `top()`/`front()` of empty.
- Comparator not strict (`<=`); `set` treats equal-by-comp elements as duplicates (lost items).
- `unordered_map` anti-hash TLE; `map[]` accidental insertion changing size.
- Reading input wrong (`cin >> n` then `getline` without `ws`); mixing `scanf` with synced-off cin.
- Floating-point precision & printing (`setprecision`).
- Stack overflow from deep recursion; large local arrays.
- Mod: negative results, forgetting `%` in one step, mod of subtraction.
- Integer `pow` using `double`.
- Using `endl` on 1e5+ lines.

### Stress test script
```bash
g++ -std=c++14 -O2 sol.cpp -o sol && g++ -std=c++14 -O2 brute.cpp -o brute && g++ -std=c++14 gen.cpp -o gen
for i in $(seq 1 1000); do ./gen $i > in.txt; ./sol < in.txt > o1.txt; ./brute < in.txt > o2.txt
  cmp -s o1.txt o2.txt || { echo "DIFF on seed $i"; break; }; done
```

### Compile/run
```bash
g++ -std=c++14 -O2 -Wall -Wextra -Wshadow -fsanitize=address,undefined -g file.cpp -o a && ./a < in.txt
```
Debug macro: `#define dbg(x) cerr << #x << " = " << (x) << '\n'` (stderr, remove/guard before submitting).

[⬆️ Back to contents](#contents)

---

> [!TIP]
> **Next:** [notes_advanced.md](notes_advanced.md) — advanced algorithms, data structures, theory, interview toolkit and practice roadmap.

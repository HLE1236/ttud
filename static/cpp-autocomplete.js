/* C++17 Syntax & STL Autocomplete for CodeMirror 5 (offline, zero-AI) */
(function (global) {
  "use strict";

  const KEYWORDS = [
    "auto", "break", "case", "char", "class", "const", "constexpr", "continue",
    "default", "delete", "do", "double", "else", "enum", "explicit", "extern",
    "false", "final", "float", "for", "friend", "goto", "if", "inline", "int",
    "long", "mutable", "namespace", "new", "nullptr", "operator", "override",
    "private", "protected", "public", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "template", "this", "throw", "true", "try",
    "catch", "typedef", "typename", "union", "unsigned", "using", "virtual",
    "void", "volatile", "while"
  ];

  const TYPES = [
    { label: "long long", insert: "long long ", detail: "64-bit int" },
    { label: "unsigned long long", insert: "unsigned long long ", detail: "64-bit uint" },
    { label: "int64_t", insert: "int64_t ", detail: "cstdint" },
    { label: "uint64_t", insert: "uint64_t ", detail: "cstdint" },
    { label: "size_t", insert: "size_t ", detail: "unsigned size" },
    { label: "string", insert: "string", detail: "std::string" },
    { label: "vector", insert: "vector<|>", detail: "std::vector<T>" },
    { label: "pair", insert: "pair<|, >", detail: "std::pair<A, B>" },
    { label: "map", insert: "map<|, >", detail: "std::map<K, V>" },
    { label: "unordered_map", insert: "unordered_map<|, >", detail: "hash map" },
    { label: "set", insert: "set<|>", detail: "std::set<T>" },
    { label: "unordered_set", insert: "unordered_set<|>", detail: "hash set" },
    { label: "multiset", insert: "multiset<|>", detail: "std::multiset<T>" },
    { label: "queue", insert: "queue<|>", detail: "FIFO queue" },
    { label: "deque", insert: "deque<|>", detail: "double-ended queue" },
    { label: "stack", insert: "stack<|>", detail: "LIFO stack" },
    { label: "priority_queue", insert: "priority_queue<|>", detail: "max-heap" },
    { label: "array", insert: "array<|, >", detail: "std::array<T, N>" },
    { label: "tuple", insert: "tuple<|>", detail: "std::tuple" },
    { label: "bitset", insert: "bitset<|>", detail: "std::bitset<N>" },
    { label: "stringstream", insert: "stringstream", detail: "sstream" },
  ];

  const STL_FUNCS = [
    // I/O
    { label: "cin", insert: "cin >> |", detail: "stdin" },
    { label: "cout", insert: "cout << |", detail: "stdout" },
    { label: "cerr", insert: "cerr << |", detail: "stderr" },
    { label: "endl", insert: "endl", detail: "newline + flush" },
    { label: "getline", insert: "getline(cin, |)", detail: "read line" },
    { label: "scanf", insert: "scanf(\"|\", )", detail: "cstdio" },
    { label: "printf", insert: "printf(\"|\", )", detail: "cstdio" },
    // Algorithms
    { label: "sort", insert: "sort(|.begin(), .end())", detail: "O(N log N)" },
    { label: "stable_sort", insert: "stable_sort(|.begin(), .end())", detail: "stable O(N log N)" },
    { label: "reverse", insert: "reverse(|.begin(), .end())", detail: "reverse range" },
    { label: "lower_bound", insert: "lower_bound(|.begin(), .end(), )", detail: "first >= val" },
    { label: "upper_bound", insert: "upper_bound(|.begin(), .end(), )", detail: "first > val" },
    { label: "binary_search", insert: "binary_search(|.begin(), .end(), )", detail: "bool exists" },
    { label: "min", insert: "min(|, )", detail: "minimum" },
    { label: "max", insert: "max(|, )", detail: "maximum" },
    { label: "minmax", insert: "minmax(|, )", detail: "pair(min, max)" },
    { label: "min_element", insert: "min_element(|.begin(), .end())", detail: "iterator to min" },
    { label: "max_element", insert: "max_element(|.begin(), .end())", detail: "iterator to max" },
    { label: "swap", insert: "swap(|, )", detail: "swap two values" },
    { label: "fill", insert: "fill(|.begin(), .end(), )", detail: "fill range" },
    { label: "memset", insert: "memset(|, 0, sizeof())", detail: "fill bytes" },
    { label: "memcpy", insert: "memcpy(|, , sizeof())", detail: "copy bytes" },
    { label: "accumulate", insert: "accumulate(|.begin(), .end(), 0LL)", detail: "sum range" },
    { label: "next_permutation", insert: "next_permutation(|.begin(), .end())", detail: "next perm" },
    { label: "prev_permutation", insert: "prev_permutation(|.begin(), .end())", detail: "prev perm" },
    { label: "unique", insert: "unique(|.begin(), .end())", detail: "dedup adjacent" },
    { label: "count", insert: "count(|.begin(), .end(), )", detail: "count occurrences" },
    { label: "find", insert: "find(|.begin(), .end(), )", detail: "linear find" },
    { label: "iota", insert: "iota(|.begin(), .end(), 0)", detail: "0, 1, 2..." },
    { label: "gcd", insert: "gcd(|, )", detail: "C++17 GCD" },
    { label: "lcm", insert: "lcm(|, )", detail: "C++17 LCM" },
    { label: "__gcd", insert: "__gcd(|, )", detail: "GNU GCD" },
    { label: "make_pair", insert: "make_pair(|, )", detail: "pair" },
    { label: "make_tuple", insert: "make_tuple(|, )", detail: "tuple" },
    { label: "tie", insert: "tie(|, )", detail: "unpack tuple/pair" },
    // Math & Bitwise
    { label: "abs", insert: "abs(|)", detail: "absolute value" },
    { label: "fabs", insert: "fabs(|)", detail: "float abs" },
    { label: "sqrt", insert: "sqrt(|)", detail: "square root" },
    { label: "pow", insert: "pow(|, )", detail: "power" },
    { label: "log2", insert: "log2(|)", detail: "base-2 log" },
    { label: "ceil", insert: "ceil(|)", detail: "round up" },
    { label: "floor", insert: "floor(|)", detail: "round down" },
    { label: "round", insert: "round(|)", detail: "nearest int" },
    { label: "__builtin_popcount", insert: "__builtin_popcount(|)", detail: "set bits (int)" },
    { label: "__builtin_popcountll", insert: "__builtin_popcountll(|)", detail: "set bits (ll)" },
    { label: "__builtin_clz", insert: "__builtin_clz(|)", detail: "leading zeros" },
    { label: "__builtin_ctz", insert: "__builtin_ctz(|)", detail: "trailing zeros" },
    // String & Char
    { label: "to_string", insert: "to_string(|)", detail: "number -> string" },
    { label: "stoi", insert: "stoi(|)", detail: "string -> int" },
    { label: "stoll", insert: "stoll(|)", detail: "string -> long long" },
    { label: "stod", insert: "stod(|)", detail: "string -> double" },
    { label: "isdigit", insert: "isdigit(|)", detail: "is digit char" },
    { label: "isalpha", insert: "isalpha(|)", detail: "is alpha char" },
    { label: "tolower", insert: "tolower(|)", detail: "lowercase char" },
    { label: "toupper", insert: "toupper(|)", detail: "uppercase char" },
    // Constants
    { label: "INT_MAX", insert: "INT_MAX", detail: "2147483647" },
    { label: "INT_MIN", insert: "INT_MIN", detail: "-2147483648" },
    { label: "LLONG_MAX", insert: "LLONG_MAX", detail: "9.22e18" },
    { label: "LLONG_MIN", insert: "LLONG_MIN", detail: "-9.22e18" },
  ];

  const SNIPPETS = [
    {
      label: "fori",
      insert: "for (int i = 0; i < |n; i++) {\n    \n}",
      detail: "for (0 .. n-1)"
    },
    {
      label: "forj",
      insert: "for (int j = 0; j < |m; j++) {\n    \n}",
      detail: "for (0 .. m-1)"
    },
    {
      label: "for1",
      insert: "for (int i = 1; i <= |n; i++) {\n    \n}",
      detail: "for (1 .. n)"
    },
    {
      label: "forr",
      insert: "for (int i = |n - 1; i >= 0; i--) {\n    \n}",
      detail: "reverse for"
    },
    {
      label: "fore",
      insert: "for (auto &x : |) {\n    \n}",
      detail: "range-based for"
    },
    {
      label: "while",
      insert: "while (|) {\n    \n}",
      detail: "while loop"
    },
    {
      label: "if",
      insert: "if (|) {\n    \n}",
      detail: "if block"
    },
    {
      label: "ifelse",
      insert: "if (|) {\n    \n} else {\n    \n}",
      detail: "if / else block"
    },
    {
      label: "struct",
      insert: "struct |Name {\n    \n};",
      detail: "struct definition"
    },
    {
      label: "fastio",
      insert: "ios::sync_with_stdio(false);\ncin.tie(nullptr);",
      detail: "fast I/O"
    },
    {
      label: "using namespace std",
      insert: "using namespace std;",
      detail: "std namespace"
    },
  ];

  const PREPROCESSOR = [
    { label: "#include <bits/stdc++.h>", insert: "#include <bits/stdc++.h>", detail: "All STL" },
    { label: "#include <iostream>", insert: "#include <iostream>", detail: "cin / cout" },
    { label: "#include <vector>", insert: "#include <vector>", detail: "vector" },
    { label: "#include <algorithm>", insert: "#include <algorithm>", detail: "sort, etc." },
    { label: "#include <string>", insert: "#include <string>", detail: "string" },
    { label: "#include <queue>", insert: "#include <queue>", detail: "queue / pq" },
    { label: "#include <stack>", insert: "#include <stack>", detail: "stack" },
    { label: "#include <map>", insert: "#include <map>", detail: "map" },
    { label: "#include <set>", insert: "#include <set>", detail: "set" },
    { label: "#include <unordered_map>", insert: "#include <unordered_map>", detail: "unordered_map" },
    { label: "#include <unordered_set>", insert: "#include <unordered_set>", detail: "unordered_set" },
    { label: "#include <cmath>", insert: "#include <cmath>", detail: "math" },
    { label: "#include <cstring>", insert: "#include <cstring>", detail: "memset" },
    { label: "#include <climits>", insert: "#include <climits>", detail: "INT_MAX" },
    { label: "#define", insert: "#define |", detail: "macro" },
    { label: "#ifdef", insert: "#ifdef |", detail: "conditional" },
    { label: "#ifndef", insert: "#ifndef |", detail: "conditional" },
    { label: "#endif", insert: "#endif", detail: "end ifdef" },
    { label: "#pragma GCC optimize", insert: '#pragma GCC optimize("O3,unroll-loops")', detail: "O3 pragma" },
  ];

  const CONTAINER_METHODS = {
    vector: [
      { label: "push_back", insert: "push_back(|)", detail: "append element" },
      { label: "emplace_back", insert: "emplace_back(|)", detail: "construct in-place" },
      { label: "pop_back", insert: "pop_back()", detail: "remove last" },
      { label: "size", insert: "size()", detail: "element count" },
      { label: "empty", insert: "empty()", detail: "is empty" },
      { label: "clear", insert: "clear()", detail: "remove all" },
      { label: "resize", insert: "resize(|)", detail: "change size" },
      { label: "assign", insert: "assign(|, )", detail: "fill n copies" },
      { label: "begin", insert: "begin()", detail: "start iterator" },
      { label: "end", insert: "end()", detail: "end iterator" },
      { label: "rbegin", insert: "rbegin()", detail: "reverse start" },
      { label: "rend", insert: "rend()", detail: "reverse end" },
      { label: "front", insert: "front()", detail: "first element" },
      { label: "back", insert: "back()", detail: "last element" },
      { label: "insert", insert: "insert(|, )", detail: "insert at pos" },
      { label: "erase", insert: "erase(|)", detail: "erase at pos" },
      { label: "reserve", insert: "reserve(|)", detail: "reserve capacity" },
    ],
    string: [
      { label: "length", insert: "length()", detail: "string length" },
      { label: "size", insert: "size()", detail: "string length" },
      { label: "empty", insert: "empty()", detail: "is empty" },
      { label: "substr", insert: "substr(|, )", detail: "substring(pos, len)" },
      { label: "find", insert: "find(|)", detail: "find substring" },
      { label: "rfind", insert: "rfind(|)", detail: "find from right" },
      { label: "push_back", insert: "push_back(|)", detail: "append char" },
      { label: "pop_back", insert: "pop_back()", detail: "remove last char" },
      { label: "append", insert: "append(|)", detail: "append string" },
      { label: "clear", insert: "clear()", detail: "clear string" },
      { label: "begin", insert: "begin()", detail: "start iterator" },
      { label: "end", insert: "end()", detail: "end iterator" },
      { label: "c_str", insert: "c_str()", detail: "const char*" },
      { label: "front", insert: "front()", detail: "first char" },
      { label: "back", insert: "back()", detail: "last char" },
      { label: "erase", insert: "erase(|, )", detail: "erase chars" },
    ],
    deque: [
      { label: "push_back", insert: "push_back(|)", detail: "append back" },
      { label: "push_front", insert: "push_front(|)", detail: "prepend front" },
      { label: "pop_back", insert: "pop_back()", detail: "remove back" },
      { label: "pop_front", insert: "pop_front()", detail: "remove front" },
      { label: "front", insert: "front()", detail: "first element" },
      { label: "back", insert: "back()", detail: "last element" },
      { label: "size", insert: "size()", detail: "element count" },
      { label: "empty", insert: "empty()", detail: "is empty" },
      { label: "clear", insert: "clear()", detail: "remove all" },
      { label: "begin", insert: "begin()", detail: "start iterator" },
      { label: "end", insert: "end()", detail: "end iterator" },
    ],
    queue: [
      { label: "push", insert: "push(|)", detail: "enqueue" },
      { label: "emplace", insert: "emplace(|)", detail: "construct in-place" },
      { label: "pop", insert: "pop()", detail: "dequeue" },
      { label: "front", insert: "front()", detail: "next element" },
      { label: "back", insert: "back()", detail: "last element" },
      { label: "size", insert: "size()", detail: "element count" },
      { label: "empty", insert: "empty()", detail: "is empty" },
    ],
    stack: [
      { label: "push", insert: "push(|)", detail: "push to top" },
      { label: "emplace", insert: "emplace(|)", detail: "construct in-place" },
      { label: "pop", insert: "pop()", detail: "pop top" },
      { label: "top", insert: "top()", detail: "top element" },
      { label: "size", insert: "size()", detail: "element count" },
      { label: "empty", insert: "empty()", detail: "is empty" },
    ],
    priority_queue: [
      { label: "push", insert: "push(|)", detail: "insert element" },
      { label: "emplace", insert: "emplace(|)", detail: "construct in-place" },
      { label: "pop", insert: "pop()", detail: "remove top" },
      { label: "top", insert: "top()", detail: "max/min element" },
      { label: "size", insert: "size()", detail: "element count" },
      { label: "empty", insert: "empty()", detail: "is empty" },
    ],
    set: [
      { label: "insert", insert: "insert(|)", detail: "insert key" },
      { label: "emplace", insert: "emplace(|)", detail: "construct in-place" },
      { label: "erase", insert: "erase(|)", detail: "erase key/iter" },
      { label: "find", insert: "find(|)", detail: "find key" },
      { label: "count", insert: "count(|)", detail: "count key" },
      { label: "lower_bound", insert: "lower_bound(|)", detail: "first >= key" },
      { label: "upper_bound", insert: "upper_bound(|)", detail: "first > key" },
      { label: "size", insert: "size()", detail: "element count" },
      { label: "empty", insert: "empty()", detail: "is empty" },
      { label: "clear", insert: "clear()", detail: "remove all" },
      { label: "begin", insert: "begin()", detail: "min iterator" },
      { label: "end", insert: "end()", detail: "end iterator" },
    ],
    map: [
      { label: "insert", insert: "insert({|, })", detail: "insert pair" },
      { label: "emplace", insert: "emplace(|, )", detail: "construct in-place" },
      { label: "erase", insert: "erase(|)", detail: "erase key/iter" },
      { label: "find", insert: "find(|)", detail: "find key" },
      { label: "count", insert: "count(|)", detail: "0 or 1" },
      { label: "lower_bound", insert: "lower_bound(|)", detail: "first >= key" },
      { label: "upper_bound", insert: "upper_bound(|)", detail: "first > key" },
      { label: "size", insert: "size()", detail: "entry count" },
      { label: "empty", insert: "empty()", detail: "is empty" },
      { label: "clear", insert: "clear()", detail: "remove all" },
      { label: "begin", insert: "begin()", detail: "start iterator" },
      { label: "end", insert: "end()", detail: "end iterator" },
    ],
    pair: [
      { label: "first", insert: "first", detail: "1st element" },
      { label: "second", insert: "second", detail: "2nd element" },
    ],
    bitset: [
      { label: "set", insert: "set(|)", detail: "set bit(s) to 1" },
      { label: "reset", insert: "reset(|)", detail: "set bit(s) to 0" },
      { label: "flip", insert: "flip(|)", detail: "toggle bit(s)" },
      { label: "test", insert: "test(|)", detail: "read bit" },
      { label: "count", insert: "count()", detail: "count 1-bits" },
      { label: "any", insert: "any()", detail: "any bit set" },
      { label: "none", insert: "none()", detail: "all bits 0" },
      { label: "all", insert: "all()", detail: "all bits 1" },
      { label: "size", insert: "size()", detail: "bit count" },
      { label: "to_ulong", insert: "to_ulong()", detail: "to unsigned long" },
      { label: "to_ullong", insert: "to_ullong()", detail: "to ull" },
    ],
    cin: [
      { label: "tie", insert: "tie(nullptr)", detail: "untie cin/cout" },
      { label: "ignore", insert: "ignore(|)", detail: "skip chars" },
      { label: "peek", insert: "peek()", detail: "peek next char" },
      { label: "eof", insert: "eof()", detail: "end of file" },
    ],
    cout: [
      { label: "precision", insert: "precision(|)", detail: "float precision" },
      { label: "flush", insert: "flush()", detail: "flush buffer" },
    ],
    stringstream: [
      { label: "str", insert: "str(|)", detail: "get/set string" },
      { label: "clear", insert: "clear()", detail: "reset flags" },
    ],
  };

  // Deduplicated fallback list of all common member names when variable type isn't inferred
  const ALL_MEMBER_METHODS = (() => {
    const seen = new Set();
    const out = [];
    for (const list of Object.values(CONTAINER_METHODS)) {
      for (const item of list) {
        if (!seen.has(item.label)) {
          seen.add(item.label);
          out.push(item);
        }
      }
    }
    return out;
  })();

  const SCOPE_MEMBERS = {
    std: [
      ...TYPES,
      ...STL_FUNCS.filter((f) => !f.label.startsWith("__")),
      { label: "numeric_limits", insert: "numeric_limits<|>", detail: "limits" },
      { label: "ios_base", insert: "ios_base", detail: "stream base" },
      { label: "greater", insert: "greater<|>", detail: "comparator >" },
      { label: "less", insert: "less<|>", detail: "comparator <" },
      { label: "function", insert: "function<|>", detail: "std::function" },
    ],
    ios_base: [
      { label: "sync_with_stdio", insert: "sync_with_stdio(false);", detail: "fast I/O" },
    ],
    ios: [
      { label: "sync_with_stdio", insert: "sync_with_stdio(false);", detail: "fast I/O" },
    ],
    string: [
      { label: "npos", insert: "npos", detail: "not found" },
    ],
  };

  const KEYWORD_SET = new Set(KEYWORDS);

  const CONTAINER_ALIASES = {
    unordered_set: "set",
    multiset: "set",
    unordered_map: "map",
    multimap: "map",
  };

  function extractDocumentInfo(code, currentWord) {
    const varTypes = Object.create(null);
    const localIds = new Set();

    // Light regex for common C++ container declarations:
    // e.g. vector<int> adj, adj2; priority_queue<pair<int,int>> pq; string s;
    const declRe = /\b(vector|string|deque|queue|stack|priority_queue|set|unordered_set|multiset|map|unordered_map|multimap|pair|bitset|stringstream)\s*(?:<[^;{}()]*>)?\s+([a-zA-Z_][a-zA-Z0-9_]*(?:\s*,\s*[a-zA-Z_][a-zA-Z0-9_]*)*)/g;
    let m;
    while ((m = declRe.exec(code)) !== null) {
      const rawType = CONTAINER_ALIASES[m[1]] || m[1];
      const names = m[2].split(",");
      for (const rawName of names) {
        const name = rawName.trim().replace(/[^a-zA-Z0-9_].*$/, "");
        if (name && !KEYWORD_SET.has(name)) {
          varTypes[name] = rawType;
        }
      }
    }

    const idRe = /\b[a-zA-Z_][a-zA-Z0-9_]{1,}\b/g;
    while ((m = idRe.exec(code)) !== null) {
      const id = m[0];
      if (id !== currentWord && !KEYWORD_SET.has(id)) {
        localIds.add(id);
      }
    }

    return { varTypes, localIds };
  }

  function scoreMatch(label, query) {
    if (!query) return 100;
    const lLow = label.toLowerCase();
    const qLow = query.toLowerCase();
    if (label === query) return -1; // exact full match already typed
    if (label.startsWith(query)) return 300 - label.length;
    if (lLow.startsWith(qLow)) return 200 - label.length;
    const idx = lLow.indexOf(qLow);
    if (idx !== -1) return 100 - idx - label.length * 0.1;
    return -1;
  }

  function attachCppAutocomplete(cm, options) {
    const opts = Object.assign({ enabled: true }, options);
    let enabled = opts.enabled;
    let popup = null;
    let items = [];
    let selectedIdx = 0;
    let replaceFrom = null;
    let replaceTo = null;
    let debounceTimer = null;

    function createPopup() {
      if (popup) return popup;
      popup = document.createElement("div");
      popup.className = "cpp-hint-popup hidden";
      popup.setAttribute("role", "listbox");
      popup.addEventListener("mousedown", (e) => {
        e.preventDefault();
        const row = e.target.closest(".cpp-hint-item");
        if (!row) return;
        const idx = Number(row.dataset.idx);
        if (!Number.isNaN(idx)) {
          selectedIdx = idx;
          acceptCurrent();
        }
      });
      document.body.appendChild(popup);
      return popup;
    }

    function isOpen() {
      return popup && !popup.classList.contains("hidden");
    }

    const activeKeyMap = {
      Up: () => moveSelection(-1),
      Down: () => moveSelection(1),
      PageUp: () => moveSelection(-5),
      PageDown: () => moveSelection(5),
      Enter: () => acceptCurrent(),
      Tab: () => acceptCurrent(),
      Esc: () => closePopup(),
    };

    function openPopup(suggestions, from, to) {
      if (!suggestions.length) {
        closePopup();
        return;
      }
      const el = createPopup();
      items = suggestions;
      selectedIdx = 0;
      replaceFrom = from;
      replaceTo = to;

      renderList();
      el.classList.remove("hidden");
      positionPopup();

      cm.removeKeyMap(activeKeyMap);
      cm.addKeyMap(activeKeyMap);
    }

    function closePopup() {
      if (popup) popup.classList.add("hidden");
      items = [];
      selectedIdx = 0;
      cm.removeKeyMap(activeKeyMap);
    }

    function positionPopup() {
      if (!popup || !isOpen()) return;
      const coords = cm.cursorCoords(true, "window");
      const vw = window.innerWidth;
      const vh = window.innerHeight;
      const rect = popup.getBoundingClientRect();
      const width = rect.width || 300;
      const height = rect.height || 220;

      let left = coords.left;
      if (left + width > vw - 12) {
        left = Math.max(12, vw - width - 12);
      }

      let top = coords.bottom + 4;
      if (top + height > vh - 12 && coords.top - height - 4 > 12) {
        top = coords.top - height - 4;
      }

      popup.style.left = `${Math.round(left)}px`;
      popup.style.top = `${Math.round(top)}px`;
    }

    function highlightLabel(label, query) {
      const esc = (s) =>
        String(s).replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
      if (!query) return esc(label);
      const idx = label.toLowerCase().indexOf(query.toLowerCase());
      if (idx === -1) return esc(label);
      return (
        esc(label.slice(0, idx)) +
        `<mark class="cpp-hint-match">${esc(label.slice(idx, idx + query.length))}</mark>` +
        esc(label.slice(idx + query.length))
      );
    }

    function renderList() {
      if (!popup) return;
      const query = cm.getRange(replaceFrom, replaceTo);
      const rowsHtml = items
        .map((it, idx) => {
          const active = idx === selectedIdx ? " active" : "";
          return `<div class="cpp-hint-item${active}" role="option" aria-selected="${idx === selectedIdx}" data-idx="${idx}">
            <span class="cpp-hint-kind kind-${it.kind}">${it.kindLabel}</span>
            <span class="cpp-hint-label">${highlightLabel(it.label, query)}</span>
            ${it.detail ? `<span class="cpp-hint-detail">${it.detail.replace(/</g, "&lt;")}</span>` : ""}
          </div>`;
        })
        .join("");

      popup.innerHTML = `<div class="cpp-hint-list">${rowsHtml}</div>
        <div class="cpp-hint-footer">↑↓ Chọn · Tab/↵ Chèn · Esc Đóng</div>`;

      const activeEl = popup.querySelector(".cpp-hint-item.active");
      if (activeEl) activeEl.scrollIntoView({ block: "nearest" });
    }

    function moveSelection(delta) {
      if (!items.length) return;
      selectedIdx = (selectedIdx + delta + items.length) % items.length;
      renderList();
    }

    function acceptCurrent() {
      const item = items[selectedIdx];
      if (!item) return;

      const lineText = cm.getLine(replaceFrom.line);
      const indentMatch = /^\s*/.exec(lineText);
      const baseIndent = indentMatch ? indentMatch[0] : "";

      let rawInsert = item.insert || item.label;

      // Avoid duplicating parentheses if "(" already follows cursor
      const afterChar = lineText.charAt(replaceTo.ch);
      if (afterChar === "(" && rawInsert.endsWith("()")) {
        rawInsert = rawInsert.slice(0, -2);
      } else if (afterChar === "(" && rawInsert.includes("(")) {
        const parenIdx = rawInsert.indexOf("(");
        rawInsert = rawInsert.slice(0, parenIdx);
      }

      // Apply current line indentation to multi-line snippets
      const indented = rawInsert.replace(/\n/g, "\n" + baseIndent);

      // Find cursor placeholder "|"
      const markerIdx = indented.indexOf("|");
      const finalText = markerIdx !== -1
        ? indented.slice(0, markerIdx) + indented.slice(markerIdx + 1)
        : indented;

      cm.operation(() => {
        cm.replaceRange(finalText, replaceFrom, replaceTo, "+complete");
        if (markerIdx !== -1) {
          const beforeMarker = indented.slice(0, markerIdx);
          const lines = beforeMarker.split("\n");
          const targetLine = replaceFrom.line + lines.length - 1;
          const targetCh = lines.length === 1
            ? replaceFrom.ch + lines[0].length
            : lines[lines.length - 1].length;
          cm.setCursor({ line: targetLine, ch: targetCh });
        }
      });

      closePopup();
      cm.focus();
    }

    function collectSuggestions(manual) {
      const cursor = cm.getCursor();
      if (cm.somethingSelected()) return null;

      const tokenType = cm.getTokenTypeAt(cursor) || "";
      if (!manual && (tokenType.includes("comment") || tokenType.includes("string"))) {
        return null;
      }

      const line = cm.getLine(cursor.line);
      const prefix = line.slice(0, cursor.ch);

      // 1. Preprocessor (#include, #define, ...)
      const ppMatch = /^\s*(#[a-zA-Z0-9_<>/.]*)$/.exec(prefix);
      if (ppMatch) {
        const query = ppMatch[1];
        const from = { line: cursor.line, ch: cursor.ch - query.length };
        const scored = PREPROCESSOR
          .map((item) => ({
            ...item,
            kind: "pp",
            kindLabel: "#",
            score: scoreMatch(item.label, query),
          }))
          .filter((x) => x.score >= 0)
          .sort((a, b) => b.score - a.score)
          .slice(0, 12);
        return { list: scored, from, to: cursor };
      }

      // 2. Scope resolution (std::, ios_base::, ...)
      const scopeMatch = /([a-zA-Z_][a-zA-Z0-9_]*)::([a-zA-Z0-9_]*)$/.exec(prefix);
      if (scopeMatch) {
        const ns = scopeMatch[1];
        const query = scopeMatch[2];
        const from = { line: cursor.line, ch: cursor.ch - query.length };
        const pool = SCOPE_MEMBERS[ns] || SCOPE_MEMBERS.std;
        const scored = pool
          .map((item) => ({
            ...item,
            kind: "stl",
            kindLabel: "::",
            score: scoreMatch(item.label, query),
          }))
          .filter((x) => x.score >= 0)
          .sort((a, b) => b.score - a.score)
          .slice(0, 14);
        return { list: scored, from, to: cursor };
      }

      // 3. Member access (obj.method or ptr->method)
      const memMatch = /([a-zA-Z_][a-zA-Z0-9_]*)\s*(?:\[[^\]\n]*\]|\(\))?\s*(?:\.|->)\s*([a-zA-Z0-9_]*)$/.exec(prefix);
      if (memMatch) {
        const objName = memMatch[1];
        const query = memMatch[2];
        const from = { line: cursor.line, ch: cursor.ch - query.length };
        const { varTypes } = extractDocumentInfo(cm.getValue(), "");
        const inferred = CONTAINER_METHODS[objName] ? objName : varTypes[objName];
        const pool = (inferred && CONTAINER_METHODS[inferred]) || ALL_MEMBER_METHODS;
        const scored = pool
          .map((item) => ({
            ...item,
            kind: "fn",
            kindLabel: "fn",
            score: scoreMatch(item.label, query),
          }))
          .filter((x) => x.score >= 0)
          .sort((a, b) => b.score - a.score)
          .slice(0, 14);
        return { list: scored, from, to: cursor };
      }

      // 4. General identifier / keyword / STL / snippet
      const wordMatch = /([a-zA-Z_][a-zA-Z0-9_]*)$/.exec(prefix);
      const query = wordMatch ? wordMatch[1] : "";
      if (!manual && query.length < 2) return null;

      const from = { line: cursor.line, ch: cursor.ch - query.length };
      const { localIds } = extractDocumentInfo(cm.getValue(), query);

      const candidates = new Map();
      const addCandidate = (label, insert, kind, kindLabel, detail, boost) => {
        const s = scoreMatch(label, query);
        if (s < 0) return;
        const prev = candidates.get(label);
        const total = s + boost;
        if (!prev || total > prev.score) {
          candidates.set(label, { label, insert, kind, kindLabel, detail, score: total });
        }
      };

      for (const s of SNIPPETS) addCandidate(s.label, s.insert, "snip", "snip", s.detail, 25);
      for (const f of STL_FUNCS) addCandidate(f.label, f.insert, "fn", "stl", f.detail, 20);
      for (const t of TYPES) addCandidate(t.label, t.insert, "type", "type", t.detail, 18);
      for (const k of KEYWORDS) addCandidate(k, k, "kw", "kw", "keyword", 10);
      for (const id of localIds) addCandidate(id, id, "var", "var", "local", 15);

      const list = Array.from(candidates.values())
        .sort((a, b) => b.score - a.score)
        .slice(0, 14);

      return { list, from, to: cursor };
    }

    function trigger(manual) {
      if (!enabled && !manual) return;
      const res = collectSuggestions(manual);
      if (res && res.list.length > 0) {
        openPopup(res.list, res.from, res.to);
      } else {
        closePopup();
      }
    }

    cm.on("inputRead", (_cm, change) => {
      if (!enabled) return;
      if (change.origin === "+complete" || change.origin === "setValue" || change.origin === "paste") {
        closePopup();
        return;
      }
      const ch = change.text && change.text[0];
      if (!ch) return;
      if (/^[a-zA-Z0-9_.:>#]$/.test(ch)) {
        clearTimeout(debounceTimer);
        debounceTimer = setTimeout(() => trigger(false), 40);
      } else {
        closePopup();
      }
    });

    cm.on("cursorActivity", () => {
      if (!isOpen()) return;
      // Update or close if cursor moved away from token
      const res = collectSuggestions(false);
      if (res && res.list.length > 0 && res.from.line === replaceFrom?.line) {
        openPopup(res.list, res.from, res.to);
      } else {
        closePopup();
      }
    });

    cm.on("scroll", () => {
      if (isOpen()) positionPopup();
    });

    cm.on("blur", () => {
      setTimeout(() => {
        if (!cm.hasFocus()) closePopup();
      }, 120);
    });

    cm.addKeyMap({
      "Ctrl-Space": () => trigger(true),
    });

    return {
      trigger: () => trigger(true),
      close: closePopup,
      setEnabled(val) {
        enabled = Boolean(val);
        if (!enabled) closePopup();
      },
      isEnabled: () => enabled,
    };
  }

  global.attachCppAutocomplete = attachCppAutocomplete;
})(window);

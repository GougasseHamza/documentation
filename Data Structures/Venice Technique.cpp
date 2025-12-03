/**
 * Venice Technique (Global Add Priority Queue)
 * * * PROBLEM:
 * Maintain a set of values with operations:
 * 1. Push(x)
 * 2. Pop Min
 * 3. Add V to ALL elements currently in the set.
 *
 * * * LOGIC:
 * Instead of updating all elements, we maintain a global 'sum'.
 * - Real Value = Stored Value + sum.
 * - When Pushing 'x', we store 'x - sum' so that (x - sum) + sum = x.
 * - When Popping, we return 'top + sum'.
 */


#include <bits/stdc++.h>
using namespace std; 
 template<typename T> struct PQ {
  long long sum = 0; // The global add value
  priority_queue<T, vector<T>, greater<T>> Q; // Min-Heap

  void push(T x) { 
      // Store relative value
      x.w -= sum; 
      Q.push(x); 
  }
  
  T pop() { 
      auto ans = Q.top(); Q.pop(); 
      // Reconstruct real value
      ans.w += sum; 
      return ans; 
  }
  
  void add(long long x) { sum += x; } // Update global adder
  
  int size() { return Q.size(); }
  
  // Merge another PQ into this one (Small-to-Large)
  void merge(PQ &&x) {
    if (size() < x.size()) swap(sum, x.sum), swap(Q, x.Q);
    while (x.size()) {
      auto tmp = x.pop(); // Pop real value from x
      push(tmp);          // Push real value to this (handles sum logic internaly)
    }
  }
};
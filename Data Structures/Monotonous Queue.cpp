/**
 * Monotonous Queue (Max Queue)
 *
 * * PROBLEMS SOLVED:
 * 1. Find the Maximum (or Minimum) element in a sliding window.
 * 2. DP optimizations where transition involves max/min over a range.
 *
 * * COMPLEXITY:
 * Time: O(1) Amortized per push/pop. Each element enters and leaves once.
 * Space: O(N).
 *
 * * LOGIC:
 * Maintains a list of elements that are candidates for being the maximum.
 * If a new element is larger than existing elements at the back, those existing
 * elements can never be the maximum again (since the new one is larger AND newer).
 * So we pop them from the back.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 3e5 + 9;
 
 struct monotonous_queue { 
   // 'a' stores values. 'b' stores counts (how many elements were "flattened" into this one).
   // Actually, 'b' here logic is slightly non-standard. Usually we store (value, index).
   // This implementation stores:
   // a[i]: The value.
   // b[i]: How many "popped" elements this value effectively replaced + 1 (itself).
   // This allows tracking window size implicitly without indices.
   
   int a[N + 10], b[N + 10];
   int l = 0, r = -1; // Pointers: l=front, r=back
 
   void push(int val) {
     int cnt = 0;
     // While current val >= back of queue, pop back
     while(l <= r && a[r] <= val) {
       cnt += b[r] + 1; // Accumulate the "width" of popped elements
       r--;
     }
     // Push new max candidate
     a[++r] = val; 
     b[r] = cnt; // Store how many elements this value covers
   };
 
   int top() {
     return a[l]; // Front is always the Max
   }
 
   void pop() {
     if(l > r) return;
     // If the front element represents multiple original pushes (cnt > 0), decrement count.
     if (b[l] > 0) {
       b[l] --;
       return;
     }
     // Else, physically remove it.
     l++;
   }
 };
 
 int32_t main() {
   monotonous_queue mq;
   mq.push(10);
   mq.push(5);
   cout << mq.top() << endl; // 10
   mq.pop();
   cout << mq.top() << endl; // 5
   return 0;
 }
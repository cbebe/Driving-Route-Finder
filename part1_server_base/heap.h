#include <vector>
#include <algorithm>

// Only assumes the key type K is totally ordered and comparable via <

template <class T, class K>
struct HeapItem {
  T item;
  K key;
};

template <class T, class K>
class BinaryHeap {
public:
  // constructor not required because the only "initialization"
  // is through the constructor of the variable "heap" which is called by default

  // return the minimum element in the heap
  HeapItem<T, K> min() const;

  // insert an item with the given key
  // if the item is already in the heap, will still insert a new copy with this key
  void insert(const T& item, const K& key);

  // pop the minimum item from the heap
  void popMin();

  // returns the number of items held in the heap
  int size() const;

private:
  // the array holding the heap
  std::vector< HeapItem<T, K> > heap;

  //  will fix the heap property at index i and recurse with its parent
  void fixHeapUp(int i);

  // will fix the heap property at index i and recurse with the child
  // that received i's item (if appropriate)
  void fixHeapDown(int i);
};

template <class T, class K>
HeapItem<T, K> BinaryHeap<T, K>::min() const {
  return heap[0];
}

template <class T, class K>
void BinaryHeap<T, K>::insert(const T& item, const K& key) {
  HeapItem<T, K> node = {item, key};

  // add the new item to the end of the heap
  heap.push_back(node);
  // fix the heap property
  fixHeapUp(heap.size()-1);
}

template <class T, class K>
void BinaryHeap<T, K>::popMin() {
  // move the last item of the last layer to the top
  // if the heap has size 1, this just pops it
  heap[0] = heap.back();
  heap.pop_back();

  // if there is anything left in the heap, fix the heap property
  if (heap.size() > 0) {
    fixHeapDown(0);
  }
}

template <class T, class K>
int BinaryHeap<T, K>::size() const {
  return heap.size();
}

template <class T, class K>
void BinaryHeap<T, K>::fixHeapUp(int i) {
  while (i > 0) {
    int pi = (i-1)>>1; // parent index

    // if i's key is smaller than its parent's key, swap it and go up
    if (heap[i].key < heap[pi].key) {
      std::swap(heap[i], heap[pi]);
      i = pi;
    }
    else {
      // otherwise, no more fixing needs to be done
      return;
    }
  }
}

template <class T, class K>
void BinaryHeap<T, K>::fixHeapDown(int i) {
  while (true) {
    // calculate indices of the two children
    int lchild = (i<<1)+1, rchild = (i<<1)+2;

    // if no children, no problem
    if (lchild >= heap.size()) {
      return;
    }

    int min_child;
    // identify the child with the minimum key, being careful
    // to handle the case where there is no right child
    if (rchild >= heap.size() || heap[lchild].key <= heap[rchild].key) {
      min_child = lchild;
    }
    else {
      min_child = rchild;
    }

    // if there is a violation of the heap property for i, swap its node
    // with the node held by the minimum-key child and repeat with this child
    if (heap[min_child].key < heap[i].key) {
      std::swap(heap[i], heap[min_child]);
      i = min_child;
    }
    else {
      return;
    }
  }
}

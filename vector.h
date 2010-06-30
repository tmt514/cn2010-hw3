#ifndef _VECTOR_H
#define _VECTOR_H
template<class T>
class vector {
public:
  vector() {n = 0;}
  void clear() {n = 0;}
  unsigned size() {return n;}
  void fill(const T& v) {
    for (int i = 0; i < n; ++i)
      s[i] = v;
  }
  void resize(unsigned m) {n = m;}
  T& operator[](unsigned i) {return s[i];}
  void push_back(const T& v) {s[n++] = v;}
private:  
  unsigned n;
  T s[768];
};
#endif

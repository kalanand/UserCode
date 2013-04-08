#ifndef _INSET_HH
#define _INSET_HH

#include <set>

template<class T>
inline bool notInSet(std::set<T>& aset, T anum) {
  return (aset.find(anum) == aset.end());
}

template<class T>
inline bool inSet(std::set<T>& aset, T anum) {
  return (aset.find(anum) != aset.end());
}

#endif // _INSET_HH

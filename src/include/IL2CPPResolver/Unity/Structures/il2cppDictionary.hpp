#pragma once
#include "il2cppArray.hpp"
#include <map>
#include <vector>
namespace Unity {
template <typename TKey, typename TValue>
struct il2cppDictionary : il2cppObject {
  struct Entry {
    int hashCode{}, next{};
    TKey key{};
    TValue value{};
  };
  il2cppArray<int> *buckets{};
  il2cppArray<Entry> *entries{};
  int count{};
  int version{};
  int freeList{};
  int freeCount{};
  void *comparer{};
  il2cppArray<TKey> *keys{};
  il2cppArray<TValue> *values{};
  void *syncRoot{};

  /**
      @brief Convert to std::map.
      @return Map of elements
  */
  std::map<TKey, TValue> ToMap() const {
    std::map<TKey, TValue> ret{};
    for (auto it = (Entry *)&entries->m_pValues;
         it != ((Entry *)&entries->m_pValues + count); ++it)
      ret.emplace(std::make_pair(it->key, it->value));
    return ret;
  }

  /**
      @brief Convert to std::vector.
      @return Vector of keys
  */
  std::vector<TKey> GetKeys() const {
    std::vector<TKey> ret{};
    for (int i = 0; i < count; ++i)
      ret.emplace_back(entries->m_pValues[i].key);
    return ret;
  }

  /**
      @brief Convert to std::vector.
      @return Vector of values
  */
  std::vector<TValue> GetValues() const {
    std::vector<TValue> ret{};
    for (int i = 0; i < count; ++i)
      ret.emplace_back(entries->m_pValues[i].value);
    return ret;
  }
};
} // namespace Unity

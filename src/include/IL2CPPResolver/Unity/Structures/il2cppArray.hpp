#pragma once

#include "IL2CPPResolver/Unity/Structures/il2cpp.hpp"
#include <cstring> // For memcpy
#include <vector>  // For std::vector

namespace Unity {
template <typename T> struct il2cppArray : il2cppObject {
  il2cppArrayBounds *m_pBounds = nullptr;
  uintptr_t m_uMaxLength = 0;
  T m_Items[1];

  T *GetData() {
    // 正确的方式是返回 m_Items 成员的地址
    return &m_Items[0];
  }

  // 获取数组数据区的起始地址 (const 版本)
  const T *GetData() const { return &m_Items[0]; }

  T &operator[](unsigned int m_uIndex) {
    // 直接通过 GetData() 返回的指针进行索引
    return GetData()[m_uIndex];
  }

  const T &operator[](unsigned int m_uIndex) const {
    return GetData()[m_uIndex];
  }

  T &At(unsigned int m_uIndex) { return operator[](m_uIndex); }

  const T &At(unsigned int m_uIndex) const { return operator[](m_uIndex); }

  void Insert(const T *m_pArray, uintptr_t m_uSize, uintptr_t m_uIndex = 0) {
    if (!m_pArray || m_uSize == 0 || m_uIndex >= m_uMaxLength) {
      return;
    }

    // 计算可以安全复制的元素数量
    uintptr_t copyCount = m_uSize;
    if ((m_uIndex + m_uSize) > m_uMaxLength) {
      copyCount = m_uMaxLength - m_uIndex;
    }

    // 使用 memcpy 进行高效的块复制
    memcpy(GetData() + m_uIndex, m_pArray, copyCount * sizeof(T));
  }

  void Fill(const T &m_tValue) {
    for (uintptr_t u = 0; u < m_uMaxLength; ++u) {
      operator[](u) = m_tValue;
    }
  }

  // 注意：Il2Cpp 数组的大小是固定的，通常不能像 std::vector 那样 "移除"
  // 元素来缩小。 以下方法将通过移动元素来模拟移除，但数组的 m_uMaxLength
  // 不会改变。 如果你想改变大小，你需要创建一个新的、更小的数组并复制数据。
  // 这里我将保留移动元素的逻辑，但不会修改 m_uMaxLength。

  void RemoveAt(unsigned int m_uIndex) {
    if (m_uIndex >= m_uMaxLength) {
      return;
    }

    // 将后面的元素向前移动，覆盖掉要删除的元素
    if (m_uIndex < m_uMaxLength - 1) {
      memmove(GetData() + m_uIndex, GetData() + m_uIndex + 1,
              (m_uMaxLength - m_uIndex - 1) * sizeof(T));
    }
    // 通常，你可能想将最后一个元素清零
    // memset(GetData() + m_uMaxLength - 1, 0, sizeof(T));
    // 注意：不应该修改 m_uMaxLength，因为数组的分配大小是固定的。
  }

  void RemoveRange(unsigned int m_uIndex, unsigned int m_uCount) {
    if (m_uCount == 0 || m_uIndex >= m_uMaxLength) {
      return;
    }

    unsigned int m_uEndIndex = m_uIndex + m_uCount;
    if (m_uEndIndex > m_uMaxLength) {
      m_uEndIndex = m_uMaxLength;
    }

    // 将范围之后的元素向前移动
    if (m_uEndIndex < m_uMaxLength) {
      memmove(GetData() + m_uIndex, GetData() + m_uEndIndex,
              (m_uMaxLength - m_uEndIndex) * sizeof(T));
    }
    // 注意：同样不应该修改 m_uMaxLength。
  }

  void RemoveAll() {
    if (m_uMaxLength > 0) {
      // 只是将所有元素清零，而不是改变大小
      memset(GetData(), 0, sizeof(T) * m_uMaxLength);
    }
    // m_uMaxLength = 0; // 不应该修改这个值！
  }

  // --- 修正后的静态 Create 方法 ---

  static il2cppArray<T> *Create(il2cppClass *elementClass, uintptr_t capacity) {
    if (!elementClass) {
      return nullptr;
    }
    // 保持你原来的、正确的调用方式
    return reinterpret_cast<il2cppArray<T> *>(elementClass->NewArray(capacity));
  }

  static il2cppArray<T> *Create(il2cppClass *elementClass,
                                const std::vector<T> &vec) {
    il2cppArray<T> *newArray = Create(elementClass, vec.size());
    if (newArray && !vec.empty()) {
      // 关键修正：使用 GetData() 来获取正确的目标地址
      memcpy(newArray->GetData(), vec.data(), vec.size() * sizeof(T));
    }
    return newArray;
  }

  static il2cppArray<T> *Create(il2cppClass *elementClass, T *arr,
                                uintptr_t size) {
    il2cppArray<T> *newArray = Create(elementClass, size);
    if (newArray && arr && size > 0) {
      // 关键修正：使用 GetData() 来获取正确的目标地址
      memcpy(newArray->GetData(), arr, size * sizeof(T));
    }
    return newArray;
  }
};

// Defined here because its basically same shit
template <typename T> struct il2cppList : il2cppObject {
  il2cppArray<T> *m_pListArray;

  il2cppArray<T> *ToArray() { return m_pListArray; }
};
} // namespace Unity
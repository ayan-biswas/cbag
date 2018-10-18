#ifndef CBAG_UTIL_VECTOR_MAP_H
#define CBAG_UTIL_VECTOR_MAP_H

#include <functional>
#include <map>
#include <utility>

#include <cbag/util/sorted_vector.h>

namespace cbag {
namespace util {

namespace detail {
template <class Key, class T, typename Comp> class KeyCompareBase {
  public:
    using value_type = std::pair<Key, T>;

    Comp comp_;

    constexpr bool operator()(const value_type &lhs, const value_type &rhs) const {
        return comp_(lhs.first, rhs.first);
    }

    constexpr bool operator()(const Key &lhs, const value_type &rhs) const {
        return comp_(lhs, rhs.first);
    }

    constexpr bool operator()(const value_type &lhs, const Key &rhs) const {
        return comp_(lhs.first, rhs);
    }
};

template <class Key, class T, typename Comp, typename = void>
class KeyCompare : public KeyCompareBase<Key, T, Comp> {};

template <class Key, class T, typename Comp>
class KeyCompare<Key, T, Comp, typename Comp::is_transparent>
    : public KeyCompareBase<Key, T, Comp> {
  public:
    using value_type = typename KeyCompareBase<Key, T, Comp>::value_type;

    template <class K> constexpr bool operator()(const K &lhs, const value_type &rhs) const {
        return comp_(lhs, rhs.first);
    }

    template <class K> constexpr bool operator()(const value_type &lhs, const K &rhs) const {
        return comp_(lhs.first, rhs);
    }
};

} // namespace detail

template <class Key, class T, class Compare = std::less<Key>> class vector_map {
  public:
    using value_type = std::pair<Key, T>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using compare_type = detail::KeyCompare<Key, T, Compare>;
    using vector_type = sorted_vector<value_type, compare_type>;
    using size_type = typename vector_type::size_type;
    using iterator = typename vector_type::iterator;
    using const_iterator = typename vector_type::const_iterator;
    using difference_type = typename vector_type::difference_type;

  private:
    vector_type data_;

  public:
    vector_map() = default;

    size_type size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
    const_iterator begin() const { return data_.begin(); }
    const_iterator end() const { return data_.end(); }
    const_reference at_front() const { return data_.at_front(); }
    const_reference at_back() const { return data_.at_back(); }
    const compare_type &get_compare() const { return data_.get_compare(); }

    template <class K> const_iterator find(const K &x) const {
        auto iter_range = data_.equal_range(x);
        if (iter_range.first == iter_range.second)
            return data_.end();
        return iter_range.first;
    }

    const_iterator find_exact(const Key &x) const {
        auto iter = data_.lower_bound(x);
        auto end = data_.end();
        return (iter == end) ? end : (iter->first == x) ? iter : end;
    }

    template <class K> difference_type equal_size(const K &x) const {
        auto iter_range = data_.equal_range(x);
        return iter_range.second - iter_range.first;
    }

    template <class K> std::pair<const_iterator, const_iterator> equal_range(const K &x) const {
        return data_.equal_range(x);
    }

    template <class K> std::pair<iterator, iterator> equal_range(const K &x) {
        return data_.equal_range(x);
    }

    template <class... Args> std::pair<iterator, bool> emplace(Args &&... args) {
        return data_.emplace_unique(std::forward<Args>(args)...);
    }

    std::pair<iterator, bool> insert(value_type &&val) {
        return data_.insert_unique(std::move(val));
    }

    iterator erase(const_iterator pos) { return data_.erase(std::move(pos)); }

    iterator erase(const_iterator first, const_iterator last) {
        return data_.erase(std::move(first), std::move(last));
    }
};

} // namespace util
} // namespace cbag

#endif
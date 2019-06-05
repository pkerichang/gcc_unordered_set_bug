#include <iostream>
#include <iterator>
#include <optional>
#include <unordered_set>
#include <vector>

class input_iter;

class resource {
private:
  int cur_;
  int stop_;

public:
  resource(int start, int stop) : cur_(start), stop_(stop) {}

  int get() const { return cur_; }

  void next() { ++cur_; }

  bool done() { return cur_ == stop_; }

  input_iter begin();
  input_iter end() const;
};

class input_iter {
public:
  using iterator_category = std::input_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = int;
  using reference = const value_type &;
  using pointer = const value_type *;

private:
  resource *ptr_ = nullptr;
  std::optional<int> val_;

public:
  input_iter() = default;

  explicit input_iter(resource *source) : ptr_(source) {}

  reference operator*() const {
    const_cast<input_iter &>(*this)._eval();
    return *val_;
  }

  input_iter &operator++() {
    if (ptr_) {
      if (ptr_->done()) {
        ptr_ = nullptr;
        val_.reset();
      } else {
        val_ = ptr_->get();
        ptr_->next();
      }
    }
    return *this;
  }

  bool operator==(const input_iter &rhs) const {
    const_cast<input_iter &>(*this)._eval();
    const_cast<input_iter &>(rhs)._eval();
    return ptr_ == rhs.ptr_;
  }

  bool operator!=(const input_iter &rhs) const { return !(*this == rhs); }

private:
  void _eval() {
    if (ptr_ && !val_) {
      if (ptr_->done()) {
        ptr_ = nullptr;
      } else {
        val_ = ptr_->get();
        ptr_->next();
      }
    }
  }
};

input_iter resource::begin() { return input_iter(this); }

input_iter resource::end() const { return input_iter(); }

int main(int argc, char *argv[]) {

  resource src1(0, 6);
  resource src2(0, 6);

  std::vector<int> vec(src1.begin(), src1.end());
  std::cout << "vector values:" << std::endl;
  for (const auto &v : vec) {
    std::cout << v << ", ";
  }
  std::cout << std::endl;

  std::unordered_set<int> set(src2.begin(), src2.end());
  std::cout << "unordered_set values:" << std::endl;
  for (const auto &v : set) {
    std::cout << v << ", ";
  }
  std::cout << std::endl;

  return 0;
}

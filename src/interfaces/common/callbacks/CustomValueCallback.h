#pragma once

#include <iostream>

template <typename Fn, typename T>
class CustomValueCallback {
private:
protected:
  Fn fn;
  T value;
public:
  CustomValueCallback():
      fn(), value() {
  }

  CustomValueCallback(Fn fn, T value):
      fn(fn), value(value) {
  }

  void setCallback(Fn fn, T value) {
    this->fn=fn;
    this->value=value;
  }

  template <typename R, typename... Args>
  R call(Args... args) {
    return fn(value, args...);
  }

  template <typename... Args>
  void call(Args... args) {
    fn(value, args...);
  }

  template <typename... Args>
  void callExceptIfNotSet(Args... args) {
    if (fn != nullptr) {
      fn(value, args...);
    }
  }


};
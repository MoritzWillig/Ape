#pragma once

#include <stdexcept>

//FIXME Signal as interface - this as RWSignal
class Signal {
private:
  bool signal;
protected:
public:
  Signal():
      signal(false) {
  }

  Signal(bool signal):
      signal(signal) {
  }

  void set() {
    set(true);
  }

  virtual void set(bool signal) {
    this->signal=signal;
  }

  void reset() {
    set(false);
  }

  bool hasSignal() const {
    return signal;
  }

  explicit operator bool() const {
    return hasSignal();
  }
};

//FIXME see above - use SignalInterface
template <typename T>
class ValueSignal: public Signal {
protected:
  T value;
public:
  ValueSignal():
      Signal() {
  }

  ValueSignal(T value):
      ValueSignal() {
    setValue(value);
  }

  void setValue(T value) {
    this->set();
    this->value = value;
  }

  T getValue() const {
    if (!hasSignal()) {
      throw std::runtime_error("Signal not set");
    }

    return value;
  }
};

template <typename T>
class SignalOrDefault {
private:
  T defaultValue;

  const Signal* signal;
  T value;
protected:
public:
  SignalOrDefault(const Signal* signal, T defaultValue):
      defaultValue(defaultValue), signal(signal), value() {
  }

  SignalOrDefault(Signal* signal):
      defaultValue(), signal(signal), value() {
  }

  void setValue(T value) {
    this->value=value;
  }

  T get() const {
    return (*signal)?value:defaultValue;
  }

  bool hasSignal() {
    return signal->hasSignal();
  }

  explicit operator T() const {
    return get();
  }
};
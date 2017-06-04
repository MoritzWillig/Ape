#pragma once
//
// Created by moritz on 03.06.17.
//

class Signal {
private:
  bool signal;
protected:
public:
  Signal():
      signal(true) {
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
    signal=false;
  }

  bool hasSignal() const {
    return signal;
  }

  explicit operator bool() const {
    return hasSignal();
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
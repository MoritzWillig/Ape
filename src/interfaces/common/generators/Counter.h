#pragma once

#include "./Generator.h"

class Counter: public Generator<int> {
private:
protected:
  int counter;
public:
  //Default
  Counter();

  Counter(int startingIndex);

  // Copy constructor
  Counter(const Counter&) = default;

  // Move constructor
  Counter(Counter&&) = default;

  // Copy assignment operator
  Counter& operator=(const Counter&)& = default;

  // Move assignment operator
  Counter& operator=(Counter&&)& = default;

  // Destructor
  virtual ~Counter() = default;

  virtual int getNew() override;

  virtual int getCurrent() override;
};



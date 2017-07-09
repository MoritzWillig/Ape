//
// Created by moritz on 09.07.17.
//

#include "Counter.h"

Counter::Counter():
    Counter(0) {
}

Counter::Counter(int startIndex):
  counter(startIndex) {
}

int Counter::getNew() {
  counter++;
  return getCurrent();
}

int Counter::getCurrent() {
  return counter;
}

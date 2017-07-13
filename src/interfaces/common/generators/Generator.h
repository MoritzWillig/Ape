#pragma once

template <typename T>
class Generator {
private:
protected:
public:
  virtual T getNew() = 0;
  virtual T getCurrent() = 0;
};
#pragma once

#include "ISmoother.h"

#include <list>

namespace ape {
  namespace imageProcessing {

    template <typename T>
    class MeanSmoother: public ISmoother<T,T> {
    private:
      int storedValuesCount;
    protected:
      std::list<T> history;
    public:
      //Default
      MeanSmoother() = delete;

      MeanSmoother(int storedValuesCount):
          storedValuesCount(storedValuesCount), history() {
      }

      // Copy constructor
      MeanSmoother(const MeanSmoother&) = default;

      // Move constructor
      MeanSmoother(MeanSmoother&&) = default;

      // Copy assignment operator
      MeanSmoother& operator=(const MeanSmoother&)& = default;

      // Move assignment operator
      MeanSmoother& operator=(MeanSmoother&&)& = default;

      // Destructor
      virtual ~MeanSmoother() = default;


      virtual void recordValue(T value) override {
        if (history.size()==storedValuesCount) {
          history.pop_back();
        }

        history.emplace_back(value);
      }

      virtual T getSmoothedValue() const override {
        T value;
        for (auto v: history) {
          value+=(v/(float)storedValuesCount);
        }
        return value;
      }


      virtual void clearHistory() override {
        history.clear();
      }
    };

  }
}


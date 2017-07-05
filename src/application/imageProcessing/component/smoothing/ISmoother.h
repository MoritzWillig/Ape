#pragma once

namespace ape {
  namespace imageProcessing {

    template<typename Input, typename Output>
    class ISmoother {
    private:
    protected:
    public:
      //Default
      ISmoother() = default;

      // Copy constructor
      ISmoother(const ISmoother&) = default;

      // Move constructor
      ISmoother(ISmoother&&) = default;

      // Copy assignment operator
      ISmoother&
      operator=(const ISmoother&)& = default;

      // Move assignment operator
      ISmoother& operator=(ISmoother&&)& = default;

      // Destructor
      virtual ~ISmoother() = default;

      virtual void recordValue(Input value) = 0;

      virtual Output getSmoothedValue() const = 0;

      virtual void clearHistory() = 0;
    };

  }
}

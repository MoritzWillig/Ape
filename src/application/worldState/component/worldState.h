#pragma once

#include <memory>

namespace ape {
  namespace worldState {

    class IWorldStateController {
    private:
    protected:
    public:
      virtual void update(float timeStep) = 0;

      static std::shared_ptr<IWorldStateController> createInstance();
    };
  }
}
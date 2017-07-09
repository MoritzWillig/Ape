#pragma once

#include <vector>
#include <memory>

namespace ape {
  namespace visualization {

    class IVisualModel {
    private:
    protected:
    public:
      typedef int VisualModelHandle;

      virtual std::vector<std::shared_ptr<IVisualModel>> const * getSubEntities() = 0;

      virtual VisualModelHandle getHandle() = 0;
    };

  }
}
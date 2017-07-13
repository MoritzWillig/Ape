
#include "OgreVisualModel.h"

namespace ape {
  namespace visualization {

    OgreVisualModel::OgreVisualModel(VisualModelHandle handle):
        handle(handle), subEntities() {
    }

    std::vector<std::shared_ptr<IVisualModel>> const *
    OgreVisualModel::getSubEntities() {
      return &subEntities;
    }

    IVisualModel::VisualModelHandle OgreVisualModel::getHandle() {
      return handle;
    }

    void
    OgreVisualModel::addSubEntity(std::shared_ptr<IVisualModel> visualModel) {
      subEntities.emplace_back(visualModel);
    }

  }
}
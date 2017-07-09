#pragma once

#include <visualization/VisualModel.h>

namespace ape {
  namespace visualization {

    class OgreVisualModel: public IVisualModel {
    private:
      VisualModelHandle handle;
    protected:
      std::vector<std::shared_ptr<IVisualModel>> subEntities;
    public:
      //Default
      OgreVisualModel() = delete;

      OgreVisualModel(VisualModelHandle handle);

      // Copy constructor
      OgreVisualModel(const OgreVisualModel&) = default;

      // Move constructor
      OgreVisualModel(OgreVisualModel&&) = default;

      // Copy assignment operator
      OgreVisualModel& operator=(const OgreVisualModel&)& = default;

      // Move assignment operator
      OgreVisualModel& operator=(OgreVisualModel&&)& = default;

      // Destructor
      virtual ~OgreVisualModel() = default;

      virtual std::vector<std::shared_ptr<IVisualModel>> const *
        getSubEntities() override;

      virtual VisualModelHandle getHandle() override;

      void addSubEntity(std::shared_ptr<IVisualModel> visualModel);
    };

  }
}

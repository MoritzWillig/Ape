#pragma once
//
// Created by moritz on 16.05.17.
//


namespace ape {
  namespace worldState {

    class World {
    private:
    protected:
    public:
      //Default
      World() = default;

      // Copy constructor
      World(const World&) = default;

      // Move constructor
      World(World&&) = default;

      // Copy assignment operator
      World& operator=(const World&)& = default;

      // Move assignment operator
      World& operator=(World&&)& = default;

      // Destructor
      virtual ~World() = default;
    };

  }
}

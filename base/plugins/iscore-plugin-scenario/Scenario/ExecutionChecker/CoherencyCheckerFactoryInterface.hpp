#pragma once

#include <iscore/plugins/customfactory/FactoryInterface.hpp>
#include <iscore/plugins/customfactory/StringFactoryKey.hpp>
#include <iscore_plugin_scenario_export.h>

#include "CSPCoherencyCheckerInterface.hpp"
namespace iscore
{
struct ApplicationContext;
}
namespace Scenario
{
class ISCORE_PLUGIN_SCENARIO_EXPORT CoherencyCheckerFactoryInterface
    : public iscore::Interface<CoherencyCheckerFactoryInterface>
{
  ISCORE_INTERFACE("e9942ad6-1e39-4bdf-bb93-f31962e3cf79")

public:
  virtual CSPCoherencyCheckerInterface* make(
      Scenario::ProcessModel& scenario,
      const iscore::ApplicationContext& ctx,
      Scenario::ElementsProperties& elementsProperties)
      = 0;
  virtual ~CoherencyCheckerFactoryInterface();
};
}

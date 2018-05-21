// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "BaseScenario.hpp"

#include <ossia/detail/algorithms.hpp>

#include <QString>
#include <Scenario/Commands/MoveBaseEvent.hpp>
#include <Scenario/Document/BaseScenario/BaseScenarioContainer.hpp>
#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Document/Interval/IntervalModel.hpp>
#include <Scenario/Document/State/StateModel.hpp>
#include <Scenario/Document/TimeSync/TimeSyncModel.hpp>
#include <score/command/Dispatchers/CommandDispatcher.hpp>
#include <score/document/DocumentInterface.hpp>
#include <score/model/Identifier.hpp>
#include <score/selection/Selection.hpp>
#include <score/tools/std/Optional.hpp>
#include <Process/TimeValueSerialization.hpp>
#include <Scenario/Commands/MoveBaseEvent.hpp>
#include <tuple>

#include <wobjectimpl.h>
W_OBJECT_IMPL(Scenario::BaseScenario)
namespace Scenario
{
BaseScenario::BaseScenario(const Id<BaseScenario>& id, QObject* parent)
    : IdentifiedObject<BaseScenario>{id, "Scenario::BaseScenario", parent}
    , BaseScenarioContainer{this}
{
  m_endNode->setActive(true);
}

BaseScenario::~BaseScenario()
{
}

Selection BaseScenario::selectedChildren() const
{
  Selection s;
  ossia::for_each_in_tuple(elements(), [&](auto elt) {
    if (elt->selection.get())
      s.append(elt);
  });
  return s;
}

bool BaseScenario::focused() const
{
  bool res = false;
  ossia::for_each_in_tuple(elements(), [&](auto elt) {
    if (elt->selection.get())
    {
      res = true;
    }
  });

  return res;
}

void BaseScenario::changeDuration(IntervalModel& , const TimeVal& v)
{
  Command::MoveBaseEvent<BaseScenario> cmd(
      *this, endEvent().id(), v, 0., ExpandMode::GrowShrink, LockMode::Free);
  cmd.redo(score::IDocument::documentContext(*this));
}

void BaseScenario::changeDuration(const Scenario::IntervalModel& itv,
                                  OngoingCommandDispatcher& dispatcher,
                                  const TimeVal& val,
                                  ExpandMode expandmode,
                                  LockMode lockmode)
{
  auto& scenario = *this;
  dispatcher.submitCommand<Command::MoveBaseEvent<BaseScenario>>(
      scenario, scenario.endEvent().id(), val, 0., expandmode, lockmode);
}

const QVector<Id<IntervalModel>> intervalsBeforeTimeSync(
    const BaseScenario& scen, const Id<TimeSyncModel>& timeSyncId)
{
  if (timeSyncId == scen.endTimeSync().id())
  {
    return {scen.interval().id()};
  }
  return {};
}
}

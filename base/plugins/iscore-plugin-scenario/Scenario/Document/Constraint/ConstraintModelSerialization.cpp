#include <Process/Process.hpp>

#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Scenario/Document/Constraint/Rack/RackModel.hpp>
#include <Scenario/Document/Constraint/ViewModels/FullView/FullViewConstraintViewModel.hpp>

#include <iscore/tools/std/Optional.hpp>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <algorithm>
#include <sys/types.h>

#include <Process/ProcessList.hpp>
#include <Process/TimeValue.hpp>
#include <iscore/application/ApplicationContext.hpp>
#include <iscore/model/ModelMetadata.hpp>
#include <iscore/plugins/customfactory/StringFactoryKey.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONValueVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
#include <iscore/model/EntityMap.hpp>
#include <iscore/model/Identifier.hpp>

namespace Scenario
{
class StateModel;
}

template <typename T>
class IdentifiedObject;
template <typename T>
class Reader;
template <typename T>
class Writer;

// Note : comment gérer le cas d'un process shared model qui ne sait se
// sérializer qu'en binaire, dans du json?
// Faire passer l'info en base64 ?

template <>
ISCORE_PLUGIN_SCENARIO_EXPORT void DataStreamReader::read(
    const Scenario::ConstraintModel& constraint)
{
  // Processes
  m_stream << (int32_t)constraint.processes.size();
  for (const auto& process : constraint.processes)
  {
    readFrom(process);
  }

  // Rackes
  m_stream << (int32_t)constraint.racks.size();
  for (const auto& rack : constraint.racks)
  {
    readFrom(rack);
  }

  // Full view
  readFrom(*constraint.m_fullViewModel);

  // Common data
  m_stream << constraint.duration << constraint.m_startState
           << constraint.m_endState

           << constraint.m_startDate << constraint.m_heightPercentage
           << constraint.m_looping;

  insertDelimiter();
}


template <>
ISCORE_PLUGIN_SCENARIO_EXPORT void
DataStreamWriter::writeTo(Scenario::ConstraintModel& constraint)
{
  // Processes
  int32_t process_count;
  m_stream >> process_count;

  auto& pl = components.interfaces<Process::ProcessFactoryList>();
  for (; process_count-- > 0;)
  {
    auto proc = deserialize_interface(pl, *this, &constraint);
    if (proc)
    {
      // TODO why isn't AddProcess used here ?!
      constraint.processes.add(proc);
    }
    else
    {
      ISCORE_TODO;
    }
  }

  // Rackes
  int32_t rack_count;
  m_stream >> rack_count;

  for (; rack_count-- > 0;)
  {
    constraint.racks.add(new Scenario::RackModel(*this, &constraint));
  }

  constraint.setFullView(new Scenario::FullViewConstraintViewModel{
      *this, constraint, &constraint});

  // Common data
  m_stream >> constraint.duration >> constraint.m_startState
      >> constraint.m_endState

      >> constraint.m_startDate >> constraint.m_heightPercentage
      >> constraint.m_looping;

  checkDelimiter();
}


template <>
ISCORE_PLUGIN_SCENARIO_EXPORT void JSONObjectReader::readFrom(
    const Scenario::ConstraintModel& constraint)
{
  readFrom(static_cast<const iscore::Entity<Scenario::ConstraintModel>&>(
      constraint));

  // Processes
  obj["Processes"] = toJsonArray(constraint.processes);

  // Rackes
  obj["Rackes"] = toJsonArray(constraint.racks);

  // Full view
  obj["FullView"] = toJsonObject(*constraint.fullView());

  // Common data
  // The fields will go in the same level as the
  // rest of the constraint
  readFrom(constraint.duration);

  obj["StartState"] = toJsonValue(constraint.m_startState);
  obj["EndState"] = toJsonValue(constraint.m_endState);

  obj["StartDate"] = toJsonValue(constraint.m_startDate);
  obj["HeightPercentage"] = constraint.m_heightPercentage;
  obj["Looping"] = constraint.m_looping;
}


template <>
ISCORE_PLUGIN_SCENARIO_EXPORT void
JSONObjectWriter::writeTo(Scenario::ConstraintModel& constraint)
{
  auto& pl = components.interfaces<Process::ProcessFactoryList>();

  QJsonArray process_array = obj["Processes"].toArray();
  for (const auto& json_vref : process_array)
  {
    JSONObject::Deserializer deserializer{json_vref.toObject()};
    auto proc = deserialize_interface(pl, deserializer, &constraint);
    if (proc)
      constraint.processes.add(proc);
    else
      ISCORE_TODO;
  }

  QJsonArray rack_array = obj["Rackes"].toArray();
  for (const auto& json_vref : rack_array)
  {
    JSONObject::Deserializer deserializer{json_vref.toObject()};
    constraint.racks.add(new Scenario::RackModel(deserializer, &constraint));
  }

  constraint.setFullView(new Scenario::FullViewConstraintViewModel{
      JSONObject::Deserializer{obj["FullView"].toObject()}, constraint,
      &constraint});

  writeTo(constraint.duration);
  constraint.m_startState
      = fromJsonValue<Id<Scenario::StateModel>>(obj["StartState"]);
  constraint.m_endState
      = fromJsonValue<Id<Scenario::StateModel>>(obj["EndState"]);

  constraint.m_startDate = fromJsonValue<TimeValue>(obj["StartDate"]);
  constraint.m_heightPercentage = obj["HeightPercentage"].toDouble();
  constraint.m_looping = obj["Looping"].toBool();
}

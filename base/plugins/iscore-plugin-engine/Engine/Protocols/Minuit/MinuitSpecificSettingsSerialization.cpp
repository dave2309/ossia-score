#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>

#include "MinuitSpecificSettings.hpp"


template <>
void DataStreamReader::read(
    const Engine::Network::MinuitSpecificSettings& n)
{
  m_stream << n.host << n.inputPort << n.outputPort;
  insertDelimiter();
}


template <>
void DataStreamWriter::writeTo(
    Engine::Network::MinuitSpecificSettings& n)
{
  m_stream >> n.host >> n.inputPort >> n.outputPort;
  checkDelimiter();
}


template <>
void JSONObjectReader::readFromConcrete(
    const Engine::Network::MinuitSpecificSettings& n)
{
  obj["InPort"] = n.inputPort;
  obj["OutPort"] = n.outputPort;
  obj["Host"] = n.host;
}


template <>
void JSONObjectWriter::writeTo(
    Engine::Network::MinuitSpecificSettings& n)
{
  n.inputPort = obj["InPort"].toInt();
  n.outputPort = obj["OutPort"].toInt();
  n.host = obj["Host"].toString();
}

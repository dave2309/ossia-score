#include "PresenterInterface.hpp"
#include <QApplication>
#include <core/presenter/Presenter.hpp>
#include <core/application/Application.hpp>
#include <iscore/plugins/plugincontrol/PluginControlInterface.hpp>

iscore::SerializableCommand*
iscore::IPresenter::instantiateUndoCommand(
        const CommandParentFactoryKey& parent_name,
        const CommandFactoryKey& name,
        const QByteArray& data)
{
    return Application::instance().presenter()->applicationRegistrar().instantiateUndoCommand(
                parent_name,
                name,
                data);
}



QList<iscore::PanelFactory*>
iscore::IPresenter::panelFactories()
{
    return Application::instance().presenter()->applicationRegistrar().panelFactories();
}



const std::vector<iscore::PluginControlInterface*>& iscore::IPresenter::pluginControls()
{
    return Application::instance().presenter()->applicationRegistrar().pluginControls();
}

#include "DurationSectionWidget.hpp"

#include "Inspector/Constraint/ConstraintInspectorWidget.hpp"

#include "Document/Constraint/ConstraintModel.hpp"

#include "Commands/Constraint/SetMinDuration.hpp"
#include "Commands/Constraint/SetMaxDuration.hpp"
#include "Commands/Scenario/ResizeConstraint.hpp"
#include "Commands/ResizeBaseConstraint.hpp"
#include "Commands/Constraint/SetRigidity.hpp"

#include "core/interface/document/DocumentInterface.hpp"
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>


DurationSectionWidget::DurationSectionWidget(ConstraintInspectorWidget* parent) :
    InspectorSectionWidget {"Durations", parent},
    m_model {parent->model()},
    m_parent {parent},
    m_cmdManager{new OngoingCommandDispatcher{iscore::IDocument::commandQueue(iscore::IDocument::documentFromObject(m_model)),
                                           this}}
{

    QWidget* widg{new QWidget{this}};
    QGridLayout* lay = new QGridLayout{widg};
    lay->setContentsMargins(0, 0, 0 , 0);
    widg->setLayout(lay);

    m_valueLabel = new QLabel{"Default: "};
    auto checkbox = new QCheckBox{"Rigid"};
    auto minSpin = new QSpinBox{};
    auto maxSpin = new QSpinBox{};
    auto valueSpin = new QSpinBox{};

    // TODO these need to be updated when the default duration changes
    connect(m_model, &ConstraintModel::defaultDurationChanged,
            this,	 &DurationSectionWidget::on_defaultDurationChanged);


    minSpin->setMinimum(0);
    minSpin->setMaximum(m_model->defaultDuration().msec());
    maxSpin->setMinimum(m_model->defaultDuration().msec() + 1);
    maxSpin->setMaximum(std::numeric_limits<int>::max());
    valueSpin->setMinimum(std::numeric_limits<int>::min());
    valueSpin->setMaximum(std::numeric_limits<int>::max());

    connect(checkbox, &QCheckBox::toggled,
            [ = ](bool val)
    {
        minSpin->setEnabled(!val);
        maxSpin->setEnabled(!val);
    });

    minSpin->setValue(m_model->minDuration().msec());
    maxSpin->setValue(m_model->maxDuration().msec());

    if(m_model->minDuration() == m_model->maxDuration())
    {
        checkbox->setChecked(true);
    }

    valueSpin->setValue(m_model->defaultDuration().msec());

    lay->addWidget(checkbox, 0, 0);
    lay->addWidget(new QLabel{tr("Min duration") }, 1, 0);
    lay->addWidget(minSpin, 1, 1);
    lay->addWidget(new QLabel{tr("Max duration") }, 2, 0);
    lay->addWidget(maxSpin, 2, 1);

    lay->addWidget(m_valueLabel, 3, 0);
    lay->addWidget(valueSpin, 3, 1);

    connect(minSpin,	SIGNAL(valueChanged(int)),
            this,		SLOT(minDurationSpinboxChanged(int)));
    connect(minSpin,	&QSpinBox::editingFinished,
            [&]() { m_cmdManager->commit(); });
    connect(maxSpin,	SIGNAL(valueChanged(int)),
            this,		SLOT(maxDurationSpinboxChanged(int)));
    connect(maxSpin,	&QSpinBox::editingFinished,
            [&]() { m_cmdManager->commit(); });

    connect(valueSpin,  SIGNAL(valueChanged(int)),
            this,       SLOT(defaultDurationSpinboxChanged(int)));
    connect(valueSpin,  &QSpinBox::editingFinished,
            [&]() { m_cmdManager->commit(); });

    connect(checkbox,	&QCheckBox::toggled,
            this,		&DurationSectionWidget::rigidCheckboxToggled);



    addContent(widg);
}

using namespace Scenario::Command;

void DurationSectionWidget::minDurationSpinboxChanged(int val)
{
    auto cmd = new SetMinDuration(
                   iscore::IDocument::path(m_model),
                   std::chrono::milliseconds {val});

    m_cmdManager->send(cmd);
}

void DurationSectionWidget::maxDurationSpinboxChanged(int val)
{
    auto cmd = new SetMaxDuration(
                   iscore::IDocument::path(m_model),
                   std::chrono::milliseconds {val});

    m_cmdManager->send(cmd);
}

void DurationSectionWidget::defaultDurationSpinboxChanged(int val)
{
    iscore::SerializableCommand* cmd {};

    if(m_model->objectName() != "BaseConstraintModel")
    {
        cmd = new ResizeConstraint(
                  iscore::IDocument::path(m_model),
                  std::chrono::milliseconds {val});
    }
    else
    {
        cmd = new ResizeBaseConstraint(
                  iscore::IDocument::path(m_model),
                  std::chrono::milliseconds {val});
    }

    m_cmdManager->send(cmd);
}

void DurationSectionWidget::rigidCheckboxToggled(bool b)
{
    auto cmd = new SetRigidity(
                   iscore::IDocument::path(m_model),
                   b);

    emit m_parent->commandDispatcher()->send(cmd);
}

void DurationSectionWidget::on_defaultDurationChanged(TimeValue dur)
{
    //	m_valueLabel->setText(QString{"Default: %1 s"}.arg(dur));
}

#pragma once
#include <iscore/tools/IdentifiedObject.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
#include <iscore/selection/Selectable.hpp>
#include "StateMachine/CurvePoint.hpp"


// Gives the data.
class CurveSegmentModel : public IdentifiedObject<CurveSegmentModel>
{
        Q_OBJECT

        friend void Visitor<Writer<DataStream>>::writeTo<CurveSegmentModel>(CurveSegmentModel& ev);
        friend void Visitor<Writer<JSONObject>>::writeTo<CurveSegmentModel>(CurveSegmentModel& ev);
    public:
        Selectable selection;
        CurveSegmentModel(
                const id_type<CurveSegmentModel>& id,
                QObject* parent);

        template<typename Impl>
        CurveSegmentModel(Deserializer<Impl>& vis, QObject* parent) :
            IdentifiedObject<CurveSegmentModel>{vis, parent}
        {
            vis.writeTo(*this);
        }

        virtual CurveSegmentModel* clone(
                const id_type<CurveSegmentModel>& id,
                QObject* parent) const = 0;


        virtual QString name() const = 0;
        virtual void serialize(const VisitorVariant&) const = 0;
        virtual QVector<QPointF> data(int numInterp) const = 0; // Will interpolate


        CurvePoint start() const;
        void setStart(const CurvePoint& pt);

        CurvePoint end() const;
        void setEnd(const CurvePoint& pt);

        const id_type<CurveSegmentModel>& previous() const;
        void setPrevious(const id_type<CurveSegmentModel>& previous);

        const id_type<CurveSegmentModel>& following() const;
        void setFollowing(const id_type<CurveSegmentModel>& following);


    signals:
        void dataChanged();
        void previousChanged();
        void followingChanged();

    protected:
        virtual void on_startChanged() = 0;
        virtual void on_endChanged() = 0;

    private:
        CurvePoint m_start, m_end;
        id_type<CurveSegmentModel> m_previous, m_following;
};

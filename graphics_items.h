#pragma once

#include <QGraphicsRectItem>

#include "enums.h"
#include "JTask.h"

class JGraphicsPortItem;
class JGraphicsTaskItem : public QGraphicsRectItem
{
public:
    explicit JGraphicsTaskItem(JTask* task, QGraphicsItem* parent = nullptr);

    std::vector<JGraphicsPortItem*>& inputPorts() { return _inputPorts; }
    std::vector<JGraphicsPortItem*>& outputPorts() { return _outputPorts; }

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

private:
    void createPortItemsAndResize();

private:
    std::vector<JGraphicsPortItem*> _inputPorts;
    std::vector<JGraphicsPortItem*> _outputPorts;
    JTask* const _task;
};

class JGraphicsFlowItem;
class JGraphicsPortItem : public QGraphicsRectItem
{
public:
    explicit JGraphicsPortItem(const QString& name, PortType type, QGraphicsItem* parent = nullptr);

    const QString& name() const { return _name; }
    PortType portType() const { return _portType; }

    std::vector<JGraphicsFlowItem*>& incomingFlows() { return _incomingFlows; }
    std::vector<JGraphicsFlowItem*>& outgoingFlows() { return _outgoingFlows; }

    QPointF topEdgeCenter() const;
    QPointF bottomEdgeCenter() const;

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    std::vector<JGraphicsFlowItem*> _incomingFlows;
    std::vector<JGraphicsFlowItem*> _outgoingFlows;
    QString _name;
    PortType _portType;
};

class JGraphicsFlowItem : public QGraphicsPathItem
{
public:
    explicit JGraphicsFlowItem(JGraphicsPortItem* from /*Input port*/,
                               JGraphicsPortItem* to /*Output port*/,
                               QGraphicsItem* parent = nullptr);

    void adjustShape();

    enum { Type = UserType + 3 };
    int type() const override { return Type; }

private:
    JGraphicsPortItem* const _from;
    JGraphicsPortItem* const _to;
};

QPainterPath genBezierCurvePath(const QPointF& start, const QPointF& end);

bool isConnectable(const JGraphicsPortItem* port1, const JGraphicsPortItem* port2);

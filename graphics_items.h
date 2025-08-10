#pragma once

#include <QGraphicsRectItem>

#include "JTask.h"

class JGraphicsPortItem;
class JGraphicsTaskItem : public QGraphicsRectItem
{
public:
    explicit JGraphicsTaskItem(JTask* task, QGraphicsItem* parent = nullptr);

    std::vector<JGraphicsPortItem*>& inputPorts() { return _inputPorts; }
    std::vector<JGraphicsPortItem*>& outputPorts() { return _outputPorts; }

private:
    void createPortItemsAndResize();

private:
    std::vector<JGraphicsPortItem*> _inputPorts;
    std::vector<JGraphicsPortItem*> _outputPorts;
    JTask* const _task;
};

enum class PortType;
class JGraphicsFlowItem;
class JGraphicsPortItem : public QGraphicsRectItem
{
public:
    explicit JGraphicsPortItem(const QString& name, PortType type, QGraphicsItem* parent = nullptr);

    std::vector<JGraphicsFlowItem*>& incomingFlows() { return _incomingFlows; }
    std::vector<JGraphicsFlowItem*>& outgoingFlows() { return _outgoingFlows; }

    QPointF topEdgeCenter() const;
    QPointF bottomEdgeCenter() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    std::vector<JGraphicsFlowItem*> _incomingFlows;
    std::vector<JGraphicsFlowItem*> _outgoingFlows;
    QString _name;
    PortType _type;
};

class JGraphicsFlowItem : public QGraphicsPathItem
{
public:
    explicit JGraphicsFlowItem(JGraphicsPortItem* start, JGraphicsPortItem* end,
                               QGraphicsItem* parent = nullptr);

    void adjust();

private:
    JGraphicsPortItem* const _start;
    JGraphicsPortItem* const _end;
};

#include <QDebug>

#include <QGraphicsSceneMouseEvent>
#include <QPointer>

#include "JTask.h"
#include "graphics_items.h"

#include "JGraphicsScene.h"

namespace {
JGraphicsPortItem* toPortItem(QGraphicsItem* const item)
{
    if (!item)
        return nullptr;

    if (qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item))
        return toPortItem(item->parentItem());

    return qgraphicsitem_cast<JGraphicsPortItem*>(item);
}
} // namespace

class JGraphicsScenePrivate
{
    Q_DECLARE_PUBLIC(JGraphicsScene)
public:
    explicit JGraphicsScenePrivate() : _auxFlow(new QGraphicsPathItem)
    {
        _auxFlow->setZValue(-1);
        _auxFlow->setAcceptedMouseButtons(Qt::NoButton);
    };

    ~JGraphicsScenePrivate() { delete _auxFlow; }

    JGraphicsScenePrivate(const JGraphicsScenePrivate&) = delete;
    JGraphicsScenePrivate& operator=(const JGraphicsScenePrivate&) = delete;

    JGraphicsScenePrivate(JGraphicsScenePrivate&&) = delete;
    JGraphicsScenePrivate& operator=(JGraphicsScenePrivate&&) = delete;

    bool isDrawingAuxFlow() const { return _auxFlow->scene() && _start; }

    void startDrawingAuxFlow(JGraphicsPortItem* port)
    {
        if (!port || isDrawingAuxFlow())
            return;

        _start = port;
        q_ptr->addItem(_auxFlow);
    }

    void updateAuxFlowEndPoint(const QPointF& scenePos)
    {
        if (!isDrawingAuxFlow())
            return;

        QPointF startPosOnItem;
        if (_start->portType() == PortType::Input) {
            startPosOnItem = _auxFlow->mapFromItem(_start, _start->topEdgeCenter());
        } else {
            startPosOnItem = _auxFlow->mapFromItem(_start, _start->bottomEdgeCenter());
        }
        _auxFlow->setPath(genBezierCurvePath(startPosOnItem, _auxFlow->mapFromScene(scenePos)));
        _auxFlow->update();
    }

    void stopDrawingAuxFlowAndCreateRealFlow(JGraphicsPortItem* end)
    {
        if (!isDrawingAuxFlow())
            return;

        q_ptr->removeItem(_auxFlow);
        _auxFlow->setPath({});
        if (isConnectable(_start, end)) {
            if (_start->portType() == PortType::Output) {
                std::swap(_start, end);
            }
            q_ptr->addItem(new JGraphicsFlowItem(_start, end));
        }
        _start = nullptr;
    }

private:
    QGraphicsPathItem* const _auxFlow;
    JGraphicsPortItem* _start{nullptr};
    QGraphicsScene* q_ptr{nullptr};
};

JGraphicsScene::JGraphicsScene(QObject* parent)
    : QGraphicsScene(parent), d_ptr(new JGraphicsScenePrivate)
{
    d_ptr->q_ptr = this;
    setSceneRect({0, 0, 4000, 4000});

    addItem(new JGraphicsTaskItem(new Task1(this)));
    addItem(new JGraphicsTaskItem(new Task2(this)));
    addItem(new JGraphicsTaskItem(new Task3(this)));
}

void JGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && !d_ptr->isDrawingAuxFlow()) {
        d_ptr->startDrawingAuxFlow(toPortItem(itemAt(event->scenePos(), {})));
    }
    QGraphicsScene::mousePressEvent(event);
}

void JGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && d_ptr->isDrawingAuxFlow()) {
        d_ptr->updateAuxFlowEndPoint(event->scenePos());
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void JGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && d_ptr->isDrawingAuxFlow()) {
        d_ptr->stopDrawingAuxFlowAndCreateRealFlow(toPortItem(itemAt(event->scenePos(), {})));
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

#include "moc_JGraphicsScene.cpp"

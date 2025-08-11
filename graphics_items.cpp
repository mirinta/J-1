#include <QDebug>
#include <QFont>
#include <QPainter>
#include <QPen>

#include "enums.h"

#include "graphics_items.h"

namespace {
void createTextAndResize(const QString& text, QGraphicsRectItem* const mainBody)
{
    static constexpr qreal kPadding = 4;

    if (!mainBody)
        return;

    auto* const textItem = new QGraphicsSimpleTextItem(text, mainBody);
    textItem->setAcceptedMouseButtons(Qt::NoButton);
    textItem->setFont(QFont{QStringLiteral("Arial"), 10, QFont::Bold});
    const QRectF textRect = textItem->boundingRect();
    QRectF bodyRect = mainBody->boundingRect();
    bodyRect.setWidth(2 * kPadding + std::max(bodyRect.width(), textRect.width()));
    bodyRect.setHeight(2 * kPadding + std::max(bodyRect.height(), textRect.height()));
    mainBody->setRect(bodyRect);
    textItem->setPos((bodyRect.width() - textRect.width()) / 2,
                     (bodyRect.height() - textRect.height()) / 2);
}

qreal createPortItems(std::vector<JGraphicsPortItem*>& itemsVec, const JTaskIoDescVec& descs,
                      PortType type, JGraphicsTaskItem* const mainBody)
{
    static constexpr qreal kHMargin = 4;
    static constexpr qreal kVMargin = 2;

    if (!mainBody)
        return 0;

    qreal totalWidth = 0;
    for (const auto& desc : descs) {
        auto* const item = new JGraphicsPortItem(desc.name(), type, mainBody);
        itemsVec.push_back(item);
        if (type == PortType::Input) {
            item->setPos(totalWidth,
                         -item->boundingRect().height() - kVMargin - mainBody->pen().widthF());
        } else {
            item->setPos(totalWidth,
                         kVMargin + mainBody->boundingRect().height() + mainBody->pen().widthF());
        }
        totalWidth += item->boundingRect().width();
        if (itemsVec.size() < descs.size()) {
            totalWidth += kHMargin;
        }
    }
    return totalWidth;
}
}; // namespace

JGraphicsTaskItem::JGraphicsTaskItem(JTask* const task, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), _task(task)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setBrush(Qt::white);

    setRect(0, 0, 150, 80);
    createTextAndResize(task->desc(), this);
    createPortItemsAndResize();
}

void JGraphicsTaskItem::createPortItemsAndResize()
{
    const qreal topItemsWidth =
        createPortItems(_inputPorts, _task->inputDescVec(), PortType::Input, this);
    const qreal bottomItemsWith =
        createPortItems(_outputPorts, _task->outputDescVec(), PortType::Output, this);
    QRectF resizedRect = rect();
    resizedRect.setWidth(std::max({resizedRect.width(), topItemsWidth, bottomItemsWith}));
    setRect(resizedRect);
}

JGraphicsPortItem::JGraphicsPortItem(const QString& name, PortType type, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), _name(name), _portType(type)
{
    setBrush(Qt::white);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setRect(0, 0, 40, 20);
    createTextAndResize(_name, this);
}

QPointF JGraphicsPortItem::topEdgeCenter() const
{
    return (rect().topLeft() + rect().topRight()) / 2;
}

QPointF JGraphicsPortItem::bottomEdgeCenter() const
{
    return (rect().bottomLeft() + rect().bottomRight()) / 2;
}

QVariant JGraphicsPortItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemScenePositionHasChanged) {
        for (auto* const flow : _incomingFlows) {
            flow->adjustShape();
        }
        for (auto* const flow : _outgoingFlows) {
            flow->adjustShape();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

JGraphicsFlowItem::JGraphicsFlowItem(JGraphicsPortItem* const from, JGraphicsPortItem* const to,
                                     QGraphicsItem* parent)
    : QGraphicsPathItem(parent), _from(from), _to(to)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setZValue(-1);

    from->outgoingFlows().push_back(this);
    to->incomingFlows().push_back(this);
    adjustShape();
}

void JGraphicsFlowItem::adjustShape()
{
    if (!_from || !_to)
        return;

    const QPointF start = mapFromItem(_from, _from->bottomEdgeCenter());
    const QPointF end = mapFromItem(_to, _to->topEdgeCenter());
    setPath(genBezierCurvePath(start, end));
}

QPainterPath genBezierCurvePath(const QPointF& start, const QPointF& end)
{
    const qreal dx = end.x() - start.x();
    QPainterPath path;
    path.moveTo(start);
    QPointF ctrl1(start.x() + dx * 0.5, start.y());
    QPointF ctrl2(end.x() - dx * 0.5, end.y());
    path.cubicTo(ctrl1, ctrl2, end);
    return path;
}

bool isConnectable(const JGraphicsPortItem* const port1, const JGraphicsPortItem* const port2)
{
    if (!port1 || !port2)
        return false;

    if (port1 == port2)
        return false;

    if (port1->portType() == port2->portType())
        return false;

    return port1->name() == port2->name();
}

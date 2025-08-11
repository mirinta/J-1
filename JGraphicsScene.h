#pragma once

#include <memory>

#include <QGraphicsScene>

class JGraphicsScenePrivate;
class JGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit JGraphicsScene(QObject* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Q_DECLARE_PRIVATE(JGraphicsScene)
    std::unique_ptr<JGraphicsScenePrivate> d_ptr;
};

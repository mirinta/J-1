#pragma once

#include <QGraphicsScene>

class JGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit JGraphicsScene(QObject* parent = nullptr);
};

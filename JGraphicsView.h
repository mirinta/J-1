#pragma once

#include <QGraphicsView>

class JGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit JGraphicsView(QWidget* parent = nullptr);
};

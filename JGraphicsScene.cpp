#include <QDebug>

#include "JTask.h"
#include "graphics_items.h"

#include "JGraphicsScene.h"

JGraphicsScene::JGraphicsScene(QObject* parent) : QGraphicsScene(parent)
{
    setSceneRect({0, 0, 4000, 4000});

    auto* const task1 = new JGraphicsTaskItem(new JTask(this));
    auto* const task2 = new JGraphicsTaskItem(new JTask(this));
    addItem(task1);
    addItem(task2);

    auto* const flow = new JGraphicsFlowItem(task1->outputPorts()[0], task2->inputPorts()[1]);
    task1->outputPorts()[0]->outgoingFlows().push_back(flow);
    task2->inputPorts()[1]->incomingFlows().push_back(flow);
    addItem(flow);
}

#include "moc_JGraphicsScene.cpp"

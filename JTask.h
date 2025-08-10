#pragma once

#include <QObject>
#include <QVariant>

#include "JTaskIoDesc.h"

class JTask : public QObject
{
    Q_OBJECT

public:
    explicit JTask(QObject* parent = nullptr) : QObject(parent) {}

    virtual QString desc()
    {
        static int count = 0;
        return QStringLiteral("This is task(%1).").arg(count++);
    }
    virtual JTaskIoDescVec inputDescVec()
    {
        return {JTaskIoDesc{"int"}, JTaskIoDesc{"QString"}, JTaskIoDesc{"Custom type 1"}};
    }
    virtual JTaskIoDescVec outputDescVec()
    {
        return {JTaskIoDesc{"Custom type 2"}, JTaskIoDesc{"Custom type 3"}};
    }

    virtual std::vector<QVariant> run(const std::vector<QVariant>& inputs) { return {}; }
};

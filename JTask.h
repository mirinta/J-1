#pragma once

#include <QObject>
#include <QVariant>

#include "JTaskIoDesc.h"

class JTask : public QObject
{
    Q_OBJECT

public:
    explicit JTask(QObject* parent = nullptr);

    virtual QString desc() = 0;
    virtual JTaskIoDescVec inputDescVec() = 0;
    virtual JTaskIoDescVec outputDescVec() = 0;

    virtual std::vector<QVariant> run(const std::vector<QVariant>& inputs) { return {}; };
};

class Task1 : public JTask
{
    Q_OBJECT

public:
    using JTask::JTask;

    QString desc() override { return QStringLiteral("This is Task1."); }
    JTaskIoDescVec inputDescVec() override { return {}; }
    JTaskIoDescVec outputDescVec() override
    {
        return {JTaskIoDesc{"Custom type 1"}, JTaskIoDesc{"Custom type 2"}};
    }
};

class Task2 : public JTask
{
    Q_OBJECT

public:
    using JTask::JTask;

    QString desc() override { return QStringLiteral("This is Task2."); }
    JTaskIoDescVec inputDescVec() override
    {
        return {JTaskIoDesc{"Custom type 1"}, JTaskIoDesc{"Custom type 2"}};
    }
    JTaskIoDescVec outputDescVec() override
    {
        return {JTaskIoDesc{"Custom type 2"}, JTaskIoDesc{"Custom type 3"}};
    }
};

class Task3 : public JTask
{
    Q_OBJECT

public:
    using JTask::JTask;

    QString desc() override { return QStringLiteral("This is Task3."); }
    JTaskIoDescVec inputDescVec() override
    {
        return {JTaskIoDesc{"Custom type 2"}, JTaskIoDesc{"Custom type 3"}};
    }
    JTaskIoDescVec outputDescVec() override
    {
        return {JTaskIoDesc{"Custom type 1"}, JTaskIoDesc{"Custom type 2"}};
    }
};

#include "dllmaper.h"
#include "ctkLog.h"

#include <QLibrary>
#include <QCoreApplication>


CDllMaper::CDllMaper(const QString &dllname, const QString &functionname)
{
    if (dllname == "NULL" || functionname == "NULL")
    {
        return;
    }
    QString strLibPath = QCoreApplication::applicationDirPath() + "/procpe/sys/";
    QLibrary oLib(strLibPath + dllname);
    if (!oLib.load())
    {
        QSLOG_ERROR("load library:" + dllname + " failure");
        return;
    }

    m_pFunctioner = (DllFunction) oLib.resolve(functionname.toStdString().c_str());
    if (NULL == m_pFunctioner)
    {
        QSLOG_ERROR("load library:" + dllname + " failure, resolve function:" + functionname + " failure");
        return;
    }
}

CDllMaper::~CDllMaper()
{

}

void CDllMaper::trigger(bool)
{
    m_pFunctioner();
}

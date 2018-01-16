#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QTextStream>

#include "pathbuilder.h"

QString CPathBuilder::getPluginManagerPath()
{
    if (makeBuilderOrRunning())
    {
        return qApp->applicationDirPath();
    }
    else
    {
#ifdef _WIN32
        return qApp->applicationDirPath() + "/plugins/ctkPluginManager";
#else
        return qApp->applicationDirPath();
#endif
        // return m_strProgramPath;
    }
}

bool CPathBuilder::makeBuilderOrRunning()
{
    QString startup = QCoreApplication::applicationDirPath();

    QString prefixPath = qApp->applicationDirPath();

    QFile f;
    foreach ( QString path, QStringList() << "" << "/.." << "/bin" << "/../../.." )
    {
        f.setFileName( prefixPath + path + "/projectbuildpath.txt" );
        if ( f.exists() )
        {
            break;
        }
    }

    QString translatorpath;

    // 是编译时
    if ( f.exists() && f.open( QIODevice::ReadOnly ) )
    {
        QTextStream stream(&f);

        m_strProgramPath = stream.readLine();
        m_strBuildTye = stream.readLine();
        return true;
    }
    else
    {
        m_strProgramPath = startup;
        return false;
    }
}

QString CPathBuilder::getPluginsPath()
{
    QString strPluginPath;

    QString prefixPath = QCoreApplication::applicationDirPath();

    if (makeBuilderOrRunning())
    {
        //文件记录路径与startupPath之间，剩余的为Cfg路径
        QString mCfgIntDir;
        if (prefixPath.length() > m_strProgramPath.length())
        {
            mCfgIntDir = prefixPath.mid(m_strProgramPath.length());
        }

        strPluginPath = m_strProgramPath + "plugins/" + mCfgIntDir;
    }
    else
    {
        strPluginPath = prefixPath + "/plugins";
    }
    return strPluginPath;
}

QString CPathBuilder::getAlgorithmPath()
{
    QString strAlgorithmpath;
    if (makeBuilderOrRunning())
    {
        strAlgorithmpath = qApp->applicationDirPath();
    }
    else
    {
        strAlgorithmpath = qApp->applicationDirPath() + "/algorithm";
    }
    return strAlgorithmpath;
}


QString CPathBuilder::getGisPluginsPath()
{
    QString strPluginPath;

    QString prefixPath = QCoreApplication::applicationDirPath();

    if (makeBuilderOrRunning())
    {
        //文件记录路径与startupPath之间，剩余的为Cfg路径
        QString mCfgIntDir;
        if (prefixPath.length() > m_strProgramPath.length())
        {
            mCfgIntDir = prefixPath.mid(m_strProgramPath.length());
        }

        strPluginPath = m_strProgramPath + "gisplugins/" + mCfgIntDir;
    }
    else
    {
        strPluginPath = prefixPath + "/gisplugins";
    }
    return strPluginPath;
}

QString CPathBuilder::getBaseMapsPath()
{
    QString strMapPath;
    strMapPath = qApp->applicationDirPath() + "/maps";
    return strMapPath;
}

QString CPathBuilder::getResourcesPath()
{
    QString strResourcesPath;
    strResourcesPath = qApp->applicationDirPath() + "/resources";
    return strResourcesPath;
}

QString CPathBuilder::getImagePath()
{
    QString strImagePath;
    strImagePath = qApp->applicationDirPath() + "/image";
    return strImagePath;
}

QString CPathBuilder::getCurlFilePath()
{
    QString strFilePath = qApp->applicationDirPath() + "/tmp/download";
    return strFilePath;
}

QString CPathBuilder::getControlPath()
{
    QString strControlPath;

    QString prefixPath = QCoreApplication::applicationDirPath();

    if (makeBuilderOrRunning())
    {
        strControlPath = m_strProgramPath + "procpe/sys/" + m_strBuildTye + "/";
    }
    else
    {
        strControlPath = prefixPath + "/procpe/sys/";
    }
    return strControlPath;
}

QString CPathBuilder::getProcpePath()
{
    QString strProcpePath;

    QString prefixPath = QCoreApplication::applicationDirPath();

    if (makeBuilderOrRunning())
    {
        strProcpePath = m_strProgramPath + "procpe/res/";
    }
    else
    {
        strProcpePath = prefixPath + "/procpe/res/";
    }
    return strProcpePath;
}

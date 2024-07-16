#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H
#include<QString>
#include <QProcess>
#include<QDebug>
#include <QStandardPaths>
#include <QDir>
#include<QDateTime>
#include<QFileInfo>
#include <fstream>
#include <string>
class FileController
{
public:
    FileController();

    bool Save(int n);
    bool Load(int n);

    QString getSaveTime(int n);
    QString getThisTime(int n);

    int getZPoint();
    bool setZPoint(int n);

    QString getHomePath();

private:
    QDir homePath;
    QDir backUpPath;
    QString gstatsPath;
    QString gstats_modPath;
    qint64 zPointPosition = -1;

    QDir initCurrentHomePath();
    QDir initCurrentBackUpPath();
    bool copyFile(const QString &sourceFilePath, const QString &destFilePath);
    bool findZPoint();
};

#endif // FILECONTROLLER_H

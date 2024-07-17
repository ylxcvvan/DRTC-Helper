#include "filecontroller.h"

FileController::FileController() {

    homePath=initCurrentHomePath();
    backUpPath=initCurrentBackUpPath();

    gstatsPath=QString(homePath.absolutePath()+"/gstats.save");
    gstats_modPath=QString(homePath.absolutePath()+"/gstats-mod.save");


}

bool FileController::Save(int n)
{
    QString sourceFilePath=homePath.absolutePath();
    QString destFilePath=backUpPath.absolutePath();

    QString sourceFileSave=sourceFilePath+QString("/%0mod.save").arg(n);
    QString sourceFileSlot=sourceFilePath+QString("/%0mod.slot").arg(n);

    QString destFileSave=destFilePath+QString("/%0mod.save").arg(n);
    QString destFileSlot=destFilePath+QString("/%0mod.slot").arg(n);

    bool res= copyFile(sourceFileSave,destFileSave)&&copyFile(sourceFileSlot,destFileSlot);
    return res;
}

bool FileController::Load(int n)
{
    QString sourceFilePath=backUpPath.absolutePath();
    QString destFilePath=homePath.absolutePath();

    QString sourceFileSave=sourceFilePath+QString("/%0mod.save").arg(n);
    QString sourceFileSlot=sourceFilePath+QString("/%0mod.slot").arg(n);

    QString destFileSave=destFilePath+QString("/%0mod.save").arg(n);
    QString destFileSlot=destFilePath+QString("/%0mod.slot").arg(n);

    bool res= copyFile(sourceFileSave,destFileSave)&&copyFile(sourceFileSlot,destFileSlot);
    return res;
}

QString FileController::getSaveTime(int n)
{
    QString sourceFilePath=backUpPath.absolutePath();
    QString sourceFileSave=sourceFilePath+QString("/%0mod.save").arg(n);

    QFileInfo fileInfo(sourceFileSave);

    if(!fileInfo.exists()||fileInfo.size()/ 1024.0==0)
        return "空";

    return fileInfo.lastModified().toString("最近保存时间：MM-dd HH:mm:ss");
}

QString FileController::getThisTime(int n)
{
    QString sourceFilePath=homePath.absolutePath();
    QString sourceFileSave=sourceFilePath+QString("/%0mod.save").arg(n);

    QFileInfo fileInfo(sourceFileSave);

    if(!fileInfo.exists()||fileInfo.size()/ 1024.0==0)
        return "空";

    return fileInfo.lastModified().toString(" 存档 ");
}

bool FileController::findZPoint() {
    QFile file(gstats_modPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "文件无法打开！";
        return false;
    }

    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        zPointPosition++;
        if (line.contains(" gstats{ ' zombo-point } <to")) {
            file.close(); // 提前关闭文件
            qDebug()<<"zPointPos="<<zPointPosition;
            return true;
        }
        // 更新 zPointPosition，移动到下一行开头
    }

    file.close(); // 在文件末尾关闭文件
    return false;
}


int FileController::getZPoint() {
    if (zPointPosition == -1 && !findZPoint()) {
        return -1;
    }

    QFile file(gstats_modPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "文件无法打开！";
        return -1;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    for (int i=0;i<zPointPosition;i++) {
        line = in.readLine();
    }
    file.close();

    int pos = line.indexOf(" gstats{ ' zombo-point } <to");
    if (pos != -1) {
        qDebug()<<line.left(pos);
        return line.left(pos).toInt();
    }

    return -1;
}

bool FileController::setZPoint(int n) {
    if (zPointPosition == -1 && !findZPoint()) {
        return false;
    }

    QFile file(gstats_modPath);
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "文件无法打开！";
        return false;
    }

    QString content;
    QTextStream in(&file);
    while (!in.atEnd()) {
        content += in.readLine();
        content += "\n"; // Add newline characters that were removed by readLine()
    }
    file.close();

    int pos = content.indexOf(" gstats{ ' zombo-point } <to", static_cast<int>(zPointPosition));
    if (pos != -1) {
        int startPos = content.lastIndexOf("\n", pos) + 1;
        content.replace(startPos, pos - startPos, QString::number(n));
    }

    QFile outFile(gstatsPath);
    QFile outModFile(gstats_modPath);
    if (!outFile.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开文件进行写入！";
        return false;
    }
    outModFile.open(QIODevice::WriteOnly);

    QTextStream out(&outFile);
    out << content;
    QTextStream outmod(&outModFile);
    outmod<<content;

    outFile.close();
    outModFile.close();

    zPointPosition = -1; // Reset the position to force re-find next time

    return true;
}

QString FileController::getHomePath()
{
    return homePath.absolutePath();
}


QDir FileController::initCurrentHomePath()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    dir.cdUp();
    QString path=dir.absolutePath();
    path+="/.madgarden/DR2C";
    return QDir(path);
}

QDir FileController::initCurrentBackUpPath()
{
    QDir dir(homePath);
    if (!dir.exists("backup")) {
        // 创建backup文件夹
        dir.mkpath("backup");
    }

    // 设置为backup文件夹
    dir.cd("backup");
    return dir;

}

bool FileController::copyFile(const QString &sourceFilePath, const QString &destFilePath)
{
    QFile sourceFile(sourceFilePath);
    if (!sourceFile.exists()) {
        qDebug() << "Source file does not exist:" << sourceFilePath;
        return false;
    }

    if (QFile::exists(destFilePath)) {
        qDebug() << "Destination file already exists, removing it:" << destFilePath;
        QFile::remove(destFilePath);
    }

    if (sourceFile.copy(destFilePath)) {
        qDebug() << "File copied successfully from" << sourceFilePath << "to" << destFilePath;
        return true;
    } else {
        qDebug() << "Failed to copy file from" << sourceFilePath << "to" << destFilePath;
        return false;
    }
}



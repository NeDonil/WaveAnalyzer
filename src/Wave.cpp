#include "Wave.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

bool isDigit(QString& value){
    for(auto i : value){
        if(!i.isDigit())
            return false;
    }

    return true;
}

bool Wave::fromFile(const QString& filename){
	QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    QStringList tokens = line.split(" ");

    std::vector<int> values;
    for(auto& i : tokens){
        if(isDigit(i)){
            values.push_back(i.toInt());
        }
        else{
            file.close();
            return false;
        }
    }

    m_Values = values;
    file.close();
    return true;
}



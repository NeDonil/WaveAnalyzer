#include "Wave.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

void Wave::setPeriod(int period){
    m_Period = period;
}

void Wave::fromFile(const QString& filename){
	QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Could not to open file:" + filename;
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    QStringList tokens = line.split(" ");

    std::vector<int> values;
    for(auto& i : tokens){
        values.push_back(i.toInt());
    }

    m_Values = values;
    file.close();
}



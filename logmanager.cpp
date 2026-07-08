#include "logmanager.h"
#include <QDatetime>
#include <QTextEdit>
LogManager::LogManager(QTextEdit *logEdit):m_logEdit(logEdit){}
void LogManager::append(const QString &message){
    if(!m_logEdit){
        return;
    }
    QString time=QDateTime::currentDateTime().toString("HH:mm::ss");
    m_logEdit->append("["+time+"]"+message);
}
void LogManager::clear(){
    if(!m_logEdit){
        return;
    }
    m_logEdit->clear();
}
#ifndef LOGMANAGER_H
#define LOGMANAGER_H
#include <QString>
class QTextEdit;
class LogManager{
public:
    explicit LogManager(QTextEdit *logEdit);
    void append(const QString &message);
    void clear();
private:
    QTextEdit *m_logEdit;
};

#endif // LOGMANAGER_H

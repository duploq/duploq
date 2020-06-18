#ifndef CONSOLEGUI_H
#define CONSOLEGUI_H

#include <QWidget>

namespace Ui {
class ConsoleGUI;
}

class ConsoleGUI : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleGUI(QWidget *parent = nullptr);
    ~ConsoleGUI();

    void clear();
    void append(const QString &text);

private:
    Ui::ConsoleGUI *ui;
};

#endif // CONSOLEGUI_H

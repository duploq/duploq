#ifndef RAWOUTPUTGUI_H
#define RAWOUTPUTGUI_H

#include <QWidget>

namespace Ui {
class RawOutputGUI;
}

class BasicXMLSyntaxHighlighter;


class RawOutputGUI : public QWidget
{
    Q_OBJECT

public:
    explicit RawOutputGUI(QWidget *parent = nullptr);
    ~RawOutputGUI();

    void clear();
    void setContentFrom(const QString &fileResultPath);

private:
    Ui::RawOutputGUI *ui;

    BasicXMLSyntaxHighlighter *m_highlighter;
};

#endif // RAWOUTPUTGUI_H

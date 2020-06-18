#ifndef SIDEBYSIDEOUTPUT_H
#define SIDEBYSIDEOUTPUT_H

class QPlainTextEdit;

#include <QWidget>
#include <QSettings>

#include "ResultProcessor.h"

namespace Ui {
class SideBySideOutput;
}

class SideBySideOutputGUI : public QWidget
{
    Q_OBJECT

public:
    explicit SideBySideOutputGUI(QWidget *parent = nullptr);
    ~SideBySideOutputGUI();

    void clear();
    void setContentFrom(const QString &filePath1, const QString &filePath2, const ResultInfo &results);

	void storeConfig(QSettings &set);
	void restoreConfig(QSettings &set);

Q_SIGNALS:
	void blockShown(int index);

private:
    void showCross(int index);
	void setupBlock(QPlainTextEdit *view, int line, int size);

private Q_SLOTS:
    void on_PrevChunk_clicked();
    void on_NextChunk_clicked();

private:
    Ui::SideBySideOutput *ui;

    bool m_valid = false;
    int m_chunkIndex = 0;

    QList<CrossInfo> m_crosses;
	const ResultInfo *m_results = nullptr;
};

#endif // SIDEBYSIDEOUTPUT_H

#ifndef SETTINGSGUI_H
#define SETTINGSGUI_H

#include <QDialog>

namespace Ui {
class SettingsGUI;
}


class Engine;
class InputProcessor;


class SettingsGUI : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsGUI(QWidget *parent = nullptr);
    ~SettingsGUI();

    bool exec(InputProcessor &input, Engine &engine);

private:
    Ui::SettingsGUI *ui;
};

#endif // SETTINGSGUI_H

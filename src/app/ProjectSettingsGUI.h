#pragma once

#include <QDialog>

namespace Ui {
class ProjectSettingsGUI;
}

class ProjectManager;


class ProjectSettingsGUI : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectSettingsGUI(QWidget *parent = nullptr);
    ~ProjectSettingsGUI();

    bool exec(ProjectManager &manager);

private Q_SLOTS:

private:

    Ui::ProjectSettingsGUI *ui;
};

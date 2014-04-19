#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QList>

class QMovie;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    QString getNumber(const QString &name);
    QStringList pokeLines();
    QString path = ":/Images/sprites/";

    QString dir;

private:
    Ui::MainWidget *ui;

public slots:
    void pokeButtonClicked();
    void changeOpacity(int val);
    void toggleShowOpacity();
    void showTeam();
};

#endif // MAINWIDGET_H

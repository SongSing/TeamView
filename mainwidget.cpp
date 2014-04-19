#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QException>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QRgb>
#include <QPainter>
#include <QSettings>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{

    ui->setupUi(this);

    setWindowFlags(Qt::WindowStaysOnTopHint);

    connect(ui->opacity, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity(int)));

    connect(ui->view, SIGNAL(clicked()), this, SLOT(showTeam()));
    connect(ui->team, SIGNAL(returnPressed()), this, SLOT(showTeam()));

    connect(ui->typeChart, SIGNAL(clicked()), this, SLOT(showTypeChart()));

    connect(ui->showOpacity, SIGNAL(clicked()), this, SLOT(toggleShowOpacity()));

    QList<QPushButton *> buttons = { ui->p1, ui->p2, ui->p3, ui->p4, ui->p5, ui->p6 };

    foreach (QPushButton *b, buttons)
    {
        connect(b, SIGNAL(clicked()), this, SLOT(pokeButtonClicked()));
        b->setProperty("dead", false);
        b->setProperty("path", path + "0.png");
        b->setIcon(QIcon(b->property("path").toString()));
    }

    QSettings s("SongSing", "TeamView");
    ui->opacity->setValue(s.value("opacity", "100").toInt());

    ui->opacity->setVisible(false);
    ui->opacityLabel->setVisible(false);
    ui->typeChart->setVisible(false);
}

MainWidget::~MainWidget()
{
    delete ui;
}

QString MainWidget::getNumber(const QString &name)
{
    QStringList pokes = pokeLines();

    foreach (QString poke, pokes)
    {
        if (poke.split(";")[1].toLower() == name.toLower())
        {
            return poke.split(";")[0];
        }
    }

    return 0;
}

QStringList MainWidget::pokeLines()
{
    QStringList ret;

    QFile file(":/PokeInfo/PokeInfo.txt");

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            ret.append(stream.readLine());
        }
    }

    file.flush();
    file.close();

    return ret;
}

void MainWidget::pokeButtonClicked()
{
    QPushButton *b = dynamic_cast<QPushButton *>(sender());

    if (!b->property("dead").toBool())
    {
        // make the thing greyed out //

        QImage img(b->property("path").toString());

        img = img.alphaChannel();

        QColor foreground = qRgba(0,0,0,150);

        for(int i = 0; i < img.colorCount(); ++i)
        {
            foreground.setAlpha((qGray(img.color(i)) == 0 ? 0 : 150));
            img.setColor(i, foreground.rgba());
        }


        b->setIcon(QIcon(QPixmap::fromImage(img)));
    }
    else
    {
        // bring it back to livings //

        b->setIcon(QIcon(b->property("path").toString()));
    }


    b->setProperty("dead", !b->property("dead").toBool());
}

void MainWidget::changeOpacity(int val)
{
    ui->opacityLabel->setText(QString::number(val));
    this->setWindowOpacity((float)val / 100);

    QSettings s("SongSing", "TeamView");
    s.setValue("opacity", val);
}

void MainWidget::toggleShowOpacity()
{
    ui->opacity->setVisible(!ui->opacity->isVisible());
    ui->opacityLabel->setVisible(!ui->opacityLabel->isVisible());
    ui->typeChart->setVisible(!ui->typeChart->isVisible());
    ui->showOpacity->setText((ui->showOpacity->text() == "▼" ? "▲" : "▼"));
}

void MainWidget::showTeam()
{
    QString team = ui->team->text();
    QStringList pokes;

    if (team.length() > 0)
    {
        if (team.contains("/"))
        {
            while (team.contains("  "))
            {
                team = team.replace("  ", " ");
            }

            team = team.replace(" / ", "/").replace(" /", "/").replace("/ ", "/").trimmed();
            pokes = team.split("/");
        }
        else
        {
            pokes.append(team.trimmed());
        }

        QList<QPushButton *> buttons = { ui->p1, ui->p2, ui->p3, ui->p4, ui->p5, ui->p6 };

        for (int i = 0; i < 6; i++)
        {
            QPushButton *b = buttons[i];

            if (i < pokes.length())
            {
                b->setProperty("path", path + getNumber(pokes[i]) + ".png");
            }
            else
            {
                b->setProperty("path", path + "0.png");
            }

            b->setIcon(QIcon(b->property("path").toString()));
            b->setProperty("dead", false);
        }
    }
}

void MainWidget::showTypeChart()
{
    QLabel *w = new QLabel(this);
    w->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    w->setAttribute(Qt::WA_DeleteOnClose, true);
    w->setGeometry(this->x(), this->y(), 800, 600);
    w->setStyleSheet("border-image:url(':/Images/typeChart.png');");
    w->show();
}

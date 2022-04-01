#include "snackbarwidget.h"
#include "ui_snackbarwidget.h"
#include <QTimer>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QDebug>

SnackBarWidget::SnackBarWidget(const QString &message,
                               int autoHideDuration,
                               Position anchorOrigin,
                               const QString &icon,
                               const QString &closeIcon,
                               const QColor &color,
                               QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnackBarWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowOpacity(0.9);

    ui->messageLabel->setText(message);
    this->adjustSize();

    if(!icon.isEmpty())
        ui->iconLabel->setPixmap(QPixmap(icon));
    if(!closeIcon.isEmpty())
        ui->closeBtn->setIcon(QIcon(closeIcon));

    if(color.name() != "#2E7D32")
    {
        ui->frame->setStyleSheet(QString(R"(
                                         #frame{
                                         background-color: %0;
                                         border: transparent;
                                         border-radius: 5px;
                                         padding: 10px;
                                         }
                                         )").arg(color.name()));
    }

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0);
    shadow->setBlurRadius(10.0);
    ui->frame->setGraphicsEffect(shadow);
//    QTimer::singleShot(autoHideDuration, this, &SnackBarWidget::close);
    connect(ui->closeBtn, &QToolButton::clicked, this, &SnackBarWidget::close);

    int srcX, srcY;
    int targetX, targetY;
    int padding = 10;
    QDesktopWidget *screen = qApp->desktop();
    switch (anchorOrigin) {
    case TOPLEFT:
        srcX = screen->x() - this->width();
        srcY = screen->y() + padding;
        targetX = screen->x() + padding;
        targetY = screen->y() + padding;
        break;
    case TOPCENTER:
        srcX = screen->width()/2 - this->width()/2;
        srcY = screen->y() - this->height();
        targetX = screen->width()/2 - this->width()/2;
        targetY = screen->y() + padding;
        break;
    case TOPRIGHT:
        srcX = screen->width();
        srcY = screen->y() + padding;
        targetX = screen->width() - (this->width() + padding);
        targetY = screen->y() + padding;
        break;
    //Bottom
    case BOTTOMLEFT:
        srcX = screen->x() - this->width();
        srcY = screen->height() - (this->height() + padding);
        targetX = screen->x() + padding;
        targetY = screen->height() - (this->height() + padding);
        break;
    case BOTTOMCENTER:
        srcX = screen->width()/2 - this->width()/2;
        srcY = screen->height();
        targetX = screen->width()/2 - this->width()/2;
        targetY = screen->height() - (this->height() + padding);
        break;
    case BOTTOMRIGHT:
        srcX = screen->width();
        srcY = screen->height() - (this->height() + padding);
        targetX = screen->width() - (this->width() + padding);
        targetY = screen->height() - (this->height() + padding);
        break;
    default:
        break;
    }

    this->show();

    QPropertyAnimation *anim1 = new QPropertyAnimation(this, "geometry");
    anim1->setDuration(300);
    anim1->setEasingCurve(QEasingCurve::InOutBack);
    anim1->setStartValue(QRect(srcX, srcY, this->width(), this->height()));
    anim1->setEndValue(QRect(targetX, targetY, this->width(), this->height()));

    QPropertyAnimation *anim2 = new QPropertyAnimation(this, "geometry");
    anim2->setDuration(300);
    anim2->setEasingCurve(QEasingCurve::InOutBack);
    anim2->setStartValue(QRect(targetX, targetY, this->width(), this->height()));
    anim2->setEndValue(QRect(srcX, srcY, this->width(), this->height()));
    connect(anim2, &QPropertyAnimation::finished, this, &SnackBarWidget::close);

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
    group->addAnimation(anim1);
    group->addAnimation(new QPauseAnimation(autoHideDuration));
    group->addAnimation(anim2);
    group->start();
}

SnackBarWidget::~SnackBarWidget()
{
    delete ui;
}

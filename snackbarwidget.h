#ifndef SNACKBARWIDGET_H
#define SNACKBARWIDGET_H

#include <QWidget>

namespace Ui {
class SnackBarWidget;
}

class SnackBarWidget : public QWidget
{
    Q_OBJECT

public:
    enum Position{
        TOPCENTER,
        TOPRIGHT,
        BOTTOMRIGHT,
        BOTTOMCENTER,
        BOTTOMLEFT,
        TOPLEFT
    };

    Q_ENUM(Position)

    explicit SnackBarWidget(const QString &message,
                            int autoHideDuration = 5000,
                            Position anchorOrigin = TOPRIGHT,
                            const QString &icon = QString(),
                            const QString &closeIcon = QString(),
                            const QColor &color = QColor("#2E7D32"),
                            QWidget *parent = 0);
    ~SnackBarWidget();

private:
    Ui::SnackBarWidget *ui;
};

#endif // SNACKBARWIDGET_H

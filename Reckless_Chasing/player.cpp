#include "player.h"
#define steps 5
#include <QPointF>
#include <iostream>
#include <QPoint>
#include <QCursor>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <math.h>
#include <QWidget>

Player::Player() {}

Player::~Player() {}

void Player::keyPressEvent(QKeyEvent *event) {
    pressedKeys.insert(reinterpret_cast<QKeyEvent*>(event)->key());

    if (pressedKeys.contains(Qt::Key_W) && pressedKeys.contains(Qt::Key_S)) {
        //Do  nothing
    }
    else if (pressedKeys.contains(Qt::Key_W)) {
        QPointF center = get_MousePos();
        fixed_Pos(center, true);
    }
    else if (pressedKeys.contains(Qt::Key_S)) {
        QPointF center = get_MousePos();
        fixed_Pos(center, false);
    }
}

void Player::keyReleaseEvent(QKeyEvent *event) {
    pressedKeys.remove(reinterpret_cast<QKeyEvent*>(event)->key());
}

void Player::fixed_Pos(QPointF center, bool isUp) {
    qreal dy = (center.y() - this->new_y);
    qreal dx = (center.x() - this->new_x);
    dy = (dy / sqrt(pow(dy, 2) + pow(dx, 2)));
    dx = (dx / sqrt(pow(dy, 2) + pow(dx, 2)));
    qreal x, y;

    if (isUp) {
        x = this->new_x + (steps * dx);
        y = this->new_y + (steps * dy);
    }
    else {
        x = this->new_x - (steps * dx);
        y = this->new_y - (steps * dy);
    }

    if (!is_boundary_crossed(x, y, this->initial_pos.x(), this->initial_pos.y())) {
        if (!(((center.x() - this->new_x)*(center.x() - (this->new_x + (steps * dx)))) < 0)) {
            this->new_x = x;
        }
        /*else {
            this->new_x = center.x();
        }*/
        if (!(((center.y() - this->new_y)*(center.y() - (this->new_y + (steps * dy)))) < 0)) {
            this->new_y = y;
        }
        /*else {
            this->new_y = center.y();
        }*/
    }
    else {
        if (x < -(this->initial_pos.x())) {
            this->new_x = (-(this->initial_pos.x()));
        }
        if (x > (window_size.x() - this->initial_pos.x() - (2 * this->radius))) {
            this->new_x = (window_size.x() - this->initial_pos.x() - (2 * this->radius));
        }
        if (y > (window_size.y() - this->initial_pos.y() - (2 * this->radius))) {
            this->new_y = (window_size.y() - this->initial_pos.y() - (2 * this->radius));
        }
        if (y < -(this->initial_pos.y())) {
            this->new_y = (-(this->initial_pos.y()));
        }
    }

    this->players = this->scene()->items();

   /* for (QList<QGraphicsItem*>::iterator itr = this->players.begin(); itr != this->players.end(); itr++) {
        if (static_cast<Player*>(*itr) != this && typeid(static_cast<Player*>(*itr)) == typeid(this)) {
            this->iscolliding(static_cast<Player*>(*itr));
        }

    }*/
}

QPointF Player::get_MousePos() {
    QPointF window_origin = QPointF(this->initial_pos.x(), this->initial_pos.y());
    QGraphicsView *view = this->scene()->views().first();
    QPointF origin = view->mapToGlobal(QPoint(0, 0));
    QPointF mouse_pos = QCursor::pos();
    mouse_pos -= origin;
    mouse_pos -= window_origin;
    QPointF center = mouse_pos - QPointF(this->radius, this->radius);
    return center;
}


bool Player::is_boundary_crossed(double x, double y, double initialx, double initialy) {
    return ((x < -initialx) ||
            (x > (window_size.x() - initialx - (2 * this->radius))) ||
            (y > (window_size.y() - initialy - (2 * this->radius))) ||
            (y < -initialy));
}

bool Player::iscolliding(Player *player) {
    if (QGraphicsItem::collidesWithItem(player)) {

        qDebug() << "Colliding with " << '\n';
        player->hide();
    }

    return false;
}

#include "SUI_shape.h"
#include "SDL_rect.h"
#include "SUI_canvas.h"
#include "SUI_in_debug.h"

static const double math_pi = 4 * atan(1);
namespace sui {

Shape::Shape() = default;
Shape::~Shape() = default;

Point::Point(double x, double y, double z) : x(x), y(y), z(z) {}

void Point::draw_shape(Canvas &canvas) const {
    canvas.draw_point(*this);
}

void Point::fill_shape(Canvas &canvas) const {
    draw_shape(canvas);
}

Rect::Rect(double x1, double y1, double z1, double x2, double y2, double z2)
    : p1{x1, y1, z1}, p2{x2, y2, z2} {}

Rect::Rect(double x1, double y1, double x2, double y2) : Rect{x1, y1, 0, x2, y2, 0} {}

double Rect::get_width() const {
    return p2.x - p1.x;
}

double Rect::get_height() const {
    return p2.y - p1.y;
}

void Rect::draw_shape(Canvas &canvas) const {
    canvas.draw_rect(*this);
}

void Rect::fill_shape(Canvas &canvas) const {
    canvas.fill_rect(*this);
}

Ellipse_arc::Ellipse_arc(const Point &center, double semiX_axis, double semiY_axis, double start_angle, double end_angle)
    : center{center}, semiX_axis{semiX_axis}, semiY_axis{semiY_axis}, start_angle{start_angle}, end_angle{end_angle} {}

void Ellipse_arc::draw_shape(Canvas &canvas) const {
    if (semiX_axis <= 0 || semiY_axis <= 0) {
        ERR(<< "the axis of ellipse is zero!");
        return;
    }

    constexpr int count = 100;
    std::vector<Point> points(count);
    Point temp;

    double delta = std::min(0.001, 0.02 / (semiX_axis + semiY_axis));
    delta = end_angle > start_angle ? delta : -delta;

    double angle = start_angle;
    bool first = true;
    while (fabs(angle - end_angle) >= delta) {
        int i = 1;
        for (i = 1; abs(angle - end_angle) >= delta && i < count; angle += delta, ++i) {
            points[i].x = center.x + semiX_axis * cos(angle);
            points[i].y = center.y - semiY_axis * sin(angle);
        }
        if (first) {
            points[0] = points[1];
            first = false;
        } else {
            points[0] = temp;
        }
        temp = points[i - 1];
        canvas.draw_lines(points);
    }
}


// very very slow
void Ellipse_arc::fill_shape(Canvas &canvas) const {
    double max_length = std::max(semiX_axis, semiY_axis);
    Ellipse_arc temp = *this;

    for (int i = 0; i < max_length; ++i) {
        temp.semiX_axis = semiX_axis * (max_length - i) / max_length;
        temp.semiY_axis = semiY_axis * (max_length - i) / max_length;
        temp.draw_shape(canvas);
    }
}

Round_rect::Round_rect(const Rect &rect, double radius) : rect{rect}, radius{radius} {}

void Round_rect::draw_shape(Canvas &canvas) const {
    double new_radius = std::min({fabs(rect.get_width()) / 2, fabs(rect.get_height()) / 2, radius});
    Point p1 = rect.p1;
    Point p2 = rect.p2;
    if (p1.x > p2.x) {
        std::swap(p1.x, p2.x);
    }
    if (p1.y > p2.y) {
        std::swap(p1.y, p2.y);
    }
    canvas.draw_line(Point{p1.x + new_radius, p1.y, 0}, Point{p2.x - new_radius, p1.y, 0});
    canvas.draw_line(Point{p1.x, p1.y + new_radius, 0}, Point{p1.x, p2.y - new_radius, 0});
    /**
    * @todo maybe you should use the drawlineF? sub 1 is to make the line not be covered
    */
    canvas.draw_line(Point{p1.x + new_radius, p2.y - 0.5, 0}, Point{p2.x - new_radius, p2.y - 0.5, 0});
    canvas.draw_line(Point{p2.x - 0.5, p1.y + new_radius, 0}, Point{p2.x - 0.5, p2.y - new_radius, 0});
    canvas.draw_arc(Point{p1.x + new_radius, p1.y + new_radius, 0}, new_radius, math_pi / 2, math_pi);
    canvas.draw_arc(Point{p2.x - new_radius, p1.y + new_radius, 0}, new_radius, 0, math_pi / 2);
    canvas.draw_arc(Point{p1.x + new_radius, p2.y - new_radius, 0}, new_radius, math_pi, math_pi * 3 / 2);
    canvas.draw_arc(Point{p2.x - new_radius, p2.y - new_radius, 0}, new_radius, math_pi * 3 / 2, math_pi * 2);
}

void Round_rect::fill_shape(Canvas &canvas) const {
    double new_radius = std::min({fabs(rect.get_width()) / 2, fabs(rect.get_height()) / 2, radius});
    Point top_left, bottom_right;
    top_left.x = std::min(rect.p1.x, rect.p2.x) + new_radius;
    top_left.y = std::min(rect.p1.y, rect.p2.y) + new_radius;
    bottom_right.x = std::max(rect.p1.x, rect.p2.x) - new_radius;
    bottom_right.y = std::max(rect.p1.y, rect.p2.y) - new_radius;
    Point start_point, end_point;
    for (int i = 0; i <= fabs(rect.get_height()) / 2; ++i) {
        if (i < new_radius) {
            start_point.x = top_left.x - sqrt(2 * new_radius * i - i * i);
        } else {
            start_point.x = top_left.x - new_radius;
        }
        start_point.y = top_left.y - new_radius + i;
        end_point.y = start_point.y;
        end_point.x = bottom_right.x + top_left.x - start_point.x;
        canvas.draw_line(start_point, end_point);
        start_point.y = bottom_right.y + new_radius - i;
        end_point.y = start_point.y;
        canvas.draw_line(start_point, end_point);
    }
}

}
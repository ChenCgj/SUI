#ifndef SUI_SHAPE_H
#define SUI_SHAPE_H

namespace sui {

class Canvas;

class Shape {
public:
    Shape();
    virtual ~Shape();
    virtual Shape *clone() const = 0;
protected:
    virtual void draw_shape(Canvas &canvas) const = 0;
    virtual void fill_shape(Canvas &canvas) const = 0;
    friend Canvas;
};

class Point : public Shape {
public:
    Point(double x = 0.0, double y = 0.0, double z = 0.0);
    Point *clone() const override;
    double x;
    double y;
    double z;
private:
    void draw_shape(Canvas &canvas) const override;
    void fill_shape(Canvas &canvas) const override;
    friend Canvas;
};

class Rect : public Shape {
public:
    Rect(double x1, double y1, double z1, double x2, double y2, double z2);
    Rect(double x1 = 0.0, double y1 = 0.0, double x2 = 0.0, double y2 = 0.0);
    Rect *clone() const override;
    double get_width() const;
    double get_height() const;
    Point p1, p2;
private:
    void draw_shape(Canvas &canvas) const override;
    void fill_shape(Canvas &canvas) const override;
    friend Canvas;
};

class Ellipse_arc : public Shape {
public:
    Ellipse_arc(const Point &center, double semiX_axis = 1.0, double semiY_axis = 1.0, double start_angle = 0.0, double end_angle = 1.0);
    Ellipse_arc *clone() const override;
    Point center;
    double semiX_axis, semiY_axis;
    double start_angle, end_angle;
private:
    void draw_shape(Canvas &canvas) const override;
    void fill_shape(Canvas &canvas) const override;
    friend Canvas;
};

class Round_rect : public Shape {
public:
    Round_rect(const Rect &rect, double radius = 0);
    Round_rect *clone() const override;
    Rect rect;
    double radius;
private:
    void draw_shape(Canvas &canvas) const override;
    void fill_shape(Canvas &canvas) const override;
    friend Canvas;
};
}
#endif
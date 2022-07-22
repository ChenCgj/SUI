#ifndef SUI_PANE_GRID_H
#define SUI_PANE_GRID_H
#include "SUI_pane_base.h"

namespace sui {
class Grid_pane : public Pane_base {
public:
    Grid_pane(int posX, int posY, int width, int height, int row = 1, int col = 1);
    void add_content(Geometry &node, int row, int col);
    void remove_content(Geometry &node);
    void set_grid(int row, int col);
    int get_row_count() const;
    int get_col_count() const;
    void set_gap(int row_gap, int col_gap);
    int get_row_gap() const;
    int get_col_gap() const;
private:
    int row_count;
    int col_count;
    int row_gap;
    int col_gap;
    std::function<int (const int &)> get_posX_func(int row, int col);
    std::function<int (const int &)> get_posY_func(int row, int col);
    std::function<int (const int &)> get_width_func(int row, int col);
    std::function<int (const int &)> get_height_func(int row, int col);
};

}
#endif
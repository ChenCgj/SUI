#include "SUI_pane_base.h"

namespace sui {

Pane_base::Pane_base(int posX, int posY, int width, int height) :
    Geometry{posX, posY, width, height},
    Element{posX, posY, width, height} {
    
    object_name = "pane base";
    statu = Element_status::pane_normal;
}


}
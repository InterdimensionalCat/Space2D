#pragma once

#ifdef _SFML_ENABLED
#include "SFML\Graphics.hpp"
#endif

#include "AngularType.h"
#include "LinearType.h"

#include "S2DMath.h"

#include "Mat3.h"
#include "Point2.h"
#include "Vec2.h"
#include "Dim2.h"
#include "NormVec2.h"
#include "Rect2.h"
#include "Poly2.h"

namespace Space2D {

    using namespace Space2D::literals;

    using Point2f = Point2<float>;
    using Vec2f = Vec2<float>;
    using Dim2f = Dim2<float>;
    using NormVec2f = NormVec2<float>;
    using Rect2f = Rect2<float>;
    using Poly2f = Poly2<float>;
    using Mat3f = Mat3<float>;

    using Point2p = Point2<Pixels>;
    using Vec2p = Vec2<Pixels>;
    using Dim2p = Dim2<Pixels>;
    using NormVec2p = NormVec2<Pixels>;
    using Rect2p = Rect2<Pixels>;
    using Poly2p = Poly2<Pixels>;
    using Mat3p = Mat3<Pixels>;

    using Point2m = Point2<Meters>;
    using Vec2m = Vec2<Meters>;
    using Dim2m = Dim2<Meters>;
    using NormVec2m = NormVec2<Meters>;
    using Rect2m = Rect2<Meters>;
    using Poly2m = Poly2<Meters>;
    using Mat3m = Mat3<Meters>;
}

//alias for Space2D
namespace s2d {
    using namespace Space2D;
}


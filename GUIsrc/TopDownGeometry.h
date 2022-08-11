#ifndef TOP_DOWN_GEOMETRY_H
#define TOP_DOWN_GEOMETRY_H

namespace topDownGeometry
{

class Point
{
private:
    int x;
    int y;
public:
    Point() : x{0},y{0} {}
    Point(int xValue, int yValue) : x{xValue},y{yValue} {}
    
    int getX() const {return x;}
    int getY() const {return y;}
};

//TopDownRectangle
class Rect
{
private:
    Point topLeft;
    Point topRight;
    Point bottomLeft;
    Point bottomRight;

    int rectWidth;
    int rectHeight;
public:
    Rect() : topLeft{},topRight{},bottomLeft{},bottomRight{}, rectWidth{0},rectHeight{0} {}
    Rect(Point topLeftPoint, int width, int height) : 
                                                    topLeft{topLeftPoint},
                                                    topRight{Point{topLeftPoint.getX()+width,topLeftPoint.getY()}},
                                                    bottomLeft{Point{topLeftPoint.getX(),topLeftPoint.getY()+height}},
                                                    bottomRight{Point{topLeftPoint.getX()+width,topLeftPoint.getY()+height}},
                                                    rectWidth{width},
                                                    rectHeight{height} {}
    Rect(Point topLeftPoint, Point bottomRightPoint): 
                                                    topLeft{topLeftPoint},
                                                    topRight{bottomRightPoint.getX(),topLeftPoint.getY()},
                                                    bottomLeft{topLeftPoint.getX(),bottomRightPoint.getY()},
                                                    bottomRight{bottomRightPoint} ,
                                                    rectWidth{bottomRightPoint.getX()- topLeftPoint.getX()},
                                                    rectHeight{bottomRightPoint.getY()- topLeftPoint.getY()} {}

    bool pointInRect(Point point) 
    {
        if (point.getX() < topLeft.getX() || point.getX() > bottomRight.getX() ||
            point.getY() > bottomRight.getY() || point.getY() < topLeft.getY()) {
            return false;
        }
        return true;
    }

    bool rectInRect(Rect innerRect)
    {
        if (!this->pointInRect(innerRect.topLeft) ||
            !this->pointInRect(innerRect.topRight) ||
            !this->pointInRect(innerRect.bottomLeft) ||
            !this->pointInRect(innerRect.bottomRight) ) {
            return false;
        }
        return true;
    }

    Point getTopLeft() const {return topLeft;}
    Point getBottomLeft() const {return bottomLeft;}
    Point getTopRight() const {return topRight;}
    Point getBottomRight() const {return bottomRight;}
    int getHeight() const {return rectHeight;}
    int getWidth() const {return rectWidth;}
};

}

#endif
#ifndef TOP_DOWN_GEOMETRY_H
#define TOP_DOWN_GEOMETRY_H

#include <iostream>

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

    void movePoint(int xChange, int yChange) {x+=xChange; y+=yChange;}
    friend std::ostream& operator<< (std::ostream& out, const Point& point)
    {
        out << "Point(" << point.x << ", " << point.y << ')';
        return out;
    }
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

    void recalculateDimensions()
    {
        rectWidth = bottomRight.getX()- topLeft.getX();
        rectHeight = bottomRight.getY()- topLeft.getY();
    }

    void moveRectangle(int xValue, int yValue)
    {
        topLeft.movePoint(xValue,yValue);
        topRight.movePoint(xValue,yValue);
        bottomLeft.movePoint(xValue,yValue);
        bottomRight.movePoint(xValue,yValue);
    }

    Point getTopLeft() const {return topLeft;}
    Point getBottomLeft() const {return bottomLeft;}
    Point getTopRight() const {return topRight;}
    Point getBottomRight() const {return bottomRight;}
    int getHeight() const {return rectHeight;}
    int getWidth() const {return rectWidth;}

    Rect transformRect(Rect oldEncompassingRect, Rect newEncompassingRect)
    {
        //Does not apply a transformation if the rectangle is not encompassing
        Rect resultRect{*this};
        if (!oldEncompassingRect.rectInRect(*this)) {
            return resultRect;
        }
        
        double xScalingFactor = static_cast<double>(newEncompassingRect.getWidth())/static_cast<double>(oldEncompassingRect.getWidth());
        double yScalingFactor = static_cast<double>(newEncompassingRect.getHeight())/static_cast<double>(oldEncompassingRect.getHeight());

        //Calculate distances between oldEncompassingRect walls and this Rect's walls
        double leftWallDistance = (this->topLeft.getX()-oldEncompassingRect.topLeft.getX());
        double rightWallDistance = (this->topRight.getX()-oldEncompassingRect.topLeft.getX());
        double topWallDistance = (this->topLeft.getY()-oldEncompassingRect.topLeft.getY());
        double bottomWallDistance = (this->bottomLeft.getY()-oldEncompassingRect.topLeft.getY());

        //Apply transformation
        resultRect.topLeft.movePoint(static_cast<int>(leftWallDistance*xScalingFactor-leftWallDistance),static_cast<int>(topWallDistance*yScalingFactor-topWallDistance));
        resultRect.bottomLeft.movePoint(static_cast<int>(leftWallDistance*xScalingFactor-leftWallDistance),static_cast<int>(bottomWallDistance*yScalingFactor-bottomWallDistance));
        resultRect.topRight.movePoint(static_cast<int>(rightWallDistance*xScalingFactor-rightWallDistance),static_cast<int>(topWallDistance*yScalingFactor-topWallDistance));
        resultRect.bottomRight.movePoint(static_cast<int>(rightWallDistance*xScalingFactor-rightWallDistance),static_cast<int>(bottomWallDistance*yScalingFactor-bottomWallDistance));

        //Apply translations
        resultRect.moveRectangle(newEncompassingRect.topLeft.getX()-oldEncompassingRect.topLeft.getX(),newEncompassingRect.topLeft.getY()-oldEncompassingRect.topLeft.getY());

        resultRect.recalculateDimensions();
        return resultRect;
    }

    friend std::ostream& operator<< (std::ostream& out, const Rect& rect)
    {
        out << "Rect{"<<"TopLeft"<<rect.topLeft<<", "<<"BottomRight"<<rect.bottomRight<<'}';
        return out;
    }

};

}

#endif